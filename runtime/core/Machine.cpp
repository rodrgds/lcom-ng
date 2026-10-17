#include "Machine.hpp"

#include <lcom/uart16550.h>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace lcom {

namespace {
uint32_t checkedVbeBase(const MachineConfig &config) {
  if (!validMachineConfig(config))
    throw std::invalid_argument("Invalid or overlapping machine physical apertures");
  return config.vbe_physical_base;
}
}

Machine::Machine(MachineConfig config)
    : pit_(irqs_), i8042_(irqs_), rtc_(irqs_), vbe_(checkedVbeBase(config)), ac97_(config.ac97_pcm_base),
      com1_(COM1_BASE, COM1_IRQ, irqs_), com2_(COM2_BASE, COM2_IRQ, irqs_) {
  com1_.connectPeer(&com2_);
  com2_.connectPeer(&com1_);
  bus_.attach(&pit_);
  bus_.attach(&i8042_);
  bus_.attach(&rtc_);
  bus_.attach(&vbe_);
  bus_.attach(&ac97_);
  bus_.attach(&com1_);
  bus_.attach(&com2_);
}

bool Machine::readPort8(uint16_t port, uint8_t &value) {
  bool ok = bus_.read8(port, value);
  if (trace_ != nullptr) trace_->logPortRead(tick_, port, ok ? value : 0, ok);
  return ok;
}

std::optional<Machine::PhysicalRange> Machine::physicalRange(uint64_t address, uint64_t length) const {
  if (vbe_.ownsRange(address, length))
    return PhysicalRange{PhysicalRange::Device::Video, static_cast<size_t>(address - vbe_.framebufferPhys())};
  if (length && address >= LCOM_VGA_TEXT_PHYS &&
      address <= LCOM_VGA_TEXT_PHYS + LCOM_VGA_TEXT_BYTES &&
      length <= LCOM_VGA_TEXT_PHYS + LCOM_VGA_TEXT_BYTES - address)
    return PhysicalRange{PhysicalRange::Device::Video, Vbe::vramSize() +
                         static_cast<size_t>(address - LCOM_VGA_TEXT_PHYS)};
  if (ac97_.ownsRange(address, length))
    return PhysicalRange{PhysicalRange::Device::Audio, static_cast<size_t>(address - ac97_.bufferPhys())};
  return std::nullopt;
}

std::optional<Machine::PhysicalRange> Machine::mapPhysicalRange(uint64_t address, uint64_t length) {
  auto range = physicalRange(address, length);
  if (range && range->device == PhysicalRange::Device::Video && range->offset >= Vbe::vramSize())
    vbe_.mapTextMemory();
  return range;
}

bool Machine::writePort8(uint16_t port, uint8_t value) {
  bool ok = bus_.write8(port, value);
  if (trace_ != nullptr) trace_->logPortWrite(tick_, port, value, ok);
  return ok;
}

bool Machine::subscribeIrq(uint8_t irq, IrqSubscription &out) {
  bool ok = irqs_.subscribe(irq, out);
  if (ok) refreshDeviceIrqs();
  return ok;
}

bool Machine::unsubscribeIrq(uint8_t irq) {
  return irqs_.unsubscribe(irq);
}

uint32_t Machine::consumePendingIrqs() {
  return irqs_.consumePending();
}

uint32_t Machine::pendingIrqs() const {
  return irqs_.pendingMask();
}

void Machine::refreshDeviceIrqs() {
  i8042_.refreshIrq();
  rtc_.refreshIrq();
  com1_.refreshIrq();
  com2_.refreshIrq();
}

void Machine::advanceTick() {
  tick_++;
  pit_.advanceTick();
  rtc_.advanceTick(pit_.channelFrequency(0));
  vbe_.advanceTick(pit_.channelFrequency(0));
  com1_.advanceTick(pit_.channelFrequency(0));
  com2_.advanceTick(pit_.channelFrequency(0));
  if (trace_ != nullptr && pendingIrqs() != 0) {
    for (uint8_t irq = 0; irq < 32; irq++) {
      uint32_t mask = (1u << irq);
      if ((pendingIrqs() & mask) != 0) trace_->logIrq(tick_, irq, mask);
    }
  }
}

bool Machine::delayTargetTick(uint64_t system_ticks, uint64_t &target) const {
  // Minix delays use a 60 Hz system clock even when the student changes PIT0.
  long double elapsed = std::ceil(static_cast<long double>(system_ticks) *
                                  pit_.channelFrequency(0) / 60.0L);
  if (elapsed > static_cast<long double>(std::numeric_limits<uint64_t>::max() - tick_))
    return false;
  target = tick_ + static_cast<uint64_t>(elapsed);
  return true;
}

void Machine::injectKey(const std::string &key, bool pressed) {
  injectKeyBytes(ps2Set1ForKey(key, pressed));
}

void Machine::injectKeyBytes(const std::vector<uint8_t> &bytes) {
  if (input_observer_ != nullptr) {
    for (uint8_t byte : bytes) input_observer_->onKeyScancode(byte);
  }
  i8042_.injectKeySequence(bytes);
  if (trace_ != nullptr && pendingIrqs() != 0) {
    trace_->log("input", "keyboard");
  }
}

void Machine::injectMouse(int dx, int dy, uint8_t buttons) {
  if (input_observer_ != nullptr && i8042_.mouseReporting()) {
    input_observer_->onMousePacket(dx, dy, buttons);
  }
  i8042_.injectMousePacket(dx, dy, buttons);
  if (trace_ != nullptr && pendingIrqs() != 0) {
    trace_->log("input", "mouse");
  }
}

void Machine::connectSerialPeer(Machine *peer) {
  if (peer == nullptr) {
    com1_.connectPeer(&com2_);
    com2_.connectPeer(&com1_);
    return;
  }
  com1_.connectPeer(&peer->com1_);
  com2_.connectPeer(&peer->com2_);
}

} // namespace lcom
