#ifndef MACHINE_LAB_I8042_HPP
#define MACHINE_LAB_I8042_HPP

#include "../core/IrqController.hpp"
#include "../core/PortDevice.hpp"

#include <cstdint>
#include <deque>
#include <string>
#include <vector>

namespace lcom {

class I8042 : public PortDevice {
public:
  explicit I8042(IrqController &irqs);

  bool handles(uint16_t port) const override;
  bool read8(uint16_t port, uint8_t &value) override;
  bool write8(uint16_t port, uint8_t value) override;

  // Replay an already-received controller byte, preserving its source and errors.
  void injectOutputByte(uint8_t byte, bool aux, uint8_t errors);
  void injectKeyScancode(uint8_t byte);
  void injectKeySequence(const std::vector<uint8_t> &bytes);
  void injectMousePacket(int dx, int dy, uint8_t buttons);
  void refreshIrq();

  uint8_t status() const;
  uint8_t commandByte() const { return command_byte_; }
  bool mouseReporting() const { return mouse_reporting_; }

private:
  struct OutputByte {
    uint8_t value = 0;
    bool aux = false;
    uint8_t errors = 0;
  };

  enum class PendingWrite {
    None,
    CommandByte,
    MouseCommand
  };

  void queueOutput(uint8_t value, bool aux, uint8_t errors = 0);
  void raiseForFront();
  void handleMouseCommand(uint8_t command);
  void queueMousePacket();
  void resetMouseCounters();

  IrqController &irqs_;
  std::deque<OutputByte> output_;
  uint8_t command_byte_ = 0x03;
  PendingWrite pending_write_ = PendingWrite::None;
  bool keyboard_enabled_ = true;
  bool mouse_reporting_ = false;
  bool mouse_remote_ = false;
  uint8_t mouse_parameter_ = 0;
  uint8_t mouse_sample_rate_ = 100, mouse_resolution_ = 2;
  int mouse_dx_ = 0, mouse_dy_ = 0;
  bool mouse_x_overflow_ = false, mouse_y_overflow_ = false;
  uint8_t mouse_buttons_ = 0;
};

std::vector<uint8_t> ps2Set1ForKey(const std::string &key, bool pressed);

} // namespace lcom

#endif
