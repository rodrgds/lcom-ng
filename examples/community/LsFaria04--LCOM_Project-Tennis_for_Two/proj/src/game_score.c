#include "game_score.h"

static Sprite p1;
static Sprite p2;
static Sprite p1Score;
static Sprite p2Score;

int (loadInitialXPMScore)(){
  xpm_image_t img;
  Sprite *sprite = (Sprite*)malloc(sizeof(Sprite));

  sprite->map = (uint32_t *)malloc(sizeof(char*));

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) p1_xpm, XPM_8_8_8_8, &img);
  sprite->height = img.height;
  sprite->width = img.width;
  p1 = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) p2_xpm, XPM_8_8_8_8, &img);
  sprite->height = img.height;
  sprite->width = img.width;
  p2 = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) r0_xpm, XPM_8_8_8_8, &img);
  sprite->height = img.height;
  sprite->width = img.width;
  p1Score = *sprite;

  sprite->map = (uint32_t *) xpm_load((xpm_map_t) r0_xpm, XPM_8_8_8_8, &img);
  sprite->height = img.height;
  sprite->width = img.width;
  p2Score = *sprite;

  if((p1.map == NULL) || (p2.map == NULL) || (p1Score.map == NULL) || (p2Score.map == NULL)){
    return EXIT_FAILURE;
  }

  free(sprite);

  return EXIT_SUCCESS;
}

int (drawScore)(int playerNumb){
  int width = 0;
  int height = 0;
  uint32_t *map = NULL;
  switch(playerNumb){
    case 1:
      width = p1Score.width;
      height = p1Score.height;
      map = p1Score.map;

      for(int j =  230 ; j < 230 + height; j++){
        for(int i = 1035 ; i < 1035 + width; i++){
          if(*map != TRANSPARENCY_SCORE){
            if(vg_draw_color(i,j,*map) != 0){
              return EXIT_FAILURE;
            }
          }
          map++;
        }
      }
      return EXIT_SUCCESS;
    
    case 2:
      width = p2Score.width;
      height = p2Score.height;
      map = p2Score.map;

      for(int j =  130 ; j < 130 + height; j++){
        for(int i = 1035 ; i < 1035 + width; i++){
          if(*map != TRANSPARENCY_SCORE){
            if(vg_draw_color(i,j,*map) != 0){
              return EXIT_FAILURE;
            }
          }
          map++;
        }
      }
      return EXIT_SUCCESS;
    
    default:
      return EXIT_FAILURE;
  }
}

int (drawScoreText)(){
  int width = p1.width;
  int height = p1.height;
  uint32_t *map = p1.map;

  for(int j =  230 ; j < 230 + height; j++){
    for(int i = 950 ; i < 950 + width; i++){
      if(*map != TRANSPARENCY_SCORE){
        if(vg_draw_color(i,j,*map) != 0){
          return EXIT_FAILURE;
        }
      }
      map++;
    }
  }

  width = p2.width;
  height = p2.height;
  map = p2.map;
    
  for(int j =  130 ; j < 130 + height; j++){
      for(int i = 950 ; i < 950 + width; i++){
        if(*map != TRANSPARENCY_SCORE){
          if(vg_draw_color(i,j,*map) != 0){
            return EXIT_FAILURE;
          }
        }
        map++;
      }
    }
    return EXIT_SUCCESS;
}

int (updateXPMScore)(int playerNumb, int score){
  

  switch (playerNumb)
  {
  case 1:
    chooseXPMScore(score, &p1Score);

    if(p1Score.map == NULL){
      return EXIT_FAILURE;
    }

    break;

  case 2:

    chooseXPMScore(score, &p2Score);

    if(p2Score.map == NULL){
      return EXIT_FAILURE;
    }

    break;
  default:
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;

}

void (chooseXPMScore)(int score, Sprite *scoreSprite){
  xpm_image_t img;

  switch(score){
    case 0:
      scoreSprite->map = (uint32_t *) xpm_load((xpm_map_t) r0_xpm, XPM_8_8_8_8, &img);
      scoreSprite->height = img.height;
      scoreSprite->width = img.width;
      break;
    case 1:
     scoreSprite->map = (uint32_t *) xpm_load((xpm_map_t) r1_xpm, XPM_8_8_8_8, &img);
     scoreSprite->height = img.height;
      scoreSprite->width = img.width;
      break;
    case 2:
      scoreSprite->map = (uint32_t *) xpm_load((xpm_map_t) r2_xpm, XPM_8_8_8_8, &img);
      scoreSprite->height = img.height;
      scoreSprite->width = img.width;
      break;
    case 3:
      scoreSprite->map = (uint32_t *) xpm_load((xpm_map_t) r3_xpm, XPM_8_8_8_8, &img);
      scoreSprite->height = img.height;
      scoreSprite->width = img.width;
      break;
    case 4:
      scoreSprite->map = (uint32_t *) xpm_load((xpm_map_t) r4_xpm, XPM_8_8_8_8, &img);
      scoreSprite->height = img.height;
      scoreSprite->width = img.width;
      break;
    case 5:
      scoreSprite->map = (uint32_t *) xpm_load((xpm_map_t) r5_xpm, XPM_8_8_8_8, &img);
      scoreSprite->height = img.height;
      scoreSprite->width = img.width;
      break;
    case 6:
      scoreSprite->map = (uint32_t *) xpm_load((xpm_map_t) r6_xpm, XPM_8_8_8_8, &img);
      scoreSprite->height = img.height;
      scoreSprite->width = img.width;
      break;
    case 7:
      scoreSprite->map = (uint32_t *) xpm_load((xpm_map_t) r7_xpm, XPM_8_8_8_8, &img);
      scoreSprite->height = img.height;
      scoreSprite->width = img.width;
      break;
    case 8:
      scoreSprite->map = (uint32_t *) xpm_load((xpm_map_t) r8_xpm, XPM_8_8_8_8, &img);
      scoreSprite->height = img.height;
      scoreSprite->width = img.width;
      break;
    case 9:
      scoreSprite->map = (uint32_t *) xpm_load((xpm_map_t) r9_xpm, XPM_8_8_8_8, &img);
      scoreSprite->height = img.height;
      scoreSprite->width = img.width;
      break;
    default:
      //cannot have a score bigger than 9
      break;
  }
}

void (freeXPMScore)(){
  if(p1.map != NULL){
    free(p1.map);
  }

  if(p2.map != NULL){
    free(p2.map);
  }

  if(p1Score.map != NULL){
    free(p1Score.map);
  }

  if(p2Score.map != NULL){
    free(p2Score.map);
  }
}
