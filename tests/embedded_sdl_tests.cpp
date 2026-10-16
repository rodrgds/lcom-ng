#include "embedded/Host.hpp"
#include "embedded/SdlHost.hpp"
#include "embedded/Controls.hpp"
#include "embedded_sdl_capture.hpp"
#include <lcom/lcom.h>
#include <lcom/vbe.h>
#include <lcom/i8042.h>
#include <SDL3/SDL.h>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#define CHECK(value) do { if (!(value)) { std::fprintf(stderr,"line %d: %s (%s)\n",__LINE__,#value,SDL_GetError()); dumpRecentSdlEvents(); std::exit(1); } } while (0)

static void touch(SDL_EventType type, SDL_FingerID id, float x, float y) {
  SDL_Event e{}; e.type = type; e.tfinger.type = type; e.tfinger.fingerID = id;
  e.tfinger.x = x; e.tfinger.y = y;
  CHECK(SDL_PushEvent(&e)); lcom::embedded::host().pump();
}
static void key(Uint32 type) {
  SDL_Event e{}; e.type=type; e.key.scancode=SDL_SCANCODE_A; e.key.down=type==SDL_EVENT_KEY_DOWN;
  CHECK(SDL_PushEvent(&e)); lcom::embedded::host().pump();
}
static uint8_t readKey() { uint8_t b=0; CHECK(lcom_port_read8(KBC_OUT_BUF,&b)==LCOM_OK); return b; }
static bool keyReady() { uint8_t status=0; CHECK(lcom_port_read8(KBC_ST_REG,&status)==LCOM_OK); return status & KBC_ST_OBF; }
static bool expectedKey(uint8_t expected) {
  uint8_t actual=readKey();
  if(actual==expected)return true;
  uint8_t status=0;CHECK(lcom_port_read8(KBC_ST_REG,&status)==LCOM_OK);
  std::fprintf(stderr,"Expected key 0x%02x, received 0x%02x, remaining controller status 0x%02x, paused=%d, pending touch=%d\n",
               expected,actual,status,lcom::embedded::host().paused(),SDL_HasEvent(SDL_EVENT_FINGER_DOWN));
  int count=0;auto windows=SDL_GetWindows(&count);
  for(int i=0;i<count;++i) {
    int w=0,h=0,pw=0,ph=0;
    SDL_GetWindowSize(windows[i],&w,&h);SDL_GetWindowSizeInPixels(windows[i],&pw,&ph);
    std::fprintf(stderr,"Window %u: %dx%d logical, %dx%d pixels, flags 0x%llx\n",
                 SDL_GetWindowID(windows[i]),w,h,pw,ph,static_cast<unsigned long long>(SDL_GetWindowFlags(windows[i])));
  }
  SDL_free(windows);return false;
}
static void positionedControls() {
  using namespace lcom::embedded;
  ControlConfig controls;controls.orientation="landscape";controls.layout="overlay";
  controls.keyboard=controls.mouseButtons=controls.joystickMenu=false;controls.pointer="off";controls.quickKeys.clear();
  controls.dpad={true,"wasd","left",1,true};controls.dpad.axes="horizontal";
  controls.actions={{"","Esc","ESC",0,"top-right"}};
  configureControls(controls);CHECK(lcom_init()==LCOM_OK);
  lcom_irq_t keyboard;CHECK(lcom_irq_subscribe(KBC_IRQ,0,&keyboard)==LCOM_OK);
  // Only the horizontal arms exist. The old square's upper region must not
  // claim this touch or deliver an unwanted vertical movement key.
  touch(SDL_EVENT_FINGER_DOWN,400,64.f/760,292.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_UP,400,64.f/760,292.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_DOWN,401,34.f/760,314.f/400);CHECK(readKey()==0x1e);
  touch(SDL_EVENT_FINGER_UP,401,34.f/760,314.f/400);CHECK(readKey()==0x9e);
  // An explicit upper-right action is reachable without reaching a bottom toolbar.
  touch(SDL_EVENT_FINGER_DOWN,402,726.f/760,34.f/400);CHECK(readKey()==0x01);
  touch(SDL_EVENT_FINGER_UP,402,726.f/760,34.f/400);CHECK(readKey()==0x81);
  int count=0;auto windows=SDL_GetWindows(&count);CHECK(count==1);
  auto frame=readPresentedFrame(SDL_GetRenderer(windows[0]));SDL_free(windows);CHECK(frame);
  Uint8 r,g,b,a;CHECK(SDL_ReadSurfacePixel(frame,660,34,&r,&g,&b,&a));CHECK(r==0 && g==0 && b==0);
  CHECK(SDL_ReadSurfacePixel(frame,86,262,&r,&g,&b,&a));CHECK(r==0 && g==0 && b==0);
  std::filesystem::create_directories("build/mobile-ux2/native");CHECK(SDL_SaveBMP(frame,"build/mobile-ux2/native/horizontal-pad.bmp"));SDL_DestroySurface(frame);
  lcom_exit();
  controls.dpad={};controls.actions.clear();controls.quickKeys={"A"};
  for(const std::string corner:{"top-left","top-right","bottom-left","bottom-right"}) {
    controls.toolbar=corner;configureControls(controls);CHECK(lcom_init()==LCOM_OK);
    CHECK(lcom_irq_subscribe(KBC_IRQ,0,&keyboard)==LCOM_OK);
    float x=corner.find("right")!=std::string::npos?726:34;
    float y=corner.find("bottom")!=std::string::npos?366:30;
    touch(SDL_EVENT_FINGER_DOWN,410,x/760,y/400);CHECK(readKey()==0x1e);
    touch(SDL_EVENT_FINGER_UP,410,x/760,y/400);CHECK(readKey()==0x9e);lcom_exit();
  }
  controls.toolbar="auto";controls.quickKeys.clear();controls.keyboard=true;
  for(const std::string position:{"left","right","bottom"}) {
    controls.keyboardPosition=position;configureControls(controls);CHECK(lcom_init()==LCOM_OK);
    CHECK(lcom_irq_subscribe(KBC_IRQ,0,&keyboard)==LCOM_OK);
    touch(SDL_EVENT_FINGER_DOWN,420,715.f/760,30.f/400);
    touch(SDL_EVENT_FINGER_UP,420,715.f/760,30.f/400);CHECK(!keyReady());
    float x=position=="left"?32:position=="right"?330:49;
    float y=position=="bottom"?318:270;
    touch(SDL_EVENT_FINGER_DOWN,421,x/760,y/400);CHECK(readKey()==0x1e);
    touch(SDL_EVENT_FINGER_UP,421,x/760,y/400);CHECK(readKey()==0x9e);
    windows=SDL_GetWindows(&count);frame=readPresentedFrame(SDL_GetRenderer(windows[0]));SDL_free(windows);CHECK(frame);
    CHECK(SDL_SaveBMP(frame,("build/mobile-ux2/native/keyboard-"+position+".bmp").c_str()));SDL_DestroySurface(frame);
    lcom_exit();
  }
  // Action-only pointer profiles retain a discoverable Move-only switch.
  controls={};controls.orientation="landscape";controls.layout="overlay";
  controls.keyboard=controls.mouseButtons=controls.joystickMenu=false;controls.quickKeys.clear();
  controls.pointer="direct";controls.touch="tap";controls.touchClick=false;controls.actions={{"","Esc","ESC",0,"bottom-right"}};
  configureControls(controls);CHECK(lcom_init()==LCOM_OK);
  lcom_irq_t mouse;CHECK(lcom_irq_subscribe(MOUSE_IRQ,0,&mouse)==LCOM_OK);
  CHECK(lcom_port_write8(KBC_CMD_REG,KBC_CMD_WRITE_MOUSE)==LCOM_OK);
  CHECK(lcom_port_write8(KBC_IN_BUF,0xf4)==LCOM_OK);CHECK(readKey()==0xfa);
  touch(SDL_EVENT_FINGER_DOWN,429,.5f,.5f);touch(SDL_EVENT_FINGER_UP,429,.5f,.5f);
  while(keyReady()) {CHECK((readKey()&1)==0);readKey();readKey();}
  touch(SDL_EVENT_FINGER_DOWN,430,715.f/760,30.f/400);
  touch(SDL_EVENT_FINGER_UP,430,715.f/760,30.f/400);
  touch(SDL_EVENT_FINGER_DOWN,431,.5f,.5f);touch(SDL_EVENT_FINGER_UP,431,.5f,.5f);
  bool clicked=false;
  while(keyReady()) {clicked|=(readKey()&1)!=0;readKey();readKey();}
  CHECK(clicked);
  lcom_exit();
  controls.touchClick=true;
  // A configured hover-only profile must not advertise a click mode switch.
  controls.touch="move";configureControls(controls);CHECK(lcom_init()==LCOM_OK);
  windows=SDL_GetWindows(&count);frame=readPresentedFrame(SDL_GetRenderer(windows[0]));SDL_free(windows);CHECK(frame);
  CHECK(SDL_ReadSurfacePixel(frame,715,30,&r,&g,&b,&a));CHECK(r==0 && g==0 && b==0);
  SDL_DestroySurface(frame);lcom_exit();
  // A toolbar that fits the actual side gutter must leave all guest pixels
  // visible. This protects game clocks as well as board edges in overlay mode.
  controls.actions.clear();controls.keyboard=true;controls.touch="hold-drag";
  controls.quickKeys={"ESC","ENTER","BACKSPACE"};
  configureControls(controls);CHECK(lcom_init()==LCOM_OK);
  lcom_vbe_mode_info_t mode;CHECK(lcom_vbe_get_mode_info(LCOM_VBE_MODE_800_600_24,&mode)==LCOM_OK);
  CHECK(lcom_vbe_set_mode(mode.mode)==LCOM_OK);uint8_t *pixels=nullptr;
  CHECK(lcom_phys_map(mode.framebuffer_phys,mode.framebuffer_size,reinterpret_cast<void **>(&pixels))==LCOM_OK);
  for(size_t i=0;i<mode.framebuffer_size;i+=3){pixels[i]=51;pixels[i+1]=34;pixels[i+2]=17;}
  windows=SDL_GetWindows(&count);CHECK(count==1);CHECK(SDL_SetWindowSize(windows[0],820,400));
  CHECK(SDL_SyncWindow(windows[0]));
  lcom::embedded::host().pump();CHECK(lcom_vbe_present()==LCOM_OK);
  frame=readPresentedFrame(SDL_GetRenderer(windows[0]));SDL_free(windows);CHECK(frame);
  if(frame->w!=820 || frame->h!=400)
    std::fprintf(stderr,"Resized renderer output: %dx%d, expected 820x400\n",frame->w,frame->h);
  CHECK(frame->w==820 && frame->h==400);
  CHECK(SDL_ReadSurfacePixel(frame,660,30,&r,&g,&b,&a));
  if(r!=17 || g!=34 || b!=51)
    std::fprintf(stderr,"Resized game pixel: RGB(%u,%u,%u), expected RGB(17,34,51)\n",r,g,b);
  CHECK(r==17 && g==34 && b==51);
  CHECK(SDL_ReadSurfacePixel(frame,700,30,&r,&g,&b,&a));CHECK(r==0 && g==0 && b==0);
  CHECK(SDL_ReadSurfacePixel(frame,750,222,&r,&g,&b,&a));CHECK(r==0 && g==0 && b==0);
  CHECK(SDL_ReadSurfacePixel(frame,780,205,&r,&g,&b,&a));CHECK(g>r && g>b);
  CHECK(SDL_SaveBMP(frame,"build/mobile-ux2/native/gutter-toolbar.bmp"));SDL_DestroySurface(frame);
  touch(SDL_EVENT_FINGER_DOWN,432,790.f/820,222.f/400);
  touch(SDL_EVENT_FINGER_UP,432,790.f/820,222.f/400);
  windows=SDL_GetWindows(&count);frame=readPresentedFrame(SDL_GetRenderer(windows[0]));SDL_free(windows);CHECK(frame);
  CHECK(SDL_ReadSurfacePixel(frame,780,205,&r,&g,&b,&a));CHECK(r==g && g==b);
  SDL_DestroySurface(frame);
  lcom_exit();configureControls({});
}
static void steeringWheelControls() {
  using namespace lcom::embedded;
  ControlConfig c;c.orientation="landscape";c.layout="overlay";c.pointer="off";
  c.keyboard=c.mouseButtons=c.joystickMenu=false;c.quickKeys.clear();
  PadConfig wheel{true,"arrows","left",1};wheel.wheel=true;wheel.axes="horizontal";
  wheel.keys={"A","","D",""};
  c.pads={wheel};
  c.actions={{"","Accelerate","W",0,"bottom-right","accelerator"},
             {"","Brake / reverse","S",0,"bottom-right","brake"}};
  configureControls(c);CHECK(lcom_init()==LCOM_OK);
  CHECK(lcom_vbe_set_mode(LCOM_VBE_MODE_800_600_24)==LCOM_OK);CHECK(lcom_vbe_present()==LCOM_OK);
  lcom_irq_t keyboard;CHECK(lcom_irq_subscribe(KBC_IRQ,0,&keyboard)==LCOM_OK);
  // Circular hit region excludes the square's corners. A rim contact starts
  // neutral, then a five-degree turn stays inside the authored dead zone.
  touch(SDL_EVENT_FINGER_DOWN,500,9.f/760,237.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_UP,500,9.f/760,237.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_DOWN,501,86.f/760,246.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_MOTION,501,92.f/760,246.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_MOTION,501,134.f/760,266.f/400);CHECK(expectedKey(0x20));CHECK(!keyReady());
  // Passing through the hub invalidates its undefined bearing. Reappearing
  // across the wheel must retain the prior steering until angular movement.
  touch(SDL_EVENT_FINGER_MOTION,501,86.f/760,314.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_MOTION,501,18.f/760,314.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_MOTION,501,86.f/760,314.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_MOTION,501,134.f/760,266.f/400);CHECK(!keyReady());
  // A second contact cannot take over a wheel already owned by this finger.
  touch(SDL_EVENT_FINGER_DOWN,502,18.f/760,314.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_MOTION,502,86.f/760,382.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_DOWN,503,634.f/760,366.f/400);CHECK(expectedKey(0x11));
  touch(SDL_EVENT_FINGER_DOWN,504,714.f/760,366.f/400);CHECK(expectedKey(0x1f));CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_MOTION,501,38.f/760,266.f/400);
  CHECK(expectedKey(0xa0));CHECK(expectedKey(0x1e));CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_CANCELED,501,38.f/760,266.f/400);CHECK(expectedKey(0x9e));CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_MOTION,502,154.f/760,314.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_UP,502,154.f/760,314.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_CANCELED,503,634.f/760,366.f/400);CHECK(expectedKey(0x91));
  touch(SDL_EVENT_FINGER_CANCELED,504,714.f/760,366.f/400);CHECK(expectedKey(0x9f));CHECK(!keyReady());
  // Crossing atan2's wrap boundary must keep a clockwise turn clockwise.
  touch(SDL_EVENT_FINGER_DOWN,505,86.f/760,246.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_MOTION,505,154.f/760,314.f/400);CHECK(expectedKey(0x20));
  touch(SDL_EVENT_FINGER_MOTION,505,86.f/760,382.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_MOTION,505,18.f/760,314.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_MOTION,505,38.f/760,266.f/400);CHECK(!keyReady());
  // At the 135-degree lock, unwinding 45 then 90 degrees centers the
  // wheel. Unbounded accumulation would incorrectly keep steering right.
  touch(SDL_EVENT_FINGER_MOTION,505,18.f/760,314.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_MOTION,505,86.f/760,382.f/400);CHECK(expectedKey(0xa0));CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_UP,505,86.f/760,382.f/400);CHECK(!keyReady());
  // Re-touch starts at neutral even on the opposite rim. Focus loss releases
  // wheel and pedal together, and restored focus never restores held input.
  touch(SDL_EVENT_FINGER_DOWN,506,18.f/760,314.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_MOTION,506,86.f/760,246.f/400);CHECK(expectedKey(0x20));
  touch(SDL_EVENT_FINGER_DOWN,507,634.f/760,366.f/400);CHECK(expectedKey(0x11));
  int count=0;auto windows=SDL_GetWindows(&count);CHECK(count==1);
  SDL_Event event{};event.type=SDL_EVENT_WINDOW_FOCUS_LOST;event.window.windowID=SDL_GetWindowID(windows[0]);
  CHECK(SDL_PushEvent(&event));host().pump();CHECK(host().paused());
  event.type=SDL_EVENT_WINDOW_FOCUS_GAINED;CHECK(SDL_PushEvent(&event));host().pump();
  CHECK(expectedKey(0xa0));CHECK(expectedKey(0x91));CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_DOWN,508,86.f/760,246.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_MOTION,508,134.f/760,266.f/400);CHECK(expectedKey(0x20));
  touch(SDL_EVENT_FINGER_DOWN,509,634.f/760,366.f/400);CHECK(expectedKey(0x11));
  event.type=SDL_EVENT_WINDOW_EXPOSED;CHECK(SDL_PushEvent(&event));host().pump();CHECK(!keyReady());
  // Hiding controls releases both contacts, then reopening starts neutral.
  touch(SDL_EVENT_FINGER_DOWN,510,30.f/760,30.f/400);
  CHECK(expectedKey(0xa0));CHECK(expectedKey(0x91));CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_UP,510,30.f/760,30.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_DOWN,511,30.f/760,30.f/400);
  touch(SDL_EVENT_FINGER_UP,511,30.f/760,30.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_DOWN,512,154.f/760,314.f/400);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_MOTION,512,86.f/760,246.f/400);CHECK(expectedKey(0x1e));
  event.type=SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED;CHECK(SDL_PushEvent(&event));host().pump();
  CHECK(expectedKey(0x9e));CHECK(!keyReady());SDL_free(windows);
  lcom_exit();configureControls({});
}
static void mouseActionTapDuration() {
  using namespace lcom::embedded;
  ControlConfig c;c.orientation="landscape";c.layout="overlay";c.pointer="off";
  c.keyboard=c.mouseButtons=c.joystickMenu=false;c.quickKeys.clear();c.tapDuration=200;
  c.actions={{"","Flip","",1,"bottom-right"},{"","Flag","",2,"bottom-right"},{"","Middle","",4,"bottom-right"}};
  uint8_t buttons=0;std::vector<uint8_t> transitions;
  auto init=[&] {
    configureControls(c);CHECK(lcom_init()==LCOM_OK);
    lcom_irq_t mouse;CHECK(lcom_irq_subscribe(MOUSE_IRQ,0,&mouse)==LCOM_OK);
    CHECK(lcom_port_write8(KBC_CMD_REG,KBC_CMD_WRITE_MOUSE)==LCOM_OK);
    CHECK(lcom_port_write8(KBC_IN_BUF,0xf4)==LCOM_OK);CHECK(readKey()==0xfa);buttons=0;
  };
  auto drain=[&] {
    while(keyReady()) {
      uint8_t status=0;CHECK(lcom_port_read8(KBC_ST_REG,&status)==LCOM_OK);CHECK(status&KBC_ST_AUX);
      uint8_t flags=readKey();CHECK((flags&0xc8)==8);CHECK(readKey()==0);CHECK(readKey()==0);buttons=flags&7;transitions.push_back(buttons);
    }
  };
  auto contact=[&](SDL_EventType type,SDL_FingerID id,float x,float y) {
    touch(type,id,x/760,y/400);drain();
  };
  auto physical=[&](Uint32 type) {
    SDL_Event e{};e.type=type;e.button.button=SDL_BUTTON_LEFT;e.button.down=type==SDL_EVENT_MOUSE_BUTTON_DOWN;
    e.button.x=380;e.button.y=200;CHECK(SDL_PushEvent(&e));host().pump();drain();
  };
  init();
  for(int index=0;index<3;++index) {
    float x=index==1?714:634,y=index==2?366:310;uint8_t bit=1<<index;
    Uint64 beforeDown=SDL_GetTicks();contact(SDL_EVENT_FINGER_DOWN,600,x,y);CHECK(buttons==bit);
    // A released tap remains down until its ORIGINAL press deadline, not
    // a fresh interval counted from release.
    SDL_Delay(c.tapDuration/2);
    contact(SDL_EVENT_FINGER_UP,600,x,y);
    if(SDL_GetTicks()-beforeDown<c.tapDuration)CHECK(buttons==bit);
    SDL_Delay(c.tapDuration/2+10);host().pump();drain();CHECK(buttons==0);
    // A long press must not start a second minimum-duration interval on up.
    contact(SDL_EVENT_FINGER_DOWN,601,x,y);CHECK(buttons==bit);
    SDL_Delay(c.tapDuration+10);contact(SDL_EVENT_FINGER_UP,601,x,y);CHECK(buttons==0);
  }
  // Releasing/canceling one owner cannot drop another owner's held button.
  contact(SDL_EVENT_FINGER_DOWN,602,634,310);CHECK(buttons==1);
  contact(SDL_EVENT_FINGER_DOWN,603,634,310);
  contact(SDL_EVENT_FINGER_CANCELED,602,634,310);CHECK(buttons==1);
  contact(SDL_EVENT_FINGER_UP,603,634,310);
  SDL_Delay(c.tapDuration+10);host().pump();drain();CHECK(buttons==0);
  physical(SDL_EVENT_MOUSE_BUTTON_DOWN);CHECK(buttons==1);
  contact(SDL_EVENT_FINGER_DOWN,604,634,310);contact(SDL_EVENT_FINGER_UP,604,634,310);
  SDL_Delay(c.tapDuration+10);host().pump();drain();CHECK(buttons==1);
  physical(SDL_EVENT_MOUSE_BUTTON_UP);CHECK(buttons==0);
  // Canceling a pulse whose contact already ended clears only that owner.
  physical(SDL_EVENT_MOUSE_BUTTON_DOWN);
  Uint64 cancelDown=SDL_GetTicks();
  contact(SDL_EVENT_FINGER_DOWN,605,714,310);contact(SDL_EVENT_FINGER_UP,605,714,310);
  if(SDL_GetTicks()-cancelDown<c.tapDuration)CHECK(buttons==3);
  contact(SDL_EVENT_FINGER_CANCELED,605,714,310);CHECK(buttons==1);
  physical(SDL_EVENT_MOUSE_BUTTON_UP);CHECK(buttons==0);
  contact(SDL_EVENT_FINGER_DOWN,606,634,310);contact(SDL_EVENT_FINGER_CANCELED,606,634,310);CHECK(buttons==0);
  // Reusing a touch ID starts a distinct press, even inside a previous
  // minimum interval. Its superseded timer must not release the new hold.
  for(int index=0;index<2;++index) {
    contact(SDL_EVENT_FINGER_DOWN,612,634,310);contact(SDL_EVENT_FINGER_UP,612,634,310);
    bool pending=buttons==1;transitions.clear();
    float x=index==0?634:714;uint8_t bit=index==0?1:2;
    contact(SDL_EVENT_FINGER_DOWN,612,x,310);
    CHECK(transitions==(pending?std::vector<uint8_t>{0,bit}:std::vector<uint8_t>{bit}));
    SDL_Delay(c.tapDuration+10);host().pump();drain();CHECK(buttons==bit);
    contact(SDL_EVENT_FINGER_CANCELED,612,x,310);CHECK(buttons==0);
  }
  // App lifecycle and control disclosure cancel pending pulses immediately.
  contact(SDL_EVENT_FINGER_DOWN,607,634,310);contact(SDL_EVENT_FINGER_UP,607,634,310);
  SDL_Event focus{};focus.type=SDL_EVENT_WINDOW_FOCUS_LOST;CHECK(SDL_PushEvent(&focus));host().pump();CHECK(host().paused());
  focus.type=SDL_EVENT_WINDOW_FOCUS_GAINED;CHECK(SDL_PushEvent(&focus));host().pump();drain();CHECK(buttons==0);
  contact(SDL_EVENT_FINGER_DOWN,608,634,310);contact(SDL_EVENT_FINGER_UP,608,634,310);
  contact(SDL_EVENT_FINGER_DOWN,609,30,30);CHECK(buttons==0);contact(SDL_EVENT_FINGER_UP,609,30,30);
  lcom_exit();init();host().pump();drain();CHECK(buttons==0);
  contact(SDL_EVENT_FINGER_DOWN,610,634,310);contact(SDL_EVENT_FINGER_UP,610,634,310);lcom_exit();
  init();SDL_Delay(c.tapDuration+10);host().pump();drain();CHECK(buttons==0);lcom_exit();
  c.tapDuration=0;init();contact(SDL_EVENT_FINGER_DOWN,611,634,310);CHECK(buttons==1);
  contact(SDL_EVENT_FINGER_UP,611,634,310);CHECK(buttons==0);lcom_exit();configureControls({});
}
int main() {
  mouseActionTapDuration();
  steeringWheelControls();
  positionedControls();
  CHECK(lcom_init()==LCOM_OK);
  lcom_irq_t keyboard;
  CHECK(lcom_irq_subscribe(KBC_IRQ,0,&keyboard)==LCOM_OK);
  lcom::embedded::host().pump();
  // The keyboard disclosure changes layout without generating game input.
  SDL_Event click{}; click.type=SDL_EVENT_MOUSE_BUTTON_DOWN;
  click.button.button=SDL_BUTTON_LEFT; click.button.down=true;
  click.button.x=344; click.button.y=630;
  CHECK(SDL_PushEvent(&click)); lcom::embedded::host().pump();
  click.type=SDL_EVENT_MOUSE_BUTTON_UP; click.button.down=false;
  CHECK(SDL_PushEvent(&click)); lcom::embedded::host().pump();
  CHECK(!keyReady());
  // A finger and a physical key own the same guest key independently.
  touch(SDL_EVENT_FINGER_DOWN,2,0.06f,0.89f);
  CHECK(readKey()==0x1e);
  key(SDL_EVENT_KEY_DOWN); CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_UP,2,0.06f,0.89f); CHECK(!keyReady());
  key(SDL_EVENT_KEY_UP); CHECK(readKey()==0x9e);
  key(SDL_EVENT_KEY_DOWN); CHECK(readKey()==0x1e);
  SDL_Event focus{}; focus.type=SDL_EVENT_WINDOW_FOCUS_LOST;
  CHECK(SDL_PushEvent(&focus)); lcom::embedded::host().pump();
  CHECK(lcom::embedded::host().paused());
  focus.type=SDL_EVENT_WINDOW_FOCUS_GAINED;
  CHECK(SDL_PushEvent(&focus)); lcom::embedded::host().pump();
  CHECK(!lcom::embedded::host().paused()); CHECK(readKey()==0x9e);
  lcom_vbe_mode_info_t mode;
  CHECK(lcom_vbe_get_mode_info(LCOM_VBE_MODE_800_600_24,&mode)==LCOM_OK);
  CHECK(lcom_vbe_set_mode(mode.mode)==LCOM_OK);
  uint8_t *pixels=nullptr;
  CHECK(lcom_phys_map(mode.framebuffer_phys,mode.framebuffer_size,reinterpret_cast<void **>(&pixels))==LCOM_OK);
  for (size_t i=0;i<mode.framebuffer_size;i+=3) { pixels[i]=51; pixels[i+1]=34; pixels[i+2]=17; }
  CHECK(lcom_vbe_present()==LCOM_OK);
  int count=0; SDL_Window **windows=SDL_GetWindows(&count);
  CHECK(windows && count==1);
  SDL_Window *window=windows[0]; SDL_free(windows);
  SDL_Surface *frame=readPresentedFrame(SDL_GetRenderer(window));
  CHECK(frame);
  Uint8 r=0,g=0,b=0,a=0;
  CHECK(SDL_ReadSurfacePixel(frame,200,180,&r,&g,&b,&a));
  CHECK(r==17 && g==34 && b==51);
  SDL_Renderer *renderer=SDL_GetRenderer(window);
  // Android can replace the display surface while the guest keeps the same
  // framebuffer. Resume must restore that frame without a guest repaint.
  CHECK(SDL_SetRenderDrawColor(renderer,0,0,0,255));
  CHECK(SDL_RenderClear(renderer)); CHECK(SDL_RenderPresent(renderer));
  SDL_Event foreground{}; foreground.type=SDL_EVENT_DID_ENTER_FOREGROUND;
  CHECK(SDL_PushEvent(&foreground));
  // Android's app events reach watchers without entering the polled queue.
  SDL_FlushEvent(SDL_EVENT_DID_ENTER_FOREGROUND);
  lcom::embedded::host().pump();
  SDL_Surface *restored=readPresentedFrame(renderer);
  CHECK(restored); CHECK(SDL_ReadSurfacePixel(restored,200,180,&r,&g,&b,&a));
  CHECK(r==17 && g==34 && b==51); SDL_DestroySurface(restored);
  std::filesystem::create_directories("build/android-qa");
  CHECK(SDL_SaveBMP(frame,"build/android-qa/native-player.bmp")); SDL_DestroySurface(frame);
  lcom_irq_t mouse;
  CHECK(lcom_irq_subscribe(MOUSE_IRQ,0,&mouse)==LCOM_OK);
  CHECK(lcom_port_write8(KBC_CMD_REG,KBC_CMD_WRITE_MOUSE)==LCOM_OK);
  CHECK(lcom_port_write8(KBC_IN_BUF,0xf4)==LCOM_OK);
  CHECK(readKey()==0xfa);
  // Relative touch starts without cursor motion, then maps screen Y to PS/2 Y.
  touch(SDL_EVENT_FINGER_DOWN,5,0.5f,0.3f);
  CHECK((readKey() & 3)==1); CHECK(readKey()==0); CHECK(readKey()==0);
  touch(SDL_EVENT_FINGER_MOTION,5,0.51f,0.31f);
  CHECK((readKey() & 3)==1); CHECK(readKey()==8); CHECK(readKey()==0xf0);
  touch(SDL_EVENT_FINGER_DOWN,6,0.36f,0.7f);
  CHECK((readKey() & 3)==2); CHECK(readKey()==0); CHECK(readKey()==0);
  touch(SDL_EVENT_FINGER_UP,6,0.36f,0.7f);
  CHECK((readKey() & 3)==1); CHECK(readKey()==0); CHECK(readKey()==0);
  touch(SDL_EVENT_FINGER_CANCELED,5,0.51f,0.31f);
  CHECK((readKey() & 3)==0); CHECK(readKey()==0); CHECK(readKey()==0);
  SDL_Event quit{}; quit.type=SDL_EVENT_QUIT; CHECK(SDL_PushEvent(&quit));
  bool stopped=false;
  try { SDL_Delay(10); uint16_t value; lcom_port_read16(KBC_ST_REG,&value); } catch(const lcom::embedded::QuitRequested &) { stopped=true; }
  CHECK(stopped); lcom_exit();
  // Creator-configured movement and mouse aiming own separate fingers.
  lcom::embedded::ControlConfig controls;
  controls.name="Configured native app"; controls.layout="overlay";
  controls.keyboard=true; controls.mouseButtons=false; controls.joystickMenu=false;
  controls.quickKeys.clear(); controls.pointer="off";
  controls.joystick={true,"wasd","left",1}; controls.look={true,"mouse","right",1};
  controls.font="mono";
  lcom::embedded::configureControls(controls);
  CHECK(lcom_init()==LCOM_OK);
  CHECK(lcom_irq_subscribe(KBC_IRQ,0,&keyboard)==LCOM_OK);
  CHECK(lcom_irq_subscribe(MOUSE_IRQ,0,&mouse)==LCOM_OK);
  CHECK(lcom_port_write8(KBC_CMD_REG,KBC_CMD_WRITE_MOUSE)==LCOM_OK);
  CHECK(lcom_port_write8(KBC_IN_BUF,0xf4)==LCOM_OK); CHECK(readKey()==0xfa);
  touch(SDL_EVENT_FINGER_DOWN,10,0.07f,0.916f);
  CHECK(readKey()==0x1e);
  touch(SDL_EVENT_FINGER_DOWN,11,0.95f,0.87f);
  SDL_Delay(30); lcom::embedded::host().pump();
  CHECK(keyReady());
  auto mouseByte=[] {
    uint8_t status=0;CHECK(lcom_port_read8(KBC_ST_REG,&status)==LCOM_OK);
    CHECK((status&(KBC_ST_OBF|KBC_ST_AUX))==(KBC_ST_OBF|KBC_ST_AUX));
    return readKey();
  };
  int travelX=0,travelY=0;
  auto sampleLookPacket=[&] {
    uint8_t flags=mouseByte(),dx=mouseByte(),dy=mouseByte();
    CHECK((flags&0xc7)==0 && (flags&8)!=0);
    int x=int(dx)-((flags&0x10)?256:0),y=int(dy)-((flags&0x20)?256:0);
    CHECK(x>=0 && y>=0);
    travelX+=x;travelY+=y;
  };
  // An early fractional sample can move just one axis. The held stick must
  // produce positive cumulative travel on both axes, independent of host speed.
  for(int packet=0;packet<16 && keyReady();++packet)sampleLookPacket();
  CHECK(travelX>0 && travelY>0);
  touch(SDL_EVENT_FINGER_CANCELED,11,0.95f,0.87f);
  int pendingPackets=0;
  while(keyReady()) {CHECK(++pendingPackets<=16);sampleLookPacket();}
  touch(SDL_EVENT_FINGER_CANCELED,10,0.07f,0.916f);
  CHECK(readKey()==0x9e);
  SDL_Delay(30); lcom::embedded::host().pump(); CHECK(!keyReady());
  // Expanding the overlay keyboard keeps its last rows on screen and hides pads.
  touch(SDL_EVENT_FINGER_DOWN,12,0.85f,0.04f);
  touch(SDL_EVENT_FINGER_UP,12,0.85f,0.04f);
  touch(SDL_EVENT_FINGER_DOWN,13,0.17f,0.956f);
  CHECK(readKey()==0x2c); CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_UP,13,0.17f,0.956f); CHECK(readKey()==0xac);
  lcom_exit();
  // Route real normalized SDL contacts through a letterboxed landscape game.
  // Its 800x600 image must use all 400px of height, with 4:3 pillarboxing.
  controls={}; controls.orientation="landscape"; controls.layout="overlay";
  controls.keyboard=controls.mouseButtons=controls.joystickMenu=false;
  controls.quickKeys.clear(); controls.pointer="direct";
  lcom::embedded::configureControls(controls);
  CHECK(lcom_init()==LCOM_OK);
  CHECK(lcom_vbe_set_mode(LCOM_VBE_MODE_800_600_24)==LCOM_OK);
  CHECK(lcom_phys_map(mode.framebuffer_phys,mode.framebuffer_size,reinterpret_cast<void **>(&pixels))==LCOM_OK);
  for (size_t i=0;i<mode.framebuffer_size;i+=3) { pixels[i]=51; pixels[i+1]=34; pixels[i+2]=17; }
  CHECK(lcom_vbe_present()==LCOM_OK);
  windows=SDL_GetWindows(&count); CHECK(windows && count==1);
  window=windows[0]; SDL_free(windows);
  frame=readPresentedFrame(SDL_GetRenderer(window)); CHECK(frame);
  for (auto point : {SDL_Point{380,0},SDL_Point{380,399},SDL_Point{115,200},SDL_Point{644,200}}) {
    CHECK(SDL_ReadSurfacePixel(frame,point.x,point.y,&r,&g,&b,&a));
    CHECK(r==17 && g==34 && b==51);
  }
  for (int x : {110,650}) {
    CHECK(SDL_ReadSurfacePixel(frame,x,200,&r,&g,&b,&a)); CHECK(r==0 && g==0 && b==0);
  }
  SDL_DestroySurface(frame);
  CHECK(lcom_irq_subscribe(MOUSE_IRQ,0,&mouse)==LCOM_OK);
  CHECK(lcom_port_write8(KBC_CMD_REG,KBC_CMD_WRITE_MOUSE)==LCOM_OK);
  CHECK(lcom_port_write8(KBC_IN_BUF,0xf4)==LCOM_OK); CHECK(readKey()==0xfa);
  int cx=400,cy=300;
  auto drain=[&](int press_x,int press_y) {
    int count=0; bool pressed=false;
    while (keyReady()) {
      CHECK(++count<30);
      uint8_t flags=readKey(),dx=readKey(),dy=readKey();
      cx=std::clamp(cx+int(dx)-((flags&16)?256:0),0,799);
      cy=std::clamp(cy-int(dy)+((flags&32)?256:0),0,599);
      if (flags&1) { CHECK(cx==press_x && cy==press_y); pressed=true; }
    }
    return pressed;
  };
  touch(SDL_EVENT_FINGER_DOWN,20,(113.333333f+80*2.f/3)/760,100.f/400);
  CHECK(drain(80,150) && cx==80 && cy==150);
  touch(SDL_EVENT_FINGER_DOWN,21,0.7f,0.7f);
  CHECK(!keyReady()); // Another game-area finger cannot steal the cursor.
  touch(SDL_EVENT_FINGER_CANCELED,21,0.7f,0.7f);
  CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_UP,20,(113.333333f+80*2.f/3)/760,100.f/400);
  CHECK(!drain(80,150));
  touch(SDL_EVENT_FINGER_DOWN,22,0.05f,0.5f);
  CHECK(!keyReady()); // Letterboxing is not game input.
  touch(SDL_EVENT_FINGER_CANCELED,22,0.05f,0.5f);
  lcom_exit();
  // Chess-style touch hovers during contact and clicks only a stationary release.
  controls.touch="tap";
  lcom::embedded::configureControls(controls);
  CHECK(lcom_init()==LCOM_OK);
  CHECK(lcom_irq_subscribe(MOUSE_IRQ,0,&mouse)==LCOM_OK);
  CHECK(lcom_port_write8(KBC_CMD_REG,KBC_CMD_WRITE_MOUSE)==LCOM_OK);
  CHECK(lcom_port_write8(KBC_IN_BUF,0xf4)==LCOM_OK); CHECK(readKey()==0xfa);
  cx=400;cy=300;
  touch(SDL_EVENT_FINGER_DOWN,23,(113.333333f+80*2.f/3)/760,100.f/400);
  CHECK(!drain(-1,-1) && cx==80 && cy==150);
  touch(SDL_EVENT_FINGER_UP,23,(113.333333f+80*2.f/3)/760,100.f/400);
  CHECK(drain(80,150));
  touch(SDL_EVENT_FINGER_DOWN,24,0.5f,0.5f); CHECK(!drain(-1,-1));
  touch(SDL_EVENT_FINGER_MOTION,24,0.6f,0.5f); CHECK(!drain(-1,-1));
  touch(SDL_EVENT_FINGER_MOTION,24,0.5f,0.5f); CHECK(!drain(-1,-1));
  touch(SDL_EVENT_FINGER_UP,24,0.5f,0.5f); CHECK(!drain(-1,-1));
  touch(SDL_EVENT_FINGER_DOWN,25,0.5f,0.5f); CHECK(!drain(-1,-1));
  touch(SDL_EVENT_FINGER_CANCELED,25,0.5f,0.5f); CHECK(!drain(-1,-1));
  lcom_exit();
  controls.touch="tap-hold";
  lcom::embedded::configureControls(controls);
  CHECK(lcom_init()==LCOM_OK);CHECK(lcom_irq_subscribe(MOUSE_IRQ,0,&mouse)==LCOM_OK);
  CHECK(lcom_port_write8(KBC_CMD_REG,KBC_CMD_WRITE_MOUSE)==LCOM_OK);
  CHECK(lcom_port_write8(KBC_IN_BUF,0xf4)==LCOM_OK);CHECK(readKey()==0xfa);
  cx=400;cy=300;
  touch(SDL_EVENT_FINGER_DOWN,26,(113.333333f+80*2.f/3)/760,100.f/400);CHECK(!drain(-1,-1));
  touch(SDL_EVENT_FINGER_UP,26,(113.333333f+80*2.f/3)/760,100.f/400);CHECK(drain(80,150));
  uint8_t last_buttons=1;
  auto heldTravel=[&] {
    while(keyReady()) {
      uint8_t flags=readKey(),dx=readKey(),dy=readKey();last_buttons=flags&7;
      cx+=int(dx)-((flags&16)?256:0);cy-=int(dy)-((flags&32)?256:0);
    }
  };
  // SDL_Delay is a minimum wait. Scheduler pauses and rendering can cross
  // a gesture deadline, so bracket both its start and the observed packets.
  auto timedButtons=[&](Uint64 startMin,Uint64 startMax,Uint64 duration,
                        uint8_t initial,uint8_t elapsed) {
    Uint64 before=SDL_GetTicks();
    lcom::embedded::host().pump();heldTravel();
    Uint64 after=SDL_GetTicks();
    if(after-startMin<duration) CHECK(last_buttons==initial);
    else if(before-startMax>=duration) CHECK(last_buttons==elapsed);
  };
  touch(SDL_EVENT_FINGER_DOWN,27,0.5f,0.5f);heldTravel();CHECK(last_buttons==1 && cx==400 && cy==300);
  touch(SDL_EVENT_FINGER_UP,27,0.5f,0.5f);heldTravel();CHECK(last_buttons==0);
  touch(SDL_EVENT_FINGER_DOWN,28,0.5f,0.5f);CHECK(!drain(-1,-1));
  touch(SDL_EVENT_FINGER_UP,28,0.5f,0.5f);CHECK(drain(400,300));
  touch(SDL_EVENT_FINGER_DOWN,29,0.5f,0.5f);heldTravel();
  touch(SDL_EVENT_FINGER_CANCELED,29,0.5f,0.5f);heldTravel();CHECK(last_buttons==0);
  touch(SDL_EVENT_FINGER_DOWN,290,0.5f,0.5f);CHECK(!drain(-1,-1));
  touch(SDL_EVENT_FINGER_UP,290,0.5f,0.5f);CHECK(drain(400,300));
  // Right click cancels the touch latch, so its release cannot reselect a piece.
  click.type=SDL_EVENT_MOUSE_BUTTON_DOWN;click.button.button=SDL_BUTTON_RIGHT;click.button.down=true;
  CHECK(SDL_PushEvent(&click));lcom::embedded::host().pump();heldTravel();CHECK(last_buttons==2);
  click.type=SDL_EVENT_MOUSE_BUTTON_UP;click.button.down=false;
  CHECK(SDL_PushEvent(&click));lcom::embedded::host().pump();heldTravel();CHECK(last_buttons==0);
  lcom_exit();
  // Hold-drag starts as hover, presses only after dwelling, and releases
  // with this contact. Moving before the dwell never turns into a drag.
  controls.touch="hold-drag";
  lcom::embedded::configureControls(controls);
  CHECK(lcom_init()==LCOM_OK);CHECK(lcom_irq_subscribe(MOUSE_IRQ,0,&mouse)==LCOM_OK);
  CHECK(lcom_port_write8(KBC_CMD_REG,KBC_CMD_WRITE_MOUSE)==LCOM_OK);
  CHECK(lcom_port_write8(KBC_IN_BUF,0xf4)==LCOM_OK);CHECK(readKey()==0xfa);
  cx=400;cy=300;last_buttons=0;
  Uint64 dwellStartMin=SDL_GetTicks();
  touch(SDL_EVENT_FINGER_DOWN,300,0.5f,0.5f);
  Uint64 dwellStartMax=SDL_GetTicks();heldTravel();CHECK(last_buttons==0);
  SDL_Delay(90);timedButtons(dwellStartMin,dwellStartMax,180,0,1);
  SDL_Delay(110);lcom::embedded::host().pump();heldTravel();CHECK(last_buttons==1);
  touch(SDL_EVENT_FINGER_MOTION,300,0.6f,0.5f);heldTravel();CHECK(last_buttons==1 && cx>400);
  touch(SDL_EVENT_FINGER_UP,300,0.6f,0.5f);heldTravel();CHECK(last_buttons==0);
  touch(SDL_EVENT_FINGER_DOWN,301,0.5f,0.5f);heldTravel();CHECK(last_buttons==0);
  touch(SDL_EVENT_FINGER_MOTION,301,0.6f,0.5f);heldTravel();CHECK(last_buttons==0);
  SDL_Delay(200);lcom::embedded::host().pump();heldTravel();CHECK(last_buttons==0);
  touch(SDL_EVENT_FINGER_UP,301,0.6f,0.5f);CHECK(!drain(-1,-1));
  touch(SDL_EVENT_FINGER_DOWN,302,0.5f,0.5f);CHECK(!drain(-1,-1));
  touch(SDL_EVENT_FINGER_UP,302,0.5f,0.5f);CHECK(drain(400,300));
  touch(SDL_EVENT_FINGER_DOWN,303,0.5f,0.5f);heldTravel();
  SDL_Delay(200);lcom::embedded::host().pump();heldTravel();CHECK(last_buttons==1);
  touch(SDL_EVENT_FINGER_CANCELED,303,0.5f,0.5f);heldTravel();CHECK(last_buttons==0);
  lcom_exit();
  // Timer-polled student loops can request a minimum quick-tap pulse. It
  // remains down on a later pump, then releases without another contact.
  for(const std::string policy:{"tap","hold-drag"}) {
  controls.touch=policy;controls.tapDuration=60;
  lcom::embedded::configureControls(controls);
  CHECK(lcom_init()==LCOM_OK);CHECK(lcom_irq_subscribe(MOUSE_IRQ,0,&mouse)==LCOM_OK);
  CHECK(lcom_port_write8(KBC_CMD_REG,KBC_CMD_WRITE_MOUSE)==LCOM_OK);
  CHECK(lcom_port_write8(KBC_IN_BUF,0xf4)==LCOM_OK);CHECK(readKey()==0xfa);
  cx=400;cy=300;last_buttons=0;
  touch(SDL_EVENT_FINGER_DOWN,310,0.5f,0.5f);heldTravel();CHECK(last_buttons==0);
  Uint64 tapStartMin=SDL_GetTicks();
  touch(SDL_EVENT_FINGER_UP,310,0.5f,0.5f);
  Uint64 tapStartMax=SDL_GetTicks();heldTravel();CHECK(last_buttons==1);
  SDL_Delay(20);timedButtons(tapStartMin,tapStartMax,controls.tapDuration,1,0);
  SDL_Delay(60);lcom::embedded::host().pump();heldTravel();CHECK(last_buttons==0);
  touch(SDL_EVENT_FINGER_DOWN,312,0.5f,0.5f);heldTravel();
  touch(SDL_EVENT_FINGER_UP,312,0.5f,0.5f);heldTravel();CHECK(last_buttons==1);
  touch(SDL_EVENT_FINGER_DOWN,313,0.6f,0.5f);heldTravel();CHECK(last_buttons==0);
  touch(SDL_EVENT_FINGER_MOTION,313,0.65f,0.5f);heldTravel();CHECK(last_buttons==0);
  touch(SDL_EVENT_FINGER_CANCELED,313,0.65f,0.5f);heldTravel();CHECK(last_buttons==0);
  touch(SDL_EVENT_FINGER_DOWN,311,0.5f,0.5f);heldTravel();
  touch(SDL_EVENT_FINGER_UP,311,0.5f,0.5f);heldTravel();CHECK(last_buttons==1);
  focus.type=SDL_EVENT_WINDOW_FOCUS_LOST;CHECK(SDL_PushEvent(&focus));lcom::embedded::host().pump();CHECK(lcom::embedded::host().paused());
  focus.type=SDL_EVENT_WINDOW_FOCUS_GAINED;CHECK(SDL_PushEvent(&focus));lcom::embedded::host().pump();heldTravel();CHECK(last_buttons==0);
  SDL_Delay(80);lcom::embedded::host().pump();heldTravel();CHECK(last_buttons==0);
  lcom_exit();
  }
  // Authored action-only profiles show only their keys. A ten-key profile
  // wraps into rows with usable targets instead of a single crowded strip.
  controls={}; controls.orientation="landscape"; controls.layout="overlay";
  controls.keyboard=controls.mouseButtons=controls.joystickMenu=false;
  controls.pointer="off"; controls.quickKeys={"A","B"};
  lcom::embedded::configureControls(controls);
  CHECK(lcom_init()==LCOM_OK); CHECK(lcom_irq_subscribe(KBC_IRQ,0,&keyboard)==LCOM_OK);
  touch(SDL_EVENT_FINGER_DOWN,30,726.f/760,30.f/400);
  CHECK(keyReady() && readKey()==0x1e);
  touch(SDL_EVENT_FINGER_UP,30,726.f/760,30.f/400); CHECK(readKey()==0x9e);
  touch(SDL_EVENT_FINGER_DOWN,31,300.f/760,300.f/400); CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_UP,31,300.f/760,300.f/400); CHECK(!keyReady());
  lcom_exit();
  controls.quickKeys={"A","B","C","D","E","F","G","H","I","J"};
  lcom::embedded::configureControls(controls);
  CHECK(lcom_init()==LCOM_OK); CHECK(lcom_irq_subscribe(KBC_IRQ,0,&keyboard)==LCOM_OK);
  // The final key occupies the bottom row and has room to hit away from center.
  touch(SDL_EVENT_FINGER_DOWN,32,726.f/760,126.f/400);
  CHECK(keyReady() && readKey()==0x24);
  touch(SDL_EVENT_FINGER_UP,32,726.f/760,126.f/400); CHECK(readKey()==0xa4);
  lcom_exit();
  // Two players can hold different pads and their own action buttons together.
  controls.quickKeys.clear();
  controls.pads={{true,"wasd","left",1,true},{true,"keypad","right",1,true}};
  controls.actions={{"left","P1 Bomb","SPACE",0},{"right","P2 Bomb","KP0",0}};
  lcom::embedded::configureControls(controls);
  CHECK(lcom_init()==LCOM_OK);CHECK(lcom_irq_subscribe(KBC_IRQ,0,&keyboard)==LCOM_OK);
  touch(SDL_EVENT_FINGER_DOWN,40,86.f/760,262.f/400);CHECK(expectedKey(0x11));
  // Exposing an unchanged window repaints it without releasing held controls.
  windows=SDL_GetWindows(&count);CHECK(count==1);
  SDL_Event exposed{};exposed.type=SDL_EVENT_WINDOW_EXPOSED;exposed.window.windowID=SDL_GetWindowID(windows[0]);
  SDL_free(windows);CHECK(SDL_PushEvent(&exposed));lcom::embedded::host().pump();CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_DOWN,41,674.f/760,262.f/400);CHECK(expectedKey(0x48));CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_DOWN,42,86.f/760,210.f/400);CHECK(expectedKey(0x39));
  touch(SDL_EVENT_FINGER_DOWN,43,674.f/760,210.f/400);CHECK(expectedKey(0x52));CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_CANCELED,40,86.f/760,262.f/400);CHECK(expectedKey(0x91));
  touch(SDL_EVENT_FINGER_CANCELED,41,674.f/760,262.f/400);CHECK(expectedKey(0xc8));
  touch(SDL_EVENT_FINGER_CANCELED,42,86.f/760,210.f/400);CHECK(expectedKey(0xb9));
  touch(SDL_EVENT_FINGER_CANCELED,43,674.f/760,210.f/400);CHECK(expectedKey(0xd2));CHECK(!keyReady());
  lcom_exit();
  controls.repeatKeys={"W"};
  lcom::embedded::configureControls(controls);
  CHECK(lcom_init()==LCOM_OK);CHECK(lcom_irq_subscribe(KBC_IRQ,0,&keyboard)==LCOM_OK);
  touch(SDL_EVENT_FINGER_DOWN,44,86.f/760,262.f/400);CHECK(readKey()==0x11);
  SDL_Delay(370);lcom::embedded::host().pump();CHECK(keyReady() && readKey()==0x11);CHECK(!keyReady());
  SDL_Delay(110);lcom::embedded::host().pump();CHECK(keyReady() && readKey()==0x11);CHECK(!keyReady());
  touch(SDL_EVENT_FINGER_CANCELED,44,86.f/760,262.f/400);CHECK(readKey()==0x91);
  SDL_Delay(110);lcom::embedded::host().pump();CHECK(!keyReady());
  lcom_exit();
  // A separate panel must keep all controls, including its disclosure and
  // configured pads/actions, outside every playable framebuffer pixel.
  controls.repeatKeys.clear();controls.quickKeys={"ESC"};controls.layout="panel";
  lcom::embedded::configureControls(controls);
  CHECK(lcom_init()==LCOM_OK);CHECK(lcom_vbe_set_mode(LCOM_VBE_MODE_800_600_24)==LCOM_OK);
  CHECK(lcom_phys_map(mode.framebuffer_phys,mode.framebuffer_size,reinterpret_cast<void **>(&pixels))==LCOM_OK);
  for(size_t i=0;i<mode.framebuffer_size;i+=3) {pixels[i]=51;pixels[i+1]=34;pixels[i+2]=17;}
  windows=SDL_GetWindows(&count);CHECK(windows && count==1);window=windows[0];SDL_free(windows);
  CHECK(SDL_SetWindowSize(window,960,400));CHECK(SDL_SyncWindow(window));lcom::embedded::host().pump();
  CHECK(lcom_vbe_present()==LCOM_OK);
  frame=readPresentedFrame(SDL_GetRenderer(window));CHECK(frame);
  CHECK(frame->w==960 && frame->h==400);
  // Independently fitted4:3guest:512x384within536x384remaining area.
  for(int y=9;y<391;y+=3) for(int x=21;x<531;x+=3) {
    CHECK(SDL_ReadSurfacePixel(frame,x,y,&r,&g,&b,&a));CHECK(r==17 && g==34 && b==51);
  }
  SDL_DestroySurface(frame);
  CHECK(SDL_SetWindowSize(window,400,500));CHECK(SDL_SyncWindow(window));lcom::embedded::host().pump();
  frame=readPresentedFrame(SDL_GetRenderer(window));CHECK(frame);
  CHECK(frame->w==400 && frame->h==500);
  // A short portrait panel reserves its pads/actions below a336x252guest.
  for(int y=9;y<203;y+=3) for(int x=72;x<328;x+=3) {
    CHECK(SDL_ReadSurfacePixel(frame,x,y,&r,&g,&b,&a));CHECK(r==17 && g==34 && b==51);
  }
  SDL_DestroySurface(frame);
  lcom_exit(); lcom::embedded::configureControls({});
  puts("native SDL input/render/lifecycle checks passed");
}
