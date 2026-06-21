#include "I8042.hpp"

#include <lcom/i8042.h>

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <map>

namespace lcom {

I8042::I8042(IrqController &irqs) : irqs_(irqs) {}

bool I8042::handles(uint16_t port) const {
  return port == KBC_OUT_BUF || port == KBC_ST_REG;
}

uint8_t I8042::status() const {
  uint8_t st = 0;
  if (!output_.empty()) {
    st |= KBC_ST_OBF;
    if (output_.front().aux) st |= KBC_ST_AUX;
  }
  return st;
}

bool I8042::read8(uint16_t port, uint8_t &value) {
  if (port == KBC_ST_REG) {
    value = status();
    return true;
  }

  if (port == KBC_OUT_BUF) {
    if (output_.empty()) {
      value = 0;
      return true;
    }
    value = output_.front().value;
    output_.pop_front();
    raiseForFront();
    return true;
  }

  return false;
}

bool I8042::write8(uint16_t port, uint8_t value) {
  if (port == KBC_CMD_REG) {
    switch (value) {
    case KBC_CMD_READ_CB:
      queueOutput(command_byte_, false);
      break;
    case KBC_CMD_WRITE_CB:
      pending_write_ = PendingWrite::CommandByte;
      break;
    case KBC_CMD_DISABLE:
      keyboard_enabled_ = false;
      command_byte_ |= KBC_CB_DIS;
      break;
    case KBC_CMD_ENABLE:
      keyboard_enabled_ = true;
      command_byte_ &= static_cast<uint8_t>(~KBC_CB_DIS);
      break;
    case KBC_CMD_WRITE_MOUSE:
      pending_write_ = PendingWrite::MouseCommand;
      break;
    default:
      break;
    }
    return true;
  }

  if (port == KBC_IN_BUF) {
    if (pending_write_ == PendingWrite::CommandByte) {
      command_byte_ = value;
      keyboard_enabled_ = (command_byte_ & KBC_CB_DIS) == 0;
      pending_write_ = PendingWrite::None;
      return true;
    }
    if (pending_write_ == PendingWrite::MouseCommand) {
      pending_write_ = PendingWrite::None;
      handleMouseCommand(value);
      return true;
    }
    return true;
  }

  return false;
}

void I8042::queueOutput(uint8_t value, bool aux) {
  output_.push_back(OutputByte{value, aux});
  if (output_.size() == 1) raiseForFront();
}

void I8042::raiseForFront() {
  if (output_.empty()) return;
  if (output_.front().aux) {
    if ((command_byte_ & KBC_CB_INT2) != 0) irqs_.raise(MOUSE_IRQ);
  } else {
    if ((command_byte_ & KBC_CB_INT) != 0) irqs_.raise(KBC_IRQ);
  }
}

void I8042::refreshIrq() {
  raiseForFront();
}

void I8042::injectKeyScancode(uint8_t byte) {
  if (!keyboard_enabled_) return;
  queueOutput(byte, false);
}

void I8042::injectKeySequence(const std::vector<uint8_t> &bytes) {
  for (uint8_t byte : bytes) injectKeyScancode(byte);
}

void I8042::injectMousePacket(int dx, int dy, uint8_t buttons) {
  if (!mouse_reporting_) return;
  if (dx > 255) dx = 255;
  if (dx < -255) dx = -255;
  if (dy > 255) dy = 255;
  if (dy < -255) dy = -255;

  uint8_t b0 = MOUSE_SYNC_BIT;
  if (buttons & 0x01u) b0 |= BIT(0);
  if (buttons & 0x02u) b0 |= BIT(1);
  if (buttons & 0x04u) b0 |= BIT(2);
  if (dx < 0) b0 |= BIT(4);
  if (dy < 0) b0 |= BIT(5);

  queueOutput(b0, true);
  queueOutput(static_cast<uint8_t>(dx & 0xFF), true);
  queueOutput(static_cast<uint8_t>(dy & 0xFF), true);
}

void I8042::handleMouseCommand(uint8_t command) {
  switch (command) {
  case MOUSE_CMD_ENABLE_DR:
    mouse_reporting_ = true;
    queueOutput(MOUSE_ACK, true);
    break;
  case MOUSE_CMD_DISABLE_DR:
  case MOUSE_CMD_SET_DEFAULT:
  case MOUSE_CMD_RESET:
    mouse_reporting_ = false;
    queueOutput(MOUSE_ACK, true);
    break;
  case MOUSE_CMD_SET_STREAM:
  case MOUSE_CMD_SET_REMOTE:
  case MOUSE_CMD_READ_DATA:
    queueOutput(MOUSE_ACK, true);
    break;
  default:
    queueOutput(MOUSE_NACK, true);
    break;
  }
}

static std::string upperKey(std::string key) {
  std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c) {
    return static_cast<char>(std::toupper(c));
  });
  return key;
}

