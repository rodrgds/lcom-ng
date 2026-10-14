#ifndef MACHINE_LAB_RTC_CMOS_HPP
#define MACHINE_LAB_RTC_CMOS_HPP

#include "../core/IrqController.hpp"
#include "../core/PortDevice.hpp"

#include <array>
#include <cstdint>
#include <ctime>
#include <string>

namespace lcom {

class RtcCmos : public PortDevice {
public:
  explicit RtcCmos(IrqController &irqs);

  bool handles(uint16_t port) const override;
  bool read8(uint16_t port, uint8_t &value) override;
  bool write8(uint16_t port, uint8_t value) override;

  void setHostTime();
  bool setIsoTime(const std::string &iso);
  void setBinaryMode(bool binary);
  void setUpdateInProgress(bool uip) { update_in_progress_ = uip; }
  void advanceTick(uint32_t timer_frequency);
  void refreshIrq();

private:
  uint8_t readRegister(uint8_t reg);
  bool writeRegister(uint8_t reg, uint8_t value);
  uint8_t encode(uint8_t value) const;
  uint8_t decode(uint8_t value) const;
  uint8_t encodedHour() const;
  bool alarmMatches();
  void advanceSecond();
  static int daysInMonth(int year, int month);

  IrqController &irqs_;
  uint8_t selected_ = 0;
  std::tm tm_{};
  std::tm clock_{}; // Internal clock keeps counting while SET freezes user registers.
  bool time_written_ = false;
  std::array<uint8_t, 128> ram_{};
  uint8_t register_a_ = 0x26;
  uint8_t register_b_ = 0x02;
  uint8_t interrupt_flags_ = 0;
  bool update_in_progress_ = false;
  long double second_phase_ = 0;
  long double periodic_phase_ = 0;
};

} // namespace lcom

#endif
