#!/bin/sh

# This script creates an ISO image for the GooberSys EFI application.
ISO_NAME="GooberSys.iso"
ISO_DIR="iso_root"
EFI_DIR="$ISO_DIR/EFI/BOOT"

# Clean and recreate ISO root
rm -rf "$ISO_DIR"
mkdir -p "$EFI_DIR"

# Copy GRUB EFI binary
cp /usr/lib/grub/x86_64-efi/monolithic/grubx64.efi "$EFI_DIR/BOOTX64.EFI"

# Copy your compiled kernel (make sure it exists!)
cp build/TARGET_FILE "$EFI_DIR/kernel.efi"

# Create GRUB config
mkdir -p "$ISO_DIR/boot/grub"
cat <<EOF > "$ISO_DIR/boot/grub/grub.cfg"
insmod fat
insmod part_gpt

set timeout=5
set default=0

menuentry "Goober OS Kernel" {
    chainloader /EFI/BOOT/kernel.efi
    boot
}
EOF

# Create the ISO using grub-mkrescue
grub-mkrescue -o "$ISO_NAME" "$ISO_DIR"

echo "ISO created: $ISO_NAME"
