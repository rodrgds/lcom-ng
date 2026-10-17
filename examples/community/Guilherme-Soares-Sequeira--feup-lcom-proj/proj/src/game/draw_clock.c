#include <lcom/lcf.h>
#include <math.h>

#include "../devices/rtc/rtc.h"
#include "../devices/video/graphics.h"
#include "../utils/color.h"
#include "canvas.h"
#include "draw_clock.h"

#include "../xpm/clock.xpm"

#include "text.h"

static xpm_image_t clock_xpm; /**< @brief xpm image of the clock */

static position clock_pos = {
  H_RES - CANVAS_MARGIN - 40,
  V_RES - CANVAS_MARGIN - 40}; /**< @brief the top-left corner position of the clock XPM */

static position clock_pos_center = {
  H_RES - CANVAS_MARGIN,
  V_RES - CANVAS_MARGIN}; /**< @brief the center position of the clock XPM */

void(clock_load)() {
  clock_xpm = vg_load_xpm(xpm_clock);
}

void(clock_draw)() {
  const pixel_buffer *const buf = get_back_buffer();

  buf_draw_xpm(buf, clock_xpm, clock_pos);

  int hour = rtc_get_hours() % 12;
  int minute = rtc_get_minutes();
  int second = rtc_get_seconds();
  double hour_angle = M_PI * (hour + (minute / 60.0)) / 6.0;
  int hour_pos_x = clock_pos_center.x + HOUR_HANDLE_LENGTH * sin(hour_angle);
  int hour_pos_y = clock_pos_center.y - HOUR_HANDLE_LENGTH * cos(hour_angle);
  position hour_pos = {hour_pos_x, hour_pos_y};
  buf_draw_line(buf, clock_pos_center, hour_pos, COLOR_BLACK);

  double minute_angle = M_PI * (minute + (second / 60.0)) / 30.0;
  int minute_pos_x = clock_pos_center.x + MINUTE_HANDLE_LENGTH * sin(minute_angle);
  int minute_pos_y = clock_pos_center.y - MINUTE_HANDLE_LENGTH * cos(minute_angle);
  position minute_pos = {minute_pos_x, minute_pos_y};
  buf_draw_line(buf, clock_pos_center, minute_pos, COLOR_BLACK);
}
