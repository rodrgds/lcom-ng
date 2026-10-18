#include "embedded/Host.hpp"
#include <emscripten.h>
#include <cstdint>
extern "C" EMSCRIPTEN_KEEPALIVE double benchmark_present(unsigned mode, unsigned iterations) {
  lcom::Machine machine;
  auto &vbe=machine.vbe();
  if(!vbe.setMode(mode)) return -1;
  const auto &info=vbe.currentMode();
  auto &frame=vbe.framebuffer();
  for(size_t i=0;i<frame.size();++i)frame[i]=static_cast<uint8_t>((i*73+i/17)%256);
  // Include a changed DAC component and mask in the indexed conversion.
  if(info.bpp==8){vbe.write8(0x3c8,42);vbe.write8(0x3c9,63);vbe.write8(0x3c9,17);vbe.write8(0x3c9,5);vbe.write8(0x3c6,0x7f);}
  const double start=emscripten_get_now();
  for(unsigned i=0;i<iterations;++i)lcom::embedded::host().present(machine);
  return (emscripten_get_now()-start)/iterations;
}
