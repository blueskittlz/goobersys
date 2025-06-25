cmake_minimum_required(VERSION 3.15)
project(GooberSys LANGUAGES C CXX ASM_NASM)

# Include path for libs
include_directories(${CMAKE_SOURCE_DIR}/lib/include)

# Assemble bootloader.asm to raw binary with NASM
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/bootloader.bin
    COMMAND nasm -f bin ${CMAKE_SOURCE_DIR}/boot/bootloader.asm -o ${CMAKE_BINARY_DIR}/bootloader.bin
    DEPENDS ${CMAKE_SOURCE_DIR}/boot/bootloader.asm
    COMMENT "Assembling bootloader with NASM"
)
add_custom_target(bootloader_bin ALL DEPENDS ${CMAKE_BINARY_DIR}/bootloader.bin)

# Kernel sources (exclude bootloader.asm)
set(SOURCES
    kernel/main.cc
    kernel/arch/x86/isr.cc
    kernel/drivers/keyboard.cc
    kernel/drivers/vga.cc
    shell/shell.cpp
    lib/string.cc
    kernel/arch/x86/gdt.asm
    kernel/arch/x86/idt.asm
)

# Enable NASM assembler
enable_language(ASM_NASM)

# Kernel executable
add_executable(kernel.bin ${SOURCES})

# Make kernel depend on bootloader
add_dependencies(kernel.bin bootloader_bin)

# Include dirs
target_include_directories(kernel.bin PRIVATE ${CMAKE_SOURCE_DIR}/lib/include)

# Compile options for 32-bit, no pie
target_compile_options(kernel.bin PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-Wall -Wextra -nostdlib -ffreestanding -m32 -fno-pie>
    $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -nostdlib -ffreestanding -m32 -fno-pie>
)

# Linker options, 32-bit, no pie, use linker script
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/kernel.ld)
target_link_options(kernel.bin PRIVATE -no-pie -m32 -T ${LINKER_SCRIPT})

# NASM flags for assembly files (elf32)
set_source_files_properties(
    kernel/arch/x86/gdt.asm
    kernel/arch/x86/idt.asm
    PROPERTIES
    LANGUAGE ASM_NASM
    COMPILE_FLAGS "-f elf32"
)
