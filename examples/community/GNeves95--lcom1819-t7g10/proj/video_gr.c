// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

//#include <lcom/vbe.h>

#include <lcom/video_gr.h>

#include <liblm.h>

#include <machine/int86.h>

//#include <minix/driver.h>
#include <sys/mman.h>

#include <stdint.h>
#include <stdio.h>

#include "vga.h"

#include "kbd.h"

//int sys_int86(struct reg86u *reg86p);
/*

Initializes the video module in graphics mode.

Uses the VBE INT 0x10 interface to set the desired graphics mode using a linear frame buffer, maps VRAM to the process' address space and initializes static global variables with the resolution of the screen, and the color depth (i.e the no. of bits per pixel). Initially, you should call function vbe_get_mode_info() provided by the LCF, to get this information. Later, you can implement your own version of this function.

Parameters
    mode	16-bit VBE mode to set

Returns
    Virtual address VRAM was mapped to. NULL, upon failure. 

*/

int (vbe_get_info)(unsigned short mode, vbe_mode_info_t *vmi_p) {

    mmap_t buf;
	struct reg86u r;
	memset(&r, 0, sizeof(struct reg86u));
	
	/* use liblm.a to initialize buf*/
	lm_init(true);
	
	vbe_mode_info_t* virt_mem = lm_alloc(sizeof(vbe_mode_info_t),&buf);

	r.u.w.ax = VBE_INFO; /* VBE get mode info */
	
	/* translate the buffer linear address to a far pointer */
	r.u.w.es = PB2BASE(buf.phys); /* set a segment base */
	r.u.w.di = PB2OFF(buf.phys); /* set the offset accordingly */
	r.u.w.cx = mode;
	r.u.b.intno = VBE_INT;
	if( sys_int86(&r) != OK ) { /* call BIOS */
		fprintf(stderr, "Failed call to BIOS");
		lm_free(&buf);
		return 1;
	}
	
	if(r.u.b.ah == 0x01){
		printf("Mapping Failed!\n");
		lm_free(&buf);
		return 1;
	}
	
	if(r.u.b.ah == 0x02){
		printf("Function not supported in current Hardware config!\n");
		lm_free(&buf);
		return 1;
	}
	
	if(r.u.b.ah == 0x03){
		printf("Function not supported in current mode!\n");
		lm_free(&buf);
		return 1;
	}
	
    //*vmi_p = (vbe_mode_info_t*) buf.virtual;
    *vmi_p = *virt_mem;
    //vmi_p = /*(vbe_mode_info_t)*/ (buf.virt);
	lm_free(&buf);
	return 0;
}

int (getHRes)(){
	return h_res;
}

int (getVRes)(){
	return v_res;
}

int (getBitsPerPixel)(){
	return bits_per_pixel;
}

colorMode (getcm)(){
	return cm;
}

uint8_t (getRSM)(){
	return RedScreenMask;
}

uint8_t (getGSM)(){
	return GreenScreenMask;
}

uint8_t (getBSM)(){
	return BlueScreenMask;
}

