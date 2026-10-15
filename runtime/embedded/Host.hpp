#ifndef LCOM_EMBEDDED_HOST_HPP
#define LCOM_EMBEDDED_HOST_HPP

#include "core/Machine.hpp"
#include "Pointer.hpp"
#include <cstddef>
#include <cstdint>
#include <string>

namespace lcom::embedded {

// An embedded game and its host run on one thread. The host owns event pumping,
// presentation, audio and monotonic time. Device behavior stays in Machine.
class Host {
public:
  virtual ~Host() = default;
  virtual bool start() = 0;
  virtual void stop() = 0;
  virtual double nowMs() = 0;
  virtual void sleepMs(unsigned milliseconds) = 0;
  virtual void pump() = 0;
  virtual bool paused() = 0;
  virtual void present(Machine &machine) = 0;
  virtual void audio(const int16_t *samples, size_t frames, uint32_t rate, uint8_t channels) = 0;
  virtual void stopAudio() = 0;
  virtual bool setClipboard(const std::string &text) = 0;
  virtual bool getClipboard(std::string &text) = 0;
};

// Each executable links one host, e.g. browser or SDL. No platform branches
// belong in the SDK implementation. Call injection functions on the host thread.
Host &host();
void injectKey(const char *key, bool down);
void injectMouse(int dx, int screen_dy, uint8_t buttons);
void injectPointer(int x, int y, uint8_t buttons);
void configureCursor(CursorConfig config);

} // namespace lcom::embedded
#endif
