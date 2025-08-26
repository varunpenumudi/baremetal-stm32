#include "common-defines.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/memorymap.h>

#include "core/system.h"
#include "core/uart.h"
#include "comms.h"

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

static void jump_to_app(void) {
    typedef void (*void_fn)(void);

    uint32_t *reset_vector_entry = (uint32_t *)(APP_START_ADDRESS + 4U);
    uint32_t *reset_vector =  (uint32_t *)(*reset_vector_entry);

    void_fn jump_func = (void_fn) reset_vector;
    jump_func();
}

int main(void) {
    // comms_packet_t packet;
    system_setup();
    gpio_setup();
    uart_setup();
    comms_setup();
    comms_packet_t packet;

    uint8_t boot_string[12] = "Booting uC\r\n";
    for (uint8_t i = 0; i < 5; i++) uart_write(boot_string, 12);

    while (1) {
        // gpio_toggle(LED_PORT, LED_PIN);
        // system_delay_ms(1000);
        while (uart_data_available()) {
            comms_update();
        }
        if (comms_packets_availabe()) {
            comms_read(&packet);
        }
        // jump_to_app();
    }

    return 0;
}