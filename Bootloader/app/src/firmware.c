#include "common-defines.h"
#include "core/system.h"
#include "core/timer.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/scb.h>

#define BOOTLOADER_SIZE (0x6000)

#define LED_PORT (GPIOA) 
#define LED_PIN  (GPIO1) 


static void vector_setup(void) {
    SCB_VTOR = BOOTLOADER_SIZE;
}

static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOA);   // For PA1
    rcc_periph_clock_enable(RCC_AFIO);    // Needed for alternate functions
    gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, LED_PIN);
}


static void delay_ms(uint64_t millis) {
    uint64_t now = system_get_ticks();
    while (system_get_ticks() < now + millis);
}


int main(void) {
    vector_setup();
    system_setup();
    timer_setup();
    gpio_setup();

    float duty_cycle = 0.0;
    timer_set_pwm_duty_cycle(duty_cycle);

    while (1) {
        for (float i=0; i<100; i++) {
            timer_set_pwm_duty_cycle(duty_cycle);
            duty_cycle += 1.0;
            delay_ms(10);
        }
        for (float i=0; i<100; i++) {
            timer_set_pwm_duty_cycle(duty_cycle);
            duty_cycle -= 1.0;
            delay_ms(10);
        }
    }

    return 0;
}