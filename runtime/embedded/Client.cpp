// Shared in-process SDK for browser and native app hosts. Device behavior belongs to Machine.
#include "core/Machine.hpp"
#include <lcom/lcom.h>
#include <lcom/ac97.h>
#include <lcom/vbe.h>
#include <lcom/i8042.h>
#include "Host.hpp"

#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <memory>
#include <vector>

using lcom::embedded::host;

namespace {
struct Mapping { void *pointer; size_t length; };
struct Session {
  lcom::Machine machine;
  // Fixed backing allocations retain mapped C pointers across mode changes.
  std::vector<uint8_t> video = std::vector<uint8_t>(lcom::Vbe::vramSize());
  std::vector<uint8_t> audio = std::vector<uint8_t>(machine.ac97().bufferSize());
  std::vector<Mapping> mappings;
  lcom::embedded::Pointer mouse;
  double next_tick = host().nowMs();
  double last_yield = next_tick;
  bool frame_invalid = true;
  bool mouse_subscribed = false;
  bool mouse_irq_only = [] { const char *v=std::getenv("LCOM_COMPAT_MOUSE_IRQ_ONLY"); return v && std::strcmp(v,"1")==0; }();
};
std::unique_ptr<Session> session;

void flushMouse(Session &s) {
  // One complete PS/2 packet at a time. High-rate host motion stays coalescible
  // until the guest has consumed all three bytes of the preceding packet.
  if ((s.mouse_irq_only && !s.mouse_subscribed) || !s.machine.i8042().mouseStreaming() ||
      (s.machine.i8042().status() & KBC_ST_OBF)) return;
  if (auto packet = s.mouse.next())
    s.machine.injectMouse(packet->dx, packet->dy, packet->buttons);
}

void service(bool wait = false) {
  if (!session) return;
  auto &s = *session;
  double now = host().nowMs();
  if (wait || now - s.last_yield >= 8.0 || host().paused()) {
    host().sleepMs(wait ? 2 : 0);
    host().pump();
    while (host().paused()) {
      host().sleepMs(50);
      host().pump();
      s.next_tick = host().nowMs();
    }
    now = host().nowMs();
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
  flushMouse(s);
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
    if (!host().start()) { host().stop(); session.reset(); return LCOM_ERR; }
  }
  return LCOM_OK;
}

void lcom_exit(void) {
  host().stopAudio();
  host().stop();
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
  bool was_streaming = s.machine.i8042().mouseStreaming();
  if (!s.machine.writePort8(port, value)) return LCOM_ERR;
  if (was_streaming && !s.machine.i8042().mouseStreaming()) s.mouse.cancelPending();
  // Indexed colors can change while framebuffer bytes remain identical.
  if (s.machine.vbe().handles(port)) s.frame_invalid = true;
  bool retrigger = port == AC97_BM_BASE + AC97_PO_CR && (value & AC97_PO_CR_RUN);
  if (device.playing() && (!was_playing || retrigger)) {
    std::copy(s.audio.begin(), s.audio.end(), device.pcm().begin());
    host().audio(reinterpret_cast<const int16_t *>(device.pcm().data()),
                       device.playByteCount() / (2 * device.channels()),
                       device.sampleRate(), device.channels());
  } else if (was_playing && !device.playing()) host().stopAudio();
  return LCOM_OK;
}

int lcom_irq_subscribe(uint8_t irq, uint32_t, lcom_irq_t *out) {
  if (!session || !out) return LCOM_ERR;
  lcom::IrqSubscription sub;
  if (!session->machine.subscribeIrq(irq, sub)) return LCOM_ERR;
  if(irq==MOUSE_IRQ) session->mouse_subscribed=true;
  *out = {sub.irq, sub.bit_no, sub.mask};
  return LCOM_OK;
}

int lcom_irq_unsubscribe(lcom_irq_t *irq) {
  if (!session || !irq || !session->machine.unsubscribeIrq(irq->irq)) return LCOM_ERR;
  if(irq->irq==MOUSE_IRQ) {
    session->mouse_subscribed=false;
    if(session->mouse_irq_only) session->mouse.cancelPending();
  }
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
  session->frame_invalid = true;
  if (mode != 3) {
    const auto &info = session->machine.vbe().currentMode();
    session->mouse.reset(info.width, info.height);
  }
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
  const auto *pixels = s.video.data() + vbe.displayOffset();
  if (s.frame_invalid || std::memcmp(pixels, vbe.framebuffer().data(), vbe.framebufferSize()) != 0) {
    std::copy_n(pixels, vbe.framebufferSize(), vbe.framebuffer().data());
    s.frame_invalid = false;
    host().present(s.machine);
  }
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

// The host owns clipboard policy: private in a browser, system clipboard in SDL.
int lcom_clipboard_set_text(const char *text) {
  if (!session || !text || strnlen(text, 4092) >= 4092) return LCOM_ERR;
  return host().setClipboard(text) ? LCOM_OK : LCOM_ERR;
}
int lcom_clipboard_get_text(char *buffer, size_t capacity) {
  if (!session || !buffer) return LCOM_ERR;
  std::string text;
  if (!host().getClipboard(text) || text.size() >= 4092 || capacity <= text.size()) return LCOM_ERR;
  std::memcpy(buffer, text.c_str(), text.size() + 1);
  return LCOM_OK;
}

} // extern "C"

namespace lcom::embedded {
void configureCursor(CursorConfig config) {
  if (session) {
    session->mouse.configure(config);
    const auto &mode = session->machine.vbe().currentMode();
    session->mouse.reset(mode.width, mode.height);
  }
}
void injectKey(const char *key, bool down) {
  if (session) session->machine.injectKey(key, down != 0);
}
void injectMouse(int dx, int dy, uint8_t buttons) {
  if (session) {
    if(session->mouse_irq_only && !session->mouse_subscribed) return;
    if (!session->machine.i8042().mouseStreaming()) {
      session->machine.injectMouse(std::clamp(dx,-96,96), -std::clamp(dy,-96,96), buttons);
      return;
    }
    session->mouse.relative(dx, dy, buttons);
    flushMouse(*session);
  }
}
void injectPointer(int x, int y, uint8_t buttons) {
  if (session && (!session->mouse_irq_only || session->mouse_subscribed) && session->machine.i8042().mouseStreaming()) {
    session->mouse.absolute(x,y,buttons);
    flushMouse(*session);
  }
}
}
