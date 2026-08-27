/*
 * adc.c
 *
 *  Created on: 16 de abr. de 2026
 *      Author: PC
 */
#include "adc.h"
#include "esp_adc/adc_oneshot.h"
#include <stdint.h>
#include <stdio.h>
#include <sys/_types.h>
#include "temporizador.h"

static MediaBuffer buffer = {
    .count = 0,
    .index = 0
};

static void atualiza_media(MediaBuffer *buf, uint16_t adc_bruto, float temp_bruto,
                          uint16_t *adc_saida, float *temp_saida)
{
    float soma_temp = temp_bruto;
    uint32_t soma_adc = adc_bruto;

    // Soma das médias anteriores
    for (uint8_t i = 0; i < buf->count; i++) {
        soma_temp += buf->temp_medias[i];
        soma_adc  += buf->adc_medias[i];
    }

    uint8_t divisor = buf->count + 1;

    float nova_temp = soma_temp / divisor;
    uint16_t novo_adc = soma_adc / divisor; // mantém inteiro

    // Armazena no buffer circular
    buf->temp_medias[buf->index] = nova_temp;
    buf->adc_medias[buf->index]  = novo_adc;

    buf->index = (buf->index + 1) % MAX_MEDIA;

    if (buf->count < MAX_MEDIA) {
        buf->count++;
    }

    *temp_saida = nova_temp;
    *adc_saida  = novo_adc;
}

 void setup_ADC(adc_oneshot_unit_handle_t * adc){
 	
 	adc_oneshot_unit_init_cfg_t init_config1 = {
 		.unit_id = ADC_UNIT_2,
 		.ulp_mode = ADC_ULP_MODE_DISABLE
 	};
 	
 	adc_oneshot_new_unit(&init_config1, adc);
 	
 	adc_oneshot_chan_cfg_t config = {		
 		.bitwidth = ADC_BITWIDTH_DEFAULT,
 		.atten = ADC_ATTEN_DB_0
 	};
 	
 	adc_oneshot_config_channel(*adc, CANAL_FIM_DE_CURSO, &config);
	adc_oneshot_config_channel(*adc, CANAL_TERMOPAR, &config);
 }

float printa_ADC(adc_oneshot_unit_handle_t * adc, uint8_t canal, float offset, float esforco){
    int adc_leitura;
    adc_oneshot_read(*adc, canal, &adc_leitura);
    
    uint16_t tempo = (horas * 60 * 60) + (minutos * 60) + segundos;
    
	// Temperatura BRUTA
    //float temp_bruta = (adc_leitura * 715.0) / 2096.0;
	if(adc_leitura < 1489) adc_leitura = 1489;
	float escala_ajuste = 4095.0 * (adc_leitura - 1489) / 2606.0; // escala o valor para 0-4095
	float temp_bruta = ((escala_ajuste) * 715.0) / 2096.0; // |converte o valor de 0-4095 até 0-650
    temp_bruta += offset;
    // ===================== APLICA MÉDIA =====================
    /*uint16_t adc_filtrado;
    float temp_filtrada;

    atualiza_media(&buffer, (uint16_t)escala_ajuste, temp_bruta,
                   &adc_filtrado, &temp_filtrada);*/

    // ===================== PRINT =====================
    // Imprime: tempo, ADC bruto, temp bruta, ADC filtrado, temp filtrada
    printf("%d,%d,%.2f,%f \n",
           tempo,
           adc_leitura,
           temp_bruta,
		   esforco);
		   

    return temp_bruta;
}