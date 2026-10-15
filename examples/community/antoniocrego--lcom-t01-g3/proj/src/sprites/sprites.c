#include "sprites.h"

Sprite *cursor;
Sprite *character;
Sprite *title1;
Sprite *title2;
Sprite *daybg;
Sprite *nightbg;
Sprite *level1;
Sprite *level2;
Sprite *level3;
Sprite *level4;
Sprite *level5;
Sprite *level6;
Sprite *level7;
Sprite *paused1;
Sprite *paused2;
Sprite *paused3;
Sprite *jump_powerup;
Sprite *soul_blast_powerup;
Sprite *soul_blast_atk;
Sprite *mouse;
Sprite *attack_anim;
Sprite *bully1;
Sprite *crawlie1;
Sprite *fly1;
Sprite *fly_knight1;
Sprite *gunk_bully1;
Sprite *hpfull;
Sprite *hpempty;
Sprite *game_over;
Sprite *boss;
Sprite *boss_attack;
Sprite *boss_crouch;
Sprite *boss_stand;
Sprite *victory;

Sprite *createSprite(xpm_map_t sprite){
    Sprite *sp = (Sprite *) malloc (sizeof(Sprite));
    if( sp == NULL ) return NULL;

    xpm_image_t img;
    sp->colors = (uint32_t *) xpm_load(sprite, XPM_8_8_8_8, &img);
    sp->height = img.height;
    sp->width = img.width;

    if( sp->colors == NULL ) {
        free(sp);
        return NULL;
    }

    return sp;
}

int drawSprite(Sprite *sp, int x, int y){
    uint16_t height = sp->height, width = sp->width;
    for (int h = 0 ; h < height ; h++) {
      for (int w = 0 ; w < width ; w++) {
        if (paint_pixel(x + w, y + h, sp->colors[w + h*width])) return 1;
      }
    }
    return 0; 
}

int draw_sprite_flipped(Sprite *sp, int x, int y){
    uint16_t height = sp->height, width = sp->width;
    for (int h = 0 ; h < height ; h++) {
      for (int w = 0 ; w < width ; w++) {
        if (paint_pixel(x + width - w, y + h, sp->colors[w + h*width])) return 1;
      }
    }
    return 0; 
}

int draw_sprite_down(Sprite *sp, int x, int y){
    uint16_t height = sp->height, width = sp->width;
    for (int h = 0 ; h < height ; h++) {
      for (int w = 0 ; w < width ; w++) {
        if (paint_pixel(x+h, y+w, sp->colors[w + h*width])) return 1;
      }
    }
    return 0; 
}

int draw_sprite_up(Sprite *sp, int x, int y){
    uint16_t height = sp->height, width = sp->width;
    for (int h = 0 ; h < height ; h++) {
      for (int w = 0 ; w < width ; w++) {
        if (paint_pixel(x+h, y+width-w, sp->colors[w + h*width])) return 1;
      }
    }
    return 0; 
}

void destroy_sprite(Sprite *sp){
    if(sp == NULL) {
      return;
    }
    else if(sp->colors){
        free(sp->colors);
    }
    
    free(sp);
    sp = NULL;
}

void loadSprites(){
  cursor=createSprite((xpm_map_t) cursor_xpm);
  character=createSprite((xpm_map_t) char_xpm);
  title1=createSprite((xpm_map_t) title1_xpm);
  title2=createSprite((xpm_map_t) title2_xpm);
  daybg=createSprite((xpm_map_t) daybg_xpm);
  nightbg=createSprite((xpm_map_t) nightbg_xpm);
  level1=createSprite((xpm_map_t) level1_xpm);
  level2=createSprite((xpm_map_t) level2_xpm);
  level3=createSprite((xpm_map_t) level3_xpm);
  level4=createSprite((xpm_map_t) level4_xpm);
  level5=createSprite((xpm_map_t) level5_xpm);
  level6=createSprite((xpm_map_t) level6_xpm);
  level7=createSprite((xpm_map_t) level7_xpm);
  paused1=createSprite((xpm_map_t) paused1_xpm);
  paused2=createSprite((xpm_map_t) paused2_xpm);
  paused3=createSprite((xpm_map_t) paused3_xpm);
  jump_powerup=createSprite((xpm_map_t) jump_powerup_xpm);
  soul_blast_powerup=createSprite((xpm_map_t) soul_blast_powerup_xpm);
  soul_blast_atk=createSprite((xpm_map_t) soul_blast_atk_xpm);
  attack_anim=createSprite((xpm_map_t) attack_anim_xpm);
  mouse=createSprite((xpm_map_t) mouse_xpm);
  bully1=createSprite((xpm_map_t) bully1_xpm);
  crawlie1 = createSprite((xpm_map_t) crawlie1_xpm);
  fly1 = createSprite((xpm_map_t) fly1_xpm);
  fly_knight1 = createSprite((xpm_map_t) fly_knight1_xpm);
  gunk_bully1 = createSprite((xpm_map_t) gunk_bully1_xpm);

  hpfull=createSprite((xpm_map_t) full_hp);
  hpempty=createSprite((xpm_map_t) empty_hp);
  game_over=createSprite((xpm_map_t) game_over_xpm);
  boss=createSprite((xpm_map_t) boss_xpm);
  boss_attack=createSprite((xpm_map_t) boss_attack_xpm);
  boss_crouch=createSprite((xpm_map_t) boss_crouch_xpm);
  boss_stand=createSprite((xpm_map_t) boss_stand_xpm);

  victory=createSprite((xpm_map_t) victory_xpm);
}

void unloadSprites(){
  destroy_sprite(cursor);
  destroy_sprite(character);
  destroy_sprite(title1);
  destroy_sprite(title2);
  destroy_sprite(daybg);
  destroy_sprite(nightbg);
  destroy_sprite(level1);
  destroy_sprite(level2);
  destroy_sprite(level3);
  destroy_sprite(level4);
  destroy_sprite(level5);
  destroy_sprite(level6);
  destroy_sprite(level7);
  destroy_sprite(paused1);
  destroy_sprite(paused2);
  destroy_sprite(paused3);
  destroy_sprite(jump_powerup);
  destroy_sprite(soul_blast_powerup);
  destroy_sprite(soul_blast_atk);
  destroy_sprite(attack_anim);
  destroy_sprite(mouse);
  destroy_sprite(bully1);
  destroy_sprite(gunk_bully1);
  destroy_sprite(fly1);
  destroy_sprite(fly_knight1);
  destroy_sprite(crawlie1);
  destroy_sprite(game_over);
  destroy_sprite(boss);
  destroy_sprite(boss_attack);
  destroy_sprite(boss_stand);
  destroy_sprite(boss_crouch);
  destroy_sprite(victory);
}

