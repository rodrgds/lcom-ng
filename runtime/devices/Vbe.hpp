#ifndef MACHINE_LAB_VBE_HPP
#define MACHINE_LAB_VBE_HPP

#include "../core/PortDevice.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <lcom/vbe_modes.h>
#include <lcom/memory_map.h>
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
  explicit Vbe(uint32_t framebuffer_phys = LCOM_VBE_DEFAULT_PHYS);

  bool handles(uint16_t port) const override;
  bool read8(uint16_t port, uint8_t &value) override;
  bool write8(uint16_t port, uint8_t value) override;

  bool modeInfo(uint16_t mode, VbeModeInfo &out) const;
  bool setMode(uint16_t mode);
  bool setDisplayStart(uint16_t x, uint16_t y);
  bool dacFormat(uint8_t operation, uint8_t requested, uint8_t &actual);
  bool scanline(uint8_t operation, uint16_t value);
  uint16_t scanlinePixels() const { return current_.bytes_per_pixel ? current_.pitch / current_.bytes_per_pixel : 0; }
  uint16_t scanlineLines() const { return current_.pitch ? vramSize() / current_.pitch : 0; }
  // Copy the visible viewport from guest VRAM, retaining logical row pitch.
  // Returns whether any visible pixel changed; padding never reaches a host.
  bool syncFramebuffer(const uint8_t *vram, size_t length);
  size_t displayOffset() const { return display_offset_; }
  static constexpr size_t vramSize() { return LCOM_VBE_VRAM_BYTES; }
  const VbeModeInfo &currentMode() const { return current_; }
  bool graphicsMode() const { return graphics_mode_; }
  bool textMode() const { return !graphics_mode_ && text_mapped_; }
  bool displayMode() const { return graphicsMode() || textMode(); }
  void mapTextMemory();
  void advanceTick(uint32_t frequency);
  static constexpr size_t mappedVideoBytes() { return vramSize() + LCOM_VGA_TEXT_BYTES; }

  uint64_t framebufferPhys() const { return framebuffer_phys_; }
  size_t framebufferSize() const { return framebuffer_.size(); }
  std::vector<uint8_t> &framebuffer() { return framebuffer_; }
  const std::vector<uint8_t> &framebuffer() const { return framebuffer_; }
  bool ownsRange(uint64_t phys, uint64_t length) const;
  // Decode one framebuffer pixel to RGB using the current mode. Indexed mode
  // uses the current six- or eight-bit VGA DAC palette and pixel mask.
  void pixelRgb(const uint8_t *pixel, uint8_t rgb[3]) const;
  // Copy a visible framebuffer row to width * 3 bytes of RGB output.
  void copyRgbRow(uint16_t y, uint8_t *rgb) const;
  // Copy a visible row to opaque RGBA without an intermediate RGB buffer.
  void copyRgbaRow(uint16_t y, uint8_t *rgba) const;
  bool dumpPpm(const std::string &path, const std::string &caption = "",
               const std::string &caption_position = "bottom") const;

private:
  void copyRgb(const uint8_t *pixels, size_t count, uint8_t *rgb) const;
  void resetPalette(bool text_mode = false);
  void refreshPalette();
  void enterTextMode();
  bool syncTextFramebuffer(const uint8_t *vram, size_t length);
  bool text_mapped_ = false;
  bool text_invalid_ = true;
  bool text_blink_hidden_ = false;
  double text_elapsed_seconds_ = 0;
  std::array<uint8_t, 80 * 25 * 2> text_cells_{};
  uint8_t dac_width_ = 6;
  std::array<std::array<uint8_t, 3>, 256> palette_{};
  // Expanded DAC colors, refreshed on reset and each component write.
  std::array<std::array<uint8_t, 4>, 256> rgba_palette_{};
  uint8_t pixel_mask_ = 0xff;
  uint8_t dac_read_index_ = 0;
  uint8_t dac_write_index_ = 0;
  uint8_t dac_read_component_ = 0;
  uint8_t dac_write_component_ = 0;
  uint8_t dac_state_ = 0;
  const uint64_t framebuffer_phys_;
  VbeModeInfo current_{};
  bool graphics_mode_ = false;
  bool has_frame_ = false;
  size_t display_offset_ = 0;
  uint16_t display_x_ = 0;
  uint16_t display_y_ = 0;
  std::vector<uint8_t> framebuffer_;
};

} // namespace lcom

#endif
