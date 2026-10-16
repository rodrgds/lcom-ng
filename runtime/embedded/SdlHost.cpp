#include "Host.hpp"
#include "SdlHost.hpp"
#include "Controls.hpp"
#include "ControlFont.hpp"
#include "ControlIcons.hpp"
#include "AndroidLayout.hpp"
#include "backends/SdlAudioBackend.hpp"
#include <SDL3/SDL.h>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <cstdio>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <tuple>

namespace lcom::embedded {
namespace {
ControlConfig settings;
struct Pad { SDL_FRect rect; PadConfig config; float x = 0, y = 0, dx = 0, dy = 0; float angle = 0, contactAngle = 0; };
struct Button { SDL_FRect rect; std::string label, key; int action = 0; uint8_t mouseButton = 0; std::string icon; };
uint8_t buttonMouse(const Button &b) { return b.mouseButton | (b.action==3?1:b.action==4?2:0); }
struct Finger {
  float x, y; int button = -1; bool game = false; float dx = 0, dy = 0; int pad = -1;
  float originX = 0, originY = 0; bool moved = false, blocked = false;
  Uint64 downAt = 0; bool held = false;
};
std::array<std::string,4> padKeys(const PadConfig &p) {
  if (!p.keys[0].empty()) return p.keys;
  if (p.mode=="wasd") return {"A","W","D","S"};
  if (p.mode=="keypad") return {"KP4","KP8","KP6","KP2"};
  return {"LEFT","UP","RIGHT","DOWN"};
}
bool padDirection(const PadConfig &p,int direction) {
  return p.axes=="both" || (direction%2==0 ? p.axes=="horizontal" : p.axes=="vertical");
}
std::vector<std::pair<int,SDL_FRect>> padTargets(const Pad &p) {
  std::vector<std::pair<int,SDL_FRect>> targets;
  float unit=p.rect.w/3;
  for(int direction=0;direction<4;++direction) if(padDirection(p.config,direction)) {
    float x=p.rect.x+(direction==0?0:direction==2?2:1)*unit;
    float y=p.rect.y+(direction==1?0:direction==3?2:1)*unit;
    targets.push_back({direction,{x,y,unit,unit}});
  }
  return targets;
}
bool inside(float x, float y, const SDL_FRect &r) { return x >= r.x && x < r.x+r.w && y >= r.y && y < r.y+r.h; }
bool padContains(const Pad &pad,float x,float y) {
  if(!inside(x,y,pad.rect))return false;
  if(pad.config.wheel)return std::hypot(x-pad.rect.x-pad.rect.w/2,y-pad.rect.y-pad.rect.h/2)<=pad.rect.w/2;
  if(!pad.config.directional)return true;
  for(const auto &target:padTargets(pad)) if(inside(x,y,target.second))return true;
  return false;
}
std::string keyName(SDL_Scancode code) {
  if (code >= SDL_SCANCODE_A && code <= SDL_SCANCODE_Z) return std::string(1, 'A' + code - SDL_SCANCODE_A);
  if (code >= SDL_SCANCODE_1 && code <= SDL_SCANCODE_9) return std::string(1, '1' + code - SDL_SCANCODE_1);
  if (code >= SDL_SCANCODE_F1 && code <= SDL_SCANCODE_F12) return "F" + std::to_string(code - SDL_SCANCODE_F1 + 1);
  if (code >= SDL_SCANCODE_KP_1 && code <= SDL_SCANCODE_KP_9) return "KP" + std::to_string(code-SDL_SCANCODE_KP_1+1);
  switch (code) {
  case SDL_SCANCODE_0: return "0";
  case SDL_SCANCODE_KP_0: return "KP0";
  case SDL_SCANCODE_ESCAPE: return "ESC";
  case SDL_SCANCODE_RETURN: return "ENTER";
  case SDL_SCANCODE_KP_ENTER: return "KP_ENTER";
  case SDL_SCANCODE_SPACE: return "SPACE";
  case SDL_SCANCODE_BACKSPACE: return "BACKSPACE";
  case SDL_SCANCODE_TAB: return "TAB";
  case SDL_SCANCODE_CAPSLOCK: return "CAPSLOCK";
  case SDL_SCANCODE_UP: return "UP";
  case SDL_SCANCODE_DOWN: return "DOWN";
  case SDL_SCANCODE_LEFT: return "LEFT";
  case SDL_SCANCODE_RIGHT: return "RIGHT";
  case SDL_SCANCODE_LSHIFT: case SDL_SCANCODE_RSHIFT: return "LSHIFT";
  case SDL_SCANCODE_LCTRL: case SDL_SCANCODE_RCTRL: return "LCTRL";
  case SDL_SCANCODE_LALT: case SDL_SCANCODE_RALT: return "LALT";
  case SDL_SCANCODE_MINUS: return "MINUS";
  case SDL_SCANCODE_EQUALS: return "EQUAL";
  case SDL_SCANCODE_LEFTBRACKET: return "LBRACKET";
  case SDL_SCANCODE_RIGHTBRACKET: return "RBRACKET";
  case SDL_SCANCODE_BACKSLASH: return "BACKSLASH";
  case SDL_SCANCODE_SEMICOLON: return "SEMICOLON";
  case SDL_SCANCODE_APOSTROPHE: return "APOSTROPHE";
  case SDL_SCANCODE_GRAVE: return "BACKQUOTE";
  case SDL_SCANCODE_COMMA: return "COMMA";
  case SDL_SCANCODE_PERIOD: return "PERIOD";
  case SDL_SCANCODE_SLASH: return "SLASH";
  case SDL_SCANCODE_DELETE: return "DELETE";
  case SDL_SCANCODE_INSERT: return "INSERT";
  case SDL_SCANCODE_HOME: return "HOME";
  case SDL_SCANCODE_END: return "END";
  case SDL_SCANCODE_PAGEUP: return "PAGEUP";
  case SDL_SCANCODE_PAGEDOWN: return "PAGEDOWN";
  default: return {};
  }
}

class SdlHost final : public Host {
  SDL_Window *window_ = nullptr;
  SDL_Renderer *renderer_ = nullptr;
  SDL_Texture *texture_ = nullptr, *font_texture_ = nullptr;
  SDL_PixelFormat format_ = SDL_PIXELFORMAT_UNKNOWN;
  // The Session owns this machine until stop(). Retain its last committed
  // frame so display restoration does not require another guest present call.
  Machine *last_machine_ = nullptr;
  std::unique_ptr<AudioBackend> audio_;
  std::vector<uint8_t> rgb_;
  std::map<SDL_FingerID, Finger> fingers_;
  std::set<SDL_Scancode> physical_;
  std::set<std::string> keys_;
  std::map<std::string,Uint64> repeat_keys_;
  std::vector<Button> buttons_;
  std::vector<Pad> pads_;
  uint64_t last_pad_time_ = 0;
  Uint64 tap_until_ = 0;
  // Released mouse actions retain their own minimum press, independently
  // of other contacts and physical button ownership.
  std::map<std::pair<SDL_FingerID,uint8_t>,Uint64> mouse_action_until_;
  bool joystick_open_ = false;
  SDL_FRect game_rect_{};
  std::vector<float> toggle_layout_;
  std::map<std::string,std::pair<std::vector<float>,SDL_FRect>> group_layout_;
  SDL_FRect toggle_rect_{};
  float scale_ = 1, width_ = 0, height_ = 0;
  int frame_width_ = 800, frame_height_ = 600;
  bool paused_ = false, keyboard_ = false, collapsed_ = false, move_only_ = false, tap_hold_ = false;
  bool controls_dirty_ = false;
  bool frame_dirty_ = false;
  unsigned page_ = 0;
  uint8_t mouse_ = 0, physical_mouse_ = 0;
  Uint32 lifecycle_event_ = 0;

