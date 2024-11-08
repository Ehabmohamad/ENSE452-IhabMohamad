/*
 * timer.h
 *
 *  Created on: Nov 7, 2024
 *      Author: Ihab Mohamad
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_


#include "stm32f1xx_hal.h"
#include "main.h"
#include "cli.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

// Timer control functions
void timer_init(void);              // Initialize and start the timer
int16_t timer_start(void);          // Capture the current timer count
uint16_t timer_stop(int16_t start_time); // Calculate elapsed time with wraparound handling

// Function to print the result of each measurement operation
void print_result(const char *operation, uint16_t ticks);

// Measurement functions for various operations, each returning average ticks
uint16_t measure_add_32bit(void);
uint16_t measure_add_64bit(void);
uint16_t measure_multiply_32bit(void);
uint16_t measure_multiply_64bit(void);
uint16_t measure_divide_32bit(void);
uint16_t measure_divide_64bit(void);
uint16_t measure_copy_8byte(void);
uint16_t measure_copy_128byte(void);
uint16_t measure_copy_1024byte(void);

// Main function to measure all operations and print results
void measure_all_operations(void);

#endif /* INC_TIMER_H_ */
