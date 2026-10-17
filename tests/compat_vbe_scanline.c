/* BIOS virtual scanlines and horizontal page flipping through the real SDK. */
#include <lcom/lcf.h>
#include <minix/vm.h>
#include <lcom/memory_map.h>
#include <lcom/vbe_modes.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr, "scanline check line %d: %s\n", __LINE__, #x); return 1; } } while (0)
static reg86_t bios(uint16_t function, uint8_t operation, uint16_t value) {
  reg86_t r = {0}; r.intno = 0x10; r.ax = function; r.bl = operation; r.cx = value; return r;
}
int main(int argc, char **argv) {
  sef_startup();
  reg86_t r = bios(0x4f02, 0, 0); r.bx = 0x4115;
  CHECK(sys_int86(&r) == OK && r.ax == 0x004f);
  r = bios(0x4f06, 1, 0);
  CHECK(sys_int86(&r) == OK && r.bx == 2400 && r.cx == 800 && r.dx == 6990);
  r = bios(0x4f06, 0, 1600);
  CHECK(sys_int86(&r) == OK && r.ax == 0x004f && r.bx == 4800 && r.cx == 1600 && r.dx == 3495);
  r = bios(0x4f06, 2, 4801);
  CHECK(sys_int86(&r) == OK && r.bx == 4801 && r.cx == 1600 && r.dx == 3494);
  r = bios(0x4f06, 0, 799); CHECK(sys_int86(&r) != OK && r.ax == 0x014f);
  r = bios(0x4f06, 0, 65535); CHECK(sys_int86(&r) != OK && r.ax == 0x014f);
  r = bios(0x4f06, 3, 0); CHECK(sys_int86(&r) != OK && r.ax == 0x014f);
  r = bios(0x4f06, 1, 0);
  CHECK(sys_int86(&r) == OK && r.bx == 4801 && r.cx == 1600 && r.dx == 3494);
  r = bios(0x4f07, 0, 800); r.dx = 2894; CHECK(sys_int86(&r) == OK);
  r = bios(0x4f07, 0, 801); CHECK(sys_int86(&r) != OK);
  r = bios(0x4f07, 0, 800); r.dx = 2895; CHECK(sys_int86(&r) != OK);
  /* A mode switch resets both virtual pitch and display origin. */
  r = bios(0x4f02, 0, 0); r.bx = 0x4115; CHECK(sys_int86(&r) == OK);
  r = bios(0x4f07, 1, 0); CHECK(sys_int86(&r) == OK && r.cx == 0 && r.dx == 0);
  r = bios(0x4f06, 1, 0); CHECK(sys_int86(&r) == OK && r.bx == 2400 && r.cx == 800);
  r = bios(0x4f06, 2, 4801); CHECK(sys_int86(&r) == OK);
  uint8_t *vram = vm_map_phys(SELF, (void *)(uintptr_t)LCOM_VBE_DEFAULT_PHYS, LCOM_VBE_VRAM_BYTES);
  CHECK(vram != MAP_FAILED);
  for (unsigned y = 0; y < 600; ++y) {
    for (unsigned x = 0; x < 1600; ++x) {
      size_t at = y * 4801u + x * 3u;
      vram[at] = x < 800 ? 0 : 0x33;
      vram[at + 1] = x < 800 ? 0 : (y % 2 ? 0xaa : 0xee);
      vram[at + 2] = x < 800 ? 0xbb : 0;
    }
    vram[y * 4801u + 4800] = 0xff; /* Padding must never leak into pixels. */
  }
  uint16_t origin = argc == 2 && !strcmp(argv[1], "left") ? 0 : 800;
  r = bios(0x4f07, 0x80, origin); CHECK(sys_int86(&r) == OK);
  r = bios(0x4f07, 1, 0); CHECK(sys_int86(&r) == OK && r.cx == origin && r.dx == 0);
  CHECK(lcom_vbe_present() == LCOM_OK);
  puts("VBE virtual scanline contract passed");
  lcf_cleanup(); return 0;
}
