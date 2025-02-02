// Incluir bibliotecas padrão e específicas do RP2040
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "TarefaAnimacao.pio.h"

// Definições de hardware Raspebery Pico W RP2040 "BitDogLab"
#define NUM_PIXELS 25
#define OUT_PIN 7
#define LED_VERDE 11
#define LED_AZUL 12
#define LED_VERMELHO 13
#define BOTAO_A 5
#define BOTAO_B 6

// Variável global para armazenar o contador
volatile int contador = 0;
PIO pio;
uint sm;

// Variáveis para implementar o debouncing
uint32_t last_interrupt_time = 0;  // Armazena o tempo da última interrupção
#define DEBOUNCE_DELAY 200  // Tempo de debounce em milissegundos

// Converte valores de cores RGB para o formato de 32 bits esperado pela matriz de LEDs
uint32_t matrix_rgb(double r, double g, double b) {
    unsigned char R = r * 255;
    unsigned char G = g * 255;
    unsigned char B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

// Exibe um número na matriz de LEDs
void mostrar_numero(int num, PIO pio, uint sm) {
    static double numeros[10][25] = {
        {1, 1, 1, 1, 1,
         1, 0, 0, 0, 1,
         1, 0, 0, 0, 1,
         1, 0, 0, 0, 1,
         1, 1, 1, 1, 1}, //0
        {0, 0, 1, 0, 0,
         0, 1, 1, 0, 0,
         1, 0, 1, 0, 0,
         0, 0, 1, 0, 0,
         1, 1, 1, 1, 1}, //1
        {1, 1, 1, 1, 1,
         0, 0, 0, 0, 1,
         1, 1, 1, 1, 1,
         1, 0, 0, 0, 0,
         1, 1, 1, 1, 1}, //2
        {1, 1, 1, 1, 1,
         0, 0, 0, 0, 1,
         0, 1, 1, 1, 1,
         0, 0, 0, 0, 1,
         1, 1, 1, 1, 1}, //3
        {1, 0, 0, 1, 1,
         1, 0, 0, 1, 1,
         1, 1, 1, 1, 1,
         0, 0, 0, 1, 1,
         0, 0, 0, 1, 1}, //4
        {1, 1, 1, 1, 1,
         1, 0, 0, 0, 0,
         1, 1, 1, 1, 1,
         0, 0, 0, 0, 1,
         1, 1, 1, 1, 1}, //5
        {1, 1, 1, 1, 1,
         1, 0, 0, 0, 0,
         1, 1, 1, 1, 1,
         1, 0, 0, 0, 1,
         1, 1, 1, 1, 1}, //6
        {1, 1, 1, 1, 1,
         0, 0, 0, 0, 1,
         0, 0, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 1, 0, 0, 0}, //7
        {1, 1, 1, 1, 1,
         1, 0, 0, 0, 1,
         1, 1, 1, 1, 1,
         1, 0, 0, 0, 1,
         1, 1, 1, 1, 1}, //8
        {1, 1, 1, 1, 1,
         1, 0, 0, 0, 1,
         1, 1, 1, 1, 1,
         0, 0, 0, 0, 1,
         1, 1, 1, 1, 1} //9
    };
    for (int i = 0; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(pio, sm, matrix_rgb(numeros[num][24 - i], 0, 0));
    }
}

// Função de interrupção para os botões com debouncing
void gpio_callback(uint gpio, uint32_t events) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time()); // Tempo atual em ms
    if (current_time - last_interrupt_time > DEBOUNCE_DELAY) {  // Verifica se o intervalo é maior que o debounce
        if (gpio == BOTAO_A && contador < 9) contador++;
        if (gpio == BOTAO_B && contador > 0) contador--;
        mostrar_numero(contador, pio, sm);
        last_interrupt_time = current_time;  // Atualiza o tempo da última interrupção
    }
}

int main() {
    // Inicializa a comunicação serial
    stdio_init_all();

    // Configuração dos LEDs e botões
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);
    
    // Configuração do PIO para controle da matriz de LEDs
    pio = pio0;
    uint offset = pio_add_program(pio, &TarefaAnimacao_program);
    sm = pio_claim_unused_sm(pio, true);
    TarefaAnimacao_program_init(pio, sm, offset, OUT_PIN);
    
    // Configuração das interrupções para os botões
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    
    // Loop principal alternando o estado do LED vermelho
    while (true) {
        gpio_put(LED_VERMELHO, !gpio_get(LED_VERMELHO));
        sleep_ms(500);
    }
    return 0;
}
