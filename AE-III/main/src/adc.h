/*
 * adc.h
 *
 *  Created on: 16 de abr. de 2026
 *      Author: PC
 */

#ifndef MAIN_SRC_ADC_H_
#define MAIN_SRC_ADC_H_

#include <esp_adc/adc_oneshot.h>

#define CANAL_FIM_DE_CURSO 8 //gpio26
#define CANAL_TERMOPAR 9 //gpio25

void setup_ADC(adc_oneshot_unit_handle_t * adc);

float  printa_ADC(adc_oneshot_unit_handle_t * adc, uint8_t canal, float offset, float esforco);
 
#define MAX_MEDIA 5

typedef struct {
    float temp_medias[MAX_MEDIA];
    uint16_t adc_medias[MAX_MEDIA];

    uint8_t count;   // quantas médias já existem (máx 20)
    uint8_t index;   // índice circular
} MediaBuffer;

#endif /* MAIN_SRC_ADC_H_ */
