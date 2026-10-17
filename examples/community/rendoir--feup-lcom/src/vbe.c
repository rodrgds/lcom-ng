/**
  @file vbe.c
  @author Daniel Marques
  @author João Almeida

  Imported from lab5
*/


#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include "lmlib.h"
#include "vbe.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
  mmap_t map;
  struct reg86u r;
  lm_init();
  if (lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL){
    printf("LM_ALLOC FAILED! \n");
    return 1;
  }

  r.u.w.ax = GET_MODE_INFO;
  r.u.w.cx = mode;
  r.u.w.es = PB2BASE(map.phys);
  r.u.w.di = PB2OFF(map.phys);
  r.u.b.intno = 0x10;

  if (sys_int86(&r) != OK){
    printf("VBE GET MODE FAILED! \n");
    return 1;
  }
  *vmi_p = *(vbe_mode_info_t *)map.virtual;
  lm_free(&map);
  return 0;
}
