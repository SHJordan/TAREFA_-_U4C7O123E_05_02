/*
 * servo_pwm.c - Controle de servomotor por PWM usando Raspberry Pi Pico W com Pico SDK v2.1.0
 *
 * Funcionalidades:
 * 1) Configura a GPIO22 com PWM a ~50Hz (20ms de período).
 * 2) Envia pulso de 2400µs para posicionar o servo aproximadamente em 180° e aguarda 5s.
 * 3) Envia pulso de 1470µs para posicionar o servo aproximadamente em 90° e aguarda 5s.
 * 4) Envia pulso de 500µs para posicionar o servo aproximadamente em 0° e aguarda 5s.
 * 5) Realiza movimentação suave entre 0° e 180° com incremento de 5µs a cada 10ms.
 * 6) Configura a GPIO12 para controlar um LED RGB via PWM, aplicando um efeito de fade.
 *
 * No experimento (BitDogLab) observou-se que o LED apresentou variações suaves de brilho
 * sincronizadas com o movimento do servo, evidenciando a aplicação do PWM também para controle
 * de luminosidade.
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Definições para o servomotor (GPIO22)
#define SERVO_GPIO 22
#define SERVO_PWM_DIV 125.0f // Divisor para obter 1MHz (125e6/125 = 1e6), ou seja, 1µs por tick
#define SERVO_PWM_WRAP 19999 // Período de 20ms (20000 ticks, pois 0..19999)

// Pulsos (em µs) para as posições do servo
#define SERVO_PULSE_MAX 2400 // Aproximadamente 180°
#define SERVO_PULSE_MID 1470 // Aproximadamente 90°
#define SERVO_PULSE_MIN 500  // Aproximadamente 0°

#define SERVO_STEP 5      // Incremento de 5µs para movimento suave
#define SERVO_DELAY_MS 10 // Atraso de 10ms entre incrementos

// Definições para o LED RGB (GPIO12)
#define LED_GPIO 12
#define LED_PWM_WRAP 255   // Resolução de 8 bits para brilho
#define LED_PWM_DIV 488.0f // Aproximadamente 1kHz de frequência para PWM do LED

int main()
{
    // Inicializa a stdio (para eventuais mensagens de debug)
    stdio_init_all();

    // --- Configuração do PWM para o Servomotor ---
    gpio_set_function(SERVO_GPIO, GPIO_FUNC_PWM);
    uint servo_slice = pwm_gpio_to_slice_num(SERVO_GPIO);
    uint servo_channel = pwm_gpio_to_channel(SERVO_GPIO);
    pwm_set_wrap(servo_slice, SERVO_PWM_WRAP);
    pwm_set_clkdiv(servo_slice, SERVO_PWM_DIV);
    pwm_set_chan_level(servo_slice, servo_channel, 0);
    pwm_set_enabled(servo_slice, true);

    // --- Configuração do PWM para o LED RGB ---
    gpio_set_function(LED_GPIO, GPIO_FUNC_PWM);
    uint led_slice = pwm_gpio_to_slice_num(LED_GPIO);
    uint led_channel = pwm_gpio_to_channel(LED_GPIO);
    pwm_set_wrap(led_slice, LED_PWM_WRAP);
    pwm_set_clkdiv(led_slice, LED_PWM_DIV);
    pwm_set_chan_level(led_slice, led_channel, 0); // Inicia com LED apagado
    pwm_set_enabled(led_slice, true);

    // --- Etapas Iniciais de Posicionamento do Servo ---
    // 1) Posição de 180° (2400µs)
    pwm_set_chan_level(servo_slice, servo_channel, SERVO_PULSE_MAX);
    printf("Posição: 180° (pulse: %d µs)\n", SERVO_PULSE_MAX);
    sleep_ms(5000);

    // 2) Posição de 90° (1470µs)
    pwm_set_chan_level(servo_slice, servo_channel, SERVO_PULSE_MID);
    printf("Posição: 90° (pulse: %d µs)\n", SERVO_PULSE_MID);
    sleep_ms(5000);

    // 3) Posição de 0° (500µs)
    pwm_set_chan_level(servo_slice, servo_channel, SERVO_PULSE_MIN);
    printf("Posição: 0° (pulse: %d µs)\n", SERVO_PULSE_MIN);
    sleep_ms(5000);

    // Variáveis para o efeito de fade do LED
    int led_brightness = 0;
    int led_direction = 1; // 1 para aumentar, -1 para diminuir

    // --- Movimento Suave do Servo (0° a 180° e retorno) ---
    while (true)
    {
        // Movimento de 0° a 180°
        for (int pulse = SERVO_PULSE_MIN; pulse <= SERVO_PULSE_MAX; pulse += SERVO_STEP)
        {
            pwm_set_chan_level(servo_slice, servo_channel, pulse);

            // Atualiza o brilho do LED para efeito fade
            led_brightness += led_direction;
            if (led_brightness >= LED_PWM_WRAP)
            {
                led_brightness = LED_PWM_WRAP;
                led_direction = -1;
            }
            else if (led_brightness <= 0)
            {
                led_brightness = 0;
                led_direction = 1;
            }
            pwm_set_chan_level(led_slice, led_channel, led_brightness);
            sleep_ms(SERVO_DELAY_MS);
        }
        // Movimento de 180° a 0°
        for (int pulse = SERVO_PULSE_MAX; pulse >= SERVO_PULSE_MIN; pulse -= SERVO_STEP)
        {
            pwm_set_chan_level(servo_slice, servo_channel, pulse);

            // Atualiza o brilho do LED para efeito fade
            led_brightness += led_direction;
            if (led_brightness >= LED_PWM_WRAP)
            {
                led_brightness = LED_PWM_WRAP;
                led_direction = -1;
            }
            else if (led_brightness <= 0)
            {
                led_brightness = 0;
                led_direction = 1;
            }
            pwm_set_chan_level(led_slice, led_channel, led_brightness);
            sleep_ms(SERVO_DELAY_MS);
        }
    }

    return 0;
}
