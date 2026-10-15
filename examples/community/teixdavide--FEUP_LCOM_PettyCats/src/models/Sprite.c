#include <lcom/lcf.h>

#include "Sprite.h"


Sprite characters[CHAR_COUNT];

Sprite menu_instructions;
Sprite menu_logo;
Sprite menu_play_game;
Sprite menu_quit;


Sprite load_generic_sprite(xpm_map_t xpm_map) {
  Sprite sprite;
  xpm_image_t img;
  sprite.sprite = xpm_load(xpm_map, XPM_INDEXED, &img);
  if (sprite.sprite == NULL) {
    printf("load_generic_sprite(): error loading sprite\n");
    return sprite;
  }

  sprite.width = img.width;
  sprite.height = img.height;

  return sprite;
}

int load_char_sprites() {

  for (int i = 0; i < character_count; i++) {
    characters[i] = load_generic_sprite(xpm_map_characters[i]);
    if (characters[i].sprite == NULL) {
      printf("load_char_sprites(): error loading character sprite\n");
      return 1;
    }
  }

  return 0;
}

int load_menu_sprites() {

  menu_instructions = load_generic_sprite((xpm_map_t) instructions);
  if (menu_instructions.sprite == NULL) {
    printf("load_menu_sprites(): error loading menu_instructions sprite\n");
    return 1;
  }

  menu_logo = load_generic_sprite((xpm_map_t) logo);
  if (menu_logo.sprite == NULL) {
    printf("load_menu_sprites(): error loading menu_logo sprite\n");
    return 1;
  }

  menu_play_game = load_generic_sprite((xpm_map_t) play_game);
  if (menu_play_game.sprite == NULL) {
    printf("load_menu_sprites(): error loading menu_play_game sprite\n");
    return 1;
  }

  menu_quit = load_generic_sprite((xpm_map_t) quit);
  if (menu_quit.sprite == NULL) {
    printf("load_menu_sprites(): error loading menu_quit sprite\n");
    return 1;
  }

  return 0;
}
