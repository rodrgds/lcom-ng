#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
  
	void * addr = lm_init();
	if (addr == NULL) return 1;
	mmap_t map;
	lm_alloc(sizeof(vbe_mode_info_t), &map);
	//printf("vbe_get_mode_info(%u): map physical address - %x\n", mode, map.phys);

	struct reg86u r;
	r.u.w.ax = RETURN_VBE_MODE; /* VBE get mode info */
	/* translate the buffer linear address to a far pointer */
	r.u.w.es = PB2BASE(map.phys); /* set a segment base */
	r.u.w.di = PB2OFF(map.phys); /* set the offset accordingly */
	r.u.w.cx = mode;
	r.u.b.intno = 0x10;
	if( sys_int86(&r) != OK ){
		printf("vbe_get_mode_info(%u): sys_int86() failed \n", mode);
		return 1;
	}
	if (r.u.w.ax != 0x004F){
		printf("vbe_get_mode_info(%u): vbe function call failed \n", mode);
		return 1;
	}
	*vmi_p = *(vbe_mode_info_t*)map.virtual;

	lm_free(&map);
	return 0;
}

int vbe_get_controller_info(vbe_ctr_info_t *vci_p)
{
	void * addr = lm_init();
	if (addr == NULL) return 1;
	mmap_t map;
	lm_alloc(512, &map);

	struct reg86u r;
	int i;
	char signature[] = "vbe2";
	char * vbe_sign = map.virtual;
	for (i = 0; i < 4; i++)
	{
		*vbe_sign = signature[i];
		vbe_sign++;
	}
	r.u.w.ax = RETURN_VBE_CONTROL; /* VBE get controller info */
	/* translate the buffer linear address to a far pointer */
	r.u.w.es = PB2BASE(map.phys); /* set a segment base */
	r.u.w.di = PB2OFF(map.phys); /* set the offset accordingly */;
	r.u.b.intno = 0x10;
	if( sys_int86(&r) != OK ){
		printf("vbe_get_mode_info(): sys_int86() failed \n");
		return 1;
	}
	if (r.u.w.ax != 0x004F){
		printf("vbe_get_mode_info(): vbe function call failed \n");
		return 1;
	}
	*vci_p = *(vbe_ctr_info_t*)map.virtual;

	/*The Capabilities field indicates the support of specific features in the graphics environment. The bits are defined as follows:
	 * D0 = 0 DAC is fixed width, with 6 bits per primary color = 1 DAC width is switchable to 8 bits per primary color
	 * D1 = 0 Controller is VGA compatible = 1 Controller is not VGA compatible
	 * D2 = 0 Normal RAMDAC operation = 1 When programming large blocks of information to the RAMDAC, use the blank bit in Function 09h.
	 * D3-31 = Reserved
	 * */
	printf("\nCapabilities of graphics controller\n");
	if (vci_p->Capabilities[3] & 0x1) printf("- DAC width is switchable to 8 bits per primary color\n");
	else printf("- DAC is fixed width, with 6 bits per primary color\n");
	if (vci_p->Capabilities[3] & 0x2) printf("- Controller is not VGA compatible\n");
	else printf("- Controller is VGA compatible\n");
	if (vci_p->Capabilities[3] & 0x4) printf("- When programming large blocks of information to the RAMDAC, use the blank bit in Function 09h\n");
	else printf("- Normal RAMDAC operation\n");

	//printf("VideoModePtr: 0x%x\n", vci_p->VideoModePtr);
	short * ptr = ((vci_p->VideoModePtr) >>12 & 0x0F0000) + (vci_p->VideoModePtr & 0x0FFFF) + addr;
	//printf("VideoModePtr: 0x%x\n", ptr);
	printf("\nList of supported modes:\n");
	while(*ptr != -1){
		printf("0x%x; ", *ptr);
		ptr++;
	}
	printf("\n");

	printf("\nSize of VRAM: %dKB\n", vci_p->TotalMemory);

	lm_free(&map);

	return 0;
}
