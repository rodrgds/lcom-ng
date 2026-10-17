#include "vbe.h"


#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)


int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
	struct reg86u reg86;
	mmap_t aux;

	lm_init();
	lm_alloc(sizeof(vbe_mode_info_t),&aux);

	reg86.u.b.intno= BIOS_VIDEOCARD;
	reg86.u.w.ax= VBE_MODE_INFO;
	reg86.u.w.es=PB2BASE(aux.phys);
	reg86.u.w.di=PB2OFF(aux.phys);
	reg86.u.w.cx=mode;


	if(sys_int86(&reg86) != OK)
	{
		printf("vbe_get_mode_info(): sys_int86() failed");
		return 1;
	}

	else if(reg86.u.w.ax == FUNC_CALL_FAILED)
	{
		printf("Function call failed");
		return 1;
	}
	else if(reg86.u.w.ax == FUNC_NOT_SUPP)
	{
		printf("Not supported in the current hardware configuration");
		return 1;
	}
	else if(reg86.u.w.ax == FUNC_INVALID)
	{
		printf("Function call invalid in the current mode");
		return 1;
	}

	*vmi_p=*((vbe_mode_info_t *)aux.virtual);
	lm_free(&aux);
	return 0;
}


int vbe_controller_info(VbeInfoBlock *info)
{
	struct reg86u reg86;
	mmap_t aux;

	lm_init();
	lm_alloc(sizeof(VbeInfoBlock),&aux);

	reg86.u.b.intno= BIOS_VIDEOCARD;
	reg86.u.w.ax= VBE_CTRL_INFO;
	reg86.u.w.es=PB2BASE(aux.phys);
	reg86.u.w.di=PB2OFF(aux.phys);

	info->VESASignature[0]='V';
	info->VESASignature[1]='B';
	info->VESASignature[2]='E';
	info->VESASignature[3]='2';

	if(sys_int86(&reg86) != OK)
	{
		printf("vbe_get_mode_info(): sys_int86() failed");
		return 1;
	}

	if(reg86.u.w.ax == FUNC_CALL_FAILED)
	{
		printf("Function call failed");
		return 1;
	}
	else if(reg86.u.w.ax == FUNC_NOT_SUPP)
	{
		printf("Not supported in the current hardware configuration");
		return 1;
	}
	else if(reg86.u.w.ax == FUNC_INVALID)
	{
		printf("Function call invalid in the current mode");
		return 1;
	}

	*info=*((VbeInfoBlock *)aux.virtual);
	lm_free(&aux);
	return 0;
}







