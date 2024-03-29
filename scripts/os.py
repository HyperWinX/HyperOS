#!/usr/bin/env python3

import glob
import os
import subprocess
import sys
import datetime

COLOR_RED_BOLD = '\033[31;1m'
COLOR_YELLOW_BOLD = '\033[33;1m'
COLOR_GREEN_BOLD = '\033[32;1m'
COLOR_CYAN_BOLD = '\033[36;1m'
COLOR_RESET = '\033[0m'

ASSEMBLE_COMMAND = "nasm -f elf32 %s -o %s"
ASSEMBLE_COMMAND_DEBUG = "nasm -f elf32 -g %s -o %s"
C_COMPILE_COMMAND = "gcc -m32 -Wall -Wextra -fno-stack-protector -fno-builtin -ffreestanding -nostdlib -Isrc/include -c %s -o %s"
CXX_COMPILE_COMMAND = "g++ -m32 -Wall -Wextra -fno-stack-protector -fno-builtin -ffreestanding -nostdlib -Isrc/include -c %s -o %s"
C_COMPILE_COMMAND_DEBUG = "gcc -O0 -g -Wall -Wextra -nostdlib -Isrc/include -m32 -ffreestanding -c %s -o %s"
CXX_COMPILE_COMMAND_DEBUG = "g++ -O0 -g -Wall -Wextra -nostdlib -Isrc/include -m32 -ffreestanding -c %s -o %s"
COMPILE_TEST_COMMAND = "g++ %s %s -m32 -lgtest -lgtest_main -pthread -fprofile-arcs -ftest-coverage"
LINK_COMMAND = "ld -m elf_i386 --gc-sections -static -o %s %s %s -T %s"
LINK_COMMAND_DEBUG = "ld -m elf_i386 -O0 -g -static -o %s %s %s -T %s"

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC_DIR = os.path.join(REPO_ROOT, 'src')
TEST_DIR = os.path.join(REPO_ROOT, 'test')
BUILD_DIR = os.path.join(REPO_ROOT, 'build')
DIST_DIR = os.path.join(REPO_ROOT, 'dist')
OBJ_DIR = os.path.join(REPO_ROOT, 'objects')
LINKER_SCRIPT = os.path.join(REPO_ROOT, 'src', 'linker.ld')

ASM_FILES = glob.glob(os.path.join(SRC_DIR, '**', '*.asm'), recursive=True)
SOURCE_FILES = glob.glob(os.path.join(SRC_DIR, '**', '*.c'), recursive=True)
UNIT_TEST_FILES = glob.glob(os.path.join(TEST_DIR, 'unit', '*.cpp'), recursive=True)

BIN_FILES = [f.replace('.asm', '.bin') for f in ASM_FILES]
BIN_FILES = [f.replace('src', 'objects') for f in BIN_FILES]
O_FILES = [f.replace('.c', '.o') for f in SOURCE_FILES]
O_FILES = [f.replace('src', 'objects') for f in O_FILES]

KERNEL_OUT = os.path.join(DIST_DIR, 'hyperos.elf')
ISO_OUT = os.path.join(DIST_DIR, 'hyperos.iso')

PATHS = ['objects',
         'objects/common',
         'objects/drivers',
         'objects/drivers/ide',
         'objects/drivers/pci',
         'objects/drivers/vga',
         'objects/kernel',
         'objects/kernel/pit',
         'objects/kernel/gdt',
         'objects/kernel/idt',
         'objects/kernel/cpuid',
         'objects/memory',
         'objects/screen']


def create_dirs(arr):
    for path in arr:
        try:
            os.mkdir(os.path.join(REPO_ROOT, path))
        except:
            a = 0

def pretty_print(message, color=COLOR_GREEN_BOLD):
    print(color + message + COLOR_RESET)

def pretty_call(command, color=COLOR_GREEN_BOLD, shell=False):
    pretty_print(command, color)
    subprocess.check_call(command.split(), shell=shell)

