#include <lcom/lcf.h>
#include "enemies.h"


void drawEnemies(Game *g){
    for(int i=0;i<g->enemies_size;i++){
        drawEnemy(g->enemies[i]);
    }
}


Enemy * newEnemy(int type){
    Enemy * e=(Enemy *)malloc(sizeof(Enemy));
    if (type==0){
        e->type=SPIKY;
        xpm_load(spiky_0_xpm,XPM_5_6_5,&e->sprites[0]);
        xpm_load(spiky_1_xpm,XPM_5_6_5,&e->sprites[1]);
        xpm_load(spiky_2_xpm,XPM_5_6_5,&e->sprites[2]);
        xpm_load(spiky_dead_0_xpm,XPM_5_6_5,&e->sprites[4]);
        xpm_load(spiky_dead_1_xpm,XPM_5_6_5,&e->sprites[5]);
        xpm_load(spiky_dead_2_xpm,XPM_5_6_5,&e->sprites[3]);
        e->currentsprite=0;
        e->frame_counter=0;
        e->xpos=800;
        e->dead=false;
        e->ypos=FLOOR-e->sprites[e->currentsprite].height;
    }
    if(type==1){
        e->type=ZEEKLING;
        xpm_load(zeekling_0_xpm,XPM_5_6_5,&e->sprites[0]);
        xpm_load(zeekling_1_xpm,XPM_5_6_5,&e->sprites[1]);
        xpm_load(zeekling_2_xpm,XPM_5_6_5,&e->sprites[2]);
        xpm_load(zeekling_dead_0_xpm,XPM_5_6_5,&e->sprites[3]);
        xpm_load(zeekling_dead_1_xpm,XPM_5_6_5,&e->sprites[4]);
        xpm_load(zeekling_dead_2_xpm,XPM_5_6_5,&e->sprites[5]);
        xpm_load(zeekling_dead_3_xpm,XPM_5_6_5,&e->sprites[6]);
        xpm_load(zeekling_dead_4_xpm,XPM_5_6_5,&e->sprites[7]);
        xpm_load(zeekling_dead_5_xpm,XPM_5_6_5,&e->sprites[8]);
        xpm_load(zeekling_diving_xpm,XPM_5_6_5,&e->sprites[9]);
        e->currentsprite=0;
        e->frame_counter=0;
        e->xpos=800;
        e->dead=false;
        e->ypos=100+(rand()%(200));
    }
    if(type==2){
        e->type=SPIKE;
        xpm_load(spike_falling_xpm,XPM_5_6_5,&e->sprites[0]);
        xpm_load(spike_broken_xpm,XPM_5_6_5,&e->sprites[1]);
        e->currentsprite=0;
        e->frame_counter=0;
        e->xpos=rand()%780;
        e->dead=false;
        e->atack=false;
        e->ypos=0;
    }
    if(type==3){
        e->type=KAMIKAZE;
        xpm_load(kamikaze_left_xpm,XPM_5_6_5,&e->sprites[0]);
        xpm_load(kamikaze_melting_2_xpm,XPM_5_6_5,&e->sprites[1]);
        xpm_load(kamikaze_melting_1_xpm,XPM_5_6_5,&e->sprites[2]);
        e->currentsprite=0;
        e->frame_counter=0;
        e->xpos=-10;
        e->dead=false;
        e->atack=false;
        e->ypos=100+(rand()%(200));
    }
    return e;
}


int createEnemy(Game * g,int type){
    if(type>=0&& type<4){
       g->enemies_size++;
       g->enemies[g->enemies_size-1]=newEnemy(type);
    }
    
    return 0;
}


