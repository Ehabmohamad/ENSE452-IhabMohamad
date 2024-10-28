#include "cli.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

#define MAX_INPUT_LENGTH 100

extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim2; // Assuming TIM2 is used for periodic status updates

static char rx_buffer[MAX_INPUT_LENGTH];
static uint8_t rx_index = 0;
static uint8_t tx_complete = 1; // Flag to check if transmission is complete
static uint8_t rx_data = 0; // Buffer for receiving 1 byte
static uint8_t show_prompt = 1; // Flag to control when the prompt should be displayed


// Function prototypes
static void cli_prompt(void);
static void cli_execute_command(const char *command);

// Initialize the CLI
void cli_init(void) {

    const char *welcome_msg = "\033[2J\033[H"
    						  "\r\nHello! Please enter the command you wish the STM32 to perform,\r\n"
                              "or enter 'help' to see the available commands.\r\n"
    						  "\r\n\033[10;0H"
    						  "\033[10;r"
    						  "\r\n\033[10;0H";
    // Start receiving data in interrupt mode
    HAL_UART_Receive_IT(&huart2, &rx_data, 1);

    HAL_UART_Transmit_IT(&huart2, (uint8_t *)welcome_msg, strlen(welcome_msg));

    // Start the timer for periodic updates
    HAL_TIM_Base_Start_IT(&htim2);

    cli_prompt();  // Display the prompt after the welcome message
}

// Display the CLI prompt
static void cli_prompt(void) {
    const char *prompt = "\r\nSTM32> ";
    HAL_UART_Transmit_IT(&huart2, (uint8_t *)prompt, strlen(prompt));
}

// Command handler
static void cli_execute_command(const char *command) {

    if (strcmp(command, "led on") == 0) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        const char *msg = "\r\nLED is ON\r\n\r\n";
        HAL_UART_Transmit_IT(&huart2, (uint8_t *)msg, strlen(msg));
    } else if (strcmp(command, "led off") == 0) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
        const char *msg = "\r\nLED is OFF\r\n\r\n";
        HAL_UART_Transmit_IT(&huart2, (uint8_t *)msg, strlen(msg));
    } else if (strcmp(command, "led status") == 0) {
        GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
        if (pinState == GPIO_PIN_SET) {
            const char *msg = "\r\nLED is ON\r\n\r\n";
            HAL_UART_Transmit_IT(&huart2, (uint8_t *)msg, strlen(msg));
        } else {
            const char *msg = "\r\nLED is OFF\r\n";
            HAL_UART_Transmit_IT(&huart2, (uint8_t *)msg, strlen(msg));
        }
    } else if (strcmp(command, "help") == 0) {
        const char *help_msg = "\r\nAvailable commands:\r\n"
                               "led on     - Turn on the LED\r\n"
                               "led off    - Turn off the LED\r\n"
                               "led status - Query the LED state\r\n"
                               "help       - Display this help message\r\n\r\n";
        HAL_UART_Transmit_IT(&huart2, (uint8_t *)help_msg, strlen(help_msg));
    } else if (strlen(command) == 0) {
        // Do nothing for empty input
    } else {
        const char *error_msg = "\r\nUnknown command\r\n\r\n";
        HAL_UART_Transmit_IT(&huart2, (uint8_t *)error_msg, strlen(error_msg));
    }

    show_prompt = 1;
}

// UART receive complete callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        if (rx_data == '\r' || rx_data == '\n') {
            rx_buffer[rx_index] = '\0'; // Null-terminate the string
            cli_execute_command(rx_buffer); // Execute command
            rx_index = 0; // Reset the index
        } else if (rx_data == 0x7F || rx_data == '\b') {
            // Handle backspace
            if (rx_index > 0) {
                rx_index--;
                HAL_UART_Transmit_IT(&huart2, (uint8_t *)"\b \b", 3);
            }
        } else {
            // Store received character
            if (rx_index < MAX_INPUT_LENGTH - 1) {
                rx_buffer[rx_index++] = rx_data;
                HAL_UART_Transmit_IT(&huart2, &rx_data, 1); // Echo the character
            }
        }
        // Continue receiving next character
        HAL_UART_Receive_IT(&huart2, &rx_data, 1);
    }
}

// UART transmit complete callback
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        tx_complete = 1; // Set flag indicating transmission is complete

        if (show_prompt) {
            show_prompt = 0;  // Reset the flag to avoid repeated prompts
            cli_prompt();
            }
    }
}
