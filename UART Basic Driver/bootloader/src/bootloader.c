#include "common-defines.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/memorymap.h>

#include "core/system.h"
#include "core/uart.h"

#define BOOTLOADER_SIZE   (0x6000)
#define APP_START_ADDRESS (FLASH_BASE + BOOTLOADER_SIZE)

#define LED_PORT (GPIOC) 
#define LED_PIN  (GPIO13) 

#define USART_PORT   (GPIOA)
#define USART_TX_PIN (GPIO_USART2_TX)
#define USART_RX_PIN (GPIO_USART2_RX)


static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOC);   // For GPIO
    rcc_periph_clock_enable(RCC_GPIOA);   // For UART
    rcc_periph_clock_enable(RCC_AFIO);    // For UART

    gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, LED_PIN);   // PC13 -> GPIO

    gpio_set_mode(USART_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, USART_TX_PIN);  // PA2 -> Tx
    gpio_set_mode(USART_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, USART_RX_PIN);                    // PA3 -> Rx
}

static void delay_ms(uint64_t millis) {
    uint64_t now = system_get_ticks();
    while (system_get_ticks() < now + millis);
}

static void jump_to_app(void) {
    typedef void (*void_fn)(void);

    uint32_t *reset_vector_entry = (uint32_t *)(APP_START_ADDRESS + 4U);
    uint32_t *reset_vector =  (uint32_t *)(*reset_vector_entry);

    void_fn jump_func = (void_fn) reset_vector;
    jump_func();
}

int main(void) {
    system_setup();
    gpio_setup();
    uart_setup();

    uint8_t data[12] = "BOOT MODE \r\n";
    for (uint32_t i=0; i<5; i++) {
        gpio_set(LED_PORT, LED_PIN);
        delay_ms(250);
        gpio_clear(LED_PORT, LED_PIN);
        delay_ms(250);
        uart_write(data, 12);
    }
    jump_to_app();


    while (1) {
    }

    return 0;
}