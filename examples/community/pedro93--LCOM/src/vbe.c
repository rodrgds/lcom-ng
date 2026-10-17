#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14
#define VBE_GET_INFO_MODE 0x4F01
#define VBE_CHANGE_VIDEO_MODE 0x4F02
#define INT_VIDEO 0x10
#define BIT(n) (0x1 << (n))

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	struct reg86u reg86;
	mmap_t buf;
	mode |= BIT(LINEAR_MODEL_BIT);

	if(lm_init() == 0)
	{
		lm_alloc(256, &buf);

		reg86.u.b.intno = INT_VIDEO;
		reg86.u.w.ax = VBE_GET_INFO_MODE; /* VBE get mode info */
		/* translate the buffer linear address to a far pointer */
		reg86.u.w.es = PB2BASE(buf.phys); /* set a segment base */
		reg86.u.w.di = PB2OFF(buf.phys); /* set the offset accordingly */
		reg86.u.w.cx = mode;
		if( sys_int86(&reg86) == OK )
		{

			vbe_mode_info_t *temp;
			temp = buf.virtual;
			*vmi_p = *temp;

		}
		else
		{
			printf("\tvbe_get_mode_info(): sys_int86() failed \n");
			return 1;
		}

		lm_free(&buf);

	}
	else
	{
		printf("Error initializing first Megabyte of memory.\n");
		return 1;
	}
	return 0;
}

