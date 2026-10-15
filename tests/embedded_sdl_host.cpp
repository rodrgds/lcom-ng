#include "embedded_sdl_capture.hpp"

namespace {
struct PresentedFrame {
  SDL_Renderer *renderer = nullptr;
  SDL_Surface *surface = nullptr;
  ~PresentedFrame() { SDL_DestroySurface(surface); }
} presented;

bool capturePresent(SDL_Renderer *renderer) {
  SDL_DestroySurface(presented.surface);
  presented.renderer = renderer;
  presented.surface = SDL_RenderReadPixels(renderer, nullptr);
  bool result = SDL_RenderPresent(renderer);
  // SDL permits backbuffer invalidation after presentation. Clear it deliberately
  // so tests cannot accidentally depend on a backend retaining its pixels.
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_FlushRenderer(renderer);
  return result;
}
}

SDL_Surface *readPresentedFrame(SDL_Renderer *renderer) {
  if (renderer != presented.renderer || !presented.surface) {
    SDL_SetError("No captured presentation for this renderer");
    return nullptr;
  }
  SDL_Surface *surface = presented.surface;
  presented.surface = nullptr;
  return surface;
}

// Intercept this call only in the test's copy of the unchanged host. Production
// builds keep the normal SDL translation unit and have no capture interface.
#define SDL_RenderPresent capturePresent
#include "../runtime/embedded/SdlHost.cpp"
