/*
 * ElevatorFunctionality.c
 *
 * Created on: Nov 24, 2024
 * Author: Ihab Mohamad
 */

#include "ElevatorFunctionality.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* FreeRTOS Task Handles */
osThreadId FloorSelectionTaskHandle;
osThreadId ElevatorMovementTaskHandle;
osThreadId ButtonPollingTaskHandle;


/* FreeRTOS Queue Handle */
QueueHandle_t floorQueue;

/* Elevator State Variables */
int currentFloor = 1;   // Current floor (starts at 1)
int targetFloor = 1;    // Target floor (initialize to the same as current floor)
ElevatorState elevatorState = ELEVATOR_IDLE;
int doorOpen = 0;       // 0 means the door is Closed, 1 means the door is Open
int Open = 0;
int elevatorIdle = 1;   // 1 = Idle, 0 = Not Idle
int elevatorMoving = -1; // 1 = Moving Up, 0 = Moving Down, -1 = Not Moving
volatile int emergencyActivated = 0; // 0 = Normal, 1 = Emergency
volatile int updateDisplayFlag = 0;

/* UART Handle */
extern UART_HandleTypeDef huart2;

/* Task Function Prototypes */
void FloorSelectionTask(void const * argument);
void ElevatorMovementTask(void const * argument);
void EmergencyButtonPollingTask(void const * argument);
const char* get_elevator_state_description(ElevatorState state);

/* Initialize Elevator System */
void InitializeElevatorSystem(UART_HandleTypeDef *huart2)
{
    /* Create the floor queue */
    floorQueue = xQueueCreate(10, sizeof(int));
    if (floorQueue == NULL) {
        Error_Handler();
    }

    /* Create FreeRTOS tasks */
    osThreadDef(FloorSelectionTaskDef, FloorSelectionTask, osPriorityNormal, 0, 128);
    FloorSelectionTaskHandle = osThreadCreate(osThread(FloorSelectionTaskDef), NULL);

    osThreadDef(ElevatorMovementTaskDef, ElevatorMovementTask, osPriorityAboveNormal, 0, 256);
    ElevatorMovementTaskHandle = osThreadCreate(osThread(ElevatorMovementTaskDef), NULL);

    osThreadDef(ButtonPollingTaskDef, EmergencyButtonPollingTask, osPriorityNormal, 0, 128);
    ButtonPollingTaskHandle = osThreadCreate(osThread(ButtonPollingTaskDef), NULL);



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
    char status_message[512];
    sprintf(status_message,
        "\033[2J\033[H\r\n\033[1;34m Elevator Status:\033[0m\n"
        "\r\n\033[1;33m Current Floor: \033[0m%d\n"
        "\r\n\033[1;33m Door Status: \033[0m%s\n"
        "\r\n\033[1;33m Elevator State: \033[0m%s\n"
        "\r\n Commands: Please enter open to open the door\n\r To call the elevator to different floor Please enter the floor number \n\r Please enter M for maintenance\n",
        currentFloor,
        (doorOpen ? "\033[1;32mOpen\033[0m" : "\033[1;31mClosed\033[0m"),
        emergencyActivated ? "\033[1;31mEmergency Stop\033[0m"
                           : (elevatorState == ELEVATOR_MAINTENANCE
                              ? "\033[1;31mOut of Order (Maintenance)\033[0m"
                              : (elevatorIdle ? "\033[1;32mIdle\033[0m"
                              : (elevatorMoving == 1 ? "\033[1;32mMoving Up\033[0m" : "\033[1;31mMoving Down\033[0m")))
    );

    CLI_Transmit(status_message);
}

void update_cli_display1(void)
{
    char status_message[256];

    CLI_Transmit("\033[2J\033[H"); // Clear screen and reset cursor
    CLI_Transmit("\r\n\033[1;34m Elevator Status:\033[0m\n");

    sprintf(status_message, "\r\n\033[1;33m Current Floor: \033[0m%d\n", currentFloor);
    CLI_Transmit(status_message);

    CLI_Transmit("\r\n\033[1;33m Door Status: \033[0m");
    CLI_Transmit(doorOpen ? "\033[1;32mOpen\033[0m\n" : "\033[1;31mClosed\033[0m\n");

    CLI_Transmit("\r\n\033[1;33m Elevator State: \033[0m");
    CLI_Transmit(get_elevator_state_description(elevatorState));
    CLI_Transmit("\n");

    CLI_Transmit("\r\n Commands: Please enter open to open the door\n");
    CLI_Transmit("\r To call the elevator to different floor Please enter the floor number\n");
    CLI_Transmit("\r Please enter M for maintenance\n");
}

