cmake_minimum_required(VERSION 3.13)
project(GooberUEFI C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify compiler
# (Do not set CMAKE_C_COMPILER or CMAKE_CXX_COMPILER here; set them via the command line)

# Compiler flags as list separate each flag with a space, no semicolons
set(COMMON_FLAGS
    -target x86_64-pc-windows-gnu
    -ffreestanding
    -fpie
    -fno-exceptions
    -fno-rtti
    -fshort-wchar
    -fvisibility=hidden
    -mcmodel=small
)

add_compile_options(${COMMON_FLAGS})

set(LINKER_FLAGS
    -nostdlib
    -fuse-ld=lld
    -Wl,--entry=efi_main
    -Wl,--subsystem=10         # EFI_APPLICATION
    -Wl,--image-base,0x100000
    -Wl,--emit-relocs
    -Wl,--dll
    -Wl,--oformat=pei-x86-64   # PE/COFF output
)

set(CMAKE_SHARED_LINKER_FLAGS "-fuse-ld=lld")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")

include_directories(
    ${PROJECT_SOURCE_DIR}/edk2/MdePkg/Include
    ${PROJECT_SOURCE_DIR}/edk2/MdePkg/Include/Protocol
    ${PROJECT_SOURCE_DIR}/edk2/MdePkg/Include/IndustryStandard
    ${PROJECT_SOURCE_DIR}/edk2/MdePkg/Include/Base
    ${PROJECT_SOURCE_DIR}/edk2/MdePkg/Include/X64
    ${PROJECT_SOURCE_DIR}/edk2/MdePkg/Include/Library
)
set(CMAKE_EXE_LINKER_FLAGS "-target x86_64-efi -nostdlib -fuse-ld=lld -Wl,--entry=efi_main -Wl,-T,${CMAKE_SOURCE_DIR}/src/efi.ld")
set(CMAKE_LINKER lld-link)
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_LINKER> /subsystem:efi_application /entry:efi_main /out:<TARGET_FILE> <OBJECTS> <LINK_LIBRARIES>")

add_executable(BOOTX64.EFI
    src/main.cpp
    src/framebuffer.cpp
    src/gui_window.cpp
    src/input_mouse.cpp
    src/input_keyboard.cpp
    src/terminal.cpp
)
set_target_properties(BOOTX64.EFI PROPERTIES
  OUTPUT_NAME "BOOTX64"
  SUFFIX ".EFI"
)

target_link_options(BOOTX64.EFI PRIVATE ${LINKER_FLAGS})

# Ensure only clang is used
if(NOT "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  message(FATAL_ERROR "Clang is required as the C compiler!")
endif()
if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  message(FATAL_ERROR "Clang is required as the C++ compiler!")
endif()