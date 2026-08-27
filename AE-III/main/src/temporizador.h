/*
 * temporizador.h
 *
 *  Created on: 16 de abr. de 2026
 *      Author: PC
 */

#ifndef MAIN_SRC_TEMPORIZADOR_H_
#define MAIN_SRC_TEMPORIZADOR_H_

#include <stdint.h>
#include <stdbool.h>
#include "driver/gptimer.h"

#define FREQUENCIA_TIMER 1000000
#define CONTAGEM_TIMER 10000

uint16_t milisegundos = 0;
uint8_t segundos = 0;
uint8_t minutos = 0;
uint16_t horas = 0;

void setup_temporizador(gptimer_handle_t * timer_tempo);

static bool conta_tempo(struct gptimer_t *, const gptimer_alarm_event_data_t *, void *);

void printa_tempo();

#endif /* MAIN_SRC_TEMPORIZADOR_H_ */
