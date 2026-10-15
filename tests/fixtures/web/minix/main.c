#include <lcom/lcf.h>

int common_value;
int read_common_value(void);

int(proj_main_loop)(int argc, char **argv) {
  (void)argc; (void)argv;
  common_value = 42;
  if (read_common_value() != 42) return 3;
  puts("legacy common:42");
  FILE *relative = fopen("assets/message.txt", "r");
  FILE *absolute = fopen("/home/lcom/labs/minix/assets/message.txt", "r");
  if (!relative || !absolute) return 4;
  char text[32] = {0};
  if (!fgets(text, sizeof(text), absolute)) return 5;
  fclose(relative); fclose(absolute);
  printf("legacy asset:%s", text);
  puts("legacy polling ready");
  for (;;) {
    uint32_t status, byte;
    if (sys_inb(0x64, &status) || !(status & 1)) continue;
    if (sys_inb(0x60, &byte)) return 2;
    printf("legacy:%02x\n", (unsigned)byte);
    if (byte == 1) return 0;
  }
}
int main(int argc, char **argv) {
  int result = lcf_start(argc, argv);
  lcf_cleanup();
  return result;
}
