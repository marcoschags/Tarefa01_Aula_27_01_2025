#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "TarefaAnimacao.pio.h"

#define NUM_PIXELS 25
#define OUT_PIN 7
#define LED_VERDE 11
#define LED_AZUL 12
#define LED_VERMELHO 13
#define BOTAO_A 5
#define BOTAO_B 6

uint32_t matrix_rgb(double r, double g, double b) {
    unsigned char R = r * 255;
    unsigned char G = g * 255;
    unsigned char B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

void mostrar_numero(int num, PIO pio, uint sm) {
    static double numeros[10][25] = {
        {1, 1, 1, 1, 1,
         1, 0, 0, 0, 1,
         1, 0, 0, 0, 1,
         1, 0, 0, 0, 1,
         1, 1, 1, 1, 1}, // 0
        
        {0, 0, 1, 0, 0,
         0, 1, 1, 0, 0,
         1, 0, 1, 0, 0,
         0, 0, 1, 0, 0,
         1, 1, 1, 1, 1}, // 1
        
        {1, 1, 1, 1, 1,
         0, 0, 0, 0, 1,
         1, 1, 1, 1, 1,
         1, 0, 0, 0, 0,
         1, 1, 1, 1, 1}, // 2
        
        {1, 1, 1, 1, 1,
         0, 0, 0, 0, 1,
         0, 1, 1, 1, 1,
         0, 0, 0, 0, 1,
         1, 1, 1, 1, 1}, // 3
        
        {1, 0, 0, 1, 1,
         1, 0, 0, 1, 1,
         1, 1, 1, 1, 1,
         0, 0, 0, 1, 1,
         0, 0, 0, 1, 1}, // 4
        
        {1, 1, 1, 1, 1,
         1, 0, 0, 0, 0,
         1, 1, 1, 1, 1,
         0, 0, 0, 0, 1,
         1, 1, 1, 1, 1}, // 5
        
        {1, 1, 1, 1, 1,
         1, 0, 0, 0, 0,
         1, 1, 1, 1, 1,
         1, 0, 0, 0, 1,
         1, 1, 1, 1, 1}, // 6
        
        {1, 1, 1, 1, 1,
         0, 0, 0, 0, 1,
         0, 0, 1, 1, 0,
         0, 0, 1, 0, 0,
         0, 1, 0, 0, 0}, // 7
        
        {1, 1, 1, 1, 1,
         1, 0, 0, 0, 1,
         1, 1, 1, 1, 1,
         1, 0, 0, 0, 1,
         1, 1, 1, 1, 1}, // 8
        
        {1, 1, 1, 1, 1,
         1, 0, 0, 0, 1,
         1, 1, 1, 1, 1,
         0, 0, 0, 0, 1,
         1, 1, 1, 1, 1}  // 9
    };
    for (int i = 0; i < NUM_PIXELS; i++) {
        pio_sm_put_blocking(pio, sm, matrix_rgb(numeros[num][24 - i], 0, 0));
    }
}

int main() {
    stdio_init_all();
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);
    
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &TarefaAnimacao_program);
    uint sm = pio_claim_unused_sm(pio, true);
    TarefaAnimacao_program_init(pio, sm, offset, OUT_PIN);
    
    int contador = 0;
    bool estado_led = false;
    
    while (true) {
        estado_led = !estado_led;
        gpio_put(LED_VERMELHO, estado_led);
        
        if (!gpio_get(BOTAO_A)) {
            if (contador < 9) contador++;
            mostrar_numero(contador, pio, sm);
            sleep_ms(200);
        }
        
        if (!gpio_get(BOTAO_B)) {
            if (contador > 0) contador--;
            mostrar_numero(contador, pio, sm);
            sleep_ms(200);
        }
        
        sleep_ms(50);
    }
    return 0;
}
