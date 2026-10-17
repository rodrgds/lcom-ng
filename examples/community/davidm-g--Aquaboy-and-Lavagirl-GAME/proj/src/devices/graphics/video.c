#include "video.h"

int set_graphic_mode(uint16_t mode) {
  reg86_t r86;
  memset(&r86, 0, sizeof(r86)); /* zero the structure */
  r86.intno = 0x10;             /* BIOS video services */
  r86.ah = 0x4F;                /* Set Video Mode function */
  r86.al = 0X02;                /* specify function, set VBE Mode */
  r86.bx = mode | BIT(14);
  if (sys_int86(&r86) != OK) {
    printf("\tset_graphic_mode(): sys_int86() failed \n");
    return 1;
  }
  return 0;
}

int set_text_mode() {
  reg86_t r86;
  memset(&r86, 0, sizeof(r86)); /* zero the structure */
  r86.intno = 0x10;             /* BIOS video services */
  r86.ah = 0x00;                /* Set Video Mode function */
  r86.al = 0x03;                /* specify function, set text mode */
  if (sys_int86(&r86) != OK) {
    printf("\tset_text_mode(): sys_int86() failed \n");
    return 1;
  }
  return 0;
}

void change_screen_vertical_retrace(int h) {
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));
  r86.intno = 0x10;
  r86.ax = 0x4F07;
  r86.bl = 0x80;
  r86.dx = h;

  if (sys_int86(&r86) != OK) {
    panic("\tchange_screen_vertical_retrace(): sys_int86() failed \n");
  }
}

void change_screen_artifacts(int h) {
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));
  r86.intno = 0x10;
  r86.ax = 0x4F07;
  r86.dx = h;

  if (sys_int86(&r86) != OK) {
    panic("\tchange_screen_artifacts(): sys_int86() failed \n");
  }
}
