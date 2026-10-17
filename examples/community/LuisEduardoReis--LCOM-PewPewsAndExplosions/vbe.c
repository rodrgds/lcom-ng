#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
  
    phys_bytes buf;
    struct reg86u r;
    mmap_t map;
    vbe_mode_info_t * p;
    p = (vbe_mode_info_t *) lm_alloc(sizeof (vbe_mode_info_t),&map);
    buf = map.phys;

    r.u.w.ax = 0x4F01; /*    VBE get mode info */
    r.u.w.es = PB2BASE(buf); /*    set a segment base    */
    r.u.w.di = PB2OFF(buf); /*    set the offset accordingly    */
    r.u.w.cx = mode;
    r.u.b.intno = 0x10;
    if( sys_int86(&r) != OK ) { /*    call BIOS    */
        printf("Error retrieving Information\n");
        return 1;
    }

    *vmi_p = *p;

    lm_free(&map);

  return 0;
}

int vbe_set_palette(char *colors) {
	phys_bytes buf;
	struct reg86u r;
	mmap_t map;
	char * p;
	p = lm_alloc(256*4,&map);
	buf = map.phys;

	int i;
	for(i = 0; i < 256; i++) {
		p[i*4] =  (i>>2); //colors[i*3 +2];// B
		p[i*4+1] = 0; //colors[i*3 +1]; // G
		p[i*4+2] = 0; //colors[i*3]; // R
		p[i*4+3] = 0; //Alignment
	}

	    r.u.w.ax = 0x4F09; /*    VBE set/get palette data */
	    r.u.w.es = PB2BASE(buf); /*    set a segment base    */
	    r.u.w.di = PB2OFF(buf); /*    set the offset accordingly    */
	    r.u.w.bx = 0x00; /* Set Palette Data*/
	    r.u.w.cx = 256;
	    r.u.w.dx = 0;
	    r.u.b.intno = 0x10;
	    if( sys_int86(&r) != OK ) { /*    call BIOS    */
	        printf("Error retrieving Information\n");
	        return 1;
	    }

	    lm_free(&map);

	  return 0;

}