def build(debug=False):
    start = datetime.datetime.now()
    os.makedirs('build', exist_ok=True)
    os.makedirs('dist', exist_ok=True)
    os.makedirs('public', exist_ok=True)
    create_dirs(PATHS)
    # Assemble assembly code
    assemble_start = datetime.datetime.now()
    for file_in, file_out in zip(ASM_FILES, BIN_FILES):
        if debug:
            assemble_command = ASSEMBLE_COMMAND_DEBUG
        else:
            assemble_command = ASSEMBLE_COMMAND
        rendered_command = assemble_command % (file_in, file_out)
        pretty_call(rendered_command, COLOR_RED_BOLD)
    assemble_end = datetime.datetime.now()
    # Compile C code
    for file_in, file_out in zip(SOURCE_FILES, O_FILES):
        if debug and file_in.endswith(".cpp"):
            compile_command = CXX_COMPILE_COMMAND_DEBUG
        elif debug and file_in.endswith(".c"):
            compile_command = C_COMPILE_COMMAND_DEBUG
        elif file_in.endswith(".cpp"):
            compile_command = CXX_COMPILE_COMMAND
        elif file_in.endswith(".c"):
            compile_command = C_COMPILE_COMMAND
        rendered_command = compile_command % (file_in, file_out)
        pretty_call(rendered_command, COLOR_YELLOW_BOLD)
    compile_end = datetime.datetime.now()
    # Link the assembly and C code together, create .bin file
    rendered_command = ""
    if (debug):
        rendered_command = LINK_COMMAND_DEBUG % (
            KERNEL_OUT,
            ' '.join(BIN_FILES),
            ' '.join(O_FILES),
	    LINKER_SCRIPT,
        )
    else:
        rendered_command = LINK_COMMAND % (
            KERNEL_OUT,
            ' '.join(BIN_FILES),
            ' '.join(O_FILES),
	    LINKER_SCRIPT,
        )
    pretty_call(rendered_command, COLOR_CYAN_BOLD)
    link_end = datetime.datetime.now()
    # Generate ISO file
    pretty_call('mkdir -p build/iso/boot/grub', COLOR_CYAN_BOLD)
    pretty_call('cp grub.cfg build/iso/boot/grub', COLOR_CYAN_BOLD)
    pretty_call('cp %s build/iso/boot/grub' % KERNEL_OUT, COLOR_CYAN_BOLD)
    pretty_call('grub-mkrescue -o %s build/iso' % ISO_OUT, COLOR_CYAN_BOLD)
    pretty_call('rm -rf build', COLOR_CYAN_BOLD)
    pretty_call('rm -rf objects', COLOR_CYAN_BOLD)
    end = datetime.datetime.now()
    pretty_print('Built %s' % KERNEL_OUT)
    pretty_print('Built %s' % ISO_OUT)
    print("Done!")
    print(f"Total assembled: {len(BIN_FILES)}, assembled in {assemble_end - assemble_start}")
    print(f"Total compiled: {len(O_FILES)}, compiled in {compile_end - assemble_end}")
    print(f"Linked in {link_end - compile_end}")
    print(f"Total time: {end - start}")

def test():
    # build()
    test_unit()
    test_integration()

def test_unit():
    pretty_call('rm -rf build')
    pretty_call('mkdir build')
    os.chdir(BUILD_DIR)
    pretty_call(
        COMPILE_TEST_COMMAND % (
            ' '.join(UNIT_TEST_FILES), 
            ' '.join(glob.glob(os.path.join(SRC_DIR, 'common', '*.c'))), # temporary workaround
            # ' '.join(BIN_FILES),
            # ' '.join(SOURCE_FILES), # TODO get unit tests working with all source files
        )
    )
    pretty_call('./a.out')
    pretty_call('gcov ../test/unit/*.cpp src/common/*.cpp')
    pretty_call('lcov -b . -d . -c --output-file coverage.info')
    # not sure why this lcov cmd doesn't work with subprocess:
    pretty_print("lcov -r coverage.info '/usr/include/*' '*/test/unit/*.cpp' --output-file coverage.info")
    os.system("lcov -r coverage.info '/usr/include/*' '*/test/unit/*.cpp' --output-file coverage.info")
    pretty_call('genhtml coverage.info --output-directory ../public/')
    os.chdir(REPO_ROOT)

def test_integration():
    pretty_call('pytest %s' % os.path.join(REPO_ROOT, 'test', 'integration'))

SATA_IMG_PATH = 'dist/SATA.img'
SATA_DRIVE_OPTS = f'-drive id=disk,file={SATA_IMG_PATH},if=none -device ahci,id=ahci -device ide-hd,drive=disk,bus=ahci.0'
def run():
    build()
    if not os.path.exists(SATA_IMG_PATH):
        with open(SATA_IMG_PATH, 'w'): # just write a blank file for now
            pass
    pretty_call(f'qemu-system-i386 -kernel {KERNEL_OUT} -monitor stdio -vnc :0 {SATA_DRIVE_OPTS}')

def run_debug():
    build(debug=True)
    if not os.path.exists(SATA_IMG_PATH):
        with open(SATA_IMG_PATH, 'w'): # just write a blank file for now
            pass
    os.system(f'qemu-system-i386 -vnc :0 -cdrom dist/hyperos.iso {SATA_DRIVE_OPTS} -s -S -machine type=pc-i440fx-3.1 &')
    os.system('gdb -x .gdbinit')

def clean():
    os.system('rm -rf build dist public *.gcno a.out')

def print_usage():
    print("Usage: %s [build,test,test-unit,run,run_debug,clean]" % sys.argv[0])
    sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print_usage()
    elif sys.argv[1] == 'build':
        build()
    elif (sys.argv[1]) == 'build_debug':
        build(debug=True)
    elif sys.argv[1] == 'test':
        test()
    elif sys.argv[1] == 'test-unit':
        test_unit()
    elif sys.argv[1] == 'run':
        run()
    elif sys.argv[1] == 'run_debug':
        run_debug()
    elif sys.argv[1] == 'clean':
        clean()
    else:
        print("Command not recognized: %s" % sys.argv[1])
        print_usage()
