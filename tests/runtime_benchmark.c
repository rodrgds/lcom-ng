#include <lcom/lcom.h>
#include <lcom/vbe.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double milliseconds(void) {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  return now.tv_sec * 1000.0 + now.tv_nsec / 1000000.0;
}

int main(int argc, char **argv) {
  if (argc != 2 || lcom_init() != LCOM_OK) return 1;
  double start;
  if (strcmp(argv[1], "logs") == 0) {
    start = milliseconds();
    for (int i = 0; i < 20000; ++i)
      if (lcom_printf("Runtime console throughput: %d\n", i) != LCOM_OK) return 1;
    /* Include consumption of asynchronous console messages in the timing. */
    uint8_t status;
    if (lcom_port_read8(0x64, &status) != LCOM_OK) return 1;
  } else if (strcmp(argv[1], "ports") == 0) {
    for (int line = 0; line < 25; ++line)
      if (lcom_printf("Port polling benchmark, line %d\n", line) != LCOM_OK) return 1;
    start = milliseconds();
    for (int i = 0; i < 2000; ++i) {
      uint8_t status;
      if (lcom_port_read8(0x64, &status) != LCOM_OK) return 1;
    }
  } else {
    uint16_t mode = (uint16_t)strtoul(argv[1], NULL, 0);
    lcom_vbe_mode_info_t info;
    uint8_t *pixels;
    if (lcom_vbe_get_mode_info(mode, &info) != LCOM_OK ||
        lcom_vbe_set_mode(mode) != LCOM_OK ||
        lcom_phys_map(info.framebuffer_phys, info.framebuffer_size,
                      (void **)&pixels) != LCOM_OK) return 1;
    for (size_t i = 0; i < info.framebuffer_size; ++i) pixels[i] = (uint8_t)(i * 37);
    start = milliseconds();
    for (int i = 0; i < 60; ++i) {
      pixels[0] = (uint8_t)i;
      if (lcom_vbe_present() != LCOM_OK) return 1;
    }
  }
  double elapsed = milliseconds() - start;
  if (lcom_printf("benchmark_ms=%.3f\n", elapsed) != LCOM_OK) return 1;
  /* Console writes are asynchronous. Wait for the runtime to consume them. */
  uint8_t status;
  if (lcom_port_read8(0x64, &status) != LCOM_OK) return 1;
  lcom_exit();
  return 0;
}
