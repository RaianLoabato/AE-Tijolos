/*
 * definicao_de_pinos.h
 *
 *  Created on: 13 de abr. de 2026
 *      Author: Gabriel
 */

#ifndef MAIN_SRC_DEFINICAO_DE_PINOS_H_
#define MAIN_SRC_DEFINICAO_DE_PINOS_H_

#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "soc/gpio_num.h"

#define DEBUG 1

#define PINO_LIGA GPIO_NUM_2
#define PINO_INICIAR GPIO_NUM_14
#define PINO_MOTOR_E GPIO_NUM_4
#define PINO_MOTOR_D GPIO_NUM_16
#define PINO_RESISTENCIA GPIO_NUM_5

void setup_pinos(){
	gpio_set_direction(PINO_LIGA, GPIO_MODE_INPUT);
	gpio_set_direction(PINO_INICIAR, GPIO_MODE_INPUT);
	gpio_set_direction(PINO_MOTOR_D, GPIO_MODE_OUTPUT);
	gpio_set_direction(PINO_MOTOR_E, GPIO_MODE_OUTPUT);
	gpio_set_direction(PINO_RESISTENCIA, GPIO_MODE_OUTPUT);
	
	gpio_set_pull_mode(PINO_LIGA, GPIO_PULLUP_ENABLE);
}

#endif /* MAIN_SRC_DEFINICAO_DE_PINOS_H_ */
