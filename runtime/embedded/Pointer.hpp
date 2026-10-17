#ifndef LCOM_EMBEDDED_POINTER_HPP
#define LCOM_EMBEDDED_POINTER_HPP

#include "core/MousePacketScheduler.hpp"
#include <algorithm>
#include <deque>

namespace lcom::embedded {
// The guest's cursor contract, in framebuffer coordinates. -1 means centered
// start or the final framebuffer pixel. Direct mapping requires unit deltas
// and these bounds, with no independent guest cursor warps or acceleration.
struct CursorConfig {
  int x = -1, y = -1, maxX = -1, maxY = -1;
  int minX = 0, minY = 0;
  bool rejectBounds = false;
  // Guest pixels per raw unit. Native profiles retain unit tracking; the
  // browser sets this only for verified physical-mouse calibration.
  double pixelsPerUnit = 1;
  bool fractional = false;
};

class Pointer {
  struct Input { bool absolute; int x, y; uint8_t buttons; bool edge; bool hover = false; };
  std::deque<Input> pending_;
  CursorConfig config_;
  double x_ = 400, y_ = 300;
  int max_x_ = 799, max_y_ = 599, min_x_ = 0, min_y_ = 0;
  uint8_t requested_buttons_ = 0, delivered_buttons_ = 0;

  void append(Input input) {
    input.buttons &= 7;
    input.edge = input.buttons != requested_buttons_;
    input.hover = input.absolute && !requested_buttons_;
    requested_buttons_ = input.buttons;
    // Preserve the position of every press/release, but replace stale motion
    // within a hold. Never edit a packet already handed to the controller.
    if (!pending_.empty() && !input.edge && !pending_.back().edge &&
        pending_.back().absolute == input.absolute) {
      auto &last = pending_.back();
      if (input.absolute) { last.x = input.x; last.y = input.y; }
      else { last.x = std::clamp(last.x + input.x, -4096, 4096); last.y = std::clamp(last.y + input.y, -4096, 4096); }
    } else pending_.push_back(input);
  }
public:
  void cancelPending() { pending_.clear(); requested_buttons_ = delivered_buttons_ = 0; }
  // A device command resets unsent motion, but does not release a button that
  // the guest has already received. Preserve that state for the next edge.
  void discardPending() { pending_.clear(); requested_buttons_ = delivered_buttons_; }
  void resetMotion() {
    // Commands clear X/Y counters, not sampled button transitions. Retain each
    // edge at zero motion so a short press/release cannot vanish while waiting
    // for its next report interval.
    std::deque<Input> edges;
    for (const auto &input : pending_)
      if (input.edge) edges.push_back({false,0,0,input.buttons,true});
    pending_=std::move(edges);
    requested_buttons_=pending_.empty() ? delivered_buttons_ : pending_.back().buttons;
  }
  void configure(CursorConfig config) { config_ = config; }
  void reset(int width, int height) {
    pending_.clear(); requested_buttons_ = delivered_buttons_ = 0;
    max_x_ = config_.maxX < 0 ? std::max(0,width-1) : config_.maxX;
    max_y_ = config_.maxY < 0 ? std::max(0,height-1) : config_.maxY;
    min_x_ = std::clamp(config_.minX,0,max_x_);
    min_y_ = std::clamp(config_.minY,0,max_y_);
    x_ = std::clamp(config_.x < 0 ? width/2 : config_.x, min_x_, max_x_);
    y_ = std::clamp(config_.y < 0 ? height/2 : config_.y, min_y_, max_y_);
  }
  // Keep motion until next() splits it into PS/2-sized packets. A fast
  // browser event can span hundreds of pixels; limiting it to one packet
  // would leave the guest cursor behind the physical pointer.
  void relative(int dx, int dy, uint8_t buttons) {
    if (dx || dy || (buttons & 7) != requested_buttons_)
      append({false, std::clamp(dx,-4096,4096), std::clamp(dy,-4096,4096), buttons, false});
  }
  void absolute(int x, int y, uint8_t buttons) {
    append({true, std::clamp(x,min_x_,max_x_), std::clamp(y,min_y_,max_y_), buttons, false});
  }
  std::optional<MousePacket> next() {
    while (!pending_.empty()) {
      auto &input = pending_.front();
      // A touch has no preceding mouse hover. Let legacy menus release their
      // click guard before either released travel or a new button press.
      if (input.hover) {
        input.hover = false;
        return MousePacket{0,0,delivered_buttons_};
      }
      int dx = input.absolute ? static_cast<int>((input.x-x_)/config_.pixelsPerUnit) : input.x;
      int dy = input.absolute ? static_cast<int>((input.y-y_)/config_.pixelsPerUnit) : input.y;
      int step_x = std::clamp(dx,-96,96), step_y = std::clamp(dy,-96,96);
      bool arrived = dx == step_x && dy == step_y;
      // Direct touch arrives before its press. Games that handle a press edge
      // instead of movement in that sample must still see the target position.
      bool defer_press = input.absolute && (input.buttons & ~delivered_buttons_) && (step_x || step_y);
      uint8_t buttons = arrived && !defer_press ? input.buttons : delivered_buttons_;
      auto advance = [&](double position,double step,int minimum,int maximum) {
        double target=position+(config_.fractional ? step : static_cast<int>(step));
        return config_.rejectBounds && (target<minimum || target>maximum) ? position : std::clamp(target,static_cast<double>(minimum),static_cast<double>(maximum));
      };
      // Track what each delivered packet does, including integer division in
      // half-speed games. Relative touch and calibrated mouse share one model.
      x_ = advance(x_,step_x*config_.pixelsPerUnit,min_x_,max_x_);
      y_ = advance(y_,step_y*config_.pixelsPerUnit,min_y_,max_y_);
      if (arrived && !defer_press) pending_.pop_front();
      else if (!input.absolute) { input.x -= step_x; input.y -= step_y; }
      if (!step_x && !step_y && buttons == delivered_buttons_) continue;
      delivered_buttons_ = buttons;
      return MousePacket{step_x,-step_y,buttons};
    }
    return std::nullopt;
  }
};
}
#endif
