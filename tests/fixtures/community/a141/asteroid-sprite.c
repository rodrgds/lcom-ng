#include SPRITE_SOURCE
#include "video_gr.h"
#include <assert.h>
static char pixels[16];static int writes;
int vg_set_pixel_buffer(unsigned long x,unsigned long y,unsigned long color){assert(x<4&&y<4);pixels[y*4+x]=(char)color;writes++;return 0;}
char vg_get_pixel_buffer(unsigned long x,unsigned long y){assert(x<4&&y<4);return pixels[y*4+x];}
int main(void){
 char *picture[]={"2 2 2",". 0","# 43",".#","#."};Sprite *s=create_sprite(picture,1,1);assert(s&&s->width==2&&s->height==2);draw_sprite(s);assert(writes==2&&pixels[1*4+2]==43&&pixels[2*4+1]==43&&pixels[1*4+1]==0);
 memset(pixels,0,sizeof(pixels));writes=0;assert(draw_battleship(s)==0);assert(writes==2);pixels[1*4+2]=63;assert(draw_battleship(s)==1);assert(pixels[1*4+2]==43);free(s->map);free(s);return 0;}
