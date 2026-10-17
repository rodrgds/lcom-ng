#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)
#define VIDEO_CARD                   0x10
#define GET_MODE_VBE               0x4F01

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	struct reg86u reg;
	reg.u.w.ax = GET_MODE_VBE;
	reg.u.w.cx = mode;
	reg.u.b.intno = VIDEO_CARD;

	if (lm_init() == NULL) {
		printf("Failed lm_init().\n");
		return 1;
	}

	mmap_t vbe_map;
	if (lm_alloc(sizeof(vbe_mode_info_t), &vbe_map) == NULL) {
		printf("Failed lm_alloc().\n");
		return 1;
	}

	reg.u.w.es = PB2BASE(vbe_map.phys);
	reg.u.w.di = PB2OFF(vbe_map.phys);

	if (sys_int86(&reg) != OK) {
		printf("Failed sys_int86().\n");
		return 1;
	}

	*vmi_p = *(vbe_mode_info_t*) vbe_map.virtual;

	lm_free(&vbe_map);

	return 0;
}

