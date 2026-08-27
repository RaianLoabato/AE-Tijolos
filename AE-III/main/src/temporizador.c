#include "temporizador.h"
#include <stdio.h>

void setup_temporizador(gptimer_handle_t * timer_tempo){
	gptimer_config_t timer_tempo_config = {
	    .clk_src = GPTIMER_CLK_SRC_DEFAULT, // Fonte de clock
	    .direction = GPTIMER_COUNT_UP,      // Direção de contagem
	    .resolution_hz = FREQUENCIA_TIMER,   // Resolução de contagem em Hz
	};
		
	// Create a timer instance
	gptimer_new_timer(&timer_tempo_config, timer_tempo);

	gptimer_alarm_config_t alarm_config = {
	    .reload_count = 0,      
	    .alarm_count = CONTAGEM_TIMER, 
	    .flags.auto_reload_on_alarm = true 
	};
	// Set the timer's alarm action
	gptimer_set_alarm_action(*timer_tempo, &alarm_config);

	gptimer_event_callbacks_t cbs = {
	    .on_alarm = conta_tempo
	};

	gptimer_register_event_callbacks(*timer_tempo, &cbs, NULL);

	gptimer_enable(*timer_tempo);
}

static bool conta_tempo(struct gptimer_t *, const gptimer_alarm_event_data_t *, void *){
	milisegundos += 10;
	
	if(milisegundos >= 1000){
		milisegundos = 0;
		segundos++;
		
		if (segundos >= 60){
			segundos = 0;
			minutos ++;
			
			if (minutos >= 60) {
				minutos = 0;
				horas++;
			}
		}
	}
	
	return false;
}

void printa_tempo(){
	printf("horas: %d minutos: %d segundos %d milisegundos: %d\n", horas, minutos, segundos, milisegundos);
}