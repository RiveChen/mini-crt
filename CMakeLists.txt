cmake_minimum_required(VERSION 3.14)
project(MiniCRT LANGUAGES C ASM)

set(CRT_SOURCES
    src/arch/aarch64/start.s
    src/entry.c
    src/main.c
    src/malloc.c
    src/stdio.c
    src/string.c
)

add_executable(main ${CRT_SOURCES})

target_include_directories(main PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# -ffreestanding: don't rely on libc
# -nostdinc: don't use standard C library
# -fno-builtin: don't use built-in functions
# -fno-stack-protector: don't use stack protector
target_compile_options(main PRIVATE
    $<$<COMPILE_LANGUAGE:C>:
        -std=c11
        -ffreestanding
        -nostdinc
        -fno-builtin
        -fno-stack-protector
    >
)

target_link_options(main PRIVATE
    -static
    -nostdlib
    -nostartfiles
    "LINKER:-e,_start"
    "LINKER:-Map=output.map"
)