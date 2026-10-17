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
	    mmap_t *mmap_t;
	    lm_init();
	    lm_alloc(sizeof(vbe_mode_info_t),mmap_t);

	    reg86.u.b.intno = 0x10; /* BIOS video services */

	    reg86.u.b.ah = 0x4F;    /* Set Video Mode function */
	    reg86.u.b.al = 0x01;    /* Return VBE Mode Information*/

	    /* translate the buffer linear address to a far pointer */
	    reg86.u.w.es = PB2BASE(vmi_p->PhysBasePtr); /* set a segment base */
	    reg86.u.w.di = PB2OFF(vmi_p->BitsPerPixel); /* set the offset accordingly */
	    reg86.u.w.cx = mode;

	    if( sys_int86(&reg86) != OK ) {
	        printf("\tvg_exit(): sys_int86() failed \n");
	        return -1;
	    } else
	        return 0;

	    lm_free(mmap_t);
}