/* Elevator State Description Helper */
const char* get_elevator_state_description(ElevatorState state)
{
    switch (state) {
        case ELEVATOR_IDLE:
            return "\033[1;32mIdle\033[0m";
        case ELEVATOR_MAINTENANCE:
            return "\033[1;31mOut of Order (Maintenance)\033[0m";
        case ELEVATOR_MOVING_UP:
            return "\033[1;32mMoving Up\033[0m";
        case ELEVATOR_MOVING_DOWN:
            return "\033[1;31mMoving Down\033[0m";
        case ELEVATOR_EMERGENCY:
            return "\033[1;31mEmergency Stop\033[0m";
        default:
            return "\033[1;33mUnknown State\033[0m";
    }
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
                    //CLI_Transmit("\r\n Failed to queue floor request.\r\n");
                } else {
                    CLI_Transmit("\r\n Floor request queued.\r\n");
                    isValid = 1;
                }
            }
        }
    }
}



/* Emergency Button Polling Task */
void EmergencyButtonPollingTask(void const * argument)
{
    int lastButtonState = 1; // Assume button starts unpressed (pulled up)

    for (;;) {
        int currentButtonState = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);

        // Detect button press (falling edge)
        if (lastButtonState == 1 && currentButtonState == 0) {
            emergencyActivated = !emergencyActivated; // Toggle emergency state

            if (emergencyActivated) {
                CLI_Transmit("\r\n Emergency Stop Activated! Elevator halted.\r\n");
            } else {
                CLI_Transmit("\r\n Emergency Stop Deactivated. Elevator is now idle.\r\n");
            }

            update_cli_display(); // Update the CLI display
        }

        lastButtonState = currentButtonState;

        osDelay(100); // Poll every 100ms
    }
}




/* Floor Selection Task */
void FloorSelectionTask(void const * argument)
{
    char command_buffer[100];

    for (;;) {
    	CLI_Transmit("\r\n Enter a command: ");

    	        if (CLI_Receive(command_buffer, sizeof(command_buffer))) {
    	            char *endPtr;
    	            int floor = strtol(command_buffer, &endPtr, 10);

    	            if (*endPtr == '\0' && floor >= 1 && floor <= 5) {
    	                /* User entered a valid floor number */
    	                if (xQueueSend(floorQueue, &floor, pdMS_TO_TICKS(100)) != pdPASS) {
    	                    CLI_Transmit("\r\n Failed to queue call request.\r\n");
    	                } else {
    	                    CLI_Transmit("\r\n Elevator called to floor ");
    	                    char floor_message[10];
    	                    sprintf(floor_message, "%d.\r\n", floor);
    	                    CLI_Transmit(floor_message);
    	                }
    	            } else if (strcmp(command_buffer, "open") == 0) {
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
    	            } else if (strcmp(command_buffer, "M") == 0) {
    	                elevatorState = ELEVATOR_MAINTENANCE;
    	                update_cli_display1();

    	                CLI_Transmit("\r\n enter 'exit' when done with the maintenance : ");

    	                    if (CLI_Receive(command_buffer, sizeof(command_buffer))) {
    	                        if (strcmp(command_buffer, "exit") == 0) {
    	                            CLI_Transmit("\r\n Exiting maintenance mode. Returning to normal operation.\r\n");
    	                            elevatorState = ELEVATOR_IDLE;
    	                            update_cli_display1();
    	                        } else {
    	                            CLI_Transmit("\r\n Invalid maintenance command.\r\n");
    	                        }
    	                    }

    	            }else {
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
        // Wait for a floor request
        if (xQueueReceive(floorQueue, &targetFloor, portMAX_DELAY) == pdPASS) {
            elevatorIdle = 0;
            elevatorMoving = (currentFloor < targetFloor) ? 1 : 0;

            while (currentFloor != targetFloor) {
                // Check if emergency is activated
                if (emergencyActivated == 1 ) {

                    // Stop movement
                    elevatorMoving = -1; // Stop movement
                    elevatorIdle = 0;


                    // Update display and wait for emergency to be deactivated

                    while (emergencyActivated==1) {
                        osDelay(500);
                    }

                    elevatorIdle = 1;
                    update_cli_display();
                    break; // Exit the movement loop


                }

                // Normal movement logic
				osDelay(1000);
				currentFloor += (currentFloor < targetFloor) ? 1 : -1;
				update_cli_display();
            }

            // Open the door at the target floor if no emergency is activated
            if (emergencyActivated == 0) {
            	//xQueueReset(floorQueue); // Clear all pending requests
                elevatorIdle = 1;
                doorOpen = 1;
                update_cli_display();
                CLI_Transmit("\r\n Arrived at floor ");
                char floor_message[10];
                sprintf(floor_message, "%d.\r\n", currentFloor);
                CLI_Transmit(floor_message);
                osDelay(3000); // Keep the door open for 3 seconds

                // Close the door
                doorOpen = 0;
                update_cli_display();
                CLI_Transmit("\r\n Door is closing...\r\n");
                osDelay(3000);

                // Re-prompt the user to enter a command
                CLI_Transmit("\r\n Enter a command (open/close/E/M): ");
            }
        }
    }
}

