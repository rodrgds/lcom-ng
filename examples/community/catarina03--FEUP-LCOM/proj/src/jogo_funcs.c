#include <lcom/lcf.h>

#include "jogo.h"
#include "jogo_funcs.h"

int scancode_handler(Game *g,Tux*t){
  if(scancode_bytes[0]==ALT_BRK)t->active_fireball=false;
    switch (t->currentmove)
  {
  case STANDING_R:
    if (scancode_bytes[1]==RIGHT_ARROW){t->currentmove=WALKING_R;t->moving_right=true;}
    if (scancode_bytes[1]==LEFT_ARROW){t->currentmove=WALKING_L;t->moving_left=true;}
    if (scancode_bytes[1]==DOWN_ARROW)t->currentmove=DUCKING_R;
    if (scancode_bytes[0]==SPACEBAR)t->currentmove=JUMPING_R;
    if( scancode_bytes[0]==ALT&&!t->active_fireball){t->active_fireball=true ;createFireball(t,1);updateFireballs(g,t);} 
    break;
  case STANDING_L:
    if( scancode_bytes[0]==ALT&&!t->active_fireball){t->active_fireball=true ;createFireball(t,0);updateFireballs(g,t);} 
    if (scancode_bytes[1]==RIGHT_ARROW){t->currentmove=WALKING_R;t->moving_right=true;}
    if (scancode_bytes[1]==LEFT_ARROW){t->currentmove=WALKING_L;t->moving_left=true;}
    if (scancode_bytes[1]==DOWN_ARROW)t->currentmove=DUCKING_L;
    if (scancode_bytes[0]==SPACEBAR)t->currentmove=JUMPING_L;
    break;  
  case WALKING_R:
    if( scancode_bytes[0]==ALT&&!t->active_fireball){t->active_fireball=true ;createFireball(t,1);updateFireballs(g,t);} 
    if (scancode_bytes[1]==RIGHT_ARROW_BRK){t->currentmove=STANDING_R;t->moving_right=false;}
    if (scancode_bytes[1]==LEFT_ARROW){t->currentmove=STANDING_L;t->moving_right=false;}
    if (scancode_bytes[1]==DOWN_ARROW){t->currentmove=DUCKING_R;t->moving_right=false;}
    if (scancode_bytes[0]==SPACEBAR){t->currentmove=JUMPING_R;t->moving_right=true;}
    break;
  case WALKING_L:
    if( scancode_bytes[0]==ALT&&!t->active_fireball){t->active_fireball=true ;createFireball(t,0);updateFireballs(g,t);} 
    if (scancode_bytes[1]==RIGHT_ARROW){t->currentmove=STANDING_R;t->moving_left=false;}
    if (scancode_bytes[1]==LEFT_ARROW_BRK){t->currentmove=STANDING_L;t->moving_left=false;}
    if (scancode_bytes[0]==SPACEBAR){t->currentmove=JUMPING_L;t->moving_left=true;}
    if (scancode_bytes[1]==DOWN_ARROW){t->currentmove=DUCKING_L;t->moving_left=false;}
    break;
  case JUMPING_R:
    if( scancode_bytes[0]==ALT&&!t->active_fireball){t->active_fireball=true ;createFireball(t,1);updateFireballs(g,t);} 
    if (scancode_bytes[0]==SPACEBAR_BRK)t->currentmove=AFTERJUMPING_R;
    if (scancode_bytes[1]==RIGHT_ARROW && !t->moving_left){t->moving_right=true;}
    if (scancode_bytes[1]==LEFT_ARROW_BRK){t->moving_left=false;}
    if (scancode_bytes[1]==RIGHT_ARROW_BRK){t->moving_right=false;}
    if (scancode_bytes[1]==LEFT_ARROW && !t->moving_right){t->moving_left=false;}
    break;
  case JUMPING_L:
    if( scancode_bytes[0]==ALT&&!t->active_fireball){t->active_fireball=true ;createFireball(t,0);updateFireballs(g,t);} 
    if (scancode_bytes[0]==SPACEBAR_BRK)t->currentmove=AFTERJUMPING_L;
    if (scancode_bytes[1]==RIGHT_ARROW && !t->moving_left){t->moving_right=false;}
    if (scancode_bytes[1]==LEFT_ARROW_BRK){t->moving_left=false;}
    if (scancode_bytes[1]==RIGHT_ARROW_BRK){t->moving_right=false;}
    if (scancode_bytes[1]==LEFT_ARROW && !t->moving_right){t->moving_left=true;}
    break;
  case DUCKING_R:
    if( scancode_bytes[0]==ALT&&!t->active_fireball){t->active_fireball=true ;createFireball(t,1);updateFireballs(g,t);} 
    if (scancode_bytes[1]==DOWN_ARROW_BRK)t->currentmove=STANDING_R;
    break;
  case DUCKING_L:
    if( scancode_bytes[0]==ALT&&!t->active_fireball){t->active_fireball=true ;createFireball(t,0);updateFireballs(g,t);} 
    if (scancode_bytes[1]==DOWN_ARROW_BRK)t->currentmove=STANDING_L;
    break;
  case AFTERJUMPING_R:
    if( scancode_bytes[0]==ALT&&!t->active_fireball){t->active_fireball=true ;createFireball(t,1);updateFireballs(g,t);} 
    if (scancode_bytes[1]==RIGHT_ARROW && !t->moving_left){t->moving_right=true;}
    if (scancode_bytes[1]==LEFT_ARROW_BRK){t->moving_left=false;}
    if (scancode_bytes[1]==RIGHT_ARROW_BRK){t->moving_right=false;}
  case AFTERJUMPING_L:
    if( scancode_bytes[0]==ALT&&!t->active_fireball){t->active_fireball=true ;createFireball(t,0);updateFireballs(g,t);} 
    if (scancode_bytes[1]==LEFT_ARROW_BRK){t->moving_left=false;}
    if (scancode_bytes[1]==RIGHT_ARROW_BRK){t->moving_right=false;}
    if (scancode_bytes[1]==LEFT_ARROW && !t->moving_right){t->moving_left=true;}
    break;
  default:
    break;
  }
  return 0;
}