void* (vg_init)(uint16_t mode){
	struct reg86u r;
	memset(&r, 0, sizeof(struct reg86u));
	r.u.w.ax = VBE_SET; // VBE call, function 02 -- set VBE mode
	r.u.w.bx = 1<<14|mode; // set bit 14: linear framebuffer
	r.u.b.intno = VBE_INT;
	if( sys_int86(&r) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}
	
	if(r.u.b.ah == 0x01){
		printf("Mapping Failed!\n");
		return NULL;
	}
	
	if(r.u.b.ah == 0x02){
		printf("Function not supported in current Hardware config!\n");
		return NULL;
	}
	
	if(r.u.b.ah == 0x03){
		printf("Function not supported in current mode!\n");
		return NULL;
	}
		
	vbe_mode_info_t vmi_p;
	
	vbe_get_info(mode, &vmi_p);
	
	h_res = vmi_p.XResolution;
	v_res = vmi_p.YResolution;
	bits_per_pixel = vmi_p.BitsPerPixel;
	
	RedMaskSize = vmi_p.RedMaskSize;
	RedScreenMask = vmi_p.RedFieldPosition;
	GreenMaskSize = vmi_p.GreenMaskSize;
	GreenScreenMask = vmi_p.GreenFieldPosition;
	BlueMaskSize = vmi_p.BlueMaskSize;
	BlueScreenMask = vmi_p.BlueFieldPosition;
	/*
	FILE *fp;
	fp = fopen("/home/lcom/labs/lab5/log.txt", "a");
	
	fprintf(fp, "%d %d %d %d %d %d\n", RedMaskSize, RedScreenMask, GreenMaskSize, GreenScreenMask, BlueMaskSize, BlueScreenMask);
	
	fclose(fp);
	*/
	//static void *video_mem;
	if(RedMaskSize == 0){
		cm = indexed;
	}
	else
		cm = direct;
	
	int ret;
	struct minix_mem_range mr;
	//unsigned int vram_base = vmi_p.PhysBasePtr;
	/* VRAM's physical addresss */
	unsigned int vram_size = ((h_res * v_res * bits_per_pixel)/8);
	
	/* VRAM's size, but you can use the frame-buffer size, instead */
	/* Allow memory mapping */
	mr.mr_base = vmi_p.PhysBasePtr;
	mr.mr_limit = mr.mr_base + vram_size;
	
	if( OK != (ret = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", ret);
		
	/* Map memory */
	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
	
	secBuf = malloc(h_res*v_res*bits_per_pixel/8);
	
	if(video_mem == MAP_FAILED)
		panic("couldn't map video memory");
		
	return video_mem;
}

/*
Draws a color to a pixel by changing the video RAM.

Note that the x coordinates start at 0 and increase from left to right
Likewise the y coordinates start at 0 and increase from top to bottom

Parameters
    x	horizontal coordinate
    y	vertical coordinate
    color	color to set the pixel

Returns
    0 on success, non-zero otherwise 
*/

int (set_pixel)(uint16_t x, uint16_t y, uint32_t color){
	if(x >= h_res || y >= v_res){
		printf("Coordinats out of bounds\n");
		return -1;
	}
	//*(video_mem + (x+y*h_res)*bits_per_pixel/8) = color;
	memcpy((video_mem + (x+y*h_res)*bits_per_pixel/8), &color, bits_per_pixel/8);
	return 0;
}

int (set_pixel_double)(uint16_t x, uint16_t y, uint32_t color){
	//printf("%d %d %d\n", x, y, color);
	if(x >= h_res || y >= v_res){
		//printf("Coordinats out of bounds\n");
		return -1;
	}
	//*(video_mem + (x+y*h_res)*bits_per_pixel/8) = color;
	memcpy((secBuf + (x+y*h_res)*bits_per_pixel/8), &color, bits_per_pixel/8);
	return 0;
}

void drawNewXPM(xpm_image_t xpm, int x, int y){
	uint16_t width = xpm.width, height = xpm.height;
	
	for(uint16_t i=0; i < height; i++){
		for(uint16_t j=0; j < width; j++){
			uint8_t b = *(xpm.bytes + (j+i*width)*bits_per_pixel/8);
			uint8_t g = *(xpm.bytes + 1 + (j+i*width)*bits_per_pixel/8);
			uint8_t r = *(xpm.bytes + 2 + (j+i*width)*bits_per_pixel/8);
			uint32_t aux = (r<<16) + (g << 8) + (b);
			if(aux == CHROMA_KEY_GREEN_888){
				continue;
			}
			else{
				if(i < v_res && j < h_res) set_pixel_double(x+j,y+i,aux);
			}
		}
	}
}
/*
Draws a horizontal line by changing the video RAM.

Draws a horizontal line with the specified length and color, starting at the specified coordinates.

Note that the x coordinates start at 0 and increase from left to right
Likewise the y coordinates start at 0 and increase from top to bottom

Parameters
    x	horizontal coordinate of the initial point
    y	vertical coordinate of the initial point
    len	line's length in pixels
    color	color to set the pixel
    addr	virtual address of the VRAM location with the initial point

Returns
    0 on success, non-zero otherwise 
*/

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
	for(uint16_t i = x; i < x+len; i++){
		if(i < h_res) set_pixel(i,y,color);
	}
	return 0;
}

/*
Draws a filled rectangle by changing the video RAM.

Draws a filled rectangle with the specified width, height and color, starting at the specified coordinates.

Note that the x coordinates start at 0 and increase from left to right.
Likewise, the y coordinates start at 0 and increase from top to bottom.

Parameters
    x	horizontal coordinate of the rectangle's top-left vertex
    y	vertical coordinate of the rectangle's top-left vertex
    width	rectangle's width in pixels
    height	rectangle's height in pixels
    color	color to set the pixel

Returns
    0 on success, non-zero otherwise 
*/

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
	for(uint16_t i = y; i < y+height; i++){
		for(uint16_t j = x; j < x+width; j++){
			if(i < v_res && j < h_res) set_pixel(j,i,color);
		}
	}
	return 0;
}

int vg_draw_rectangle_double(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
	for(uint16_t i = y; i < y+height; i++){
		for(uint16_t j = x; j < x+width; j++){
			if(i < v_res && j < h_res) set_pixel_double(j,i,color);
		}
	}
	return 0;
}