std::vector<uint8_t> ps2Set1ForKey(const std::string &key_in, bool pressed) {
  static const std::map<std::string, uint8_t> one_byte = {
      {"ESC", 0x01},       {"ESCAPE", 0x01},   {"1", 0x02},
      {"2", 0x03},         {"3", 0x04},        {"4", 0x05},
      {"5", 0x06},         {"6", 0x07},        {"7", 0x08},
      {"8", 0x09},         {"9", 0x0A},        {"0", 0x0B},
      {"MINUS", 0x0C},     {"EQUALS", 0x0D},   {"EQUAL", 0x0D},
      {"BACKSPACE", 0x0E}, {"BS", 0x0E},       {"TAB", 0x0F},
      {"Q", 0x10},         {"W", 0x11},        {"E", 0x12},
      {"R", 0x13},         {"T", 0x14},        {"Y", 0x15},
      {"U", 0x16},         {"I", 0x17},        {"O", 0x18},
      {"P", 0x19},         {"LEFTBRACKET", 0x1A}, {"LBRACKET", 0x1A},
      {"RIGHTBRACKET", 0x1B}, {"RBRACKET", 0x1B},
      {"ENTER", 0x1C},     {"RETURN", 0x1C},   {"LCTRL", 0x1D},
      {"LEFTCTRL", 0x1D},  {"CTRL", 0x1D},     {"A", 0x1E},
      {"S", 0x1F},         {"D", 0x20},        {"F", 0x21},
      {"G", 0x22},         {"H", 0x23},        {"J", 0x24},
      {"K", 0x25},         {"L", 0x26},        {"SEMICOLON", 0x27},
      {"APOSTROPHE", 0x28}, {"QUOTE", 0x28},   {"GRAVE", 0x29},
      {"BACKQUOTE", 0x29}, {"LSHIFT", 0x2A},   {"LEFTSHIFT", 0x2A},
      {"BACKSLASH", 0x2B}, {"Z", 0x2C},        {"X", 0x2D},
      {"C", 0x2E},         {"V", 0x2F},        {"B", 0x30},
      {"N", 0x31},         {"M", 0x32},        {"COMMA", 0x33},
      {"PERIOD", 0x34},    {"DOT", 0x34},      {"SLASH", 0x35},
      {"RSHIFT", 0x36},    {"RIGHTSHIFT", 0x36},
      {"KP_MULTIPLY", 0x37}, {"LALT", 0x38},   {"LEFTALT", 0x38},
      {"ALT", 0x38},       {"SPACE", 0x39},    {"CAPSLOCK", 0x3A},
      {"CAPS", 0x3A},      {"F1", 0x3B},       {"F2", 0x3C},
      {"F3", 0x3D},        {"F4", 0x3E},       {"F5", 0x3F},
      {"F6", 0x40},        {"F7", 0x41},       {"F8", 0x42},
      {"F9", 0x43},        {"F10", 0x44},      {"NUMLOCK", 0x45},
      {"SCROLLLOCK", 0x46}, {"KP_7", 0x47},    {"KP7", 0x47},
      {"KP_8", 0x48},      {"KP8", 0x48},      {"KP_9", 0x49},
      {"KP9", 0x49},       {"KP_MINUS", 0x4A}, {"KP_4", 0x4B},
      {"KP4", 0x4B},       {"KP_5", 0x4C},     {"KP5", 0x4C},
      {"KP_6", 0x4D},      {"KP6", 0x4D},      {"KP_PLUS", 0x4E},
      {"KP_1", 0x4F},      {"KP1", 0x4F},      {"KP_2", 0x50},
      {"KP2", 0x50},       {"KP_3", 0x51},     {"KP3", 0x51},
      {"KP_0", 0x52},      {"KP0", 0x52},      {"KP_PERIOD", 0x53},
      {"F11", 0x57},       {"F12", 0x58}};

  static const std::map<std::string, uint8_t> e0 = {
      {"KP_DIVIDE", 0x35}, {"KP_ENTER", 0x1C}, {"RCTRL", 0x1D},
      {"RIGHTCTRL", 0x1D}, {"RALT", 0x38},     {"RIGHTALT", 0x38},
      {"HOME", 0x47},      {"UP", 0x48},       {"PAGEUP", 0x49},
      {"PGUP", 0x49},      {"LEFT", 0x4B},     {"RIGHT", 0x4D},
      {"END", 0x4F},       {"DOWN", 0x50},     {"PAGEDOWN", 0x51},
      {"PGDOWN", 0x51},    {"INSERT", 0x52},   {"INS", 0x52},
      {"DELETE", 0x53},    {"DEL", 0x53},      {"MENU", 0x5D}};

  std::string key = upperKey(key_in);
  auto it = one_byte.find(key);
  if (it != one_byte.end()) {
    uint8_t code = it->second;
    if (!pressed) code |= 0x80u;
    return {code};
  }

  auto ext = e0.find(key);
  if (ext != e0.end()) {
    uint8_t code = ext->second;
    if (!pressed) code |= 0x80u;
    return {0xE0, code};
  }

  if (key.rfind("0X", 0) == 0) {
    uint8_t code = static_cast<uint8_t>(std::strtoul(key.c_str(), nullptr, 16));
    if (!pressed) code |= 0x80u;
    return {code};
  }

  return {};
}

} // namespace lcom
