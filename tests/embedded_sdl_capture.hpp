#pragma once
#include <SDL3/SDL.h>

// Take ownership of the latest submitted frame. Every read requires a new
// presentation, so an absent redraw cannot reuse an earlier valid capture.
SDL_Surface *readPresentedFrame(SDL_Renderer *renderer);
void dumpRecentSdlEvents();
