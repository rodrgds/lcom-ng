#include "Pit8254.hpp"
#include <lcom/i8254.h>

namespace lcom {

Pit8254::Pit8254(IrqController &irqs) : irqs_(irqs) {}
bool Pit8254::handles(uint16_t port) const { return port >= TIMER_0 && port <= TIMER_CTRL; }
int Pit8254::channelFromPort(uint16_t port) {
  return port >= TIMER_0 && port <= TIMER_2 ? static_cast<int>(port - TIMER_0) : -1;
}
uint32_t Pit8254::decodedReload(const Channel &ch) {
  if (!ch.bcd) return ch.divisor == 0 ? 65536u : ch.divisor;
  if (!ch.divisor) return 10000u;
  return (ch.divisor & 15u) + ((ch.divisor >> 4) & 15u) * 10u +
         ((ch.divisor >> 8) & 15u) * 100u + ((ch.divisor >> 12) & 15u) * 1000u;
}
uint16_t Pit8254::encodedCount(const Channel &ch) {
  if (!ch.bcd) return static_cast<uint16_t>(ch.count);
  uint32_t value = ch.count % 10000u;
  return static_cast<uint16_t>((value % 10u) | ((value / 10u % 10u) << 4) |
                               ((value / 100u % 10u) << 8) | ((value / 1000u) << 12));
}
void Pit8254::latchCount(Channel &ch) {
  if (ch.latched_count_valid) return;
  ch.latched_count = encodedCount(ch);
  ch.latched_count_valid = true; ch.latched_count_msb = false;
}

bool Pit8254::read8(uint16_t port, uint8_t &value) {
  int channel = channelFromPort(port);
  if (channel < 0) return false;
  Channel &ch = channels_[static_cast<size_t>(channel)];
  if (ch.latched_status_valid) {
    value = ch.latched_status; ch.latched_status_valid = false; return true;
  }
  uint16_t count = ch.latched_count_valid ? ch.latched_count : encodedCount(ch);
  bool &msb = ch.latched_count_valid ? ch.latched_count_msb : ch.read_msb;
  if (ch.access == TIMER_MSB) {
    value = static_cast<uint8_t>(count >> 8); ch.latched_count_valid = false;
  } else if (ch.access == TIMER_LSB) {
    value = static_cast<uint8_t>(count); ch.latched_count_valid = false;
  } else if (!msb) {
    value = static_cast<uint8_t>(count); msb = true;
  } else {
    value = static_cast<uint8_t>(count >> 8); msb = false; ch.latched_count_valid = false;
  }
  return true;
}
void Pit8254::loadCount(Channel &ch, uint16_t divisor) {
  ch.divisor = divisor; ch.null_count = ch.load_pending = true;
}

bool Pit8254::write8(uint16_t port, uint8_t value) {
  if (port == TIMER_CTRL) { writeControl(value); return true; }
  int channel = channelFromPort(port);
  if (channel < 0) return false;
  Channel &ch = channels_[static_cast<size_t>(channel)];
  switch (ch.access) {
  case TIMER_LSB: loadCount(ch, value); break;
  case TIMER_MSB: loadCount(ch, static_cast<uint16_t>(value) << 8); break;
  case TIMER_LSB_MSB:
    if (!ch.write_msb) {
      ch.pending_lsb = value; ch.write_msb = true;
      if (ch.mode == 0) { ch.running = false; ch.output = false; }
    }
    else { loadCount(ch, static_cast<uint16_t>((static_cast<uint16_t>(value) << 8) | ch.pending_lsb)); ch.write_msb = false; }
    break;
  default: break;
  }
  return true;
}

void Pit8254::writeControl(uint8_t value) {
  if ((value & TIMER_RB_CMD) == TIMER_RB_CMD) {
    for (uint8_t channel = 0; channel < 3; ++channel) {
      if (!(value & TIMER_RB_SEL(channel))) continue;
      Channel &ch = channels_[channel];
      if (!(value & TIMER_RB_COUNT_)) latchCount(ch);
      if (!(value & TIMER_RB_STATUS_) && !ch.latched_status_valid) {
        ch.latched_status = composeStatus(channel); ch.latched_status_valid = true;
      }
    }
    return;
  }
  uint8_t selected = (value >> 6) & 3u;
  if (selected > 2) return;
  Channel &ch = channels_[selected];
  if (!(value & TIMER_LSB_MSB)) { latchCount(ch); return; }
  ch.access = value & TIMER_LSB_MSB;
  ch.mode = (value >> 1) & 7u;
  if (ch.mode == 6) ch.mode = 2;
  if (ch.mode == 7) ch.mode = 3;
  ch.bcd = (value & TIMER_BCD) != 0;
  ch.write_msb = ch.read_msb = false;
  ch.null_count = true; ch.load_pending = ch.running = ch.strobe_pending = ch.terminal_count = false;
  ch.output = ch.mode != 0;
}

uint8_t Pit8254::composeStatus(uint8_t channel) const {
  const Channel &ch = channels_[channel];
  return static_cast<uint8_t>((ch.output ? 0x80u : 0) | (ch.null_count ? 0x40u : 0) |
                              ch.access | (ch.mode << 1) | (ch.bcd ? 1u : 0));
}

void Pit8254::advanceTick() {
  // The existing runtime quantum is one programmed counter-0 period. Input
  // clock edges within that period are coalesced. New counts transfer at the
  // start of this quantum; sub-quantum load delays and OUT edges are not exposed.
  uint32_t clocks = decodedReload(channels_[0]);
  for (uint8_t channel = 0; channel < 3; ++channel) {
    Channel &ch = channels_[channel];
    if (ch.load_pending) {
      // Modes 1 and 5 need an external GATE edge. This runtime does not provide
      // the speaker/GATE pins, so they remain armed without a fabricated trigger.
      if (ch.mode == 1 || ch.mode == 5) continue;
      ch.load_pending = ch.null_count = false; ch.running = true;
      ch.count = decodedReload(ch); ch.phase = 0; ch.output = ch.mode != 0;
      ch.strobe_pending = ch.terminal_count = false;
    }
    if (!ch.running) continue;
    uint32_t reload = decodedReload(ch);
    bool rising_edge = false;
    if (ch.mode == 2 || ch.mode == 3) {
      uint64_t elapsed = static_cast<uint64_t>(ch.phase) + clocks;
      rising_edge = elapsed >= reload;
      ch.phase = static_cast<uint32_t>(elapsed % reload);
      if (ch.mode == 2) {
        ch.count = reload - ch.phase; ch.output = ch.count != 1;
      } else {
        uint32_t high_clocks = (reload + 1) / 2;
        ch.output = ch.phase < high_clocks;
        uint32_t local = ch.output ? ch.phase : ch.phase - high_clocks;
        if ((reload & 1u) == 0) ch.count = reload - local * 2;
        else ch.count = reload - 1 - local * 2;
      }
    } else if (ch.mode == 0 || ch.mode == 4) {
      uint32_t wrap = ch.bcd ? 10000u : 65536u;
      if (ch.terminal_count) {
        ch.count = (ch.count + wrap - clocks % wrap) % wrap;
        if (ch.strobe_pending) { ch.output = true; ch.strobe_pending = false; rising_edge = true; }
      } else if (clocks >= ch.count) {
        uint32_t remaining = clocks - ch.count;
        ch.count = (wrap - remaining % wrap) % wrap; ch.terminal_count = true;
        if (ch.mode == 0 || remaining > 0) { ch.output = true; rising_edge = true; }
        else { ch.output = false; ch.strobe_pending = true; }
      } else ch.count -= clocks;
    }
    if (channel == 0 && rising_edge) irqs_.raise(TIMER0_IRQ);
  }
}

uint32_t Pit8254::channelFrequency(uint8_t channel) const {
  return channel < channels_.size() ? TIMER_FREQ / decodedReload(channels_[channel]) : 0;
}
uint16_t Pit8254::divisor(uint8_t channel) const {
  return channel < channels_.size() ? channels_[channel].divisor : 0;
}
uint8_t Pit8254::status(uint8_t channel) const {
  return channel < channels_.size() ? composeStatus(channel) : 0;
}

} // namespace lcom
