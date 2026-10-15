#include "sprite.h"
#include "../model/xpm/xpm.h"

static enum xpm_image_type xpm_img_type = XPM_8_8_8_8;

Sprite* black_queen;
Sprite* white_queen;
Sprite* black_king;
Sprite* white_king;
Sprite* black_rook;
Sprite* white_rook;
Sprite* black_bishop;
Sprite* white_bishop;
Sprite* black_knight;
Sprite* white_knight;
Sprite* black_pawn;
Sprite* white_pawn;

Sprite* mouse;

Sprite* numbers;
Sprite* numbers_small;
Sprite* slash;

Sprite* alphabet;
Sprite* alphabet_small;

Sprite* exitButton;
Sprite* playButton;
Sprite* chessBackground;

int load_sprites(){

  black_queen = (Sprite*) malloc(sizeof(Sprite));
  if (load_sprite(black_queen, (xpm_map_t) blackQueen_xpm))
    return 1; 

  white_queen = (Sprite*) malloc(sizeof(Sprite));
  if (load_sprite(white_queen, (xpm_map_t) whiteQueen_xpm))
    return 1;

  black_king = (Sprite*) malloc(sizeof(Sprite));
  if (load_sprite(black_king, (xpm_map_t) blackKing_xpm))
    return 1;

  white_king = (Sprite*) malloc(sizeof(Sprite));
  if (load_sprite(white_king, (xpm_map_t) whiteKing_xpm))
    return 1;

  black_rook = (Sprite*) malloc(sizeof(Sprite));
  if (load_sprite(black_rook, (xpm_map_t) blackRook_xpm))
    return 1;

  white_rook = (Sprite*) malloc(sizeof(Sprite));
  if (load_sprite(white_rook, (xpm_map_t) whiteRook_xpm))
    return 1;

  black_bishop = (Sprite*) malloc(sizeof(Sprite));
  if (load_sprite(black_bishop, (xpm_map_t) blackBishop_xpm))
    return 1;

  white_bishop = (Sprite*) malloc(sizeof(Sprite));
  if (load_sprite(white_bishop, (xpm_map_t) whiteBishop_xpm))
    return 1;

  black_knight = (Sprite*) malloc(sizeof(Sprite));
  if (load_sprite(black_knight, (xpm_map_t) blackKnight_xpm))
    return 1;

  white_knight = (Sprite*) malloc(sizeof(Sprite));
  if (load_sprite(white_knight, (xpm_map_t) whiteKnight_xpm))
    return 1;

  black_pawn = (Sprite*) malloc(sizeof(Sprite));
  if (load_sprite(black_pawn, (xpm_map_t) blackPawn_xpm))
    return 1;

  white_pawn = (Sprite*) malloc(sizeof(Sprite));
  if (load_sprite(white_pawn, (xpm_map_t) whitePawn_xpm))
    return 1;

  mouse = (Sprite*) malloc(sizeof(Sprite));
  if (load_sprite(mouse, (xpm_map_t) mouse_xpm))
    return 1;

  numbers = (Sprite*) malloc(sizeof(Sprite));
  if (load_sprite(numbers, (xpm_map_t) numbers_xpm))
    return 1;

  numbers_small = (Sprite*) malloc(sizeof(Sprite));
  if (load_sprite(numbers_small, (xpm_map_t) numbers_small_xpm))
    return 1;

  slash = (Sprite*) malloc(sizeof(Sprite));
  if (load_sprite(slash, (xpm_map_t) slash_xpm))
    return 1;

  alphabet = (Sprite*) malloc(sizeof(Sprite));
  if (load_sprite(alphabet, (xpm_map_t) alphabet_courier_xpm))
    return 1;

  alphabet_small = (Sprite*) malloc(sizeof(Sprite));
  if (load_sprite(alphabet_small, (xpm_map_t) alphabet_small_xpm))
    return 1;

  exitButton = (Sprite*) malloc(sizeof(Sprite));
  if (load_sprite(exitButton, (xpm_map_t) exitButtonMenu_xpm))
    return 1;

  playButton = (Sprite*) malloc(sizeof(Sprite));
  if (load_sprite(playButton, (xpm_map_t) playButtonMenu_xpm))
    return 1;

  chessBackground = (Sprite*) malloc(sizeof(Sprite));
  if (load_sprite(chessBackground, (xpm_map_t) chessBackground_xpm))
    return 1;

  return 0;
}

void (destroy_sprites)(){

  free(black_queen);
  free(white_queen);
  free(black_king);
  free(white_king);
  free(black_rook);
  free(white_rook);
  free(black_bishop);
  free(white_bishop);
  free(black_knight);
  free(white_knight);
  free(black_pawn);
  free(white_pawn);

  free(mouse);

  free(numbers);
  free(numbers_small);
  free(slash);

  free(alphabet);
  free(alphabet_small);

  free(exitButton);
  free(playButton);

  free(chessBackground);
}

int load_sprite(Sprite* sprite, xpm_map_t xpm){

  uint8_t* addr;
  xpm_image_t sprite_img;

  if (load_xpm(xpm, xpm_img_type, &sprite_img, &addr)){
    return 1;
  }

  sprite->addr = addr;
  sprite->img = sprite_img;

  return 0;
}
