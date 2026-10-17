#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/video.h>
#include <sys/mman.h>
#include <sys/types.h>

//#include "lmlib.h"
#include "video_gr.h"
#include "Utilities.h"
#include "vbe.h"

#define SET_TEXT_MODE 0x03
#define VIDEO_MODE 0x00

/* Constants for VBE 0x105 mode */

//=== === === VBE funcs === === ===
//AH
#define VBE_MODE 0x4F //
//FOR AH 0x4F (VBE MODE) -> AL
#define GET_VBE_CONTROLLER_INFO 0x00 //get vbe controller info -> could be used to detect supported modes
//#define GET_VBE_MODEINFO 0x01 //get vbe mode info
#define SET_VBE_MODE 0x02  //Set VBE mode
//#define GET_VBE_MODE 0x03 //Return current VBE Mode
//#define SAVE_RESTORE_STATE x04 // Save/Restore State
//... ... ...

//=== === === VBE MODE === === ===
//buffer
#define LINEAR_FRAMEBUFFER 0x4000 //Use linear/flat frame buffer model
//else use windowed frame buffer mode
//display memory
//#define DONT_CLEAR_DISPLAY_MEMORY 0x8000//Don't clear display memory
//else clear display memory



/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR	0xF0000000
//now used in header
//#define H_RES             1024
//#define V_RES		  768
#define BITS_PER_PIXEL	  8 //256 colors
#define BASE2PB(x) ( ( ((x) >> 16) & 0x0000FFFF ) << 4 )
#define OFF2PB(x) (x & 0x0000FFFF )
//#define addr(x) ((x << 4) )

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

void *vg_init(unsigned short mode)
{
	//exceptions
	if (mode < 0x100 || mode > 0x11B) //check if the mode selected actually exists
	{
		printf("\nvbe_get_mode_info:invalid mode");
		return NULL;
	}
	//=== set mode ===
	struct reg86u r;
	//r.u.w.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
	r.u.b.ah = VBE_MODE;
	r.u.b.al = SET_VBE_MODE;
	r.u.w.bx = LINEAR_FRAMEBUFFER|mode; // set bit 14: linear framebuffer
	r.u.b.intno = BIOS_VIDEOSERVICE;

	if( sys_int86(&r) != OK ) {
		printf("\ng_init: sys_int86() failed \n");
		return NULL;//1??? it is a pointer should i use 1 like in the pdf?
	}

	if (check_completion_status_value(r.u.w.ax)!=OK) {
		printf("\ng_init ax prob");
		return NULL;
	}
	DEBUG_STR("SET DONE");
	//=== get info ===
	vbe_mode_info_t vmi_p;
	/*mmap_t map;
		//phys_bytes buf;//to store the VBE Mode Info desired
		//if (lm_init())
		//{
		//	printf("\nvbe_get_mode_info:lm_init failed");
		//	return 1;
		//}//init lm in main
		void* virt = lm_alloc(sizeof(vbe_mode_info_t), &map);//???is this it???
		if (virt==NULL)
		{
			lm_free(&map);
			printf("\nvbe_get_mode_info:lm_alloc failed");
			return 1;
		}*/

	//vbe_mode_info_t vmi_p;
	if (vbe_get_mode_info(mode, &vmi_p)!=OK)//(vbe_mode_info_t*) map.phys) != OK)//vbe_get_mode not done yet
	{
		printf("\ng_init: vbe_get_mode_info() failed");
		//lm_free(&map);
		return NULL;
	}

	h_res = vmi_p.XResolution;//((vbe_mode_info_t*)virt)->XResolution;
	v_res = vmi_p.YResolution;//((vbe_mode_info_t*)virt)->YResolution;
	bits_per_pixel = vmi_p.BitsPerPixel;// ((vbe_mode_info_t*)virt)->BitsPerPixel;
	//printf("\n%x",vmi_p.MemoryModel)
	//=== map === //the same from lab1
	struct mem_range mr;
	mr.mr_base = vmi_p.PhysBasePtr;//VRAM_PHYS_ADDR;//vmi_p.PhysBasePtr;

	//16colors 4bits 	//256colors 8b
	//32K//(1:5:5:5:) 16b //	//64K//(5:6:5) 16b
	//16.8M (8:8:8) 24b
	unsigned short bytes_per_pixel;
	if (bits_per_pixel==4) bytes_per_pixel=1;
	else bytes_per_pixel = bits_per_pixel/8;

	unsigned int vram_size;
	//when using 16 colors a byte should contain 2 pixels???
	if (bits_per_pixel==4) vram_size = h_res * v_res * bytes_per_pixel/2;
	else vram_size = h_res * v_res * bytes_per_pixel;
	mr.mr_limit = mr.mr_base + vram_size;

	int res;
	if ((res=sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))!=OK)
	{
		printf("\ng_init:sys_privctl (ADD_MEM) failed: %d\n", res);
		return NULL;
	}

	video_mem = vm_map_phys(SELF, (void*)mr.mr_base, vram_size);

	if (video_mem == MAP_FAILED)
	{
		printf("\ng_init:vm_map_phys failed");
		return NULL;
	}

	return video_mem;
}
/*
void vg_info(){
	//=== get info ===
	void* adr;
	if ( (adr = lm_init() )== NULL)
	{
		printf("\nvbe_get_mode_info:lm_init failed");
		//return 1;
		return;
	} else printf("\nlm_init() successful");

	vbe_controller_info_t vmi_p;
	if (vbe_get_controller_info(&vmi_p)!=OK)
	{
		printf("\ng_init: vbe_get_controller_info() failed");
		//return NULL;
		return;
	}

	printf("VBE Version %d.%d\n",vmi_p.VbeVersion>>8,vmi_p.VbeVersion&0x00FF );

	DEBUG_STR("Passo 2:\n");
	printf("Capabilities: \n\n");
	if(vmi_p.Capabilities[0] & BIT(0))
		printf("DAC width is switchable to 8 bits per primary color.\n");
	else
		printf("DAC is fixed width,with 6 bits per primary color.");

	if(vmi_p.Capabilities[0] & BIT(1))
		printf("Controller VGA is compatible.\n");
	else
		printf("Controller VGA is not compatible.");

	if(vmi_p.Capabilities[0] & BIT(2))
			printf("Normal RAMDAC operation.\n");
		else
			printf("Use blank bit  in Function 09h when programming large blocks to the RAMDAC.\n");

	printf("\nSupported Modes:\n");

	short* modesAdr = adr+BASE2PB(vmi_p.VideoModePtr)+OFF2PB(vmi_p.VideoModePtr);
	//int control=10;
	while (*modesAdr!=-1)
	{
		printf("%x | ",*modesAdr);
		modesAdr++;
	}

	printf("\nMaximum amount of memory physically installed: %dKB.\n", 64*vmi_p.TotalMemory);

	//short *aux = vmi_p.OemSoftwareRev;

	//printf("OEM Revision Level of the VBE Software: %d.%d", aux,)
	printf("\nOEM Revision Level: %d.%d\n",vmi_p.OemSoftwareRev>>8,vmi_p.OemSoftwareRev&0x00FF );

}
*/
int vg_exit() {
  struct reg86u reg86;

  reg86.u.b.intno = BIOS_VIDEOSERVICE; /* BIOS video services */

  reg86.u.b.ah = VIDEO_MODE;    /* Set Video Mode function */
  reg86.u.b.al = SET_TEXT_MODE;    /* 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return 1;
  } else
      return 0;
}
