#include "spriteLoader.h"

struct sprite_loader_imp {
  Sprite *player1_standing;
  AnimSprite *player1_left;
  AnimSprite *player1_right;
  AnimSprite *player1_up;
  AnimSprite *player1_down;
  Sprite *player2_standing;
  AnimSprite *player2_left;
  AnimSprite *player2_right;
  AnimSprite *player2_up;
  AnimSprite *player2_down;
  AnimSprite *bomb;
  Sprite *wall;
  Sprite *solid_wall;
  Sprite *grid_background;
  Sprite *cursor;
  AnimSprite *explosion;
  Sprite *menu_background;
  Sprite *menu_title;
  Sprite *single_mode_button;
  Sprite *multiplayer_mode_button;
  Sprite *exit_button;
  Sprite *exit;
  Sprite *bomb_options[3];
  Sprite *selected_options[3];
  Sprite *game_background;
  Sprite *died_title;
  Sprite *died_background;
  Sprite *play_again;
  Sprite *back_to_menu;
  Sprite *your_score;
  Sprite *number_0;
  Sprite *number_1;
  Sprite *number_2;
  Sprite *number_3;
  Sprite *number_4;
  Sprite *number_5;
  Sprite *number_6;
  Sprite *number_7;
  Sprite *number_8;
  Sprite *number_9;
  Sprite *waiting_connection;
  Sprite *warning_connection;
  Sprite *winners_background;
  Sprite *you_won;
  Sprite *you_lost;
  Sprite *draw;
};

