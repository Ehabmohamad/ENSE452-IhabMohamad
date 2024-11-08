/*
 * cli.c
 *
 *  Created on: Oct 3, 2024
 *      Author: IhabMohamad
 */


#include "cli.h"
#include "main.h"
#include "timer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>  // For generating random numbers

#define MAX_INPUT_LENGTH 100

extern UART_HandleTypeDef huart2;
static char rx_buffer[MAX_INPUT_LENGTH];
static uint8_t rx_index = 0;

// Function prototypes
static void cli_prompt(void);
static void cli_execute_command(const char *command);

// Initialize the CLI
void cli_init(void) {
	const char *welcome_msg = "\r\nHello! Please enter the command you wish the STM32 to perform,\r\n"
	                              "or enter 'help' to see the available commands.\r\n";
	HAL_UART_Transmit(&huart2, (uint8_t *)welcome_msg, strlen(welcome_msg), HAL_MAX_DELAY);
    cli_prompt();
}

// Process user input from UART
void cli_process(void) {
    uint8_t rx_data;
    if (HAL_UART_Receive(&huart2, &rx_data, 1, HAL_MAX_DELAY) == HAL_OK) {
        if (rx_data == '\r' || rx_data == '\n') {
            rx_buffer[rx_index] = '\0';
            cli_execute_command(rx_buffer);
            rx_index = 0;
            cli_prompt();
        } else if (rx_data == 0x7F || rx_data == '\b') {
            if (rx_index > 0) {
                rx_index--;
                HAL_UART_Transmit(&huart2, (uint8_t *)"\b \b", 3, HAL_MAX_DELAY);
            }
        } else {
            if (rx_index < MAX_INPUT_LENGTH - 1) {
                rx_buffer[rx_index++] = rx_data;
                HAL_UART_Transmit(&huart2, &rx_data, 1, HAL_MAX_DELAY);
            }
        }
    }
}

// Display the prompt
static void cli_prompt(void) {
    const char *prompt = "\r\nSTM32> ";
    HAL_UART_Transmit(&huart2, (uint8_t *)prompt, strlen(prompt), HAL_MAX_DELAY);
}


// Command handler
static void cli_execute_command(const char *command) {
    if (strcmp(command, "led on") == 0) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        const char *msg = "\r\nLED is ON\r\n";
        HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
    } else if (strcmp(command, "led off") == 0) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
        const char *msg = "\r\nLED is OFF\r\n";
        HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
    } else if (strcmp(command, "led status") == 0) {
        GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
        if (pinState == GPIO_PIN_SET) {
            const char *msg = "\r\nLED is ON\r\n";
            HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
        } else {
            const char *msg = "\r\nLED is OFF\r\n";
            HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
        }
    } else if (strcmp(command, "measure all") == 0) {
    	measure_all_operations();// Call the function to measure and display results
    } else if (strcmp(command, "help") == 0) {
        const char *help_msg = "\r\nAvailable commands:\r\n"
                               "led on      - Turn on the LED\r\n"
                               "led off     - Turn off the LED\r\n"
                               "led status  - Query the LED state\r\n"
                               "help        - Display this help message\r\n"
        					   "measure all - Measure and print the average time the processor takes for operations";
        HAL_UART_Transmit(&huart2, (uint8_t *)help_msg, strlen(help_msg), HAL_MAX_DELAY);
    } else if (strlen(command) == 0) {
        // Do nothing for empty input
    } else {
        const char *error_msg = "\r\nUnknown command\r\n";
        HAL_UART_Transmit(&huart2, (uint8_t *)error_msg, strlen(error_msg), HAL_MAX_DELAY);
    }
}


