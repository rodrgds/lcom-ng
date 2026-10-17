// Browser effects for the shared embedded SDK. No device behavior belongs here.
#include "embedded/Host.hpp"
#include <emscripten.h>
#include <algorithm>
#include <vector>

extern "C" {
void lcom_browser_start();
void lcom_browser_present(const uint8_t *, int, int);
void lcom_browser_audio(const int16_t *, int, int, int);
void lcom_browser_audio_stop();
int lcom_browser_paused();
}

namespace lcom::embedded {
namespace {
class BrowserHost final : public Host {
  std::vector<uint8_t> rgba_, row_;
  std::string clipboard_;
public:
  bool start() override { clipboard_.clear(); lcom_browser_start(); return true; }
  void stop() override {}
  double nowMs() override { return emscripten_get_now(); }
  void sleepMs(unsigned ms) override { emscripten_sleep(ms); }
  void pump() override {}
  bool paused() override { return lcom_browser_paused(); }
  void audio(const int16_t *samples, size_t frames, uint32_t rate, uint8_t channels) override {
    lcom_browser_audio(samples, frames, rate, channels);
  }
  void stopAudio() override { lcom_browser_audio_stop(); }
  bool setClipboard(const std::string &text) override { clipboard_ = text; return true; }
  bool getClipboard(std::string &text) override { text = clipboard_; return true; }
  void present(Machine &machine) override {
    auto &vbe = machine.vbe();
    const auto &mode = vbe.currentMode();
    rgba_.resize(static_cast<size_t>(mode.width) * mode.height * 4);
    row_.resize(static_cast<size_t>(mode.width) * 3);
    for (uint16_t y = 0; y < mode.height; ++y) {
      vbe.copyRgbRow(y, row_.data());
      for (uint16_t x = 0; x < mode.width; ++x) {
        auto pixel = rgba_.data() + (static_cast<size_t>(y) * mode.width + x) * 4;
        std::copy_n(row_.data() + x * 3, 3, pixel);
        pixel[3] = 255;
      }
    }
    lcom_browser_present(rgba_.data(), mode.width, mode.height);
  }
};
}
Host &host() { static BrowserHost platform; return platform; }
}

extern "C" {
EMSCRIPTEN_KEEPALIVE void lcom_web_key(const char *key, int down) {
  lcom::embedded::injectKey(key, down != 0);
}
EMSCRIPTEN_KEEPALIVE void lcom_web_mouse(int dx, int dy, int buttons) {
  // The JS player supplies PS/2 Y; the shared host interface uses screen Y.
  lcom::embedded::injectMouse(dx, -dy, static_cast<uint8_t>(buttons));
}
EMSCRIPTEN_KEEPALIVE void lcom_web_wheel(int delta) {
  lcom::embedded::injectWheel(delta);
}
EMSCRIPTEN_KEEPALIVE void lcom_web_pointer(int x, int y, int buttons) {
  lcom::embedded::injectPointer(x,y,static_cast<uint8_t>(buttons));
}
EMSCRIPTEN_KEEPALIVE void lcom_web_cursor(int x, int y, int max_x, int max_y, int min_x, int min_y, int reject, double scale, int fractional) {
  lcom::embedded::configureCursor({x,y,max_x,max_y,min_x,min_y,reject!=0,scale==0.5 || scale==2 ? scale : 1,fractional!=0});
}
}