SpriteLoader* load_sprites() {
  SpriteLoader *loader = (SpriteLoader *) malloc(sizeof(SpriteLoader));
  if (loader == NULL) {
    return NULL;
  }

  // Load player sprites
  loader->player1_standing = create_sprite((xpm_map_t)p1_front_1, BY_PIXEL);
  loader->player2_standing = create_sprite((xpm_map_t)p1_front_1, BY_PIXEL);
  loader->player1_left = create_anim_sprite(2, 8, (xpm_map_t)p1_left_1, (xpm_map_t)p1_left_2,
                                           (xpm_map_t)p1_left_3, (xpm_map_t)p1_left_2,
                                           (xpm_map_t)p1_left_1, (xpm_map_t)p1_left_4,
                                           (xpm_map_t)p1_left_5, (xpm_map_t)p1_left_4);
                                          
  loader->player1_right = create_anim_sprite(2, 8, (xpm_map_t)p1_right_1, (xpm_map_t)p1_right_2,
                                            (xpm_map_t)p1_right_3, (xpm_map_t)p1_right_2,
                                            (xpm_map_t)p1_right_1, (xpm_map_t)p1_right_4,
                                            (xpm_map_t)p1_right_5, (xpm_map_t)p1_right_4);

  loader->player1_up = create_anim_sprite(2, 8, (xpm_map_t)p1_back_1, (xpm_map_t)p1_back_2,
                                          (xpm_map_t)p1_back_3, (xpm_map_t)p1_back_2,
                                          (xpm_map_t)p1_back_1, (xpm_map_t)p1_back_4,
                                          (xpm_map_t)p1_back_5, (xpm_map_t)p1_back_4);  

  loader->player1_down = create_anim_sprite(2, 8, (xpm_map_t)p1_front_1, (xpm_map_t)p1_front_2,
                                            (xpm_map_t)p1_front_3, (xpm_map_t)p1_front_2,
                                            (xpm_map_t)p1_front_1, (xpm_map_t)p1_front_4,
                                            (xpm_map_t)p1_front_5, (xpm_map_t)p1_front_4);
      
  // Load player 2 sprites
  loader->player2_left = create_anim_sprite(2, 8, (xpm_map_t)p1_left_1, (xpm_map_t)p1_left_2,
                                           (xpm_map_t)p1_left_3, (xpm_map_t)p1_left_2,
                                           (xpm_map_t)p1_left_1, (xpm_map_t)p1_left_4,
                                           (xpm_map_t)p1_left_5, (xpm_map_t)p1_left_4);
                                          
  loader->player2_right = create_anim_sprite(2, 8, (xpm_map_t)p1_right_1, (xpm_map_t)p1_right_2,
                                            (xpm_map_t)p1_right_3, (xpm_map_t)p1_right_2,
                                            (xpm_map_t)p1_right_1, (xpm_map_t)p1_right_4,
                                            (xpm_map_t)p1_right_5, (xpm_map_t)p1_right_4);

  loader->player2_up = create_anim_sprite(2, 8, (xpm_map_t)p1_back_1, (xpm_map_t)p1_back_2,
                                          (xpm_map_t)p1_back_3, (xpm_map_t)p1_back_2,
                                          (xpm_map_t)p1_back_1, (xpm_map_t)p1_back_4,
                                          (xpm_map_t)p1_back_5, (xpm_map_t)p1_back_4);  

  loader->player2_down = create_anim_sprite(2, 8, (xpm_map_t)p1_front_1, (xpm_map_t)p1_front_2,
                                            (xpm_map_t)p1_front_3, (xpm_map_t)p1_front_2,
                                            (xpm_map_t)p1_front_1, (xpm_map_t)p1_front_4,
                                            (xpm_map_t)p1_front_5, (xpm_map_t)p1_front_4);
  // Load bomb and wall sprites
  loader->bomb = create_anim_sprite(6, 10, (xpm_map_t)bomb_xpm_1, (xpm_map_t)bomb_xpm_2, 
                                    (xpm_map_t)bomb_xpm_3, (xpm_map_t)bomb_xpm_4,
                                    (xpm_map_t)bomb_xpm_5, (xpm_map_t)bomb_xpm_6,
                                    (xpm_map_t)bomb_xpm_7, (xpm_map_t)bomb_xpm_8,
                                    (xpm_map_t)bomb_xpm_9, (xpm_map_t)bomb_xpm_10);

  loader->explosion = create_anim_sprite(1, 18, (xpm_map_t)explosion_xpm_9, (xpm_map_t)explosion_xpm_8,
                                          (xpm_map_t)explosion_xpm_7, (xpm_map_t)explosion_xpm_6,
                                          (xpm_map_t)explosion_xpm_5, (xpm_map_t)explosion_xpm_4,
                                          (xpm_map_t)explosion_xpm_3, (xpm_map_t)explosion_xpm_2,
                                          (xpm_map_t)explosion_xpm_1, (xpm_map_t)explosion_xpm_1,
                                          (xpm_map_t)explosion_xpm_2, (xpm_map_t)explosion_xpm_3,
                                          (xpm_map_t)explosion_xpm_4, (xpm_map_t)explosion_xpm_5,
                                          (xpm_map_t)explosion_xpm_6, (xpm_map_t)explosion_xpm_7,
                                          (xpm_map_t)explosion_xpm_8, (xpm_map_t)explosion_xpm_9);
  loader->wall = create_sprite( (xpm_map_t)wall_xpm, BY_PIXEL);
  loader->solid_wall = create_sprite( (xpm_map_t)solid_wall_xpm, BY_PIXEL);

  loader->game_background = create_sprite((xpm_map_t)game_background, FULL_IMAGE);
  loader->grid_background = create_sprite((xpm_map_t)grid_background, BY_LINE);
  loader->cursor = create_sprite((xpm_map_t)cursor_xpm, BY_PIXEL);

  loader->menu_background = create_sprite((xpm_map_t)back_menu_xpm, FULL_IMAGE); 
  loader->menu_title = create_sprite((xpm_map_t)title_menu_xpm, BY_PIXEL); 
  loader->single_mode_button = create_sprite((xpm_map_t)single_menu_xpm, BY_PIXEL); 
  loader->multiplayer_mode_button = create_sprite((xpm_map_t)multi_menu_xpm, BY_PIXEL);
  loader->exit_button = create_sprite((xpm_map_t)exit_menu_xpm, BY_PIXEL); 
  loader->exit = create_sprite((xpm_map_t)exit_sp, BY_PIXEL);

  loader->bomb_options[0] = create_sprite((xpm_map_t)bomb_option_1, BY_PIXEL);
  loader->bomb_options[1] = create_sprite((xpm_map_t)bomb_option_2, BY_PIXEL);
  loader->bomb_options[2] = create_sprite((xpm_map_t)bomb_option_3, BY_PIXEL);
  loader->selected_options[0] = create_sprite((xpm_map_t)selected_bomb1, BY_PIXEL);
  loader->selected_options[1] = create_sprite((xpm_map_t)selected_bomb2, BY_PIXEL);
  loader->selected_options[2] = create_sprite((xpm_map_t)selected_bomb3, BY_PIXEL);

  loader->died_title = create_sprite((xpm_map_t)died_title_xpm, BY_PIXEL);
  loader->died_background = create_sprite((xpm_map_t)died_background_xpm, BY_LINE);
  loader->play_again = create_sprite((xpm_map_t)play_again_xpm, BY_PIXEL);
  loader->back_to_menu = create_sprite((xpm_map_t)back_to_menu_xpm, BY_PIXEL);
  loader->your_score = create_sprite((xpm_map_t)your_score_xpm, BY_PIXEL);

  loader->number_0 = create_sprite((xpm_map_t)number_0_xpm, BY_PIXEL);
  loader->number_1 = create_sprite((xpm_map_t)number_1_xpm, BY_PIXEL);
  loader->number_2 = create_sprite((xpm_map_t)number_2_xpm, BY_PIXEL);
  loader->number_3 = create_sprite((xpm_map_t)number_3_xpm, BY_PIXEL);
  loader->number_4 = create_sprite((xpm_map_t)number_4_xpm, BY_PIXEL);
  loader->number_5 = create_sprite((xpm_map_t)number_5_xpm, BY_PIXEL);
  loader->number_6 = create_sprite((xpm_map_t)number_6_xpm, BY_PIXEL);
  loader->number_7 = create_sprite((xpm_map_t)number_7_xpm, BY_PIXEL);
  loader->number_8 = create_sprite((xpm_map_t)number_8_xpm, BY_PIXEL);
  loader->number_9 = create_sprite((xpm_map_t)number_9_xpm, BY_PIXEL);
  loader->warning_connection = create_sprite((xpm_map_t)warning_connection_xpm, BY_PIXEL);
  loader->waiting_connection = create_sprite((xpm_map_t)waiting_connection_xpm, BY_PIXEL);
  loader->winners_background = create_sprite((xpm_map_t)died_background_xpm, BY_LINE);
  loader->you_won = create_sprite((xpm_map_t)you_won_xpm, BY_PIXEL);
  loader->you_lost = create_sprite((xpm_map_t)you_lost_xpm, BY_PIXEL);
  loader->draw = create_sprite((xpm_map_t)draw_xpm, BY_PIXEL);
  return loader;
}

