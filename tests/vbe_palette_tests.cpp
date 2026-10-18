#include "../runtime/devices/Vbe.hpp"
#include <cstdio>
#include <cstring>
#include <vector>

#define CHECK(c) do { if (!(c)) { std::fprintf(stderr, "VBE palette check failed at line %d: %s\n", __LINE__, #c); return 1; } } while (0)

static int rgbaRows() {
  const struct { uint16_t mode; uint8_t raw[8], rgb[6]; } cases[] = {
    {0x200, {7,38,143,0,215,63,12,127}, {143,38,7,12,63,215}},
    {0x14c, {7,38,143,255,215,63,12,0}, {143,38,7,12,63,215}},
    {0x115, {7,38,143,215,63,12}, {143,38,7,12,63,215}},
    {0x118, {7,38,143,215,63,12}, {143,38,7,12,63,215}},
    {0x110, {0x84,0x10,0xe0,0x7f}, {32,32,32,255,255,0}},
    {0x111, {0x84,0x20,0xe0,0xff}, {32,16,32,255,255,0}},
    {0x117, {0x84,0x20,0xe0,0xff}, {32,16,32,255,255,0}},
    {0x11a, {0x84,0x20,0xe0,0xff}, {32,16,32,255,255,0}},
    {0x105, {56,72}, {125,125,255,125,255,125}},
  };
  for (const auto &test : cases) {
    lcom::Vbe vbe;
    CHECK(vbe.setMode(test.mode));
    // Odd pitches exercise row alignment and hidden padding in every depth.
    CHECK(vbe.scanline(2,vbe.currentMode().pitch+7));
    const auto &mode=vbe.currentMode();auto &frame=vbe.framebuffer();
    std::memset(frame.data(),0xa7,frame.size());
    for(unsigned y : {0u,1u,static_cast<unsigned>(mode.height-1)}) {
      for(unsigned x=0;x<mode.width;++x) {
        const unsigned color=(x/32+y)%2;
        std::memcpy(frame.data()+y*mode.pitch+x*mode.bytes_per_pixel,test.raw+color*mode.bytes_per_pixel,mode.bytes_per_pixel);
      }
      std::vector<uint8_t> rgba(mode.width*4+8,0xee);
      vbe.copyRgbaRow(y,rgba.data()+4);
      for(unsigned x=0;x<mode.width;++x) {
        const auto *pixel=rgba.data()+4+x*4;const unsigned color=(x/32+y)%2;
        CHECK(std::memcmp(pixel,test.rgb+color*3,3)==0 && pixel[3]==255);
      }
      for(unsigned i=0;i<4;++i)CHECK(rgba[i]==0xee && rgba[rgba.size()-4+i]==0xee);
    }
    if(mode.bpp==8) {
      uint8_t pixel[4];
      const uint8_t colors[][3]={{255,125,255},{255,0,255},{255,0,4}};
      CHECK(vbe.write8(0x3c8,56));
      for(unsigned component=0;component<3;++component) {
        CHECK(vbe.write8(0x3c9,component==0?63:component==1?0:1));
        std::vector<uint8_t> row(mode.width*4);vbe.copyRgbaRow(0,row.data());
        CHECK(std::memcmp(row.data(),colors[component],3)==0 && row[3]==255);
      }
      CHECK(vbe.write8(0x3c6,0));
      std::vector<uint8_t> row(mode.width*4);vbe.copyRgbaRow(0,row.data());
      std::memcpy(pixel,row.data(),4);CHECK(pixel[0]==0 && pixel[1]==0 && pixel[2]==0 && pixel[3]==255);
    }
  }
  return 0;
}

static int allDirectColors() {
  for(uint16_t mode : {uint16_t(0x110),uint16_t(0x111)}) {
    lcom::Vbe vbe;CHECK(vbe.setMode(mode));
    const unsigned width=vbe.currentMode().width;
    std::vector<uint8_t> rgba(width*4),rgb(width*3);
    auto &frame=vbe.framebuffer();
    for(unsigned first=0;first<65536;first+=width) {
      for(unsigned x=0;x<width;++x) {
        const unsigned value=(first+x)&0xffff;
        frame[x*2]=value&255;frame[x*2+1]=value>>8;
      }
      vbe.copyRgbaRow(0,rgba.data());vbe.copyRgbRow(0,rgb.data());
      for(unsigned x=0;x<width && first+x<65536;++x) {
        const unsigned value=first+x;
        const uint8_t expected[]={
          static_cast<uint8_t>(((value>>(mode==0x111?11:10))&31)*255/31),
          static_cast<uint8_t>(((value>>5)&(mode==0x111?63:31))*255/(mode==0x111?63:31)),
          static_cast<uint8_t>((value&31)*255/31)
        };
        CHECK(std::memcmp(rgba.data()+x*4,expected,3)==0 && rgba[x*4+3]==255);
        CHECK(std::memcmp(rgb.data()+x*3,expected,3)==0);
      }
    }
  }
  return 0;
}

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
  CHECK(rgbaRows()==0);
  CHECK(allDirectColors()==0);
  std::puts("Modern VGA256, BIOS text palette and VBE DAC preservation passed");
  return 0;
}
