// entry.c - generic CRT startup logic
#include <stdint.h>

// declare external symbols
extern int main(int argc, char **argv, char **envp);

extern int crt_heap_init(void);

// system call wrapper (can be moved to syscalls.c later)
static void sys_exit(int code) {
  register int32_t w0 __asm__("w0") = code;
  register int64_t x8 __asm__("x8") = 93; // 93 is AArch64's exit, x86 is 60
  __asm__ volatile("svc #0" ::"r"(w0), "r"(x8) : "memory");
  __builtin_unreachable();
}

// fatal error handler
static void crt_fatal_error(const char *msg) {
  // printf("Fatal error: %s\n", msg);
  sys_exit(1);
}

// startup logic (generic C code)
// note: the parameter sp comes from start.s's mov x0, sp
void _crt_startup(uint64_t *sp) {
  // get arguments
  int64_t argc = (int64_t)*sp;
  char **argv = (char **)(sp + 1);
  char **envp = argv + argc + 1;

  // heap init
  crt_heap_init();
  // TODO: I/O init

  // call main function
  int ret = main((int)argc, argv, envp);

  // exit program
  sys_exit(ret);
}