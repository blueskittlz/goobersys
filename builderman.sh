#!/bin/bash
set -e

# Build kernel
mkdir -p build
cd build
cmake ..
cmake --build . 
cd ..

# Build bootloader (adjust commands if needed)
nasm -f bin boot/bootloader.s -o boot/bootloader.bin

# Prepare ISO folder
ISO_DIR=iso
mkdir -p $ISO_DIR

# Copy bootloader and kernel to ISO folder
cp boot/bootloader.bin $ISO_DIR/bootloader.bin
cp build/kernel.bin $ISO_DIR/kernel.bin

# Create ISO image with isohybrid (make sure you have genisoimage or mkisofs installed)
# The bootloader must be the first 512 bytes for BIOS boot
genisoimage -o gooberos.iso -b bootloader.bin -no-emul-boot -boot-load-size 4 -boot-info-table $ISO_DIR

echo "Build complete: gooberos.iso created"
