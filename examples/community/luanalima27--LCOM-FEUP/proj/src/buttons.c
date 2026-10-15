// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include "video.h"
#include "player.h"
#include "sprites.h"
#include "i8042.h"

extern uint8_t* start_sprite;
extern uint8_t* selected_start_sprite;
extern uint8_t* start_black_sprite;
extern uint8_t* quit_sprite;
extern uint8_t* selected_quit_sprite;
extern uint8_t* quit_black_sprite;
extern uint8_t* win_black_sprite;
extern uint8_t* gameover_black_sprite;
extern uint8_t* gameover_sprite;
extern uint8_t* win_sprite;
extern xpm_image_t img_start_sprite;
extern xpm_image_t img_selected_start_sprite;
extern xpm_image_t img_start_black_sprite;
extern xpm_image_t img_quit_sprite;
extern xpm_image_t img_selected_quit_sprite;
extern xpm_image_t img_quit_black_sprite;
extern xpm_image_t img_gameover_black_sprite;
extern xpm_image_t img_win_black_sprite;
extern xpm_image_t img_gameover_sprite;
extern xpm_image_t img_win_sprite;

extern uint8_t *back_sprite;
extern uint8_t *selected_back_sprite;
extern uint8_t *back_black_sprite;
extern xpm_image_t img_back_sprite;
extern xpm_image_t img_selected_back_sprite;
extern xpm_image_t img_back_black_sprite;

extern uint8_t *easy_sprite;
extern uint8_t *selected_easy_sprite;
extern uint8_t *easy_black_sprite;
extern xpm_image_t img_easy_sprite;
extern xpm_image_t img_selected_easy_sprite;
extern xpm_image_t img_easy_black_sprite;

extern uint8_t *hard_sprite;
extern uint8_t *selected_hard_sprite;
extern uint8_t *hard_black_sprite;
extern xpm_image_t img_hard_sprite;
extern xpm_image_t img_selected_hard_sprite;
extern xpm_image_t img_hard_black_sprite;

extern uint8_t *medium_sprite;
extern uint8_t *selected_medium_sprite;
extern uint8_t *medium_black_sprite;
extern xpm_image_t img_medium_sprite;
extern xpm_image_t img_selected_medium_sprite;
extern xpm_image_t img_medium_black_sprite;


int draw_quit(uint16_t x, uint16_t y) {
  if(draw_xpm(quit_sprite, img_quit_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

int draw_selected_quit(uint16_t x, uint16_t y) {
  if(draw_xpm(selected_quit_sprite, img_selected_quit_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

int draw_start(uint16_t x, uint16_t y) {
  if(draw_xpm(start_sprite, img_start_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

int draw_selected_start(uint16_t x, uint16_t y) {
  if(draw_xpm(selected_start_sprite, img_selected_start_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

int draw_back(uint16_t x, uint16_t y) {
  if(draw_xpm(back_sprite, img_back_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

int draw_selected_back(uint16_t x, uint16_t y) {
  if(draw_xpm(selected_back_sprite, img_selected_back_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

int draw_easy(uint16_t x, uint16_t y) {
  if(draw_xpm(easy_sprite, img_easy_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

int draw_selected_easy(uint16_t x, uint16_t y) {
  if(draw_xpm(selected_easy_sprite, img_selected_easy_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}
int draw_medium(uint16_t x, uint16_t y) {
  if(draw_xpm(medium_sprite, img_medium_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

int draw_selected_medium(uint16_t x, uint16_t y) {
  if(draw_xpm(selected_medium_sprite, img_selected_medium_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

int draw_hard(uint16_t x, uint16_t y) {
  if(draw_xpm(hard_sprite, img_hard_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

int draw_selected_hard(uint16_t x, uint16_t y) {
  if(draw_xpm(selected_hard_sprite, img_selected_hard_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

int delete_quit(uint16_t x, uint16_t y) {
  if(draw_xpm(quit_black_sprite, img_quit_black_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

int delete_start(uint16_t x, uint16_t y) {
  if(draw_xpm(start_black_sprite, img_start_black_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

int delete_back(uint16_t x, uint16_t y) {
  if(draw_xpm(back_black_sprite, img_back_black_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

int delete_easy(uint16_t x, uint16_t y) {
  if(draw_xpm(easy_black_sprite, img_easy_black_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

int delete_medium(uint16_t x, uint16_t y) {
  if(draw_xpm(medium_black_sprite, img_medium_black_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

int delete_hard(uint16_t x, uint16_t y) {
  if(draw_xpm(hard_black_sprite, img_hard_black_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}

int draw_game_over(uint16_t x, uint16_t y) {
 if (draw_xpm(gameover_sprite, img_gameover_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
    }
  return 0;
}


int draw_win(uint16_t x, uint16_t y) {
 if (draw_xpm(win_sprite, img_win_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
    }
  return 0;
}


int delete_win(uint16_t x, uint16_t y) {
  if(draw_xpm(win_black_sprite, img_win_black_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}


int delete_gameover(uint16_t x, uint16_t y) {
  if(draw_xpm(gameover_black_sprite, img_gameover_black_sprite, x, y) != 0) {
    printf("xpm failed to load");
    return 1;
  }

  return 0;
}
