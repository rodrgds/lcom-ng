#include "embedded/Host.hpp"
#include "embedded/Pointer.hpp"
#include <lcom/lcom.h>
#include <lcom/i8254.h>
#include <lcom/i8042.h>
#include <lcom/vbe.h>
#include <lcom/ac97.h>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <array>

#define CHECK(value) do { if (!(value)) { std::fprintf(stderr, "line %d: %s\n", __LINE__, #value); std::exit(1); } } while (0)

namespace {
class Host final : public lcom::embedded::Host {
public:
  double clock = 0;
  std::vector<uint8_t> frame;
  std::vector<int16_t> sound;
  bool stopped = false;
  unsigned presentations = 0;
  std::array<uint8_t,3> first_rgb{};
  std::string clipboard;
  bool start() override { clipboard.clear(); stopped = false; return true; }
  void stop() override { stopped = true; }
  double nowMs() override { return clock; }
  void sleepMs(unsigned ms) override { clock += ms; }
  void pump() override {}
  bool paused() override { return false; }
  void present(lcom::Machine &machine) override {
    ++presentations; frame = machine.vbe().framebuffer();
    std::vector<uint8_t> row(machine.vbe().currentMode().width*3);
    machine.vbe().copyRgbRow(0,row.data());
    std::copy_n(row.begin(),3,first_rgb.begin());
  }
  void audio(const int16_t *samples, size_t frames, uint32_t rate, uint8_t channels) override {
    CHECK(rate == 48000); CHECK(channels == 2);
    sound.assign(samples, samples + frames * channels);
  }
  void stopAudio() override {}
  bool setClipboard(const std::string &text) override { clipboard = text; return true; }
  bool getClipboard(std::string &text) override { text = clipboard; return true; }
} platform;
}
namespace lcom::embedded { Host &host() { return platform; } }

static std::vector<std::array<int,3>> drainPointer(int &x, int &y) {
  std::vector<std::array<int,3>> positions;
  for (int i=0;i<100;++i) {
    uint8_t flags, dx, dy;
    CHECK(lcom_port_read8(KBC_ST_REG,&flags)==LCOM_OK);
    if (!(flags & KBC_ST_OBF)) return positions;
    CHECK(flags & KBC_ST_AUX);
    lcom_event_t event;
    CHECK(lcom_event_wait(&event)==LCOM_OK);
    CHECK(lcom_port_read8(KBC_OUT_BUF,&flags)==LCOM_OK);
    CHECK(lcom_port_read8(KBC_OUT_BUF,&dx)==LCOM_OK);
    CHECK(lcom_port_read8(KBC_OUT_BUF,&dy)==LCOM_OK);
    CHECK(!(flags & 0xc0));
    x=std::clamp(x + int(dx) - ((flags&16)?256:0),0,799);
    y=std::clamp(y - int(dy) + ((flags&32)?256:0),0,599);
    positions.push_back({x,y,flags&7});
  }
  CHECK(false); return positions;
}

static void directTouchClearsClickGate() {
  for(bool hover_first : {false,true}) {
  lcom::embedded::Pointer pointer;
  pointer.reset(800,600);
  // Some legacy menus discard input until a released mouse sample arrives.
  // A short first tap still has to reach its target before its press is seen.
  if(hover_first) pointer.absolute(400,245,0);
  pointer.absolute(400,245,1);
  pointer.relative(0,0,0);
  pointer.absolute(420,250,1);
  pointer.relative(0,0,0);
  bool awaiting_release=true;
  int x=400,y=300;
  std::vector<std::array<int,2>> presses;
  while (auto packet=pointer.next()) {
    if (awaiting_release) {
      if (!packet->buttons) awaiting_release=false;
      continue;
    }
    x=std::clamp(x+packet->dx,0,799);
    y=std::clamp(y-packet->dy,0,599);
    if (packet->buttons&1) presses.push_back({x,y});
  }
  CHECK((presses==std::vector<std::array<int,2>>{{400,245},{420,250}}));
  }
}

