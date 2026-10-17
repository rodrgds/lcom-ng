#ifndef LCOM_EMBEDDED_CONTROLS_HPP
#define LCOM_EMBEDDED_CONTROLS_HPP
#include <cstdint>
#include <array>
#include <string>
#include <vector>
#include "Pointer.hpp"
namespace lcom::embedded {
struct PadConfig {
  bool enabled = false;
  std::string mode = "mouse", side = "left";
  float sensitivity = 1;
  bool directional = false;
  std::array<std::string,4> keys{}; // left, up, right, down; empty uses mode.
  std::string axes = "both";
  bool wheel = false;
  float maxAngle = 135, deadZone = 8;
};
struct ActionConfig {
  std::string side, label, key;
  uint8_t mouseButton = 0;
  std::string position;
  std::string icon;
  int wheel = 0;
};
struct ControlConfig {
  std::string name = "lcom-ng", orientation = "auto", layout = "panel", pointer = "drag", font = "sans";
  bool fullscreen = false, keyboard = true, mouseButtons = true, joystickMenu = true;
  std::vector<std::string> quickKeys{"ESC", "SPACE", "ENTER"};
  std::vector<std::string> repeatKeys;
  std::vector<std::string> repeatReleaseKeys;
  PadConfig joystick, look, dpad;
  std::string touch = "auto";
  std::vector<PadConfig> pads;
  std::vector<ActionConfig> actions;
  CursorConfig cursor;
  uint32_t accent = 0x72d6b0, keyboardAccent = 0x72d6b0, button = 0x222222, text = 0xeeeeee;
  std::string toolbar = "auto", keyboardPosition = "auto";
  int tapDuration = 0;
  bool touchClick = true;
};
// Set before lcom_init. Desktop's normal runtime does not use this mobile host.
void configureControls(const ControlConfig &config);
}
#endif
