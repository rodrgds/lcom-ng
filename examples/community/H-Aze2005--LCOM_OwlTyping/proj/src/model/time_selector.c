#include "time_selector.h"

TimeSelector *construct_TimeSelector(int16_t x, int16_t y) {
  TimeSelector *timeSelector = (TimeSelector *) malloc(sizeof(TimeSelector));

  if (!timeSelector) {
    printf("%s: malloc() error\n", __func__);
    return NULL;
  }

  timeSelector->x = x;
  timeSelector->y = y;
  timeSelector->state = S30;

  return timeSelector;
}

void destroy_TimeSelector(TimeSelector *timeSelector) {
  if (timeSelector)
    free(timeSelector);
}


void draw_TimeSelector(TimeSelector *timeSelector) {
    switch (timeSelector->state) {
        case S30:
            draw_image_to_both((xpm_map_t) time_picker_30s_xpm, timeSelector->x, timeSelector->y);
            break;
        case S60:
            draw_image_to_both((xpm_map_t) time_picker_60s_xpm, timeSelector->x, timeSelector->y);
            break;
        case S120:
            draw_image_to_both((xpm_map_t) time_picker_120s_xpm, timeSelector->x, timeSelector->y);
            break;
    }
}

void set_time_selector_state(TimeSelector *timeSelector, SelectorState state) {
    timeSelector->state = state;
    draw_TimeSelector(timeSelector);
}

int get_selected_time(TimeSelector *timeSelector) {
  switch (timeSelector->state) {
      case S30: return 30;
      case S60: return 60;
      case S120: return 120;
      default: return 30; // default
  }
}