void destroy_sprites(SpriteLoader *loader) {
  if (loader == NULL) {
    return;
  }

  destroy_anim_sprite(loader->player1_left);
  destroy_anim_sprite(loader->player1_right);
  destroy_anim_sprite(loader->player1_up);
  destroy_anim_sprite(loader->player1_down);
  destroy_anim_sprite(loader->player2_left);
  destroy_anim_sprite(loader->player2_right);
  destroy_anim_sprite(loader->player2_up);
  destroy_anim_sprite(loader->player2_down);
  destroy_anim_sprite(loader->bomb);
  destroy_sprite(loader->wall);
  destroy_sprite(loader->solid_wall);
  destroy_sprite(loader->game_background);
  destroy_sprite(loader->grid_background);
  destroy_sprite(loader->cursor);
  destroy_sprite(loader->menu_background);
  destroy_sprite(loader->menu_title);
  destroy_sprite(loader->single_mode_button);
  destroy_sprite(loader->multiplayer_mode_button);
  destroy_sprite(loader->exit_button);
  destroy_sprite(loader->exit);
  destroy_sprite(loader->died_title);
  destroy_sprite(loader->died_background);
  destroy_sprite(loader->back_to_menu);
  destroy_sprite(loader->play_again);
  destroy_sprite(loader->your_score);
  destroy_anim_sprite(loader->explosion);
  destroy_sprite(loader->player1_standing);
  destroy_sprite(loader->player2_standing);
  destroy_sprite(loader->number_0);
  destroy_sprite(loader->number_1);
  destroy_sprite(loader->number_2); 
  destroy_sprite(loader->number_3);
  destroy_sprite(loader->number_4);
  destroy_sprite(loader->number_5);
  destroy_sprite(loader->number_6);
  destroy_sprite(loader->number_7);
  destroy_sprite(loader->number_8);
  destroy_sprite(loader->number_9);
  destroy_sprite(loader->winners_background);
  destroy_sprite(loader->you_won);
  destroy_sprite(loader->you_lost);
  destroy_sprite(loader->draw);


  for (int i = 0; i < 3; i++) {
    destroy_sprite(loader->bomb_options[i]);
    destroy_sprite(loader->selected_options[i]);
  }

  free(loader);
}

