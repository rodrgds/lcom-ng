#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_controller_info (vbe_info_block *vbe_info) {

	mmap_t mem_map;
	struct reg86u r;

	lm_init();

	lm_alloc(sizeof(vbe_info_block), &mem_map);
	if ( & mem_map == NULL) {
		printf("lm_alloc: failed to allocate a memory.\n");
		return 1;
	}

	phys_bytes mem_copy = mem_map.phys;

	r.u.b.ah = CALL_VBE;
	r.u.b.al = 0x00;
	r.u.w.es = PB2BASE(mem_copy);
	r.u.w.di = PB2OFF(mem_copy);
	r.u.b.intno = INTERRUPT_VBE;

	if(sys_int86(&r) != OK) {
		return 1;
	}

	*vbe_info = *(vbe_info_block*) mem_map.virtual;	//Saving on the allocated memory pointer
	lm_free(&mem_map);

	return 0;
}


int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	mmap_t mem_map;
	struct reg86u r;

	lm_init();

	lm_alloc(sizeof(vbe_mode_info_t), &mem_map);
	if (& mem_map == NULL) {
		printf("lm_alloc: failed to allocate memory.\n");
		return 1;
	}

	phys_bytes mem_copy = mem_map.phys;

	r.u.b.ah = CALL_VBE;
	r.u.b.al = 0x01; //TODO make this a macro
	r.u.w.es = PB2BASE(mem_copy);
	r.u.w.di = PB2OFF(mem_copy);
	r.u.w.cx = mode;
	r.u.b.intno = INTERRUPT_VBE;

	if (sys_int86(&r) != OK) {
		return 1;
	}

	*vmi_p = *(vbe_mode_info_t*) mem_map.virtual;
	lm_free(&mem_map);

	return 0;
}