int (drawXPM)(const char *xpm[], int x, int y){
	int width, height;
	char * sprite = read_xpm(xpm, &width, &height);
	for(unsigned int i=0; i < (unsigned int)height; i++){
		for(unsigned int j=0; j < (unsigned int)width; j++){
			if(i < v_res && j < h_res) set_pixel(x+j,y+i,*(sprite + (j+i*width)*bits_per_pixel/8));
		}
	}
	return 0;
}

char *readXpm(const char *xpm[], int *width, int *height){
	*width = atoi(strsep((char**)xpm, " "));
	*height = atoi(strsep((char**)xpm, " "));
	int colors = atoi(strsep((char**)xpm, " "));
	char *ret = (char *)malloc((*width)*(*height)*sizeof(char));
	
	for(int i=1; i <= colors; i++){
		
	}
	return ret;
}

int (drawXPMdouble)(const char *xpm[], int x, int y){
	int width, height;
	char * spt = read_xpm(xpm, &width, &height);
	if(spt == NULL){
		printf("Failed loading sprite!\n");
		return -1;
	}
	int ratio = bits_per_pixel/8;
	//printf("%d %d %d %d\n", x, y, width, height);
	for(unsigned int i=0; i < (unsigned int)height; i++){
		for(unsigned int j=0; j < (unsigned int)width; j++){
			//printf("%d %d\n", i, j);
			if(i < v_res && j < h_res) 
				set_pixel_double(x+j,y+i,*(spt + (j+i*width)*ratio));
		}
	}
	return 0;
}

void (swap)(){
	memcpy(video_mem,secBuf,(h_res*v_res*bits_per_pixel/8));
}

int (clearScreen)(){
	for(uint16_t i = 0; i < v_res; i++){
		for(uint16_t j = 0; j < h_res; j++){
			if(i < v_res && j < h_res) *(video_mem + (j+i*h_res)*bits_per_pixel/8) = 0;
		}
	}
	return 0;
}

int (clearScreenDouble)(){
	memset(secBuf, 0, (v_res*h_res)*bits_per_pixel/8);
	return 0;
}

int (vg_vbe_get_info)(vg_vbe_contr_info_t *info_p){
	mmap_t buf;
	struct reg86u r;
	//memset(&r, 0, sizeof(struct reg86u));
	
	/* use liblm.a to initialize buf*/
	if(lm_init(true)){
		printf("Problem with lm_init\n");
		return 1;
	}
	
	vg_vbe_contr_info_t * virt_mem = (vg_vbe_contr_info_t *)lm_alloc(sizeof(vg_vbe_contr_info_t),&buf);
	
	if(virt_mem == NULL){
		printf("Failed to allocate memory!\n");
		return 1;
	}

	r.u.w.ax = 0x4F00; /* VBE get mode info */
	
	/* translate the buffer linear address to a far pointer */
	r.u.w.es = PB2BASE(buf.phys); /* set a segment base */
	r.u.w.di = PB2OFF(buf.phys); /* set the offset accordingly */
	r.u.b.intno = VBE_INT;
	if( sys_int86(&r) != OK ) { /* call BIOS */
		fprintf(stderr, "Failed call to BIOS");
		lm_free(&buf);
		return 1;
	}
	
	if(r.u.b.ah == 0x01){
		printf("Mapping Failed!\n");
		lm_free(&buf);
		return 1;
	}
	
	if(r.u.b.ah == 0x02){
		printf("Function not supported in current Hardware config!\n");
		lm_free(&buf);
		return 1;
	}
	
	if(r.u.b.ah == 0x03){
		printf("Function not supported in current mode!\n");
		lm_free(&buf);
		return 1;
	}
    
    char *farptr = (char *)((*(virt_mem->VideoModeList) & 0xFFFF0000) >> 12);
    farptr = farptr + (*(virt_mem->VideoModeList) & 0x0FFFF);
    farptr = farptr + ((uint32_t)virt_mem & 0xF0000000);
	
	*info_p = *virt_mem;
	
	lm_free(&buf);
	
	uint16_t *modes = (uint16_t *)farptr;
	int num_video_modes = 0;
	size_t i;
	for (i = 0; *modes != (uint16_t)-1; ++modes, num_video_modes += 1)
	{
		// Find the total number of video modes
	}
	if ((info_p->VideoModeList = malloc(num_video_modes * sizeof(uint16_t))) == NULL)
	{
		printf("failed allocating memory for video modes\n");
		return 1;
	}
	//(void *)modes = farptr;
	for(int i = 0; i < num_video_modes; ++i)
	{
		++modes;
		modes = (uint16_t *)farptr;
		(info_p->VideoModeList)[i] = *modes;
	}
	
	return 0;
}
