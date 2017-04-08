Addresses
=========

Setor Size: 0x1000 bytes
Flashsize: 4MB = 0x400000 bytes
Bootloader Size: 0x2000 bytes
Rom Size: 0x100000 byte or 1000 Sectors

First 0x2000 byte are only used for bootloader in Rom0
Emtpy in the others -> same base address inside mapped memory space -> same linker file

physical Flash addresses:
Rom0: 0x000000 + 0x2000 = 0x002000
Rom1: 0x100000 + 0x2000 = 0x102000
Rom2: 0x200000 + 0x2000 = 0x202000
Rom3: 0x300000 + 0x2000 = 0x302000

Virtual Base Address: 0x40200000
Bootloader Size: 0x2000
Rom Header: 0x10 byte after space for bootloader

-> Base address: 0x40200000 + 0x2000 + 0x10 = 0x40202010