Sprite* get_player1_standing(SpriteLoader *loader) {
  return loader->player1_standing;
}

Sprite* get_player2_standing(SpriteLoader *loader) {
  return loader->player2_standing;
}

AnimSprite* get_player1_left(SpriteLoader *loader) {
  return loader->player1_left;
}
AnimSprite* get_player1_right(SpriteLoader *loader) {
  return loader->player1_right;
}
AnimSprite* get_player1_up(SpriteLoader *loader) {
  return loader->player1_up;
}
AnimSprite* get_player1_down(SpriteLoader *loader) {
  return loader->player1_down;
}
AnimSprite* get_player2_left(SpriteLoader *loader) {
  return loader->player2_left;
}
AnimSprite* get_player2_right(SpriteLoader *loader) {
  return loader->player2_right;
}
AnimSprite* get_player2_up(SpriteLoader *loader) {
  return loader->player2_up;
}
AnimSprite* get_player2_down(SpriteLoader *loader) {
  return loader->player2_down;
}
AnimSprite* get_bomb(SpriteLoader *loader) {
  return loader->bomb;
}
Sprite* get_wall(SpriteLoader *loader) {
  return loader->wall;
}
Sprite* get_solid_wall(SpriteLoader *loader) {
  return loader->solid_wall;
}
Sprite* get_grid_background(SpriteLoader *loader) {
  return loader->grid_background;
}
Sprite* get_cursor(SpriteLoader *loader) {
  return loader->cursor;
}
AnimSprite* get_explosion(SpriteLoader *loader) {
  return loader->explosion;
}
Sprite* get_menu_background(SpriteLoader *loader) {
  return loader->menu_background;
}
Sprite* get_menu_title(SpriteLoader *loader) {
  return loader->menu_title;
}
Sprite* get_single_mode_button(SpriteLoader *loader) {
  return loader->single_mode_button;
}
Sprite* get_multi_mode_button(SpriteLoader *loader) {
  return loader->multiplayer_mode_button;
}
Sprite* get_exit_button(SpriteLoader *loader) {
  return loader->exit_button;
}
Sprite* get_exit(SpriteLoader *loader) {
  return loader->exit;
}
Sprite** get_bomb_options(SpriteLoader *loader) {
  return loader->bomb_options;
}
Sprite** get_selected_options(SpriteLoader *loader) {
  return loader->selected_options;
}
Sprite* get_game_background(SpriteLoader *loader) {
  return loader->game_background;
}

Sprite* get_died_title(SpriteLoader *loader) {
  return loader->died_title;
}

Sprite* get_died_background(SpriteLoader *loader) {
  return loader->died_background;
}

Sprite* get_play_again(SpriteLoader *loader) {
  return loader->play_again;
}

Sprite* get_back_to_menu(SpriteLoader *loader) {
  return loader->back_to_menu;
}

Sprite* get_your_score_text(SpriteLoader *loader) {
  return loader->your_score;
}

Sprite* get_number_sprite(SpriteLoader* loader, int number) {
  switch (number) {
    case 0: return loader->number_0;
    case 1: return loader->number_1;
    case 2: return loader->number_2;
    case 3: return loader->number_3;
    case 4: return loader->number_4;
    case 5: return loader->number_5;
    case 6: return loader->number_6;
    case 7: return loader->number_7;
    case 8: return loader->number_8;
    case 9: return loader->number_9;
    default: return NULL; // Invalid number
  }
}

Sprite* get_warning_connection(SpriteLoader *loader) {
  return loader->warning_connection;
}
Sprite* get_waiting_connection(SpriteLoader *loader) {
  return loader->waiting_connection;
}
Sprite* get_winners_background(SpriteLoader *loader) {
  return loader->winners_background;
}
Sprite* get_winners_you_won(SpriteLoader *loader) {
  return loader->you_won;
}
Sprite* get_winners_you_lost(SpriteLoader *loader) {
  return loader->you_lost;
}
Sprite* get_winners_draw(SpriteLoader *loader) {
  return loader->draw;
}
