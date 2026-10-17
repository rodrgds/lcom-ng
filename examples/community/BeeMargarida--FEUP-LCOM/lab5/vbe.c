#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
/*
	mmap_t m;
	struct reg86u r;
	if (lm_init() != 0){
		return 1;
	}
	if(lm_alloc(sizeof(vbe_mode_info_t),&m) == NULL){
		return 1;
	}
	r.u.w.ax = 0x4F01; // VBE get mode info
	//translate the buffer linear address to a far pointer
	r.u.w.es = PB2BASE(m.phys); // set a segment base
	r.u.w.di = PB2OFF(m.phys); // set the offset accordingly
	r.u.w.cx = mode;
	r.u.b.intno = 0x10;
	if( sys_int86(&r) != OK ) { // call BIOS
		printf("BIOS call failed\n");
		return 1;
	}
	*vmi_p = *(m.virtual);
	lm_free(&m);

	return 0;*/
}

int16_t * vbe_modes(vbe_info_block_t *vbe){
/*
	if(lm_init() != 0)
		return 0;

	struct reg86u r;

	mmap_t m;
	if(lm_alloc(sizeof(vbe_mode_info_t),&m) == NULL)
		return 0;

	r.u.w.es = PB2BASE(m.phys);
	r.u.w.di = PB2OFF(m.phys);

	if( sys_int86(&r) != OK )
	{
		printf("BIOS call failed\n");
		lm_free(&m);
		return 0;
	}

	*vbe = *(m.virtual);

	void *ptr = (((vbe->VideoModePtr & 0xF0000) >> 12) + (vbe->VideoModePtr & 0x0FFFF));

	lm_free(&m);
	int16_t *modes = ptr;

	return modes;
*/

}

