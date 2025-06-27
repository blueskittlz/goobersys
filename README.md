# GooberSys Build Tutorial

Welcome to the GooberSys build guide! Follow these steps to build and run GooberSys from source.

## Prerequisites

- **Operating System:** Linux (recommended)
- **Dependencies:**
    - `gcc` (or another cross-compiler for your target architecture)
    - `make`
    - `qemu` (for emulation/testing)
    - `nasm` (if using assembly)
    - `git`

Install dependencies (Debian/Ubuntu example):

```sh
sudo apt update
sudo apt install build-essential qemu-system-x86 nasm git
```

## Clone the Repository

```sh
git clone https://github.com/yourusername/goobersys.git
cd goobersys
```

## Build the OS

```sh
make
```

This will compile the kernel and create a bootable image (e.g., `goobersys.iso` or `goobersys.img`).

## Run in QEMU

```sh
qemu-system-x86_64 -cdrom goobersys.iso
```
or, if you have a raw image:
```sh
qemu-system-x86_64 -drive format=raw,file=goobersys.img
```

## Clean Build Files

```sh
make clean
```

## Troubleshooting

- Ensure all dependencies are installed.
- Check the `CMakeLists.txt` for architecture-specific settings.
- Review build output for errors.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

---

Happy hacking with GooberSys!
