#include "../runtime/core/Machine.hpp"
#include "../runtime/core/MousePacketScheduler.hpp"
#include "../runtime/core/Script.hpp"
#include "../runtime/core/ProtocolIO.hpp"
#include "../runtime/backends/AudioBackend.hpp"

#include <lcom/i8042.h>
#include <lcom/i8254.h>
#include <lcom/ac97.h>
#include <lcom/rtc.h>
#include <lcom/uart16550.h>
#include <lcom/vbe.h>

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

static int g_failures = 0;

#define CHECK(expr)                                                                  \
  do {                                                                               \
    if (!(expr)) {                                                                   \
      std::cerr << "CHECK failed at " << __FILE__ << ":" << __LINE__ << ": "       \
                << #expr << "\n";                                                   \
      g_failures++;                                                                  \
    }                                                                                \
  } while (0)

static void test_irq_controller_shape() {
  lcom::Machine machine;
  lcom::IrqSubscription sub;
  CHECK(machine.subscribeIrq(TIMER0_IRQ, sub));
  CHECK(sub.irq == TIMER0_IRQ);
  CHECK(sub.bit_no == TIMER0_IRQ);
  CHECK(sub.mask == BIT(TIMER0_IRQ));
  machine.advanceTick();
  CHECK(machine.pendingIrqs() == BIT(TIMER0_IRQ));
  CHECK(machine.consumePendingIrqs() == BIT(TIMER0_IRQ));
  CHECK(machine.pendingIrqs() == 0);
}

static void test_pit_programming_and_readback() {
  lcom::Machine machine;
  CHECK(machine.writePort8(TIMER_CTRL, TIMER_SEL0 | TIMER_LSB_MSB | TIMER_SQR_WAVE | TIMER_BIN));
  CHECK(machine.writePort8(TIMER_0, 0x34));
  CHECK(machine.writePort8(TIMER_0, 0x12));
  CHECK(machine.pit().divisor(0) == 0x1234);
  CHECK(machine.pit().channelFrequency(0) == TIMER_FREQ / 0x1234);

  CHECK(machine.writePort8(TIMER_0, 0x00));
  CHECK(machine.writePort8(TIMER_0, 0x00));
  CHECK(machine.pit().divisor(0) == 0);
  CHECK(machine.pit().channelFrequency(0) == TIMER_FREQ / 65536u);

  CHECK(machine.writePort8(TIMER_CTRL, TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(0)));
  uint8_t status = 0;
  CHECK(machine.readPort8(TIMER_0, status));
  CHECK((status & TIMER_LSB_MSB) == TIMER_LSB_MSB);
  CHECK(((status >> 1) & 0x07) == 3);
}

/* Intel 8254 count latches, independent read/write sequencing, and mode 0. */
static void test_pit_counter_latches_and_one_shot() {
  lcom::Machine machine;
  auto write = [&](uint16_t port, uint8_t byte) { CHECK(machine.writePort8(port, byte)); };
  auto read = [&](uint16_t port) { uint8_t byte = 0; CHECK(machine.readPort8(port, byte)); return byte; };
  auto status = [&](uint8_t channel) {
    write(0x43, static_cast<uint8_t>(0xe0 | (1u << (channel + 1)))); return read(0x40 + channel);
  };
  write(0x43, 0x34); write(0x40, 5); write(0x40, 0); // Timer 0 mode 2, five input clocks per virtual quantum.
  write(0x43, 0x74);
  CHECK((status(1) & 0x40) != 0);
  write(0x41, 0x34); CHECK((status(1) & 0x40) != 0);
  write(0x41, 0x12); CHECK((status(1) & 0x40) != 0);
  machine.advanceTick(); CHECK((status(1) & 0x40) == 0);
  write(0x43, 0x40); // Latch timer 1 without changing its mode or access format.
  for (int i = 0; i < 17; ++i) machine.advanceTick();
  CHECK(read(0x41) == 0x2f); // Frozen 0x1234 - 5 = 0x122f, even after another 85 clocks.
  write(0x43, 0x40); // A new latch is ignored until the pending high byte is read.
  for (int i = 0; i < 7; ++i) machine.advanceTick();
  CHECK(read(0x41) == 0x12);
  write(0x43, 0xc4); // Read back both status and count, status comes first.
  CHECK(read(0x41) == 0xb4);
  CHECK(read(0x41) == 0xb7 && read(0x41) == 0x11); // 0x1234 - 125 = 0x11b7.
  write(0x43, 0x74); write(0x41, 0xcd);
  (void)read(0x41); // A read between the two writes must not change the write phase.
  write(0x41, 0xab); machine.advanceTick(); write(0x43, 0x40);
  CHECK(read(0x41) == 0xc8 && read(0x41) == 0xab); // 0xabcd - 5.
  write(0x43, 0x54); write(0x41, 0x34); // LSB-only writes clear the unused MSB.
  machine.advanceTick(); write(0x43, 0x40); CHECK(read(0x41) == 0x2f);
  write(0x43, 0x64); write(0x41, 0x12); // MSB-only writes clear the unused LSB.
  machine.advanceTick(); write(0x43, 0x40); CHECK(read(0x41) == 0x11);

  auto readCount1 = [&] { write(0x43, 0x40); uint8_t low = read(0x41); return low | (static_cast<unsigned>(read(0x41)) << 8); };
  // An odd mode-3 count loads N-1 and spends the extra high clock at zero.
  write(0x43, 0x34); write(0x40, 3); write(0x40, 0);
  write(0x43, 0x76); write(0x41, 5); write(0x41, 0);
  machine.advanceTick(); CHECK(readCount1() == 4);
  machine.advanceTick(); CHECK(readCount1() == 2);
  machine.advanceTick(); CHECK(readCount1() == 2);
  machine.advanceTick(); CHECK(readCount1() == 0);

  lcom::IrqSubscription subscription;
  CHECK(machine.subscribeIrq(0, subscription)); (void)machine.consumePendingIrqs();
  write(0x43, 0x30); CHECK((status(0) & 0xc0) == 0x40);
  write(0x40, 0xae); write(0x40, 0x4d);
  machine.advanceTick(); CHECK((machine.consumePendingIrqs() & 1u) != 0);
  CHECK((status(0) & 0xc0) == 0x80);
  write(0x43, 0x00); CHECK(read(0x40) == 0 && read(0x40) == 0);
  machine.advanceTick(); CHECK(machine.consumePendingIrqs() == 0);
  write(0x43, 0x00); CHECK(read(0x40) == 0x52 && read(0x40) == 0xb2); // Binary underflow: 65536 - 19886.
  for (int i = 0; i < 3; ++i) { machine.advanceTick(); CHECK(machine.consumePendingIrqs() == 0); }
  write(0x40, 0xae); // The first byte immediately lowers OUT and stops mode 0.
  CHECK((status(0) & 0x80) == 0);
  machine.advanceTick(); CHECK(machine.consumePendingIrqs() == 0);
  write(0x40, 0x4d); // Reload arms one more terminal count.
  machine.advanceTick(); CHECK((machine.consumePendingIrqs() & 1u) != 0);
  machine.advanceTick(); CHECK(machine.consumePendingIrqs() == 0);
  write(0x43, 0x36); write(0x40, 0xae); write(0x40, 0x4d);
  for (int i = 0; i < 3; ++i) { machine.advanceTick(); CHECK((machine.consumePendingIrqs() & 1u) != 0); }
}