int handle_tux_movs(Tux *tux){
    switch (tux->currentmove)
    {
    case WALKING_R:
        if(tux->xpos>=800-tux->sprites[tux->currentsprite].width)
            tux->currentmove=STANDING_R;
        else{
          tux->xpos+=tux->speed;
          tux->currentsprite=(tux->currentsprite+1)%4;
        }
        break;
    case WALKING_L:
        if(tux->xpos <= 0)
            tux->currentmove = STANDING_L;
        else{
            tux->xpos-=tux->speed;
            tux->currentsprite=(tux->currentsprite + 1)%4 + 6;
        }
        break;
    case DUCKING_R:
        tux->currentsprite=5;
        break;
    case DUCKING_L:
        tux->currentsprite=11;
        break;
    case STANDING_R:
        tux->ypos=FLOOR-tux->sprites[tux->currentsprite].height;
        tux->currentsprite=0;
        break;
    case STANDING_L:
        tux->ypos=FLOOR-tux->sprites[tux->currentsprite].height;
        tux->currentsprite=6;
        break;
    case JUMPING_R:
        if(tux->ypos<=MAX_TUX_JUMP)tux->currentmove=AFTERJUMPING_R;
        else{
            tux->currentsprite=4;
            tux->ypos-=15;
            if(tux->moving_right && tux->xpos<800-tux->sprites[tux->currentsprite].width)
                tux->xpos+=tux->speed;
        }
        break;
    case JUMPING_L:
        if(tux->ypos<=MAX_TUX_JUMP)tux->currentmove=AFTERJUMPING_L;
        else{
            tux->currentsprite=10;
            tux->ypos-=15;
            if(tux->moving_left && tux->xpos > 0)
                tux->xpos-=tux->speed;
        }
        break;
    case AFTERJUMPING_R:
        if(tux->ypos>=FLOOR-tux->sprites[tux->currentsprite].height){
            if(tux->moving_right)tux->currentmove=WALKING_R;
            else tux->currentmove=STANDING_R;
        }
        else
        {
            tux->currentsprite=4;
            tux->ypos+=15;
            if(tux->moving_right && tux->xpos<800-tux->sprites[tux->currentsprite].width)
                tux->xpos+=tux->speed;
        }
        break;
    case AFTERJUMPING_L:
        if(tux->ypos>=FLOOR-tux->sprites[tux->currentsprite].height){
            if(tux->moving_left)tux->currentmove=WALKING_L;
            else tux->currentmove=STANDING_L;
        }
        else
        {
            tux->currentsprite=10;
            tux->ypos+=15;
            if (tux->moving_left &&tux->xpos>0)
                tux->xpos-=tux->speed;
        }
        break;
    default:
        break;
    }
    return 0;
}


