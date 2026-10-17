#include VIDEO_SOURCE
#include <assert.h>
#include <sys/mman.h>
#include <unistd.h>
int main(void) {
  enum {WIDTH=1152,HEIGHT=864};const size_t bytes=(size_t)WIDTH*HEIGHT*4,page=(size_t)sysconf(_SC_PAGESIZE);
  assert(bytes%page==0);
  uint8_t *frame=mmap(NULL,bytes+page,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);assert(frame!=MAP_FAILED);
  assert(mprotect(frame+bytes,page,PROT_NONE)==0);
  mode_info.XResolution=WIDTH;mode_info.YResolution=HEIGHT;n_bytes_per_pixel=4;video_mem2=frame;
  const uint32_t first=0x12345678,last=0x89abcdef;
  assert(vg_draw_pixel(0,0,first)==0);assert(vg_draw_pixel(WIDTH-1,HEIGHT-1,last)==0);
  assert(*(uint32_t*)frame==first&&*(uint32_t*)(frame+bytes-4)==last);
  /* Exact actual ARM64 crash coordinate: first excluded row, x784. */
  assert(vg_draw_pixel(784,HEIGHT,0x123456)==1);
  assert(vg_draw_pixel(WIDTH,HEIGHT-1,0x123456)==1);
  assert(vg_draw_pixel(WIDTH+1,HEIGHT-1,0x123456)==1);
  assert(*(uint32_t*)(frame+bytes-4)==last);
  for(size_t i=4;i<bytes-4;i++)assert(frame[i]==0);
  assert(munmap(frame,bytes+page)==0);return 0;
}
