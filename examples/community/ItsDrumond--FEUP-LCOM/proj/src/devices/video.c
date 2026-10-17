#include <lcom/lcf.h>
#include "video.h"
#include "keyboard.h"
#include "xpm/xpm_sprites.h"
#include "xpm/xpm_id.h"

extern uint8_t data;
vbe_mode_info_t info;
void *video_mem;
void *video_mem_sec;
unsigned int vram_size;
uint16_t* maps[NUM_XPM_FILES];
xpm_image_t images[NUM_XPM_FILES]; 


void (change_buffer)(){
    memcpy(video_mem, video_mem_sec, vram_size);
}

int(set_frame_buffer)(uint16_t mode){
    vbe_get_mode_info(mode, &info);

struct minix_mem_range mr;
unsigned int vram_base;  /* VRAM's physical addresss */
vram_base = info.PhysBasePtr;

uint8_t bytes_per_pixel = (info.BitsPerPixel + 7) / 8;
vram_size = info.XResolution * info.YResolution * bytes_per_pixel;
int r;				    

/* Use VBE function 0x01 to initialize vram_base and vram_size */


/* Allow memory mapping */

mr.mr_base = (phys_bytes) vram_base;	
mr.mr_limit = mr.mr_base + vram_size;  

if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
   panic("sys_privctl (ADD_MEM) failed: %d\n", r);
   return 1;}

/* Map memory */

video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

if(video_mem == MAP_FAILED){
   panic("couldn't map video memory");
   return 1;
}
    //memset(video_mem, 0, vram_size);
    reg86_t r86;
    memset(&r86, 0, sizeof(r86));
    r86.ax = VBE_CALL_FNC_2_16; // VBE call, function 02 -- set VBE mode
    r86.bx = VBE_LINEAR_FRAMEBUFFER|mode; // set bit 14: linear framebuffer
    r86.intno = 0x10;

    if( sys_int86(&r86) != OK ) {
       
        return 1;
    }
    video_mem_sec = malloc(vram_size);
    return 0;
}



int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
    if(x >= info.XResolution || y >= info.YResolution){
        return 0;
    }
    uint8_t bytes = (info.BitsPerPixel+7) / 8;
    uint8_t* ptr = (uint8_t*) video_mem_sec + (y * info.XResolution + x) * bytes;
    for(uint8_t j = 0; j < bytes; j++){
        *ptr = color >> (j * 8);
        ptr++;
    }
    return 0;
}



