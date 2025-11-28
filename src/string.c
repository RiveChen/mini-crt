#include <minicrt.h>

char *itoa(int n, char *str, int radix) {
  char digit[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char *p = str;

  if (str == NULL || radix < 2 || radix > 36) {
    return str;
  }

  // edge case: n == 0
  if (n == 0) {
    *p++ = '0';
    *p = '\0';
    return str;
  }

  // handle sign bit (only for 10-based)
  if (radix == 10 && n < 0) {
    *p++ = '-';
    // don't use n = -n, to avoid INT_MIN overflow
    // instead, use absolute value logic in the loop, or convert to unsigned
  }

  // record the start position of the number part (skip the sign)
  char *head = p;

  // use unsigned to solve INT_MIN problem
  unsigned int num = (unsigned int)n;
  if (radix == 10 && n < 0) {
    num = (unsigned int)(-(n + 1)) + 1; // 2's complement fix for INT_MIN
  }

  while (num != 0) {
    *p++ = digit[num % radix];
    num /= radix;
  }

  *p = '\0';

  char tmp;
  while (head < p) {
    tmp = *head;
    *head++ = *--p;
    *p = tmp;
  }

  return str; // 返回首地址
}
int strcmp(const char *src, const char *dst) {
  int ret = 0;
  while (!(ret = *(unsigned char *)src - *(unsigned char *)dst) && *dst)
    ++src, ++dst;
  if (ret < 0)
    ret = -1;
  else if (ret > 0)
    ret = 1;
  return (ret);
}

char *strcpy(char *dst, const char *src) {
  char *ret = dst;
  while (*src != '\0') {
    *dst++ = *src++;
  }
  *dst = '\0';
  return ret;
}

unsigned strlen(const char *str) {
  if (str == NULL) {
    return 0;
  }
  unsigned len = 0;
  while (*str != '\0') {
    len++;
    str++;
  }
  return len;
}