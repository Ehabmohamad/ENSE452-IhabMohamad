/*
 * ElevatorFunctionality.h
 *
 *  Created on: Nov 24, 2024
 *      Author: Abdus
 */

#ifndef INC_ELEVATORFUNCTIONALITY_H_
#define INC_ELEVATORFUNCTIONALITY_H_


#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "main.h"
#include "ElevatorFunctionality.h"
#include "cmsis_os.h"
#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* Function Prototypes */
void InitializeElevatorSystem(UART_HandleTypeDef *uartHandle);
void CLI_Transmit(const char *message);
int CLI_Receive(char *buffer, uint16_t buffer_size);

/* Enum for elevator states */
typedef enum {
    ELEVATOR_IDLE,
    ELEVATOR_MOVING,
    ELEVATOR_STOPPED,
    ELEVATOR_MAINTENANCE
} ElevatorState;

/* Elevator buttons and commands */
#define CMD_OPEN "open"
#define CMD_CLOSE "close"
#define CMD_EMERGENCY "E"
#define CMD_MAINTENANCE "M"


#endif /* INC_ELEVATORFUNCTIONALITY_H_ */
