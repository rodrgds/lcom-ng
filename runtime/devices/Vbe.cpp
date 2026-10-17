#include "Vbe.hpp"
#include "VgaFont.hpp"
#include "../core/MachineConfig.hpp"
#include <stdexcept>

#include <lcom/vbe.h>

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <cmath>

namespace lcom {

namespace {

template <unsigned Bits>
void copyDirectRgb(const uint8_t *pixels, size_t count, uint8_t *rgb) {
  for (size_t i = 0; i < count; ++i, pixels += (Bits + 7) / 8, rgb += 3) {
    if constexpr (Bits == 15 || Bits == 16) {
      const uint16_t value = static_cast<uint16_t>(pixels[0] | (pixels[1] << 8));
      rgb[0] = static_cast<uint8_t>(((value >> (Bits == 16 ? 11 : 10)) & 0x1f) * 255 / 31);
      rgb[1] = static_cast<uint8_t>(((value >> 5) & (Bits == 16 ? 0x3f : 0x1f)) * 255 /
                                    (Bits == 16 ? 63 : 31));
      rgb[2] = static_cast<uint8_t>((value & 0x1f) * 255 / 31);
    } else {
      rgb[0] = pixels[2];
      rgb[1] = pixels[1];
      rgb[2] = pixels[0];
    }
  }
}

const char *glyphRows(char c) {
  switch (std::toupper(static_cast<unsigned char>(c))) {
  case 'A': return "01110100011000111111100011000110001";
  case 'B': return "11110100011000111110100011000111110";
  case 'C': return "01111100001000010000100001000001111";
  case 'D': return "11110100011000110001100011000111110";
  case 'E': return "11111100001111010000100001000011111";
  case 'F': return "11111100001111010000100001000010000";
  case 'G': return "01111100001000010111100011000101111";
  case 'H': return "10001100011000111111100011000110001";
  case 'I': return "11111001000010000100001000010011111";
  case 'J': return "00111000100001000010000101001001100";
  case 'K': return "10001100101010011000101001001010001";
  case 'L': return "10000100001000010000100001000011111";
  case 'M': return "10001110111010110101100011000110001";
  case 'N': return "10001110011010110011100011000110001";
  case 'O': return "01110100011000110001100011000101110";
  case 'P': return "11110100011000111110100001000010000";
  case 'Q': return "01110100011000110001101011001001101";
  case 'R': return "11110100011000111110101001001010001";
  case 'S': return "01111100001000001110000010000111110";
  case 'T': return "11111001000010000100001000010000100";
  case 'U': return "10001100011000110001100011000101110";
  case 'V': return "10001100011000110001100010101000100";
  case 'W': return "10001100011000110101101011101110001";
  case 'X': return "10001100010101000100010101000110001";
  case 'Y': return "10001100010101000100001000010000100";
  case 'Z': return "11111000010001000100010001000011111";
  case '0': return "01110100011001110101110011000101110";
  case '1': return "00100011000010000100001000010001110";
  case '2': return "01110100010000100010001000100011111";
  case '3': return "11110000010000101110000010000111110";
  case '4': return "00010001100101010010111110001000010";
  case '5': return "11111100001111000001000011000101110";
  case '6': return "00110010001000011110100011000101110";
  case '7': return "11111000010001000100010000100001000";
  case '8': return "01110100011000101110100011000101110";
  case '9': return "01110100011000101111000010001001100";
  case ':': return "00000001000010000000001000010000000";
  case '.': return "00000000000000000000000000010000100";
  case '-': return "00000000000000011111000000000000000";
  case '/': return "00001000100001000100010001000010000";
  case '!': return "00100001000010000100001000000000100";
  case '?': return "01110100010000100010001000000000100";
  default: return "00000000000000000000000000000000000";
  }
}

void drawCaptionRow(const std::string &caption, const std::string &position,
                    uint16_t width, uint16_t height, uint16_t y, uint8_t *row) {
  if (caption.empty() || width < 160 || height < 96) return;
  const int scale = 3;
  const int char_w = 6 * scale;
  const int text_h = 7 * scale;
  const int max_chars = std::max(1, (static_cast<int>(width) - 96) / char_w);
  int text_len = static_cast<int>(std::min(caption.size(), static_cast<size_t>(max_chars)));
  int box_w = text_len * char_w + 48;
  int box_h = text_h + 28;
  int box_x = (static_cast<int>(width) - box_w) / 2;
  int box_y = position == "top" ? 24 : static_cast<int>(height) - box_h - 24;

  if (y < box_y || y >= box_y + box_h) return;
  for (int x = box_x; x < box_x + box_w; ++x) {
    row[x * 3] = 18;
    row[x * 3 + 1] = 24;
    row[x * 3 + 2] = 32;
  }
  const int ly = static_cast<int>(y) - box_y - 14;
  if (ly < 0 || ly >= text_h) return;
  for (int i = 0; i < text_len; ++i) {
    const char *glyph = glyphRows(caption[static_cast<size_t>(i)]) + (ly / scale) * 5;
    for (int column = 0; column < 5; ++column) {
      if (glyph[column] != '1') continue;
      const int x = box_x + 24 + i * char_w + column * scale;
      for (int dx = 0; dx < scale; ++dx) {
        row[(x + dx) * 3] = 236;
        row[(x + dx) * 3 + 1] = 246;
        row[(x + dx) * 3 + 2] = 255;
      }
    }
  }
}

} // namespace

Vbe::Vbe(uint32_t framebuffer_phys) : framebuffer_phys_(framebuffer_phys) {
  if (!validPhysicalAperture(framebuffer_phys, LCOM_VBE_VRAM_BYTES))
    throw std::invalid_argument("Invalid VBE physical framebuffer aperture");
  resetPalette();
  setMode(LCOM_VBE_MODE_800_600_24);
  graphics_mode_ = false;
  has_frame_ = false;
}

void Vbe::resetPalette(bool text_mode) {
  dac_width_ = 6;
  if (text_mode) {
    // VirtualBox VGA BIOS mode03 uses palette2, a 64-entry EGA DAC table.
    // Its VBE path preserves this table instead of loading mode13's palette3.
    palette_ = {};
    for (unsigned i = 0; i < 64; ++i) {
      palette_[i] = {
        static_cast<uint8_t>(((i & 4) ? 42 : 0) + ((i & 32) ? 21 : 0)),
        static_cast<uint8_t>(((i & 2) ? 42 : 0) + ((i & 16) ? 21 : 0)),
        static_cast<uint8_t>(((i & 1) ? 42 : 0) + ((i & 8) ? 21 : 0))
      };
    }
  } else {
    // The standard VGA BIOS 256-color palette: 16 EGA colors, 16 grays,
    // nine 24-color hue rings, then eight black entries. Channel values match
    // SeaBIOS rel-1.16.1 vgasrc/stdvgamodes.c palette3; generate the rings rather
    // than storing a second 768-byte table.
    static constexpr uint8_t ega[16][3] = {
      {0, 0, 0}, {0, 0, 42}, {0, 42, 0}, {0, 42, 42},
      {42, 0, 0}, {42, 0, 42}, {42, 21, 0}, {42, 42, 42},
      {21, 21, 21}, {21, 21, 63}, {21, 63, 21}, {21, 63, 63},
      {63, 21, 21}, {63, 21, 63}, {63, 63, 21}, {63, 63, 63}
    };
    static constexpr uint8_t gray[16] = {
      0, 5, 8, 11, 14, 17, 20, 24, 28, 32, 36, 40, 45, 50, 56, 63
    };
    static constexpr uint8_t levels[9][5] = {
      {0, 16, 31, 47, 63}, {31, 39, 47, 55, 63}, {45, 49, 54, 58, 63},
      {0, 7, 14, 21, 28}, {14, 17, 21, 24, 28}, {20, 22, 24, 26, 28},
      {0, 4, 8, 12, 16}, {8, 10, 12, 14, 16}, {11, 12, 13, 15, 16}
    };
    palette_ = {};
    for (size_t i = 0; i < 16; ++i) {
      std::copy_n(ega[i], 3, palette_[i].begin());
      palette_[16 + i].fill(gray[i]);
    }
    for (size_t ring = 0; ring < 9; ++ring) {
      const uint8_t *shade = levels[ring];
      for (size_t step = 0; step < 24; ++step) {
        const uint8_t ascending = shade[step % 4];
        const uint8_t descending = shade[4 - step % 4];
        auto &color = palette_[32 + ring * 24 + step];
        switch (step / 4) {
        case 0: color = {ascending, shade[0], shade[4]}; break;
        case 1: color = {shade[4], shade[0], descending}; break;
        case 2: color = {shade[4], ascending, shade[0]}; break;
        case 3: color = {descending, shade[4], shade[0]}; break;
        case 4: color = {shade[0], shade[4], ascending}; break;
        case 5: color = {shade[0], descending, shade[4]}; break;
        }
      }
    }
  }
  refreshPalette();
  pixel_mask_ = 0xff;
  dac_read_index_ = dac_write_index_ = 0;
  dac_read_component_ = dac_write_component_ = 0;
  dac_state_ = 0;
}

void Vbe::refreshPalette() {
  text_invalid_ = true;
  const unsigned mask = dac_width_ == 8 ? 255 : 63;
  for (size_t i = 0; i < palette_.size(); ++i)
    for (size_t component = 0; component < 3; ++component)
      rgb_palette_[i][component] = static_cast<uint8_t>((palette_[i][component] & mask) * 255 / mask);
}

bool Vbe::dacFormat(uint8_t operation, uint8_t requested, uint8_t &actual) {
  if ((graphics_mode_ && current_.bpp != 8) || operation > 1) return false;
  if (operation == 0) {
    if (requested < 6) return false;
    // VBE 3.0 4F08: select the next lower hardware-supported width.
    dac_width_ = requested >= 8 ? 8 : 6;
    refreshPalette();
  }
  actual = dac_width_;
  return true;
}

bool Vbe::handles(uint16_t port) const {
  return port >= 0x3c6 && port <= 0x3c9;
}

bool Vbe::read8(uint16_t port, uint8_t &value) {
  switch (port) {
  case 0x3c6: value = pixel_mask_; return true;
  case 0x3c7: value = dac_state_; return true;
  case 0x3c8: value = dac_write_index_; return true;
  case 0x3c9:
    value = palette_[dac_read_index_][dac_read_component_] & (dac_width_ == 8 ? 0xff : 0x3f);
    if (++dac_read_component_ == 3) {
      dac_read_component_ = 0;
      ++dac_read_index_;
    }
    return true;
  default: return false;
  }
}

bool Vbe::write8(uint16_t port, uint8_t value) {
  switch (port) {
  case 0x3c6: pixel_mask_ = value; text_invalid_ = true; return true;
  case 0x3c7:
    dac_read_index_ = value;
    dac_read_component_ = 0;
    dac_state_ = 3;
    return true;
  case 0x3c8:
    dac_write_index_ = value;
    dac_write_component_ = 0;
    dac_state_ = 0;
    return true;
  case 0x3c9:
    text_invalid_ = true;
    palette_[dac_write_index_][dac_write_component_] = value & (dac_width_ == 8 ? 0xff : 0x3f);
    rgb_palette_[dac_write_index_][dac_write_component_] = dac_width_ == 8 ? value :
        static_cast<uint8_t>((value & 0x3f) * 255 / 63);
    if (++dac_write_component_ == 3) {
      dac_write_component_ = 0;
      ++dac_write_index_;
    }
    return true;
  default: return false;
  }
}

bool Vbe::modeInfo(uint16_t mode, VbeModeInfo &out) const {
  out = {};
  out.mode = mode;
  out.framebuffer_phys = framebuffer_phys_;

  switch (mode) {
#define LCOM_VBE_MODE_CASE(code, w, h, depth) \
  case code: out.width = w; out.height = h; out.bpp = depth; break;
    LCOM_VBE_GRAPHICS_MODES(LCOM_VBE_MODE_CASE)
#undef LCOM_VBE_MODE_CASE
  default: return false;
  }
  out.bytes_per_pixel = static_cast<uint8_t>((out.bpp + 7) / 8);

  out.pitch = static_cast<uint32_t>(out.width * out.bytes_per_pixel);
  out.framebuffer_size = static_cast<uint64_t>(out.pitch) * out.height;
  return true;
}

bool Vbe::setMode(uint16_t mode) {
  if (mode == 3) {
    resetPalette(true);
    graphics_mode_ = false;
    if (text_mapped_) enterTextMode();
    return true;
  }
  VbeModeInfo info;
  if (!modeInfo(mode, info)) return false;
  current_ = info;
  // Every VBE mode set returns the DAC to the standard VGA six-bit width.
  dac_width_ = 6;
  refreshPalette();
  // VBE mode changes preserve the current DAC, including game-written colors.
  framebuffer_.assign(static_cast<size_t>(info.framebuffer_size), 0);
  graphics_mode_ = true;
  has_frame_ = true;
  display_offset_ = 0;
  display_x_ = display_y_ = 0;
  return true;
}

bool Vbe::setDisplayStart(uint16_t x, uint16_t y) {
  if (!graphics_mode_ || static_cast<uint32_t>(x) + current_.width > scanlinePixels() ||
      static_cast<uint32_t>(y) + current_.height > scanlineLines()) return false;
  display_x_ = x; display_y_ = y;
  display_offset_ = static_cast<size_t>(y) * current_.pitch +
                    static_cast<size_t>(x) * current_.bytes_per_pixel;
  return true;
}

bool Vbe::scanline(uint8_t operation, uint16_t value) {
  if (!graphics_mode_ || operation > 2) return false;
  if (operation == 1) return true;
  uint32_t pitch = operation == 0 ? static_cast<uint32_t>(value) * current_.bytes_per_pixel : value;
  if (pitch > UINT16_MAX || pitch < static_cast<uint32_t>(current_.width) * current_.bytes_per_pixel ||
      static_cast<uint32_t>(display_x_) + current_.width > pitch / current_.bytes_per_pixel ||
      static_cast<uint32_t>(display_y_) + current_.height > vramSize() / pitch) return false;
  if (pitch != current_.pitch) {
    current_.pitch = pitch;
    current_.framebuffer_size = static_cast<uint64_t>(pitch) * current_.height;
    framebuffer_.assign(static_cast<size_t>(current_.framebuffer_size), 0);
    display_offset_ = static_cast<size_t>(display_y_) * pitch +
                      static_cast<size_t>(display_x_) * current_.bytes_per_pixel;
  }
  return true;
}

void Vbe::advanceTick(uint32_t frequency) {
  if (!text_mapped_ || !frequency) return;
  text_elapsed_seconds_ += 1.0 / frequency;
  bool hidden = std::fmod(text_elapsed_seconds_, 32.0 / 70.0) >= 16.0 / 70.0;
  if (hidden != text_blink_hidden_) { text_blink_hidden_ = hidden; text_invalid_ = true; }
}

void Vbe::enterTextMode() {
  text_invalid_ = true;
  current_ = {3, 720, 400, 24, 3, 720 * 3, LCOM_VGA_TEXT_PHYS, 720 * 400 * 3};
  framebuffer_.assign(720 * 400 * 3, 0);
  has_frame_ = true;
}

void Vbe::mapTextMemory() {
  if (text_mapped_) return;
  text_mapped_ = true;
  if (!graphics_mode_) { resetPalette(true); enterTextMode(); }
}

bool Vbe::syncTextFramebuffer(const uint8_t *vram, size_t length) {
  if (!vram || length < mappedVideoBytes()) return false;
  const uint8_t *cells = vram + vramSize();
  bool changed = false;
  // VGA's default text palette maps logical brown/intensity through the
  // attribute-controller EGA table. Character bit 7 is blink, not background.
  static constexpr uint8_t colors[16] = {0,1,2,3,4,5,20,7,56,57,58,59,60,61,62,63};
  for (unsigned row = 0; row < 25; ++row) {
    for (unsigned col = 0; col < 80; ++col) {
      unsigned cell = (row * 80 + col) * 2;
      uint8_t ch = cells[cell], attr = cells[cell + 1];
      if (!text_invalid_ && text_cells_[cell] == ch && text_cells_[cell + 1] == attr) continue;
      text_cells_[cell] = ch; text_cells_[cell + 1] = attr;
      for (unsigned y = 0; y < 16; ++y) {
        uint8_t bits = vga_font_8x16[unsigned(ch) * 16 + y];
        for (unsigned x = 0; x < 9; ++x) {
          bool ink = x < 8 ? (bits & (0x80 >> x)) != 0 :
                     (ch >= 0xc0 && ch <= 0xdf && (bits & 1));
          if ((attr & 0x80) && text_blink_hidden_) ink = false;
          const auto &rgb = rgb_palette_[colors[ink ? attr & 15 : (attr >> 4) & 7] & pixel_mask_];
          auto *pixel = framebuffer_.data() + ((row * 16 + y) * 720 + col * 9 + x) * 3;
          for (unsigned c = 0; c < 3; ++c) {
            uint8_t value = rgb[2 - c];
            if (pixel[c] != value) { pixel[c] = value; changed = true; }
          }
        }
      }
    }
  }
  text_invalid_ = false;
  return changed;
}

bool Vbe::syncFramebuffer(const uint8_t *vram, size_t length) {
  if (textMode()) return syncTextFramebuffer(vram, length);
  if (!graphics_mode_ || !vram) return false;
  size_t row_bytes = static_cast<size_t>(current_.width) * current_.bytes_per_pixel;
  size_t span = static_cast<size_t>(current_.height - 1) * current_.pitch + row_bytes;
  if (display_offset_ > length || span > length - display_offset_) return false;
  bool changed = false;
  for (uint16_t y = 0; y < current_.height; ++y) {
    auto *destination = framebuffer_.data() + static_cast<size_t>(y) * current_.pitch;
    const auto *source = vram + display_offset_ + static_cast<size_t>(y) * current_.pitch;
    if (std::memcmp(destination, source, row_bytes) != 0) {
      std::memcpy(destination, source, row_bytes); changed = true;
    }
  }
  return changed;
}

bool Vbe::ownsRange(uint64_t phys, uint64_t length) const {
  if (length == 0) return false;
  uint64_t start = framebuffer_phys_;
  uint64_t end = start + vramSize();
  return phys >= start && phys <= end && length <= end - phys;
}

void Vbe::pixelRgb(const uint8_t *pixel, uint8_t rgb[3]) const {
  copyRgb(pixel, 1, rgb);
}

void Vbe::copyRgbRow(uint16_t y, uint8_t *rgb) const {
  copyRgb(framebuffer_.data() + static_cast<size_t>(y) * current_.pitch, current_.width, rgb);
}

void Vbe::copyRgb(const uint8_t *pixels, size_t count, uint8_t *rgb) const {
  switch (current_.bpp) {
  case 8:
    for (size_t i = 0; i < count; ++i, rgb += 3) {
      const auto &color = rgb_palette_[pixels[i] & pixel_mask_];
      std::copy(color.begin(), color.end(), rgb);
    }
    break;
  case 15: copyDirectRgb<15>(pixels, count, rgb); break;
  case 16: copyDirectRgb<16>(pixels, count, rgb); break;
  case 24: copyDirectRgb<24>(pixels, count, rgb); break;
  case 32: copyDirectRgb<32>(pixels, count, rgb); break;
  }
}

bool Vbe::dumpPpm(const std::string &path, const std::string &caption,
                  const std::string &caption_position) const {
  if (!has_frame_ || framebuffer_.empty()) return false;
  FILE *f = std::fopen(path.c_str(), "wb");
  if (f == nullptr) return false;
  // A large frame has hundreds of rows. Combine their writes before entering
  // the filesystem; the buffer remains alive until fclose flushes the tail.
  std::array<char, 64 * 1024> file_buffer;
  if (std::setvbuf(f, file_buffer.data(), _IOFBF, file_buffer.size()) != 0) {
    std::fclose(f);
    return false;
  }
  if (std::fprintf(f, "P6\n%u %u\n255\n", current_.width, current_.height) < 0) {
    std::fclose(f);
    return false;
  }
  std::vector<uint8_t> rgb_row(static_cast<size_t>(current_.width) * 3);
  for (uint16_t y = 0; y < current_.height; y++) {
    copyRgbRow(y, rgb_row.data());
    drawCaptionRow(caption, caption_position, current_.width, current_.height, y, rgb_row.data());
    if (std::fwrite(rgb_row.data(), 1, rgb_row.size(), f) != rgb_row.size()) {
      std::fclose(f);
      return false;
    }
  }
  return std::fclose(f) == 0;
}

} // namespace lcom
