#include "../runtime/core/Machine.hpp"
#include "../runtime/core/MachineConfig.hpp"
#include <cstdio>
#include <cstdlib>
#define CHECK(expr) do { if (!(expr)) { std::fprintf(stderr,"Failed: %s at %d\n",#expr,__LINE__); std::abort(); } } while (false)
#include <stdexcept>
#include <string>

int main() {
  lcom::MachineConfig config;
  std::string error;
  CHECK(lcom::parseMachineConfig(nullptr,config,error));
  CHECK(config.vbe_physical_base==0xe0000000u);
  CHECK(lcom::parseMachineConfig("0xF0000000",config,error));
  lcom::Machine relocated(config), original;
  lcom::VbeModeInfo info;
  CHECK(relocated.vbe().modeInfo(0x117,info));
  CHECK(info.framebuffer_phys==0xf0000000u);
  CHECK(relocated.physicalRange(0xf0000000u,1));
  auto offset=relocated.physicalRange(0xf0001000u,1);
  CHECK(offset && offset->device==lcom::Machine::PhysicalRange::Device::Video && offset->offset==4096);
  CHECK(!relocated.physicalRange(0xe0000000u,1));
  CHECK(!relocated.physicalRange(0xf0ffffffu,2));
  CHECK(relocated.physicalRange(0xd0000000u,1)->device==lcom::Machine::PhysicalRange::Device::Audio);
  CHECK(original.vbe().modeInfo(0x117,info) && info.framebuffer_phys==0xe0000000u);
  CHECK(original.physicalRange(0xe0000000u,1) && !original.physicalRange(0xf0000000u,1));
  for(const char *bad:{"","-1","+1"," 0xf0000000","0xf0000000tail","0x100000000","0xf0000001","0xff001000","0","0x80000","0x90000","0xB8000","0xd0000000","0xcffff000"}) {
    config.vbe_physical_base=0xe0000000u;
    CHECK(!lcom::parseMachineConfig(bad,config,error) && !error.empty());
    CHECK(config.vbe_physical_base==0xe0000000u);
  }
  for(const char *good:{"4026531840","0xC0000","0xd0200000","0xff000000"}) {
    CHECK(lcom::parseMachineConfig(good,config,error));
    lcom::Machine machine(config);
    CHECK(machine.vbe().modeInfo(0x105,info) && info.framebuffer_phys==config.vbe_physical_base);
  }
  CHECK(lcom::parseMachineConfig("0xD0000000","0xC0000000",config,error));
  lcom::Machine dual(config);
  CHECK(dual.vbe().modeInfo(0x105,info) && info.framebuffer_phys==0xd0000000u);
  CHECK(dual.ac97().bufferPhys()==0xc0000000u);
  auto video=dual.physicalRange(0xd0001000u,4096);
  auto audio=dual.physicalRange(0xc0001000u,4096);
  CHECK(video && video->device==lcom::Machine::PhysicalRange::Device::Video && video->offset==4096);
  CHECK(audio && audio->device==lcom::Machine::PhysicalRange::Device::Audio && audio->offset==4096);
  CHECK(!dual.physicalRange(0xe0000000u,1));
  for(const char *bad:{"", "-1", "0xc0000001", "0x80000", "0x90000", "0xB8000", "0xffe01000", "0xd0000000", "0xcffff000", "0xd0fff000"}) {
    config={0xd0000000u,0xc0000000u};
    CHECK(!lcom::parseMachineConfig("0xD0000000",bad,config,error));
    CHECK(config.vbe_physical_base==0xd0000000u && config.ac97_pcm_base==0xc0000000u);
  }
  for(const char *adjacent:{"0xcfe00000","0xd1000000","0xffe00000"}) {
    CHECK(lcom::parseMachineConfig("0xD0000000",adjacent,config,error));
    lcom::Machine machine(config);
    CHECK(machine.ac97().bufferPhys()==config.ac97_pcm_base);
  }
  config={0xd0000000u,0xd0000000u};
  bool overlapRejected=false;
  try {lcom::Machine invalid(config);} catch(const std::invalid_argument&){overlapRejected=true;}
  CHECK(overlapRejected);
  config.vbe_physical_base=0;
  bool rejected=false;
  try {lcom::Machine invalid(config);} catch(const std::invalid_argument&){rejected=true;}
  CHECK(rejected);
}
