/*
 * ElevatorFunctionality.c
 *
 *  Created on: Nov 24, 2024
 *      Author: Ihab Mohamad
 */

#include "ElevatorFunctionality.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* FreeRTOS Task Handles */
osThreadId FloorSelectionTaskHandle;
osThreadId ElevatorMovementTaskHandle;

/* FreeRTOS Queue Handle */
QueueHandle_t floorQueue;

/* Elevator State Variables */
int currentFloor = 1;   // Current floor (starts at 1)
int targetFloor = 1;    // Target floor (initialize to the same as current floor)
ElevatorState elevatorState = ELEVATOR_IDLE;
int doorOpen = 0;       // 0 means the door is Closed, 1 means the door is Open
int elevatorIdle = 1;   // 1 = Idle, 0 = Stopped for emergency
int elevatorMoving = -1; // 1 = Moving Up, 0 = Moving Down, -1 = Not Moving

/* UART Handle */
extern UART_HandleTypeDef huart2;

/* Task Function Prototypes */
void FloorSelectionTask(void const * argument);
void ElevatorMovementTask(void const * argument);

/* Initialize Elevator System */
void InitializeElevatorSystem(UART_HandleTypeDef *huart2)
{
    /* Create the floor queue */
    floorQueue = xQueueCreate(5, sizeof(int));
    if (floorQueue == NULL) {
        Error_Handler();
    }

    /* Create FreeRTOS tasks */
    osThreadDef(FloorSelectionTaskDef, FloorSelectionTask, osPriorityNormal, 0, 128);
    FloorSelectionTaskHandle = osThreadCreate(osThread(FloorSelectionTaskDef), NULL);

    osThreadDef(ElevatorMovementTaskDef, ElevatorMovementTask, osPriorityAboveNormal, 0, 256);
    ElevatorMovementTaskHandle = osThreadCreate(osThread(ElevatorMovementTaskDef), NULL);

    update_cli_display();
}

/* UART CLI Transmit Function */
void CLI_Transmit(const char *message)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)message, strlen(message), HAL_MAX_DELAY);
}

/* UART CLI Receive Function */
int CLI_Receive(char *buffer, uint16_t buffer_size)
{
    uint16_t index = 0;
    uint8_t rx_char;

    while (index < buffer_size - 1) {
        if (HAL_UART_Receive(&huart2, &rx_char, 1, HAL_MAX_DELAY) == HAL_OK) {
            if (rx_char == '\r' || rx_char == '\n') {
                buffer[index] = '\0'; // Null-terminate the string
                return 1;
            } else {
                buffer[index++] = rx_char;
                HAL_UART_Transmit(&huart2, &rx_char, 1, HAL_MAX_DELAY); // Echo input
            }
        } else {
            return 0; // Error or timeout
        }
    }

    buffer[index] = '\0'; // Null-terminate the string
    return 1;
}

/* Update CLI Display */
void update_cli_display(void)
{
    char status_message[256];
    sprintf(status_message,
        "\033[2J\033[H\r\n\033[1;34m Elevator Status:\033[0m\n"
        "\r\n\033[1;33m Current Floor: \033[0m%d\n"
        "\r\n\033[1;33m Door Status: \033[0m%s\n"
        "\r\n\033[1;33m Elevator State: \033[0m%s\n"
        "\r\n Commands: Please enter open to open the door\n\r Please enter M for maintenance\n",
        currentFloor,
        doorOpen ? "\033[1;32mOpen\033[0m" : "\033[1;31mClosed\033[0m",
        elevatorIdle ? "\033[1;32mIdle\033[0m" : (elevatorMoving == 1 ? "\033[1;32mMoving Up\033[0m" : "\033[1;31mMoving Down\033[0m")
    );

    CLI_Transmit(status_message);
}

/* Prompt Floor Input */
void PromptFloorInput(void)
{
    char command_buffer[100];
    int targetFloor;
    int isValid = 0;

    while (!isValid) {
        CLI_Transmit("\r\n Please enter the floor (1-5): ");

        if (CLI_Receive(command_buffer, sizeof(command_buffer))) {
            char *endPtr;
            targetFloor = strtol(command_buffer, &endPtr, 10);

            if (*endPtr != '\0') {
                CLI_Transmit("\r\n Please enter the floor as a number: 1, 2, 3, 4, or 5.\r\n");
            } else if (targetFloor < 1 || targetFloor > 5) {
                CLI_Transmit("\r\n Invalid floor. There are only 5 floors in this building.\r\n");
            } else {
                if (xQueueSend(floorQueue, &targetFloor, pdMS_TO_TICKS(100)) != pdPASS) {
                    CLI_Transmit("\r\n Failed to queue floor request.\r\n");
                } else {
                    CLI_Transmit("\r\n Floor request queued.\r\n");
                    isValid = 1;
                }
            }
        }
    }
}

/* Floor Selection Task */
void FloorSelectionTask(void const * argument)
{
    char command_buffer[100];

    for (;;) {
        CLI_Transmit("\r\n Enter a command (open/close/E/M): ");

        if (CLI_Receive(command_buffer, sizeof(command_buffer))) {
            if (strcmp(command_buffer, "open") == 0) {
                if (doorOpen) {
                    CLI_Transmit("\r\n The door is already open.\r\n");
                } else {
                    doorOpen = 1;
                    CLI_Transmit("\r\n The door is now open.\r\n");
                    osDelay(3000); // Wait before auto-closing
                    doorOpen = 0;
                    CLI_Transmit("\r\n The door has closed automatically.\r\n");
                    PromptFloorInput();
                }
            } else if (strcmp(command_buffer, "close") == 0) {
                if (!doorOpen) {
                    CLI_Transmit("\r\n The door is already closed.\r\n");
                } else {
                    doorOpen = 0;
                    CLI_Transmit("\r\n The door is now closed.\r\n");
                    PromptFloorInput();
                }
            } else if (strcmp(command_buffer, "E") == 0) {
                elevatorIdle = 0;
                elevatorMoving = 0;
                update_cli_display();
                CLI_Transmit("\r\n Emergency! Elevator stopped.\r\n");
            } else if (strcmp(command_buffer, "M") == 0) {
                elevatorIdle = 0;
                elevatorMoving = 0;
                update_cli_display();
                CLI_Transmit("\r\n Elevator is now in maintenance mode.\r\n");
            } else {
                CLI_Transmit("\r\n Invalid command.\r\n");
            }
        }
    }
}

/* Elevator Movement Task */
void ElevatorMovementTask(void const * argument)
{
    int targetFloor;

    for (;;) {
        if (xQueueReceive(floorQueue, &targetFloor, portMAX_DELAY) == pdPASS) {
            elevatorIdle = 0;
            elevatorMoving = (currentFloor < targetFloor) ? 1 : 0;

            while (currentFloor != targetFloor) {
                osDelay(1000);
                currentFloor += (currentFloor < targetFloor) ? 1 : -1;
                update_cli_display();
            }

            elevatorIdle = 1;
            doorOpen = 1;
            update_cli_display();
            CLI_Transmit("\r\n Arrived at target floor. Door is opening...\r\n");
            osDelay(3000);

            doorOpen = 0;
            update_cli_display();
            CLI_Transmit("\r\n Door is closing...\r\n");
            osDelay(3000);

            // Re-prompt the user to enter a command
            CLI_Transmit("\r\n Enter a command (open/close/E/M): ");
        }
    }
}
