#include "background.h"

int createBackground() {
    background = (uint8_t*) malloc(get_frame_size() * sizeof(uint8_t));
    if (background == NULL)
    {
      return 1;
    } 
    return 0;
}

void setBackground(uint32_t* img) {
    printf("vg_draw_circle inside %s\n", (void*)img);
    memcpy(background, img, get_frame_size());
}
