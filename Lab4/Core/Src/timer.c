#include "timer.h"
#include "main.h"
#include <stdlib.h>  // For rand()
#include <stdio.h>   // For sprintf()

extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart2;  // UART for printing results

// Initialize and start the timer
void timer_init(void) {
    HAL_TIM_Base_Start(&htim4);
}

// Start the timer and return the current counter value
int16_t timer_start(void) {
    return __HAL_TIM_GET_COUNTER(&htim4);
}

// Stop the timer and calculate elapsed ticks since start_time, handling wraparound
uint16_t timer_stop(int16_t start_time) {
    int16_t end_time = __HAL_TIM_GET_COUNTER(&htim4);
    if (end_time >= start_time) {
        return end_time - start_time;
    } else {
        return (0xFFFF - start_time) + end_time + 1;  // Handle wraparound
    }
}

// Helper function to print results
void print_result(const char *operation, uint16_t ticks) {
    char buffer[100];
    sprintf(buffer, "%s: %u ticks (average over 100 trials)\r\n", operation, ticks);
    HAL_UART_Transmit(&huart2, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
}

// Measurement functions for each operation, returning average ticks as uint16_t
uint16_t measure_add_32bit(void) {
    volatile uint32_t a, b, result;
    uint32_t total_ticks = 0;

    for (int i = 0; i < 100; i++) {
        a = rand();
        b = rand();
        int16_t start_time = timer_start();
        result = a + b;
        total_ticks += timer_stop(start_time);
    }

    return total_ticks / 100;
}

uint16_t measure_add_64bit(void) {
    volatile uint64_t a, b, result;
    uint32_t total_ticks = 0;

    for (int i = 0; i < 100; i++) {
        a = ((uint64_t)rand() << 32) | rand();
        b = ((uint64_t)rand() << 32) | rand();
        int16_t start_time = timer_start();
        result = a + b;
        total_ticks += timer_stop(start_time);
    }

    return total_ticks / 100;
}

uint16_t measure_multiply_32bit(void) {
    volatile uint32_t a, b, result;
    uint32_t total_ticks = 0;

    for (int i = 0; i < 100; i++) {
        a = rand();
        b = rand();
        int16_t start_time = timer_start();
        result = a * b;
        total_ticks += timer_stop(start_time);
    }

    return total_ticks / 100;
}

uint16_t measure_multiply_64bit(void) {
    volatile uint64_t a, b, result;
    uint32_t total_ticks = 0;

    for (int i = 0; i < 100; i++) {
        a = ((uint64_t)rand() << 32) | rand();
        b = ((uint64_t)rand() << 32) | rand();
        int16_t start_time = timer_start();
        result = a * b;
        total_ticks += timer_stop(start_time);
    }

    return total_ticks / 100;
}

uint16_t measure_divide_32bit(void) {
    volatile uint32_t a, b, result;
    uint32_t total_ticks = 0;

    for (int i = 0; i < 100; i++) {
        a = rand();
        b = rand() | 1;  // Ensure b is not zero
        int16_t start_time = timer_start();
        result = a / b;
        total_ticks += timer_stop(start_time);
    }

    return total_ticks / 100;
}

uint16_t measure_divide_64bit(void) {
    volatile uint64_t a, b, result;
    uint32_t total_ticks = 0;

    for (int i = 0; i < 100; i++) {
        a = ((uint64_t)rand() << 32) | rand();
        b = ((uint64_t)rand() << 32) | (rand() | 1);  // Ensure b is not zero
        int16_t start_time = timer_start();
        result = a / b;
        total_ticks += timer_stop(start_time);
    }

    return total_ticks / 100;
}

uint16_t measure_copy_8byte(void) {
    struct Small { uint64_t data; } src, dst;
    uint32_t total_ticks = 0;

    for (int i = 0; i < 100; i++) {
        int16_t start_time = timer_start();
        dst = src;
        total_ticks += timer_stop(start_time);
    }

    return total_ticks / 100;
}

uint16_t measure_copy_128byte(void) {
    struct Medium { uint8_t data[128]; } src, dst;
    uint32_t total_ticks = 0;

    for (int i = 0; i < 100; i++) {
        int16_t start_time = timer_start();
        dst = src;
        total_ticks += timer_stop(start_time);
    }

    return total_ticks / 100;
}

uint16_t measure_copy_1024byte(void) {
    struct Large { uint8_t data[1024]; } src, dst;
    uint32_t total_ticks = 0;

    for (int i = 0; i < 100; i++) {
        int16_t start_time = timer_start();
        dst = src;
        total_ticks += timer_stop(start_time);
    }

    return total_ticks / 100;
}

// Main function to measure all operations and print results
void measure_all_operations(void) {
    print_result("\r\n32-bit Addition", measure_add_32bit());
    print_result("64-bit Addition", measure_add_64bit());
    print_result("32-bit Multiplication", measure_multiply_32bit());
    print_result("64-bit Multiplication", measure_multiply_64bit());
    print_result("32-bit Division", measure_divide_32bit());
    print_result("64-bit Division", measure_divide_64bit());
    print_result("8-byte Struct Copy", measure_copy_8byte());
    print_result("128-byte Struct Copy", measure_copy_128byte());
    print_result("1024-byte Struct Copy", measure_copy_1024byte());
}
