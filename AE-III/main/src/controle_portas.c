#include "adc.h"
#include "hal/adc_types.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/dirent.h>
#include <unistd.h>
#include <driver/gpio.h>

#include "controle_portas.h"
#include "definicao_de_pinos.h"



uint8_t ler_fim_de_curso(adc_oneshot_unit_handle_t * adc, uint8_t canal){
	
	int adc_read = 10;
	int med = 0;
	int reads = 6;
	uint8_t input = 0;
	
	for(int i = 0; i < reads; i++){
			adc_oneshot_read(*adc, canal, &adc_read);
			med += adc_read;
	}
			
	med = med/reads;
			
	if(med > 1900){
		input = 0b00000011;
			
	}else if (med > 1500){
		input = 0b00000010;
			
	}else if (med > 600){
		input = 0b00000001;
			
	}else{
		input = 0b00000000;
	}
	
	if(DEBUG){
	   	printf("valor ADC: %d\n", med);
		printf("input: %d\n", input);
	}
	
	return input;
}


uint8_t move_porta(uint8_t direcao, adc_oneshot_unit_handle_t * adc){
	uint8_t ret = 0;
	uint8_t input = ler_fim_de_curso(adc, CANAL_FIM_DE_CURSO);
	
		
	//liga o motor da porta
	switch (direcao) {
		case 1:
			gpio_set_level(PINO_MOTOR_D, 1);
			gpio_set_level(PINO_MOTOR_E, 0);	
		break;
		
		case 2:
			gpio_set_level(PINO_MOTOR_D, 1);
			gpio_set_level(PINO_MOTOR_E, 0);	
		break;
		
		default:
			gpio_set_level(PINO_MOTOR_D, 0);
			gpio_set_level(PINO_MOTOR_E, 0);	
		break;		
	}
		
	//desliga motor na mudaça de fim de curso
	switch (input) {
		case 1:
			if(aux_fim_curso == 2){
				ret = 1;
			}
			aux_fim_curso = 1;
		break;
			
		case 2:
			if(aux_fim_curso == 1){
				ret = 1;
			}
			aux_fim_curso = 2;
		break;
	}
	
	if(DEBUG){
		printf("direcao: %d aux_porta: %d\n",direcao, aux_fim_curso);
    	usleep(100000);
	}
	
	//desliga motores caso tenha terminado
	if(ret == 1){
		gpio_set_level(PINO_MOTOR_D, 0);
		gpio_set_level(PINO_MOTOR_E, 0);
	}
	
	return ret;
}