#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
    struct reg86u r;
    mmap_t map;

    if (lm_init())
        return 1;

    lm_alloc(sizeof(vbe_mode_info_t), &map);

    r.u.b.intno = 0x10;

    r.u.b.ah = 0x4F;
    r.u.b.al = 0x01;

    r.u.w.cx = mode | (0x1 << LINEAR_MODEL_BIT);

    r.u.w.es = PB2BASE(map.phys);
    r.u.w.di = PB2OFF(map.phys);

    if (sys_int86(&r))
        return 1;

    *vmi_p = *(vbe_mode_info_t*)map.virtual;

    lm_free(&map);

    return r.u.w.ax;
}