static void test_i8042_keyboard_irq_and_ports() {
  lcom::Machine machine;
  uint8_t flags=0;
  CHECK(machine.readPort8(0x64,flags) && (flags&0x10));
  CHECK(machine.writePort8(0x64,0xad));
  CHECK(machine.readPort8(0x64,flags) && !(flags&0x10));
  CHECK(machine.writePort8(0x64,0xae));
  CHECK(machine.readPort8(0x64,flags) && (flags&0x10));
  lcom::IrqSubscription sub;
  CHECK(machine.subscribeIrq(KBC_IRQ, sub));
  machine.injectKey("A", true);
  CHECK(machine.pendingIrqs() == BIT(KBC_IRQ));

  uint8_t status = 0;
  CHECK(machine.readPort8(KBC_ST_REG, status));
  CHECK((status & KBC_ST_OBF) != 0);
  CHECK((status & KBC_ST_AUX) == 0);

  uint8_t data = 0;
  CHECK(machine.readPort8(KBC_OUT_BUF, data));
  CHECK(data == 0x1E);
}

static void test_i8042_reasserts_buffered_irq_on_subscribe() {
  lcom::Machine machine;
  machine.injectKey("A", true);
  CHECK(machine.pendingIrqs() == 0);

  lcom::IrqSubscription sub;
  CHECK(machine.subscribeIrq(KBC_IRQ, sub));
  CHECK(machine.pendingIrqs() == BIT(KBC_IRQ));

  uint8_t data = 0;
  CHECK(machine.readPort8(KBC_OUT_BUF, data));
  CHECK(data == 0x1E);
}

static void test_i8042_keyboard_common_scancodes() {
  lcom::Machine machine;
  lcom::IrqSubscription sub;
  CHECK(machine.subscribeIrq(KBC_IRQ, sub));

  auto expect_byte = [&](uint8_t expected) {
    uint8_t data = 0;
    CHECK(machine.readPort8(KBC_OUT_BUF, data));
    CHECK(data == expected);
  };

  machine.injectKey("ENTER", true);
  expect_byte(0x1C);
  machine.injectKey("ENTER", false);
  expect_byte(0x9C);
  machine.injectKey("BACKSPACE", true);
  expect_byte(0x0E);
  machine.injectKey("MINUS", true);
  expect_byte(0x0C);
  machine.injectKey("F12", true);
  expect_byte(0x58);

  machine.injectKey("LEFT", true);
  expect_byte(0xE0);
  expect_byte(0x4B);
  machine.injectKey("DELETE", false);
  expect_byte(0xE0);
  expect_byte(0xD3);
  machine.injectKey("RCTRL", true);
  expect_byte(0xE0);
  expect_byte(0x1D);
}

static void test_mouse_scheduler_coalesces_motion() {
  lcom::MousePacketScheduler scheduler;
  for (int i = 0; i < 10000; i++) scheduler.addMotion(1, 1);

  auto packet = scheduler.nextPacket(false);
  CHECK(packet.has_value());
  CHECK(packet->dx == 96);
  CHECK(packet->dy == -96);
  CHECK(!scheduler.nextPacket(false).has_value());
}

static void test_mouse_scheduler_clamps_and_flips_y() {
  lcom::MousePacketScheduler scheduler;
  scheduler.addMotion(-400, 300);
  auto packet = scheduler.nextPacket(false);
  CHECK(packet.has_value());
  CHECK(packet->dx == -96);
  CHECK(packet->dy == -96);
}

static void test_mouse_scheduler_button_change_emits_zero_delta() {
  lcom::MousePacketScheduler scheduler;
  CHECK(!scheduler.nextPacket(false).has_value());
  scheduler.setButtons(0x01);
  auto packet = scheduler.nextPacket(true);
  CHECK(packet.has_value());
  CHECK(packet->dx == 0);
  CHECK(packet->dy == 0);
  CHECK(packet->buttons == 0x01);
  CHECK(!scheduler.nextPacket(false).has_value());
}

static void test_mouse_scheduler_bounds_backlog() {
  lcom::MousePacketScheduler scheduler;
  scheduler.addMotion(50000, -50000);
  CHECK(scheduler.pendingDx() == 96);
  CHECK(scheduler.pendingScreenDy() == -96);
  auto packet = scheduler.nextPacket(false);
  CHECK(packet.has_value());
  CHECK(packet->dx == 96);
  CHECK(packet->dy == 96);
  CHECK(scheduler.pendingDx() == 0);
  CHECK(scheduler.pendingScreenDy() == 0);
  CHECK(!scheduler.nextPacket(false).has_value());
}

static void test_i8042_command_byte_and_mouse_packet() {
  lcom::Machine machine;
  lcom::IrqSubscription sub;
  CHECK(machine.subscribeIrq(MOUSE_IRQ, sub));

  CHECK(machine.writePort8(KBC_CMD_REG, KBC_CMD_READ_CB));
  uint8_t cb = 0;
  CHECK(machine.readPort8(KBC_OUT_BUF, cb));
  CHECK((cb & KBC_CB_INT) != 0);

  CHECK(machine.writePort8(KBC_CMD_REG, KBC_CMD_WRITE_MOUSE));
  CHECK(machine.writePort8(KBC_IN_BUF, MOUSE_CMD_ENABLE_DR));
  uint8_t ack = 0;
  CHECK(machine.readPort8(KBC_OUT_BUF, ack));
  CHECK(ack == MOUSE_ACK);
  // Polling a command ACK consumes its only byte. An empty notification would
  // send legacy interrupt handlers into a read loop before their first frame.
  CHECK((machine.pendingIrqs() & BIT(MOUSE_IRQ)) == 0);

  machine.injectMouse(5, -2, 1);
  CHECK(machine.pendingIrqs() == BIT(MOUSE_IRQ));
  uint8_t b0 = 0, b1 = 0, b2 = 0;
  CHECK(machine.readPort8(KBC_OUT_BUF, b0));
  CHECK(machine.readPort8(KBC_OUT_BUF, b1));
  CHECK(machine.readPort8(KBC_OUT_BUF, b2));
  CHECK((b0 & MOUSE_SYNC_BIT) != 0);
  CHECK((b0 & BIT(0)) != 0);
  CHECK((b0 & BIT(5)) != 0);
  CHECK(b1 == 5);
  CHECK(b2 == static_cast<uint8_t>(-2));
}

