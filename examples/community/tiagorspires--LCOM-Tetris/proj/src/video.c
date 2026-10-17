#include <lcom/lcf.h>
#include <video.h>
#include <stdint.h>
#include "vbe.h"

uint8_t* video_mem;
uint8_t* double_buffer;
vbe_mode_info_t mode_info;

int (set_video_mode)(uint16_t submode) {
  reg86_t reg86;

    memset(&reg86, 0, sizeof(reg86)); /* zero the structure */
    reg86.ax = AX_GRAPHICS_MODE; /* VBE call, function 02 -- set VBE mode */
    reg86.bx = BX_GRAPHICS_MODE | submode; /* set bit 14: linear framebuffer */
    reg86.intno = INTNO;

  return sys_int86(&reg86);

}

int get_mode_info (uint16_t mode){ // encontrar o modo de resolucao
    memset(&mode_info, 0, sizeof(vbe_mode_info_t));
    if(vbe_get_mode_info(mode ,&mode_info)!=0) return 1;
    return 0;
}



unsigned int (bytes_per_pixel)(){ // calculo do numero de bytes por pixel
    return (mode_info.BitsPerPixel + 7) >> 3;
}

unsigned int (get_frame_buffer_size)(){ // calculo do tamanho do buffer
    return mode_info.XResolution * mode_info.YResolution * bytes_per_pixel();
}

int set_frame_buffer(uint16_t mode){
    get_mode_info(mode);
    unsigned int frame_buffer_size = get_frame_buffer_size();

    struct minix_mem_range mr; // memoria fisica (memory mapping)
    mr.mr_base = mode_info.PhysBasePtr;
    mr.mr_limit = mr.mr_base + frame_buffer_size;

    // alocar a memomoria fisica necessaria
    if(sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) return 1;

    // mapear a memoria fisica para a memoria virtual

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, frame_buffer_size);

    if(video_mem == NULL) return 1;

    double_buffer = malloc(frame_buffer_size);

    if(double_buffer == NULL) return 1;

return 0;
}

int (set_pixel_color) (uint16_t x, uint16_t y, uint32_t color){
    
    // verificar se as coordenadas estao dentro dos limites do ecrã

    if(x >= mode_info.XResolution || y >= mode_info.YResolution ) return 1;

    // define where to color the pixel

    unsigned int start_index = (y * mode_info.XResolution + x) * bytes_per_pixel();

    // copiar a cor para o pixel
    if(memcpy(&double_buffer[start_index], &color, bytes_per_pixel()) == NULL) return 1;

    
    return 0;

}

// desenhar uma linha horizontal

int (draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    
    while (len > 0){
        if(set_pixel_color(x, y, color) != 0) return 1;
        x++;
        len--;
    }

    return 0;
}  

int (draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    
    while (len > 0){
        if(set_pixel_color(x, y, color) != 0) return 1;
        y++;
        len--;
    }

    return 0;
}

int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    while (height > 0){
        if(draw_hline(x, y, width, color) != 0) return 1;
        y++;
        height--;
    }
    return 0;
}

int (draw_xpm)(uint16_t x, uint16_t y, xpm_map_t xpm){

xpm_image_t image;

uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &image);

if(colors == NULL) return 1;

for (int i = 0; i < image.height; i++){
    for (int j = 0; j < image.width; j++){
        uint32_t color = colors[i * image.width + j];
        if(set_pixel_color(x + j, y + i, color) != 0) {
            free(colors);
            return 1;
        }
    }
}

free(colors);
return 0;
}

int (clean_buffer) (){
    memset(double_buffer, 0, get_frame_buffer_size());
    return 0;
}

int (swap_buffer) (){
    memcpy(video_mem, double_buffer, get_frame_buffer_size());
    return 0;
}



