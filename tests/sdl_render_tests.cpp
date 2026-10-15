#include "../runtime/backends/SdlBackend.hpp"

#include <SDL3/SDL.h>

#include <chrono>
#include <cstdio>
#include <cstring>
#include <stdexcept>

static void require(bool ok, const char *message) {
  if (!ok) throw std::runtime_error(message);
}

int main(int argc, char **argv) {
  try {
    const int frames = argc == 2 && std::strcmp(argv[1], "--benchmark") == 0 ? 100 : 1;
    lcom::Machine machine;
    auto display = lcom::createSdlBackend({});
    require(display->start(machine), "SDL backend startup");
    // Check interoperability with the real SDL clipboard in both directions.
    char *saved_clipboard = SDL_GetClipboardText();
    std::string clipboard;
    require(SDL_SetClipboardText("host text"), "host clipboard write");
    require(display->getClipboardText(clipboard) && clipboard == "host text", "guest reads host clipboard");
    require(display->setClipboardText("guest café"), "guest clipboard write");
    char *host_clipboard = SDL_GetClipboardText();
    bool copied = host_clipboard != nullptr && std::strcmp(host_clipboard, "guest café") == 0;
    SDL_free(host_clipboard);
    SDL_SetClipboardText(saved_clipboard == nullptr ? "" : saved_clipboard);
    SDL_free(saved_clipboard);
    require(copied, "host reads guest clipboard");
    int count = 0;
    SDL_Window **windows = SDL_GetWindows(&count);
    require(count == 1, "one runtime window");
    SDL_Renderer *renderer = SDL_GetRenderer(windows[0]);
    SDL_free(windows);
    require(renderer != nullptr, "runtime renderer");

    // Command must reach the guest for native desktop shortcuts.
    // Exercise real SDL dispatch, not a duplicate of the key-name mapping.
    auto send_key = [&](SDL_Scancode code, bool down) {
      SDL_Event event{};
      event.type = down ? SDL_EVENT_KEY_DOWN : SDL_EVENT_KEY_UP;
      event.key.scancode = code;
      event.key.down = down;
      require(SDL_PushEvent(&event), "queue host key");
      display->pump(machine);
    };
    send_key(SDL_SCANCODE_LGUI, true);
    send_key(SDL_SCANCODE_S, true);
    send_key(SDL_SCANCODE_S, false);
    send_key(SDL_SCANCODE_LGUI, false);
    for (uint8_t expected : {0xE0, 0x5B, 0x1F, 0x9F, 0xE0, 0xDB}) {
      uint8_t status = 0, byte = 0;
      require(machine.readPort8(0x64, status) && (status & 1), "Command-S guest byte available");
      require(machine.readPort8(0x60, byte) && byte == expected, "Command-S guest scancode");
    }

    send_key(SDL_SCANCODE_RGUI, true);
    SDL_Event focus{};
    focus.type = SDL_EVENT_WINDOW_FOCUS_LOST;
    require(SDL_PushEvent(&focus), "queue focus loss");
    display->pump(machine);
    for (uint8_t expected : {0xE0, 0x5C, 0xE0, 0xDC}) {
      uint8_t status = 0, byte = 0;
      require(machine.readPort8(0x64, status) && (status & 1), "focus-loss modifier byte available");
      require(machine.readPort8(0x60, byte) && byte == expected, "focus loss releases held Command");
    }

    // Literal expectations cover channel order, exact bitfield scaling, opaque
    // padding bytes, row pitch, and texture recreation when switching modes.
    const struct {
      uint16_t mode;
      uint8_t raw[8];
      uint8_t rgb[6];
    } cases[] = {
      {0x200, {7, 38, 143, 0, 215, 63, 12, 127}, {143, 38, 7, 12, 63, 215}},
      {0x14c, {7, 38, 143, 255, 215, 63, 12, 0}, {143, 38, 7, 12, 63, 215}},
      {0x115, {7, 38, 143, 215, 63, 12}, {143, 38, 7, 12, 63, 215}},
      {0x118, {7, 38, 143, 215, 63, 12}, {143, 38, 7, 12, 63, 215}},
      {0x110, {0x84, 0x10, 0xe0, 0x7f}, {32, 32, 32, 255, 255, 0}},
      {0x111, {0x84, 0x20, 0xe0, 0xff}, {32, 16, 32, 255, 255, 0}},
      {0x117, {0x84, 0x20, 0xe0, 0xff}, {32, 16, 32, 255, 255, 0}},
      {0x11a, {0x84, 0x20, 0xe0, 0xff}, {32, 16, 32, 255, 255, 0}},
      {0x105, {56, 72}, {125, 125, 255, 125, 255, 125}},
    };
    for (const auto &test : cases) {
      require(machine.vbe().setMode(test.mode), "supported mode");
      const auto &mode = machine.vbe().currentMode();
      auto &fb = machine.vbe().framebuffer();
      for (unsigned y = 0; y < mode.height; ++y) {
        for (unsigned x = 0; x < mode.width; ++x) {
          unsigned color = (x / 32 + y / 32) % 2;
          std::memcpy(fb.data() + y * mode.pitch + x * mode.bytes_per_pixel,
                      test.raw + color * mode.bytes_per_pixel, mode.bytes_per_pixel);
        }
      }
      SDL_Texture *target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                                              SDL_TEXTUREACCESS_TARGET, mode.width, mode.height);
      require(target && SDL_SetRenderTarget(renderer, target), "readback target");
      // A texture target keeps pixels readable after present(), unlike a window
      // backbuffer. SDL_RenderPresent cannot display this target; readback flushes it.
      uint8_t expected[6];
      std::memcpy(expected, test.rgb, sizeof(expected));
      if (frames > 1) {
        auto start = std::chrono::steady_clock::now();
        for (int frame = 0; frame < frames; ++frame)
          require(machine.vbe().dumpPpm("/dev/null", "CAPTURE PERFORMANCE"), "capture benchmark");
        double ms = std::chrono::duration<double, std::milli>(
            std::chrono::steady_clock::now() - start).count() / frames;
        std::printf("mode=0x%x capture_ms=%.3f\n", test.mode, ms);
      }
      for (int phase = 0; phase < (mode.bpp == 8 ? 5 : 1); ++phase) {
        if (phase >= 1 && phase <= 3) {
          if (phase == 1) require(machine.writePort8(0x3c8, 56), "DAC index");
          const uint8_t dac[] = {63, 21, 0};
          const uint8_t rgb[] = {255, 85, 0};
          require(machine.writePort8(0x3c9, dac[phase - 1]), "DAC component");
          expected[phase - 1] = rgb[phase - 1];
        } else if (phase == 4) {
          require(machine.writePort8(0x3c6, 0x0f), "pixel mask");
          // Both indices now select VGA entry 8, dark gray.
          for (auto &channel : expected) channel = 85;
        }
        auto start = std::chrono::steady_clock::now();
        for (int frame = 0; frame < frames; ++frame) display->present(machine);
        SDL_Surface *pixels = SDL_RenderReadPixels(renderer, nullptr);
        require(pixels != nullptr, "frame readback");
        if (frames > 1) {
          double ms = std::chrono::duration<double, std::milli>(
              std::chrono::steady_clock::now() - start).count() / frames;
          std::printf("mode=0x%x phase=%d render_ms=%.3f\n", test.mode, phase, ms);
        }
        // Stay below the runtime's startup hint, but check every row and edges.
        for (unsigned y = 100; y < mode.height; ++y) {
          for (unsigned x : {0u, 31u, 32u, 63u, static_cast<unsigned>(mode.width - 1)}) {
            Uint8 r, g, b, a;
            require(SDL_ReadSurfacePixel(pixels, x, y, &r, &g, &b, &a), "read pixel");
            const uint8_t *rgb = expected + ((x / 32 + y / 32) % 2) * 3;
            require(r == rgb[0] && g == rgb[1] && b == rgb[2] && a == 255,
                    "rendered color and opacity");
          }
        }
        SDL_DestroySurface(pixels);
      }
      require(SDL_SetRenderTarget(renderer, nullptr), "restore window target");
      SDL_DestroyTexture(target);
    }
  } catch (const std::exception &error) {
    std::fprintf(stderr, "%s: %s\n", error.what(), SDL_GetError());
    return 1;
  }
  return 0;
}