static void interruptOnlyMouseDoesNotBlockKeyboard() {
  // Some frozen games unsubscribe before disabling their mouse stream. A
  // touch release in that gap must not strand AUX bytes in their keyboard-only loop.
  setenv("LCOM_COMPAT_MOUSE_IRQ_ONLY","1",1);
  CHECK(lcom_init()==LCOM_OK);
  lcom_irq_t mouse,keyboard;
  CHECK(lcom_irq_subscribe(MOUSE_IRQ,0,&mouse)==LCOM_OK);
  CHECK(lcom_irq_subscribe(KBC_IRQ,0,&keyboard)==LCOM_OK);
  CHECK(lcom_port_write8(KBC_CMD_REG,KBC_CMD_WRITE_MOUSE)==LCOM_OK);
  CHECK(lcom_port_write8(KBC_IN_BUF,0xf4)==LCOM_OK);
  uint8_t value=0; CHECK(lcom_port_read8(KBC_OUT_BUF,&value)==LCOM_OK && value==0xfa);
  lcom::embedded::injectMouse(0,0,1);
  for(int i=0;i<3;++i) CHECK(lcom_port_read8(KBC_OUT_BUF,&value)==LCOM_OK);
  CHECK(lcom_irq_unsubscribe(&mouse)==LCOM_OK);
  lcom::embedded::injectMouse(0,0,0);
  CHECK(lcom_port_write8(KBC_CMD_REG,KBC_CMD_WRITE_MOUSE)==LCOM_OK);
  CHECK(lcom_port_write8(KBC_IN_BUF,0xf5)==LCOM_OK);
  CHECK(lcom_port_read8(KBC_OUT_BUF,&value)==LCOM_OK && value==0xfa);
  lcom::embedded::injectKey("W",true);
  lcom_event_t event;CHECK(lcom_event_wait(&event)==LCOM_OK && (event.irq_mask&keyboard.mask));
  CHECK(lcom_port_read8(KBC_OUT_BUF,&value)==LCOM_OK && value==0x11);
  lcom_exit();unsetenv("LCOM_COMPAT_MOUSE_IRQ_ONLY");
}

