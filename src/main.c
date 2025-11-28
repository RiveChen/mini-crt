#include <minicrt.h>

int main(int argc, char **argv) {

  printf("=== MiniCRT Printf Test ===\n");
  printf("String test: %s\n", "Hello ARM64");
  printf("Integer test: %d\n", 12345);
  printf("Negative test: %d\n", -6789);
  printf("Hex test: 0x%x\n", 255);
  printf("Char test: %c\n", 'A');
  printf("Mixed: %s is %d years old.\n", "MiniCRT", 1);

  FILE *fp = fopen("log.txt", "w");
  if (fp) {
    fprintf(fp, "Log entry: argc=%d\n", argc);
    fprintf(fp, "Program name: %s\n", argv[0]);
    fclose(fp);
    printf("Log written to log.txt\n");
  }

  return 0;
}