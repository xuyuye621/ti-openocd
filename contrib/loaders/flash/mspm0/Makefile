# SPDX-License-Identifier: GPL-2.0-or-later

BIN2C = ../../../../src/helper/bin2char.sh

CROSS_COMPILE ?= arm-none-eabi-

CC = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

CFLAGS = -static -nostartfiles -mlittle-endian -mthumb -mcpu=cortex-m0plus -Wa,-EL

all: mspm0_erase.inc mspm0_write.inc mspm0_mass_erase.inc

.PHONY: clean

.INTERMEDIATE: mspm0_erase.elf mspm0_write.elf mspm0_mass_erase.elf

%.elf: %.S
	$(CC) $(CFLAGS) $< -o $@

%.lst: %.elf
	$(OBJDUMP) -S $< > $@

%.bin: %.elf
	$(OBJCOPY) -Obinary $< $@

%.inc: %.bin
	$(BIN2C) < $< > $@

clean:
	-rm -f *.elf *.lst *.bin *.inc
