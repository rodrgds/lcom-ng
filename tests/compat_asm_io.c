/* Private instruction helpers preserve exact bytes and never invent I/O data. */
#include <lcom/compat.h>
#include <lcom/compat_asm.h>
#include <assert.h>
#include <string.h>
static bool failing;
int lcom_init(void) { return LCOM_OK; }
int lcom_vbe_set_mode(uint16_t mode) { assert(mode == 3); return LCOM_OK; }
int lcom_port_read8(uint16_t port, uint8_t *value) {
  assert(port == 0x1234); if (failing) return LCOM_ERR;
  *value = 0xe7; return LCOM_OK;
}
int lcom_port_write8(uint16_t port, uint8_t value) {
  assert(port == 0x1234 && value == 0xa5);
  return failing ? LCOM_ERR : LCOM_OK;
}
/* PE linkers can retain unused adapters. None belongs to byte-port I/O. */
void lcom_exit(void) { abort(); }
int lcom_irq_subscribe(uint8_t irq, uint32_t flags, lcom_irq_t *out) {
  (void)irq; (void)flags; (void)out; abort();
}
int lcom_irq_unsubscribe(lcom_irq_t *irq) { (void)irq; abort(); }
int lcom_event_wait(lcom_event_t *event) { (void)event; abort(); }
int lcom_delay_ticks(uint64_t ticks, lcom_event_t *event) {
  (void)ticks; (void)event; abort();
}
int lcom_phys_map(uintptr_t phys, size_t len, void **out) {
  (void)phys; (void)len; (void)out; abort();
}
int lcom_phys_unmap(void *ptr, size_t len) { (void)ptr; (void)len; abort(); }
int lcom_vbe_get_mode_info(uint16_t mode, lcom_vbe_mode_info_t *out) {
  (void)mode; (void)out; abort();
}
int lcom_vbe_present(void) { abort(); }
int lcom_vbe_set_display_start(uint16_t x, uint16_t y) { (void)x; (void)y; abort(); }
int lcom_vbe_dac_format(uint8_t operation, uint8_t requested, uint8_t *actual) {
  (void)operation; (void)requested; (void)actual; abort();
}
int lcom_vbe_scanline(uint8_t operation, uint16_t value, lcom_vbe_scanline_info_t *out) {
  (void)operation; (void)value; (void)out; abort();
}
int lcom_compat_output_log_stop(void) { abort(); }
int main(int argc, char **argv) {
  failing = argc > 1;
  if (argc > 1 && !strcmp(argv[1], "read-failure")) {
    (void)lcom_compat_asm_in8(0x1234);
    puts("UNEXPECTED_RETURN"); return 99;
  }
  lcom_compat_asm_out8(0x1234, 0xa5);
  if (failing) { puts("UNEXPECTED_RETURN"); return 99; }
  assert(lcom_compat_asm_in8(0x1234) == 0xe7);
  puts("instruction byte I/O and exact16-bit port passed");
  return 0;
}
