/* The LCF calls student implementations with the command-line contract. */
#include <lcom/lcf.h>
#include "compat_local_timer.h"
#define CHECK(c) do { if (!(c)) { fprintf(stderr, "LCF check failed at line %d: %s\n", __LINE__, #c); lcf_cleanup(); return 1; } } while (0)
static int calls;
int rtc_test_conf(void) { ++calls; return 0; }
int video_test_xpm(xpm_map_t map, uint16_t x, uint16_t y) {
  xpm_image_t image;
  uint8_t *pixels = xpm_load(map, XPM_INDEXED, &image);
  if (!pixels || image.width != 32 || image.height != 13 || x != 12 || y != 34) return 91;
  /* pic1's first green pixel is column 14 of its second row. */
  if (pixels[32 + 14] != 2 || pixels[0] != 0) return 92;
  free(pixels); ++calls; return 0;
}
int video_test_move(xpm_map_t map, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t rate) {
  xpm_image_t image;
  uint8_t *pixels = xpm_load(map, XPM_INDEXED, &image);
  if (!pixels || image.width != 64 || image.height != 64 || xi != 10 || yi != 20 || xf != 30 || yf != 40 || speed != -3 || rate != 60) return 93;
  free(pixels); ++calls; return 0;
}
int timer_test_read_config(uint8_t timer, enum timer_status_field field) {
  if (timer != 2 || field != tsf_mode) return 94;
  ++calls; return 0;
}
static int invoke(int argc, char **argv) { return lcf_start(argc, argv); }
int main(void) {
  /* Modern LCF entry points can retain older nested BIOS register drivers. */
  mmap_t bios_map;
  CHECK(lm_init(true) && lm_alloc(256, &bios_map));
  struct reg86u old_registers = {0};
  old_registers.u.b.intno=0x10; old_registers.u.w.ax=0x4f01;
  old_registers.u.w.cx=0x115; old_registers.u.w.es=PB2BASE(bios_map.phys);
  old_registers.u.w.di=PB2OFF(bios_map.phys);
  CHECK(sys_int86(&old_registers)==OK && old_registers.u.w.ax==0x004f);
  unsigned char *mode_bytes=bios_map.virt;
  CHECK((mode_bytes[18] | mode_bytes[19]<<8)==800 && mode_bytes[25]==24);
  reg86_t current_registers={0};
  current_registers.intno=0x10; current_registers.ax=0x4f01;
  current_registers.cx=0x117; current_registers.es=PB2BASE(bios_map.phys);
  current_registers.di=PB2OFF(bios_map.phys);
  CHECK(sys_int86(&current_registers)==OK && current_registers.ax==0x004f);
  CHECK((mode_bytes[18] | mode_bytes[19]<<8)==1024 && mode_bytes[25]==16);
  CHECK(lm_free(&bios_map));

  CHECK(proj_demo(0x115, true, true, 0) == OK);
  CHECK(proj_demo(0xffff, false, false, 0) == EINVAL);
  char *xpm[] = {"lab5", "video_test_xpm", "pic1", "12", "34"};
  CHECK(invoke(5, xpm) == 0 && calls == 1);
  char *move[] = {"lab5", "video_test_move", "penguin", "10", "20", "30", "40", "-3", "60"};
  CHECK(invoke(9, move) == 0 && calls == 2);
  move[7] = "-32769"; CHECK(invoke(9, move) == EINVAL && calls == 2);
  move[7] = "32768"; CHECK(invoke(9, move) == EINVAL && calls == 2);
  move[7] = "-3x"; CHECK(invoke(9, move) == EINVAL && calls == 2);
  move[7] = "-3"; move[8] = "256"; CHECK(invoke(9, move) == EINVAL && calls == 2);
  xpm[2] = "unknown"; CHECK(invoke(5, xpm) == EINVAL && calls == 2);
  xpm[2] = "pic1"; xpm[3] = "65536"; CHECK(invoke(5, xpm) == EINVAL && calls == 2);
  char *timer[] = {"lab2", "timer_test_read_config", "2", "2"};
  CHECK(invoke(4, timer) == 0 && calls == 3);
  timer[2] = "3"; CHECK(invoke(4, timer) == EINVAL && calls == 3);
  timer[2] = " 2"; CHECK(invoke(4, timer) == EINVAL && calls == 3);
  char *rtc[] = {"lab6", "rtc_test_conf"};
  CHECK(invoke(2, rtc) == 0 && calls == 4);
  CHECK(invoke(1, rtc) == ENOSYS);
  char *missing[] = {"lab3", "kbd_test_scan"};
  CHECK(invoke(2, missing) == ENOSYS);
  CHECK(lcf_cleanup() == OK); puts("LCF student callback dispatch passed"); return 0;
}
