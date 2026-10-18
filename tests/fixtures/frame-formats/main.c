#include <lcom/lcom.h>
#include <lcom/vbe.h>
#include <lcom/i8042.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const struct { uint16_t mode; uint8_t pixels[8]; } formats[] = {
  {0x200,{7,38,143,0,215,63,12,127}}, {0x14c,{7,38,143,255,215,63,12,0}},
  {0x115,{7,38,143,215,63,12}}, {0x118,{7,38,143,215,63,12}},
  {0x110,{0x84,0x10,0xe0,0x7f}}, {0x111,{0x84,0x20,0xe0,0xff}},
  {0x117,{0x84,0x20,0xe0,0xff}}, {0x11a,{0x84,0x20,0xe0,0xff}},
  {0x105,{56,72}}
};

static int show(unsigned index) {
  const unsigned format=index%9;
  lcom_vbe_mode_info_t info;
  if(lcom_vbe_set_mode(formats[format].mode)!=LCOM_OK || lcom_vbe_get_mode_info(formats[format].mode,&info)!=LCOM_OK)return 1;
  uint8_t *pixels=NULL;
  if(lcom_phys_map(info.framebuffer_phys,info.framebuffer_size,(void **)&pixels)!=LCOM_OK)return 1;
  for(unsigned y=0;y<info.height;++y)for(unsigned x=0;x<info.width;++x) {
    unsigned color=((x>=info.width/2)+(y%2))%2;
    memcpy(pixels+y*info.pitch+x*info.bytes_per_pixel,formats[format].pixels+color*info.bytes_per_pixel,info.bytes_per_pixel);
  }
  if(lcom_vbe_present()!=LCOM_OK)return 1;
  lcom_phys_unmap(pixels,info.framebuffer_size);
  printf("FRAME_FORMAT step=%u mode=%x\n",index,info.mode);
  return 0;
}

int main(void) {
  if(lcom_init()!=LCOM_OK)return 1;
  lcom_irq_t keyboard;
  if(lcom_irq_subscribe(KBC_IRQ,0,&keyboard)!=LCOM_OK)return 1;
  unsigned index=0;
  if(show(index))return 1;
  for(;;) {
    lcom_event_t event;
    if(lcom_event_wait(&event)!=LCOM_OK)return 1;
    if(!(event.irq_mask&keyboard.mask))continue;
    uint8_t byte;
    if(lcom_port_read8(KBC_OUT_BUF,&byte)!=LCOM_OK)return 1;
    if(byte!=0x1c)continue;
    if(++index==9) {
      volatile uint8_t *allocation=malloc(96u*1024*1024);
      if(!allocation)return 1;
      allocation[0]=1;allocation[96u*1024*1024-1]=2;
      puts("FRAME_FORMAT heap-grown");
    }
    if(show(index))return 1;
  }
}
