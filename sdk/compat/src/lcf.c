#include <lcom/lcf.h>

#define WEAK __attribute__((weak))

/* Definitions keep optional entry points linkable with both ELF and Mach-O
 * static archives. A student's implementation overrides these defaults. */
WEAK int proj_main_loop(int argc, char **argv) { (void)argc; (void)argv; return ENOSYS; }
WEAK int timer_test_read_config(uint8_t timer, enum timer_status_field field) { (void)timer; (void)field; return ENOSYS; }
WEAK int timer_test_time_base(uint8_t timer, uint32_t freq) { (void)timer; (void)freq; return ENOSYS; }
WEAK int timer_test_int(uint8_t time) { (void)time; return ENOSYS; }
WEAK int rtc_test_conf(void) { return ENOSYS; }
WEAK int rtc_test_date(void) { return ENOSYS; }
WEAK int rtc_test_int(void) { return ENOSYS; }
WEAK int kbd_test_scan(void) { return ENOSYS; }
WEAK int kbd_test_poll(void) { return ENOSYS; }
WEAK int kbd_test_timed_scan(uint8_t time) { (void)time; return ENOSYS; }
WEAK int mouse_test_packet(uint32_t count) { (void)count; return ENOSYS; }
WEAK int mouse_test_async(uint8_t idle) { (void)idle; return ENOSYS; }
WEAK int mouse_test_remote(uint16_t period, uint8_t count) { (void)period; (void)count; return ENOSYS; }
WEAK int mouse_test_gesture(uint8_t x_len, uint8_t tolerance) { (void)x_len; (void)tolerance; return ENOSYS; }
WEAK int video_test_init(uint16_t mode, uint8_t delay) { (void)mode; (void)delay; return ENOSYS; }
WEAK int video_test_rectangle(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  (void)mode; (void)x; (void)y; (void)width; (void)height; (void)color; return ENOSYS;
}
WEAK int video_test_pattern(uint16_t mode, uint8_t rectangles, uint32_t first, uint8_t step) {
  (void)mode; (void)rectangles; (void)first; (void)step; return ENOSYS;
}
WEAK int video_test_xpm(xpm_map_t map, uint16_t x, uint16_t y) { (void)map; (void)x; (void)y; return ENOSYS; }
WEAK int video_test_move(xpm_map_t map, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t frame_rate) {
  (void)map; (void)xi; (void)yi; (void)xf; (void)yf; (void)speed; (void)frame_rate; return ENOSYS;
}
WEAK int video_test_controller(void) { return ENOSYS; }

static int number(const char *text, uint32_t limit, uint32_t *out) {
  if (!text || !*text || *text == '-' || *text == '+' || *text == ' ' || *text == '\t' || *text == '\n' || *text == '\r') return EINVAL;
  char *end; errno = 0;
  unsigned long value = strtoul(text, &end, 0);
  if (errno || *end || value > limit) return EINVAL;
  *out = (uint32_t)value; return OK;
}
static xpm_map_t builtin_xpm(const char *name) {
  if (!name) return NULL;
  if (!strcmp(name, "pic1")) return pic1;
  if (!strcmp(name, "pic2")) return pic2;
  if (!strcmp(name, "pic3")) return pic3;
  if (!strcmp(name, "cross")) return cross;
  if (!strcmp(name, "penguin")) return penguin;
  if (!strcmp(name, "minix3_xpm")) return minix3_xpm;
  return NULL;
}
static int signed_speed(const char *text, int16_t *out) {
  if (!text || !*text || *text == '+' || *text == ' ' || *text == '\t' || *text == '\n' || *text == '\r') return EINVAL;
  char *end; errno = 0;
  long value = strtol(text, &end, 0);
  if (errno || *end || value < INT16_MIN || value > INT16_MAX) return EINVAL;
  *out = (int16_t)value; return OK;
}
int lcf_start(int argc, char **argv) {
  if (argc < 1 || !argv) return EINVAL;
  int initialization = lcom_compat_init();
  if (initialization) { fprintf(stderr, "lcom-ng: compatibility initialization failed: %s\n", strerror(initialization)); return initialization; }
  if (argc == 1) return proj_main_loop(0, argv + 1);
  const char *name = argv[1];
  if (!strcmp(name, "proj_main_loop") || !strcmp(name, "proj")) return proj_main_loop(argc - 2, argv + 2);
  if (!strcmp(name, "video_test_xpm") || !strcmp(name, "video_test_move")) {
    bool move = !strcmp(name, "video_test_move");
    if (argc != (move ? 9 : 5)) return EINVAL;
    xpm_map_t map = builtin_xpm(argv[2]);
    if (!map) { fprintf(stderr, "lcom-ng: unknown LCF pixmap: %s\n", argv[2]); return EINVAL; }
    uint32_t coordinates[4] = {0};
    for (int i = 0; i < (move ? 4 : 2); ++i)
      if (number(argv[3 + i], UINT16_MAX, &coordinates[i])) return EINVAL;
    if (!move) return video_test_xpm(map, (uint16_t)coordinates[0], (uint16_t)coordinates[1]);
    int16_t speed; uint32_t rate;
    if (signed_speed(argv[7], &speed) || number(argv[8], UINT8_MAX, &rate)) return EINVAL;
    return video_test_move(map, (uint16_t)coordinates[0], (uint16_t)coordinates[1],
                           (uint16_t)coordinates[2], (uint16_t)coordinates[3], speed, (uint8_t)rate);
  }
  const char *tests[] = {
    "timer_test_read_config", "timer_test_time_base", "timer_test_int", "kbd_test_scan",
    "kbd_test_poll", "kbd_test_timed_scan", "mouse_test_packet", "mouse_test_async",
    "mouse_test_remote", "mouse_test_gesture", "video_test_init", "video_test_rectangle",
    "video_test_pattern", "video_test_controller", "rtc_test_conf", "rtc_test_date", "rtc_test_int"
  };
  bool laboratory = false;
  for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); ++i) if (!strcmp(name, tests[i])) laboratory = true;
  if (!laboratory) return proj_main_loop(argc - 1, argv + 1);
  uint32_t values[6] = {0};
  int n = argc - 2;
  if (n > 6) return EINVAL;
  for (int i = 0; i < n; ++i) if (number(argv[i + 2], UINT32_MAX, &values[i])) return EINVAL;
