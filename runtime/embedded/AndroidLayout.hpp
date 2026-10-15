#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <cstdint>
#include <utility>
#ifdef __ANDROID__
#include <SDL3/SDL_system.h>
#include <jni.h>
#include <cmath>
#endif

namespace lcom::embedded {
// Native collection chrome follows the actual SDL framebuffer and controls.
// Standalone hosts may omit the optional callback. No JNI work on unchanged frames.
inline void publishAndroidLayout(int widthPx, int heightPx, float scale,
                                 const SDL_FRect &game, const std::vector<SDL_FRect> &occupied,
                                 uint32_t button=0xd2222222,uint32_t text=0xeeeeee) {
#ifdef __ANDROID__
  std::vector<jint> geometry{widthPx, heightPx, static_cast<jint>(std::lround(44 * scale))};
  auto append = [&](const SDL_FRect &r) {
    for (float edge : {r.x, r.y, r.x+r.w, r.y+r.h})
      geometry.push_back(static_cast<jint>(std::lround(edge * scale)));
  };
  append(game);
  for (const auto &rect : occupied) append(rect);
  geometry.push_back(static_cast<jint>(button));geometry.push_back(static_cast<jint>(text));
  static std::vector<jint> previous;
  if (geometry == previous) return;
  auto *env = static_cast<JNIEnv *>(SDL_GetAndroidJNIEnv());
  auto activity = static_cast<jobject>(SDL_GetAndroidActivity());
  if (!env || !activity) return;
  jclass type = env->GetObjectClass(activity);
  jmethodID callback = env->GetMethodID(type, "onGameLayout", "([I)V");
  if (!callback) {
    // SDL's standalone Activity has no collection return control.
    env->ExceptionClear();
  } else {
    jintArray values = env->NewIntArray(static_cast<jsize>(geometry.size()));
    if (values) {
      env->SetIntArrayRegion(values, 0, static_cast<jsize>(geometry.size()), geometry.data());
      env->CallVoidMethod(activity, callback, values);
      env->DeleteLocalRef(values);
    }
  }
  env->DeleteLocalRef(type);
  env->DeleteLocalRef(activity);
  previous = std::move(geometry);
#else
  (void)widthPx; (void)heightPx; (void)scale; (void)game; (void)occupied; (void)button; (void)text;
#endif
}
} // namespace lcom::embedded
