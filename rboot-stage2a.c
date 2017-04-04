//////////////////////////////////////////////////
// rBoot open source boot loader for ESP8266.
// Copyright 2015 Richard A Burton
// richardaburton@gmail.com
// See license.txt for license terms.
//////////////////////////////////////////////////

#ifdef RBOOT_INTEGRATION
#include <rboot-integration.h>
#endif

#include "rboot-private.h"

#define UART_BASE 0x60000000
#define UART(i) (*(struct UART_REGS *)(UART_BASE + (i)*0xf00))

struct UART_REGS {
    uint32_t volatile FIFO;           // 0x00
    uint32_t volatile INT_RAW;        // 0x04
    uint32_t volatile INT_STATUS;     // 0x08
    uint32_t volatile INT_ENABLE;     // 0x0c
    uint32_t volatile INT_CLEAR;      // 0x10
    uint32_t volatile CLOCK_DIVIDER;  // 0x14
    uint32_t volatile AUTOBAUD;       // 0x18
    uint32_t volatile STATUS;         // 0x1c
    uint32_t volatile CONF0;          // 0x20
    uint32_t volatile CONF1;          // 0x24
    uint32_t volatile LOW_PULSE;      // 0x28
    uint32_t volatile HIGH_PULSE;     // 0x2c
    uint32_t volatile PULSE_COUNT;    // 0x30
    uint32_t volatile _unused[17];    // 0x34 - 0x74
    uint32_t volatile DATE;           // 0x78
    uint32_t volatile ID;             // 0x7c
};

struct UART_REGS *uart = (struct UART_REGS*)0x60000000;

/*
 {
		int i;
		for(i = 0; i < 1000; i++)
			asm volatile ("nop");

		*((volatile uint32_t*)0x60000000) = 'y';
		for(i = 0; i < 1000; i++)
			asm volatile ("nop");
	}
*/

#define FIELD2VAL(fieldname, regbits) (((regbits) >> fieldname##_S) & fieldname##_M)

#define UART_STATUS_TXFIFO_COUNT_M           0x000000ff
#define UART_STATUS_TXFIFO_COUNT_S           16
#define UART_FIFO_MAX 127

/* Wait for at least `min_count` bytes of space to be available in the UART's
 * transmit FIFO
 *
 * Returns the number of bytes actually available in the write buffer.
 */
static int uart_txfifo_wait(int uart_num, int min_count) {
    int count;
    do {
        count = UART_FIFO_MAX - FIELD2VAL(UART_STATUS_TXFIFO_COUNT, UART(uart_num).STATUS);
    } while (count < min_count);
    return count;
}


/* Write a character to the UART.  Blocks if necessary until there is space in
 * the TX FIFO.
 */
static void mputc(char c) {
    uart_txfifo_wait(0, 1);
    UART(0).FIFO = c;
}

static void mpn(char c, uint32_t num)
{
	int i;

	mputc(c);
	mputc(' ');

	for (i = 28; i >= 0; i -= 4) {
		uint8_t c = (num >> i) & 0x0F;

		if (c >= 10)
			mputc(c - 10 + 'A');
		else
			mputc(c + '0');
	}
	mputc('\r');
	mputc('\n');
}

usercode* NOINLINE load_rom(uint32 readpos) {
	
	uint8 sectcount;
	uint8 *writepos;
	uint32 remaining;
	usercode* usercode;
	
	rom_header header;
	section_header section;

	mpn('R', readpos);
	
	// read rom header
	SPIRead(readpos, &header, sizeof(rom_header));
	readpos += sizeof(rom_header);

	// create function pointer for entry point
	usercode = header.entry;
	//mpn('E', (uint32_t)usercode);
	mpn('C', (uint32_t)header.count);
	
	// copy all the sections
	for (sectcount = header.count; sectcount > 0; sectcount--) {
		mputc('0' + sectcount);
		mputc('\r');
		mputc('\n');

		// read section header
		SPIRead(readpos, &section, sizeof(section_header));
		readpos += sizeof(section_header);

		// get section address and length
		writepos = section.address;
		remaining = section.length;
		mpn('a', (uint32_t)section.address);
		mpn('l', (uint32_t)section.length);
		mpn('x', (uint32_t)readpos);
		
		while (remaining > 0) {
			// work out how much to read, up to 16 bytes at a time
			uint32 readlen = (remaining < READ_SIZE) ? remaining : READ_SIZE;
			// read the block
			mpn('W', (uint32_t)writepos);
			mpn('L', (uint32_t)readlen);
			SPIRead(readpos, writepos, readlen);
			mputc('!');
			readpos += readlen;
			// increment next write position
			writepos += readlen;
			// decrement remaining count
			remaining -= readlen;
		}
	}

	return usercode;
}

#ifdef BOOT_NO_ASM

void call_user_start(uint32 readpos) {
	usercode* user;
	user = load_rom(readpos);
	user();
}

#else

void call_user_start(uint32 readpos) {
	__asm volatile (
		"mov a15, a0\n"     // store return addr, we already splatted a15!
		"call0 load_rom\n"  // load the rom
		"mov a0, a15\n"     // restore return addr
		"jx a2\n"           // now jump to the rom code
	);
}

#endif