int updateEnemy(Enemy* e,Tux *t,Game*g){
    switch (e->type)
    {
    case SPIKY:
        if(!e->dead){
           if(e->xpos<0){
                deleteEnemy(e);
                return 1;
            }
           e->xpos-=SPIKY_SPEED;
           e->currentsprite=(e->currentsprite+1)%3; 
           Area_Collision* ae=newAreaCollision(e->xpos,e->xpos+e->sprites[e->currentsprite].width,e->ypos,e->ypos+e->sprites[e->currentsprite].height);
           for(int i=0;i<t->fireballs;i++){
            Area_Collision* af=newAreaCollision(t->fireball[i]->xpos,t->fireball[i]->xpos+t->fireball[i]->sprites[t->fireball[i]->currentsprite].width,t->fireball[i]->ypos,t->fireball[i]->ypos+t->fireball[i]->sprites[t->fireball[i]->currentsprite].height);
            if(check_collision(ae,af)){
                deleteFireball(t->fireball[i]);
                t->fireball[i]=t->fireball[t->fireballs-1];
                i--;
                t->fireballs--;
                e->dead=true;
                e->currentsprite=3;
                g->score+=SPIKY_KILL_SCORE;
                deleteAreaCollision(ae);
                deleteAreaCollision(af);
                return 0;
            }
            deleteAreaCollision(ae);
            deleteAreaCollision(af);
           } 
        }
        else{
            if(e->frame_counter>=2 &&e->currentsprite==5){
                deleteEnemy(e);
                return 1;
            }
            if(e->frame_counter<2)e->frame_counter++;
            else{
                e->frame_counter=0;
                e->currentsprite=3+(e->currentsprite+1)%3;
            }
            return 0;
        }
        break;
    case ZEEKLING:
        if(!e->dead){
            if(e->xpos<0){
                deleteEnemy(e);
                return 1;
            }
            if(!e->atack){
                e->xpos-=10;
                e->currentsprite=(e->currentsprite+1)%3; 
                Area_Collision* ae=newAreaCollision(e->xpos,e->xpos+e->sprites[e->currentsprite].width,e->ypos,e->ypos+e->sprites[e->currentsprite].height);
                Area_Collision* at=newAreaCollision(t->xpos-40,t->xpos+t->sprites[t->currentsprite].width+70,100,t->ypos);
                if(check_collision(ae,at)&&e->ypos<FLOOR-e->sprites[e->currentsprite].height)e->atack=true;
                deleteAreaCollision(ae);
                deleteAreaCollision(at);
            }
            else
            {
                e->xpos-=7;
                e->ypos+=10;
                e->currentsprite=9;
                if(e->ypos>=FLOOR-e->sprites[e->currentsprite].height)e->atack=false;
            }
            
           Area_Collision* ae=newAreaCollision(e->xpos,e->xpos+e->sprites[e->currentsprite].width,e->ypos,e->ypos+e->sprites[e->currentsprite].height);
           for(int i=0;i<t->fireballs;i++){
            Area_Collision* af=newAreaCollision(t->fireball[i]->xpos,t->fireball[i]->xpos+t->fireball[i]->sprites[t->fireball[i]->currentsprite].width,t->fireball[i]->ypos,t->fireball[i]->ypos+t->fireball[i]->sprites[t->fireball[i]->currentsprite].height);
            if(check_collision(ae,af)){
                deleteFireball(t->fireball[i]);
                t->fireball[i]=t->fireball[t->fireballs-1];
                i--;
                t->fireballs--;
                e->dead=true;
                e->currentsprite=3;
                g->score+=ZEEKLING_KILL_SCORE;
                deleteAreaCollision(ae);
                deleteAreaCollision(af);
                return 0;
            }
            deleteAreaCollision(ae);
            deleteAreaCollision(af);
           } 
        }
        else{
           
            if(e->currentsprite==8){
                deleteEnemy(e);
                return 1;
            }
            else{
                e->currentsprite=(e->currentsprite+1);
            }
            return 0; 
        }
        
        break;
    case SPIKE:
        if(!e->dead){
            if(!e->atack){
            e->frame_counter++;if(e->frame_counter==6)e->atack=true;
            }
            else{
                e->ypos+=5;
               if(e->ypos>=FLOOR-e->sprites[e->currentsprite].height){
               e->dead=true;
               e->currentsprite=1;
               }
            }

        }
        else{
            if(e->frame_counter>=10){
                deleteEnemy(e);
                return 1;
            }
            if(e->frame_counter<10)e->frame_counter++;
            
            
        }
        break;
    case KAMIKAZE:
        if(!e->dead){
            if(e->xpos>800){
                deleteEnemy(e);
                return 1;
            }
            e->xpos+=20;
            
            
            Area_Collision* ae=newAreaCollision(e->xpos,e->xpos+e->sprites[e->currentsprite].width,e->ypos,e->ypos+e->sprites[e->currentsprite].height);
           for(int i=0;i<t->fireballs;i++){
            Area_Collision* af=newAreaCollision(t->fireball[i]->xpos,t->fireball[i]->xpos+t->fireball[i]->sprites[t->fireball[i]->currentsprite].width,t->fireball[i]->ypos,t->fireball[i]->ypos+t->fireball[i]->sprites[t->fireball[i]->currentsprite].height);
            if(check_collision(ae,af)){
                deleteFireball(t->fireball[i]);
                t->fireball[i]=t->fireball[t->fireballs-1];
                i--;
                t->fireballs--;
                e->dead=true;
                e->currentsprite=1;
                g->score+=ZEEKLING_KILL_SCORE;
                deleteAreaCollision(ae);
                deleteAreaCollision(af);
                return 0;
            }
            deleteAreaCollision(ae);
            deleteAreaCollision(af);
           } 
        }
        else{
           
            if(e->currentsprite==2){
                deleteEnemy(e);
                return 1;
            }
            else{
                e->currentsprite=(e->currentsprite+1);
            }
            return 0; 
        }
        break;
    default:
        break;
    
    }
    return 0;
}


void drawEnemy(Enemy* e){
    draw_sprite(e->xpos,e->ypos,&e->sprites[e->currentsprite]);
}


void deleteEnemy(Enemy* e){
    free(e);
}


void updateEnemies(Game * g,Tux* t){
    for(int i=0;i<g->enemies_size;i++){
       if(updateEnemy(g->enemies[i],t,g)){
           
           g->enemies[i]=g->enemies[g->enemies_size-1];
           g->enemies_size--;
           i--;
       }
   
  
  }
}

