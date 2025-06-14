#include "firmware.h"

#define LED_PORT (GPIOC) 
#define LED_PIN  (GPIO13) 

static void rcc_setup(void) {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_24MHZ]);
}

static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_OPENDRAIN, LED_PIN);
}

static void delay_cycles(uint32_t cycles) {
    for (uint32_t i=0; i<cycles; i++) {
        __asm__ ("nop");
    }
}

int main(void) {
    rcc_setup();
    gpio_setup();

    while (1) {
        gpio_toggle(LED_PORT, LED_PIN);
        delay_cycles(24000000 / 4);
    }

    return 0;
}