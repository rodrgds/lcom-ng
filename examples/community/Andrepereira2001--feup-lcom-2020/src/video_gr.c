#include <lcom/lcf.h>

#include <stdint.h>
#include "videocardVars.h"
#include "gameVar.h"
#include <minix/sysutil.h>

static void *video_mem;		/** Process (virtual) address to which VRAM is mapped */
static void *buffer;        /** buffer to make the double-buffering*/
static void *background_buffer;

static int unsigned h_res;	        /** Horizontal resolution in pixels */
static int unsigned v_res;	        /** Vertical resolution in pixels */
static unsigned bits_per_pixel;     /** Number of VRAM bits per pixel */

int increment = 0; /** actual increment of the background (to make the movement) */

int (vbe_get_mode_info_me)(uint16_t mode,vbe_mode_info_t * vmi){

    mmap_t map;
    lm_alloc(sizeof(vbe_mode_info_t),&map);


    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));

    reg86.intno = VIDEO_CARD;
    reg86.ax = AX_VBE_MODE_INFO;
    reg86.cx = mode | BIT(14);

    reg86.es =  PB2BASE(map.phys);
    reg86.di =  PB2OFF(map.phys);
    

    if(sys_int86(&reg86) != 0 ) {
        printf("\tvbe_get_mode_info_me(): sys_int86() failed \n");
    return 1;
    }

    *vmi = *((vbe_mode_info_t*) (map.virt));

    lm_free(&map);

    return 0;
}

void (vg_map)(uint16_t mode){

    mmap_t map;
    // size = 1MB    
    lm_alloc(BIT(20), &map); 
      /* frame-buffer VM address 
				   (static global variable*/ 

    struct minix_mem_range mr;
    unsigned int vram_base;  /* VRAM's physical addresss */
    unsigned int vram_size;  /* VRAM's size, but you can use
				    the frame-buffer size, instead */
    int r;		
		    

/* Use VBE function 0x01 to initialize vram_base and vram_size */

    vbe_mode_info_t vmi;
    vbe_get_mode_info_me(mode,&vmi);

    vram_base = vmi.PhysBasePtr;
    unsigned bitsPerPixelMultiplier = 0;
    if(mode == MODE2){
        bitsPerPixelMultiplier = 16;
    }
    else{
        bitsPerPixelMultiplier = vmi.BitsPerPixel;
    }

    vram_size = vmi.XResolution * vmi.YResolution * bitsPerPixelMultiplier / 8;
    h_res = vmi.XResolution;
    v_res = vmi.YResolution;
    bits_per_pixel = vmi.BitsPerPixel;

/* Allow memory mapping */

    mr.mr_base = (phys_bytes) vram_base;	
    mr.mr_limit = mr.mr_base + vram_size;  

    if( 0 != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    /* Map memory */

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    buffer = malloc(vram_size);

    background_buffer = malloc(2*vram_size);

    if(video_mem == MAP_FAILED)
        panic("couldn't map video memory");
    

    lm_free(&map);

}

void *(vg_init)(uint16_t mode){

    vg_map(mode);

    reg86_t reg86;

    memset(&reg86, 0, sizeof(reg86)); // make sure all bits are at zero

    reg86.intno = VIDEO_CARD;
    reg86.ax = AX_SET_VBE_MODE;
    // bit 14 of bx must be set
    reg86.bx = BIT(14) | mode;
    
    sys_int86(&reg86);

    return video_mem;
}

void (vg_paint_pixel)(uint16_t x,uint16_t y,uint32_t color){
    char * addr = buffer;
    unsigned max = ((v_res-1)*(h_res) + (h_res));
    int bytes = bits_per_pixel/8;

    if(color == xpm_transparency_color(XPM_8_8_8_8)){
        return;
    }
    if(x >= 0 && y >= 0 && y < v_res && x < h_res){
        if((y*h_res + x) < max){
            addr += (y*h_res + x)*bytes;
            memcpy(addr,&color,bytes);
        }
    }
}

void (clean_screen)(){
    memset(video_mem, xpm_transparency_color(XPM_8_8_8_8), h_res*v_res*bits_per_pixel/8);
}

void (free_buffer_mem)(){
    free(background_buffer);
    free(buffer);
}

void (copy_buffer)(){
    memcpy(video_mem,buffer,h_res*v_res*bits_per_pixel/8);
}

void (clean_buffer)(){
    memset(buffer,xpm_transparency_color(XPM_8_8_8_8),h_res*v_res*bits_per_pixel/8);
}

void (copy_background_buffer)(){
    char * mem = background_buffer;
    increment = (increment + BACKGROUND_SPEED*8) % (h_res*8); 
    memcpy(buffer,mem + increment,h_res*v_res*bits_per_pixel/8);
}

void (copy_buffer_to_background_buffer)(){
    memcpy(background_buffer,buffer,h_res*v_res*bits_per_pixel/8);
    char * mem = background_buffer;
    memcpy(mem + h_res*v_res*bits_per_pixel/8,buffer,h_res*v_res*bits_per_pixel/8);
}

int (get_x_max)(){
    return h_res;
}

int (get_y_max)(){
    return v_res;
}
