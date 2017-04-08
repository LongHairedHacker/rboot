#
# Makefile for rBoot sample project
# https://github.com/raburton/esp8266
#


SDK_BASE   ?= $(ESP_SDK)
SDK_LIBDIR  ?= lib
SDK_INCDIR  ?= include

ESPTOOL2     ?= ../../esptool2/esptool2
FW_SECTS      = .text .data .rodata
FW_USER_ARGS  = -quiet -bin -boot2


CC := xtensa-lx106-elf-gcc
LD := xtensa-lx106-elf-gcc


BUILD_DIR = build
FIRMW_DIR = firmware

SDK_LIBDIR := $(addprefix $(SDK_BASE)/,$(SDK_LIBDIR))
SDK_INCDIR := $(addprefix -I$(SDK_BASE)/,$(SDK_INCDIR))

LIBS    = c gcc hal phy net80211 lwip wpa main2 pp crypto
CFLAGS  = -Os -g -O2 -Wpointer-arith -Wundef -Werror -Wno-implicit -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls  -mtext-section-literals  -D__ets__ -DICACHE_FLASH
LDFLAGS = -nostdlib -Wl,--no-check-sections -Wl,-static

LIBS		:= $(addprefix -l,$(LIBS))

.SECONDARY:
.PHONY: all clean


all: $(BUILD_DIR) $(FIRMW_DIR) $(BUILD_DIR)/libmain2.a $(FIRMW_DIR)/rom0.bin $(FIRMW_DIR)/rom1.bin $(FIRMW_DIR)/rom2.bin $(FIRMW_DIR)/rom3.bin

$(BUILD_DIR)/%.o: %.c
	@echo "CC $<"
	@$(CC) -I. -I.. $(SDK_INCDIR) $(CFLAGS) -o $@ -c $<

$(BUILD_DIR)/rom0.o: testload.c
	@echo "CC $< for slot 0"
	@$(CC) -I. $(SDK_INCDIR) $(CFLAGS) -DROM_ID=0 -o $@ -c $<

$(BUILD_DIR)/rom1.o: testload.c
	@echo "CC $< for slot 1"
	@$(CC) -I. $(SDK_INCDIR) $(CFLAGS) -DROM_ID=1 -o $@ -c $<

$(BUILD_DIR)/rom2.o: testload.c
	@echo "CC $< for slot 2"
	@$(CC) -I. $(SDK_INCDIR) $(CFLAGS) -DROM_ID=2 -o $@ -c $<

$(BUILD_DIR)/rom3.o: testload.c
	@echo "CC $< for slot 3"
	@$(CC) -I. $(SDK_INCDIR) $(CFLAGS) -DROM_ID=3 -o $@ -c $<

$(BUILD_DIR)/libmain2.a: $(SDK_LIBDIR)/libmain.a
	xtensa-lx106-elf-objcopy -W Cache_Read_Enable_New $< $@

$(BUILD_DIR)/%.elf: $(BUILD_DIR)/rboot-bigflash.o $(BUILD_DIR)/%.o
	@echo "LD $(notdir $@)"
	$(LD) -L$(SDK_LIBDIR) -L$(BUILD_DIR) -Trom.ld $(LDFLAGS) -Wl,--start-group $(LIBS) $^ -Wl,--end-group -o $@

$(FIRMW_DIR)/%.bin: $(BUILD_DIR)/%.elf
	@echo "FW $(notdir $@)"
	@$(ESPTOOL2) $(FW_USER_ARGS) $^ $@ $(FW_SECTS)

$(BUILD_DIR):
	@mkdir -p $@

$(FIRMW_DIR):
	@mkdir -p $@

clean:
	@echo "RM $(BUILD_DIR) $(FIRMW_DIR)"
	@rm -rf $(BUILD_DIR)
	@rm -rf $(FIRMW_DIR)
