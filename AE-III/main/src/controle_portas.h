/*
 * controle_portas.h
 *
 *  Created on: 13 de abr. de 2026
 *      Author: PC
 */

#ifndef MAIN_SRC_CONTROLE_PORTAS_H_
#define MAIN_SRC_CONTROLE_PORTAS_H_


#include "stdint.h"
#include <esp_adc/adc_oneshot.h>

#define ABRE 1
#define FECHA 2

uint8_t aux_fim_curso = 0;

uint8_t move_porta(uint8_t direcao, adc_oneshot_unit_handle_t * adc);

uint8_t ler_fim_de_curso(adc_oneshot_unit_handle_t * adc, uint8_t canal);

#endif /* MAIN_SRC_CONTROLE_PORTAS_H_ */
