#include "video.h"
#include <string.h>

vbe_mode_info_t info;
static uint8_t *video_mem; //virtual mem para frame_buffer
static uint8_t *video_mem_sec;
static bool isSecBuffer = true;

struct masks masks;
//Setar o modo gráfico
int (set_graphic_mode)(uint16_t mode){
    reg86_t r86;
   
    /* Specify the appropriate register values */
  
    memset(&r86, 0, sizeof(r86));	/* zero the structure */

    r86.intno = 0x10; /* BIOS video services */
    r86.ah = 0x4F;    
    r86.al = 0x02; 
    r86.bx = mode | BIT(14);
  
    /* Make the BIOS call */

    if( sys_int86(&r86) != OK ) {
        printf("\tvg_init(): sys_int86() failed \n");
        return 1;
    }
  return 0;
}

void(reset_video)(){
        memset(video_mem_sec,1,info.XResolution* info.YResolution *((info.BitsPerPixel +7)/8));
}

// map the video memory to the process' address space
int (allocVirtMem)(uint16_t mode){
    struct minix_mem_range mr;
    unsigned int vram_base;  /* endereço base da Vram física */
    unsigned int vram_size;  /* tamanho da Vram física */
    int r;

    if(vbe_get_mode_info(mode, &info)!=0)return 1; // ir buscar a info do MODE (ex:Resolução,Bytes por pixe,etc)

    createMasks();

    vram_size = info.XResolution * info.YResolution * ((info.BitsPerPixel + 7) / 8); // tamanho do buffer 
    vram_base = info.PhysBasePtr; //endereço base do buffer

    mr.mr_base = (phys_bytes) vram_base;	// guardar endereço base do buffer
    mr.mr_limit = mr.mr_base + vram_size;   // guardar ultimo endereço do buffer

    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) return 1; //alocar memória virtual para frame buffer
            
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if(video_mem == MAP_FAILED) return 1;

    video_mem_sec = (uint8_t *) malloc(vram_size);
        
    return 0;
}

int colorCorrection(uint32_t *color){
    uint32_t mask =0;
    if(info.BitsPerPixel < 32 ){
        for(uint32_t i = 0; i < info.BitsPerPixel;++i){
            mask|= BIT(i);
        }
        *color = (mask & *color);
    }
    return 0;
    
}


int createMasks(){
    
    if(info.MemoryModel == 0x06){ //direct model
        masks.red = 0;
        for(uint32_t i = info.RedFieldPosition; i < info.RedFieldPosition + info.RedMaskSize;++i){
            masks.red |= BIT(i);
        }

        masks.green = 0;
        for(uint32_t i = info.GreenFieldPosition; i < info.GreenFieldPosition + info.GreenMaskSize;++i){
            masks.green |= BIT(i);
        }

        masks.blue= 0;
        for(uint32_t i = info.BlueFieldPosition; i < info.BlueFieldPosition + info.BlueMaskSize;++i){
            masks.blue |= BIT(i);
        }
    }


    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    for(unsigned i = 0; i < len ; i++){
        if(paint(x+i,y,color))return 1;
    }
    return 0;
}

int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    for(unsigned i = 0; i < len ; i++){
        if(paint(x,y+i,color))return 1;
    }
    return 0;
}


int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    for(unsigned j = 0; j < height; j++){
        if(vg_draw_hline(x,y+j,width,color)!=0)
        return 1;
    }
    return 0;
}

int (paint)(uint16_t x,uint16_t y,uint32_t color){

    if( x > info.XResolution || y >  info.YResolution)return 1;
    
    unsigned index = (info.XResolution * y + x) * ((info.BitsPerPixel+ 7) /8); // como é divisão inteira, a unica forma de arredondar pra cima é somar 7

    if(memcpy(video_mem_sec + index,&color,((info.BitsPerPixel + 7)/8))==NULL)return 1;

    return 0;
}

uint32_t (getRed(uint32_t color)){
    return ((BIT(info.RedMaskSize)-1) & (color >> info.RedFieldPosition));
}

uint32_t (getGreen(uint32_t color)){
    return ((BIT(info.GreenMaskSize)-1) & (color >> info.GreenFieldPosition));
}

uint32_t (getBlue(uint32_t color)){
    return ((BIT(info.BlueMaskSize)-1) & (color >> info.BlueFieldPosition));
}

void (swapBuff)(){
    reg86_t r;
    memset(&r, 0, sizeof(r));
    uint16_t dx;
    dx = 0;

    if (!isSecBuffer) { 
        //dx = info.YResolution;
    } else {  
      //  dx = 0;
    }
    isSecBuffer = !isSecBuffer;

    r.ax = 0x4F07;
    r.bh = 0x00;
    r.bl = 0x00; 
    r.dx = dx;
    r.intno = 0x10;
    sys_int86(&r);

    // Point video_mem to the current buffer
        memcpy(video_mem, video_mem_sec, (info.XResolution * info.YResolution * ((info.BitsPerPixel + 7) / 8)));

    
}


int(show_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y){
    
    xpm_image_t img;
    int counter=0;

    uint32_t *arrayColors = (uint32_t*) xpm_load(xpm,XPM_8_8_8_8,&img);

    for(int height = y; height < img.height + y; height++){
        for(int width = x; width < img.width + x; width++){
            
            uint32_t color = arrayColors[counter++];
            if(color != 0xFFFFFE){
                if(paint(width,height,color)!=0)return 1;
            }
            
        }
    }

    return 0;

}
