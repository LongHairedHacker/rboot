// Copyright 2015 Richard A Burton
// richardaburton@gmail.com
// See license.txt for license terms.

#include "ets_sys.h"
#include "gpio.h"
#include "os_type.h"
#include "osapi.h"
#include <c_types.h>

#ifndef ROM_ID
#define ROM_ID -1
#endif

static volatile os_timer_t blink_timer;

void ICACHE_FLASH_ATTR blink_timerfunc(void *arg)
{
	ets_printf("Testload %d blink\r\n", ROM_ID);
    //Do blinky stuff
    if (GPIO_REG_READ(GPIO_OUT_ADDRESS) & BIT2)
    {
        //Set GPIO2 to LOW
        gpio_output_set(0, BIT2, BIT2, 0);
    }
    else
    {
        //Set GPIO2 to HIGH
        gpio_output_set(BIT2, 0, BIT2, 0);
    }
}


void ICACHE_FLASH_ATTR user_rf_pre_init() {
	ets_printf("Testload %d user_rf_pre_init\r\n", ROM_ID);
	ets_delay_us(20000);
}


void ICACHE_FLASH_ATTR  user_init(void) {
	uart_div_modify(0, UART_CLK_FREQ / 115200);
	ets_printf("Testload %d user_init\r\n", ROM_ID);
	ets_delay_us(20000);

	gpio_init();

	//Set GPIO2 to output mode
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);

	//Set GPIO2 low
	gpio_output_set(0, BIT2, BIT2, 0);


	os_timer_disarm(&blink_timer);
	os_timer_setfn(&blink_timer, (os_timer_func_t *)blink_timerfunc, NULL);
	os_timer_arm(&blink_timer, 1000, 1);
}
