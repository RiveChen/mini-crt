#include <minicrt.h>

void print_str(const char *str) {
  unsigned len = strlen(str);
  fwrite(str, 1, len, stdout);
  fwrite("\n", 1, 1, stdout);
}

void print_label(const char *label) {
  fwrite(label, 1, strlen(label), stdout);
  fwrite(": ", 1, 2, stdout);
}

void test_itoa() {
  char buf[32];

  print_label("Test itoa(12345)");
  itoa(12345, buf, 10);
  print_str(buf); // 期望: 12345

  print_label("Test itoa(-9876)");
  itoa(-9876, buf, 10);
  print_str(buf); // 期望: -9876

  print_label("Test itoa(0)");
  itoa(0, buf, 10);
  print_str(buf); // 期望: 0

  print_label("Test itoa(255, 16)");
  itoa(255, buf, 16);
  print_str(buf); // 期望: FF
}

void test_strcmp() {
  print_label("Test strcmp(abc, abc)");
  int res = strcmp("abc", "abc");
  if (res == 0)
    print_str("Equal (Pass)");
  else
    print_str("Fail");

  print_label("Test strcmp(abc, abd)");
  res = strcmp("abc", "abd");
  if (res < 0)
    print_str("Less (Pass)");
  else
    print_str("Fail");
}

void test_strcpy() {
  char src[] = "Hello CRT";
  char dst[32];

  strcpy(dst, src);
  print_label("Test strcpy");
  print_str(dst); // 期望: Hello CRT
}

int main(int argc, char **argv) {

  print_str("=== MiniCRT String Test ===");

  test_itoa();
  test_strcmp();
  test_strcpy();

  print_str("=== Test Finished ===");

  return 0;
}