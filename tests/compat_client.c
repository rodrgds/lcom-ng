/* Exercise the unchanged Minix API against the real runtime connection. */
#include <lcom/lcf.h>
#include <minix/syslib.h>
#include <minix/vm.h>
#include <machine/int86.h>

#define CHECK(condition) do { if (!(condition)) { fprintf(stderr, "compat check failed at line %d: %s\n", __LINE__, #condition); lcf_cleanup(); return 1; } } while (0)

int main(int argc, char **argv) {
  sef_startup();
  if (argc == 2 && !strcmp(argv[1], "--mouse-bootstrap")) {
    uint32_t status;
    CHECK(sys_inb(0x64, &status) == OK && !(status & BIT(0)));
    int mouse_hook = 11;
    CHECK(sys_irqsetpolicy(12, IRQ_REENABLE, &mouse_hook) == OK);
    lcom_event_t event;
    CHECK(lcom_delay_ticks(2, &event) == LCOM_OK);
    const char *option = getenv("LCOM_COMPAT_MOUSE_REPORTING");
    bool enabled = option && !strcmp(option, "1");
    CHECK(event.irq_mask == (enabled ? BIT(12) : 0));
    CHECK(sys_inb(0x64, &status) == OK);
    CHECK((status & BIT(0)) == (enabled ? BIT(0) : 0));
    if (enabled) {
      const uint8_t expected[] = {0x29, 0x11, 0xfa};
      for (size_t i = 0; i < sizeof(expected); ++i) {
        uint32_t byte;
        CHECK(sys_inb(0x60, &byte) == OK && byte == expected[i]);
      }
    }
    CHECK(sys_irqrmpolicy(&mouse_hook) == OK);
    lcf_cleanup(); puts("Mouse startup contract passed"); return 0;
  }
  CHECK(lcf_set_language("EN-US") == true);
  CHECK(lcf_set_language("PT-PT") == true);
  CHECK(lcf_set_language("unknown") == false);
  /* Real sys_outb/sys_inb requests reach the VGA DAC in both runtime owners. */
  CHECK(sys_outb(0x3c8, 254) == OK);
  CHECK(sys_outb(0x3c9, 63) == OK && sys_outb(0x3c9, 21) == OK && sys_outb(0x3c9, 5) == OK);
  uint32_t dac_component;
  CHECK(sys_inb(0x3c8, &dac_component) == OK && dac_component == 255);
  CHECK(sys_outb(0x3c7, 254) == OK);
  CHECK(sys_inb(0x3c7, &dac_component) == OK && dac_component == 3);
  CHECK(sys_inb(0x3c9, &dac_component) == OK && dac_component == 63);
  CHECK(sys_inb(0x3c9, &dac_component) == OK && dac_component == 21);
  CHECK(sys_inb(0x3c9, &dac_component) == OK && dac_component == 5);
  CHECK(lcom_compat_sleep(0) == 0);
  CHECK(lcom_compat_usleep(0) == 0);
  CHECK(lcom_compat_usleep(1000000) == -1 && errno == EINVAL);
  CHECK(sizeof(reg86_t) == 40);
  CHECK(sizeof(vbe_mode_info_t) == 256);
  CHECK(micros_to_ticks(1) == 1 && micros_to_ticks(16667) == 2);
  CHECK(sys_inb(-1, NULL) == EINVAL);
  CHECK(MIN(7, 3) == 3 && MAX(7, 3) == 7);
  int macro_argument = 0;
  CHECK(MIN(++macro_argument, 9) == 2 && macro_argument == 2);
  CHECK(_ENDPOINT(2, 7) == 65543 && _ENDPOINT_G(65543) == 2);
  CHECK(NONE == 31743 && ANY == 31744 && SELF == 31742);
  CHECK(is_ipc_asynch(16) && !is_ipc_asynch(1));
  CHECK(IPC_STATUS_FLAGS_TEST(0x10004, IPC_FLG_MSG_FROM_KERNEL));
  unsigned long legacy_port_value = ULONG_MAX;
  CHECK(sys_inb(0x64, &legacy_port_value) == OK && legacy_port_value == 0);
  CHECK(sys_outb(0x70, 256) == EINVAL);
  CHECK(sys_privctl(SELF, 999, NULL) == ENOSYS);
  /* Numeric operands come from the FEUP Minix 3.4 headers, not shim macros. */
  struct minix_mem_range actual_range = {0xe0000000u, 0xe0001000u};
  CHECK(sys_privctl(31742, 6, &actual_range) == OK);
  CHECK(sys_enable_iop(31742) == OK);
  CHECK(sys_enable_iop(0) == ENOSYS);

  CHECK(sys_enable_iop(SELF) == OK && sys_enable_iop(ANY) == ENOSYS);

  /* BIOS controller data contains real-mode pointers based at physical zero. */
  mmap_t controller = {0};
  CHECK(lm_alloc(512, &controller) != NULL);
  CHECK((uint8_t *)lm_init(true) + controller.phys == (uint8_t *)controller.virt);
  reg86_t controller_regs = {0}; controller_regs.intno = 0x10; controller_regs.ax = 0x4f00;
  controller_regs.es = PB2BASE(controller.phys); controller_regs.di = PB2OFF(controller.phys);
  CHECK(sys_int86(&controller_regs) == OK && controller_regs.ax == 0x004f);
  uint8_t *controller_bytes = controller.virt;
  CHECK(!memcmp(controller_bytes, "VESA", 4));
  CHECK(controller_bytes[4] == 0 && controller_bytes[5] == 3);
  uint32_t modes_far, oem_far;
  memcpy(&modes_far, controller_bytes + 14, sizeof(modes_far));
  memcpy(&oem_far, controller_bytes + 6, sizeof(oem_far));
  uint32_t modes_phys = ((modes_far >> 16) << 4) + (modes_far & 0xffff);
  uint32_t oem_phys = ((oem_far >> 16) << 4) + (oem_far & 0xffff);
  uint16_t *modes = (uint16_t *)((uint8_t *)lm_init(true) + modes_phys);
  CHECK(modes[0] == 0x105);
  bool has_rgb565 = false; size_t mode_count = 0;
  while (mode_count < 32 && modes[mode_count] != 0xffff) {
    if (modes[mode_count] == 0x11a) has_rgb565 = true;
    ++mode_count;
  }
  CHECK(has_rgb565 && mode_count < 32);
  CHECK(!strcmp((char *)lm_init(true) + oem_phys, "lcom-ng virtual VBE"));
  CHECK(controller_bytes[18] == 0 && controller_bytes[19] == 1);
  // Independent standard VBE mode contracts, including real BounceBall 0x11B.
  const struct { uint16_t number, width, height; uint8_t depth; } common_modes[] = {
    {0x100, 640, 400, 8}, {0x101, 640, 480, 8}, {0x103, 800, 600, 8},
    {0x107, 1280, 1024, 8}, {0x10d, 320, 200, 15}, {0x10e, 320, 200, 16},
    {0x10f, 320, 200, 24}, {0x112, 640, 480, 24}, {0x113, 800, 600, 15},
    {0x114, 800, 600, 16}, {0x116, 1024, 768, 15}, {0x119, 1280, 1024, 15},
    {0x11b, 1280, 1024, 24}, {0x11c, 1600, 1200, 8}, {0x11d, 1600, 1200, 15},
    {0x11e, 1600, 1200, 16}, {0x11f, 1600, 1200, 24}
  };
  for (size_t i = 0; i < sizeof(common_modes) / sizeof(common_modes[0]); ++i) {
    bool advertised = false;
    for (size_t j = 0; j < mode_count; ++j) if (modes[j] == common_modes[i].number) advertised = true;
    CHECK(advertised);
    vbe_mode_info_t mode;
    CHECK(vbe_get_mode_info(common_modes[i].number, &mode) == OK);
    CHECK(mode.XResolution == common_modes[i].width && mode.YResolution == common_modes[i].height);
    CHECK(mode.BitsPerPixel == common_modes[i].depth);
    CHECK(mode.BytesPerScanLine == common_modes[i].width * ((common_modes[i].depth + 7) / 8));
    CHECK(mode.MemoryModel == (common_modes[i].depth == 8 ? 4 : 6));
    CHECK(mode.PhysBasePtr == 0xe0000000 && mode.LinBytesPerScanLine == mode.BytesPerScanLine);
    reg86_t set = {0}; set.intno = 0x10; set.ax = 0x4f02; set.bx = common_modes[i].number | BIT(14);
    CHECK(sys_int86(&set) == OK && set.ax == 0x004f);
  }
  CHECK(lm_free(&controller));
  /* RTC update interrupts cross the same Minix hook mapping as other devices. */
  int rtc_hook = 5; uint32_t rtc_control, rtc_seconds, rtc_after, rtc_flags;
  CHECK(sys_irqsetpolicy(8, IRQ_REENABLE, &rtc_hook) == OK);
  CHECK(sys_outb(0x70, 0x0b) == OK && sys_inb(0x71, &rtc_control) == OK);
  CHECK(sys_outb(0x71, 0x16) == OK); /* UIE, binary, 24-hour. */
  CHECK(sys_outb(0x70, 0) == OK && sys_inb(0x71, &rtc_seconds) == OK);
  lcom_event_t rtc_event;
  CHECK(lcom_delay_ticks(60, &rtc_event) == LCOM_OK && rtc_event.irq_mask == BIT(8));
  CHECK(sys_outb(0x70, 0) == OK && sys_inb(0x71, &rtc_after) == OK);
  CHECK(rtc_after == (rtc_seconds + 1) % 60);
  CHECK(sys_outb(0x70, 0x0c) == OK && sys_inb(0x71, &rtc_flags) == OK);
  CHECK((rtc_flags & 0x90) == 0x90); /* IRQF plus UF. */
  CHECK(sys_inb(0x71, &rtc_flags) == OK && rtc_flags == 0);
  CHECK(sys_outb(0x70, 0x0b) == OK && sys_outb(0x71, rtc_control) == OK);
  CHECK(sys_irqrmpolicy(&rtc_hook) == OK);
  /* Mode 0 must notify once per reload through both runtime transports. */
  int one_shot_hook = 6; uint32_t one_shot_status;
  CHECK(sys_irqsetpolicy(0, IRQ_REENABLE, &one_shot_hook) == OK);
  CHECK(sys_outb(0x43, 0x30) == OK && sys_outb(0x40, 0xae) == OK && sys_outb(0x40, 0x4d) == OK);
  lcom_event_t one_shot_event;
  CHECK(lcom_delay_ticks(2, &one_shot_event) == LCOM_OK && one_shot_event.irq_mask == BIT(0));
  CHECK(lcom_delay_ticks(2, &one_shot_event) == LCOM_OK && one_shot_event.irq_mask == 0);
  CHECK(sys_outb(0x43, 0xe2) == OK && sys_inb(0x40, &one_shot_status) == OK);
  CHECK((one_shot_status & 0xc0) == 0x80);
  CHECK(sys_outb(0x43, 0x36) == OK && sys_outb(0x40, 0xae) == OK && sys_outb(0x40, 0x4d) == OK);
  CHECK(sys_irqrmpolicy(&one_shot_hook) == OK);
  mmap_t allocation = {0};
  CHECK(lm_alloc(sizeof(vbe_mode_info_t), &allocation) != NULL);
  reg86_t registers = {0};
  registers.intno = 0x10;
  registers.ax = 0x4f01;
  CHECK(registers.ah == 0x4f && registers.al == 1);
  registers.cx = 0x115;
  registers.es = PB2BASE(allocation.phys);
  registers.di = PB2OFF(allocation.phys);
  CHECK(sys_int86(&registers) == OK && registers.ax == 0x004f);
  vbe_mode_info_t *info = allocation.virtual;
  CHECK(info->XResolution == 800 && info->YResolution == 600);
  CHECK(info->BitsPerPixel == 24 && info->BytesPerScanLine == 2400);
  CHECK(info->RedFieldPosition == 16 && info->BlueFieldPosition == 0);
  CHECK(info->LinNumberOfImagePages >= 1);

  /* Students map both pages before issuing the BIOS mode switch. */
  size_t page = (size_t)info->BytesPerScanLine * info->YResolution;
  uint8_t *vram = vm_map_phys(SELF, (void *)(uintptr_t)info->PhysBasePtr, page * 2);
  CHECK(vram != MAP_FAILED);
  uint8_t *second_page = vm_map_phys(SELF, (void *)((uintptr_t)info->PhysBasePtr + page), page);
  CHECK(second_page != MAP_FAILED);
  second_page[0] = 0x34;
  CHECK(vram[page] == 0x34);
  CHECK(vm_unmap_phys(SELF, second_page, page + 1) != OK);
  CHECK(vm_unmap_phys(SELF, second_page, page) == OK);
  registers = (reg86_t){0}; registers.intno = 0x10; registers.ax = 0x4f02; registers.bx = 0x4115;
  CHECK(sys_int86(&registers) == OK);
  vram[0] = 0x12; vram[page] = 0xab;
  registers = (reg86_t){0}; registers.intno = 0x10; registers.ax = 0x4f07; registers.dx = 600;
  CHECK(sys_int86(&registers) == OK);
  registers.ax = 0x4f07; registers.bl = 1;
  CHECK(sys_int86(&registers) == OK && registers.cx == 0 && registers.dx == 600);
  registers.ax = 0x4f03;
  CHECK(sys_int86(&registers) == ENOSYS && registers.ax == 0x014f);
  CHECK(vm_unmap_phys(SELF, vram, page * 2) == OK);
  CHECK(lm_free(&allocation) && !lm_free(&allocation));

  int hook = 7;
  CHECK(sys_irqsetpolicy(0, IRQ_REENABLE, &hook) == OK && hook == 7);
  CHECK(tickdelay(3) == OK);
  message msg; int status;
  /* The delay must preserve notifications under the student's requested bit. */
  CHECK(driver_receive(31744, &msg, &status) == OK);
  CHECK(is_ipc_notify(status) && msg.m_source == HARDWARE);
  CHECK(msg.m_type == 0x1000 && is_notify(msg.m_type));
  CHECK(msg.m_source == -1 && status == 4);
  CHECK(is_ipc_notify(4) && is_ipc_notify(0x10004));
  CHECK(!is_ipc_notify(0x1000) && !is_ipc_notify(3));
  CHECK(_ENDPOINT_P(65543) == 7 && _ENDPOINT_P(65535) == -1);
  CHECK(!is_notify(0) && !is_notify(NOTIFY_MESSAGE + 0x100));
  CHECK(msg.NOTIFY_ARG == BIT(7));
  int mouse_hook = 11;
  CHECK(sys_irqsetpolicy(12, IRQ_REENABLE, &mouse_hook) == OK);
  CHECK(sys_outb(0x64, 0xd4) == OK && sys_outb(0x60, 0xf4) == OK);
  /* An unread mouse ACK keeps IRQ12 asserted. A delay must still advance time,
   * and retain both the mouse and timer notification for driver_receive. */
  CHECK(tickdelay(3) == OK);
  CHECK(driver_receive(ANY, &msg, &status) == OK);
  CHECK(msg.m_notify.interrupts == (BIT(7) | BIT(11)));
  uint32_t ack;
  CHECK(sys_inb(0x60, &ack) == OK && ack == 0xfa);
  CHECK(sys_irqrmpolicy(&mouse_hook) == OK);
  CHECK(sys_irqsetpolicy(12, IRQ_REENABLE, &mouse_hook) == OK);
  CHECK(sys_outb(0x64, 0xd4) == OK && sys_outb(0x60, 0xf4) == OK);
  CHECK(sys_irqdisable(&mouse_hook) == OK);
  /* A disabled level IRQ cannot starve another device's notification. */
  CHECK(driver_receive(ANY, &msg, &status) == OK && msg.m_notify.interrupts == BIT(7));
  CHECK(sys_irqenable(&mouse_hook) == OK);
  CHECK(driver_receive(ANY, &msg, &status) == OK && (msg.m_notify.interrupts & BIT(11)));
  CHECK(sys_inb(0x60, &ack) == OK && ack == 0xfa);
  CHECK(sys_irqrmpolicy(&mouse_hook) == OK);
  CHECK(sys_irqrmpolicy(&hook) == OK);
  CHECK(sys_irqsetpolicy(0, 0, &hook) == OK);
  CHECK(driver_receive(ANY, &msg, &status) == OK && msg.m_notify.interrupts == BIT(7));
  CHECK(sys_irqenable(&hook) == OK);
  CHECK(driver_receive(ANY, &msg, &status) == OK && msg.m_notify.interrupts == BIT(7));
  CHECK(sys_irqrmpolicy(&hook) == OK);
  CHECK(sys_irqenable(&hook) == EINVAL);
  /* COM2 loopback with a four-byte FIFO trigger must time out a short
   * message after four 8N1 characters at 300 baud, then clear on RBR. */
  int serial_hook = 3;
  CHECK(sys_irqsetpolicy(3, IRQ_REENABLE, &serial_hook) == OK);
  CHECK(sys_outb(0x2fb, 0x83) == OK);
  CHECK(sys_outb(0x2f8, 0x80) == OK && sys_outb(0x2f9, 1) == OK);
  CHECK(sys_outb(0x2fb, 3) == OK && sys_outb(0x2fc, 0x10) == OK);
  CHECK(sys_outb(0x2fa, 0x41) == OK && sys_outb(0x2f9, 1) == OK);
  CHECK(sys_outb(0x2f8, 'A') == OK && sys_outb(0x2f8, 'B') == OK);
  uint32_t serial_iir, serial_byte;
  CHECK(sys_inb(0x2fa, &serial_iir) == OK && (serial_iir & 15) == 1);
  CHECK(tickdelay(9) == OK);
  CHECK(driver_receive(ANY, &msg, &status) == OK && msg.m_notify.interrupts == BIT(3));
  CHECK(sys_inb(0x2fa, &serial_iir) == OK && (serial_iir & 15) == 12);
  CHECK(sys_inb(0x2f8, &serial_byte) == OK && serial_byte == 'A');
  CHECK(sys_inb(0x2fa, &serial_iir) == OK && (serial_iir & 15) == 1);
  CHECK(sys_outb(0x2fa, 7) == OK && sys_outb(0x2f9, 0) == OK);
  CHECK(sys_irqrmpolicy(&serial_hook) == OK);
  /* Minix clock ticks stay at 60 Hz when a student programs PIT0 to 120 Hz. */
  CHECK(sys_irqsetpolicy(0, IRQ_REENABLE, &hook) == OK);
  CHECK(sys_outb(0x43, 0x36) == OK);
  const uint16_t divisor = (uint16_t)(1193182u / 120u);
  CHECK(sys_outb(0x40, divisor & 0xff) == OK && sys_outb(0x40, divisor >> 8) == OK);
  lcom_event_t before, after;
  CHECK(lcom_delay_ticks(0, &before) == LCOM_OK);
  CHECK(lcom_delay_ticks(3, &after) == LCOM_OK);
  CHECK(after.tick - before.tick == 6 && after.irq_mask == BIT(0));
  CHECK(sys_irqrmpolicy(&hook) == OK);
  CHECK(vg_exit() == OK);
  lcf_cleanup();
  puts("Minix compatibility contracts passed");
  return 0;
}
