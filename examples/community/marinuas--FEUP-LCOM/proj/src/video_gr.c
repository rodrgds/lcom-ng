// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include "video_gr.h"

static uint8_t *video_mem;		/* Process (virtual) address to which VRAM is mapped */
static uint8_t *double_buff;
static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned bytes_per_pixel;
vbe_mode_info_t vmi_p;
int r;
struct minix_mem_range mr; /* physical memory range */
unsigned int vram_base; /* VRAM’s physical addresss */
unsigned int vram_size; /* VRAM’s size, but you can use the frame-buffer size, instead */
extern uint32_t counter;


int VBE_inteface(uint16_t mode){
    reg86_t r;
    //INICIALIZA AO R O VALOR DE 0
    memset(&r, 0, sizeof(reg86_t));

    r.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
    r.bx = 1<<14|mode; // set bit 14: linear framebuffer
    r.intno = 0x10;
    if( sys_int86(&r) != OK ) {
        printf("set_vbe_mode: sys_int86() failed \n");
        return 1;
    }
    return 0;
}

void* (vg_init)(uint16_t mode){
    vbe_get_mode_info(mode, &vmi_p);

    h_res = vmi_p.XResolution;
  	v_res = vmi_p.YResolution;
    bits_per_pixel = vmi_p.BitsPerPixel;
    bytes_per_pixel = ceil(bits_per_pixel / 8.0);
    
    mapping_vram();

    VBE_inteface(mode);
    
    
    return video_mem;
}

int mapping_vram(){
    vram_base = vmi_p.PhysBasePtr;
    vram_size = bytes_per_pixel * h_res * v_res;

    double_buff = malloc(vram_size);

    /* Allow memory mapping */
    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    if( 0 != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
        
    /* Map memory */
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    if(video_mem == MAP_FAILED)
        panic("couldn’t map video memory");

    return 0;
}

int pixel_color(uint16_t x, uint16_t y, uint32_t color){
    if(x >= h_res || y>= v_res) return 1;
    uint8_t* pixel = double_buff;
    pixel += (h_res * y * bytes_per_pixel) + (x * bytes_per_pixel);

    for (unsigned i = 0; i < bytes_per_pixel; i++) {
        *pixel = (color >> (i * 8));
        pixel++;
    }
    return 0;
}

int vg_draw_xpm_proj(xpm_image_t image, uint16_t x, uint16_t y, uint8_t* xpm_loaded){
    uint32_t color=   0;
    uint32_t image_bytes;
    for(int i=0; i<image.height;i++){

        for(int k=0; k<image.width; k++){

            for(unsigned j=0; j<bytes_per_pixel; j++){

                image_bytes = image.bytes[i * image.width +j + k] << (8*j);
                color += image_bytes;
            }

            
            if(color!=(xpm_transparency_color(XPM_8_8_8))) pixel_color( k+x, i+y, color);
            image.bytes += bytes_per_pixel - 1;
            color = 0;
        }
    }
    return 0;
}


void change_buffer(){
    memcpy(video_mem, double_buff, vram_size);
}

