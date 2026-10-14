#include "RtcCmos.hpp"

#include <lcom/rtc.h>

#include <cmath>
#include <cstdio>
#include <ctime>

namespace lcom {
namespace {
constexpr uint8_t SET = 0x80, PIE = 0x40, AIE = 0x20, UIE = 0x10;
constexpr uint8_t IRQF = 0x80, PF = 0x40, AF = 0x20, UF = 0x10;
constexpr long double phase_tolerance = 1e-12L;
}

RtcCmos::RtcCmos(IrqController &irqs) : irqs_(irqs) { setHostTime(); }

bool RtcCmos::handles(uint16_t port) const {
  return port == RTC_ADDR_REG || port == RTC_DATA_REG;
}

bool RtcCmos::read8(uint16_t port, uint8_t &value) {
  if (port == RTC_ADDR_REG) { value = selected_; return true; }
  if (port == RTC_DATA_REG) { value = readRegister(selected_); return true; }
  return false;
}

bool RtcCmos::write8(uint16_t port, uint8_t value) {
  if (port == RTC_ADDR_REG) { selected_ = value & 0x7f; return true; }
  if (port == RTC_DATA_REG) return writeRegister(selected_, value);
  return false;
}

void RtcCmos::setHostTime() {
  std::time_t now = std::time(nullptr);
#if defined(_WIN32)
  localtime_s(&tm_, &now);
#else
  localtime_r(&now, &tm_);
#endif
  clock_ = tm_; time_written_ = false;
  second_phase_ = periodic_phase_ = 0;
}

int RtcCmos::daysInMonth(int year, int month) {
  static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (month < 1 || month > 12) return 0;
  return days[month - 1] + (month == 2 && year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

bool RtcCmos::setIsoTime(const std::string &iso) {
  int year, month, day, hour, minute, second, consumed = 0;
  if (std::sscanf(iso.c_str(), "%d-%d-%dT%d:%d:%d%n", &year, &month, &day,
                  &hour, &minute, &second, &consumed) != 6) {
    if (std::sscanf(iso.c_str(), "%d-%d-%d %d:%d:%d%n", &year, &month, &day,
                    &hour, &minute, &second, &consumed) != 6) return false;
  }
  if (consumed != static_cast<int>(iso.size()) || year < 1900 || year > 9999 ||
      day < 1 || day > daysInMonth(year, month) || hour < 0 || hour > 23 ||
      minute < 0 || minute > 59 || second < 0 || second > 59) return false;
  tm_ = {};
  tm_.tm_year = year - 1900; tm_.tm_mon = month - 1; tm_.tm_mday = day;
  tm_.tm_hour = hour; tm_.tm_min = minute; tm_.tm_sec = second;
  // Gregorian weekday without host timezone or daylight-saving dependence.
  static const int offsets[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  int weekday_year = year - (month < 3);
  tm_.tm_wday = (weekday_year + weekday_year / 4 - weekday_year / 100 +
                 weekday_year / 400 + offsets[month - 1] + day) % 7;
  clock_ = tm_; time_written_ = false;
  second_phase_ = periodic_phase_ = 0;
  return true;
}

void RtcCmos::setBinaryMode(bool binary) {
  register_b_ = static_cast<uint8_t>((register_b_ & ~RTC_DM) | (binary ? RTC_DM : 0));
}

uint8_t RtcCmos::encode(uint8_t value) const {
  return (register_b_ & RTC_DM) ? value : static_cast<uint8_t>(((value / 10) << 4) | (value % 10));
}
uint8_t RtcCmos::decode(uint8_t value) const {
  return (register_b_ & RTC_DM) ? value : static_cast<uint8_t>((value >> 4) * 10 + (value & 15));
}
uint8_t RtcCmos::encodedHour() const {
  if (register_b_ & RTC_24H) return encode(static_cast<uint8_t>(tm_.tm_hour));
  return static_cast<uint8_t>(encode(static_cast<uint8_t>(tm_.tm_hour % 12 == 0 ? 12 : tm_.tm_hour % 12)) |
                              (tm_.tm_hour >= 12 ? 0x80 : 0));
}

uint8_t RtcCmos::readRegister(uint8_t reg) {
  switch (reg) {
  case RTC_REG_SECONDS: return encode(static_cast<uint8_t>(tm_.tm_sec));
  case RTC_REG_MINUTES: return encode(static_cast<uint8_t>(tm_.tm_min));
  case RTC_REG_HOURS: return encodedHour();
  case 6: return encode(static_cast<uint8_t>(tm_.tm_wday + 1));
  case RTC_REG_DAY: return encode(static_cast<uint8_t>(tm_.tm_mday));
  case RTC_REG_MONTH: return encode(static_cast<uint8_t>(tm_.tm_mon + 1));
  case RTC_REG_YEAR: return encode(static_cast<uint8_t>((tm_.tm_year + 1900) % 100));
  case RTC_REG_A: {
    bool uip = !(register_b_ & SET) && (update_in_progress_ || second_phase_ >= 1.0L - 0.000244L);
    return static_cast<uint8_t>(register_a_ | (uip ? RTC_UIP : 0));
  }
  case RTC_REG_B: return register_b_;
  case RTC_REG_C: {
    uint8_t value = interrupt_flags_;
    if (interrupt_flags_ & register_b_ & (PIE | AIE | UIE)) value |= IRQF;
    interrupt_flags_ = 0; return value;
  }
  case RTC_REG_D: return 0x80;
  default: return ram_[reg];
  }
}

bool RtcCmos::writeRegister(uint8_t reg, uint8_t value) {
  if (reg == RTC_REG_C || reg == RTC_REG_D) return true; // Read-only registers.
  if (reg == RTC_REG_A) {
    register_a_ = value & 0x7f; periodic_phase_ = 0; return true;
  }
  if (reg == RTC_REG_B) {
    bool was_set = (register_b_ & SET) != 0;
    if (!was_set && (value & SET)) time_written_ = false;
    if (was_set && !(value & SET) && time_written_) clock_ = tm_;
    register_b_ = (value & SET) ? static_cast<uint8_t>(value & ~UIE) : value;
    refreshIrq(); return true;
  }
  uint8_t decoded = decode(value);
  switch (reg) {
  case RTC_REG_SECONDS: if (decoded > 59) return false; tm_.tm_sec = decoded; break;
  case RTC_REG_MINUTES: if (decoded > 59) return false; tm_.tm_min = decoded; break;
  case RTC_REG_HOURS:
    decoded = decode(value & 0x7f);
    if (register_b_ & RTC_24H) { if (decoded > 23) return false; tm_.tm_hour = decoded; }
    else { if (decoded < 1 || decoded > 12) return false; tm_.tm_hour = decoded % 12 + ((value & 0x80) ? 12 : 0); }
    break;
  case 6: if (decoded < 1 || decoded > 7) return false; tm_.tm_wday = decoded - 1; break;
  case RTC_REG_DAY: if (decoded < 1 || decoded > 31) return false; tm_.tm_mday = decoded; break;
  case RTC_REG_MONTH: if (decoded < 1 || decoded > 12) return false; tm_.tm_mon = decoded - 1; break;
  case RTC_REG_YEAR: if (decoded > 99) return false; tm_.tm_year = (tm_.tm_year / 100) * 100 + decoded; break;
  default: ram_[reg] = value; break;
  }
  if (reg <= RTC_REG_YEAR && reg != 1 && reg != 3 && reg != 5) {
    if (register_b_ & SET) time_written_ = true;
    else clock_ = tm_;
  }
  return true;
}

bool RtcCmos::alarmMatches() {
  auto matches = [](uint8_t alarm, uint8_t value) { return (alarm & 0xc0) == 0xc0 || alarm == value; };
  return matches(ram_[1], encode(static_cast<uint8_t>(tm_.tm_sec))) &&
         matches(ram_[3], encode(static_cast<uint8_t>(tm_.tm_min))) && matches(ram_[5], encodedHour());
}

void RtcCmos::advanceSecond() {
  if (++clock_.tm_sec < 60) return;
  clock_.tm_sec = 0;
  if (++clock_.tm_min < 60) return;
  clock_.tm_min = 0;
  if (++clock_.tm_hour < 24) return;
  clock_.tm_hour = 0; clock_.tm_wday = (clock_.tm_wday + 1) % 7;
  if (++clock_.tm_mday <= daysInMonth(clock_.tm_year + 1900, clock_.tm_mon + 1)) return;
  clock_.tm_mday = 1;
  if (++clock_.tm_mon < 12) return;
  clock_.tm_mon = 0; ++clock_.tm_year;
}

void RtcCmos::advanceTick(uint32_t timer_frequency) {
  if (!timer_frequency || (register_a_ & 0x70) != 0x20) return;
  long double seconds = 1.0L / timer_frequency;
  uint8_t rate = register_a_ & 15;
  uint32_t periodic_frequency = rate == 0 ? 0 : rate == 1 ? 256 : rate == 2 ? 128 : 32768u >> (rate - 1);
  periodic_phase_ += seconds * periodic_frequency;
  if (periodic_phase_ + phase_tolerance >= 1) {
    periodic_phase_ -= std::floor(periodic_phase_ + phase_tolerance);
    if (periodic_phase_ < 0) periodic_phase_ = 0;
    interrupt_flags_ |= PF;
  }
  second_phase_ += seconds;
  if (second_phase_ + phase_tolerance >= 1) {
    second_phase_ -= 1;
    if (second_phase_ < 0) second_phase_ = 0;
    advanceSecond();
    if (!(register_b_ & SET)) {
      tm_ = clock_; interrupt_flags_ |= UF;
      if (alarmMatches()) interrupt_flags_ |= AF;
    }
  }
  refreshIrq();
}

void RtcCmos::refreshIrq() {
  if (interrupt_flags_ & register_b_ & (PIE | AIE | UIE)) irqs_.raise(8);
}

} // namespace lcom
