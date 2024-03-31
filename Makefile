# Compilers
CC				= $(shell which clang)
CXX				= $(shell which clang++)
AS				= $(shell which nasm)
LD				= $(shell which gold)
QEMU			= $(shell which qemu-system-x86_64)
XORRISO			= $(shell which xorriso)

# Defined variables
ASM_ARCH		= elf64
LD_ARCH			= elf_x86_64
BITSIZE			= 64
CLANG_TARGET	= x86_64-unknown-none

# Flags
DEFAULT_FLAGS	= -I$(INCLUDE) -mgeneral-regs-only -c -static -nostdlib -mcmodel=kernel -ffreestanding -Wall -Wextra -fno-stack-protector -fno-builtin -m$(BITSIZE) -target $(CLANG_TARGET) 
CFLAGS_STD		= $(DEFAULT_FLAGS) -O2
ASM_FLAGS_STD	= -f $(ASM_ARCH) -Ov
LINKER_FLAGS_STD= -m $(LD_ARCH) -static -O2 -T $(LINKER_SCRIPT)
QEMU_FLAGS_STD	= -cdrom $(ISO_OUT) -monitor stdio -vnc :0 -machine type=pc-i440fx-3.1
XORRISO_FLAGS	= -as mkisofs -b boot/limine/limine-bios-cd.bin 			\
				  -no-emul-boot -boot-load-size 4 -boot-info-table	   		\
				  --efi-boot boot/limine/limine-uefi-cd.bin	   				\
				  -efi-boot-part --efi-boot-image --protective-msdos-label	\
				  $(ISO_DIR) -o $(ISO_OUT)

# All files
C_FILES			= $(shell find ./$(SRC) -name *.c)
CPP_FILES		= $(shell find ./$(SRC) -name *.cpp)
ASM_FILES		= $(shell find ./$(SRC) -name *.asm)
LINKER_SCRIPT	= $(SRC)/linker.ld
LIMINE_CFG		= ./limine.cfg

OBJ_FILES_STD 	= $(C_FILES:./$(SRC)%.c=./objects%.o) $(CPP_FILES:./$(SRC)%.cpp=./objects%.o)
BIN_FILES_STD	= $(ASM_FILES:./$(SRC)%.asm=./objects%.bin)
KERNEL_OUT		= dist/hyperos.elf
ISO_OUT			= dist/hyperos.iso

# Paths to directories
ISO_DIR			= build/iso
LIMINE_REPO		= /limine
ISO_BOOT_DIR	= $(ISO_DIR)/boot
LIMINE_BOOT_DIR	= $(ISO_DIR)/boot/limine
EFI_BOOT_DIR	= $(ISO_DIR)/EFI/BOOT
SRC				= newsrc
INCLUDE			= newinclude

# Directories to create
DIRECTORIES		= 'objects' 				\
				  'objects/common'			\
				  'objects/drivers'			\
				  'objects/drivers/pci'		\
				  'objects/drivers/vga'		\
				  'objects/drivers/disk_io/blockdevice'\
				  'objects/drivers/screen'	\
				  'objects/fonts'			\
				  'objects/kernel'			\
				  'objects/kernel/pit'		\
				  'objects/kernel/gdt'		\
				  'objects/kernel/idt'		\
				  'objects/kernel/cpuid'	\
				  'objects/kernel/memory'	\
				  'objects/memory'			\
				  'objects/screen'			\
				  'build/iso/boot/grub'

all:
	@echo $(AS)
	@echo Available HyperOS build targets:
	@echo [run \| run_debug \| iso \| kernel \| kernel_debug]

# Build and run kernel
run: $(ISO_OUT)
	$(QEMU) $(QEMU_FLAGS_STD)

# Compile ISO image
iso: kernel
	@mkdir -p $(ISO_BOOT_DIR)
	@mkdir -p $(LIMINE_BOOT_DIR)
	@mkdir -p $(EFI_BOOT_DIR)
	@cp -v dist/hyperos.elf $(ISO_BOOT_DIR)
	@cp -v $(LIMINE_CFG) $(LIMINE_REPO)/limine-bios.sys $(LIMINE_REPO)/limine-bios-cd.bin \
		$(LIMINE_REPO)/limine-uefi-cd.bin $(LIMINE_BOOT_DIR)
	@cp -v $(LIMINE_REPO)/BOOTX64.EFI $(EFI_BOOT_DIR)
	@cp -v $(LIMINE_REPO)/BOOTIA32.EFI $(EFI_BOOT_DIR)
	$(XORRISO) $(XORRISO_FLAGS)
	$(LIMINE_REPO)/limine bios-install $(ISO_OUT)

# Link kernel
kernel: full_clean directories $(OBJ_FILES_STD) $(BIN_FILES_STD)
	$(LD) $(BIN_FILES_STD) $(OBJ_FILES_STD) $(LINKER_FLAGS_STD) -o $(KERNEL_OUT)

# Compile object file from C source file
objects/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS_STD) $^ -o $@

# Compile object file from C++ source file
objects/%.o: $(SRC)/%.cpp
	$(CXX) $(CFLAGS_STD) $^ -o $@

# Assemble binary file from ASM source file
objects/%.bin: $(SRC)/%.asm
	$(AS) $(ASM_FLAGS_STD) $^ -o $@

# Create required directories
directories:
	mkdir -p $(DIRECTORIES)
	mkdir -p $(ISO_DIR)
	mkdir -p dist

# Do cleanup
full_clean:
	@rm -rf build dist public objects

postbuild_clean:
	@rm -rf build objects

# Open docker shell, pull if no there is no build env
shell:
	@sudo docker-compose run dev