#include "../inc/sprite.h"

extern bool change_vram_mode;

Sprite *create_sprite(xpm_map_t sprite) {
Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
if( sp == NULL ) return NULL;

xpm_image_t img;
sp->colors = (uint32_t *) xpm_load(sprite, XPM_8_8_8_8, &img);

if( sp->colors == NULL ) {
  free(sp);
  return NULL;
}
sp->width = img.width; 
sp->height=img.height;

return sp;
}

void destroy_sprite(Sprite *sp) {
    if( sp == NULL )
        return;
    if( sp ->colors )
        free(sp->colors);
    free(sp);
    sp = NULL;
}

void create_sprites_menu(){
    if(!change_vram_mode)
  {
  start_O = create_sprite((xpm_map_t) startSel);
  start_W = create_sprite((xpm_map_t) startUnsel);
  ins_O = create_sprite((xpm_map_t) insSel);
  ins_W = create_sprite((xpm_map_t) insUns);
  exit_O = create_sprite((xpm_map_t) exitSel);
  exit_W = create_sprite((xpm_map_t) exitUns);
  title = create_sprite((xpm_map_t) titlelarger_xpm);
  insGen = create_sprite((xpm_map_t) INSTRUCTIONS_xpm);
  }
else{
  start_O = create_sprite((xpm_map_t) start_game_OL);
  start_W = create_sprite((xpm_map_t) start_game_WL);
  ins_O = create_sprite((xpm_map_t) instructions_OL);
  ins_W = create_sprite((xpm_map_t) instructions_WL);
  exit_O = create_sprite((xpm_map_t) exit_OL);
  exit_W = create_sprite((xpm_map_t) exit_WL);
  title = create_sprite((xpm_map_t) titlelarger_xpm);
  insGen = create_sprite((xpm_map_t) generalinsL_xpm);
}
}

void create_sprites_game(){

if(!change_vram_mode)
  {
  bloc = create_sprite((xpm_map_t) block40); 
  bbloc =create_sprite((xpm_map_t) bblock40);
  steven = create_sprite((xpm_map_t) steven40);
  bomb = create_sprite((xpm_map_t) orangeGem);
  explosion = create_sprite((xpm_map_t) bombExploding);
  monster = create_sprite((xpm_map_t) greenMonster);
  life = create_sprite((xpm_map_t) life_full);
  iblock = create_sprite((xpm_map_t) black_block);
  up_monster = create_sprite((xpm_map_t) redMonster);
  lost_life = create_sprite((xpm_map_t) life_lost);
  unlimited = create_sprite((xpm_map_t) unlimited_xpm);
  arrow_mouse = create_sprite((xpm_map_t) mouse_xpm);
  black = create_sprite((xpm_map_t) black_xpm);
  bar = create_sprite((xpm_map_t) bar_xpm);
  zero = create_sprite((xpm_map_t) zero_xpm);
  one = create_sprite((xpm_map_t) one_xpm);
  two = create_sprite((xpm_map_t) two_xpm);
  three = create_sprite((xpm_map_t) three_xpm);
  four = create_sprite((xpm_map_t) four_xpm);
  five = create_sprite((xpm_map_t) five_xpm);
  six = create_sprite((xpm_map_t) six_xpm);
  seven = create_sprite((xpm_map_t) seven_xpm);
  eight = create_sprite((xpm_map_t) eight_xpm);
  nine = create_sprite((xpm_map_t) nine_xpm);
  }
else{
  bloc = create_sprite((xpm_map_t) block57_xpm); 
  bbloc =create_sprite((xpm_map_t) bblock57_xpm);
  steven = create_sprite((xpm_map_t) steven57_xpm);
  bomb = create_sprite((xpm_map_t) orange57_xpm);
  explosion = create_sprite((xpm_map_t) pexplo57_xpm);
  monster = create_sprite((xpm_map_t) green57_xpm);
  life = create_sprite((xpm_map_t) life57_xpm);
  iblock = create_sprite((xpm_map_t) iblock57_xpm);
  up_monster = create_sprite((xpm_map_t) red57_xpm);
  lost_life = create_sprite((xpm_map_t) unlife57_xpm);
  unlimited = create_sprite((xpm_map_t) purple57_xpm);
  arrow_mouse = create_sprite((xpm_map_t) mouse_xpm);
  black = create_sprite((xpm_map_t) black8_xpm);
  bar = create_sprite((xpm_map_t) bar_xpm);
  zero = create_sprite((xpm_map_t) zero35_xpm);
  one = create_sprite((xpm_map_t) one35_xpm);
  two = create_sprite((xpm_map_t) two35_xpm);
  three = create_sprite((xpm_map_t) three35_xpm);
  four = create_sprite((xpm_map_t) four35_xpm);
  five = create_sprite((xpm_map_t) five35_xpm);
  six = create_sprite((xpm_map_t) six35_xpm);
  seven = create_sprite((xpm_map_t) seven35_xpm);
  eight = create_sprite((xpm_map_t) eight35_xpm);
  nine = create_sprite((xpm_map_t) nine35_xpm);
}
}

void destroy_sprites(){

  destroy_sprite(steven);
  destroy_sprite(bloc);
  destroy_sprite(bbloc);
  destroy_sprite(monster);
  destroy_sprite(explosion);
  destroy_sprite(bomb);
  destroy_sprite(life);
  destroy_sprite(iblock);
  destroy_sprite(up_monster);
  destroy_sprite(lost_life);
  destroy_sprite(start_O);
  destroy_sprite(start_W);
  destroy_sprite(ins_O);
  destroy_sprite(ins_W);
  destroy_sprite(gameover);
  destroy_sprite(unlimited);
  destroy_sprite(arrow_mouse);
  destroy_sprite(gameWin);
  destroy_sprite(one);
  destroy_sprite(two);
  destroy_sprite(three);
  destroy_sprite(four);
  destroy_sprite(five);
  destroy_sprite(six);
  destroy_sprite(seven);
  destroy_sprite(eight);
  destroy_sprite(nine);
  destroy_sprite(zero);
  destroy_sprite(black);
  destroy_sprite(bar);
}


