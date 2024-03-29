# Compilers
GCC				= $(shell which gcc)
CPPGCC			= $(shell which g++)
ASM				= $(shell which nasm)
LINKER			= $(shell which gold)
QEMU			= $(shell which qemu-system-i386)
GRUB_MKRESCUE	= $(shell which grub-mkrescue)

# Defined variables
ASM_ARCH		= elf32
LD_ARCH			= elf_i386

# Flags
DEFAULT_FLAGS	= -Iinclude -static -nostdlib -ffreestanding -Wall -Wextra -fno-stack-protector -fno-builtin -m32
CFLAGS_STD		= -O2 $(DEFAULT_FLAGS)
ASM_FLAGS_STD	= -f $(ASM_ARCH) -Ov
LINKER_FLAGS_STD= -m $(LD_ARCH) -static -O2 -T $(LINKER_SCRIPT)
QEMU_FLAGS_STD	= -cdrom $(ISO_OUT) -monitor stdio -vnc :0 -machine type=pc-i440fx-3.1

# All files
C_FILES			= $(shell find ./src -name *.c)
CPP_FILES		= $(shell find ./src -name *.cpp)
ASM_FILES		= $(shell find ./src -name *.asm)
LINKER_SCRIPT	= src/linker.ld
GRUB_CFG		= ./grub.cfg

OBJ_FILES_STD 	= $(C_FILES:./src%.c=./objects%.o) $(CPP_FILES:./src%.cpp=./objects%.o)
BIN_FILES_STD	= $(ASM_FILES:./src%.asm=./objects%.bin)
KERNEL_OUT		= dist/hyperos.elf
ISO_OUT			= dist/hyperos.iso

# Paths to directories
ISO_DIR			= build/iso
BOOT_GRUB_DIR	= $(ISO_DIR)/boot/grub

# Directories to create
DIRECTORIES		= 'objects' 			\
				  'objects/common'		\
				  'objects/drivers'		\
				  'objects/drivers/pci'	\
				  'objects/drivers/vga'	\
				  'objects/kernel'		\
				  'objects/kernel/pit'	\
				  'objects/kernel/gdt'	\
				  'objects/kernel/idt'	\
				  'objects/kernel/cpuid'\
				  'objects/memory'		\
				  'objects/screen'		\
				  'build/iso/boot/grub'

all:
	@echo $(OBJ_FILES_STD)
	@echo Available HyperOS build targets:
	@echo [run \| run_debug \| iso \| kernel \| kernel_debug]

# Build and run kernel
run: iso
	@$(QEMU) $(QEMU_FLAGS)

# Compile ISO image
iso: $(KERNEL_OUT)
	@cp $(GRUB_CFG) $(BOOT_GRUB)
	@cp $(KERNEL_OUT) $(BOOT_GRUB)
	@$(GRUB_MKRESCUE) -o $(KERNEL_OUT) $(ISO_DIR)
	@$(MAKE) postbuild_clean

# Link kernel
kernel: full_clean directories $(OBJ_FILES_STD) $(BIN_FILES_STD)
	$(LINKER) $(BIN_FILES_STD) $(OBJ_FILES_STD) $(LINKER_FLAGS_STD) -o $(KERNEL_OUT)

# Compile object file from C source file
%.o: %.c
	$(GCC) $(CFLAGS_STD) $^ -o $(patsubst src/%.c,objects/%.o,%^)

# Compile object file from C++ source file
%.o: %.cpp
	$(CPPGCC) $(CFLAGS_STD) $^ -o $(patsubst src/%.cpp,objects/%.o,%^)

# Assemble binary file from ASM source file
%.bin: %.asm
	$(ASM) $(ASM_FLAGS_STD) $^ -o $(patsubst src/%.asm,objects/%.bin,%^)

# Create required directories
directories:
	mkdir -p $(DIRECTORIES)
	mkdir -p $(BOOT_GRUB_DIR)

# Do cleanup
full_clean:
	@rm -rf build dist public objects

postbuild_clean:
	@rm 0rf build objects

# Open docker shell, pull if no there is no build env
shell:
	@sudo docker-compose run dev