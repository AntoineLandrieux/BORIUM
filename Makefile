
#
# Makefile
#
# Antoine LANDRIEUX
# BORIUM <https://github.com/AntoineLandrieux/BORIUM>
#
# MIT License
#

###############
# FILES
###############
OUT = borium.bin

###############
# PATH
###############
BIN_DIR = bin
BOOT_DIR = boot/x86
SHELL_DIR = shell
KERNEL_DIR = kernel/x86
DRIVER_DIR = driver/x86/io

LINKER_SCRIPT = script/linker.ld

###############
# FLAGS
###############
CFLAGS = -Wall -Wextra -I include -ffreestanding -m32 -fno-pie -fno-stack-protector -O1


default:
	mkdir -p $(BIN_DIR)

	nasm $(BOOT_DIR)/boot.asm -f bin -o $(BIN_DIR)/boot.bin
	nasm $(BOOT_DIR)/entry.asm -f elf -o $(BIN_DIR)/entry.o

	gcc $(CFLAGS) -c $(KERNEL_DIR)/kernel.c -o $(BIN_DIR)/kernel.o
	gcc $(CFLAGS) -c $(DRIVER_DIR)/video.c -o $(BIN_DIR)/video.o
	gcc $(CFLAGS) -c $(SHELL_DIR)/shell.c -o $(BIN_DIR)/shell.o
	gcc $(CFLAGS) -c $(DRIVER_DIR)/keyboard.c -o $(BIN_DIR)/keyboard.o
	ld -m elf_i386 -T $(LINKER_SCRIPT) -o $(BIN_DIR)/kernelfull.bin $(BIN_DIR)/entry.o $(BIN_DIR)/kernel.o $(BIN_DIR)/video.o $(BIN_DIR)/keyboard.o $(BIN_DIR)/shell.o --oformat binary
	(cat $(BIN_DIR)/boot.bin ; cat $(BIN_DIR)/kernelfull.bin) > $(BIN_DIR)/$(OUT)


run:
	qemu-system-x86_64 $(BIN_DIR)/$(OUT) -no-reboot


clean:
	rm -drf $(BIN_DIR)

