#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"
#include "Utilities.h"


#define  GET_VBE_MODEINFO 0x4F01//get vbe mode info
#define  GET_VBE_CONTROLLERINFO 0x4F00
/*AX completion status value*/
#define CSV_FAILED 					0x01	//AH Function call failed
#define CSV_NOT_HARDWARESUPPORTED	0x02	//AH Function is not supported in current HW configuration
#define CSV_INVALID 				0x03	//AH Function is invalid in current video mode
#define  CSV_SUPPORTED				0x4F    //AL Function is supported
/*=== === === === === === ===*/
#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)


int check_completion_status_value(unsigned short ax)
{
	short ret=0;
	unsigned char al = (unsigned char) (ax & 0x00FF) ;
	unsigned char ah = (unsigned char) (ax>>8) ;
	if (al!=CSV_SUPPORTED){printf("\ncheck_completion_status_value:\nFunction not supported"); ret=1;}
	if (ah==CSV_FAILED) {printf("\ncheck_completion_status_value:\nFunction call failed"); ret=1;}
	if (ah==CSV_NOT_HARDWARESUPPORTED) {printf("\ncheck_completion_status_value:\nFunction is not supported in current HW configuration");ret=1;}
	if (ah==CSV_INVALID) {printf("\ncheck_completion_status_value:\nFunction is invalid in current video mode");ret=1;}
	return ret;
}

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	struct reg86u r;
	mmap_t map;

	void* virt = lm_alloc(sizeof(vbe_mode_info_t), &map);
	if (virt==NULL)
	{
		//lm_free(&map); //
		printf("\nvbe_get_mode_info:lm_alloc failed");
		return 1;
	}

	//=== GET INFO ===
	r.u.w.ax = GET_VBE_MODEINFO;
	r.u.w.cx = mode ;
	r.u.b.intno = BIOS_VIDEOSERVICE;
	//Pointer to ModeInfoBlock
	r.u.w.es = PB2BASE(map.phys);  //(phys_bytes) vmi_p);
	r.u.w.di = PB2OFF(map.phys);   //(phys_bytes) vmi_p);

	if( sys_int86(&r) != OK ) {
		printf("\nvbe_get_mode_info: sys_int86() failed \n");
		return 1;
	}
	DEBUG_STR("get mode finished");
	if (check_completion_status_value(r.u.w.ax)!=OK) {
		printf("\ngetmode ax prob");
		//return 1;
		lm_free(&map); return 1;//if failed clean memory anyway
	}

	//*vmi_p = *virt;//same thing as below???
	*vmi_p = *(vbe_mode_info_t*)map.virtual;//save virtual to be used in Minix

	lm_free(&map);

	return 0;
}

int vbe_get_controller_info(vbe_controller_info_t *vmi_p){
	struct reg86u r;
	mmap_t map;

	if (lm_init() == NULL)
	{
		printf("\nvbe_get_mode_info:lm_init failed");
		return 1;
	} else printf("\nlm_init() successful");

	void* virt = lm_alloc(sizeof(vbe_controller_info_t), &map);
	if (virt==NULL)
	{
		printf("\nvbe_get_controller_info:lm_alloc failed");
		return 1;
	}

	vmi_p->VbeSignature[0]='V';
	vmi_p->VbeSignature[1]='B';
	vmi_p->VbeSignature[2]='E';
	vmi_p->VbeSignature[3]='2';

	//=== GET INFO ===
	r.u.w.ax = GET_VBE_CONTROLLERINFO;
	r.u.b.intno = BIOS_VIDEOSERVICE;

	//Pointer to VbeInfoBlock
	r.u.w.es = PB2BASE(map.phys);
	r.u.w.di = PB2OFF(map.phys);



	if( sys_int86(&r) != OK ) {
		printf("\nvbe_get_controller_info: sys_int86() failed \n");
		return 1;
	}
	DEBUG_STR("get mode finished");
	if (check_completion_status_value(r.u.w.ax)!=OK) {
		printf("\ngetmode ax prob");
		lm_free(&map);
		return 1;//if failed clean memory anyway
	}

	*vmi_p = *(vbe_controller_info_t*)map.virtual;//save virtual to be used in Minix
	lm_free(&map);
	return 0;




}
