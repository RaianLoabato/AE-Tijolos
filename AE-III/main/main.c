#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/unistd.h>
#include <unistd.h>
#include <driver/gpio.h>
//#include "soc/clk_tree_defs.h"
#include "src/adc.h"
#include "src/controle.h"
//#include "src/controle_portas.h"
#include "src/definicao_de_pinos.h"
#include "src/controle_portas.c"
#include "src/temporizador.c"
#include "src/temporizador.h"
#include "src/adc.c"
#include "src/controle.c"

// ================================================
//  MÁQUINA DE ESTADOS - Forno 
// ================================================


enum Estado {
	DESLIGADO,
	AGUARDANDO_SET_POINT,
	INICIO_DE_PROCESSO,
	CARREGAMENTO,
	FORNO_CARREGADO,
	COZIMENTO,
	FIM_DE_PROCESSO,
	DESCARREGAMENTO,
	FECHAMENTO_DE_PORTAS
};


uint8_t estadoAtual = DESLIGADO;
uint8_t estadoAnterior = 0;
bool a = false;

void app_main(){
	gptimer_handle_t timer_tempo = NULL;
	
	adc_oneshot_unit_handle_t adc;
	
	setup_temporizador(&timer_tempo);
	
	gptimer_start(timer_tempo);
	
	setup_pinos();
	setup_ADC(&adc);

	
	
	uint8_t aux = 0;
	bool liga = 0;
	
	double kp = 0.01654;
	double kd = 0;
	double ti = 6954.1029;
	double T= 1.0;
	double ref = 600.0;
	
	double  u = 0;
	double u_anterior = 0;
	double  e = 0;
	double e_anterior = 0;
	double e_2_anterior = 0;
	
	double alpha = kp*(1.0 + (T/(2.0*ti)));
	double beta = kp * ((T/(2.0*ti))-1.0);
	//float omega = kp * (kd/T);
	
	while(true){
		
		
		
		if(gpio_get_level(PINO_LIGA)){
			liga = 1;
		 	printf("Forno LIGADO");
	 	}
		
		if(aux != segundos){
			//printf("ciclo: %d\n", (uint16_t)ciclo_atual);
			e = ref - printa_ADC(&adc, CANAL_TERMOPAR, -60, u_anterior);;
			u = u_anterior + (alpha * e) + (beta * e_anterior);
			
			
			
			//printf("u: %f u-1: %f e: %f e-1: %f e-2:%f alpha: %f beta: %f\n", u, u_anterior, e, e_anterior, e_2_anterior, alpha, beta);
			
			u_anterior = u;
			e_2_anterior = e_anterior;
			e_anterior = e;
			
			if(u > 1.0){
				u =1.0;
			}
			
			if(u < 0.1){
				u = 0.0;
			}
			
		}
		aux = segundos;
		
		if(liga){
			PWM_resistencia(10000, u);
		}
		
		/*switch (estadoAtual) {
	
		    case DESLIGADO:
		    	// Máquina Desligada
			    // Condições de saída:
			    // - Botão Ligar pressionado
				printf("%d",gpio_get_level(PINO_LIGA) );
			    if (gpio_get_level(PINO_LIGA) == 1) {
			    	estadoAtual = AGUARDANDO_SET_POINT;
			    }
		    break;
		
		    case AGUARDANDO_SET_POINT:
			    int setpoint_temperatura = 0;
				scanf("digite a temperatura desejada: %d", &setpoint_temperatura);
			    if (setpoint_temperatura >= 0){
			    // Estado 3 - Máquina Ligada
			    	estadoAtual = INICIO_DE_PROCESSO;
			    }
		    break;
		
		    case INICIO_DE_PROCESSO:
				//abre a porta e retorna 1 quando termina
			    if (move_porta(ABRE, &adc)) {
			    	estadoAtual = CARREGAMENTO;
			    }
			break;
		
		    case CARREGAMENTO:
			    //liga motores do carrinho
			   	if (true) {
			    	estadoAtual = FORNO_CARREGADO;
			    }
		    break;
		
		    case FORNO_CARREGADO:
			    // Forno Carregado
			    if (move_porta(FECHA,&adc)) {
			    	estadoAtual = COZIMENTO;
			    }
			break;
		
		    case COZIMENTO:
			    // Liga a Temperatura e Processo de cozimento
			    // Ações: Ligar controle de temperatura + geração de gráficos
				uint16_t leitura = 0;
				
				if(aux != segundos){
					leitura = printa_ADC(&adc, CANAL_TERMOPAR);
				}
				aux = segundos;
				
			    if (leitura > 500) {
			    	estadoAtual = FIM_DE_PROCESSO;
			    }
		    break;
		
		    case FIM_DE_PROCESSO:
			    // Descarregamento
			    // Ações: Desligar resistências, parar gráficos, abrir portas
			    if (move_porta(ABRE,&adc)) {
			    	estadoAtual = DESCARREGAMENTO;
			    }
		    break;
		
		    case DESCARREGAMENTO:
			    // Ações: Aciona carro de saída
				if (true) {
			    	estadoAtual = FECHAMENTO_DE_PORTAS;
			    }
		    break;
		
			case FECHAMENTO_DE_PORTAS:
			    // Fechamento de porta
			    // Ações: Fechar a porta
			    if (move_porta(FECHA,&adc)) {
			        // Volta para o início ou estado de espera
			    	estadoAtual = AGUARDANDO_SET_POINT;   // ou ESTADO_2_AGUARDANDO_SET_POINT
			    }
			break;
		}

		//printa estado atual
		if(estadoAtual != estadoAnterior){
			printf("estado atual:");
			switch (estadoAtual) {
				
				case DESLIGADO:
					printf("DESLIGADO\n");
				break;
				
				case AGUARDANDO_SET_POINT:
					printf("AGUARDANDO_SET_POINT\n");
				break;
				
				case INICIO_DE_PROCESSO:
					printf("INICIO_DE_PROCESSO\n");
				break;
				
				case CARREGAMENTO:
					printf("CARREGAMENTO\n");
				break;
				
				case FORNO_CARREGADO:
					printf("FORNO_CARREGADO\n");
				break;
				
				case COZIMENTO:
					printf("COZIMENTO\n");
				break;
				
				case FIM_DE_PROCESSO:
					printf("FIM_DE_PROCESSO\n");
				break;
				
				case DESCARREGAMENTO:
					printf("DESCARREGAMENTO\n");
				break;
				
				case FECHAMENTO_DE_PORTAS:
					printf("FECHAMENTO_DE_PORTAS\n");
				break;
			}
		}
		estadoAnterior = estadoAtual;*/
		
		usleep(10000);
	}
}
