#include <minicrt.h>

// TODO: move to string.c
static int strcmp(const char *src, const char *dst) {
  int ret = 0;
  while (!(ret = *(unsigned char *)src - *(unsigned char *)dst) && *dst)
    ++src, ++dst;
  if (ret < 0)
    ret = -1;
  else if (ret > 0)
    ret = 1;
  return (ret);
}

// system call number definition (from /include/uapi/asm-generic/unistd.h)
#define __NR_openat 56
#define __NR_close 57
#define __NR_lseek 62
#define __NR_read 63
#define __NR_write 64

// special constant for openat
#define AT_FDCWD -100

// generic system call macro (supports up to 4 arguments)
static long syscall(long n, long a1, long a2, long a3, long a4) {
  register long x8 __asm__("x8") = n;
  register long x0 __asm__("x0") = a1;
  register long x1 __asm__("x1") = a2;
  register long x2 __asm__("x2") = a3;
  register long x3 __asm__("x3") = a4;

  __asm__ volatile("svc #0"
                   : "+r"(x0)
                   : "r"(x8), "r"(x1), "r"(x2), "r"(x3)
                   : "memory");
  return x0;
}

static int open(const char *pathname, int flags, int mode) {
  // AArch64 uses openat(AT_FDCWD, pathname, flags, mode) to simulate open
  int fd = (int)syscall(__NR_openat, AT_FDCWD, (long)pathname, flags, mode);
  return fd;
}

static int read(int fd, void *buffer, unsigned long size) {
  return (int)syscall(__NR_read, fd, (long)buffer, size, 0);
}

static int write(int fd, const void *buffer, unsigned long size) {
  return (int)syscall(__NR_write, fd, (long)buffer, size, 0);
}

static int close(int fd) { return (int)syscall(__NR_close, fd, 0, 0, 0); }

static int lseek(int fd, int offset, int mode) {
  return (int)syscall(__NR_lseek, fd, offset, mode, 0);
}

int mini_crt_io_init() {
  return 1; // no special initialization needed on Linux
}

FILE *fopen(const char *filename, const char *mode) {
  int fd = -1;
  int flags = 0;
  // default permissions: rw-rw-rw- (0666)
  int access = 0666;

  // parse mode string
  if (strcmp(mode, "w") == 0)
    flags |= O_WRONLY | O_CREAT | O_TRUNC;

  if (strcmp(mode, "w+") == 0)
    flags |= O_RDWR | O_CREAT | O_TRUNC;

  if (strcmp(mode, "r") == 0)
    flags |= O_RDONLY;

  if (strcmp(mode, "r+") == 0)
    flags |= O_RDWR | O_CREAT;

  if (strcmp(mode, "a") == 0)
    flags |= O_WRONLY | O_CREAT | O_APPEND;

  if (strcmp(mode, "a+") == 0)
    flags |= O_RDWR | O_CREAT | O_APPEND;

  fd = open(filename, flags, access);
  return (FILE *)(long)fd; // cast int fd to FILE* pointer and return
}

int fread(void *buffer, int size, int count, FILE *stream) {
  int fd = (int)(long)stream;
  return read(fd, buffer, size * count);
}

int fwrite(const void *buffer, int size, int count, FILE *stream) {
  int fd = (int)(long)stream;
  return write(fd, buffer, size * count);
}

int fclose(FILE *fp) {
  int fd = (int)(long)fp;
  return close(fd);
}

int fseek(FILE *fp, int offset, int set) {
  int fd = (int)(long)fp;
  return lseek(fd, offset, set);
}