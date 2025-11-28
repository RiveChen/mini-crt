// start.s - AArch64 entry point

.section .text //define code section
.global _start //export _start symbol, let linker see it
.type _start, %function //mark symbol type as function (ELF metadata)

.extern _crt_startup //declare external symbol (C function)

_start:
    // * 1. terminate stack unwinding (Stack Unwinding Terminator)
    // clear FP (x29) and LR (x30)
    // tell debugger (GDB): "here is the end of the call stack, don't go up anymore"
    mov x29, #0
    mov x30, #0

    // * 2. pass arguments
    // when kernel starts, SP points to argc
    // we copy the value of SP to x0, as the first argument of _crt_startup(uint64_t *sp)
    mov x0, sp

    // * 3. jump to C world
    // use b (Branch) to jump, don't save return address, because we don't want to return
    b _crt_startup

    // defensive code: theoretically never executed here
    .size _start, . - _start
