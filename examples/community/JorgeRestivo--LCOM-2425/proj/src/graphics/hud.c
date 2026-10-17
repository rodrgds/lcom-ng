#include "hud.h"

extern Player player;
extern Bullet bullets[MAX_BULLETS];

void draw_char(uint16_t x, uint16_t y, char c, uint8_t color){
  if (c < 32 || c > 90) {
    return;
  }


  const uint8_t *ch = font8x8[(uint8_t) c];
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if (ch[row] & (1 << (7 - col))) {
        for (int dy = 0; dy < FONT_SCALE; dy++) {
          for (int dx = 0; dx < FONT_SCALE; dx++) {
            
            vg_draw_pixel(x + col * FONT_SCALE + dx, y + row * FONT_SCALE + dy, color);
          }
        }
      }
    }
  }
}



void draw_text(uint16_t x, uint16_t y, const char *str, uint8_t color) {
  while (*str) {
    draw_char(x, y, *str++, color);
    x += FONT_SIZE; 
  }
}


void draw_number(uint16_t x, uint16_t y, int number, uint8_t color){

  char buff[12];
  sprintf(buff, "%d", number);
  draw_text(x, y, buff, color);

}


void draw_hp_bar(uint16_t x, uint16_t y, int width, int height, int hp) {
    int filled = (hp * width) / 100;
    vg_draw_rectangle(x, y, width, height, 0); 
    if (filled > 170){
      vg_draw_rectangle(x, y, filled, height, 2); // GREEN
    }
    else if (filled <= 170 && filled >= 120){
      vg_draw_rectangle(x, y, filled, height, 62); // YELLOW
    }
    else if (filled >=70 && filled <120){
      vg_draw_rectangle(x, y, filled, height, 52); //ORANGE
    }
    else {
      vg_draw_rectangle(x, y, filled, height, 32); //RED
    }
    
}

void draw_hud() {
  // Count bullets
  int bullets_active = 0;
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (bullets[i].active) bullets_active++;
  }
  bullets_active = MAX_BULLETS - bullets_active;

  // SCORE (top left)
  draw_text(20, 20, "SCORE:", 63);
  draw_number(20 + FONT_SIZE * 6, 20, player.score, 63);

  // HP (bottom left) - positioned near bottom of screen
  int bottom_y = SCREEN_HEIGHT - 50; // 50 pixels from bottom
  draw_text(20, bottom_y, "HP:", 63);
  // HP bar right next to "HP:" text
  draw_hp_bar(20 + FONT_SIZE * 3, bottom_y, FONT_SIZE * 10, FONT_SCALE * 8, player.health);
  

  // BULLETS (bottom right) - same vertical level as HP
  draw_text(SCREEN_WIDTH - 250, bottom_y, "BULLETS:", 63);
  // Number of bullets right next to "BULLETS:" text
  draw_number(SCREEN_WIDTH - 250 + FONT_SIZE * 8, bottom_y, bullets_active, 63);
}







