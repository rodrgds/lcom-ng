#ifndef MACHINE_LAB_DISPLAY_BACKEND_HPP
#define MACHINE_LAB_DISPLAY_BACKEND_HPP

#include "../core/Machine.hpp"
#include <string>

namespace lcom {

class DisplayBackend {
public:
  virtual ~DisplayBackend() = default;
  virtual bool start(Machine &) { return true; }
  virtual void pump(Machine &) {}
  virtual void consoleWrite(const char *, size_t) {}
  virtual void present(Machine &) {}
  virtual bool setClipboardText(const std::string &text) { clipboard_ = text; return true; }
  virtual bool getClipboardText(std::string &text) { text = clipboard_; return true; }
private:
  std::string clipboard_;
};

} // namespace lcom

#endif
