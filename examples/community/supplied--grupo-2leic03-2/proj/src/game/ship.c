#include "ship.h"
#include "../config.h"
#include "../graphics/graphics.h"

static void ship_draw_shield_point(int x, int y, uint32_t color) {
  graphics_draw_rectangle_clipped(x, y, 2, 2, color);
}

static void ship_draw_shield_circle(int center_x, int center_y, int radius) {
  int x = radius;
  int y = 0;
  int decision = 1 - radius;

  while (x >= y) {
    uint32_t color = (y % 6 == 0) ? COLOR_WHITE : COLOR_CYAN;

    ship_draw_shield_point(center_x + x, center_y + y, color);
    ship_draw_shield_point(center_x + y, center_y + x, color);
    ship_draw_shield_point(center_x - y, center_y + x, color);
    ship_draw_shield_point(center_x - x, center_y + y, color);
    ship_draw_shield_point(center_x - x, center_y - y, color);
    ship_draw_shield_point(center_x - y, center_y - x, color);
    ship_draw_shield_point(center_x + y, center_y - x, color);
    ship_draw_shield_point(center_x + x, center_y - y, color);

    y++;

    if (decision <= 0) {
      decision += 2 * y + 1;
    }
    else {
      x--;
      decision += 2 * (y - x) + 1;
    }
  }
}

void ship_update(Ship *ship, const InputState *input) {
  if (input->left) {
    ship->base.x -= ship->speed;
  }
  
  if (input->right) {
    ship->base.x += ship->speed;
  }

  if (input->up) {
    ship->base.y -= ship->speed;
  }

  if (input->down) {
    ship->base.y += ship->speed;
  }

  if (ship->base.x < 0) {
    ship->base.x = 0;
  }

  if (ship->base.x + ship->base.width > SCREEN_WIDTH) {
    ship->base.x = SCREEN_WIDTH - ship->base.width;
  }

  if (ship->base.y < 0) {
    ship->base.y = 0;
  }

  if (ship->base.y + ship->base.height > SCREEN_HEIGHT) {
    ship->base.y = SCREEN_HEIGHT - ship->base.height;
  }
}

void ship_draw(const Ship *ship, const Sprite *sprite, bool shield_active) {  
  if (sprite != NULL && sprite->loaded) {
    sprite_draw_scaled(sprite, ship->base.x, ship->base.y, 2);
  } else {
    graphics_draw_rectangle_clipped(ship->base.x,
                                    ship->base.y,
                                    ship->base.width,
                                    ship->base.height,
                                    COLOR_RED);
  }

  if (shield_active) {
    int center_x = ship->base.x + ship->base.width / 2;
    int center_y = ship->base.y + ship->base.height / 2;
    int radius = ship->base.width > ship->base.height ? ship->base.width / 2 : ship->base.height / 2;
    ship_draw_shield_circle(center_x, center_y, radius + 10);
  }
}
