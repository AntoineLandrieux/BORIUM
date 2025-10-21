SHELL := /bin/bash

OUT      := borium.iso
LINK_ELF := iso/boot/borium.elf

CC         := gcc
LD         := ld
NASM       := nasm
GRUB_MKRES := grub-mkrescue
QEMU       := qemu-system-x86_64
RM         := rm -rf
MKDIR_P    := mkdir -p

BIN     := bin
STD     := STD
CORE    := core
BOOT    := boot
KERNEL  := kernel
DRIVER  := driver
INCLUDE := include

LINKER_SCRIPT := script/linker.ld

CFLAGS := -Wall -Wextra -Wno-unused-parameter -Wno-implicit-fallthrough
CFLAGS += -ffreestanding -m32 -fno-pie -fno-stack-protector
CFLAGS += -MMD -MP -I$(INCLUDE)

ASFLAGS := -f elf
LDFLAGS := -m elf_i386 -T $(LINKER_SCRIPT)

C_SRCS := $(wildcard $(KERNEL)/*.c) $(wildcard $(DRIVER)/*.c) $(wildcard $(CORE)/*.c) $(wildcard $(STD)/*.c)
C_OBJS := $(addprefix $(BIN)/,$(notdir $(patsubst %.c,%.o,$(C_SRCS))))

ASM_OBJS := $(BIN)/entry.o
ALL_OBJS := $(ASM_OBJS) $(C_OBJS)

.PHONY: all default iso run clean distclean help

default: clean all

all: $(LINK_ELF) $(OUT)

$(ASM_OBJS): $(BOOT)/entry.asm | $(BIN)
	$(NASM) $(ASFLAGS) $< -o $@

$(BIN)/%.o: $(KERNEL)/%.c | $(BIN)
	$(CC) $(CFLAGS) -c $< -o $@
$(BIN)/%.o: $(DRIVER)/%.c | $(BIN)
	$(CC) $(CFLAGS) -c $< -o $@
$(BIN)/%.o: $(CORE)/%.c | $(BIN)
	$(CC) $(CFLAGS) -c $< -o $@
$(BIN)/%.o: $(STD)/%.c | $(BIN)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN):
	$(MKDIR_P) $(BIN)

$(LINK_ELF): $(ALL_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

$(OUT): $(LINK_ELF)
	$(GRUB_MKRES) -o $@ iso

run:
	$(QEMU) -cdrom $(OUT)

clean:
	$(RM) $(BIN)
	$(RM) iso/boot/borium.elf

distclean: clean
	$(RM) $(OUT)

-include $(C_OBJS:.o=.d)

help:
	@echo "Usage:"
	@echo "  make         -> build everything (iso)"
	@echo "  make run     -> build and run in qemu"
	@echo "  make clean   -> remove build objects"
	@echo "  make distclean -> remove build + iso"
