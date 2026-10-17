/* Original LCF timer helpers remain usable when a student supplies no body. */
#define KEYBOARD_IRQ 1 /* An independent student alias must not hide CLOCK_IRQ. */
#include <lcom/lcf.h>
#define CHECK(c) do { if (!(c)) { fprintf(stderr,"LCF timer check failed at line %d: %s\n",__LINE__,#c); return 1; } } while (0)

#ifdef LCOM_TEST_STRONG_TIMER
int timer_get_conf(uint8_t timer, uint8_t *status) {
  CHECK(timer == 1 && status);
  *status = 0xa5;
  return 73;
}
#endif

int main(void) {
  CHECK(CLOCK_IRQ == 0);
  /* Pull the real LCF object into both links, including the strong override. */
  CHECK(lcf_start(0, NULL) == EINVAL);
  uint8_t status = 0;
#ifdef LCOM_TEST_STRONG_TIMER
  CHECK(timer_get_conf(1, &status) == 73 && status == 0xa5);
#else
  CHECK(lcom_compat_init() == OK);
  const uint8_t commands[] = {0x36, 0x74, 0xb2};
  for (uint8_t timer = 0; timer < 3; ++timer) {
    CHECK(sys_outb(0x43, commands[timer]) == OK);
    CHECK(sys_outb(0x40 + timer, 0x34) == OK);
    CHECK(sys_outb(0x40 + timer, 0x12) == OK);
  }
  for (uint8_t timer = 0; timer < 3; ++timer) {
    CHECK(timer_get_conf(timer, &status) == OK);
    CHECK((status & 0x3f) == (commands[timer] & 0x3f));
  }
  status = 0x5a;
  CHECK(timer_get_conf(3, &status) != OK && status == 0x5a);
  CHECK(timer_get_conf(0, NULL) != OK);
  CHECK(lcf_cleanup() == OK);
#endif
  puts("LCF timer fallback and student override contract passed");
  return 0;
}
