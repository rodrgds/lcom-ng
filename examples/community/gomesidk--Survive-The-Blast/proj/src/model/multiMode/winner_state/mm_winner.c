#include "mm_winner.h"

struct mm_winner_imp {
  Sprite *background; 
  Sprite *you_won; 
  Sprite *you_lost;
  Sprite *draw; 
  Button *again;
  Button *menu;
  int winner; // 0 - No winner, 1 - Current player wins, 2 - Other player wins
};

MmWinner *create_mm_winner(SpriteLoader *loader, int winner) {
  MmWinner *mm_winner = (MmWinner *) malloc(sizeof(MmWinner));
  if (mm_winner == NULL) {
    return NULL;
  }
  mm_winner->winner = winner;
  mm_winner->background = get_winners_background(loader); 
  mm_winner->you_won = get_winners_you_won(loader); 
  mm_winner->you_lost = get_winners_you_lost(loader); 
  mm_winner->draw = get_winners_draw(loader); 

  if (mm_winner->background == NULL || mm_winner->you_won == NULL || mm_winner->you_lost == NULL || mm_winner->draw == NULL) {
    destroy_mm_winner(mm_winner);
    return NULL;
  }

  mm_winner->again = create_button(get_sprite_width(mm_winner->background) / 2 - get_sprite_width(get_play_again(loader)) /2 , 600, get_play_again(loader)); 
  mm_winner->menu = create_button(get_sprite_width(mm_winner->background) / 2 - get_sprite_width(get_back_to_menu(loader)) /2 , 800, get_back_to_menu(loader));
  
  return mm_winner;
}

void destroy_mm_winner(MmWinner *mm_winner) {
  if (mm_winner == NULL) {
    return;
  }

  destroy_button(mm_winner->again);
  destroy_button(mm_winner->menu);
  free(mm_winner);
}

void draw_mm_winner(MmWinner *mm_winner) {
  if (mm_winner == NULL) {
    return;
  }
  draw_sprite(mm_winner->background, 0, 0);

  int x_pos = get_sprite_width(mm_winner->background) / 2 - get_sprite_width(mm_winner->you_won) / 2;
  int y_pos = get_sprite_height(mm_winner->background) / 2 - (get_sprite_height(mm_winner->you_won) / 2) - 250;
  if (mm_winner->winner == 0) {
    draw_sprite(mm_winner->draw, x_pos, y_pos);
  } else if (mm_winner->winner == 1) {
    draw_sprite(mm_winner->you_won, x_pos, y_pos);
  } else if (mm_winner->winner == 2) {
    draw_sprite(mm_winner->you_lost, x_pos, y_pos); 
  } 

  draw_button(mm_winner->again); 
  draw_button(mm_winner->menu); 
}

// 0 - Continue game, 1 - Go back to menu, 2 - Go back to game
int process_mm_winner_mouse(MmWinner *mm_winner, Cursor *c) {
  if (mm_winner == NULL || c == NULL) {
    return 1; // Error
  }
  
  if (is_button_clicked(mm_winner->again, c)) {
    return 2;
  }else if (is_button_clicked(mm_winner->menu, c)) {
    return 1; 
  }

  return 0; 
}

// 0 - Continue game, 1 - Go back to menu
int process_mm_winner_kbd(MmWinner *mm_winner, KeyPressed *kp) {
  if (mm_winner == NULL || kp == NULL) {
    return 1; 
  }

  if (is_esc_pressed(kp)) {
    return 1; 
  }
  
  return 0; 
}
