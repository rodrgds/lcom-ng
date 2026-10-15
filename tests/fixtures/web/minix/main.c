#include <lcom/lcf.h>
#include <util.h>

int common_value;
int read_common_value(void);

int(proj_main_loop)(int argc, char **argv) {
  (void)argc; (void)argv;
  __uint16_t legacy_value = 42;
  _Static_assert(sizeof(__uint16_t) == 2, "Minix internal 16-bit alias");
  common_value = legacy_value;
  if (read_common_value() != 42) return 3;
  puts("legacy common:42");
  FILE *relative = fopen("assets/message.txt", "r");
  FILE *absolute = fopen("/home/lcom/labs/minix/assets/message.txt", "r");
  if (!relative || !absolute) return 4;
  char text[32] = {0};
  if (!fgets(text, sizeof(text), absolute)) return 5;
  fclose(relative); fclose(absolute);
  printf("legacy asset:%s", text);
  setenv("LCOM_COMPAT_PATH_ALIAS_COUNT", "1", 1);
  setenv("LCOM_COMPAT_PATH_ALIAS_0_FROM", "/legacy/save", 1);
  setenv("LCOM_COMPAT_PATH_ALIAS_0_TO", "minix/assets", 1);
  FILE *saved = fopen("/legacy/save/record.txt", "w");
  FILE *pending = fopen("/legacy/save/pending.txt", "w");
  if (!saved || !pending) return 6;
  fputs("previous\n", saved); fputs("winner\n", pending);
  fclose(saved); fclose(pending);
  if (remove("/legacy/save/record.txt") ||
      rename("/legacy/save/pending.txt", "/legacy/save/record.txt")) return 7;
  saved = fopen("/legacy/save/record.txt", "r");
  memset(text, 0, sizeof(text));
  if (!saved || !fgets(text, sizeof(text), saved) || strcmp(text, "winner\n")) return 8;
  fclose(saved);
  printf("legacy save:%s", text);
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