int (vg_load_xpm)(){
    //xpm_image_t image;
        
        maps[TITLE_XPM_ID] = (uint16_t*) xpm_load(title,XPM_5_6_5,&images[TITLE_XPM_ID]);
        maps[HELI_XPM_ID] = (uint16_t*) xpm_load(heli_xpm, XPM_5_6_5, &images[HELI_XPM_ID]);
         maps[HELI_DEAD_XPM_ID] = (uint16_t*) xpm_load(heli_destruido,XPM_5_6_5,&images[HELI_DEAD_XPM_ID]);
        maps[SOLDIER1_XPM_ID] = (uint16_t*) xpm_load(soldado1,XPM_5_6_5,&images[SOLDIER1_XPM_ID]);
        maps[SOLDIER2_XPM_ID] = (uint16_t*) xpm_load(soldado2,XPM_5_6_5,&images[SOLDIER2_XPM_ID]);
        maps[SOLDIER3_XPM_ID] = (uint16_t*) xpm_load(soldado3,XPM_5_6_5,&images[SOLDIER3_XPM_ID]);
        maps[SOLDIER_DEAD_XPM_ID] = (uint16_t*) xpm_load(soldado_morto,XPM_5_6_5,&images[SOLDIER_DEAD_XPM_ID]);
        maps[BACKGROUND_DAY_XPM_ID] = (uint16_t*) xpm_load(background_day,XPM_5_6_5,&images[BACKGROUND_DAY_XPM_ID]);
        maps[HELI_BULLET_XPM_ID] = (uint16_t*) xpm_load(bala_heli,XPM_5_6_5,&images[HELI_BULLET_XPM_ID]);
        maps[SOLDIER_BULLET_XPM_ID] = (uint16_t*) xpm_load(bala_soldado,XPM_5_6_5,&images[SOLDIER_BULLET_XPM_ID]); 
        maps[TARGET_XPM_ID] = (uint16_t*) xpm_load(target,XPM_5_6_5,&images[TARGET_XPM_ID]);
        maps[GAMEOVER_XPM_ID] = (uint16_t*) xpm_load(gameover,XPM_5_6_5,&images[GAMEOVER_XPM_ID]);
        maps[PLAYAGAIN_NO_XPM_ID] = (uint16_t*) xpm_load(playagain_no,XPM_5_6_5,&images[PLAYAGAIN_NO_XPM_ID]);
        maps[PLAYAGAIN_YES_XPM_ID] = (uint16_t*) xpm_load(playagain_yes,XPM_5_6_5,&images[PLAYAGAIN_YES_XPM_ID]);
        maps[TITLE_XPM_ID] = (uint16_t*) xpm_load(title,XPM_5_6_5,&images[TITLE_XPM_ID]);
        maps[BACK_NO_XPM_ID] = (uint16_t*) xpm_load(back_no,XPM_5_6_5,&images[BACK_NO_XPM_ID]);
        maps[BACK_YES_XPM_ID] = (uint16_t*) xpm_load(back_yes,XPM_5_6_5,&images[BACK_YES_XPM_ID]);
        maps[INSTRUCT_NO_XPM_ID] = (uint16_t*) xpm_load(instructions_no,XPM_5_6_5,&images[INSTRUCT_NO_XPM_ID]);
        maps[INSTRUCT_YES_XPM_ID] = (uint16_t*) xpm_load(instructions_yes,XPM_5_6_5,&images[INSTRUCT_YES_XPM_ID]);
        maps[START_NO_XPM_ID] = (uint16_t*) xpm_load(start_no,XPM_5_6_5,&images[START_NO_XPM_ID]);
        maps[START_YES_XPM_ID] = (uint16_t*) xpm_load(start_yes,XPM_5_6_5,&images[START_YES_XPM_ID]);
        maps[INSTRUCTEXT_XPM_ID] = (uint16_t*) xpm_load(textinstructions,XPM_5_6_5,&images[INSTRUCTEXT_XPM_ID]);
        maps[EXPLOSION1_XPM_ID] = (uint16_t*) xpm_load(explosion1,XPM_5_6_5,&images[EXPLOSION1_XPM_ID]);
        maps[EXPLOSION2_XPM_ID] = (uint16_t*) xpm_load(explosion2,XPM_5_6_5,&images[EXPLOSION2_XPM_ID]);
        maps[EXPLOSION3_XPM_ID] = (uint16_t*) xpm_load(explosion3,XPM_5_6_5,&images[EXPLOSION3_XPM_ID]);
        maps[PLATFORFORM_XPM_ID] = (uint16_t*) xpm_load(plataformas,XPM_5_6_5,&images[PLATFORFORM_XPM_ID]);
        maps[HP_100] = (uint16_t*) xpm_load(hp100,XPM_5_6_5,&images[HP_100]);
        maps[HP_80] = (uint16_t*) xpm_load(hp80,XPM_5_6_5,&images[HP_80]);
        maps[HP_60] = (uint16_t*) xpm_load(hp60,XPM_5_6_5,&images[HP_60]);
        maps[HP_40] = (uint16_t*) xpm_load(hp40,XPM_5_6_5,&images[HP_40]);
        maps[HP_20] = (uint16_t*) xpm_load(hp20,XPM_5_6_5,&images[HP_20]);
        maps[HP_0] = (uint16_t*) xpm_load(hp0,XPM_5_6_5,&images[HP_0]);
        maps[BACKGROUND_NIGHT_XPM_ID] = (uint16_t*) xpm_load(background_night,XPM_5_6_5,&images[BACKGROUND_NIGHT_XPM_ID]);
        maps[NUMBER_0] = (uint16_t*) xpm_load(zero,XPM_5_6_5,&images[NUMBER_0]);
        maps[NUMBER_1] = (uint16_t*) xpm_load(one,XPM_5_6_5,&images[NUMBER_1]);
        maps[NUMBER_2] = (uint16_t*) xpm_load(two,XPM_5_6_5,&images[NUMBER_2]);
        maps[NUMBER_3] = (uint16_t*) xpm_load(three,XPM_5_6_5,&images[NUMBER_3]);
        maps[NUMBER_4] = (uint16_t*) xpm_load(four,XPM_5_6_5,&images[NUMBER_4]);
        maps[NUMBER_5] = (uint16_t*) xpm_load(five,XPM_5_6_5,&images[NUMBER_5]);
        maps[NUMBER_6] = (uint16_t*) xpm_load(six,XPM_5_6_5,&images[NUMBER_6]);
        maps[NUMBER_7] = (uint16_t*) xpm_load(seven,XPM_5_6_5,&images[NUMBER_7]);
        maps[NUMBER_8] = (uint16_t*) xpm_load(eight,XPM_5_6_5,&images[NUMBER_8]);
        maps[NUMBER_9] = (uint16_t*) xpm_load(nine,XPM_5_6_5,&images[NUMBER_9]);
        maps[DOUBLE_DOTS] = (uint16_t*) xpm_load(double_dots,XPM_5_6_5,&images[DOUBLE_DOTS]);
        maps[JANUARY] = (uint16_t*) xpm_load(jan,XPM_5_6_5,&images[JANUARY]);
        maps[FEBRUARY] = (uint16_t*) xpm_load(feb,XPM_5_6_5,&images[FEBRUARY]);
        maps[MARCH] = (uint16_t*) xpm_load(mar,XPM_5_6_5,&images[MARCH]);
        maps[APRIL] = (uint16_t*) xpm_load(apr,XPM_5_6_5,&images[APRIL]);
        maps[MAY] = (uint16_t*) xpm_load(may,XPM_5_6_5,&images[MAY]);
        maps[JUNE] = (uint16_t*) xpm_load(jun,XPM_5_6_5,&images[JUNE]);
        maps[JULY] = (uint16_t*) xpm_load(jul,XPM_5_6_5,&images[JULY]);
        maps[AUGUST] = (uint16_t*) xpm_load(aug,XPM_5_6_5,&images[AUGUST]);
        maps[SEPTEMBER] = (uint16_t*) xpm_load(sep,XPM_5_6_5,&images[SEPTEMBER]);
        maps[OCTOBER] = (uint16_t*) xpm_load(oct,XPM_5_6_5,&images[OCTOBER]);
        maps[NOVEMBER] = (uint16_t*) xpm_load(nov,XPM_5_6_5,&images[NOVEMBER]);
        maps[DECEMBER] = (uint16_t*) xpm_load(dec,XPM_5_6_5,&images[DECEMBER]);

         

        return 0;
    }


int (vg_draw_xpm) (uint8_t id, uint16_t x, uint16_t y){
    if(x > info.XResolution || y > info.YResolution || id < 0 || id > NUM_XPM_FILES){
        return 1;
    }
    
    xpm_image_t image = images[id];
    uint16_t* map = maps[id];
    

    uint32_t counter = 0;
    uint32_t extra_x = 0;

    uint32_t width, height;
    
    if(x+image.width <= info.XResolution){
        width = x+image.width;
    } else {
        width = info.XResolution;
        extra_x = x+image.width - width;
    }

    if(y + image.height <= info.YResolution){
        height = y+image.height;
    } else {
        height = info.YResolution;
    }

    for(uint16_t i = y; i < height; i++){
        for(uint16_t j = x; j < width; j++){
            if(map[counter] != 0xFFFF){
                vg_draw_pixel(j,i,map[counter]);
            }
            counter++;
        }
        counter += extra_x;
    }
    return 0;
}

void free_buffer(){
    free(video_mem_sec);

    for (int i = 0; i < NUM_XPM_FILES; i++) {
    free(maps[i]);
}


for (int i = 0; i < NUM_XPM_FILES; i++) {
    free(images[i].bytes);
}

}
