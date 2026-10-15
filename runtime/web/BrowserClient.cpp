// In-process implementation of the public SDK. Device behavior belongs to Machine.
#include "core/Machine.hpp"
#include "core/MousePacketScheduler.hpp"
#include <lcom/lcom.h>
#include <lcom/ac97.h>
#include <lcom/vbe.h>
#include <emscripten.h>

#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <memory>
#include <vector>

extern "C" {
void lcom_browser_start();
void lcom_browser_present(const uint8_t *pixels, int width, int height);
void lcom_browser_audio(const int16_t *samples, int frames, int rate, int channels);
void lcom_browser_audio_stop();
int lcom_browser_paused();
}

namespace {
struct Mapping { void *pointer; size_t length; };
struct Session {
  lcom::Machine machine;
  // Fixed backing allocations retain mapped C pointers across mode changes.
  std::vector<uint8_t> video = std::vector<uint8_t>(lcom::Vbe::vramSize());
  std::vector<uint8_t> audio = std::vector<uint8_t>(machine.ac97().bufferSize());
  std::vector<uint8_t> rgba;
  std::vector<Mapping> mappings;
  std::string clipboard;
  lcom::MousePacketScheduler mouse;
  double next_tick = emscripten_get_now();
  double last_yield = next_tick;
};
std::unique_ptr<Session> session;

void service(bool wait = false) {
  if (!session) return;
  auto &s = *session;
  double now = emscripten_get_now();
  if (wait || now - s.last_yield >= 8.0 || lcom_browser_paused()) {
    emscripten_sleep(wait ? 2 : 0);
    while (lcom_browser_paused()) {
      emscripten_sleep(50);
      s.next_tick = emscripten_get_now();
    }
    now = emscripten_get_now();
    s.last_yield = now;
  }
  // Timer frequency is guest-controlled, independent of display refresh.
  int advanced = 0;
  while (now >= s.next_tick && advanced < 4) {
    s.machine.advanceTick();
    s.next_tick += 1000.0 / std::max(1u, s.machine.pit().channelFrequency(0));
    ++advanced;
  }
  if (advanced == 4 && now >= s.next_tick)
    s.next_tick = now + 1000.0 / std::max(1u, s.machine.pit().channelFrequency(0));
  if (auto packet = s.mouse.nextPacket(false))
    s.machine.injectMouse(packet->dx, packet->dy, packet->buttons);
}

int event(lcom_event_t *out, bool delay, uint64_t ticks) {
  if (!session || !out) return LCOM_ERR;
  auto &machine = session->machine;
  uint64_t target = 0;
  if (delay && !machine.delayTargetTick(ticks, target)) return LCOM_ERR;
  for (;;) {
    service();
    machine.refreshDeviceIrqs();
    if (delay ? machine.tick() >= target : machine.pendingIrqs() != 0) break;
    service(true);
  }
  out->irq_mask = machine.consumePendingIrqs();
  out->tick = machine.tick();
  return LCOM_OK;
}
}

extern "C" {
int lcom_init(void) {
  if (!session) {
    session = std::make_unique<Session>();
    lcom_browser_start();
  }
  return LCOM_OK;
}

void lcom_exit(void) {
  lcom_browser_audio_stop();
  session.reset();
}

int lcom_printf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  int result = vprintf(format, args);
  va_end(args);
  return result < 0 ? LCOM_ERR : LCOM_OK;
}

int lcom_port_read8(uint16_t port, uint8_t *value) {
  if (!session || !value) return LCOM_ERR;
  service();
  return session->machine.readPort8(port, *value) ? LCOM_OK : LCOM_ERR;
}

int lcom_port_write8(uint16_t port, uint8_t value) {
  if (!session) return LCOM_ERR;
  service();
  auto &s = *session;
  auto &device = s.machine.ac97();
  bool was_playing = device.playing();
  if (!s.machine.writePort8(port, value)) return LCOM_ERR;
  bool retrigger = port == AC97_BM_BASE + AC97_PO_CR && (value & AC97_PO_CR_RUN);
  if (device.playing() && (!was_playing || retrigger)) {
    std::copy(s.audio.begin(), s.audio.end(), device.pcm().begin());
    lcom_browser_audio(reinterpret_cast<const int16_t *>(device.pcm().data()),
                       device.playByteCount() / (2 * device.channels()),
                       device.sampleRate(), device.channels());
  } else if (was_playing && !device.playing()) lcom_browser_audio_stop();
  return LCOM_OK;
}

int lcom_irq_subscribe(uint8_t irq, uint32_t, lcom_irq_t *out) {
  if (!session || !out) return LCOM_ERR;
  lcom::IrqSubscription sub;
  if (!session->machine.subscribeIrq(irq, sub)) return LCOM_ERR;
  *out = {sub.irq, sub.bit_no, sub.mask};
  return LCOM_OK;
}