int subscribes_interrupts(uint32_t *timer_irq_set, uint32_t *kbd_irq_set, uint32_t *mouse_irq_set){
  uint8_t kbd_bit_no, timer_bit_no, mouse_bit_no;
  if(mouse_subscribe_int(&mouse_bit_no)){
    return 1;
  }
  *mouse_irq_set = BIT(mouse_bit_no);
  if(sys_irqdisable(&hook_mouse)){
    return 1;
  }
  do{
    if( issue_cmd_mouse(ENB_DATA_REPORT, &ack_byte)){
      return 1;// enables data reporting      
    }
    if(ack_byte == ERROR){ 
      return 1; //acknowledgment byte was ERROR;
    }
  }while(ack_byte != ACK);
  if(sys_irqenable(&hook_mouse)){
    return 1;
  }
  if(kbd_subscribe_int(&kbd_bit_no)){
    return 1;
  }
  *kbd_irq_set = BIT(kbd_bit_no);
  if(timer_subscribe_int(&timer_bit_no)){
    return 1;
  }
  *timer_irq_set = BIT(timer_bit_no);
  return 0;
}


int unsubscribes_interrupts(){
  if(sys_irqdisable(&hook_mouse)){
    return 1;
  }
  do{
      if( issue_cmd_mouse(DIS_DATA_REPORT, &ack_byte) != 0) //disables data reporting
          return 1;
        if(ack_byte == ERROR) //acknowledgment byte was ERROR; terminates program
          return 1;
  }while(ack_byte != ACK);
  if(sys_irqenable(&hook_mouse))return 1;
  if(mouse_unsubscribe_int())return 1;
  if(kbd_unsubscribe_int())return 1;
  if(timer_unsubscribe_int())return 1;
  return 0;
}


int mouse_movement(Game *mygame, Cursor *cursor, struct packet *mouse_packet, bool sign_x, bool sign_y){
    if(mygame->currentState != GAME_RUNNING && mygame->currentState != GAME_PAUSED){
        if(sign_x){
            cursor->xpos += mouse_packet->delta_x;
            if (cursor->xpos < 1){
                cursor->xpos = 1;
            }
        }
        if(!sign_x){
            cursor->xpos += mouse_packet->delta_x;
            if (cursor->xpos > 790){
                cursor->xpos = 790;
                }
        }
        if(sign_y){
            cursor->ypos -= mouse_packet->delta_y;
            if (cursor->ypos > 590){
                cursor->ypos = 590;
            }
        }
        if(!sign_y){
            cursor->ypos -= mouse_packet->delta_y;
            if (cursor->ypos < 1){
                cursor->ypos = 1;
            }
        }
    }
    return 0;
}


