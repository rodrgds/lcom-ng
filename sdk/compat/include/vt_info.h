#ifndef __VT_INFO_H
#define __VT_INFO_H
/* FEUP LCOM 2011 lab 1 libvt ABI. vram_size is a count of 16-bit words.
 * vm_map_phys continues to take a byte count, as in Minix. */
typedef struct {
  unsigned long vram_size;
  void *vram_base;
  unsigned scr_width;
  unsigned scr_lines;
} vt_info_t;
void vt_info_get(vt_info_t *info);
void vt_info_display(vt_info_t *info);
#endif
