#include "video_gr.h"

extern vbe_mode_info_t mode_info;
extern uint8_t* first_frame_buffer;
extern uint8_t* second_frame_buffer;
extern int counter;
extern uint8_t outputCode;

int (paint_pixel)(uint16_t x, uint16_t y, uint32_t color){
    if (x>mode_info.XResolution || y>mode_info.YResolution || color==0xF0F0F0) return 0;

    unsigned bytesPerPixel = (mode_info.BitsPerPixel+7)/8;

    unsigned int index = (mode_info.XResolution*y + x) * bytesPerPixel;

    if (memcpy(&second_frame_buffer[index],&color,bytesPerPixel)==NULL){
        vg_exit();
        return 1;
    }

    return 0;

}
