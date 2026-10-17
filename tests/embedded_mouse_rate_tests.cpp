#include "embedded/Host.hpp"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <lcom/i8042.h>
#include <lcom/lcom.h>
#include <vector>
#define CHECK(x)                                                               \
  do {                                                                         \
    if (!(x)) {                                                                \
      fprintf(stderr, "line%d: %s\n", __LINE__, #x);                           \
      exit(1);                                                                 \
    }                                                                          \
  } while (0)
namespace {
class Host final : public lcom::embedded::Host {
public:
  double time = 0;
  bool start() override { return true; }
  void stop() override {}
  double nowMs() override { return time; }
  void sleepMs(unsigned ms) override { time += ms; }
  void pump() override {}
  bool paused() override { return false; }
  void present(lcom::Machine &) override {}
  void audio(const int16_t *, size_t, uint32_t, uint8_t) override {}
  void stopAudio() override {}
  bool setClipboard(const std::string &) override { return false; }
  bool getClipboard(std::string &) override { return false; }
} platform;
} // namespace
namespace lcom::embedded {
Host &host() { return platform; }
} // namespace lcom::embedded
static uint8_t read() {
  uint8_t b;
  CHECK(lcom_port_read8(KBC_OUT_BUF, &b) == LCOM_OK);
  return b;
}
static void command(uint8_t b) {
  CHECK(lcom_port_write8(KBC_CMD_REG, KBC_CMD_WRITE_MOUSE) == LCOM_OK);
  CHECK(lcom_port_write8(KBC_IN_BUF, b) == LCOM_OK);
  CHECK(read() == 0xfa);
}
static bool ready() {
  uint8_t s;
  CHECK(lcom_port_read8(KBC_ST_REG, &s) == LCOM_OK);
  return (s & KBC_ST_OBF) != 0;
}
static std::vector<std::array<int, 3>> drain() {
  std::vector<std::array<int, 3>> packets;
  for (unsigned i = 0; ready(); ++i) {
    CHECK(i < 128);
    uint8_t f = read(), x = read(), y = read();
    CHECK((f & 0xc8) == 8);
    packets.push_back(
        {int(x) - ((f & 16) ? 256 : 0), int(y) - ((f & 32) ? 256 : 0), f & 7});
  }
  return packets;
}
static void begin(unsigned rate) {
  platform.time = 0;
  CHECK(lcom_init() == LCOM_OK);
  command(0xf3);
  command(rate);
  command(0xe9);
  (void)read();
  (void)read();
  CHECK(read() == rate);
  command(0xf4);
}
static void rate() {
  for (unsigned hz : {10, 100, 200}) {
    begin(hz);
    lcom::embedded::injectMouse(1, 0, 0);
    CHECK(drain().size() == 1);
    platform.time = 1;
    lcom::embedded::injectMouse(2, 0, 0);
    CHECK(!ready() && "programmed F3 report rate must limit host motion "
                      "packets even when guest drains rapidly");
    platform.time = 1000.0 / hz;
    auto p = drain();
    CHECK(p.size() == 1 && p[0][0] == 2);
    lcom_exit();
  }
  begin(100);
  command(0xf3);
  for (uint8_t invalid : {0, 255}) {
    CHECK(lcom_port_write8(KBC_CMD_REG, KBC_CMD_WRITE_MOUSE) == LCOM_OK);
    CHECK(lcom_port_write8(KBC_IN_BUF, invalid) == LCOM_OK);
    CHECK(read() == 0xfe);
  }
  command(100);
  command(0xe9);
  (void)read();
  (void)read();
  CHECK(read() == 100);
  lcom_exit();
  puts("F3_REPORT_RATE_OK");
}
struct Travel {
  int raw = 0, cells = 0;
  unsigned packets = 0;
};
static Travel travel(unsigned period) {
  begin(100);
  Travel r;
  for (unsigned t = period; t <= 100; t += period) {
    platform.time = t;
    lcom::embedded::injectMouse(0, -int(period * 3), 0);
    for (auto p : drain()) {
      r.raw += p[1];
      r.cells += p[1] / 24;
      r.packets++;
    }
  }
  platform.time = 120;
  for (auto p : drain()) {
    r.raw += p[1];
    r.cells += p[1] / 24;
    r.packets++;
  }
  lcom_exit();
  return r;
}
static void coarse() {
  auto fast = travel(2), normal = travel(10);
  printf("2ms: raw%d cells%d packets%u;10ms: raw%d cells%d packets%u\n",
         fast.raw, fast.cells, fast.packets, normal.raw, normal.cells,
         normal.packets);
  CHECK(fast.raw == 300 && normal.raw == 300);
  CHECK(fast.packets <= 11 && "100Hz PS/2 mouse must not emit500Hz reports due "
                              "to guest polling frequency");
  CHECK(fast.cells == normal.cells && normal.cells == 10 &&
        "same continuous physical travel must retain its coarse-cell motion at "
        "the programmed report interval");
  puts("PUMP_INDEPENDENT_COARSE_MOTION_OK");
}

static void commands() {
  begin(100);
  lcom::embedded::injectMouse(1, 0, 1);
  CHECK(drain().size() == 1);
  platform.time = 1;
  lcom::embedded::injectMouse(9, 0, 1);
  CHECK(!ready());
  command(0xf3);
  command(20); // ACKs bypass the motion report interval.
  platform.time = 2;
  lcom::embedded::injectMouse(0, 0, 0);
  CHECK(!ready());
  platform.time = 50;
  auto release = drain();
  CHECK(release.size() == 1 && release[0][0] == 0 && release[0][2] == 0 &&
        "F3 clears unsent movement but retains a delivered button for its "
        "release");
  platform.time = 51;
  lcom::embedded::injectMouse(30, 40, 1);
  CHECK(!ready());
  command(0xf5);
  lcom::embedded::injectMouse(4, 5, 0);
  CHECK(!ready());
  command(0xf4);
  lcom::embedded::injectMouse(2, 3, 0);
  auto fresh = drain();
  CHECK(fresh.size() == 1 && fresh[0][0] == 2 && fresh[0][1] == -3 &&
        fresh[0][2] == 0);
  lcom_exit();
  puts("COMMAND_RESET_DISABLE_OK");
}
static void backpressure() {
  begin(100);
  lcom::embedded::injectMouse(12, 34, 1);
  uint8_t header = read();
  CHECK((header & 0x29) == 0x29);
  platform.time = 20;
  lcom::embedded::injectMouse(40, 50, 0);
  CHECK(read() == 12 && read() == uint8_t(-34));
  auto next = drain();
  CHECK(next.size() == 1 && next[0][0] == 40 && next[0][1] == -50 &&
        next[0][2] == 0);
  platform.time = 40;
  CHECK(drain().empty());
  lcom_exit();
  begin(100);
  lcom::embedded::injectMouse(15, 0, 0);
  CHECK(drain().size() == 1);
  platform.time = 1;
  lcom::embedded::injectMouse(3, 0, 1);
  platform.time = 2;
  lcom::embedded::injectMouse(0, 0, 0);
  CHECK(!ready());
  platform.time = 10;
  auto press = drain();
  CHECK(press.size() == 1 && press[0][0] == 3 && press[0][2] == 1);
  platform.time = 11;
  CHECK(!ready());
  platform.time = 20;
  auto release = drain();
  CHECK(release.size() == 1 && release[0][0] == 0 && release[0][2] == 0);
  lcom_exit();
  puts("COMPLETE_PACKET_AND_BUTTON_EDGES_OK");
}
static void wheelRemote() {
  begin(100);
  command(0xf5);
  command(0xf3);
  command(200);
  command(0xf3);
  command(100);
  command(0xf3);
  command(80);
  command(0xf4);
  lcom::embedded::injectMouse(1, 0, 0);
  CHECK(read() == 8 && read() == 1 && read() == 0 && read() == 0);
  platform.time = 1;
  lcom::embedded::injectWheel(2);
  CHECK(!ready());
  platform.time = 12.5;
  CHECK(ready());
  CHECK(read() == 8 && read() == 0 && read() == 0 && read() == 2);
  platform.time = 13;
  lcom::embedded::injectWheel(-1);
  CHECK(!ready());
  platform.time = 25;
  CHECK(read() == 8 && read() == 0 && read() == 0 && read() == 255);
  lcom_exit();
  begin(100);
  command(0xf0);
  lcom::embedded::injectMouse(10, 20, 0);
  platform.time = 1;
  lcom::embedded::injectMouse(20, 30, 0);
  CHECK(!ready());
  command(0xeb);
  auto polled = drain();
  CHECK(polled.size() == 1 && polled[0][0] == 30 && polled[0][1] == -50);
  lcom_exit();
  begin(100);
  lcom::embedded::injectMouse(2, 0, 0);
  CHECK(drain().size() == 1);
  lcom_exit();
  puts("WHEEL_REMOTE_SESSION_LIFECYCLE_OK");
}

static void pendingButtons() {
  begin(100);
  lcom::embedded::injectMouse(1, 0, 0);
  CHECK(drain().size() == 1);
  platform.time = 1;
  lcom::embedded::injectMouse(9, 0, 1);
  CHECK(!ready());
  platform.time = 2;
  command(0xf3);
  command(100);
  platform.time = 10;
  auto held = drain();
  CHECK(held.size() == 1 && held[0][0] == 0 && held[0][2] == 1 &&
        "F3 resets unsent XY counters without losing a queued physical button "
        "press");
  platform.time = 11;
  command(0xf5);
  lcom::embedded::injectMouse(0, 0, 0);
  command(0xf4);
  auto released = drain();
  CHECK(released.size() == 1 && released[0][2] == 0 &&
        "reenable reports the physical release that occurred while reporting "
        "was disabled");
  platform.time = 12;
  lcom::embedded::injectMouse(0, 0, 1);
  CHECK(!ready());
  command(0xf5);
  command(0xf4);
  auto pressed = drain();
  CHECK(pressed.size() == 1 && pressed[0][2] == 1 &&
        "reenable must report a physically held press that had not reached the "
        "guest");
  lcom_exit();
  puts("PENDING_BUTTON_COMMAND_LIFECYCLE_OK");
}
static void directButtonsWhileDisabled(bool release_only = false) {
  begin(100);
  lcom::embedded::injectMouse(1, 0, release_only ? 1 : 0);
  CHECK(drain().size() == 1);
  if (!release_only) {
    command(0xf5);
    lcom::embedded::injectPointer(100, 200, 1);
    CHECK(!ready());
    command(0xf4);
    auto press = drain();
    CHECK(press.size() == 1 && press[0][0] == 0 && press[0][1] == 0 &&
          press[0][2] == 1 &&
          "a direct press while F5 is active must be reported at F4 without "
          "disabled absolute motion");
  }
  command(0xf5);
  lcom::embedded::injectPointer(700, 500, 0);
  CHECK(!ready());
  command(0xf4);
  auto release = drain();
  CHECK(release.size() == 1 && release[0][0] == 0 && release[0][1] == 0 &&
        release[0][2] == 0 &&
        "a direct release while F5 is active must reconcile the delivered held "
        "button");
  lcom_exit();
  puts("DIRECT_BUTTON_DISABLE_REENABLE_OK");
}
int main(int argc, char **argv) {
  const char *mode = argc > 1 ? argv[1] : "all";
  if (!strcmp(mode, "all")) {
    rate();
    coarse();
    commands();
    backpressure();
    wheelRemote();
    pendingButtons();
    directButtonsWhileDisabled();
  } else if (!strcmp(mode, "coarse"))
    coarse();
  else if (!strcmp(mode, "commands"))
    commands();
  else if (!strcmp(mode, "backpressure"))
    backpressure();
  else if (!strcmp(mode, "wheel-remote"))
    wheelRemote();
  else if (!strcmp(mode, "pending-buttons"))
    pendingButtons();
  else if (!strcmp(mode, "direct-release"))
    directButtonsWhileDisabled(true);
  else if (!strcmp(mode, "direct-buttons"))
    directButtonsWhileDisabled();
  else
    rate();
}