static void test_i8042_remote_mouse_polling() {
  lcom::Machine machine;
  auto send = [&](uint8_t command) {
    CHECK(machine.writePort8(KBC_CMD_REG, KBC_CMD_WRITE_MOUSE));
    CHECK(machine.writePort8(KBC_IN_BUF, command));
    uint8_t byte = 0;
    CHECK(machine.readPort8(KBC_OUT_BUF, byte) && byte == MOUSE_ACK);
  };
  auto expect = [&](uint8_t expected) {
    uint8_t status = 0, byte = 0;
    CHECK(machine.readPort8(KBC_ST_REG, status) && (status & KBC_ST_OBF));
    CHECK(machine.readPort8(KBC_OUT_BUF, byte) && byte == expected);
  };
  send(MOUSE_CMD_SET_REMOTE);
  machine.injectMouse(11, -4, 1);
  machine.injectMouse(5, -2, 1);
  uint8_t status = 0;
  CHECK(machine.readPort8(KBC_ST_REG, status) && !(status & KBC_ST_OBF));
  send(MOUSE_CMD_READ_DATA);
  expect(0x29); expect(16); expect(0xfa);
  send(MOUSE_CMD_READ_DATA);
  expect(0x09); expect(0); expect(0);
  send(MOUSE_CMD_ENABLE_DR);
  machine.injectMouse(-20, 9, 2);
  CHECK(machine.readPort8(KBC_ST_REG, status) && !(status & KBC_ST_OBF));
  send(MOUSE_CMD_READ_DATA);
  expect(0x1a); expect(0xec); expect(9);
  send(MOUSE_CMD_RESET);
  expect(0xaa); expect(0);
  machine.injectMouse(3, 4, 0);
  CHECK(machine.readPort8(KBC_ST_REG, status) && !(status & KBC_ST_OBF));
}

static void test_i8042_wheel_negotiation_and_packet_framing() {
  lcom::Machine machine;
  auto send = [&](uint8_t command) {
    CHECK(machine.writePort8(KBC_CMD_REG, KBC_CMD_WRITE_MOUSE));
    CHECK(machine.writePort8(KBC_IN_BUF, command));
    uint8_t byte = 0;
    CHECK(machine.readPort8(KBC_OUT_BUF, byte) && byte == MOUSE_ACK);
  };
  auto expect = [&](uint8_t expected) {
    uint8_t status = 0, byte = 0;
    CHECK(machine.readPort8(KBC_ST_REG, status) && (status & KBC_ST_OBF));
    CHECK(machine.readPort8(KBC_OUT_BUF, byte) && byte == expected);
  };
  auto rate = [&](uint8_t value) { send(0xf3); send(value); };
  send(0xf2); expect(0);
  // Only the IntelliMouse sample-rate sequence enables four-byte packets.
  rate(200); rate(80); rate(100); send(0xf2); expect(0);
  rate(200); rate(100); rate(80); send(0xf2); expect(3);
  send(MOUSE_CMD_ENABLE_DR);
  machine.injectMouse(12, -3, 1);
  expect(0x29); expect(12); expect(0xfd); expect(0);
  machine.injectMouse(0, 0, 0);
  expect(0x08); expect(0); expect(0); expect(0);
  machine.i8042().injectMouseWheel(2);
  expect(0x08); expect(0); expect(0); expect(2);
  machine.i8042().injectMouseWheel(-3);
  expect(0x08); expect(0); expect(0); expect(0xfd);
  send(MOUSE_CMD_SET_REMOTE);
  machine.i8042().injectMouseWheel(4);
  send(MOUSE_CMD_READ_DATA);
  expect(0x08); expect(0); expect(0); expect(4);
  send(MOUSE_CMD_RESET); expect(0xaa); expect(0);
  send(0xf2); expect(0);
  send(MOUSE_CMD_ENABLE_DR);
  machine.injectMouse(1, 2, 0);
  expect(0x08); expect(1); expect(2);
  uint8_t status = 0;
  CHECK(machine.readPort8(KBC_ST_REG, status) && !(status & KBC_ST_OBF));
}

static void test_rtc_cmos_bcd_registers() {
  lcom::Machine machine;
  CHECK(machine.rtc().setIsoTime("2026-06-16T12:34:56"));
  uint8_t value = 0;
  CHECK(machine.writePort8(RTC_ADDR_REG, RTC_REG_DAY));
  CHECK(machine.readPort8(RTC_DATA_REG, value));
  CHECK(value == 0x16);
  CHECK(machine.writePort8(RTC_ADDR_REG, RTC_REG_MONTH));
  CHECK(machine.readPort8(RTC_DATA_REG, value));
  CHECK(value == 0x06);
  CHECK(machine.writePort8(RTC_ADDR_REG, RTC_REG_YEAR));
  CHECK(machine.readPort8(RTC_DATA_REG, value));
  CHECK(value == 0x26);
  CHECK(machine.writePort8(RTC_ADDR_REG, RTC_REG_B));
  CHECK(machine.readPort8(RTC_DATA_REG, value));
  CHECK((value & RTC_24H) != 0);
  CHECK((value & RTC_DM) == 0);
}

