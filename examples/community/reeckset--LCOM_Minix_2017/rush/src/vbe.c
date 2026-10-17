#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"
#include "macros.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	phys_bytes buf;
	struct reg86u r;
	if(lm_init() == NULL) return 1; /* use liblm.a to initialize buf */
	mmap_t map;
	lm_alloc(sizeof(vbe_mode_info_t),&map);
	buf = map.phys;
	r.u.w.ax = 0x4F01; /* VBE get mode info */
	/* translate the buffer linear address to a far pointer */
	r.u.w.es = PB2BASE(buf); /* set a segment base */
	r.u.w.di = PB2OFF(buf); /* set the offset accordingly */
	r.u.w.cx = mode;
	r.u.b.intno = 0x10;
	if( sys_int86(&r) != OK ) { /* call BIOS */

	}

	*vmi_p = *((vbe_mode_info_t *)map.virtual);

	lm_free(&map);
  return 0;
}

int vg_get_vbe_info(vbe_info_t * vbeInfo) {
  phys_bytes buf;
  struct reg86u reg86;

  mmap_t mapInfo;
  if(lm_init() == NULL) return 1; //lm_init failed
  lm_alloc(sizeof(vbe_info_t), &mapInfo); // 1MB??
  buf = mapInfo.phys;

  reg86.u.b.intno = VBE_INTERFACE;
  reg86.u.w.ax = VBE_CALL_CMD; // VBE call, function 00 -- get VBE info
  reg86.u.w.es = PB2BASE(buf); // set a segment base
  reg86.u.w.di = PB2OFF(buf);  // set the offset accordingly

  printf("mpinfo: %p, buf: %p\n", mapInfo.virtual, buf);

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      lm_free(&mapInfo);
      return 1;
  } else

      *vbeInfo = *((vbe_info_t *) mapInfo.virtual);
      lm_free(&mapInfo);
      return 0;
}
