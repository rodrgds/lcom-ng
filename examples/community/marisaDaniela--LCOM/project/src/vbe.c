#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
	struct reg86u reg86;

	mmap_t map;

	if(lm_init() == NULL){
		printf(" Failed lm_init!\n");;
		return -1;
	}

	lm_alloc( sizeof( vbe_mode_info_t), &map);

	reg86.u.b.intno = 0x10;  // interrupt
	reg86.u.b.ah = 0x4F;  // Quando e invocado uma vbe function: ax = 0x4f
	reg86.u.b.al = 0x01;	// chamar a funcao Return vbe mode information

	reg86.u.w.cx = mode;
	reg86.u.w.es = PB2BASE(map.phys);
	reg86.u.w.di = PB2OFF(map.phys);

	if( sys_int86(&reg86) != OK ) { /*call BIOS */
		printf("\tget_vbe_mode_info():: sys_int86() failed \n");
		return 1;
	}

	*vmi_p = *(vbe_mode_info_t*) map.virtual;

	lm_free(&map);

	return 0;


}