int lcom_irq_unsubscribe(lcom_irq_t *irq) {
  if (!session || !irq || !session->machine.unsubscribeIrq(irq->irq)) return LCOM_ERR;
  irq->mask = 0;
  return LCOM_OK;
}

int lcom_event_wait(lcom_event_t *out) { return event(out, false, 0); }
int lcom_delay_ticks(uint64_t ticks, lcom_event_t *out) { return event(out, true, ticks); }

int lcom_phys_map(uintptr_t phys, size_t length, void **out) {
  if (!session || !out) return LCOM_ERR;
  auto range = session->machine.physicalRange(phys, length);
  if (!range) return LCOM_ERR;
  auto &memory = range->device == lcom::Machine::PhysicalRange::Device::Video
                     ? session->video : session->audio;
  *out = memory.data() + range->offset;
  session->mappings.push_back({*out, length});
  return LCOM_OK;
}

int lcom_phys_unmap(void *pointer, size_t length) {
  if (!session) return LCOM_ERR;
  auto &mappings = session->mappings;
  auto found = std::find_if(mappings.begin(), mappings.end(), [&](const Mapping &mapping) {
    return mapping.pointer == pointer && mapping.length == length;
  });
  if (found == mappings.end()) return LCOM_ERR;
  mappings.erase(found);
  return LCOM_OK;
}

int lcom_vbe_get_mode_info(uint16_t mode, lcom_vbe_mode_info_t *out) {
  if (!session || !out) return LCOM_ERR;
  lcom::VbeModeInfo info;
  if (!session->machine.vbe().modeInfo(mode, info)) return LCOM_ERR;
  *out = {info.mode, info.width, info.height, info.bpp, info.bytes_per_pixel,
          info.pitch, static_cast<uintptr_t>(info.framebuffer_phys),
          static_cast<size_t>(info.framebuffer_size)};
  return LCOM_OK;
}

int lcom_vbe_set_mode(uint16_t mode) {
  if (!session || !session->machine.vbe().setMode(mode)) return LCOM_ERR;
  if (mode != 3) std::fill(session->video.begin(), session->video.end(), 0);
  return LCOM_OK;
}

int lcom_vbe_set_display_start(uint16_t x, uint16_t y) {
  return session && session->machine.vbe().setDisplayStart(x, y) ? LCOM_OK : LCOM_ERR;
}

int lcom_vbe_present(void) {
  if (!session) return LCOM_ERR;
  auto &s = *session;
  auto &vbe = s.machine.vbe();
  if (!vbe.graphicsMode()) return LCOM_OK;
  std::copy_n(s.video.data() + vbe.displayOffset(), vbe.framebufferSize(), vbe.framebuffer().data());
  auto &mode = vbe.currentMode();
  s.rgba.resize(static_cast<size_t>(mode.width) * mode.height * 4);
  std::vector<uint8_t> row(static_cast<size_t>(mode.width) * 3);
  for (uint16_t y = 0; y < mode.height; ++y) {
    vbe.copyRgbRow(y, row.data());
    for (uint16_t x = 0; x < mode.width; ++x) {
      auto pixel = &s.rgba[(static_cast<size_t>(y) * mode.width + x) * 4];
      std::copy_n(row.data() + x * 3, 3, pixel);
      pixel[3] = 255;
    }
  }
  lcom_browser_present(s.rgba.data(), mode.width, mode.height);
  service();
  return LCOM_OK;
}

int lcom_ac97_get_buffer(lcom_ac97_buffer_info_t *out) {
  if (!session || !out) return LCOM_ERR;
  auto &audio = session->machine.ac97();
  *out = {static_cast<uintptr_t>(audio.bufferPhys()), audio.bufferSize(),
          audio.sampleRate(), audio.channels(), 16};
  return LCOM_OK;
}

// Clipboard is private to this game, matching the native headless contract.
int lcom_clipboard_set_text(const char *text) {
  if (!session || !text || strnlen(text, 4092) >= 4092) return LCOM_ERR;
  session->clipboard = text;
  return LCOM_OK;
}
int lcom_clipboard_get_text(char *buffer, size_t capacity) {
  if (!session || !buffer || capacity <= session->clipboard.size()) return LCOM_ERR;
  std::memcpy(buffer, session->clipboard.c_str(), session->clipboard.size() + 1);
  return LCOM_OK;
}

EMSCRIPTEN_KEEPALIVE void lcom_web_key(const char *key, int down) {
  if (session) session->machine.injectKey(key, down != 0);
}
EMSCRIPTEN_KEEPALIVE void lcom_web_mouse(int dx, int dy, int buttons) {
  if (session) {
    session->mouse.addMotion(dx, -dy);
    session->mouse.setButtons(buttons);
    if (auto packet = session->mouse.nextPacket(false))
      session->machine.injectMouse(packet->dx, packet->dy, packet->buttons);
  }
}
}