#define MATCH(function, count) (!strcmp(name, #function) && n == count)
#define U8(i) (values[i] <= UINT8_MAX)
#define U16(i) (values[i] <= UINT16_MAX)
  if (MATCH(timer_test_read_config, 2) && values[0] < 3 && values[1] <= tsf_base)
    return timer_test_read_config((uint8_t)values[0], (enum timer_status_field)values[1]);
  if (MATCH(timer_test_time_base, 2) && values[0] < 3 && values[1]) return timer_test_time_base((uint8_t)values[0], values[1]);
  if (MATCH(timer_test_int, 1) && U8(0)) return timer_test_int((uint8_t)values[0]);
  if (MATCH(kbd_test_scan, 0)) return kbd_test_scan();
  if (MATCH(kbd_test_poll, 0)) return kbd_test_poll();
  if (MATCH(kbd_test_timed_scan, 1) && U8(0)) return kbd_test_timed_scan((uint8_t)values[0]);
  if (MATCH(mouse_test_packet, 1)) return mouse_test_packet(values[0]);
  if (MATCH(mouse_test_async, 1) && U8(0)) return mouse_test_async((uint8_t)values[0]);
  if (MATCH(mouse_test_remote, 2) && U16(0) && U8(1)) return mouse_test_remote((uint16_t)values[0], (uint8_t)values[1]);
  if (MATCH(mouse_test_gesture, 2) && U8(0) && U8(1)) return mouse_test_gesture((uint8_t)values[0], (uint8_t)values[1]);
  if (MATCH(video_test_init, 2) && U16(0) && U8(1)) return video_test_init((uint16_t)values[0], (uint8_t)values[1]);
  if (MATCH(video_test_rectangle, 6) && U16(0) && U16(1) && U16(2) && U16(3) && U16(4))
    return video_test_rectangle((uint16_t)values[0], (uint16_t)values[1], (uint16_t)values[2], (uint16_t)values[3], (uint16_t)values[4], values[5]);
  if (MATCH(video_test_pattern, 4) && U16(0) && U8(1) && U8(3))
    return video_test_pattern((uint16_t)values[0], (uint8_t)values[1], values[2], (uint8_t)values[3]);
  if (MATCH(video_test_controller, 0)) return video_test_controller();
  if (MATCH(rtc_test_conf, 0)) return rtc_test_conf();
  if (MATCH(rtc_test_date, 0)) return rtc_test_date();
  if (MATCH(rtc_test_int, 0)) return rtc_test_int();
  fprintf(stderr, "lcom-ng: unknown or unavailable LCF function, or invalid arguments: %s\n", name);
  return EINVAL;
}
int lcf_start_fast(int argc, char **argv) { return lcf_start(argc, argv); }