int checkGameState(Game *g, Cursor *c, Tux *t, struct mouse_ev *mouse_event){
  Letras* letras = createLetras();
  Numeros nums = createNums();
  char frase[7] = "SCORE:";
  Date * d=get_date();
    switch (g->currentState)
    {
    case INITIAL_MENU:
        draw_background(&g->background[1]);
        if(scancode == ESC_BRK_CODE){
          g->currentState = EXIT;
        }
        if (c->xpos > 250 && c->xpos < 550){
            if (c->ypos > 250 && c->ypos < 320){
                draw_background(&g->background[2]);
                if (mouse_event->type == LB_RELEASED){
                    g->currentState = GAME_RUNNING;
                }
            }
            if (c->ypos > 350 && c->ypos < 420){
                draw_background(&g->background[3]);
                if (mouse_event->type == LB_RELEASED){
                        g->currentState = INSTRUCTIONS_MENU;
                }
            }
            if (c->ypos > 450 && c->ypos < 520){
                draw_background(&g->background[4]);
                if (mouse_event->type == LB_RELEASED){
                        g->currentState = HIGHSCORE_MENU;
                }
            }
        }
        break;
    case GAME_RUNNING:
        if (scancode == 0x99){ //LETTER P BREAKCODE
            g->currentState = GAME_PAUSED;
            scancode = 0x00;
        }
        break;
    case INSTRUCTIONS_MENU:
        draw_background(&g->background[5]);
        if (c->xpos > 295 && c->xpos < 505){
            if (c->ypos > 515 && c->ypos < 575){
                draw_background(&g->background[9]);
                if (mouse_event->type == LB_RELEASED){
                    g->currentState = INITIAL_MENU;
                }
            }
        }
        break;
    case HIGHSCORE_MENU:
        draw_background(&g->background[6]);
        draw_highscores(g, letras, nums);
        if (c->xpos > 295 && c->xpos < 505){
            if (c->ypos > 515 && c->ypos < 575){
                draw_background(&g->background[8]);
                draw_highscores(g, letras, nums);
                if (mouse_event->type == LB_RELEASED){
                    g->currentState = INITIAL_MENU;
                }
            }
        }
        break;
    case GAME_PAUSED:
        draw_background(&g->background[7]);
        c->xpos = 2;
        c->ypos = 2;
        if (scancode == 0x99){ //LETTER P
            g->currentState = GAME_RUNNING;
            scancode = 0x00;
        }
        if (scancode == ESC_BRK_CODE){
            write_highscores(g->highscores);
            t->currentmove=STANDING_R;
            t->speed=JUMP_XMOV;
            t->xpos = 0;
            t->ypos = 0;
            t->fireballs=0;
            t->dead=false;
            g->background[0].x=0;
            g->background[0].y=0;
            g->score=0;
            g->background[0].speed=GAME_SPEED;
            g->coins_size=0;
            g->enemies_size=0;
            g->stage=0;
            g->count_secs=0;            
            g->currentState = INITIAL_MENU;
            scancode = 0x00;
        }
        break;
    case GAME_OVER:
        draw_background(&g->background[10]);
        printLetras(frase, letras, 280, 400);
        printNum(g->score, &nums, 410, 400);

        printNum(d->day, &nums, 250, 450);
        printLetras("/", letras, 275, 450);
        printNum(d->month, &nums, 300, 450);
        printLetras("/", letras, 325, 450);
        printNum(d->year, &nums, 350, 450);
        printNum(d->hour, &nums, 450, 450);
        printLetras(":", letras, 488, 450);
        printNum(d->minute, &nums, 500, 450); 
        

        if (c->ypos > 520 && c->ypos < 570){
            if (c->xpos > 150 && c->xpos < 350){ //BACK OPTION
                    draw_background(&g->background[11]);
                    printLetras(frase, letras, 280, 400);
                    printNum(g->score, &nums, 410, 400);

                    printNum(d->day, &nums, 250, 450);
                    printLetras("/", letras, 275, 450);
                    printNum(d->month, &nums, 300, 450);
                    printLetras("/", letras, 325, 450);
                    printNum(d->year, &nums, 350, 450);
                    printNum(d->hour, &nums, 450, 450);
                    printLetras(":", letras, 488, 450);
                    printNum(d->minute, &nums, 500, 450); 

                    if (mouse_event->type == LB_RELEASED){
                        update_highscores(g);
                        t->currentmove=STANDING_R;
                        t->speed=JUMP_XMOV;
                        t->xpos = 0;
                        t->ypos = 0;
                        t->fireballs=0;
                        t->dead=false;
                        g->background[0].x=0;
                        g->background[0].y=0;
                        g->score=0;
                        g->background[0].speed=GAME_SPEED;
                        g->coins_size=0;
                        g->enemies_size=0;
                        g->stage=0;
                        g->count_secs=0;
                        g->currentState = INITIAL_MENU;
                    }
                }
            
            if (c->xpos > 450 && c->xpos < 650){ //EXIT OPTION
                    draw_background(&g->background[12]);
                    printLetras(frase, letras, 280, 400);
                    printNum(g->score, &nums, 410, 400);

                    printNum(d->day, &nums, 250, 450);
                    printLetras("/", letras, 275, 450);
                    printNum(d->month, &nums, 300, 450);
                    printLetras("/", letras, 325, 450);
                    printNum(d->year, &nums, 350, 450);
                    printNum(d->hour, &nums, 450, 450);
                    printLetras(":", letras, 488, 450);
                    printNum(d->minute, &nums, 500, 450); 
                    if (mouse_event->type == LB_RELEASED){
                      update_highscores(g);
                      write_highscores(g->highscores);
                      g->currentState = EXIT;
                    }
                }
            
        }
        break;
    case EXIT:
        write_highscores(g->highscores);
        break;
    default:
        break;
    }
    return 0;
}


