#include "core/Machine.hpp"
#include <lcom/vbe.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <vector>
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #x); std::exit(1); } } while (0)
static void color(lcom::Vbe &vbe, unsigned x, unsigned y, unsigned r, unsigned g, unsigned b) {
  std::vector<uint8_t> row(vbe.currentMode().width * 3);
  vbe.copyRgbRow(y, row.data());
  CHECK(row[x*3] == r && row[x*3+1] == g && row[x*3+2] == b);
}
int main() {
  lcom::Machine machine;
  CHECK(!machine.vbe().displayMode());
  CHECK(machine.physicalRange(0xB8000, 4000));
  CHECK(!machine.physicalRange(0xB8000, 0));
  CHECK(!machine.physicalRange(0xB7fff, 2));
  CHECK(!machine.physicalRange(0xBffff, 2));
  CHECK(machine.mapPhysicalRange(0xB8000, 4000));
  auto &vbe = machine.vbe();
  CHECK(vbe.textMode() && !vbe.graphicsMode());
  CHECK(vbe.currentMode().mode == 3 && vbe.currentMode().width == 720 && vbe.currentMode().height == 400);
  std::vector<uint8_t> memory(lcom::Vbe::mappedVideoBytes());
  auto *text = memory.data() + lcom::Vbe::vramSize();
  for (unsigned i = 0; i < 2000; ++i) { text[i*2] = ' '; text[i*2+1] = 0x17; }
  text[0] = 0xDB; text[1] = 0x1e; // CP437 solid block, bright yellow on blue.
  text[3998] = ' '; text[3999] = 0x42; // bottom-right cell has red background.
  CHECK(vbe.syncFramebuffer(memory.data(), memory.size()));
  color(vbe, 0, 0, 255, 255, 85);
  color(vbe, 8, 15, 255, 255, 85); // VGA duplicates box/block ninth column.
  color(vbe, 9, 0, 0, 0, 170);
  color(vbe, 719, 399, 170, 0, 0);
  CHECK(!vbe.syncFramebuffer(memory.data(), memory.size()));
  text[1] = 0x9e; // blink flag retains blue background.
  CHECK(!vbe.syncFramebuffer(memory.data(), memory.size()));
  for (unsigned i = 0; i < 15; ++i) machine.advanceTick();
  CHECK(vbe.syncFramebuffer(memory.data(), memory.size()));
  color(vbe, 0, 0, 0, 0, 170);
  CHECK(vbe.setMode(0x115));
  memory[0] = 9; memory[1] = 8; memory[2] = 7;
  CHECK(vbe.syncFramebuffer(memory.data(), memory.size()));
  color(vbe, 0, 0, 7, 8, 9);
  CHECK(text[0] == 0xDB && text[1] == 0x9e);
  CHECK(vbe.setMode(3));
  CHECK(vbe.textMode() && !vbe.graphicsMode());
  CHECK(vbe.syncFramebuffer(memory.data(), memory.size()));
  color(vbe, 0, 0, 0, 0, 170);
  for (unsigned i = 0; i < 15; ++i) machine.advanceTick();
  CHECK(vbe.syncFramebuffer(memory.data(), memory.size()));
  color(vbe, 0, 0, 255, 255, 85);
  // VGA DAC writes affect text and invalidate unchanged character cells.
  CHECK(vbe.write8(0x3c8, 62));
  CHECK(vbe.write8(0x3c9, 0)); CHECK(vbe.write8(0x3c9, 63)); CHECK(vbe.write8(0x3c9, 0));
  CHECK(vbe.syncFramebuffer(memory.data(), memory.size()));
  color(vbe, 0, 0, 0, 255, 0);
  puts("VGA text mapping, attributes, CP437 edges, blink, DAC, and mode transitions pass");
}
