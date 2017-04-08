Flash layout
============

Setor Size: 0x1000 bytes
Flashsize: 4MB = 0x400000 bytes
Bootloader Size: 0x2000 bytes
Rom Size: 0x100000 byte or 1000 Sectors

First 0x2000 bytes are used for bootloader in Rom0 and are emtpy in the others
-> same base address inside mapped memory space -> same linker file

Flash addresses
---------------
* Rom0: 0x000000 + 0x2000 = 0x002000
* Rom1: 0x100000 + 0x2000 = 0x102000
* Rom2: 0x200000 + 0x2000 = 0x202000
* Rom3: 0x300000 + 0x2000 = 0x302000

Linking
-------
Virtual Base Address: 0x40200000
Bootloader Size: 0x2000
Rom Header: 0x10 byte after space for bootloader
-> Base address: 0x40200000 + 0x2000 + 0x10 = 0x40202010


Building & Testing
==================
1. Set correct paths in Makefile and testroms/Makefile
2. Build and flash rboot
	```
	make all
	esptool.py erase_flash
	esptool.py write_flash 0x0000 firmware/rboot.bin
	```

3. Test rboot without any roms.
 	Use a serial monitor (e.g. screen) and reset the esp.
	```
	screen /dev/ttyUSB0 115200
	<press reset>
	<some garbage appears>
	rBoot v1.4.2 - richardaburton@gmail.com
	Flash Size:   32 Mbit
	Flash Mode:   QIO
	Flash Speed:  40 MHz
	rBoot Option: Big flash

	Rom 0 is bad.
	Rom 3 is bad.
	Rom 2 is bad.
	Rom 1 is bad.
	No good rom available.
	user code done
	```

4. Build the testroms
 	```
	cd testroms
	make all
	```

5. Flash one of the testroms, ideally not 0, since we want to test bgiflash
	```
	esptool.py write_flash 0x102000 firmware/rom1.bin
	```

6. Use a serial monitor to check the result
	```
	screen /dev/ttyUSB0 115200
	<reset>
	rBoot v1.4.2 - richardaburton@gmail.com
	Flash Size:   32 Mbit
	Flash Mode:   QIO
	Flash Speed:  40 MHz
	rBoot Option: Big flash

	Booting rom 1.
	Testload 1 user_rf_pre_init
	rf_cal[0] !=0x05,is 0xFF
	<lots of repeating garbage>
	```

**NOTE:** If you only see garbage it might help to replug the usb cable, the CH340 seems to get confused from time to time.
Since there are 20s boot delay, replugging the usb and then starting the serial monitor works just fine to capture the output before the garbage.
