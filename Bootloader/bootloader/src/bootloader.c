#include "common-defines.h"
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/memorymap.h>

#define BOOTLOADER_SIZE   (0x6000)
#define APP_START_ADDRESS (FLASH_BASE + BOOTLOADER_SIZE)

#define LED_PORT (GPIOC) 
#define LED_PIN  (GPIO13) 

static void rcc_setup(void) {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_24MHZ]);
}

static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_OPENDRAIN, LED_PIN);
}

static void delay_cycles(uint32_t cycles) {
    for(uint32_t i=0; i<cycles; i++) {
        __asm__ volatile ("nop");
    }
}

static void jump_to_app(void) {
    typedef void (*void_fn)(void);

    uint32_t *reset_vector_entry = (uint32_t *)(APP_START_ADDRESS + 4U);
    uint32_t *reset_vector =  (uint32_t *)(*reset_vector_entry);

    void_fn jump_func = (void_fn) reset_vector;
    jump_func();
}

int main(void) {
    rcc_setup();
    gpio_setup();

    for (uint32_t i=0; i<21; i++) {
        gpio_toggle(LED_PORT, LED_PIN);
        delay_cycles(24000000/4);
    }
    jump_to_app();


    while (1) {
    }

    return 0;
}