/* DS12887 register and IRQ contracts from the manufacturer's data sheet. */
static void test_rtc_register_writes_and_interrupts() {
  lcom::Machine machine;
  CHECK(machine.rtc().setIsoTime("2026-12-31T23:59:59"));
  lcom::IrqSubscription subscription;
  CHECK(machine.subscribeIrq(8, subscription));
  auto write = [&](uint8_t reg, uint8_t value) {
    CHECK(machine.writePort8(0x70, reg)); CHECK(machine.writePort8(0x71, value));
  };
  auto read = [&](uint8_t reg) {
    uint8_t value = 0; CHECK(machine.writePort8(0x70, reg));
    CHECK(machine.readPort8(0x71, value)); return value;
  };
  write(0x0b, 0x12); // UIE, 24-hour BCD.
  write(1, 0x10); // Keep the alarm from matching midnight during the UIE check.
  CHECK(read(0x0b) == 0x12);
  for (int i = 0; i < 59; ++i) machine.advanceTick();
  CHECK((machine.pendingIrqs() & (1u << 8)) == 0);
  machine.advanceTick();
  CHECK((machine.consumePendingIrqs() & (1u << 8)) != 0);
  // The RTC holds one output assertion until C is acknowledged. Polling the
  // edge-triggered PIC must not turn that held output into new notifications.
  for (int i = 0; i < 100; ++i) {
    machine.refreshDeviceIrqs();
    CHECK((machine.consumePendingIrqs() & (1u << 8)) == 0);
  }
  CHECK(read(0x0c) == 0xd0); // IRQF + default-rate PF + UF.
  machine.refreshDeviceIrqs();
  CHECK((machine.pendingIrqs() & (1u << 8)) == 0 && read(0x0c) == 0);
  CHECK(read(0) == 0 && read(2) == 0 && read(4) == 0);
  CHECK(read(7) == 1 && read(8) == 1 && read(9) == 0x27);
  CHECK(read(0x0d) == 0x80); // Valid battery-backed clock.
  write(0x0a, 0x2f); write(0x0b, 0x46); // 2 Hz periodic, binary, 24-hour.
  CHECK(read(0x0a) == 0x2f && read(0x0b) == 0x46);
  for (int i = 0; i < 29; ++i) machine.advanceTick();
  CHECK((machine.pendingIrqs() & (1u << 8)) == 0);
  machine.advanceTick();
  CHECK((machine.consumePendingIrqs() & (1u << 8)) != 0 && read(0x0c) == 0xc0);
  write(0x0b, 0x26); write(1, 1); write(3, 0xc0); write(5, 0xc0);
  for (int i = 0; i < 30; ++i) machine.advanceTick();
  CHECK((machine.consumePendingIrqs() & (1u << 8)) != 0);
  CHECK((read(0x0c) & 0xb0) == 0xb0); // IRQF, AF and UF.
  write(0x0b, 0x06); write(4, 13); CHECK(read(4) == 13);
  write(0x0b, 0x04); CHECK(read(4) == 0x81); // Binary 12-hour, 1 PM.
  write(0x0b, 0); CHECK(read(4) == 0x81); // BCD 12-hour.
  write(0x0b, 0x92); CHECK(read(0x0b) == 0x82); // SET clears UIE.
  write(0, 0x25); CHECK(read(0) == 0x25);
  for (int i = 0; i < 60; ++i) machine.advanceTick();
  CHECK(read(0) == 0x25); // User clock registers stay frozen under SET.
  write(0x0b, 0x02); write(0x0a, 0x26);
  CHECK(!machine.rtc().setIsoTime("2026-02-30T00:00:00"));
  CHECK(machine.rtc().setIsoTime("2024-02-28T23:59:59"));
  for (int i = 0; i < 60; ++i) machine.advanceTick();
  CHECK(read(7) == 0x29 && read(8) == 2);
  CHECK(machine.writePort8(0x43, 0x36));
  uint16_t divisor = 1193182 / 120;
  CHECK(machine.writePort8(0x40, static_cast<uint8_t>(divisor)));
  CHECK(machine.writePort8(0x40, static_cast<uint8_t>(divisor >> 8)));
  CHECK(machine.rtc().setIsoTime("2026-10-13T12:00:00"));
  (void)read(0x0c); (void)machine.consumePendingIrqs(); write(0x0b, 0x16);
  for (int i = 0; i < 119; ++i) machine.advanceTick();
  CHECK((machine.pendingIrqs() & (1u << 8)) == 0);
  machine.advanceTick();
  CHECK((machine.consumePendingIrqs() & (1u << 8)) != 0 && read(0) == 1);
  CHECK((read(0x0c) & 0x90) == 0x90);
  CHECK(machine.rtc().setIsoTime("2026-10-13T12:00:00"));
  write(0x0b, 0x86);
  for (int i = 0; i < 240; ++i) machine.advanceTick();
  CHECK(read(0) == 0);
  write(0x0b, 0x06);
  for (int i = 0; i < 120; ++i) machine.advanceTick();
  CHECK(read(0) == 3); // Internal time continued while the user registers froze.

  // Reading C deasserts the device output, but must not erase an edge already
  // latched in the PIC. Clearing and restoring an enable rearms a pending cause.
  write(0x0b, 0x06); (void)read(0x0c); (void)machine.consumePendingIrqs();
  write(0x0a, 0x2f); write(0x0b, 0x46); // 2 Hz periodic, no UIE/alarm.
  for (int i=0;i<60;++i) machine.advanceTick(); // Current PIT runs at120 Hz.
  CHECK((read(0x0c) & 0xc0) == 0xc0);
  CHECK(machine.consumePendingIrqs() == (1u<<8));
  machine.refreshDeviceIrqs();
  CHECK(machine.consumePendingIrqs() == 0);
  for (int i=0;i<60;++i) machine.advanceTick();
  write(0x0b, 0x06); // Disable PIE, leaving PF latched in C.
  CHECK(machine.consumePendingIrqs() == (1u<<8));
  machine.refreshDeviceIrqs(); CHECK(machine.consumePendingIrqs() == 0);
  write(0x0b, 0x46); // Reassert the held PF through a fresh enable edge.
  CHECK(machine.consumePendingIrqs() == (1u<<8));
  machine.refreshDeviceIrqs(); CHECK(machine.consumePendingIrqs() == 0);
  CHECK((read(0x0c) & 0xc0) == 0xc0 && read(0x0c) == 0);

  // Keep an initial undelivered RTC cause available when its first handler
  // subscribes after the flag was set, as with the other device IRQ owners.
  lcom::Machine late;
  CHECK(late.writePort8(0x70,0x0b) && late.writePort8(0x71,0x12));
  for (int i=0;i<60;++i) late.advanceTick();
  CHECK(late.pendingIrqs()==0);
  CHECK(late.subscribeIrq(8,subscription));
  CHECK(late.consumePendingIrqs()==(1u<<8));
  late.refreshDeviceIrqs(); CHECK(late.consumePendingIrqs()==0);

}

