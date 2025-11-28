#include <minicrt.h>

int main(int argc, char **argv) {
  char *msg = "Hello from MiniCRT ARM64!\n";
  fwrite(msg, 1, 26, stdout);

  FILE *fp = fopen("test.txt", "w");
  if (fp != 0) {
    char *file_msg = "Writing to file works.\n";
    fwrite(file_msg, 1, 23, fp);
    fclose(fp);

    char *success = "File written.\n";
    fwrite(success, 1, 14, stdout);
  } else {
    char *fail = "Open failed.\n";
    fwrite(fail, 1, 13, stdout);
  }

  return 0;
}