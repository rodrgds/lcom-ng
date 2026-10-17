#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"
#include "defs.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	mmap_t m_map;
	phys_bytes buf;
	struct reg86u r;

	if (lm_init() == NULL)
		return 1;

	if (lm_alloc(sizeof(vbe_mode_info_t), &m_map) == NULL)
		return 1;

	buf = m_map.phys;

	r.u.b.ah = VBE_CALL;
	r.u.b.al = GET_MODE_INFO;
	r.u.w.es = PB2BASE(buf);
	r.u.w.di = PB2OFF(buf);
	r.u.w.cx = mode;
	r.u.b.intno = VIDEO_CARD_INTERRUPT;

	if (sys_int86(&r) != OK) {
		return 1;

	}

	*vmi_p = *(vbe_mode_info_t *) m_map.virtual;

	lm_free(&m_map);

	return OK;

}

