#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

#define VBE_BLOCK_SIZE sizeof(vbe_mode_info_t)
#define BIT(n) (0x1 << (n))
#define BIOS_VIDEO 0x10
#define VBE 0x4F
#define RET_VBE 0x01
#define RET_INFO 0x00

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
	struct reg86u reg86;
	mmap_t map;
	lm_init();
	lm_alloc(VBE_BLOCK_SIZE, &map);
	reg86.u.b.ah = VBE;
	reg86.u.b.al = RET_VBE;
	reg86.u.w.es = PB2BASE(map.phys);
	reg86.u.w.di = PB2OFF(map.phys);
	reg86.u.w.cx = mode | BIT(LINEAR_MODEL_BIT);
	reg86.u.b.intno = BIOS_VIDEO;
	sys_int86(&reg86);
	*vmi_p = *(vbe_mode_info_t*) map.virtual;
	lm_free(&map);
	return reg86.u.w.ax;
}

int vbe_get_controler_info(phys_bytes address) {

        struct reg86u reg86;
        reg86.u.b.ah = VBE;
        reg86.u.b.al = RET_INFO;
        reg86.u.w.es = PB2BASE(address);
        reg86.u.w.di = PB2OFF(address);
        reg86.u.b.intno = BIOS_VIDEO;
        sys_int86(&reg86);
        if (reg86.u.b.ah == 0x01 || reg86.u.b.ah == 0x02 || reg86.u.b.ah == 0x03){
                printf("Function vbe_get_controler_info() failed \n");
                return 1;
        }
        return 0;
}

