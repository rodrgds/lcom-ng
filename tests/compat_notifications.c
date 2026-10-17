/* Minix merges pending notifications from HARDWARE while a process delays. */
#include <lcom/compat.h>
#include <lcom/compat_asm.h>
#include <stdio.h>

#define CHECK(c) do { if (!(c)) { fprintf(stderr, "notification check failed at line %d: %s\n", __LINE__, #c); return 1; } } while (0)
static unsigned delays, waits;
static bool fail_wait;
int lcom_init(void) { return LCOM_OK; }
void lcom_exit(void) {}
int lcom_vbe_set_mode(uint16_t mode) { (void)mode; return LCOM_OK; }
int lcom_vbe_present(void) { return LCOM_OK; }
int lcom_vbe_get_mode_info(uint16_t mode, lcom_vbe_mode_info_t *out) { (void)mode; (void)out; return LCOM_UNSUPPORTED; }
int lcom_vbe_set_display_start(uint16_t x, uint16_t y) { (void)x; (void)y; return LCOM_UNSUPPORTED; }
int lcom_vbe_dac_format(uint8_t op, uint8_t request, uint8_t *out) { (void)op; (void)request; (void)out; return LCOM_UNSUPPORTED; }
int lcom_vbe_scanline(uint8_t op, uint16_t value, lcom_vbe_scanline_info_t *out) { (void)op; (void)value; (void)out; return LCOM_UNSUPPORTED; }
int lcom_irq_subscribe(uint8_t irq, uint32_t flags, lcom_irq_t *out) {
  (void)flags; out->mask = BIT(irq); return LCOM_OK;
}
int lcom_irq_unsubscribe(lcom_irq_t *irq) { (void)irq; return LCOM_OK; }
int lcom_port_read8(uint16_t port, uint8_t *value) { (void)port; *value = 0; return LCOM_OK; }
int lcom_port_write8(uint16_t port, uint8_t value) { (void)port; (void)value; return LCOM_OK; }
/* PE linkers can retain unused video adapters from compat.c. */
int lcom_phys_map(uintptr_t phys, size_t len, void **out) {
  (void)phys; (void)len; (void)out; abort();
}
int lcom_phys_unmap(void *ptr, size_t len) { (void)ptr; (void)len; return LCOM_OK; }
int lcom_compat_output_log_stop(void) { return 0; }
int lcom_delay_ticks(uint64_t ticks, lcom_event_t *event) {
  (void)ticks;
  ++delays;
  /* Repeated timer IRQs, with a keyboard IRQ during the third delay. */
  event->irq_mask = delays == 5 ? 0 : BIT(0) | (delays == 3 ? BIT(1) : 0);
  event->tick = delays;
  return LCOM_OK;
}
int lcom_event_wait(lcom_event_t *event) {
  ++waits;
  if (fail_wait) return LCOM_ERR;
  event->irq_mask = waits == 1 ? BIT(4) : BIT(0);
  event->tick = 100 + waits;
  return LCOM_OK;
}
int main(void) {
  int timer = 5, keyboard = 9, serial = 13;
  CHECK(sys_irqsetpolicy(0, IRQ_REENABLE, &timer) == OK);
  CHECK(sys_irqsetpolicy(1, IRQ_REENABLE, &keyboard) == OK);
  CHECK(sys_irqsetpolicy(4, IRQ_REENABLE, &serial) == OK);
  CHECK(sys_irqenable(&timer) == OK && sys_irqenable(&keyboard) == OK && sys_irqenable(&serial) == OK);
  for (unsigned i = 0; i < 5; ++i) CHECK(tickdelay(1) == OK);
  message msg; int status;
  CHECK(driver_receive(ANY, &msg, &status) == OK);
  CHECK(is_ipc_notify(status) && msg.m_source == HARDWARE);
  CHECK(msg.m_notify.interrupts == (BIT(5) | BIT(9)) && waits == 0);
  /* A second receive must wait for a fresh source, not replay stale timers. */
  CHECK(driver_receive(ANY, &msg, &status) == OK);
  CHECK(msg.m_notify.interrupts == BIT(13) && waits == 1);
  CHECK(tickdelay(1) == OK && lcf_cleanup() == OK);
  timer = 7;
  CHECK(sys_irqsetpolicy(0, IRQ_REENABLE, &timer) == OK && sys_irqenable(&timer) == OK);
  CHECK(driver_receive(ANY, &msg, &status) == OK);
  CHECK(msg.m_notify.interrupts == BIT(7) && waits == 2);
  lcom_compat_cpu_interrupts(0);
  lcom_compat_cpu_interrupts(0);
  CHECK(tickdelay(1) == OK);
  fail_wait = true;
  /* A host wait error distinguishes blocked delivery from a leaked timer.
   * Neither blocked delivery nor a failed wait may discard pending IRQs. */
  CHECK(driver_receive(ANY, &msg, &status) == EIO && waits == 3);
  lcom_compat_cpu_interrupts(1);
  CHECK(driver_receive(ANY, &msg, &status) == OK);
  CHECK(msg.m_notify.interrupts == BIT(7) && waits == 3);
  /* CLI/CLI/STI assigns one flag; it is not a nesting counter. */
  CHECK(tickdelay(1) == OK);
  lcom_compat_cpu_interrupts(0);
  lcom_compat_cpu_interrupts(0);
  lcom_compat_cpu_interrupts(1);
  CHECK(driver_receive(ANY, &msg, &status) == OK);
  CHECK(msg.m_notify.interrupts == BIT(7) && waits == 3);
  lcom_compat_cpu_interrupts(0);
  CHECK(lcf_cleanup() == OK);
  timer = 8;
  CHECK(sys_irqsetpolicy(0, IRQ_REENABLE, &timer) == OK && sys_irqenable(&timer) == OK);
  CHECK(tickdelay(1) == OK && driver_receive(ANY, &msg, &status) == OK);
  CHECK(msg.m_notify.interrupts == BIT(8) && waits == 3);
  CHECK(lcf_cleanup() == OK);
  puts("coalesced Minix notifications and CPU interrupt flag passed");
  return 0;
}
