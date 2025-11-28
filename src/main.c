void *malloc(unsigned size);
void free(void *ptr);

int main(int argc, char **argv) {
  char *p = (char *)malloc(100);
  if (!p)
    return 1;

  // write test (if no Crash, means memory is writable)
  for (int i = 0; i < 100; i++) {
    p[i] = (char)i;
  }

  free(p);
  return 0; // success
}