#include GRAPHICS_SOURCE
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>
GameStates state = MENU_START;
int mouse_x,mouse_y,score[2],money,bet,result,fakeScore;
int player_cards[5],dealer_cards[5],player_c_count,dealer_c_count;
bool dealer_hidden;
static uint32_t *guarded_pixels(unsigned count,void **allocation,size_t *length) {
  size_t page=(size_t)sysconf(_SC_PAGESIZE);
  *length=2*page;*allocation=mmap(NULL,*length,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
  assert(*allocation!=MAP_FAILED&&count*sizeof(uint32_t)<page);
  assert(mprotect((char*)*allocation+page,page,PROT_NONE)==0);
  uint32_t *pixels=(uint32_t*)((char*)*allocation+page)-count;
  for(unsigned i=0;i<count;i++)pixels[i]=0x11220000u+i;
  return pixels;
}
int main(int argc,char **argv) {
  enum{WIDTH=32,HEIGHT=20,IMAGE_W=5,IMAGE_H=3,GUARD=16};
  uint32_t frame[WIDTH*HEIGHT+2*GUARD];for(unsigned i=0;i<sizeof frame/sizeof *frame;i++)frame[i]=0xa5a5a5a5u;
  video_mem=frame+GUARD;vmi.XResolution=WIDTH;vmi.YResolution=HEIGHT;bytespp=4;
  void *allocation;size_t length;uint32_t *source=guarded_pixels(IMAGE_W*IMAGE_H,&allocation,&length);
  xpm_image_t image={.width=IMAGE_W,.height=IMAGE_H,.type=XPM_8_8_8_8,.size=IMAGE_W*IMAGE_H*4,.bytes=(uint8_t*)source};
  int left,top;
  if(argc>1&&!strcmp(argv[1],"number")) {
    numbers[0]=numbers[3]=image;left=8;top=2;drawNumbers(3,0,left,top);
  } else {
    pixmap_list[MENU_START]=image;cursor.width=cursor.height=0;check_state=-1;
    left=(WIDTH-IMAGE_W)/2;top=(HEIGHT-IMAGE_H)/2;assert(draw_screen()==1);
  }
  for(unsigned y=0;y<HEIGHT;y++)for(unsigned x=0;x<WIDTH;x++){
    uint32_t expected=x>=(unsigned)left&&x<(unsigned)(left+IMAGE_W)&&y>=(unsigned)top&&y<(unsigned)(top+IMAGE_H)?source[(y-top)*IMAGE_W+x-left]:0xa5a5a5a5u;
    assert(video_mem[y*WIDTH+x]==expected&&"A 32-bit logical pixel must copy exactly one source pixel, including the last glyph column and row");
  }
  for(unsigned i=0;i<GUARD;i++)assert(frame[i]==0xa5a5a5a5u&&frame[GUARD+WIDTH*HEIGHT+i]==0xa5a5a5a5u);
  assert(munmap(allocation,length)==0);return 0;
}