static void test_vbe_mode_and_framebuffer() {
  lcom::Machine machine;
  lcom::VbeModeInfo info;
  CHECK(machine.vbe().modeInfo(LCOM_VBE_MODE_800_600_24, info));
  CHECK(info.width == 800);
  CHECK(info.height == 600);
  CHECK(info.bytes_per_pixel == 3);
  // Standard VBE 0x11A is RGB565, distinct from the host's 32-bit extension.
  lcom::VbeModeInfo standard16;
  CHECK(machine.vbe().modeInfo(0x11A, standard16));
  CHECK(standard16.width == 1280 && standard16.height == 1024);
  CHECK(standard16.bpp == 16 && standard16.bytes_per_pixel == 2);
  CHECK(standard16.pitch == 2560);
  CHECK(machine.vbe().setMode(LCOM_VBE_MODE_800_600_24));
  CHECK(machine.vbe().ownsRange(info.framebuffer_phys, info.framebuffer_size));
  // Minix maps front/back VRAM before selecting a graphics mode.
  CHECK(machine.vbe().ownsRange(info.framebuffer_phys, 2u * 1280u * 1024u * 4u));
  CHECK(machine.vbe().setMode(3));
  CHECK(!machine.vbe().graphicsMode());
  CHECK(machine.vbe().setMode(LCOM_VBE_MODE_800_600_24));
  CHECK(!machine.vbe().ownsRange(std::numeric_limits<uint64_t>::max(), 2));
  CHECK(!machine.vbe().ownsRange(info.framebuffer_phys, std::numeric_limits<uint64_t>::max()));
  machine.vbe().framebuffer()[0] = 0xAA;
  CHECK(machine.vbe().framebuffer()[0] == 0xAA);

  // Verify public image output against VGA palette and RGB bitfield contracts.
  const struct {
    uint16_t mode;
    uint8_t raw[8];
    uint8_t expected[6];
  } color_cases[] = {
    {LCOM_VBE_MODE_1024_768_8, {4, 14}, {170, 0, 0, 255, 255, 85}},
    // SeaBIOS VGA palette3 entries, normalized from six-bit DAC channels.
    {LCOM_VBE_MODE_1024_768_8, {16, 31}, {0, 0, 0, 255, 255, 255}},
    {LCOM_VBE_MODE_1024_768_8, {32, 40}, {0, 0, 255, 255, 0, 0}},
    {LCOM_VBE_MODE_1024_768_8, {56, 72}, {125, 125, 255, 125, 255, 125}},
    {LCOM_VBE_MODE_1024_768_8, {248, 255}, {0, 0, 0, 0, 0, 0}},
    {0x107, {32, 40}, {0, 0, 255, 255, 0, 0}},
    {0x11b, {0, 0, 255, 0, 255, 255}, {255, 0, 0, 255, 255, 0}},
    {0x143, {0, 0, 255, 0, 0, 255, 255, 0}, {255, 0, 0, 255, 255, 0}},
    {0x144, {0, 0, 255, 0, 0, 255, 255, 0}, {255, 0, 0, 255, 255, 0}},
    {0x14b, {0, 0, 255, 0, 255, 255}, {255, 0, 0, 255, 255, 0}},
    {LCOM_VBE_MODE_640_480_15, {0x00, 0x7c, 0xe0, 0x7f}, {255, 0, 0, 255, 255, 0}},
    {0x111, {0x00, 0xf8, 0xe0, 0xff}, {255, 0, 0, 255, 255, 0}},
    {LCOM_VBE_MODE_800_600_24, {0, 0, 255, 0, 255, 255}, {255, 0, 0, 255, 255, 0}},
    {LCOM_VBE_MODE_1280_1024_32, {0, 0, 255, 0, 0, 255, 255, 0}, {255, 0, 0, 255, 255, 0}}
  };
  const char *frame_path = "/tmp/lcom-ng-vbe-colors.ppm";
  lcom::Machine modern_machine; // Native SDK default, independent of textmode03 above.
  for (const auto &test : color_cases) {
    CHECK(modern_machine.vbe().setMode(test.mode));
    const auto &mode = modern_machine.vbe().currentMode();
    // Exercise row boundaries too, so buffered output cannot repeat or omit rows.
    for (uint16_t y = 0; y < mode.height; ++y) {
      uint8_t *row = modern_machine.vbe().framebuffer().data() + static_cast<size_t>(y) * mode.pitch;
      for (size_t x = 0; x < 2; ++x) {
        const uint8_t *pixel = test.raw + ((x + y) % 2) * mode.bytes_per_pixel;
        std::memcpy(row + x * mode.bytes_per_pixel, pixel, mode.bytes_per_pixel);
        std::memcpy(row + (mode.width - 2 + x) * mode.bytes_per_pixel,
                    pixel, mode.bytes_per_pixel);
      }
    }
    CHECK(modern_machine.vbe().dumpPpm(frame_path));
    std::ifstream frame(frame_path, std::ios::binary);
    std::string header;
    std::getline(frame, header);
    CHECK(header == "P6");
    std::getline(frame, header);
    CHECK(header == std::to_string(mode.width) + " " + std::to_string(mode.height));
    std::getline(frame, header);
    CHECK(header == "255");
    std::vector<uint8_t> row(static_cast<size_t>(mode.width) * 3);
    for (uint16_t y = 0; y < mode.height; ++y) {
      frame.read(reinterpret_cast<char *>(row.data()), row.size());
      CHECK(frame.gcount() == static_cast<std::streamsize>(row.size()));
      for (size_t x = 0; x < 2; ++x) {
        const uint8_t *expected = test.expected + ((x + y) % 2) * 3;
        CHECK(std::memcmp(row.data() + x * 3, expected, 3) == 0);
        CHECK(std::memcmp(row.data() + row.size() - 6 + x * 3, expected, 3) == 0);
      }
    }
    CHECK(frame.peek() == std::char_traits<char>::eof());
  }
  std::remove(frame_path);
}

static void test_vbe_caption_pixels() {
  lcom::Vbe vbe;
  CHECK(vbe.setMode(LCOM_VBE_MODE_640_480_15));
  const char *path = "/tmp/lcom-ng-caption.ppm";
  for (const char *position : {"top", "bottom"}) {
    for (bool clipped : {false, true}) {
      CHECK(vbe.dumpPpm(path, clipped ? std::string(100, 'A') : "A", position));
      std::ifstream frame(path, std::ios::binary);
      std::string header;
      for (int i = 0; i < 3; ++i) std::getline(frame, header);
      std::vector<uint8_t> pixels(640 * 480 * 3);
      frame.read(reinterpret_cast<char *>(pixels.data()), pixels.size());
      CHECK(frame.gcount() == static_cast<std::streamsize>(pixels.size()));
      auto pixel = [&](int x, int y, uint8_t r, uint8_t g, uint8_t b) {
        size_t offset = (y * 640 + x) * 3;
        CHECK(pixels[offset] == r && pixels[offset + 1] == g && pixels[offset + 2] == b);
      };
      // At 640x480 the caption box is 49px high, inset 24px. A short
      // caption is centered; a clipped caption fits exactly 30 glyph cells.
      const int y = std::strcmp(position, "top") == 0 ? 24 : 407;
      const int left = clipped ? 26 : 287;
      const int right = clipped ? 614 : 353;
      pixel(left - 1, y, 0, 0, 0);
      pixel(left, y - 1, 0, 0, 0);
      pixel(left, y, 18, 24, 32);
      pixel(right - 1, y + 48, 18, 24, 32);
      pixel(right, y, 0, 0, 0);
      pixel(left, y + 49, 0, 0, 0);
      // A starts with .###. and ends with #...#, scaled by three.
      pixel(left + 24, y + 14, 18, 24, 32);
      pixel(left + 27, y + 14, 236, 246, 255);
      pixel(left + 24, y + 32, 236, 246, 255);
      pixel(left + 27, y + 32, 18, 24, 32);
      pixel(left + 39, y + 14, 18, 24, 32);
      if (clipped) pixel(575, y + 14, 236, 246, 255);
    }
  }
  std::remove(path);
}

