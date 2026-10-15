#include "menu.h"
#include "renderer.h"
#include <lcom/i8042.h>
#include <stdio.h>

enum { BUTTON_X = 270, BUTTON_W = 260, BUTTON_H = 48, BUTTON_PRIMARY_Y = 286, BUTTON_SECONDARY_Y = 350 };

static void draw_button(draw_context_t *ctx, int y, const char *label, int selected) {
  uint32_t bg = selected ? 0xF2C94Cu : 0x354C59u;
  uint32_t text = selected ? 0x25332Eu : 0xEFF8F5u;
  fill_rect(ctx, BUTTON_X, y, BUTTON_W, BUTTON_H, bg);
  fill_rect(ctx, BUTTON_X, y, BUTTON_W, 3, selected ? 0xFFE9A5u : 0x617785u);
  fill_rect(ctx, BUTTON_X, y + BUTTON_H - 3, BUTTON_W, 3, selected ? 0xB98323u : 0x233643u);
  draw_text(ctx, BUTTON_X + (BUTTON_W - (int)font_text_width(label, 3)) / 2, y + 13, label, text, 3);
}

void menu_init(menu_t *menu) {
  *menu = (menu_t){.title = "LCOM BIRD", .primary_label = "PLAY", .secondary_label = "QUIT"};
}

void menu_set(menu_t *menu, const char *title, const char *primary, const char *secondary) {
  menu->title = title;
  menu->primary_label = primary;
  menu->secondary_label = secondary;
}

menu_action_t menu_key(menu_t *menu, uint8_t scancode) {
  if (scancode & KBD_BREAK_BIT) return MENU_ACTION_NONE;
  if (scancode == 0x48 || scancode == 0x50 || scancode == 0x0F) {
    menu->selected = 1 - menu->selected;
  } else if (scancode == 0x39 || scancode == 0x1C) {
    return menu->selected == 0 ? MENU_ACTION_PRIMARY : MENU_ACTION_QUIT;
  }
  return MENU_ACTION_NONE;
}

menu_action_t menu_mouse(menu_t *menu, int x, int y, int clicked) {
  if (x < BUTTON_X || x >= BUTTON_X + BUTTON_W) return MENU_ACTION_NONE;
  int option = y >= BUTTON_PRIMARY_Y && y < BUTTON_PRIMARY_Y + BUTTON_H ? 0 :
               y >= BUTTON_SECONDARY_Y && y < BUTTON_SECONDARY_Y + BUTTON_H ? 1 : -1;
  if (option < 0) return MENU_ACTION_NONE;
  menu->selected = option;
  return !clicked ? MENU_ACTION_NONE : option == 0 ? MENU_ACTION_PRIMARY : MENU_ACTION_QUIT;
}

void menu_render(menu_t *menu, draw_context_t *ctx, int score) {
  int title_x = (ctx->info.width - (int)font_text_width(menu->title, 6)) / 2;
  draw_text(ctx, title_x + 3, 129, menu->title, 0xE5F9F0u, 6);
  draw_text(ctx, title_x, 126, menu->title, 0x203F42u, 6);
  fill_rect(ctx, 238, 204, 324, 244, 0x203742u);
  draw_text(ctx, 270, 224, "FIND YOUR RHYTHM", 0xEAF7F0u, 2);
  char best[32];
  snprintf(best, sizeof(best), "SESSION BEST %d", score);
  draw_text(ctx, (800 - (int)font_text_width(best, 2)) / 2, 254, best, 0xBDD5DCu, 2);
  draw_button(ctx, BUTTON_PRIMARY_Y, menu->primary_label, menu->selected == 0);
  draw_button(ctx, BUTTON_SECONDARY_Y, menu->secondary_label, menu->selected == 1);
  draw_text(ctx, 262, 420, "ENTER / CLICK TO CHOOSE", 0xBDD5DCu, 1);
}
