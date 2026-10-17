#include <lcom/lcf.h>
#include "coins.h"


void updateCoins(Game * g,Tux* t){
  int ty0 = t->ypos+TUX_COLLISION_TOLERANCE_Y;
  if(t->currentsprite==5||t->currentsprite==11) ty0+=20;
  Area_Collision *at=newAreaCollision(t->xpos+10,t->xpos+t->sprites[t->currentsprite].width-10,ty0,t->ypos+t->sprites[t->currentsprite].height);
  for(int i=0;i<g->coins_size;i++){
    updateCoin(g->coins[i]);
    Area_Collision* ac=newAreaCollision(g->coins[i]->xpos,g->coins[i]->xpos+g->coins[i]->sprites[g->coins[i]->currentsprite].width,g->coins[i]->ypos,g->coins[i]->ypos+g->coins[i]->sprites[g->coins[i]->currentsprite].height);
    if(check_collision(at,ac)){
      deleteCoin(g->coins[i]);
      g->coins[i]=g->coins[g->coins_size-1];
      g->coins_size--;
      g->score+=COIN_SCORE;
      i--;
    }
    else if(g->coins[i]->xpos<-10){  
      deleteCoin(g->coins[i]);
      g->coins[i]=g->coins[g->coins_size-1];
      g->coins_size--;
      i--;
    }
    deleteAreaCollision(ac);
  }
  deleteAreaCollision(at);
}


void drawCoins(Game *g){
  for(int i=0;i<g->coins_size;i++){        
    drawCoin(g->coins[i]); 
  }
}


Coin * newCoin(){
  Coin *c=(Coin *)malloc(sizeof(Coin));
  xpm_load(coin_0_xpm,XPM_5_6_5,&c->sprites[0]);
  xpm_load(coin_1_xpm,XPM_5_6_5,&c->sprites[1]);
  xpm_load(coin_2_xpm,XPM_5_6_5,&c->sprites[2]);
  xpm_load(coin_3_xpm,XPM_5_6_5,&c->sprites[3]);
  xpm_load(coin_4_xpm,XPM_5_6_5,&c->sprites[4]);
  xpm_load(coin_5_xpm,XPM_5_6_5,&c->sprites[5]);
  xpm_load(coin_6_xpm,XPM_5_6_5,&c->sprites[6]);
  xpm_load(coin_7_xpm,XPM_5_6_5,&c->sprites[7]);
  c->xpos=800;
  c->ypos=(rand() % (325 - 190 + 1)) + 190 ;
  c->catched=false;
  return c;
}


int createCoin(Game * g){
  g->coins_size++;
  g->coins[g->coins_size-1]=newCoin();
  return 0;
}


void updateCoin(Coin* c){
  c->xpos-=2;
  c->currentsprite=(c->currentsprite+1)%8;
}


void drawCoin(Coin* c){
  draw_sprite(c->xpos,c->ypos,&c->sprites[c->currentsprite]);
}


void deleteCoin(Coin* c){
  free(c);
}