static void test_vga_dac_ports() {
  lcom::Machine machine;
  CHECK(machine.vbe().setMode(LCOM_VBE_MODE_1024_768_8));
  uint8_t value = 0;
  CHECK(machine.readPort8(0x3c6, value) && value == 0xff);
  CHECK(machine.writePort8(0x3c8, 255));
  // Components are six-bit values; DAC indices wrap after the RGB triple.
  CHECK(machine.writePort8(0x3c9, 255));
  CHECK(machine.writePort8(0x3c9, 21));
  CHECK(machine.writePort8(0x3c9, 0));
  CHECK(machine.readPort8(0x3c8, value) && value == 0);
  CHECK(machine.readPort8(0x3c7, value) && value == 0);
  CHECK(machine.writePort8(0x3c7, 255));
  CHECK(machine.readPort8(0x3c7, value) && value == 3);
  for (uint8_t channel : {63, 21, 0}) {
    CHECK(machine.readPort8(0x3c9, value) && value == channel);
  }
  uint8_t index = 255, rgb[3] = {};
  machine.vbe().pixelRgb(&index, rgb);
  CHECK(rgb[0] == 255 && rgb[1] == 85 && rgb[2] == 0);
  CHECK(machine.writePort8(0x3c6, 0x0f));
  machine.vbe().pixelRgb(&index, rgb);
  CHECK(rgb[0] == 255 && rgb[1] == 255 && rgb[2] == 255);
  CHECK(machine.vbe().setMode(LCOM_VBE_MODE_1024_768_8));
  CHECK(machine.readPort8(0x3c6, value) && value == 0x0f);
  machine.vbe().pixelRgb(&index, rgb);
  CHECK(rgb[0] == 255 && rgb[1] == 255 && rgb[2] == 255);
  CHECK(machine.writePort8(0x3c6, 0xff));
  machine.vbe().pixelRgb(&index, rgb);
  CHECK(rgb[0] == 255 && rgb[1] == 85 && rgb[2] == 0);
  // VBE preserves the DAC and mask; BIOS textmode03 resets both.
  CHECK(machine.vbe().setMode(3));
  CHECK(machine.vbe().setMode(LCOM_VBE_MODE_1024_768_8));
  CHECK(machine.readPort8(0x3c6, value) && value == 0xff);
  machine.vbe().pixelRgb(&index, rgb);
  CHECK(rgb[0] == 0 && rgb[1] == 0 && rgb[2] == 0);
}

static uint32_t read_u32_le(const std::vector<uint8_t> &bytes, size_t offset) {
  return static_cast<uint32_t>(bytes[offset]) |
         (static_cast<uint32_t>(bytes[offset + 1]) << 8) |
         (static_cast<uint32_t>(bytes[offset + 2]) << 16) |
         (static_cast<uint32_t>(bytes[offset + 3]) << 24);
}

static void test_wav_backend_streams_multiple_blocks() {
  const char *path = "/tmp/machine-lab-streaming-audio.wav";
  std::remove(path);
  const int16_t first[] = {1, -1, 2, -2};
  const int16_t second[] = {3, -3};
  std::string error;
  {
    lcom::WavAudioBackend wav(path);
    CHECK(wav.playPcm16(first, 2, 48000, 2, error));
    wav.stop();
    CHECK(wav.playPcm16(second, 1, 48000, 2, error));
  }

  std::ifstream input(path, std::ios::binary);
  std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(input)),
                             std::istreambuf_iterator<char>());
  CHECK(bytes.size() == 44 + sizeof(first) + sizeof(second));
  if (bytes.size() >= 44) {
    CHECK(std::string(bytes.begin(), bytes.begin() + 4) == "RIFF");
    CHECK(std::string(bytes.begin() + 8, bytes.begin() + 12) == "WAVE");
    CHECK(read_u32_le(bytes, 4) == bytes.size() - 8);
    CHECK(read_u32_le(bytes, 40) == bytes.size() - 44);
  }

  lcom::WavAudioBackend invalid("/missing-machine-lab-dir/audio.wav");
  CHECK(!invalid.playPcm16(first, 2, 48000, 2, error));
}

static void test_protocol_decode_accepts_unaligned_storage() {
  lcom_port_write8_t expected{};
  expected.port = 0x3F8;
  expected.value = 0x5A;
  std::vector<uint8_t> storage(sizeof(expected) + 1);
  std::memcpy(storage.data() + 1, &expected, sizeof(expected));
  lcom_port_write8_t decoded{};
  CHECK(lcom::protocol::decodePayload(storage.data() + 1, sizeof(expected), decoded));
  CHECK(decoded.port == expected.port);
  CHECK(decoded.value == expected.value);
  CHECK(!lcom::protocol::decodePayload(storage.data() + 1, sizeof(expected) - 1, decoded));
}

static void test_uart_loopback_registers() {
  lcom::Machine machine;
  lcom::IrqSubscription sub;
  CHECK(machine.subscribeIrq(COM1_IRQ, sub));
  CHECK(machine.writePort8(COM1_BASE + SER_LCR, LCR_DLAB));
  CHECK(machine.writePort8(COM1_BASE + SER_DLL, 0x0C));
  CHECK(machine.writePort8(COM1_BASE + SER_DLM, 0x00));
  CHECK(machine.com1().divisor() == 12);
  CHECK(machine.writePort8(COM1_BASE + SER_LCR, 0x03));
  CHECK(machine.writePort8(COM1_BASE + SER_SR, 0x5A));
  uint8_t scratch = 0;
  CHECK(machine.readPort8(COM1_BASE + SER_SR, scratch));
  CHECK(scratch == 0x5A);
  // A 16550A identifies working enabled FIFOs in both high IIR bits.
  CHECK(machine.writePort8(COM1_BASE + SER_FCR, FCR_ENABLE_FIFO));
  uint8_t iir = 0;
  CHECK(machine.readPort8(COM1_BASE + SER_IIR, iir));
  CHECK((iir & 0xC0u) == 0xC0u);
  CHECK(machine.writePort8(COM1_BASE + SER_FCR, 0));
  CHECK(machine.readPort8(COM1_BASE + SER_IIR, iir));
  CHECK((iir & 0xC0u) == 0);
  CHECK(machine.writePort8(COM1_BASE + SER_MCR, MCR_LOOP));
  CHECK(machine.writePort8(COM1_BASE + SER_IER, IER_RDA));
  CHECK(machine.writePort8(COM1_BASE + SER_THR, 'X'));
  CHECK(machine.pendingIrqs() == BIT(COM1_IRQ));
  uint8_t lsr = 0;
  CHECK(machine.readPort8(COM1_BASE + SER_LSR, lsr));
  CHECK((lsr & LSR_RX_RDY) != 0);
  uint8_t data = 0;
  CHECK(machine.readPort8(COM1_BASE + SER_RBR, data));
  CHECK(data == 'X');
}

