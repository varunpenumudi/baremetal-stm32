#ifndef __FIRMWARE_HEADER_INCLUDED__
#define __FIRMWARE_HEADER_INCLUDED__

#include <stdio.h>
#include <stdint.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

static void rcc_setup(void);
static void gpio_setup(void);

#endif