#include "../runtime/devices/Vbe.hpp"
#include <cstdio>
#include <cstring>

#define CHECK(c) do { if (!(c)) { std::fprintf(stderr, "VBE palette check failed at line %d: %s\n", __LINE__, #c); return 1; } } while (0)

int main() {
  lcom::Vbe modern;
  CHECK(modern.setMode(0x105));
  uint8_t pixel = 32, rgb[3];
  modern.pixelRgb(&pixel, rgb);
  const uint8_t modern_blue[] = {0, 0, 255};
  CHECK(std::memcmp(rgb, modern_blue, 3) == 0);

  // VirtualBox BIOS palette2 has these fixed six-bit EGA DAC entries. Its
  // VBE set-mode path preserves them after text mode03, unlike mode13 VGA256.
  lcom::Vbe minix;
  CHECK(minix.setMode(3));
  CHECK(minix.setMode(0x107));
  const struct { uint8_t index, rgb[3]; } cases[] = {
    {20, {170, 85, 0}}, {38, {255, 170, 0}},
    {56, {85, 85, 85}}, {62, {255, 255, 85}},
    {63, {255, 255, 255}}, {64, {0, 0, 0}}, {255, {0, 0, 0}}
  };
  for (const auto &test : cases) {
    minix.pixelRgb(&test.index, rgb);
    CHECK(std::memcmp(rgb, test.rgb, 3) == 0);
  }
  // A game-programmed DAC entry survives a VBE resolution/depth transition.
  CHECK(minix.write8(0x3c8, 64));
  for (uint8_t c : {uint8_t(63), uint8_t(21), uint8_t(0)}) CHECK(minix.write8(0x3c9, c));
  CHECK(minix.setMode(0x115));
  CHECK(minix.setMode(0x105));
  pixel = 64; minix.pixelRgb(&pixel, rgb);
  const uint8_t orange[] = {255, 85, 0};
  CHECK(std::memcmp(rgb, orange, 3) == 0);
  CHECK(minix.setMode(3));
  CHECK(minix.setMode(0x105));
  minix.pixelRgb(&pixel, rgb);
  const uint8_t black[] = {0, 0, 0};
  CHECK(std::memcmp(rgb, black, 3) == 0);
  // VBE DAC widths are machine-owned, including ordinary VGA port access.
  uint8_t actual = 0xcc;
  CHECK(modern.dacFormat(0, 8, actual) && actual == 8);
  CHECK(minix.dacFormat(1, 0, actual) && actual == 6);
  CHECK(modern.write8(0x3c8, 254));
  for (uint8_t c : {uint8_t(227), uint8_t(66), uint8_t(129)}) CHECK(modern.write8(0x3c9, c));
  pixel = 254; modern.pixelRgb(&pixel, rgb);
  const uint8_t eight_bit[] = {227, 66, 129};
  CHECK(std::memcmp(rgb, eight_bit, 3) == 0);
  CHECK(modern.write8(0x3c7, 254));
  for (uint8_t expected : eight_bit) { uint8_t value; CHECK(modern.read8(0x3c9, value) && value == expected); }
  CHECK(modern.dacFormat(0, 7, actual) && actual == 6);
  CHECK(modern.dacFormat(0, 9, actual) && actual == 8);
  actual = 0xcc;
  CHECK(!modern.dacFormat(0, 5, actual) && actual == 0xcc);
  CHECK(!modern.dacFormat(2, 8, actual) && actual == 0xcc);
  CHECK(modern.dacFormat(1, 0, actual) && actual == 8);
  CHECK(!modern.setMode(0xffff));
  CHECK(modern.dacFormat(1, 0, actual) && actual == 8);
  CHECK(modern.setMode(0x105));
  CHECK(modern.dacFormat(1, 0, actual) && actual == 6);
  CHECK(modern.write8(0x3c7, 254));
  CHECK(modern.read8(0x3c9, actual) && actual == (227 & 63));
  CHECK(modern.setMode(0x115));
  actual = 0xcc;
  CHECK(!modern.dacFormat(1, 0, actual) && actual == 0xcc);
  std::puts("Modern VGA256, BIOS text palette and VBE DAC preservation passed");
  return 0;
}
