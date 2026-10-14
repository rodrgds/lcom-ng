#ifndef MACHINE_LAB_VBE_HPP
#define MACHINE_LAB_VBE_HPP

#include "../core/PortDevice.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace lcom {

struct VbeModeInfo {
  uint16_t mode = 0;
  uint16_t width = 0;
  uint16_t height = 0;
  uint8_t bpp = 0;
  uint8_t bytes_per_pixel = 0;
  uint32_t pitch = 0;
  uint64_t framebuffer_phys = 0;
  uint64_t framebuffer_size = 0;
};

class Vbe : public PortDevice {
public:
  Vbe();

  bool handles(uint16_t port) const override;
  bool read8(uint16_t port, uint8_t &value) override;
  bool write8(uint16_t port, uint8_t value) override;

  bool modeInfo(uint16_t mode, VbeModeInfo &out) const;
  bool setMode(uint16_t mode);
  bool setDisplayStart(uint16_t x, uint16_t y);
  size_t displayOffset() const { return display_offset_; }
  static constexpr size_t vramSize() { return 16u * 1024u * 1024u; }
  const VbeModeInfo &currentMode() const { return current_; }
  bool graphicsMode() const { return graphics_mode_; }

  uint64_t framebufferPhys() const { return framebuffer_phys_; }
  size_t framebufferSize() const { return framebuffer_.size(); }
  std::vector<uint8_t> &framebuffer() { return framebuffer_; }
  const std::vector<uint8_t> &framebuffer() const { return framebuffer_; }
  bool ownsRange(uint64_t phys, uint64_t length) const;
  // Decode one framebuffer pixel to RGB using the current mode. Indexed mode
  // uses the current six-bit VGA DAC palette and pixel mask.
  void pixelRgb(const uint8_t *pixel, uint8_t rgb[3]) const;
  // Copy a visible framebuffer row to width * 3 bytes of RGB output.
  void copyRgbRow(uint16_t y, uint8_t *rgb) const;
  bool dumpPpm(const std::string &path, const std::string &caption = "",
               const std::string &caption_position = "bottom") const;

private:
  void copyRgb(const uint8_t *pixels, size_t count, uint8_t *rgb) const;
  void resetPalette();
  std::array<std::array<uint8_t, 3>, 256> palette_{};
  // Expanded DAC colors, refreshed on reset and each component write.
  std::array<std::array<uint8_t, 3>, 256> rgb_palette_{};
  uint8_t pixel_mask_ = 0xff;
  uint8_t dac_read_index_ = 0;
  uint8_t dac_write_index_ = 0;
  uint8_t dac_read_component_ = 0;
  uint8_t dac_write_component_ = 0;
  uint8_t dac_state_ = 0;
  uint64_t framebuffer_phys_ = 0xE0000000ull;
  VbeModeInfo current_{};
  bool graphics_mode_ = false;
  bool has_graphics_frame_ = false;
  size_t display_offset_ = 0;
  std::vector<uint8_t> framebuffer_;
};

} // namespace lcom

#endif
