/*
 * controle.c
 *
 *  Created on: 17 de abr. de 2026
 *      Author: Raian
 */

#include "controle.h"
#include "driver/gpio.h"
#include "temporizador.h"
#include "definicao_de_pinos.h"
#include <stdint.h>

void PWM_resistencia(uint16_t periodo_m, float duty) {
    // Calcula o tempo total em ms desde o início
    uint32_t tempo_total_ms = ((uint32_t)horas * 3600 * 1000) + ((uint32_t)minutos * 60 * 1000) + ((uint32_t)segundos * 1000) + milisegundos;
    
    // Calcula a fase dentro do ciclo atual (0 a periodo_m - 1)
    uint16_t fase = tempo_total_ms % periodo_m;
    
    // Calcula o tempo "on" baseado no duty cycle (em ms)
    uint16_t tempo_on = (periodo_m * duty);
    
    // Aciona o pino: alto se dentro do tempo "on", baixo caso contrário
    if (fase < tempo_on) {
        gpio_set_level(PINO_RESISTENCIA, 1);
    } else {
        gpio_set_level(PINO_RESISTENCIA, 0);
    }
    
    // Opcional: atualiza ciclo_atual para debug (progresso em % do ciclo)
    ciclo_atual = (fase * 100) / periodo_m;
}
