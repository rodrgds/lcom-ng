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
