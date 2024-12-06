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
	ELEVATOR_IDLE,         // Elevator is idle
	ELEVATOR_MAINTENANCE,  // Elevator is in maintenance mode
	ELEVATOR_MOVING_UP,    // Elevator is moving up
	ELEVATOR_MOVING_DOWN,  // Elevator is moving down
	ELEVATOR_EMERGENCY     // Elevator is in emergency stop
} ElevatorState;

/* Elevator buttons and commands */
#define CMD_OPEN "open"
#define CMD_CLOSE "close"
#define CMD_EMERGENCY "E"
#define CMD_MAINTENANCE "M"


#endif /* INC_ELEVATORFUNCTIONALITY_H_ */