int main() {
  directTouchClearsClickGate();
  interruptOnlyMouseDoesNotBlockKeyboard();
  // Knight and chess reject an invalid delta independently on each axis.
  // Relative travel to an edge must not desynchronize the following direct tap.
  {
    lcom::embedded::Pointer pointer;
    pointer.configure({400,300,799,599,1,1,true}); pointer.reset(800,600);
    int x=400,y=300;
    auto drain=[&] {
      while(auto packet=pointer.next()) {
        if(x+packet->dx>0 && x+packet->dx<800) x+=packet->dx;
        if(y-packet->dy>0 && y-packet->dy<600) y-=packet->dy;
        if(packet->buttons) CHECK(x==200 && y==150);
      }
    };
    for(int i=0;i<5;++i) { pointer.relative(96,96,0);drain(); }
    CHECK(x==784 && y==588);
    pointer.absolute(200,150,1); pointer.relative(0,0,0);drain();
    CHECK(x==200 && y==150);
  }
  CHECK(lcom_init() == LCOM_OK);
  CHECK(lcom_init() == LCOM_OK);
  lcom_vbe_mode_info_t mode;
  CHECK(lcom_vbe_get_mode_info(LCOM_VBE_MODE_800_600_24, &mode) == LCOM_OK);
  CHECK(lcom_vbe_set_mode(mode.mode) == LCOM_OK);
  uint8_t *pixels = nullptr;
  CHECK(lcom_phys_map(mode.framebuffer_phys, mode.framebuffer_size, reinterpret_cast<void **>(&pixels)) == LCOM_OK);
  pixels[0] = 51; pixels[1] = 34; pixels[2] = 17;
  CHECK(lcom_vbe_present() == LCOM_OK);
  CHECK(platform.frame.size() == 800u * 600u * 3u);
  CHECK(platform.frame[0] == 51 && platform.frame[1] == 34 && platform.frame[2] == 17);
  const auto presented = platform.presentations;
  // Legacy driver_receive presents between every input byte, even when the
  // guest hasn't drawn. Repeated frames must not stall input behind uploads.
  for (int i = 0; i < 300; ++i) CHECK(lcom_vbe_present() == LCOM_OK);
  CHECK(platform.presentations == presented);
  pixels[0] = 52;
  CHECK(lcom_vbe_present() == LCOM_OK);
  CHECK(platform.presentations == presented + 1 && platform.frame[0] == 52);
  void *invalid = nullptr;
  CHECK(lcom_phys_map(0xffffffffu, 16, &invalid) == LCOM_ERR);
  CHECK(lcom_phys_unmap(pixels, mode.framebuffer_size - 1) == LCOM_ERR);
  CHECK(lcom_phys_unmap(pixels, mode.framebuffer_size) == LCOM_OK);
  lcom_irq_t keyboard, timer;
  CHECK(lcom_irq_subscribe(KBC_IRQ, 0, &keyboard) == LCOM_OK);
  CHECK(lcom_irq_subscribe(TIMER0_IRQ, 0, &timer) == LCOM_OK);
  lcom::embedded::injectKey("A", true);
  lcom_event_t event;
  CHECK(lcom_event_wait(&event) == LCOM_OK);
  CHECK((event.irq_mask & keyboard.mask) != 0);
  uint8_t byte = 0;
  CHECK(lcom_port_read8(KBC_OUT_BUF, &byte) == LCOM_OK && byte == 0x1e);
  lcom::embedded::injectKey("A", false);
  CHECK(lcom_port_read8(KBC_OUT_BUF, &byte) == LCOM_OK && byte == 0x9e);
  const auto before = event.tick;
  CHECK(lcom_delay_ticks(3, &event) == LCOM_OK);
  CHECK(event.tick >= before + 3);
  CHECK((event.irq_mask & timer.mask) != 0);
  lcom_ac97_buffer_info_t audio;
  CHECK(lcom_ac97_get_buffer(&audio) == LCOM_OK);
  int16_t *samples = nullptr;
  CHECK(lcom_phys_map(audio.pcm_phys, audio.pcm_bytes, reinterpret_cast<void **>(&samples)) == LCOM_OK);
  samples[0] = 4096; samples[1] = -4096;
  CHECK(lcom_port_write16(AC97_BM_BASE + AC97_PO_PICB, 4) == LCOM_OK);
  CHECK(lcom_port_write8(AC97_BM_BASE + AC97_PO_CR, AC97_PO_CR_RUN) == LCOM_OK);
  CHECK(platform.sound.size() == 2 && platform.sound[0] == 4096 && platform.sound[1] == -4096);
  CHECK(lcom_clipboard_set_text("native") == LCOM_OK);
  char text[16] = "unchanged";
  CHECK(lcom_clipboard_get_text(text, 2) == LCOM_ERR && text[0] == 'u');
  CHECK(lcom_clipboard_get_text(text, sizeof(text)) == LCOM_OK && std::string(text) == "native");
  lcom_irq_t mouse;
  CHECK(lcom_irq_subscribe(MOUSE_IRQ,0,&mouse)==LCOM_OK);
  CHECK(lcom_port_write8(KBC_CMD_REG,KBC_CMD_WRITE_MOUSE)==LCOM_OK);
  CHECK(lcom_port_write8(KBC_IN_BUF,0xf4)==LCOM_OK);
  CHECK(lcom_port_read8(KBC_OUT_BUF,&byte)==LCOM_OK && byte==0xfa);
  int x=400,y=300;
  // A fast tap and release must arrive at the tap, despite needing several
  // relative packets. The intermediate travel must not press at old positions.
  lcom::embedded::injectPointer(25,80,1);
  lcom::embedded::injectMouse(0,0,0);
  auto positions=drainPointer(x,y);
  CHECK(x==25 && y==80);
  unsigned presses=0;
  for (const auto &p:positions) if (p[2]==1) { CHECK(p[0]==25 && p[1]==80); ++presses; }
  CHECK(presses==1 && positions.back()[2]==0);
  // Physical motion at a clamp updates the same cursor model. A subsequent
  // direct tap must not drift because raw deltas overshot the guest's edge.
  for (int i=0;i<12;++i) { lcom::embedded::injectMouse(-90,-90,0); drainPointer(x,y); }
  CHECK(x==0 && y==0);
  lcom::embedded::injectPointer(710,540,1);
  lcom::embedded::injectMouse(0,0,0);
  positions=drainPointer(x,y);
  CHECK(x==710 && y==540 && positions.back()[2]==0);
  // Motion can arrive much faster than a byte-at-a-time guest. Keep the newest
  // target without placing hundreds of obsolete packets in the device FIFO.
  for (int i=0;i<1000;++i) lcom::embedded::injectPointer(100+i%500,200,0);
  positions=drainPointer(x,y);
  CHECK(x==599 && y==200 && positions.size()<10);
  // Palette-only animation changes visible pixels without changing VRAM.
  CHECK(lcom_vbe_set_mode(LCOM_VBE_MODE_800_600_8)==LCOM_OK);
  CHECK(lcom_vbe_present()==LCOM_OK);
  CHECK((platform.first_rgb==std::array<uint8_t,3>{0,0,0}));
  CHECK(lcom_port_write8(0x3c8,0)==LCOM_OK);
  for (uint8_t channel : {63,0,0}) CHECK(lcom_port_write8(0x3c9,channel)==LCOM_OK);
  CHECK(lcom_vbe_present()==LCOM_OK);
  CHECK((platform.first_rgb==std::array<uint8_t,3>{255,0,0}));
  lcom_exit();
  CHECK(platform.stopped);
  CHECK(lcom_port_read8(KBC_OUT_BUF, &byte) == LCOM_ERR);
  CHECK(lcom_init() == LCOM_OK);
  CHECK(lcom_clipboard_get_text(text, sizeof(text)) == LCOM_OK && text[0] == 0);
  lcom_exit();
  puts("embedded SDK checks passed");
}
