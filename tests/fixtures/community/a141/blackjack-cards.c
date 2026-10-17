#include GRAPHICS_SOURCE
#include <assert.h>
static unsigned loaded;
uint8_t*xpm_load(xpm_map_t map,enum xpm_image_type type,xpm_image_t*image){
 assert(map&&type==XPM_8_8_8_8);
 assert((uintptr_t)image>=(uintptr_t)cards_list&&(uintptr_t)image+sizeof *image<=(uintptr_t)cards_list+sizeof cards_list&&"53 loaded card images require 53 owning slots, including the card back");
 *image=(xpm_image_t){.width=95,.height=145,.type=type};loaded++;return NULL;
}
int main(int argc,char**argv){
 if(argc>1&&!strcmp(argv[1],"load")){loadCards();assert(loaded==53);return 0;}
 enum{WIDTH=620,HEIGHT=8};uint32_t*frame=malloc((WIDTH*HEIGHT+32)*sizeof *frame);assert(frame);for(unsigned i=0;i<WIDTH*HEIGHT+32;i++)frame[i]=0xa5a5a5a5;
 uint32_t source[]={0x010203,0x102030,0x203040,0x304050,0x405060,0x506070,0x607080,0x708090,0x8090a0,0x90a0b0,0xa0b0c0,0xb0c0d0,0xc0d0e0,0xd0e0f0,0xe0f001};
 cards_list[0]=(xpm_image_t){.width=5,.height=3,.type=XPM_8_8_8_8,.size=sizeof source,.bytes=(uint8_t*)source};video_mem=frame+16;vmi.XResolution=WIDTH;vmi.YResolution=HEIGHT;bytespp=4;draw_card(0,0,2);
 for(unsigned y=0;y<HEIGHT;y++)for(unsigned x=0;x<WIDTH;x++){uint32_t expected=x>=481&&x<486&&y>=2&&y<5?source[(y-2)*5+x-481]:0xa5a5a5a5;assert(video_mem[y*WIDTH+x]==expected&&"card renderer must use the selected image row stride and write exactly its own complete pixels");}
 for(unsigned i=0;i<16;i++)assert(frame[i]==0xa5a5a5a5&&frame[16+WIDTH*HEIGHT+i]==0xa5a5a5a5);free(frame);return 0;
}
