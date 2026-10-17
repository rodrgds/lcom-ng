#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define VBE_INFO_BLOCK 256
#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)


void vbe_memcpy(void * buffer, vbe_mode_info_t * dest);

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	struct reg86u reg86;
	mmap_t addr;

	lm_alloc(VBE_INFO_BLOCK , &addr);


	reg86.u.b.intno = 0x10;
	reg86.u.b.ah = 0x4F;
	reg86.u.b.al = 0x01;
	reg86.u.w.cx = mode;
	reg86.u.w.es = PB2BASE(addr.phys);
	reg86.u.w.di = PB2OFF(addr.phys);

	if(sys_int86(&reg86) != OK)
	{
		printf("return_vbe_mode: sys_int86() failed\n");
		return 1;
	}


	vmi_p->XResolution = ((vbe_mode_info_t*)(addr.virtual))->XResolution;
	vmi_p->YResolution = ((vbe_mode_info_t*)(addr.virtual))->YResolution;
	vmi_p->BitsPerPixel = ((vbe_mode_info_t*)(addr.virtual))->BitsPerPixel;
	vmi_p->PhysBasePtr = ((vbe_mode_info_t*)(addr.virtual))->PhysBasePtr;


	printf("%d", vmi_p->XResolution);

	lm_free(&addr);


  /* To be completed */
  
  return 1;
}

