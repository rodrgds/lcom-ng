#include <minix/drivers.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  int hook = 5;
  if (sys_irqsetpolicy(0, IRQ_REENABLE, &hook) != OK || sys_irqenable(&hook) != OK) return 1;
  message msg; int status;
  if (argc > 1 && !strcmp(argv[1], "right")) {
    for (unsigned i = 0; i < 3; ++i)
      if (driver_receive(ANY, &msg, &status) != OK || msg.m_notify.interrupts != BIT(5)) return 2;
    puts("independent right guest received timer IRQs");
  } else {
    asm("cli");
    asm("cli");
    if (tickdelay(10) != OK) return 3;
    asm("sti");
    if (driver_receive(ANY, &msg, &status) != OK || msg.m_notify.interrupts != BIT(5)) return 4;
    puts("interrupt flag enabled after CLI/CLI/STI");
  }
  return lcf_cleanup();
}
