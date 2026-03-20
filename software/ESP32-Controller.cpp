

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "driver/dac.h"
#include "driver/ledc.h"

static const char *TAG = "ESP32_IO";

// === PINOS ===

#define R1_IN1_GPIO          (gpio_num_t)36   // ADC1_CH6  (R1-IN1)
#define R1_IN2_GPIO          (gpio_num_t)39   // ADC1_CH7  (R1-IN2)

#define DAC_2 (gpio_num_t)25
#define DAC_2 (gpio_num_t)26 

#define SPI_CLK (gpio_num_t)14
#define SPI_MISO (gpio_num_t)12
#define SPI_MOSI (gpio_num_t)13

#define GND (gpio_num_t)14 
#define CINCOV (gpio_num_t)15

#define SCL (gpio_num_t)22
#define SDA (gpio_num_t)21

#define MOTOR1_SAIDA1_GPIO   (gpio_num_t)19   // MOTOR1-SAIDA1
#define MOTOR1_SAIDA2_GPIO   (gpio_num_t)18   // MOTOR1-SAIDA2
#define RESISTENCIA_SAIDA_GPIO (gpio_num_t)5 // RESISTENCIA-SAIDA3

#define UART_TX (gpio_num_t)24 
#define UART_RX (gpio_num_t)25

#define MOTOR2_SAIDA1_GPIO   (gpio_num_t)2   // MOTOR2-SAIDA1
#define MOTOR2_SAIDA2_GPIO   (gpio_num_t)15   // MOTOR2-SAIDA2

#define GND (gpio_num_t)29
#define CINCOVSAIDA (gpio_num_t)30

void setup(){
    ESP_LOGI(TAG, "=== ESP32 ESP-IDF + FreeRTOS iniciado (baseado na imagem) ===");

    // ================== CONFIGURAÇÃO DAS SAÍDAS DIGITAIS ==================
    gpio_set_direction(MOTOR1_SAIDA1_GPIO,   GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR1_SAIDA2_GPIO,   GPIO_MODE_OUTPUT);
    gpio_set_direction(RESISTENCIA_SAIDA_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR2_SAIDA1_GPIO,   GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR2_SAIDA2_GPIO,   GPIO_MODE_OUTPUT);

    // Estado inicial seguro (tudo desligado)
    gpio_set_level(MOTOR1_SAIDA1_GPIO,   0);
    gpio_set_level(MOTOR1_SAIDA2_GPIO,   0);
    gpio_set_level(RESISTENCIA_SAIDA_GPIO, 0);
    gpio_set_level(MOTOR2_SAIDA1_GPIO,   0);
    gpio_set_level(MOTOR2_SAIDA2_GPIO,   0);

    // ================== CONFIGURAÇÃO ADC 12 BITS (datasheet seção 4.2) ==================
    adc1_config_width(ADC_WIDTH_BIT_12);                    // Resolução 12 bits (0-4095)
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11); // GPIO34 – faixa ~0-3,3V
    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11); // GPIO35 – faixa ~0-3,3V

}
//Testes Digitais
static const struct {
    gpio_num_t pin;
    const char* nome;
} saidas[] = {
    {SPI_CLK,       "SPI_CLK"},
    {SCL,           "SCL"},
    {SDA,           "SDA"},
    {MOTOR1_SAIDA1_GPIO, "M1 A1"},
    {MOTOR1_SAIDA2_GPIO, "M1 A2"},
    {RESISTENCIA_SAIDA_GPIO, "Resist."},
    {UART_TX,       "UART_TX"},
    {UART_RX,       "UART_RX"},
    {MOTOR2_SAIDA1_GPIO, "M2 A1"},
    {MOTOR2_SAIDA2_GPIO, "M2 A2"},
    {SPI_MISO,      "SPI_MISO"},
    {SPI_MOSI,      "SPI_MOSI"},
};

void TesteDigitais(void)
{
    esp_err_t err = ESP_OK;

    while (err == ESP_OK)
    {
        for (size_t i = 0; i < sizeof(saidas)/sizeof(saidas[0]) && err == ESP_OK; i++)
        {
            err = gpio_set_level(saidas[i].pin, 1);
            if (err == ESP_OK) vTaskDelay(pdMS_TO_TICKS(100));

            if (err == ESP_OK) {
                err = gpio_set_level(saidas[i].pin, 0);
                if (err == ESP_OK) vTaskDelay(pdMS_TO_TICKS(100));
            }
        }

        if (err == ESP_OK) {
            vTaskDelay(pdMS_TO_TICKS(1000));  // pausa entre ciclos
        }
    }

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Erro detectado no teste digital");
    }
}
// ESTE DAC (RAMPA 0 → 255 em exatamente 2 segundos) 

void TestePWM(void)
{
    ESP_LOGI("TESTE_PWM", "Iniciando teste PWM rampa duty (pino %d)", MOTOR1_SAIDA1_GPIO);

    // Configuração do timer PWM
    ledc_timer_config_t timer_config = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .duty_resolution  = LEDC_TIMER_10_BIT,
        .timer_num        = LEDC_TIMER_0,
        .freq_hz          = 5000,
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer_config));

    // Canal PWM – ordem correta dos campos (igual ao header ledc.h)
    ledc_channel_config_t channel_config = {
        .gpio_num       = MOTOR1_SAIDA1_GPIO,
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER_0,
        .duty           = 0,
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&channel_config));

    // Rampa de duty 0 → 1023 em ~2 segundos
    const int passos = 1024;
    const int delay_por_passo = 2000 / passos;  // ≈ 1.95 ms por passo

    for (int duty = 0; duty <= 1023; duty++) {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        vTaskDelay(pdMS_TO_TICKS(delay_por_passo));
    }

    // Desliga no final
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

    ESP_LOGI("TESTE_PWM", "Rampa PWM finalizada");
}

void loop(){
    TestePWM();
    TesteDigitais();
}
