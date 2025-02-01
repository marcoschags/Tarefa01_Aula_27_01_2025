#include <stdio.h>
#include "pico/stdlib.h"

// Bibliotecas para a Matriz de LED
#include <math.h> // Funções matemáticas
#include "hardware/pio.h" // Permite o uso do PIO
#include "hardware/clocks.h" // Clocks do microcontrolador
#include "hardware/adc.h" // Controla o ADC (Conversor Analógico-Digital)
#include "pico/bootrom.h" // Bootloader e recuperação do firmware do Raspberry Pi Pico

// Definição de portas Pico W BitDogLab
#define LED_GREEN 11
#define LED_BLUE 12
#define LED_RED 13  // Correção do nome da variável
#define BUTTON_A 5
#define BUTTON_B 6  // Correção do nome duplicado

int main() {
    // Inicializa a GPIO do LED como saída
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);

    while (1) {
        gpio_put(LED_RED, 1); // Liga o LED
        sleep_ms(100); // Aguarda 100ms
        gpio_put(LED_RED, 0); // Desliga o LED
        sleep_ms(100); // Aguarda 100ms
    }
}
