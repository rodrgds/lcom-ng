#include <lcom/lcf.h>


#include "vbe.h"

void * video_mem;
void * video_buffer;
uint16_t hres; /*XResolution*/
uint16_t vres; /*YResolution*/
int bitsPerPixel; /*color depth*/

/*Retrieve the info for the specified mode*/
vbe_mode_info_t info;


void * (vg_init)(uint16_t mode){

        int i;
	struct minix_mem_range mr; /*physical memory range*/

	/*Allow memory mapping*/
	mr.mr_base = BASE_ADDRESS;
	mr.mr_limit = MiB; /*First MiB*/
        

	if( OK != (i = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", i);



        if(get_mode_info(mode,&info)){
                return NULL;
        }

        hres = info.XResolution;
        vres = info.YResolution;
        bitsPerPixel = info.BitsPerPixel;

        unsigned int vram_base = info.PhysBasePtr;
        unsigned int vram_size = info.XResolution*info.YResolution*info.BitsPerPixel/8;

        mr.mr_base = (phys_bytes) vram_base;
        mr.mr_limit = mr.mr_base + vram_size;

        if( OK != (i = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", i);

        video_buffer = vm_map_phys(SELF,(void *) mr.mr_base,vram_size);

        video_mem = malloc(XRES*YRES*info.BitsPerPixel/8);

	if(video_buffer == MAP_FAILED)
		panic("couldn’t map video memory");


	reg86_t r;
        memset(&r,0,sizeof(r));

	r.ah = VBE_FUNCTION_CALL; // VBE call, function 02 -- set VBE mode
  	r.al = VBE_FUNCTION_SET_MODE;
	r.bx = BIT(14)|mode; // set bit 14: linear framebuffer
	r.intno = VBE_INTNO;
	
	if( sys_int86(&r) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}
        
  	return video_mem;
}

void vg_fillPixel(uint16_t x, uint16_t y, uint8_t *color)
{
        uint8_t * copyVideo_mem = video_mem;
        uint32_t numBytes = (bitsPerPixel + 7) / 8;

        copyVideo_mem += numBytes * (XRES * y + x);

        memcpy(copyVideo_mem,color,numBytes);

}

int get_mode_info(uint16_t mode,vbe_mode_info_t * info){

        vbe_mode_info_t * aux;

        int i;
	struct minix_mem_range mr; /*physical memory range*/
        mmap_t map;

	/*Allow memory mapping*/
	mr.mr_base = BASE_ADDRESS;
	mr.mr_limit = MiB; //1 MiB
        
	if( OK != (i = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", i);

        aux = (vbe_mode_info_t*) lm_alloc(sizeof(vbe_mode_info_t),&map);

        reg86_t r;
        memset(&r,0,sizeof(r));

	r.ah = VBE_FUNCTION_CALL;
        r.al = VBE_FUNCTION_MODE_INFO;
        r.es = PB2BASE(map.phys);
        r.di = PB2OFF(map.phys);
        r.cx = mode;
	r.intno = VBE_INTNO;
	
	if( sys_int86(&r) != OK ) {
		printf("get_contr_info: sys_int86() failed \n");
		return 1;
	}

        *info = *aux;

        lm_free(&map);

        return 0;

}

void* farToPtr(uint32_t far,mmap_t map){

        uint32_t MS = ((far & 0xFFFF0000) >> 12);
        uint32_t LS = far & 0xFFFF;

        return (void*)((uint32_t) map.virt - map.phys + MS + LS);

}


int vbe_get_contr_info(vg_vbe_contr_info_t * contr_info_1){

        VbeInfoBlock * contr_info;

        int i;
	struct minix_mem_range mr; /*physical memory range*/
        mmap_t map;

	/*Allow memory mapping*/
	mr.mr_base = 0;
	mr.mr_limit = MiB; //1 MiB
        
	if( OK != (i = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", i);

        contr_info = (VbeInfoBlock*) lm_alloc(sizeof(VbeInfoBlock),&map);

        (*contr_info).VbeSignature[0] = 'V';
        (*contr_info).VbeSignature[1] = 'B';
        (*contr_info).VbeSignature[2] = 'E';
        (*contr_info).VbeSignature[3] = '2';


        reg86_t r;
        memset(&r,0,sizeof(r));

	r.ah = VBE_FUNCTION_CALL;
        r.al = VBE_FUNCTION_CONTROLLER_INFO;
        r.es = PB2BASE(map.phys);
        r.di = PB2OFF(map.phys);
	r.intno = VBE_INTNO;
	
	if( sys_int86(&r) != OK ) {
		printf("get_contr_info: sys_int86() failed \n");
		return 1;
	}


        *contr_info_1 = *((vg_vbe_contr_info_t*) contr_info);

        (*contr_info_1).OEMString = (char*)farToPtr((*contr_info).OemStringPtr,map);
        (*contr_info_1).VideoModeList = (uint16_t*)farToPtr((*contr_info).VideoModePtr,map);
        (*contr_info_1).TotalMemory = (*contr_info).TotalMemory * 64;//Total memory
        (*contr_info_1).OEMVendorNamePtr = (char*)farToPtr((*contr_info).OemVendorNamePtr,map);
        (*contr_info_1).OEMProductNamePtr = (char*)farToPtr((*contr_info).OemProductNamePtr,map);
        (*contr_info_1).OEMProductRevPtr = (char*)farToPtr((*contr_info).OemProductRevPtr,map);

        lm_free(&map);

        return 0;
}

void displayBuffer(void* video_buffer,void* video_mem,int x,int y){

        uint8_t * copyVideo_mem = (uint8_t*)video_mem;
        uint8_t * copyVideo_buffer = (uint8_t*)video_buffer;

        copyVideo_mem += (bitsPerPixel/8) * (XRES * y + x);

        for(size_t i = 0; i < vres; ++i){
                memcpy(copyVideo_buffer,copyVideo_mem,hres*bitsPerPixel/8);
                copyVideo_mem += XRES*bitsPerPixel/8;
                copyVideo_buffer += hres*bitsPerPixel/8;
        }

}
