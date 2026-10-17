#include "vbe.h"

#include <lcom/lcf.h>

// vbe 2.0 standard: https://www.phatcode.net/res/221/files/vbe20.pdf

int vbe_set_mode(uint16_t mode) {

  reg86_t r86;
  memset(&r86, 0, sizeof(r86));

  r86.intno = 0x10;

  r86.ah = VBE;
  r86.al = VBE_SETMODE;
  r86.bx = mode | BIT(14);

  if (sys_int86(&r86) != OK) {
    printf("vbe_set_mode(): bios call failed\n");
    return 1;
  }

  if (r86.ax != 0x4F) {
    printf("vbe_set_mode(): vbe call failed\n");
    return 2;
  }

  return 0;
}

int vbe_flip_buffer(uint32_t vres) {

  reg86_t r86;
  memset(&r86, 0, sizeof(r86));

  r86.intno = 0x10;

  r86.ah = VBE;
  r86.al = 0x07;
  r86.bx = 0x1; // get display start

  if (sys_int86(&r86) != OK) {
    printf("vbe_flip_buffer(): bios call failed\n");
    return 1;
  }

  uint16_t start = r86.dx;
  memset(&r86, 0, sizeof(r86));

  if (start == 0x0) { // at buffer 0
    r86.dx = vres;
  }
  //else { // at buffer 1
  // r86.dx = 0;
  //}

  r86.intno = 0x10;
  r86.ah = VBE;
  r86.al = 0x07;
  // r86.bx = 0x0;

  if (sys_int86(&r86) != OK) {
    printf("vbe_flip_buffer(): bios call failed (2)\n");
    return 1;
  }
  return 0;
}
