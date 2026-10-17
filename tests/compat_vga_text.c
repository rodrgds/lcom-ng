#include <minix/drivers.h>
#include <sys/mman.h>
#include <vt_info.h>
#include <lcom/vbe.h>
#include <assert.h>
int main(void) {
  sef_startup();
  vt_info_t info;
  vt_info_get(&info);
  assert(info.scr_width == 80 && info.scr_lines == 25);
  assert(info.vram_base == (void *)0xB8000 && info.vram_size == 0x2000);
  char *text = vm_map_phys(SELF, info.vram_base, info.vram_size * 2);
  assert(text != MAP_FAILED);
  for (int i = 0; i < 2000; ++i) { text[i*2] = ' '; text[i*2+1] = 0x17; }
  text[0] = (char)0xDB; text[1] = 0x1e;
  text[3999] = 0x42;
  assert(tickdelay(1) == OK);
  assert(lcom_vbe_set_mode(0x115) == LCOM_OK);
  assert((unsigned char)text[0] == 0xDB && text[1] == 0x1e);
  assert(lcom_vbe_set_mode(3) == LCOM_OK);
  assert(tickdelay(1) == OK);
  return 0;
}