void handle_tux_collisions(Tux *t, Game * g){
  int ty0=t->ypos+TUX_COLLISION_TOLERANCE_Y;
  if(t->currentsprite==5||t->currentsprite==11)ty0+=20;
  Area_Collision *at=newAreaCollision(t->xpos+10,t->xpos+t->sprites[t->currentsprite].width-10,ty0,t->ypos+t->sprites[t->currentsprite].height);
  for(int i=0;i<g->enemies_size;i++){
    if(!g->enemies[i]->dead){
    Area_Collision* ac=newAreaCollision(g->enemies[i]->xpos,g->enemies[i]->xpos+g->enemies[i]->sprites[g->enemies[i]->currentsprite].width,g->enemies[i]->ypos,g->enemies[i]->ypos+g->enemies[i]->sprites[g->enemies[i]->currentsprite].height);
    if(check_collision (ac,at)){
      t->dead=true;
      return;
    }  
    }
  }
}


void drawFireballs(Tux *t){
  for(int i=0;i<t->fireballs;i++){  
    draw_sprite(t->fireball[i]->xpos,t->fireball[i]->ypos,&t->fireball[i]->sprites[t->fireball[i]->currentsprite]);
  }
}


FireBall * newFireball(int x,int y,int front){
  FireBall* f=(FireBall*)malloc(sizeof(FireBall));
  xpm_load(fire_bullet_0_xpm,XPM_5_6_5,&f->sprites[0]);
  xpm_load(fire_bullet_1_xpm,XPM_5_6_5,&f->sprites[1]);
  xpm_load(fire_bullet_2_xpm,XPM_5_6_5,&f->sprites[2]);
  f->currentsprite=0;
  f->front=front;
  f->xpos=x;
  f->ypos=y;
  return f;
}


void createFireball(Tux *t,int front){
  t->fireballs++;
  if(front==1){
    t->fireball[t->fireballs-1]=newFireball(t->xpos+t->sprites[t->currentsprite].width-20,t->ypos+t->sprites[t->currentsprite].height/2,1);
  }
  else{
    t->fireball[t->fireballs-1]=newFireball(t->xpos-t->sprites[t->currentsprite].width+60,t->ypos+t->sprites[t->currentsprite].height/2,0);
  }
}


void updateFireball(FireBall* f){
  f->currentsprite=(f->currentsprite+1)%3;
  if(f->front)f->xpos+=FIREBALL_SPEED;
  if(!f->front)f->xpos-=FIREBALL_SPEED;
}


void deleteFireball(FireBall* f){
  free(f);
}


void updateFireballs(Game * g,Tux* t){
  for(int i=0;i<t->fireballs;i++){
    updateFireball(t->fireball[i]);
    if(t->fireball[i]->xpos>800||t->fireball[i]->xpos<-5){
      deleteFireball(t->fireball[i]);
      t->fireball[i]=t->fireball[t->fireballs-1];
      i--;
      t->fireballs--;
    }
  } 
}


