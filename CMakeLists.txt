cmake_minimum_required(VERSION 3.14)
project(MiniCRT LANGUAGES C ASM)

# -ffreestanding: disable standard library features
# -fno-builtin: disable builtin functions
# -nostdinc: disable standard C library includes
add_compile_options(-ffreestanding -fno-builtin -nostdinc)

# -nostdlib: disable standard library
# -nostartfiles: disable startup files
add_link_options(-nostdlib -nostartfiles)

# SYSTEM: include directories are searched in the system directories first
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(main src/entry.c src/arch/aarch64/start.s src/main.c)