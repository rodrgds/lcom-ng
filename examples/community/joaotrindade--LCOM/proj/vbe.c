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
	struct reg86u r;
	r.u.w.ax = 0x4F01;
	//  translate the buffer linear
	r.u.w.es = PB2BASE(buf);
	r.u.w.di = PB2OFF(buf);
	r.u.w.cx = mode;
	r.u.b.intno = 0x10;

	if( sys_int86(&r) != OK ) {
		printf("Error sys_int86 call /n");
		return 1;
	}*/
  return 0;
}


