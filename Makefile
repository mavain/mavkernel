NASM_PATH	= nasm
GCC_PATH	= /opt/cross/bin/i686-elf-gcc

NASM_FLAGS	= -felf32
GCC_FLAGS	= -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Iinc -Iinc/libc
LD_FLAGS	= -ffreestanding -O2 -nostdlib -lgcc

LD_FILE		= linker.ld
OUT_FILE	= mavos.bin

SRC_PATH	= src
ASM_FILES	= multiboot kernel/asm/gdt kernel/asm/idt kernel/asm/keyboard
C_FILES		= kernel/kernel_main kernel/tty kernel/gdt kernel/idt kernel/keyboard libc/string 

ASM_FILES_IN	= $(addprefix $(SRC_PATH)/,$(addsuffix .asm,$(ASM_FILES)))
C_FILES_IN		= $(addprefix $(SRC_PATH)/,$(addsuffix .c,$(C_FILES)))

ALL_FILES_OUT	= $(addsuffix .o,$(addprefix $(SRC_PATH)/,$(C_FILES) $(ASM_FILES)))

%.o: %.asm
	$(NASM_PATH) $(NASM_FLAGS) $< -o $@

%.o: %.c
	$(GCC_PATH) -c $< -o $@ $(GCC_FLAGS)

$(OUT_FILE): $(LD_FILE) $(ALL_FILES_OUT)
	$(GCC_PATH) -T $(LD_FILE) -o $(OUT_FILE) $(LD_FLAGS) $(ALL_FILES_OUT)	

.PHONY: run
run:
	make all
	qemu-system-i386 -kernel $(OUT_FILE)

.PHONY: all
all:
	make $(OUT_FILE)

.PHONY: clean
clean:
	rm -rf $(ALL_FILES_OUT)
	rm -rf $(OUT_FILE)
