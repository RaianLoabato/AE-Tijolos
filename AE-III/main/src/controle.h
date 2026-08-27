/*
 * controle.h
 *
 *  Created on: 17 de abr. de 2026
 *      Author: Raian
 */

#ifndef MAIN_SRC_CONTROLE_H_
#define MAIN_SRC_CONTROLE_H_

#include <stdint.h>
#include <sys/types.h>

uint16_t ultimo_ciclo = 0;
uint16_t  ciclo_atual = 0;

void PWM_resistencia(uint16_t periodo_m, float duty);



#endif /* MAIN_SRC_CONTROLE_H_ */
