#include <minix/drivers.h>
#include <sys/mman.h>
#include <vt_info.h>
#include <lcom/vbe.h>

int main(void) {
  sef_startup();
  vt_info_t info;
  vt_info_get(&info);
  if (info.scr_width != 80 || info.scr_lines != 25) return 1;
  unsigned char *text = vm_map_phys(SELF, info.vram_base, info.vram_size * 2);
  if (text == MAP_FAILED) return 2;
  for (unsigned i = 0; i < 2000; ++i) { text[i*2] = ' '; text[i*2+1] = 0x17; }
  text[0] = 0xDB; text[1] = 0x1e; /* CP437 block, bright yellow on blue. */
  text[3999] = 0x42; /* The bottom-right space has a red background. */
  int hook = 1;
  if (sys_irqsetpolicy(1, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook) || sys_irqenable(&hook)) return 3;
  if (lcom_vbe_present()) return 4;
  for (;;) {
    message msg;
    int status;
    if (driver_receive(ANY, &msg, &status)) return 5;
    if (!is_ipc_notify(status) || !(msg.NOTIFY_ARG & (1u << 1))) continue;
    uint32_t flags, key;
    if (sys_inb(0x64, &flags) || !(flags & 1) || (flags & 0x20)) continue;
    if (sys_inb(0x60, &key)) return 6;
    if (key == 0x22) { /* G enters a normal 640x480 VBE graphics mode. */
      lcom_vbe_mode_info_t mode;
      if (lcom_vbe_get_mode_info(0x112, &mode) || lcom_vbe_set_mode(mode.mode)) return 7;
      unsigned char *pixels = vm_map_phys(SELF, (void *)mode.framebuffer_phys, mode.framebuffer_size);
      if (pixels == MAP_FAILED) return 8;
      for (size_t i = 0; i < mode.framebuffer_size; i += 3) {
        pixels[i] = 73; pixels[i+1] = 37; pixels[i+2] = 19;
      }
      if (lcom_vbe_present() || vm_unmap_phys(SELF, pixels, mode.framebuffer_size)) return 9;
    } else if (key == 0x14) { /* T restores existing text, without redrawing it. */
      if (lcom_vbe_set_mode(3) || lcom_vbe_present()) return 10;
    } else if (key == 0x01) {
      if (sys_irqrmpolicy(&hook) || vm_unmap_phys(SELF, text, info.vram_size * 2)) return 11;
      return 0;
    }
  }
}
