// Copyright 2015 Richard A Burton
// richardaburton@gmail.com
// See license.txt for license terms.

#include <c_types.h>

#ifndef ROM_ID
#define ROM_ID -1
#endif

void ICACHE_FLASH_ATTR user_rf_pre_init() {
	ets_printf("Testload %d user_rf_pre_init\r\n", ROM_ID);
}


void ICACHE_FLASH_ATTR  user_init(void) {
	uint8 loop;
	for(loop = 0; loop < 50; loop++) {
		ets_printf("Testload %d user_init\r\n", ROM_ID);
		ets_delay_us(20000);
	}
}
