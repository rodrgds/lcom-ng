/** @file vbe.c
 * @brief file containing the function definitions to handle the vbe standard
 */
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
	mmap_t map;
	if (lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL) {
		printf("vbe_get_mode_info: lm_alloc failed!");
		return 1;
	}
	struct reg86u reg86;
	reg86.u.b.intno = VBE_BIOS_SERV; 			/* BIOS video services */
	reg86.u.b.ah = VBE_INV_AH_REG;   			/* invoke VBE function */
	reg86.u.b.al = VBE_GET_MODE_INFO;    		/* get Video Mode information function */
	reg86.u.w.cx = mode;						/* intended video mode */
	reg86.u.w.es = PB2BASE(map.phys);			/* set segment base */
	reg86.u.w.di = PB2OFF(map.phys);			/* set offset */
	if (sys_int86(&reg86) != OK) {
		printf("vbe_get_mode_info: sys_int86 failed!\n");
		lm_free(&map);
		return 1;
	}
	if (vbe_check_reply(reg86)) {
		lm_free(&map);
		return 1;
	}
	*vmi_p = *((vbe_mode_info_t *)map.virtual);
	lm_free(&map);
	return 0;
}

int vbe_set_mode(unsigned short mode) {
	struct reg86u reg86;
	reg86.u.b.intno = VBE_BIOS_SERV; 			/* BIOS video services */
	reg86.u.b.ah = VBE_INV_AH_REG;   			/* invoke VBE function */
	reg86.u.b.al = VBE_SET_VBE_MODE;    		/* set Video Mode function */
	reg86.u.w.bx = mode | VBE_BX_LINEAR;		/* set mode and set bit 14 to use a linear frame buffer model */
	if (sys_int86(&reg86) != OK) {
		printf("vbe_set_mode: sys_int86() failed!\n");
		return 1;
	}
	if (vbe_check_reply(reg86))	return 1;
	return 0;
}

int vbe_check_reply(struct reg86u reg86) {
	if (reg86.u.b.al != VBE_REP_AL_REG_SUP)
		printf("vbe_check_reply: VBE function is not supported!\n");
	if (reg86.u.b.ah == VBE_REP_AH_REG_FAIL) {
		printf("vbe_check_reply: VBE function call failed!\n");
		return 1;
	}
	if (reg86.u.b.ah == VBE_REP_AH_REG_HDW) {
		printf("vbe_check_reply: VBE function is not supported in current hardware configuration!\n");
		return 1;
	}
	if (reg86.u.b.ah == VBE_REP_AH_REG_IVM) {
		printf("vbe_check_reply: VBE function is invalid in current video mode!\n");
		return 1;
	}
	return 0;
}

int vbe_get_ctrl_info(vbe_info_block_t *vib_p, uint16_t **vbe_modes, unsigned *nbr_vbe_modes) {
	mmap_t map;
	if (lm_alloc(sizeof(vbe_info_block_t), &map) == NULL) {
		printf("vbe_get_ctrl_info: lm_alloc failed!");
		return 1;
	}
	struct reg86u reg86;
	reg86.u.b.intno = VBE_BIOS_SERV; 			/* BIOS video services */
	reg86.u.b.ah = VBE_INV_AH_REG;   			/* invoke VBE function */
	reg86.u.b.al = VBE_GET_CTRL_INFO;    		/* get Video Controller information function */
	reg86.u.w.es = PB2BASE(map.phys);			/* set segment base */
	reg86.u.w.di = PB2OFF(map.phys);			/* set offset */
	if (sys_int86(&reg86) != OK) {
		printf("vbe_get_ctrl_info: sys_int86 failed!\n");
		lm_free(&map);
		return 1;
	}
	if (vbe_check_reply(reg86)) {
		lm_free(&map);
		return 1;
	}
	*vib_p = *((vbe_info_block_t *)map.virtual);
	// Conversão linear -> físico
	void *address = (void *) (PB2OFF(vib_p->VideoModePtr) - PB2OFF(map.phys) + map.virtual);
	int16_t *modes = address;
	*nbr_vbe_modes = 0;
	unsigned i;
	while (*modes != -1) {
		*nbr_vbe_modes += 1;
		modes++;
	}
	/* doing malloc here but cannot do free and very likely will forget to do free later - bad!!! */
	/* not used in project */
	if ((*vbe_modes = malloc(*nbr_vbe_modes * sizeof(uint16_t))) == NULL) {
		printf("vbe_get_ctrl_info: malloc failed!\n");
		lm_free(&map);
		return 1;
	}
	modes = address;
	for (i = 0; i < *nbr_vbe_modes; i++) {
		(*vbe_modes)[i] = *modes;
		modes++;
	}
	lm_free(&map);
	return 0;
}

void print_controller_info(vbe_info_block_t *vib_p, uint16_t *vbe_modes, unsigned nbr_vbe_modes) {
	printf("\nVBE controller information\n");
	printf("\nVBE Controller Capabilities: 0x%x\n", vib_p->Capabilities);
	if (vib_p->Capabilities[0] & BIT(0))
		printf("DAC width is switchable to 8 bits per primary color\n");
	else printf("DAC is fixed width, with 6 bits per primary color\n");
	if (vib_p->Capabilities[0] & BIT(1))
		printf("Controller is not VGA compatible\n");
	else printf("Controller is VGA compatible\n");
	if (vib_p->Capabilities[0] & BIT(2))
		printf("When programming large blocks of information to the RAMDAC, use the blank bit in Function 0x09\n");
	else printf("Normal RAMDAC operation\n");
	printf("\nVBE Supported Modes: ");
	unsigned int i;
	for (i = 0; i < nbr_vbe_modes - 1; i++) {
		printf("0x%x, ", vbe_modes[i]);
	}
	if (nbr_vbe_modes > 0) printf("0x%x\n", vbe_modes[nbr_vbe_modes - 1]);
	printf("\nSize of VRAM Memory: %lu KB\n", vib_p->TotalMemory * 64);
}
