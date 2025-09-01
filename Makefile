
#
# Makefile
#
# Antoine LANDRIEUX
# BORIUM <https://github.com/AntoineLandrieux/BORIUM>
#
# MIT License
#

OUT = borium.iso

LD = ld
CC = gcc
NASM = nasm

BIN = bin
STD = STD
CORE = core
BOOT = boot
KERNEL = kernel
DRIVER = driver
INCLUDE = include

LINKER_SCRIPT = script/linker.ld

CFLAGS = -Wall -Wextra -ffreestanding -m32 -fno-pie -fno-stack-protector -O1 -Wno-implicit-fallthrough

default: clean
	mkdir -p bin
	#
	$(NASM) $(BOOT)/entry.asm -f elf -o $(BIN)/entry.o
	#
	$(CC) $(CFLAGS) -c $(KERNEL)/kernel.c -o $(BIN)/kernel.o -I $(INCLUDE)
	#
	$(CC) $(CFLAGS) -c $(DRIVER)/io.c -o $(BIN)/dio.o -I $(INCLUDE)
	$(CC) $(CFLAGS) -c $(DRIVER)/keyboard.c -o $(BIN)/dkeyboard.o -I $(INCLUDE)
	$(CC) $(CFLAGS) -c $(DRIVER)/speaker.c -o $(BIN)/dspeaker.o -I $(INCLUDE)
	$(CC) $(CFLAGS) -c $(DRIVER)/video.c -o $(BIN)/dvideo.o -I $(INCLUDE)
	#
	$(CC) $(CFLAGS) -c $(STD)/stdlib.c -o $(BIN)/stdlib.o -I $(INCLUDE)
	#
	$(CC) $(CFLAGS) -c $(CORE)/Error.c -o $(BIN)/Error.o -I $(INCLUDE)
	$(CC) $(CFLAGS) -c $(CORE)/Math.c -o $(BIN)/Math.o -I $(INCLUDE)
	$(CC) $(CFLAGS) -c $(CORE)/Memory.c -o $(BIN)/Memory.o -I $(INCLUDE)
	$(CC) $(CFLAGS) -c $(CORE)/Parser.c -o $(BIN)/Parser.o -I $(INCLUDE)
	$(CC) $(CFLAGS) -c $(CORE)/Runtime.c -o $(BIN)/Runtime.o -I $(INCLUDE)
	$(CC) $(CFLAGS) -c $(CORE)/Tokenizer.c -o $(BIN)/Tokenizer.o -I $(INCLUDE)
	#
	$(LD) -m elf_i386 -T $(LINKER_SCRIPT) -o iso/boot/borium.elf $(BIN)/entry.o $(BIN)/kernel.o $(BIN)/dio.o $(BIN)/dvideo.o $(BIN)/dspeaker.o $(BIN)/dkeyboard.o $(BIN)/stdlib.o $(BIN)/Error.o $(BIN)/Math.o $(BIN)/Memory.o $(BIN)/Parser.o $(BIN)/Runtime.o $(BIN)/Tokenizer.o
	#
	grub-mkrescue -o $(OUT) iso

run:
	qemu-system-x86_64 -cdrom $(OUT)

clean:
	rm -drf $(BIN)
	rm -drf $(OUT)
