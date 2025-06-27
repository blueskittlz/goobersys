# GooberSys Build Tutorial

Welcome to the GooberSys build guide! Follow these steps to build and run GooberSys from source.

## Prerequisites

- **Operating System:** Ubuntu Linux (recommended, or use WSL with Ubuntu)
- **Dependencies:**
    - `clang` 
    - `make`
    - `qemu` (for emulation/testing)
    - `nasm` 
    - `git`
    - `llvm`

Install dependencies (Debian/Ubuntu example):

```sh
sudo apt update
sudo apt install build-essential qemu-system-x86 git clang llvm
```

## Clone the Repository

```sh
git clone https://github.com/blueskittlz/goobersys.git
cd goobersys
```

## Build the OS

```sh
mkdir build
cd build
cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ..
make
```

## Create a bootable ISO

```sh
./iso.sh
```

This will compile the kernel and create a bootable image (e.g., `goobersys.iso` or `goobersys.img`).

## Run the ISO in QEMU (or any emulator)

```sh
qemu-system-x86_64 -cdrom GooberSys.iso -bios /usr/share/ovmf/OVMF.fd
```

## Clean Build Files

```sh
rm -rf build/
rm -rf iso_root/
```

## Troubleshooting

- Ensure all dependencies are installed.
- Check the `CMakeLists.txt` for architecture-specific settings.
- Review build output for errors.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

---

Happy hacking with GooberSys!