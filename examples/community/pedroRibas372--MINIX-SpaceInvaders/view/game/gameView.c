#include "gameView.h"

int drawlvl(){
  if(draw_xpm(lvl->map,lvl->x,lvl->y,lvl->img) != OK){return 1;}

  return 0;
}

int drawscr(){
  if(draw_xpm(scr->map,scr->x,scr->y,scr->img) != OK){return 1;}

  return 0;
}

int drawhp(){
  if(draw_xpm(hp->map,hp->x,hp->y,hp->img) != OK){return 1;}

  return 0;
}

int draw_number_value(int value, int digits, int x, int y) {

    int width = digits * 55;
    int height = 50;
    draw_rectangle(x, y, width, height, 0); 
  
    int temp = value;
    int digit;
    Sprite* numberSprite;
  
    for (int i = digits - 1; i >= 0; i--) {
      digit = temp % 10;
      temp /= 10;
  
      numberSprite = create_static(numbers[digit], x + i * 55, y);
      if (!numberSprite) return 1;
      if (draw_xpm(numberSprite->map, numberSprite->x, numberSprite->y, numberSprite->img) != OK)
        return 1;
      destroy_sprite(numberSprite);
    }
    return 0;
}


int drawHeader(){
    if(drawhp()!= OK){return 1;}
    if(drawscr()!= OK){return 1;}
    if(drawlvl()!= OK){return 1;}
    if (draw_number_value(score, 3, scr->x+165, scr->y) != OK) return 1;
    if (draw_number_value(level, 1, lvl->x +165, lvl->y) != OK) return 1;
    if (draw_number_value(health, 1, hp->x + 165, hp->y) != OK) return 1;
    return 0;
  }


  int drawEnemies(){
    for(int i=0; i< indexEnemies; i++){
      Sprite* enemy = enemies[i];
      if (enemy->dead) {
        continue;
      }
      if (draw_xpm(enemy->map, enemy->x, enemy->y, enemy->img) != OK){
        return 1;
      }
    }
  
    return 0;
  
  }
  
  int drawHero(){
    if(draw_xpm(hero->map,hero->x,hero->y,hero->img) != OK){return 1;}
  
    return 0;
  }
  
  int drawGame(){
    if(drawEnemies() != OK){return 1;}
    if(drawHero()!= OK){return 1;}
  
    return 0;
  }