int game_run(Tux *mytux, Game * mygame,Letras * letras,Numeros *numeros){
   int create_spike;
   int type=0;
    if(mytux->dead){
              mygame->currentState = GAME_OVER;
              draw_background(&mygame->background[0]);
              drawCoins(mygame);
              drawEnemies(mygame);
              printLetras("time",letras,620,5);
              printLetras("score",letras,30,5);
              printNum(mygame->score,numeros,145,5);
              countdown(mygame->count_secs,numeros);
              printLetras("GAME OVER", letras,400,300);
              refresh_graphics();
          }else{
            
          if(counter%60==0)mygame->count_secs+=1;
          if(counter==600)mygame->stage=1;
          else if(counter==2400)mygame->stage=2;
          else if(counter==5000){mygame->stage=3;mytux->speed=12;}
          else if(counter==7000)mygame->stage=4;
          else if (counter==9500)mygame->stage=5;

          if(counter%5==0){
            handle_tux_movs(mytux);
            handle_tux_collisions(mytux,mygame);
            updateFireballs(mygame,mytux);
            updateEnemies(mygame,mytux);
            updateCoins(mygame,mytux);
          }

          switch (mygame->stage)
          {
          case 0:
            if(counter%200==0){
            createCoin(mygame);
            updateCoins(mygame,mytux);
            }
            break;
          case 1:
            if(counter%230==0){
            createCoin(mygame);
            updateCoins(mygame,mytux);
            }
            if(type<=mygame->stage && counter%250==0){
            createEnemy(mygame,rand()%2);
            updateEnemies(mygame,mytux);
            }
            break;
          case 2:
            if(counter%240==0){
            createCoin(mygame);
            updateCoins(mygame,mytux);
            }
            if(type<=mygame->stage && counter%150==0){
            createEnemy(mygame,rand()%3);
            updateEnemies(mygame,mytux);
            }
            break;
          case 3:
            create_spike=rand()%2;
            if(counter%240==0){
            createCoin(mygame);
            updateCoins(mygame,mytux);
            }
            if(counter%400==0&&create_spike){
              createEnemy(mygame,2);
              updateEnemies(mygame,mytux);
            }
            if(counter%200==0){
            createEnemy(mygame,rand()%3);
            updateEnemies(mygame,mytux);
            }
            break;
          case 4:
            create_spike=rand()%2;
            if(counter%130==0){
            createCoin(mygame);
            updateCoins(mygame,mytux);
            }
            if(counter%400==0&&create_spike){
              createEnemy(mygame,2);
              updateEnemies(mygame,mytux);
            }
            
            if(counter%100==0){
            createEnemy(mygame,rand()%5 );
            updateEnemies(mygame,mytux);
            }
            if((rand()%1723)==0){
            createEnemy(mygame,3);
            updateEnemies(mygame,mytux);
            }
            if((rand()%2334)==0){
              createEnemy(mygame,rand()%5 );
              updateEnemies(mygame,mytux);
            }
          
            break;
          case 5:
            
            if(counter%130==0){
            createCoin(mygame);
            updateCoins(mygame,mytux);
            }
         
            if((rand()%1720)==0){
            createEnemy(mygame,3);
            updateEnemies(mygame,mytux);
            }
            if((rand()%50)==0){
              createEnemy(mygame,rand()%5 );
              updateEnemies(mygame,mytux);
            }
            break;
          default:
            break;
          } 
          
          if(counter%10000==0) mygame->background[0].speed+=1;

          timer_int_handler();
          mygame->background[0].x+=mygame->background[0].speed;
          draw_background(&mygame->background[0]);
          draw_sprite(mytux->xpos,mytux->ypos,&mytux->sprites[mytux->currentsprite]);
          drawCoins(mygame);
          drawFireballs(mytux);
          drawEnemies(mygame);
          printLetras("time",letras,620,5);
          printLetras("score",letras,30,5);
          printNum(mygame->score,numeros,145,5);
          countdown(mygame->count_secs,numeros);
          refresh_graphics();
          
        }
        return 0;
}

void draw_highscores(Game *g ,Letras*l,Numeros n){
  int y=190;
  for(int i=0;i<5;i++){
    printNum(i+1,&n,100,y); 
    printLetras("Date:",l,120,y);
    printNum(g->highscores[i].d.day,&n,220,y);
    printLetras("/", l, 260, y + 3);
    printNum(g->highscores[i].d.month,&n,280,y);
    printLetras("/", l, 320, y + 3);
    printNum(g->highscores[i].d.year,&n,340,y);
    printNum(g->highscores[i].d.hour,&n,420,y);
    printLetras(":", l, 455, y);
    printNum(g->highscores[i].d.minute,&n,465,y); 
    printLetras("Score:",l,520,y);
    printNum(g->highscores[i].score,&n,635,y);
    y+=45;
  }
}

int update_highscores(Game*g){
  int i;
  Date * d=get_date();
  if(g->score>g->highscores[4].score){
    
    if(g->score>g->highscores[0].score)i=0;
    else if(g->score>g->highscores[1].score)i=1;
    else if(g->score>g->highscores[2].score)i=2;
    else if(g->score>g->highscores[3].score)i=3;
    else i=4;

  }else return 0;
  switch (i)
  {
  case 0:
    g->highscores[4]=g->highscores[3];
    g->highscores[3]=g->highscores[2];
    g->highscores[2]=g->highscores[1];
    g->highscores[1]=g->highscores[0];
    g->highscores[0].score=g->score;
    g->highscores[0].d=*d;
    
    break;
  case 1:
    g->highscores[4]=g->highscores[3];
    g->highscores[3]=g->highscores[2];
    g->highscores[2]=g->highscores[1];
    g->highscores[1].score=g->score;
    g->highscores[1].d=*d;
    break;
  case 2:
    g->highscores[4]=g->highscores[3];
    g->highscores[3]=g->highscores[2];
    g->highscores[2].score=g->score;
    g->highscores[2].d=*d;
    break;
  case 3:
    g->highscores[4]=g->highscores[3];
    g->highscores[3].score=g->score;
    g->highscores[3].d=*d;
    break;
  case 4:
    g->highscores[4].score=g->score;
    g->highscores[4].d=*d;
    break;
  default:
    break;
  }
  return 1;
}


