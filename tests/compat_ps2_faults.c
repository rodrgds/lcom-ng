#include <lcom/lcf.h>

#define CHECK(condition) do { if (!(condition)) { fprintf(stderr, "PS/2 check failed at line %d: %s\n", __LINE__, #condition); return 1; } } while (0)

int main(void) {
  sef_startup();
  int keyboard = 8, mouse = 9, dropped = 0, accepted = 0;
  CHECK(sys_irqsetpolicy(1, IRQ_REENABLE, &keyboard) == OK);
  CHECK(sys_irqsetpolicy(12, IRQ_REENABLE, &mouse) == OK);
  for (int i = 0; i < 5; ++i) {
    message notification; int ipc_status;
    CHECK(driver_receive(ANY, &notification, &ipc_status) == OK && is_ipc_notify(ipc_status));
    uint32_t status, byte;
    CHECK(sys_inb(0x64, &status) == OK && (status & 1));
    CHECK(notification.m_notify.interrupts == BIT((status & 0x20) ? mouse : keyboard));
    CHECK(sys_inb(0x60, &byte) == OK);
    if (status & 0xc0) { dropped++; continue; }
    CHECK(byte == ((status & 0x20) ? 9 : 0x30)); accepted++;
  }
  CHECK(dropped == 3 && accepted == 2);
  uint32_t status; CHECK(sys_inb(0x64, &status) == OK && status == 0);
  CHECK(sys_irqrmpolicy(&keyboard) == OK && sys_irqrmpolicy(&mouse) == OK);
  puts("PS/2 faulty bytes discarded, healthy keyboard and mouse bytes accepted");
  return lcf_cleanup();
}
