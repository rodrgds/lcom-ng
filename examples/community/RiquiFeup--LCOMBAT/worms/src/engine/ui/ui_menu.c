#include "ui_menu.h"

#include <stdlib.h>

#include "ui_panel.h"
#include "config.h"

struct UiMenu {
  UiPanel     buttons[UI_MENU_MAX_BUTTONS];
  const char *labels[UI_MENU_MAX_BUTTONS];
  int         btn_count;
  int         selected;
  UiPanel     title_panel;
  bool        has_title;
  const char *title_label;
};

UiMenu *ui_menu_create(int center_x, int y_start,
                       int btn_w, int btn_h, int btn_gap,
                       const char *title,
                       const char *const labels[], int btn_count) {
  if (btn_count < 1 || btn_count > UI_MENU_MAX_BUTTONS) return NULL;
  UiMenu *m = malloc(sizeof(UiMenu));
  if (m == NULL) return NULL;

  m->btn_count   = btn_count;
  m->selected    = 0;
  m->has_title   = (title != NULL);
  m->title_label = title;

  int btn_x = center_x - btn_w / 2;
  int cur_y = y_start;

  if (m->has_title) {
    m->title_panel = (UiPanel){
      .x = btn_x, .y = cur_y, .w = btn_w, .h = MENU_TITLE_H,
      .border        = MENU_BTN_BORDER,
      .border_color  = COLOR_MENU_BTN_BDR,
      .fill_color    = COLOR_MENU_TITLE_BG,
      .fill_selected = COLOR_MENU_TITLE_BG
    };
    cur_y += MENU_TITLE_H + btn_gap;
  }

  for (int i = 0; i < btn_count; i++) {
    m->buttons[i] = (UiPanel){
      .x = btn_x, .y = cur_y, .w = btn_w, .h = btn_h,
      .border        = MENU_BTN_BORDER,
      .border_color  = COLOR_MENU_BTN_BDR,
      .fill_color    = COLOR_MENU_BTN,
      .fill_selected = COLOR_MENU_BTN_SEL
    };
    m->labels[i] = (labels != NULL) ? labels[i] : NULL;
    cur_y += btn_h + btn_gap;
  }

  return m;
}

void ui_menu_destroy(UiMenu *m) {
  if (m == NULL) return;
  free(m);
}

void ui_menu_draw(const UiMenu *m, const Font *f) {
  if (m == NULL) return;
  if (m->has_title)
    ui_panel_draw(&m->title_panel, f, m->title_label, false);
  for (int i = 0; i < m->btn_count; i++)
    ui_panel_draw(&m->buttons[i], f, m->labels[i], m->selected == i);
}

int ui_menu_update(UiMenu *m, int16_t mx, int16_t my,
                   bool lb_click, int nav_delta, bool confirm) {
  if (m == NULL) return -1;
  if (nav_delta != 0)
    m->selected = (m->selected + nav_delta + m->btn_count) % m->btn_count;
  for (int i = 0; i < m->btn_count; i++) {
    if (ui_panel_contains(&m->buttons[i], (int)mx, (int)my)) {
      m->selected = i;
      break;
    }
  }
  if (confirm) return m->selected;
  if (lb_click) {
    for (int i = 0; i < m->btn_count; i++) {
      if (ui_panel_contains(&m->buttons[i], (int)mx, (int)my))
        return i;
    }
  }
  return -1;
}