static void test_uart_pair_virtual_cable() {
  lcom::Machine machine;
  lcom::IrqSubscription sub;
  CHECK(machine.subscribeIrq(COM2_IRQ, sub));
  CHECK(machine.writePort8(COM2_BASE + SER_FCR, FCR_ENABLE_FIFO | FCR_CLEAR_RX | FCR_CLEAR_TX));
  CHECK(machine.writePort8(COM2_BASE + SER_IER, IER_RDA));
  CHECK(machine.writePort8(COM1_BASE + SER_THR, 'P'));
  CHECK(machine.pendingIrqs() == BIT(COM2_IRQ));

  uint8_t com1_lsr = 0;
  CHECK(machine.readPort8(COM1_BASE + SER_LSR, com1_lsr));
  CHECK((com1_lsr & LSR_RX_RDY) == 0);

  uint8_t com2_lsr = 0;
  CHECK(machine.readPort8(COM2_BASE + SER_LSR, com2_lsr));
  CHECK((com2_lsr & LSR_RX_RDY) != 0);
  uint8_t data = 0;
  CHECK(machine.readPort8(COM2_BASE + SER_RBR, data));
  CHECK(data == 'P');
}

static void test_uart_virtual_wire_preserves_bursts() {
  lcom::Machine machine;
  CHECK(machine.writePort8(COM2_BASE + SER_FCR, FCR_ENABLE_FIFO | FCR_CLEAR_RX | FCR_CLEAR_TX));
  CHECK(machine.writePort8(COM2_BASE + SER_IER, IER_RDA));

  for (uint8_t i = 0; i < 64; i++) {
    CHECK(machine.writePort8(COM1_BASE + SER_THR, static_cast<uint8_t>('A' + (i % 26))));
  }

  lcom::IrqSubscription sub;
  CHECK(machine.subscribeIrq(COM2_IRQ, sub));
  CHECK((machine.pendingIrqs() & BIT(COM2_IRQ)) != 0);

  for (uint8_t i = 0; i < 64; i++) {
    uint8_t lsr = 0;
    CHECK(machine.readPort8(COM2_BASE + SER_LSR, lsr));
    CHECK((lsr & LSR_RX_RDY) != 0);
    uint8_t data = 0;
    CHECK(machine.readPort8(COM2_BASE + SER_RBR, data));
    CHECK(data == static_cast<uint8_t>('A' + (i % 26)));
  }
}

static void test_uart_fifo_threshold_and_timeout() {
  lcom::Machine machine;
  lcom::IrqSubscription sub;
  CHECK(machine.subscribeIrq(COM2_IRQ, sub));
  CHECK(machine.writePort8(COM2_BASE + SER_LCR, 0x83));
  CHECK(machine.writePort8(COM2_BASE + SER_DLL, 0x80));
  CHECK(machine.writePort8(COM2_BASE + SER_DLM, 1)); // 115200 / 384 = 300 baud.
  CHECK(machine.writePort8(COM2_BASE + SER_LCR, 3)); // 8N1 = 10 bits per character.
  CHECK(machine.writePort8(COM2_BASE + SER_FCR, 0x41)); // Four-byte receive trigger.
  CHECK(machine.writePort8(COM2_BASE + SER_IER, IER_RDA));
  for (uint8_t byte = 1; byte <= 3; ++byte) machine.com2().injectRx(byte);
  uint8_t value = 0;
  CHECK(machine.pendingIrqs() == 0);
  CHECK(machine.readPort8(COM2_BASE + SER_IIR, value) && (value & 15) == 1);
  machine.com2().injectRx(4);
  CHECK(machine.consumePendingIrqs() == BIT(COM2_IRQ));
  CHECK(machine.readPort8(COM2_BASE + SER_IIR, value) && (value & 15) == 4);
  CHECK(machine.readPort8(COM2_BASE + SER_RBR, value) && value == 1);
  CHECK(machine.readPort8(COM2_BASE + SER_IIR, value) && (value & 15) == 1);
  // Four 10-bit characters at 300 baud take 133.3 ms. Seven ticks at
  // 60 Hz are too early; nine ticks exceed that interval.
  for (unsigned tick = 0; tick < 7; ++tick) machine.advanceTick();
  CHECK(machine.pendingIrqs() == 0);
  machine.advanceTick(); machine.advanceTick();
  CHECK(machine.consumePendingIrqs() == BIT(COM2_IRQ));
  CHECK(machine.readPort8(COM2_BASE + SER_IIR, value) && (value & 15) == 12);
  CHECK(machine.readPort8(COM2_BASE + SER_IIR, value) && (value & 15) == 12);
  CHECK(machine.readPort8(COM2_BASE + SER_RBR, value) && value == 2);
  CHECK(machine.readPort8(COM2_BASE + SER_IIR, value) && (value & 15) == 1);
  CHECK(machine.writePort8(COM2_BASE + SER_FCR, 0)); // A FIFO-mode transition clears it.
  CHECK(machine.readPort8(COM2_BASE + SER_LSR, value) && !(value & LSR_RX_RDY));
}

static void test_ac97_registers_and_buffer() {
  lcom::Machine machine;
  CHECK(machine.ac97().bufferPhys() == 0xD0000000ull);
  CHECK(machine.ac97().bufferSize() >= 1024u * 1024u);
  CHECK(machine.ac97().ownsRange(machine.ac97().bufferPhys(), 4096));

  CHECK(machine.writePort8(AC97_NAM_BASE + AC97_PCM_FRONT_DAC_RATE, 0x80));
  CHECK(machine.writePort8(AC97_NAM_BASE + AC97_PCM_FRONT_DAC_RATE + 1, 0xBB));
  CHECK(machine.ac97().sampleRate() == 48000);

  CHECK(machine.writePort8(AC97_BM_BASE + AC97_PO_CR, AC97_PO_CR_RUN));
  CHECK(machine.ac97().playing());
  uint8_t status = 0;
  CHECK(machine.readPort8(AC97_BM_BASE + AC97_PO_SR, status));
  CHECK((status & AC97_PO_SR_BCIS) != 0);
  CHECK(machine.writePort8(AC97_BM_BASE + AC97_PO_CR, 0));
  CHECK(!machine.ac97().playing());
}