  static bool SDLCALL lifecycle(void *userdata, SDL_Event *event) {
    if (event->type==SDL_EVENT_WILL_ENTER_BACKGROUND ||
        event->type==SDL_EVENT_DID_ENTER_FOREGROUND || event->type==SDL_EVENT_TERMINATING) {
      // SDL delivers Android app lifecycle events only to watchers. Defer
      // handling to our event loop, after SDL has restored its GL context.
      auto *self=static_cast<SdlHost *>(userdata);
      SDL_Event queued{};
      queued.type=self->lifecycle_event_;
      queued.user.code=static_cast<Sint32>(event->type);
      SDL_PushEvent(&queued);
    }
    return true;
  }

  void color(int r, int g, int b) { SDL_SetRenderDrawColor(renderer_, r, g, b, 255); }
  void release() {
    for (const auto &key : keys_) injectKey(key.c_str(), false);
    keys_.clear(); repeat_keys_.clear(); fingers_.clear(); physical_.clear();mouse_action_until_.clear();
    for (auto &pad : pads_) pad.x = pad.y = pad.dx = pad.dy = pad.angle = pad.contactAngle = 0;
    last_pad_time_ = SDL_GetTicksNS();
    mouse_ = physical_mouse_ = 0; tap_hold_=false;tap_until_=0;
    injectMouse(0, 0, 0);
  }
  void syncInput() {
    std::set<std::string> next;
    for (auto code : physical_) { auto key = keyName(code); if (!key.empty()) next.insert(key); }
    uint8_t mouse = physical_mouse_;
    bool game_touch = false;
    for (const auto &[id, finger] : fingers_) {
      (void)id;
      game_touch |= finger.game;
      if (finger.pad >= 0 && static_cast<size_t>(finger.pad) < pads_.size()) {
        const auto &p = pads_[finger.pad];
        if (p.config.mode != "mouse") {
          const auto keys=padKeys(p.config);
          bool left=p.config.wheel?p.angle < -p.config.deadZone:p.x < -.3f;
          bool right=p.config.wheel?p.angle > p.config.deadZone:p.x > .3f;
          if (padDirection(p.config,0) && left) next.insert(keys[0]);
          if (padDirection(p.config,2) && right) next.insert(keys[2]);
          if (!p.config.wheel && padDirection(p.config,1) && p.y < -0.3f) next.insert(keys[1]);
          if (!p.config.wheel && padDirection(p.config,3) && p.y > 0.3f) next.insert(keys[3]);
        }
      }
      if (finger.button < 0 || static_cast<size_t>(finger.button) >= buttons_.size()) continue;
      const auto &b = buttons_[finger.button];
      if (!b.key.empty()) next.insert(b.key);
      mouse |= buttonMouse(b);
    }
    for(const auto &[owner,deadline]:mouse_action_until_) mouse|=owner.second;
    for (auto &[id,finger] : fingers_) if (finger.game && mouse) finger.blocked=true;
    if(mouse&2) {tap_hold_=false;tap_until_=0;}
    if(tap_until_) mouse |= 1;
    if(tap_hold_) mouse |= 1;
    for (const auto &[id,f] : fingers_) if (f.game && f.held && !f.blocked && !(mouse&2)) mouse |= 1;
    bool press=settings.touch=="press" || (settings.touch=="auto" && settings.pointer!="move");
    if (game_touch && press && !move_only_ && !(mouse & 2)) mouse |= 1;
    for (const auto &key : keys_) if (!next.count(key)) injectKey(key.c_str(), false);
    for (const auto &key : next) if (!keys_.count(key)) injectKey(key.c_str(), true);
    for(auto it=repeat_keys_.begin();it!=repeat_keys_.end();) {
      if(!next.count(it->first)) it=repeat_keys_.erase(it);else ++it;
    }
    for(const auto &key:next) if(!keys_.count(key) &&
        std::find(settings.repeatKeys.begin(),settings.repeatKeys.end(),key)!=settings.repeatKeys.end())
      repeat_keys_[key]=SDL_GetTicks()+350;
    keys_ = std::move(next);
    bool direct = settings.pointer == "direct" && game_touch;
    if (direct && game_rect_.w > 0) {
      const auto &f = std::find_if(fingers_.begin(),fingers_.end(),[](const auto &v){return v.second.game;})->second;
      mouse_ = mouse;
      injectPointer(std::lround((f.x-game_rect_.x)*frame_width_/game_rect_.w),
                    std::lround((f.y-game_rect_.y)*frame_height_/game_rect_.h),mouse_);
    } else if (mouse != mouse_) { mouse_ = mouse; injectMouse(0, 0, mouse_); }
  }
  void row(float x, float y, float w, const std::vector<std::pair<std::string,std::string>> &keys) {
    float cell = w / keys.size();
    for (size_t i = 0; i < keys.size(); ++i)
      buttons_.push_back({{x + i*cell, y, cell-4, 44}, keys[i].first, keys[i].second});
  }
  const std::vector<std::string> keyboardQuickKeys_{"ESC","SPACE","ENTER"};
  bool configsArrowRow() const {
    return !settings.joystick.enabled && !joystick_open_ && !settings.look.enabled && !settings.dpad.enabled &&
           settings.pads.empty() &&
           (keyboard_ || settings.joystickMenu);
  }
  float textWidth(const std::string &value) const {
    float width=0;
    for(unsigned char c:value) if(c>=32 && c<127)
      width+=settings.font=="mono"?font::mono_advance[c-32]:font::ui_advance[c-32];
    return width;
  }
  SDL_FRect placeGroup(const std::string &name,SDL_FRect group,const std::string &corner,float left,float top,float width,float height,
                       const std::vector<SDL_FRect> &occupied) {
    group.w=std::min(group.w,width);group.h=std::min(group.h,height);
    float preferred_x=corner.find("right")!=std::string::npos?left+width-group.w:left;
    float preferred_y=corner.find("bottom")!=std::string::npos?top+height-group.h:top;
    std::vector<float> geometry{group.w,group.h,left,top,width,height,preferred_x,preferred_y};
    for(const auto &rect:occupied) geometry.insert(geometry.end(),{rect.x,rect.y,rect.w,rect.h});
    auto &cached=group_layout_[name];
    if(cached.first==geometry) return cached.second;
    std::vector<float> xs{preferred_x,left,left+width-group.w},ys{preferred_y,top,top+height-group.h};
    for(const auto &rect:occupied) {
      xs.insert(xs.end(),{rect.x-group.w-8,rect.x+rect.w+8});
      ys.insert(ys.end(),{rect.y-group.h-8,rect.y+rect.h+8});
    }
    std::tuple<float,float> best{INFINITY,INFINITY};
    for(float x:xs) for(float y:ys) {
      x=std::clamp(x,left,left+width-group.w);y=std::clamp(y,top,top+height-group.h);
      float overlap=0;
      for(const auto &rect:occupied) overlap+=std::max(0.f,std::min(x+group.w,rect.x+rect.w+4)-std::max(x,rect.x-4))*
          std::max(0.f,std::min(y+group.h,rect.y+rect.h+4)-std::max(y,rect.y-4));
      auto score=std::tuple{overlap,std::abs(x-preferred_x)+std::abs(y-preferred_y)};
      if(score<best) {best=score;group.x=x;group.y=y;}
    }
    cached={std::move(geometry),group};
    return group;
  }
  void placeToggle(float left,float top,float width,float height) {
    auto toggle=std::find_if(buttons_.begin(),buttons_.end(),[](const auto &b){return b.action==5;});
    if(toggle==buttons_.end() || width<44 || height<44) return;
    std::vector<SDL_FRect> occupied;
    for(const auto &b:buttons_) if(&b!=&*toggle) occupied.push_back(b.rect);
    if(!collapsed_ && !keyboard_) for(const auto &p:pads_) {
        if(p.config.directional) for(const auto &target:padTargets(p)) occupied.push_back(target.second);
        else occupied.push_back(p.rect);
      }
    std::vector<float> geometry{left,top,width,height,game_rect_.x,game_rect_.y,game_rect_.w,game_rect_.h};
    for(const auto &r:occupied) geometry.insert(geometry.end(),{r.x,r.y,r.w,r.h});
    if(geometry==toggle_layout_) {toggle->rect=toggle_rect_;return;}
    std::vector<float> xs{left,left+width-44},ys{top,top+height-44};
    auto edges=[&](const SDL_FRect &r) {
      xs.insert(xs.end(),{r.x-52,r.x+r.w+8});ys.insert(ys.end(),{r.y-52,r.y+r.h+8});
    };
    edges(game_rect_);for(const auto &r:occupied) edges(r);
    auto overlap=[](const SDL_FRect &a,const SDL_FRect &b,float gap) {
      return std::max(0.f,std::min(a.x+a.w,b.x+b.w+gap)-std::max(a.x,b.x-gap))*
             std::max(0.f,std::min(a.y+a.h,b.y+b.h+gap)-std::max(a.y,b.y-gap));
    };
    auto best=std::tuple<float,float,float>{INFINITY,INFINITY,INFINITY};
    for(float x:xs) for(float y:ys) {
      SDL_FRect candidate{std::clamp(x,left,left+width-44),std::clamp(y,top,top+height-44),44,44};
      float covered=0;for(const auto &r:occupied) covered+=overlap(candidate,r,4);
      auto score=std::tuple{covered,overlap(candidate,game_rect_,0),candidate.x-left+candidate.y-top};
      if(score<best) {best=score;toggle->rect=candidate;}
    }
    toggle_layout_=std::move(geometry);toggle_rect_=toggle->rect;
  }
  void layout() {
    int w, h; SDL_GetWindowSizeInPixels(window_, &w, &h);
    SDL_Rect safe; SDL_GetWindowSafeArea(window_, &safe);
    int window_w, window_h; SDL_GetWindowSize(window_, &window_w, &window_h);
    // Logical controls have at least 44 units per target at phone sizes.
    scale_ = std::max(1.f, std::min(w, h) / 400.f);
    width_ = w / scale_; height_ = h / scale_;
    const float sx = static_cast<float>(w) / std::max(1, window_w) / scale_;
    const float sy = static_cast<float>(h) / std::max(1, window_h) / scale_;
    float left = safe.x*sx + 8, top = safe.y*sy + 8;
    float available_w = safe.w*sx - 16, available_h = safe.h*sy - 16;
    bool landscape = available_w > available_h * 1.2f;
    bool has_controls = settings.keyboard || settings.mouseButtons || settings.joystickMenu ||
                        !settings.quickKeys.empty() || settings.joystick.enabled || settings.look.enabled || settings.dpad.enabled ||
                        !settings.pads.empty() || !settings.actions.empty();
    const bool touch_toggle=settings.pointer!="off" && has_controls && settings.touch!="move" &&
        !(settings.touch=="auto" && settings.pointer=="move");
    bool overlay = settings.layout == "overlay";
    const bool has_pads=settings.joystick.enabled || joystick_open_ || settings.look.enabled || settings.dpad.enabled || !settings.pads.empty();
    float pad_size=156.f;
    size_t action_height=0;
    for(const std::string vertical:{"top","bottom"}) {
      size_t widest=0;
      for(const std::string side:{"left","right"}) {
        size_t count=std::count_if(settings.actions.begin(),settings.actions.end(),[&](const auto &action){
          return (action.position.empty()?"bottom-"+action.side:action.position)==vertical+"-"+side;
        });
        widest=std::max(widest,count);
      }
      bool pedals=std::any_of(settings.actions.begin(),settings.actions.end(),[&](const auto &action){
        return !action.icon.empty() && (action.position.empty()?"bottom-"+action.side:action.position).find(vertical)==0;
      });
      action_height+=((widest+1)/2)*(pedals?92:56);
    }
    bool side_panel=landscape;
    bool left_panel=false;
    if(keyboard_) {
      side_panel=settings.keyboardPosition=="left" || settings.keyboardPosition=="right" ||
          (settings.keyboardPosition=="auto" && landscape);
      if(available_w<600) side_panel=false;
      left_panel=side_panel && settings.keyboardPosition=="left";
    }
    float panel_w = side_panel ? std::min(keyboard_?480.f:400.f, available_w*(keyboard_?0.6f:0.46f)) : available_w;
    const auto &quick_keys = keyboard_ ? keyboardQuickKeys_ : settings.quickKeys;
    float quick_w = settings.keyboard ? panel_w*0.75f : panel_w;
    const bool arrow_row = configsArrowRow();
    const bool action_row = touch_toggle || settings.mouseButtons || settings.joystickMenu || keyboard_;
    auto rows_for = [&](float width) {
      const size_t columns=std::max(1,static_cast<int>(width/64));
      return std::max<size_t>(1,(quick_keys.size()+columns-1)/columns);
    };
    float quick_h = (quick_keys.empty() && !settings.keyboard) ? 0 : rows_for(quick_w)*48.f;
    float controls_h = quick_h + (arrow_row?48:0) + (action_row?48:0) + (keyboard_?192:0);
    if(!overlay && !keyboard_) controls_h+=(has_pads?pad_size+8:0)+(action_height?action_height+8:0);
    float panel_h = collapsed_ ? 48.f : controls_h;
    if (side_panel) panel_h = available_h;
    if (!has_controls) panel_w = panel_h = 0;
    float px = side_panel && !left_panel ? left+available_w-panel_w : left;
    float py = side_panel ? top : top+available_h-panel_h;
    // Overlay controls stay inside the safe area. The game itself fits the
    // whole surface, including space beneath hidden Android system bars.
    bool full_surface = (overlay && !keyboard_) || !has_controls || collapsed_;
    float game_left = full_surface ? 0 : left+(side_panel && left_panel ? panel_w+8 : 0), game_top = full_surface ? 0 : top;
    float gw = full_surface ? width_ : side_panel ? available_w-panel_w-8 : available_w;
    float gh = full_surface ? height_ : !side_panel ? available_h-panel_h-8 : available_h;
    float fit = std::max(0.f, std::min(gw/frame_width_, gh/frame_height_));
    game_rect_ = {game_left+(gw-frame_width_*fit)/2, game_top+(gh-frame_height_*fit)/2, frame_width_*fit, frame_height_*fit};
    buttons_.clear();
    auto publish=[&] {
      placeToggle(left,top,available_w,available_h);
      std::vector<SDL_FRect> occupied;
      for(const auto &b:buttons_) occupied.push_back(b.rect);
      if(!collapsed_ && !keyboard_) for(const auto &p:pads_) {
        if(p.config.directional) for(const auto &target:padTargets(p)) occupied.push_back(target.second);
        else occupied.push_back(p.rect);
      }
      publishAndroidLayout(w,h,scale_,game_rect_,occupied,
          (overlay?0xd2000000u:0xff000000u)|settings.button,settings.text);
    };
    if (!has_controls) { pads_.clear(); publish(); return; }
    // Stable pad order preserves ownership while a rendered frame recomputes geometry.
    std::vector<PadConfig> configs;
    if (settings.joystick.enabled) configs.push_back(settings.joystick);
    else if (joystick_open_) configs.push_back({true,"mouse","left",1});
    if (settings.look.enabled) configs.push_back(settings.look);
    if (settings.dpad.enabled) configs.push_back(settings.dpad);
    configs.insert(configs.end(),settings.pads.begin(),settings.pads.end());
    if (pads_.size() != configs.size()) pads_.assign(configs.size(),{});
    float pad_left=overlay?left:px, pad_width=overlay?available_w:panel_w;
    for (size_t i=0;i<configs.size();++i) {
      auto &p=pads_[i]; p.config=configs[i];
      float x=p.config.side=="left" ? pad_left : pad_left+pad_width-pad_size;
      float y=top+available_h-pad_size;
      p.rect={x,y,pad_size,pad_size};
    }
    bool stack_keyboard=false;
    if (overlay && !keyboard_) {
      panel_w=std::min(280.f,available_w);
      const bool at_right=settings.toolbar=="auto" || settings.toolbar.find("right")!=std::string::npos;
      float gutter=at_right ? left+available_w-game_rect_.x-game_rect_.w-8 : game_rect_.x-left-8;
      float minimum=(settings.mouseButtons || settings.joystickMenu || arrow_row)?224.f:70.f;
      float candidate=std::min(panel_w,gutter);
      if(candidate>=minimum) {
        bool stacked=settings.keyboard && candidate<140;
        float quick_width=candidate-(settings.keyboard && !stacked?70.f:0.f);
        float rows=quick_keys.empty() ? (settings.keyboard && !stacked?1:0):rows_for(quick_width);
        if(rows*48+(stacked?48:0)+(arrow_row?48:0)+(action_row?48:0)<=available_h)panel_w=candidate;
      }
      px=left+(available_w-panel_w)/2;
      stack_keyboard=settings.keyboard && panel_w<140;
      quick_w=settings.keyboard && !stack_keyboard ? panel_w-70 : panel_w;
      quick_h=quick_keys.empty() ? (settings.keyboard && !stack_keyboard?48:0):rows_for(quick_w)*48.f;
      if(stack_keyboard)quick_h+=48;
      controls_h=quick_h+(arrow_row?48:0)+(action_row?48:0)+(keyboard_?192:0);
      py=top+available_h-controls_h;
      if (!landscape && !pads_.empty() && !keyboard_) py-=pad_size+8;
      if (!landscape && !keyboard_ && !settings.actions.empty()) {
        py-=action_height+8;
      }
    }
    if (collapsed_) { buttons_.push_back({{left,top,44,44},"@controls","",5}); publish(); return; }
    const float cell = panel_w/4;
    std::vector<std::pair<std::string,std::string>> quick;
    for (const auto &key : quick_keys) quick.push_back({key == "KP_ENTER" ? "Num Enter" : key,key});
    const size_t columns=std::max(1,static_cast<int>(quick_w/64));
    for (size_t start=0;start<quick.size();start+=columns) {
      std::vector<std::pair<std::string,std::string>> keys(quick.begin()+start,quick.begin()+std::min(start+columns,quick.size()));
      if(overlay && !keyboard_) {
        const float limit=quick_w/keys.size()-4;
        std::vector<float> widths;
        float total=0;
        for(const auto &key:keys) {
          float width=std::min(limit,std::max(44.f,textWidth(key.first)+20));
          widths.push_back(width);total+=width+4;
        }
        float x=settings.toolbar.find("left")!=std::string::npos?px:px+quick_w-total;
        for(size_t i=0;i<keys.size();++i) {
          buttons_.push_back({{x,py+(start/columns)*48,widths[i],44},keys[i].first,keys[i].second,0});
          x+=widths[i]+4;
        }
      } else row(px,py+(start/columns)*48,quick_w,keys);
    }
    if (settings.keyboard) buttons_.push_back({
        overlay && !keyboard_ ? SDL_FRect{px+panel_w-66,stack_keyboard?py+quick_h-48:py,66,44}:SDL_FRect{px+3*cell,py,cell-4,44},"@keyboard","",1});
    if (arrow_row)
      row(px,py+quick_h,panel_w,{{"@left","LEFT"},{"@up","UP"},{"@down","DOWN"},{"@right","RIGHT"}});
    float action_y=py+quick_h+(arrow_row?48:0);
    if (settings.mouseButtons) {
      buttons_.push_back({{px,action_y,cell-4,44},"@mouse-left","",3});
      buttons_.push_back({{px+cell,action_y,cell-4,44},"@mouse-right","",4});
    }
    if (keyboard_) buttons_.push_back({{px+2*cell,action_y,cell-4,44},page_==0?"Fn":page_==1?"#+=":"ABC","",2});
    else if (settings.joystickMenu) buttons_.push_back({{px+2*cell,action_y,cell-4,44},"@stick","",7});
    if (touch_toggle)
      buttons_.push_back({SDL_FRect{px+panel_w-44,action_y,44,44},move_only_?"@move":
          settings.touch=="hold-drag"?"@hold-drag":settings.touch=="tap"?"@tap":"@drag","",6});
    buttons_.push_back({{left,top,44,44},"@hide","",5});
    if (!keyboard_) {
      std::vector<SDL_FRect> occupied;
      for(const auto &pad:pads_) {
        if(pad.config.directional) for(const auto &target:padTargets(pad)) occupied.push_back(target.second);
        else occupied.push_back(pad.rect);
      }
      if(overlay || settings.toolbar!="auto") {
        std::string corner=settings.toolbar=="auto"?"top-right":settings.toolbar;
        SDL_FRect toolbar{px,py,panel_w,controls_h};
        auto placed=placeGroup("toolbar",toolbar,corner,left,top,available_w,available_h,occupied);
        for(auto &button:buttons_) if(button.action!=5) {button.rect.x+=placed.x-px;button.rect.y+=placed.y-py;}
      }
      for(const auto &button:buttons_) if(button.action!=5) occupied.push_back(button.rect);
      for(const std::string corner : {"top-left","top-right","bottom-left","bottom-right"}) {
        std::vector<ActionConfig> actions;
        for(const auto &action:settings.actions) {
          std::string position=action.position.empty()?"bottom-"+action.side:action.position;
          if(position==corner) actions.push_back(action);
        }
        if(actions.empty()) continue;
        bool pedals=std::any_of(actions.begin(),actions.end(),[](const auto &action){return !action.icon.empty();});
        float action_w=std::min(pedals?212.f:156.f,pedals?pad_width-4:pad_width/2-4);
        float action_h=pedals?88.f:52.f;
        if(corner.find("top")==0) {
          float cell=52;
          for(const auto &action:actions)cell=std::max(cell,textWidth(action.label)+24);
          action_w=std::min(actions.size()>1?cell*2+4:cell,pad_width/2-4);
        }
        float gutter=corner.find("right")!=std::string::npos ?
            left+available_w-game_rect_.x-game_rect_.w-8:game_rect_.x-left-8;
        if(overlay && gutter>=(actions.size()>1?108:52))action_w=std::min(action_w,gutter);
        SDL_FRect group{0,0,action_w,(action_h+4)*((actions.size()+1)/2)-4};
        group=placeGroup(corner,group,corner,pad_left,top,pad_width,available_h,occupied);
        for(size_t i=0;i<actions.size();++i) {
          float bw=actions.size()==1 ? action_w : (action_w-4)/2;
          SDL_FRect rect{group.x+(i%2)*(bw+4),group.y+(i/2)*(action_h+4),bw,action_h};
          buttons_.push_back({rect,actions[i].label,actions[i].key,0,actions[i].mouseButton,actions[i].icon});occupied.push_back(rect);
        }
      }
      publish(); return;
    }
    const float keyboard_y=action_y+48;
    row(px,keyboard_y,panel_w,{{"1","1"},{"2","2"},{"3","3"},{"4","4"},{"5","5"},{"6","6"},{"7","7"},{"8","8"},{"9","9"},{"0","0"}});
    if (page_ == 1) {
      row(px,keyboard_y+48,panel_w,{{"F1","F1"},{"F2","F2"},{"F3","F3"},{"F4","F4"},{"F5","F5"},{"F6","F6"}});
      row(px,keyboard_y+96,panel_w,{{"F7","F7"},{"F8","F8"},{"F9","F9"},{"F10","F10"},{"F11","F11"},{"F12","F12"}});
      row(px,keyboard_y+144,panel_w,{{"Home","HOME"},{"End","END"},{"PgUp","PAGEUP"},{"PgDn","PAGEDOWN"},{"Del","DELETE"}});
    } else if (page_ == 2) {
      row(px,keyboard_y+48,panel_w,{{"Ctrl","LCTRL"},{"Alt","LALT"},{"Tab","TAB"},{"[","LBRACKET"},{"]","RBRACKET"},{"=","EQUAL"}});
      row(px,keyboard_y+96,panel_w,{{";","SEMICOLON"},{"'","APOSTROPHE"},{"`","BACKQUOTE"},{",","COMMA"},{".","PERIOD"},{"/","SLASH"}});
      row(px,keyboard_y+144,panel_w,{{"Shift","LSHIFT"},{"Caps","CAPSLOCK"},{"-","MINUS"},{"Backslash","BACKSLASH"}});
    } else {
      row(px,keyboard_y+48,panel_w,{{"Q","Q"},{"W","W"},{"E","E"},{"R","R"},{"T","T"},{"Y","Y"},{"U","U"},{"I","I"},{"O","O"},{"P","P"}});
      row(px,keyboard_y+96,panel_w,{{"A","A"},{"S","S"},{"D","D"},{"F","F"},{"G","G"},{"H","H"},{"J","J"},{"K","K"},{"L","L"}});
      row(px,keyboard_y+144,panel_w,{{"Shift","LSHIFT"},{"Z","Z"},{"X","X"},{"C","C"},{"V","V"},{"B","B"},{"N","N"},{"M","M"},{"Back","BACKSPACE"}});
    }
    publish();
  }
  bool loadFont() {
    std::vector<uint8_t> atlas(95*16*36*4,0);
    for (int mono=0;mono<2;++mono) for (int c=0;c<95;++c) for (int row=0;row<18;++row) for (int col=0;col<(mono?9:16);++col) {
      uint64_t bits=mono?font::mono_rows[c][row]:font::ui_rows[c][row];
      size_t offset=((mono*18+row)*95*16+c*16+col)*4;
      atlas[offset]=atlas[offset+1]=atlas[offset+2]=255;
      atlas[offset+3]=((bits>>(((mono?9:16)-1-col)*4))&15)*17;
    }
    SDL_Surface *font_surface=SDL_CreateSurfaceFrom(95*16,36,SDL_PIXELFORMAT_RGBA32,atlas.data(),95*16*4);
    if (!font_surface) return false;
    font_texture_=SDL_CreateTextureFromSurface(renderer_,font_surface);
    SDL_DestroySurface(font_surface);
    if (!font_texture_) return false;
    SDL_SetTextureBlendMode(font_texture_,SDL_BLENDMODE_BLEND);
    return true;
  }
  void ink(uint32_t rgb, Uint8 alpha=255) { SDL_SetRenderDrawColor(renderer_,rgb>>16,(rgb>>8)&255,rgb&255,alpha); }
  void text(const std::string &value, float cx, float cy, uint32_t rgb, float size = 1) {
    bool mono=settings.font=="mono";
    float width=textWidth(value);
    float x=cx-width*size/2;
    SDL_SetTextureColorMod(font_texture_,rgb>>16,(rgb>>8)&255,rgb&255);
    for (unsigned char c : value) {
      if (c<32 || c>=127) continue;
      SDL_FRect src{static_cast<float>((c-32)*16),mono?18.f:0.f,mono?9.f:16.f,18.f};
      SDL_FRect dst{x,cy-9*size,src.w*size,18*size};
      SDL_RenderTexture(renderer_,font_texture_,&src,&dst);
      x+=(mono?font::mono_advance[c-32]:font::ui_advance[c-32])*size;
    }
  }
  void label(const Button &b, uint32_t rgb) {
    float cx=b.rect.x+b.rect.w/2, cy=b.rect.y+b.rect.h/2;
    std::string name=b.label;
    if(!b.icon.empty()) {
      const float top=b.rect.y+8, pedal_h=b.rect.h-40, pedal_w=std::min(36.f,b.rect.w*.45f);
      const float tilt=b.icon=="accelerator"?6.f:0.f;
      ink(rgb,220);
      SDL_FPoint outline[]={{cx-pedal_w/2+tilt,top},{cx+pedal_w/2,top},
        {cx+pedal_w/2-tilt,top+pedal_h},{cx-pedal_w/2,top+pedal_h},{cx-pedal_w/2+tilt,top}};
      SDL_RenderLines(renderer_,outline,5);
      for(int i=1;i<=3;++i) {
        float y=top+pedal_h*i/4;
        SDL_RenderLine(renderer_,cx-pedal_w/2+5,y,cx+pedal_w/2-5,y);
      }
      if(b.icon=="reverse") text("R",cx,top+pedal_h/2,rgb,.9f);
      auto split=name.find(" / ");
      if(split!=std::string::npos) {
        text(name.substr(0,split),cx,b.rect.y+b.rect.h-23,rgb,.8f);
        text(name.substr(split+3),cx,b.rect.y+b.rect.h-9,rgb,.8f);
      } else text(name,cx,b.rect.y+b.rect.h-14,rgb,std::min(.9f,(b.rect.w-8)/std::max(1.f,textWidth(name))));
      return;
    }
    if (name=="LEFT" || name=="RIGHT" || name=="UP" || name=="DOWN") {
      for (auto &c : name) c=static_cast<char>(std::tolower(c));
      name="@"+name;
    }
    const char *caption = name=="@stick"?"Joystick":name=="@mouse-left" && b.action!=6?"Left click":
      name=="@mouse-right"?"Right click":name=="@keyboard"?"Keyboard":nullptr;
    if (!name.empty() && name[0]=='@') {
      float icon_scale=(name=="@left" || name=="@right" || name=="@up" || name=="@down")?1.35f:1.f;
      drawControlIcon(renderer_,name=="@hold-drag"?"@drag":name=="@tap"?"@mouse-left":name,cx-12*icon_scale,cy-(caption?19:12*icon_scale),rgb,icon_scale);
      if (caption) text(caption,cx,cy+12,rgb,0.75f);
      return;
    }
    float advance=textWidth(name);
    text(name,cx,cy,rgb,std::min(b.action==8?1.35f:1.f,(b.rect.w-8)/std::max(1.f,advance)));
  }
  void wheel(const Pad &p) {
    const float cx=p.rect.x+p.rect.w/2,cy=p.rect.y+p.rect.h/2,radius=p.rect.w/2-3;
    const float radians=p.angle*3.14159265f/180;
    auto point=[&](float angle,float radius) {return SDL_FPoint{cx+std::cos(angle+radians)*radius,cy+std::sin(angle+radians)*radius};};
    auto ring=[&](float outer,float thickness,uint32_t color) {
      std::vector<SDL_Vertex> vertices;
      SDL_FColor tint{((color>>16)&255)/255.f,((color>>8)&255)/255.f,(color&255)/255.f,.94f};
      for(int i=0;i<64;++i) {
        float a=i*6.2831853f/64,b=(i+1)*6.2831853f/64;
        for(const auto &v:{point(a,outer),point(b,outer),point(a,outer-thickness),
                           point(b,outer),point(b,outer-thickness),point(a,outer-thickness)})
          vertices.push_back({v,tint,{0,0}});
      }
      SDL_RenderGeometry(renderer_,nullptr,vertices.data(),static_cast<int>(vertices.size()),nullptr,0);
    };
    ring(radius,18,0x101416);ring(radius-2,3,0x697478);ring(radius-7,7,0x30393d);
    ink(settings.text,190);
    for(int i=0;i<3;++i) {
      float a=(-150+i*120)*3.14159265f/180;
      auto start=point(a,12),end=point(a,radius-15);
      float dx=std::sin(a+radians)*4,dy=-std::cos(a+radians)*4;
      SDL_FColor tint{.38f,.43f,.45f,1};
      SDL_Vertex vertices[]={{{start.x+dx,start.y+dy},tint,{0,0}},{{end.x+dx,end.y+dy},tint,{0,0}},
        {{start.x-dx,start.y-dy},tint,{0,0}},{{end.x+dx,end.y+dy},tint,{0,0}},
        {{end.x-dx,end.y-dy},tint,{0,0}},{{start.x-dx,start.y-dy},tint,{0,0}}};
      SDL_RenderGeometry(renderer_,nullptr,vertices,6,nullptr,0);
    }
    ring(18,18,0x20282c);ring(18,2,0x879398);
    SDL_FColor accent{((settings.accent>>16)&255)/255.f,((settings.accent>>8)&255)/255.f,(settings.accent&255)/255.f,1};
    SDL_Vertex mark[]={{point(-1.62f,radius-1),accent,{0,0}},{point(-1.52f,radius-1),accent,{0,0}},
      {point(-1.62f,radius-17),accent,{0,0}},{point(-1.52f,radius-1),accent,{0,0}},
      {point(-1.52f,radius-17),accent,{0,0}},{point(-1.62f,radius-17),accent,{0,0}}};
    SDL_RenderGeometry(renderer_,nullptr,mark,6,nullptr,0);
  }
  void draw() {
    if (!renderer_ || paused_) return;
    layout();
    SDL_SetRenderScale(renderer_, scale_, scale_);
    color(0,0,0); SDL_RenderClear(renderer_);
    if (texture_) SDL_RenderTexture(renderer_, texture_, nullptr, &game_rect_);
    SDL_SetRenderDrawBlendMode(renderer_,SDL_BLENDMODE_BLEND);
    if (!collapsed_ && !keyboard_) for (const auto &p : pads_) {
      float cx=p.rect.x+p.rect.w/2, cy=p.rect.y+p.rect.h/2;
      if(p.config.wheel) { wheel(p);continue; }
      if (p.config.directional) {
        const auto keys=padKeys(p.config);
        for (const auto &[direction,rect] : padTargets(p)) {
          bool held=(direction==0?p.x<-.3f:direction==1?p.y<-.3f:direction==2?p.x>.3f:p.y>.3f);
          ink(held?settings.accent:settings.button,210);SDL_RenderFillRect(renderer_,&rect);
          std::string name=keys[direction];
          if(name.rfind("KP",0)==0) name="Num "+name.substr(2);
          Button target{rect,name,"",8};label(target,held?0x101010:settings.text);
        }
        continue;
      }
      ink(settings.button,200); SDL_RenderFillRect(renderer_,&p.rect);
      ink(settings.text,130);
      SDL_RenderLine(renderer_,cx-12,cy,cx+12,cy); SDL_RenderLine(renderer_,cx,cy-12,cx,cy+12);
      ink(settings.accent,220);
      SDL_FRect knob{cx-12+p.x*(p.rect.w/2-14),cy-12+p.y*(p.rect.h/2-14),24,24}; SDL_RenderFillRect(renderer_,&knob);
    }
    for (size_t i = 0; i < buttons_.size(); ++i) {
      const auto &b = buttons_[i];
      bool held = (b.action==6 && !move_only_) || std::any_of(fingers_.begin(), fingers_.end(), [i](const auto &p) { return p.second.button == static_cast<int>(i); });
      uint32_t accent=keyboard_ && !b.key.empty()?settings.keyboardAccent:settings.accent;
      ink(held?accent:settings.button,settings.layout=="overlay"?210:255); SDL_RenderFillRect(renderer_, &b.rect);
      uint32_t luminance=((accent>>16)*299+((accent>>8)&255)*587+(accent&255)*114)/1000;
      label(b,held?(luminance>140?0x101010:0xffffff):settings.text);
    }
    SDL_RenderPresent(renderer_);
  }
  void movePad(Finger &finger, float x, float y) {
    auto &p=pads_[finger.pad];
    if(p.config.wheel) {
      // Ignore the hub where pointer angle is undefined. Rim movement uses
      // incremental signed angles, so crossing +/-180 degrees stays smooth.
      float dx=x-p.rect.x-p.rect.w/2,dy=y-p.rect.y-p.rect.h/2;
      if(std::hypot(dx,dy)<p.rect.w*.075f) {finger.held=false;return;}
      float angle=std::atan2(dy,dx)*180/3.14159265f;
      if(!finger.held) {p.contactAngle=angle;finger.held=true;return;}
      float delta=angle-p.contactAngle;
      if(delta>180)delta-=360;
      if(delta< -180)delta+=360;
      p.angle=std::clamp(p.angle+delta,-p.config.maxAngle,p.config.maxAngle);
      p.contactAngle=angle;p.x=p.angle/p.config.maxAngle;p.y=0;return;
    }
    p.x=(x-p.rect.x-p.rect.w/2)/(p.rect.w/2); p.y=(y-p.rect.y-p.rect.h/2)/(p.rect.h/2);
    if(p.config.axes=="horizontal") p.y=0;
    if(p.config.axes=="vertical") p.x=0;
    float length=std::hypot(p.x,p.y); if (length>1) { p.x/=length; p.y/=length; }
  }
  void tickPads() {
    bool changed=false;Uint64 ticks=SDL_GetTicks();
    if(tap_until_ && ticks>=tap_until_) {tap_until_=0;changed=true;}
    for(auto it=mouse_action_until_.begin();it!=mouse_action_until_.end();) {
      if(ticks>=it->second) {it=mouse_action_until_.erase(it);changed=true;}
      else ++it;
    }
    if(changed)syncInput();
    if (settings.touch=="hold-drag" && !paused_ && !move_only_) {
      bool changed=false;
      for (auto &[id,f] : fingers_) if (f.game && !f.held && !f.moved && !f.blocked && SDL_GetTicks()-f.downAt>=180) {
        f.held=true;changed=true;
      }
      if (changed) syncInput();
    }
    uint64_t now=SDL_GetTicksNS();
    float elapsed=last_pad_time_?std::min((now-last_pad_time_)/1e9f,1.f/30):0;
    last_pad_time_=now;
    if (paused_ || collapsed_) return;
    for(auto &[key,deadline]:repeat_keys_) if(SDL_GetTicks()>=deadline) {
      injectKey(key.c_str(),true);deadline=SDL_GetTicks()+100;
    }
    for (auto &p : pads_) if (p.config.mode=="mouse") {
      float length=std::hypot(p.x,p.y);
      if (length<=0.15f) continue;
      float speed=std::max(frame_width_,frame_height_)*0.75f*p.config.sensitivity*(length-0.15f)/(0.85f*length);
      p.dx+=p.x*speed*elapsed; p.dy+=p.y*speed*elapsed;
      int dx=std::lround(p.dx),dy=std::lround(p.dy); p.dx-=dx;p.dy-=dy;
      if(dx || dy) injectMouse(dx,dy,mouse_);
    }
  }
  bool clearMouseAction(SDL_FingerID id) {
    bool changed=false;
    for(auto it=mouse_action_until_.begin();it!=mouse_action_until_.end();) {
      if(it->first.first==id) {it=mouse_action_until_.erase(it);changed=true;}
      else ++it;
    }
    return changed;
  }
  void finger(const SDL_TouchFingerEvent &event) {
    float x = event.x*width_, y = event.y*height_;
    if (event.type == SDL_EVENT_FINGER_DOWN) {
      // A reused contact starts a distinct press instead of inheriting the
      // released contact's remaining pulse or its original deadline.
      if(clearMouseAction(event.fingerID))syncInput();
      Finger f{x,y};
      f.originX=x; f.originY=y; f.downAt=SDL_GetTicks(); f.blocked=(mouse_ & ~(tap_hold_?1:0))!=0;
      for (size_t i = 0; i < buttons_.size(); ++i) if (inside(x,y,buttons_[i].rect)) { f.button = i; break; }
      if (f.button >= 0) {
        int action = buttons_[f.button].action;
        if (action == 1 || action == 2 || action == 5 || action == 6 || action == 7) {
          release();
          if (action == 1) keyboard_ = !keyboard_;
          if (action == 2) page_ = (page_+1)%3;
          if (action == 5) collapsed_ = !collapsed_;
          if (action == 6) move_only_ = !move_only_;
          if (action == 7) joystick_open_ = !joystick_open_;
          draw(); return;
        }
      } else {
        bool pad_hit=false;
        if (!collapsed_ && !keyboard_) for (size_t i=0;i<pads_.size();++i) if (padContains(pads_[i],x,y)) {
          pad_hit=true;
          bool owned=std::any_of(fingers_.begin(),fingers_.end(),[i](const auto &v){return v.second.pad==static_cast<int>(i);});
          if (!owned) { f.pad=static_cast<int>(i); movePad(f,x,y); }
          break;
        }
        bool owned=std::any_of(fingers_.begin(),fingers_.end(),[](const auto &v){return v.second.game;});
        f.game=!pad_hit && !owned && settings.pointer!="off" && inside(x,y,game_rect_);
        if(f.game && tap_until_) {
          tap_until_=0;syncInput();
          f.blocked=(mouse_ & ~(tap_hold_?1:0))!=0;
        }
      }
      fingers_[event.fingerID] = f;
    } else {
      bool canceled=event.type==SDL_EVENT_FINGER_CANCELED && clearMouseAction(event.fingerID);
      auto found = fingers_.find(event.fingerID);
      if (found == fingers_.end()) {if(canceled)syncInput();return;}
      auto &f=found->second;
      f.moved |= std::hypot(x-f.originX,y-f.originY)>8;
      if (event.type == SDL_EVENT_FINGER_UP || event.type == SDL_EVENT_FINGER_CANCELED) {
        if(event.type==SDL_EVENT_FINGER_UP && settings.tapDuration>0 && f.button>=0 && static_cast<size_t>(f.button)<buttons_.size()) {
          uint8_t buttons=buttonMouse(buttons_[f.button]);
          Uint64 deadline=f.downAt+settings.tapDuration;
          if(buttons && SDL_GetTicks()<deadline) {
            auto &until=mouse_action_until_[{event.fingerID,buttons}];until=std::max(until,deadline);
          }
        }
        if (event.type == SDL_EVENT_FINGER_UP && found->second.game && settings.pointer == "direct") {
          found->second.x=x; found->second.y=y;
          syncInput();
        }
        if(event.type==SDL_EVENT_FINGER_UP && f.game && (settings.touch=="tap" || (settings.touch=="hold-drag" && !f.held)) && !move_only_ && !f.moved && !f.blocked && !mouse_) {
          mouse_=1; injectMouse(0,0,mouse_);
          if(settings.tapDuration>0)tap_until_=SDL_GetTicks()+settings.tapDuration;
        }
        if(event.type==SDL_EVENT_FINGER_UP && f.game && settings.touch=="tap-hold" && !move_only_ && !f.moved && !f.blocked) {
          tap_hold_=!tap_hold_;syncInput();
        }
        if(event.type==SDL_EVENT_FINGER_CANCELED && f.game) {tap_hold_=false;tap_until_=0;}
        if (found->second.pad>=0) { auto &p=pads_[found->second.pad]; p.x=p.y=p.dx=p.dy=p.angle=p.contactAngle=0; }
        fingers_.erase(found);
      }
      else {
        if (found->second.pad>=0) movePad(found->second,x,y);
        if (found->second.game && settings.pointer != "direct" && game_rect_.w > 0) {
          auto &f = found->second;
          f.dx += (x-f.x)*frame_width_/game_rect_.w;
          f.dy += (y-f.y)*frame_height_/game_rect_.h;
          int dx = std::lround(f.dx), dy = std::lround(f.dy);
          f.dx -= dx; f.dy -= dy;
          injectMouse(dx,dy,mouse_);
        }
        found->second.x = x; found->second.y = y;
      }
    }
    syncInput();
    controls_dirty_ = !buttons_.empty() || !pads_.empty();
  }
  bool controlClick(const SDL_MouseButtonEvent &event) {
    if (event.button != SDL_BUTTON_LEFT) return false;
    constexpr SDL_FingerID pointer = static_cast<SDL_FingerID>(-1);
    int w, h; SDL_GetWindowSize(window_, &w, &h);
    float x = event.x/std::max(1,w), y = event.y/std::max(1,h);
    bool captured = fingers_.count(pointer) != 0;
    bool control = event.down && std::any_of(buttons_.begin(),buttons_.end(),[&](const Button &b) {
      return inside(x*width_,y*height_,b.rect);
    });
    if (!captured && !control) return false;
    SDL_TouchFingerEvent touch{};
    touch.type = event.down ? SDL_EVENT_FINGER_DOWN : SDL_EVENT_FINGER_UP;
    touch.fingerID = pointer; touch.x = x; touch.y = y;
    finger(touch);
    return true;
  }
public:
  bool start() override {
    move_only_ = !settings.touchClick;
    configureCursor(settings.cursor);
    // SDL updates Android's Activity orientation when creating its resizable
    // window. Keep that request consistent with the exported manifest.
    SDL_SetHint(SDL_HINT_ORIENTATIONS, settings.orientation=="landscape" ? "LandscapeLeft LandscapeRight" :
                                      settings.orientation=="portrait" ? "Portrait PortraitUpsideDown" :
                                      "LandscapeLeft LandscapeRight Portrait PortraitUpsideDown");
    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS,"0");
    SDL_SetHint(SDL_HINT_MOUSE_TOUCH_EVENTS,"0");
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) return false;
    lifecycle_event_=SDL_RegisterEvents(1);
    if (!lifecycle_event_ || !SDL_AddEventWatch(lifecycle,this)) return false;
    window_ = SDL_CreateWindow(settings.name.c_str(),settings.orientation=="landscape"?760:400,settings.orientation=="landscape"?400:760,SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY
#ifdef __ANDROID__
      | (settings.fullscreen?SDL_WINDOW_FULLSCREEN:0)
#endif
    );
    if (!window_) return false;
    renderer_ = SDL_CreateRenderer(window_,nullptr);
    if (!renderer_) return false;
    if (!loadFont()) return false;
    audio_ = createSdlAudioBackend();
    std::string error;
    if (!audio_->start(error)) { SDL_Log("Audio: %s",error.c_str()); return false; }
    SDL_DisableScreenSaver();
    draw(); return true;
  }
  void stop() override {
    if (lifecycle_event_) SDL_RemoveEventWatch(lifecycle,this);
    lifecycle_event_=0;
    last_machine_=nullptr; frame_dirty_=controls_dirty_=false;
    audio_.reset();
    if (texture_) SDL_DestroyTexture(texture_);
    if (font_texture_) SDL_DestroyTexture(font_texture_);
    font_texture_=nullptr;
    if (renderer_) SDL_DestroyRenderer(renderer_);
    if (window_) SDL_DestroyWindow(window_);
    texture_ = nullptr; renderer_ = nullptr; window_ = nullptr;
    fingers_.clear(); physical_.clear(); keys_.clear(); repeat_keys_.clear(); pads_.clear();mouse_action_until_.clear();
    toggle_layout_.clear();group_layout_.clear();
    keyboard_=collapsed_=move_only_=joystick_open_=tap_hold_=false;
    paused_ = false; mouse_ = physical_mouse_ = 0;tap_until_=0;
    SDL_Quit();
  }
  double nowMs() override { return SDL_GetTicksNS()/1000000.0; }
  void sleepMs(unsigned ms) override { if (ms) SDL_Delay(ms); }
  bool paused() override { return paused_; }
  void pump() override {
    SDL_Event e;
    int count = 0;
    while (count++ < 256 && SDL_PollEvent(&e)) {
      if (e.type==lifecycle_event_) e.type=static_cast<Uint32>(e.user.code);
      switch (e.type) {
      case SDL_EVENT_QUIT: case SDL_EVENT_TERMINATING: case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        release(); throw QuitRequested();
      case SDL_EVENT_WILL_ENTER_BACKGROUND: case SDL_EVENT_WINDOW_FOCUS_LOST:
        release(); stopAudio(); paused_ = true; break;
      case SDL_EVENT_DID_ENTER_FOREGROUND: case SDL_EVENT_WINDOW_FOCUS_GAINED:
        paused_ = false; frame_dirty_=true; break;
      case SDL_EVENT_RENDER_DEVICE_RESET:
        frame_dirty_=true;
        if (font_texture_) SDL_DestroyTexture(font_texture_);
        font_texture_=nullptr;
        if (!loadFont()) throw QuitRequested();
        if (texture_) SDL_DestroyTexture(texture_);
        texture_ = nullptr; break;
      case SDL_EVENT_RENDER_TARGETS_RESET: case SDL_EVENT_WINDOW_SHOWN: case SDL_EVENT_WINDOW_RESTORED:
        frame_dirty_=true; break;
      case SDL_EVENT_WINDOW_RESIZED: case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
        release(); draw(); break;
      case SDL_EVENT_WINDOW_EXPOSED:
        draw(); break;
      case SDL_EVENT_FINGER_DOWN: case SDL_EVENT_FINGER_MOTION: case SDL_EVENT_FINGER_UP: case SDL_EVENT_FINGER_CANCELED:
        if (!paused_) finger(e.tfinger); break;
      case SDL_EVENT_KEY_DOWN: case SDL_EVENT_KEY_UP:
        if (e.key.key == SDLK_AC_BACK) {
          injectKey("ESC",e.key.down); break;
        }
        if (e.key.down) physical_.insert(e.key.scancode); else physical_.erase(e.key.scancode);
        syncInput(); break;
      case SDL_EVENT_MOUSE_BUTTON_DOWN: case SDL_EVENT_MOUSE_BUTTON_UP:
        if (e.button.which != SDL_TOUCH_MOUSEID) {
          if (controlClick(e.button)) break;
          uint8_t bit = e.button.button == SDL_BUTTON_LEFT ? 1 : e.button.button == SDL_BUTTON_RIGHT ? 2 : 0;
          if (e.button.down) physical_mouse_ |= bit; else physical_mouse_ &= ~bit;
          syncInput();
        }
        break;
      case SDL_EVENT_MOUSE_MOTION:
        if (e.motion.which != SDL_TOUCH_MOUSEID && game_rect_.w > 0)
          injectMouse(std::lround(e.motion.xrel*frame_width_/game_rect_.w),std::lround(e.motion.yrel*frame_height_/game_rect_.h),mouse_);
        break;
      default: break;
      }
    }
    tickPads();
    if (!paused_ && frame_dirty_ && last_machine_) {
      present(*last_machine_);
#ifdef __ANDROID__
      // Surface recreation can consume the first restored presentation. A
      // second submission reuses the texture, without another guest upload.
      draw();
#endif
    }
    else if (controls_dirty_) { controls_dirty_=false; draw(); }
  }
  void present(Machine &machine) override {
    last_machine_=&machine; frame_dirty_=controls_dirty_=false;
    auto &vbe = machine.vbe(); const auto &mode = vbe.currentMode();
    SDL_PixelFormat format = mode.bytes_per_pixel == 4 ? SDL_PIXELFORMAT_BGRX32 :
                             mode.bytes_per_pixel == 3 ? SDL_PIXELFORMAT_BGR24 : SDL_PIXELFORMAT_RGB24;
    if (frame_width_ != mode.width || frame_height_ != mode.height || format_ != format) {
      release();
      if (texture_) SDL_DestroyTexture(texture_);
      texture_ = nullptr;
    }
    frame_width_ = mode.width; frame_height_ = mode.height;
    format_ = format;
    if (!texture_) texture_ = SDL_CreateTexture(renderer_,format_,SDL_TEXTUREACCESS_STREAMING,mode.width,mode.height);
    if (!texture_) { SDL_Log("Texture: %s",SDL_GetError()); throw QuitRequested(); }
    SDL_SetTextureScaleMode(texture_,SDL_SCALEMODE_NEAREST);
    const uint8_t *pixels = vbe.framebuffer().data();
    int pitch = mode.pitch;
    if (mode.bytes_per_pixel < 3) {
      rgb_.resize(static_cast<size_t>(mode.width)*mode.height*3);
      for (uint16_t y=0;y<mode.height;++y) vbe.copyRgbRow(y,rgb_.data()+static_cast<size_t>(y)*mode.width*3);
      pixels = rgb_.data(); pitch = mode.width*3;
    }
    if (!SDL_UpdateTexture(texture_,nullptr,pixels,pitch)) { SDL_Log("Frame: %s",SDL_GetError()); throw QuitRequested(); }
    draw();
  }
  void audio(const int16_t *samples,size_t frames,uint32_t rate,uint8_t channels) override {
    std::string error;
    if (!audio_->playPcm16(samples,frames,rate,channels,error)) SDL_Log("Audio: %s",error.c_str());
  }
  void stopAudio() override { if (audio_) audio_->stop(); }
  bool setClipboard(const std::string &text) override { return SDL_SetClipboardText(text.c_str()); }
  bool getClipboard(std::string &text) override {
    char *value = SDL_GetClipboardText();
    if (!value) return false;
    text = value; SDL_free(value); return true;
  }
};
}
void configureControls(const ControlConfig &config) { settings=config; }
Host &host() { static SdlHost platform; return platform; }
}
