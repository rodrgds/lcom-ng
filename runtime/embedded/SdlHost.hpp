#ifndef LCOM_EMBEDDED_SDL_HOST_HPP
#define LCOM_EMBEDDED_SDL_HOST_HPP
#include <exception>
namespace lcom::embedded {
// SDL close terminates even legacy polling loops that ignore SDK errors.
struct QuitRequested final : std::exception {};
}
#endif
