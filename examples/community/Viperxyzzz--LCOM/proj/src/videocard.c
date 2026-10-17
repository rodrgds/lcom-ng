#include "videocard.h"

static char *video_mem;		/* Process (virtual) address to which VRAM is mapped */
char *video_buff;  /*buffer*/
static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
vbe_mode_info_t vmi_p;



int (set_vbe_mod)(uint16_t mode)
{
    reg86_t r;
    memset(&r, 0, sizeof(reg86_t));
    r.ax = 0x4F02;
    r.ah = 0x4F;
    r.bx = 1 << 14 | mode;
    r.intno = 0x10;
    if(sys_int86(&r))
    {
        printf("Failed big time");
        return 1;
    }

    
    if(r.al != 0x4F || r.ah !=  0x00)
    {
        printf("Failed....");
        return 1;   
    }


    return 0;
}


void* (vg_init)(uint16_t mode)
{

    vbe_get_mode_info(mode,&vmi_p);
    h_res = vmi_p.XResolution;
    v_res = vmi_p.YResolution;
    bits_per_pixel = vmi_p.BitsPerPixel;


    struct minix_mem_range mr;
    unsigned int vram_base = vmi_p.PhysBasePtr;  /* VRAM's physical addresss */
    unsigned int vram_size = h_res * v_res * (bits_per_pixel + 7 ) / 8 ;  /* VRAM's size, but you can use
                        the frame-buffer size, instead */
    int r;				    

    /* Use VBE function 0x01 to initialize vram_base and vram_size */
    reg86_t r86;
    memset(&r86,0,sizeof(reg86_t));
    r86.ax = 0x4F02;
    r86.bx = 1 << 14 | mode;
    r86.intno = 0x10;
    sys_int86(&r86);

    /* Allow memory mapping */

    mr.mr_base = (phys_bytes) vram_base;	
    mr.mr_limit = mr.mr_base + vram_size;  

    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    /* Map memory */

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    video_buff = (char*)malloc(vram_size);

    if(video_mem == MAP_FAILED)
        panic("couldn't map video memory");
   
    return video_mem;
    
}



void (pixel_set_color)(uint16_t x, uint16_t y, uint32_t color)
{

    if(color == IGNORED)
        return;
    
    if(bits_per_pixel == 8)
    {
        memset(video_buff + h_res * y + x ,color,1);
    }
    else
    {
        memcpy(video_buff + (h_res * y + x)*(bits_per_pixel / 8),&color,(bits_per_pixel/8));
    }
    
}



void (vg_clear_screen)()
{
    memset(video_buff,0,h_res*v_res*(bits_per_pixel/8));
}

void (double_buff)()
{
    memcpy(video_mem,video_buff,h_res*v_res*(bits_per_pixel/8));
}

int (vg_read_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y)
{
    xpm_image_t img;
    uint8_t* map = xpm_load(xpm,XPM_8_8_8,&img);
    uint32_t index = 0;
    
    
    for (uint16_t j = y ; j < y + img.height; j++)
    {
        for(uint16_t i = x; i < x + img.width; i++)
        {
            uint32_t color = 0;
            memcpy(&color,&map[index],3);

            pixel_set_color(i,j,color);
            index += 3;
        }
    }

    

    return 0;
}




