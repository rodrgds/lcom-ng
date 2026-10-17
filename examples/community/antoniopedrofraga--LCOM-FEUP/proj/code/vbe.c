#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
	mmap_t map;
	if(lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL) return 1;
	struct reg86u r;
	r.u.w.ax = 0x4F01; // VBE call, function 01 -- return vbe mode
	r.u.w.cx = mode;
	r.u.w.es = PB2BASE(map.phys);
	r.u.w.di = PB2OFF(map.phys);
	r.u.b.intno = 0x10;
	if( sys_int86(&r) != OK ) {
		printf("vbe_get_mode_info : sys_int86() failed \n");
		return 1;
	}
	*vmi_p = *(vbe_mode_info_t*)map.virtual;
	lm_free(&map);
	return 0;
}

