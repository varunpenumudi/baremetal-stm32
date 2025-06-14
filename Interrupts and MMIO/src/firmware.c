#include "firmware.h"

#define LED_PORT (GPIOC) 
#define LED_PIN  (GPIO13) 

#define CPU_FREQ (24000000)
#define SYSTICK_FREQ (1000)


volatile uint64_t  ticks=0;
void sys_tick_handler(void) {
    ticks++;
}

static uint64_t get_ticks(void) {
    return ticks;
}

static void rcc_setup(void) {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_24MHZ]);
}

static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_OPENDRAIN, LED_PIN);
}

static void systick_setup(void) {
    systick_set_frequency(SYSTICK_FREQ, CPU_FREQ);
    systick_counter_enable();
    systick_interrupt_enable();
}

// static void delay_cycles(uint32_t cycles) {
//     for (uint32_t i=0; i<cycles; i++) {
//         __asm__ ("nop");
//     }
// }

int main(void) {
    rcc_setup();
    gpio_setup();
    systick_setup();

    uint64_t next_tick = 1000;
    while (1) {
        if (get_ticks() >= next_tick) {
            gpio_toggle(LED_PORT, LED_PIN);
            next_tick = get_ticks()+1000;
        }
    }

    return 0;
}