static void test_ps2_fault_replay_and_status() {
  const char *path = "/tmp/lcom-ng-ps2-fault-unit.mlabscript";
  {
    std::ofstream out(path);
    out << "at 0 ps2 keyboard 0x1e parity\n"
        << "at 0 ps2 mouse 250 timeout\n"
        << "at 0 ps2 keyboard 48 ok\n"
        << "at 0 ps2 mouse 8 parity,timeout\n";
  }
  lcom::Script script; std::string error;
  bool loaded = script.load(path, error); CHECK(loaded);
  if (!loaded) { std::remove(path); return; }
  lcom::Machine machine; lcom::IrqSubscription keyboard, mouse;
  CHECK(machine.subscribeIrq(1, keyboard) && machine.subscribeIrq(12, mouse));
  script.injectDue(machine, 0);
  auto read = [&](uint16_t port) { uint8_t byte = 0; CHECK(machine.readPort8(port, byte)); return byte; };
  const struct { uint8_t status, byte, irq; } sequence[] = {{0x91, 0x1e, 1}, {0x71, 250, 12}, {0x11, 48, 1}, {0xf1, 8, 12}};
  for (const auto &expected : sequence) {
    CHECK(read(0x64) == expected.status);
    CHECK(read(0x64) == expected.status); // Reading status does not consume or clear the byte's errors.
    CHECK(machine.consumePendingIrqs() == (1u << expected.irq));
    CHECK(read(0x60) == expected.byte);
  }
  CHECK(read(0x64) == 0x10 && machine.consumePendingIrqs() == 0);
  machine.injectKey("A", true); CHECK(read(0x64) == 0x11 && read(0x60) == 0x1e);
  std::remove(path);
}

static void test_ps2_script_validation() {
  const char *path = "/tmp/lcom-ng-ps2-parse-unit.mlabscript";
  auto load = [&](const std::string &line, std::string &error) {
    { std::ofstream out(path); out << line << "\n"; }
    lcom::Script script; return script.load(path, error);
  };
  std::string error;
  for (const char *line : {"at 0 ps2 keyboard 0 ok", "at 1 ps2 mouse 255 parity,timeout", "at 2 ps2 mouse 0xFF timeout,parity"}) CHECK(load(line, error));
  for (const char *line : {"at 0 ps2 keyboard 256 ok", "at 0 ps2 keyboard -1 ok", "at 0 ps2 keyboard +1 ok", "at 0 ps2 keyboard 1junk ok", "at 0 ps2 keyboard 0x100 ok", "at 0 ps2 keyboard 0xGG ok", "at 0 ps2 keyboard 0x ok", "at 0 ps2 wheel 1 ok", "at 0 ps2 mouse 1 broken", "at 0 ps2 mouse 1 parity,parity", "at 0 ps2 mouse 1 parity,timeout,other", "at 0 ps2 mouse 1 ok extra", "at 0 ps2 mouse 1"}) {
    error.clear(); CHECK(!load(line, error)); CHECK(error.find("line 1") != std::string::npos);
  }
  std::remove(path);
}

static void test_script_time_and_demo_events() {
  const char *path = "/tmp/machine-lab-script-unit.mlabscript";
  {
    std::ofstream out(path);
    out << "at 1s caption top 2s HELLO LEVEL 1\n";
    out << "at 2s out\n";
    out << "at 2.5s in\n";
    out << "at 2.75s caption 1s bottom SECOND LINE\n";
    out << "at 3s move 10 -5 during 5\n";
  }

  lcom::Script script;
  std::string error;
  CHECK(script.load(path, error));

  auto due60 = script.takeDue(60);
  CHECK(due60.size() == 1);
  CHECK(due60[0].kind == lcom::ScriptEvent::Kind::Caption);
  CHECK(due60[0].duration == 120);
  CHECK(due60[0].caption_position == "top");

  auto due120 = script.takeDue(120);
  CHECK(due120.size() == 1);
  CHECK(due120[0].kind == lcom::ScriptEvent::Kind::Capture);
  CHECK(!due120[0].capture_enabled);

  auto due150 = script.takeDue(150);
  CHECK(due150.size() == 1);
  CHECK(due150[0].kind == lcom::ScriptEvent::Kind::Capture);
  CHECK(due150[0].capture_enabled);

  auto due165 = script.takeDue(165);
  CHECK(due165.size() == 1);
  CHECK(due165[0].kind == lcom::ScriptEvent::Kind::Caption);
  CHECK(due165[0].caption_position == "bottom");

  int total_dx = 0;
  int total_dy = 0;
  int move_events = 0;
  for (uint64_t tick = 180; tick < 185; tick++) {
    for (const auto &ev : script.takeDue(tick)) {
      if (ev.kind == lcom::ScriptEvent::Kind::Mouse) {
        total_dx += ev.dx;
        total_dy += ev.dy;
        move_events++;
      }
    }
  }
  CHECK(move_events == 5);
  CHECK(total_dx == 10);
  CHECK(total_dy == -5);
}

int main() {
  test_irq_controller_shape();
  test_pit_programming_and_readback();
  test_pit_counter_latches_and_one_shot();
  test_i8042_keyboard_irq_and_ports();
  test_i8042_reasserts_buffered_irq_on_subscribe();
  test_i8042_keyboard_common_scancodes();
  test_mouse_scheduler_coalesces_motion();
  test_mouse_scheduler_clamps_and_flips_y();
  test_mouse_scheduler_button_change_emits_zero_delta();
  test_mouse_scheduler_bounds_backlog();
  test_i8042_command_byte_and_mouse_packet();
  test_i8042_remote_mouse_polling();
  test_i8042_wheel_negotiation_and_packet_framing();
  test_rtc_cmos_bcd_registers();
  test_rtc_register_writes_and_interrupts();
  test_vbe_mode_and_framebuffer();
  test_vbe_caption_pixels();
  test_vga_dac_ports();
  test_wav_backend_streams_multiple_blocks();
  test_protocol_decode_accepts_unaligned_storage();
  test_uart_loopback_registers();
  test_uart_pair_virtual_cable();
  test_uart_virtual_wire_preserves_bursts();
  test_uart_fifo_threshold_and_timeout();
  test_ac97_registers_and_buffer();
  test_ps2_fault_replay_and_status();
  test_ps2_script_validation();
  test_script_time_and_demo_events();

  if (g_failures != 0) {
    std::cerr << g_failures << " unit checks failed\n";
    return 1;
  }
  std::cout << "unit tests passed\n";
  return 0;
}
