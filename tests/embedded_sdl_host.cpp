#include "embedded_sdl_capture.hpp"
#include <algorithm>
#include <array>
#include <cstdio>

namespace {
struct PresentedFrame {
  SDL_Renderer *renderer = nullptr;
  SDL_Surface *surface = nullptr;
  ~PresentedFrame() { SDL_DestroySurface(surface); }
} presented;
std::array<SDL_Event,24> recentEvents{};
size_t eventCount=0;

bool tracePollEvent(SDL_Event *event) {
  bool result=SDL_PollEvent(event);
  if(result && event)recentEvents[eventCount++%recentEvents.size()]=*event;
  return result;
}

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

void dumpRecentSdlEvents() {
  size_t count=std::min(eventCount,recentEvents.size());
  for(size_t i=eventCount-count;i<eventCount;++i) {
    const auto &event=recentEvents[i%recentEvents.size()];
    SDL_WindowID window=0;
    if(event.type>=SDL_EVENT_WINDOW_FIRST && event.type<=SDL_EVENT_WINDOW_LAST)window=event.window.windowID;
    if(event.type>=SDL_EVENT_FINGER_DOWN && event.type<=SDL_EVENT_FINGER_CANCELED)window=event.tfinger.windowID;
    char description[256]{};
#if SDL_VERSION_ATLEAST(3,4,0)
    SDL_GetEventDescription(&event,description,sizeof description);
#endif
    std::fprintf(stderr,"Polled SDL event 0x%04x, window %u",event.type,window);
    if(event.type>=SDL_EVENT_FINGER_DOWN && event.type<=SDL_EVENT_FINGER_CANCELED)
      std::fprintf(stderr,", finger %llu at %.4f,%.4f",static_cast<unsigned long long>(event.tfinger.fingerID),
                   event.tfinger.x,event.tfinger.y);
    if(event.type>=SDL_EVENT_WINDOW_FIRST && event.type<=SDL_EVENT_WINDOW_LAST)
      std::fprintf(stderr,", data %d,%d",event.window.data1,event.window.data2);
    if(event.type>=SDL_EVENT_USER)std::fprintf(stderr,", user code 0x%x",event.user.code);
    std::fprintf(stderr,": %s\n",description);
  }
}

// Intercept this call only in the test's copy of the unchanged host. Production
// builds keep the normal SDL translation unit and have no capture interface.
#define SDL_RenderPresent capturePresent
#define SDL_PollEvent tracePollEvent
#include "../runtime/embedded/SdlHost.cpp"
