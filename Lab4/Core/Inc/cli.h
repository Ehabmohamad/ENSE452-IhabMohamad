/*
 * cli.h
 *
 *  Created on: Nov 7, 2024
 *      Author: Abdus
 */

#ifndef INC_CLI_H_
#define INC_CLI_H_


#include "stm32f1xx_hal.h"
#include "main.h"
#include "timer.h"
#include <string.h>
#include <stdio.h>
void cli_init(void);
void cli_process(void);



#endif /* INC_CLI_H_ */
