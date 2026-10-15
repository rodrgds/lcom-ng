#include "game/game.h"

State gameState = MENU;
uint8_t bit_no_keyboard = 0;
uint8_t bit_no_timer = 0;
uint8_t bit_no_mouse = 0;
Background background;
Background menu;
Background lost;
Background won;
Paddle paddle;
Brick bricks[72];
Ball ball;
Ball extra_ball;
Projectile projectile;
Marker marker;
extern vbe_mode_info_t vmi_p;
extern uint8_t scancodes[2];
extern struct packet mouse_packet;
Sprite *background_sprite;
Sprite *menu_sprite;
Sprite *lost_sprite;
Sprite *won_sprite;
Sprite *ball_sprite;
Sprite *brick_sprite;
Sprite *paddle_sprite;
Sprite *extra_ball_sprite;
Sprite *projectile_sprite;
Sprite *explosion_sprite;
Sprite *life_sprite;
Sprite *projectileIcon_sprite;
Sprite *marker_sprite;
int ball_direction = 0;
int lives = 3;
int destroyed = 0;
int powerup = 0;
bool ball_power = false;
bool projectileLaunched = false;
bool ballLaunched = false;
bool moveBricks = false;
int frames = 0;
int fps = 15;  //fps
int tspan = 1; //time to refresh the whole screen
int tstart = 30; //time to start moving bricks


//----------------video--------------------------------------------------------------------------------------------------------------------


int (prepare_video)() {
    
    if(vbe_mapping_videoRAM_to_address_space(0x115)!=0) {return 1;}

    if(vbe_set_display_mode(0x115)!=0) {return 1;}

    printf("Initializing video...");

    return 0;
}

int (disable_video)() {
    if (vg_exit()) return 1;
    return 0;
}


//----------------keyboard--------------------------------------------------------------------------------------------------------------------


int (prepare_keyboard)() {
    if(keyboard_subscribe_int(&bit_no_keyboard)!=0) {return 1;}
    return 0;
}

int (disable_keyboard)() {
    if (keyboard_unsubscribe_int()) return 1;
    return 0;
}

void handle_keyboard() {
  switch(scancodes[0]) {
    case A_KEY:
    if (paddle.x > 0) {
      paddle.oldx = paddle.x;
      paddle.x -= BIT(3);
    }
      break;
    case D_KEY:
    if (paddle.x + paddle.sprite->width < vmi_p.XResolution) {
      paddle.oldx = paddle.x;
      paddle.x += BIT(3);
    }
      break;
    case ESC_KEY:
    if (gameState == MENU) {
      gameState=EXIT;
    }
    else if (gameState == GAME) {
      gameState = MENU;
    }
    else if (gameState == LOST || gameState == WON) {
      gameState = EXIT;
    }
      break;
    case SPACE_KEY:
    if (gameState == GAME) {
      if (!projectileLaunched) {
      if(projectileLaunch()) {break;}
      }
    }
      break;
    case ENTER_KEY:
    if (gameState == MENU) {
      prepare_objects();
      gameState = GAME;
    }
    else if (gameState == LOST || gameState == WON) {
      gameState = MENU;
    }
    else if (gameState == GAME) {
      ball.base = false;
    }
    break;
    default:
      break;
  }
}


//----------------timer--------------------------------------------------------------------------------------------------------------------


int (prepare_timer)() {
    if(timer_subscribe_int(&bit_no_timer)!=0) {return 1;}
    return 0;
}

int (disable_timer)() {
    if(timer_unsubscribe_int()) return 1;
    return 0;
}

//----------------mouse--------------------------------------------------------------------------------------------------------------------


int (prepare_mouse)() {
    if(mouse_subscribe_int(&bit_no_mouse)!=0) {return 1;}
    if(mouse_write_data(SET_STREAM_MODE)!=0) {return 1;}
    if(mouse_write_data(ENABLE_DATA_REPORTING)!=0) {return 1;}
    return 0;
}

int (disable_mouse)() {
    if(mouse_unsubscribe_int()) return 1;
    if(mouse_write_data(DISABLE_DATA_REPORTING)!=0) {return 1;}
    return 0;
}

void handle_mouse() {
  if (marker.x < vmi_p.XResolution - BIT(4) && marker.x > BIT(4)) {
    if (ball.base) {
      ball_direction = marker.x - (paddle.x + paddle.sprite->width/2);
      if (ball_direction > 0) {
        ball.dx = 1;
      }
      if (ball_direction > 4) {
        ball.dx = 2;
      }
      if (ball_direction > 8) {
        ball.dx = 3;
      }
      if (ball_direction > 16) {
        ball.dx = 4;
      }
      if (ball_direction > 32) {
        ball.dx = 5;
      }
      if (ball_direction > 64) {
        ball.dx = 6;
      }
      if (ball_direction > 128) {
        ball.dx = 7;
      }
      if (ball_direction > 256) {
        ball.dx = 8;
      }
      if (ball_direction < 0) {
        ball.dx = -1;
      }
      if (ball_direction < -4) {
        ball.dx = -2;
      }
      if (ball_direction < -8) {
        ball.dx = -3;
      }
      if (ball_direction < -16) {
        ball.dx = -4;
      }
      if (ball_direction < -32) {
        ball.dx = -5;
      }
      if (ball_direction < -64) {
        ball.dx = -6;
      }
      if (ball_direction < -128) {
        ball.dx = -7;
      }
      if (ball_direction < -256) {
        ball.dx = -8;
      }
    }
  }
}


//----------------objects--------------------------------------------------------------------------------------------------------------------


int (prepare_screens)() {
    menu_sprite = create_sprite((xpm_map_t)menu_xpm);
    lost_sprite = create_sprite((xpm_map_t)lost_xpm);
    won_sprite = create_sprite((xpm_map_t)won_xpm);
    initBackground(&menu,0,0,menu_sprite);
    initBackground(&lost,0,0,lost_sprite);
    initBackground(&won,0,0,won_sprite);
    return 0;
}

int (prepare_objects)() {

    background_sprite = create_sprite((xpm_map_t)background_xpm);
    ball_sprite = create_sprite((xpm_map_t)ball_xpm);
    paddle_sprite = create_sprite((xpm_map_t)paddle_xpm);
    brick_sprite = create_sprite((xpm_map_t)brick_xpm);
    extra_ball_sprite = create_sprite((xpm_map_t)ball2_xpm);
    projectile_sprite = create_sprite((xpm_map_t)missile_xpm);
    explosion_sprite = create_sprite((xpm_map_t)explosion_xpm);
    life_sprite = create_sprite((xpm_map_t)life_xpm);
    projectileIcon_sprite = create_sprite((xpm_map_t)missileIcon_xpm);
    marker_sprite = create_sprite((xpm_map_t)marker_xpm);


    initBackground(&background, 0, 0, background_sprite);
    initPaddle(&paddle, (background_sprite->width/2)-(paddle_sprite->width/2), 530, paddle_sprite);
    for (int i = 0; i < 72; i++) {
      initBrick(&bricks[i], BIT(4)+((i%12)*BIT(6)), BIT(4)+(int)(i/12)*BIT(5), brick_sprite);
    }
    initBall(&ball, (background_sprite->width/2)-(ball_sprite->width/2), 530-ball_sprite->height, ball_sprite);
    initMarker(&marker, (background_sprite->width/2)-(marker_sprite->width/2), marker_sprite);

    lives = 3;
    destroyed = 0;
    powerup = 0;
    frames = 0;
    ball_power = false;
    projectileLaunched = false;
    ballLaunched = false;
    moveBricks = false;

    return 0;
}

int projectileLaunch() {
  if (powerup > 0) {
    powerup--;
    initProjectile(&projectile, paddle, projectile_sprite, explosion_sprite); 
    projectileLaunched = true;  
    return 0;
  }
  return 1;
}


//----------------run--------------------------------------------------------------------------------------------------------------------


int (draw_init)() {
  if (gameState == GAME) {
    if(drawBackground(&background)) {return 1;}
    for (int i=0; i<72; i++) {
      if (!bricks[i].destroyed) {
        if(drawBrick(&bricks[i])){return 1;}
      }
    }
    for (int i=0; i<lives; i++) {
      if(drawSprite(life_sprite, vmi_p.XResolution/2 + vmi_p.XResolution/4 - life_sprite->width/2 + 15*i, vmi_p.YResolution - 13 )!=0) {return 1;}
    }

    for (int i=0; i<powerup; i++) {
      if(drawSprite(projectileIcon_sprite, vmi_p.XResolution/2 - vmi_p.XResolution/4 - projectileIcon_sprite->width/2 + 15*i, vmi_p.YResolution - 13 )!=0) {return 1;}
    }
    
  }
  else if (gameState == MENU) {
    if(drawBackground(&menu)) {return 1;}
  }
  else if (gameState == LOST) {
    if(drawBackground(&lost)) {return 1;}
  }
  else if (gameState == WON) {
    if(drawBackground(&won)) {return 1;}
  }
    return 0;
}

int draw_frame() {
  if (gameState == GAME) {
    if(erasePaddle(&paddle)) {return 1;}
    if(drawPaddle(&paddle)) {return 1;}
    
    if(eraseBall(&ball)) {return 1;}
    if(drawBall(&ball)) {return 1;}
    
  }
  return 0;
}

int move_ball() {
    ball.oldx = ball.x;
    ball.oldy = ball.y;

    if (ball.base) {
      ball.x = paddle.x + (paddle.sprite->width/2) - (ball.sprite->width/2);
      ball.y = paddle.y - ball.sprite->height;
      ball.dy = -BIT(2);
    } else {
        if (ball.x + ball.sprite->width >= vmi_p.XResolution - 10) {
          ball.x = vmi_p.XResolution - ball.sprite->width - BIT(3);
          ball.dx = -ball.dx;
        }
        if (ball.x <= 10) {
          ball.x = BIT(3);
          ball.dx = -ball.dx;
        }
        if (ball.y <= 10) {
          ball.y = BIT(3);
          ball.dy = -ball.dy;
        }
        
        if (ball.y + ball.sprite->height >= vmi_p.YResolution - 20) {
          ball.y = vmi_p.YResolution - ball.sprite->height - BIT(4);
          ball.base = true;
          lives--;
        }
        if (ball.y + ball.sprite->height >= paddle.y+1 && ball.x + ball.sprite->width >= paddle.x && ball.x <= paddle.x + paddle.sprite->width) {
          if (ball.x + ball.sprite->width/2 < paddle.x + 1 * paddle.sprite->width/16) {
            ball.dx = -8;
            ball.dy = -1;
          } else if (ball.x + ball.sprite->width/2 < paddle.x + 2 * paddle.sprite->width/16) {
            ball.dx = -7;
            ball.dy = -2;
          } else if (ball.x + ball.sprite->width/2 < paddle.x + 3 * paddle.sprite->width/16) {
            ball.dx = -6;
            ball.dy = -3;
          } else if (ball.x + ball.sprite->width/2 < paddle.x + 4 * paddle.sprite->width/16) {
            ball.dx = -5;
            ball.dy = -4;
          } else if (ball.x + ball.sprite->width/2 < paddle.x + 5 * paddle.sprite->width/16) {
            ball.dx = -4;
            ball.dy = -5;
          } else if (ball.x + ball.sprite->width/2 < paddle.x + 6 * paddle.sprite->width/16) {
            ball.dx = -3;
            ball.dy = -6;
          } else if (ball.x + ball.sprite->width/2 < paddle.x + 7 * paddle.sprite->width/16) {
            ball.dx = -2;
            ball.dy = -7;
          } else if (ball.x + ball.sprite->width/2 < paddle.x + 8 * paddle.sprite->width/16) {
            ball.dx = -1;
            ball.dy = -8;
          } else if (ball.x + ball.sprite->width/2 < paddle.x + 9 * paddle.sprite->width/16) {
            ball.dx = 1;
            ball.dy = -8;
          } else if (ball.x + ball.sprite->width/2 < paddle.x + 10 * paddle.sprite->width/16) {
            ball.dx = 2;
            ball.dy = -7;
          } else if (ball.x + ball.sprite->width/2 < paddle.x + 11 * paddle.sprite->width/16) {
            ball.dx = 3;
            ball.dy = -6;
          } else if (ball.x + ball.sprite->width/2 < paddle.x + 12 * paddle.sprite->width/16) {
            ball.dx = 4;
            ball.dy = -5;
          } else if (ball.x + ball.sprite->width/2 < paddle.x + 13 * paddle.sprite->width/16) {
            ball.dx = 5;
            ball.dy = -4;
          } else if (ball.x + ball.sprite->width/2 < paddle.x + 14 * paddle.sprite->width/16) {
            ball.dx = 6;
            ball.dy = -3;
          } else if (ball.x + ball.sprite->width/2 < paddle.x + 15 * paddle.sprite->width/16) {
            ball.dx = 7;
            ball.dy = -2;
          } else if (ball.x + ball.sprite->width/2 < paddle.x + 16 * paddle.sprite->width/16) {
            ball.dx = 8;
            ball.dy = -1;
          }
          
        }


        for (int i = 0; i < 72; i++) {
          if (!bricks[i].destroyed) {
            if (ball.y <= bricks[i].y + brick_sprite->height && ball.y + ball.sprite->height >= bricks[i].y && ball.x + ball.sprite->width >= bricks[i].x && ball.x <= bricks[i].x + brick_sprite->width) {
              ball.dy = -ball.dy;
              bricks[i].destroyed = true;
              if (eraseBrick(&bricks[i])) {return 1;}
              
              srand(time(0));
              int random_number = rand() % 10;
              if (random_number == 0) {
                powerup++;
              }
              if (random_number == 1 && ballLaunched == false) {
                ball_power = true;
              }
            }
          }
        }
        ball.x += ball.dx;
        ball.y += ball.dy;
        

    }
    return 0;
}

int move_extraball() {
    extra_ball.oldx = extra_ball.x;
    extra_ball.oldy = extra_ball.y;

    if (extra_ball.base) {
      extra_ball.x = paddle.x + (paddle.sprite->width/2) - (extra_ball.sprite->width/2);
      extra_ball.y = paddle.y - extra_ball.sprite->height;
      extra_ball.dy = -BIT(2);
      extra_ball.base = false;
    } else {
        if (extra_ball.x + extra_ball.sprite->width >= vmi_p.XResolution - BIT(3)) {
          extra_ball.x = vmi_p.XResolution - extra_ball.sprite->width - BIT(3);
          extra_ball.dx = -extra_ball.dx;
        }
        if (extra_ball.x <= BIT(3)) {
          extra_ball.x = BIT(3);
          extra_ball.dx = -extra_ball.dx;
        }
        if (extra_ball.y <= BIT(3)) {
          extra_ball.y = BIT(3);
          extra_ball.dy = -extra_ball.dy;
        }
        
        if (extra_ball.y + extra_ball.sprite->height >= vmi_p.YResolution - BIT(4)) {
          extra_ball.y = vmi_p.YResolution - extra_ball.sprite->height - BIT(4);
          ballLaunched = false;
        }
        if (extra_ball.y + extra_ball.sprite->height >= paddle.y+1 && extra_ball.x + extra_ball.sprite->width >= paddle.x && extra_ball.x <= paddle.x + paddle.sprite->width) {
          if (extra_ball.x + extra_ball.sprite->width/2 < paddle.x + 1 * paddle.sprite->width/16) {
            extra_ball.dx = -8;
            extra_ball.dy = -1;
          } else if (extra_ball.x + extra_ball.sprite->width/2 < paddle.x + 2 * paddle.sprite->width/16) {
            extra_ball.dx = -7;
            extra_ball.dy = -2;
          } else if (extra_ball.x + extra_ball.sprite->width/2 < paddle.x + 3 * paddle.sprite->width/16) {
            extra_ball.dx = -6;
            extra_ball.dy = -3;
          } else if (extra_ball.x + extra_ball.sprite->width/2 < paddle.x + 4 * paddle.sprite->width/16) {
            extra_ball.dx = -5;
            extra_ball.dy = -4;
          } else if (extra_ball.x + extra_ball.sprite->width/2 < paddle.x + 5 * paddle.sprite->width/16) {
            extra_ball.dx = -4;
            extra_ball.dy = -5;
          } else if (extra_ball.x + extra_ball.sprite->width/2 < paddle.x + 6 * paddle.sprite->width/16) {
            extra_ball.dx = -3;
            extra_ball.dy = -6;
          } else if (extra_ball.x + extra_ball.sprite->width/2 < paddle.x + 7 * paddle.sprite->width/16) {
            extra_ball.dx = -2;
            extra_ball.dy = -7;
          } else if (extra_ball.x + extra_ball.sprite->width/2 < paddle.x + 8 * paddle.sprite->width/16) {
            extra_ball.dx = -1;
            extra_ball.dy = -8;
          } else if (extra_ball.x + extra_ball.sprite->width/2 < paddle.x + 9 * paddle.sprite->width/16) {
            extra_ball.dx = 1;
            extra_ball.dy = -8;
          } else if (extra_ball.x + extra_ball.sprite->width/2 < paddle.x + 10 * paddle.sprite->width/16) {
            extra_ball.dx = 2;
            extra_ball.dy = -7;
          } else if (extra_ball.x + extra_ball.sprite->width/2 < paddle.x + 11 * paddle.sprite->width/16) {
            extra_ball.dx = 3;
            extra_ball.dy = -6;
          } else if (extra_ball.x + extra_ball.sprite->width/2 < paddle.x + 12 * paddle.sprite->width/16) {
            extra_ball.dx = 4;
            extra_ball.dy = -5;
          } else if (extra_ball.x + extra_ball.sprite->width/2 < paddle.x + 13 * paddle.sprite->width/16) {
            extra_ball.dx = 5;
            extra_ball.dy = -4;
          } else if (extra_ball.x + extra_ball.sprite->width/2 < paddle.x + 14 * paddle.sprite->width/16) {
            extra_ball.dx = 6;
            extra_ball.dy = -3;
          } else if (extra_ball.x + extra_ball.sprite->width/2 < paddle.x + 15 * paddle.sprite->width/16) {
            extra_ball.dx = 7;
            extra_ball.dy = -2;
          } else if (extra_ball.x + extra_ball.sprite->width/2 < paddle.x + 16 * paddle.sprite->width/16) {
            extra_ball.dx = 8;
            extra_ball.dy = -1;
          }
          
        }


        for (int i = 0; i < 72; i++) {
          if (!bricks[i].destroyed) {
            if (extra_ball.y <= bricks[i].y + brick_sprite->height && extra_ball.y + extra_ball.sprite->height >= bricks[i].y && extra_ball.x + extra_ball.sprite->width >= bricks[i].x && extra_ball.x <= bricks[i].x + brick_sprite->width) {
              extra_ball.dy = -extra_ball.dy;
              bricks[i].destroyed = true;
              if (eraseBrick(&bricks[i])) {return 1;}
              
              srand(time(0));
              int random_number = rand() % 10;
              if (random_number == 0) {
                powerup++;
              }
              if (random_number == 1 && ballLaunched == false) {
                ball_power = true;
              }
            }
          }
        }
        extra_ball.x += extra_ball.dx;
        extra_ball.y += extra_ball.dy;
        

    }
    return 0;
}

int move_projectile() {
  for (int i = 0; i < 72; i++) {
    if (!bricks[i].destroyed) {
      if (projectile.y <= bricks[i].y + brick_sprite->height && projectile.y + projectile.sprite->height >= bricks[i].y && projectile.x + projectile.sprite->width >= bricks[i].x && projectile.x <= bricks[i].x + brick_sprite->width) {
        eraseProjectile(&projectile);
        if (i % 12 > 0 && i % 12 < 11) {  // 0 1 2 3 4 5 6 7 8 9 10 11
          bricks[i].destroyed = true;
          bricks[i+1].destroyed = true;
          bricks[i-1].destroyed = true;
          if (eraseBrick(&bricks[i])) {return 1;}
          if (eraseBrick(&bricks[i+1])) {return 1;}
          if (eraseBrick(&bricks[i-1])) {return 1;}
          if (i-13 >= 0){
          bricks[i-12].destroyed = true;
          bricks[i-12-1].destroyed = true;
          bricks[i-12+1].destroyed = true;
          if (eraseBrick(&bricks[i-12])) {return 1;}
          if (eraseBrick(&bricks[i-12-1])) {return 1;}
          if (eraseBrick(&bricks[i-12+1])) {return 1;}
          }
        }
        if (i % 12 == 11) {
          bricks[i].destroyed = true;
          bricks[i-1].destroyed = true;
          if (eraseBrick(&bricks[i])) {return 1;}
          if (eraseBrick(&bricks[i-1])) {return 1;}
          if (i-13 >= 0){
          bricks[i-12].destroyed = true;
          bricks[i-12-1].destroyed = true;
          if (eraseBrick(&bricks[i-12])) {return 1;}
          if (eraseBrick(&bricks[i-12-1])) {return 1;}
          }
        }
        if (i % 12 == 0) {
          bricks[i].destroyed = true;
          bricks[i+1].destroyed = true;
          if (eraseBrick(&bricks[i])) {return 1;}
          if (eraseBrick(&bricks[i+1])) {return 1;}
          if (i-13 >= 0){
          bricks[i-12].destroyed = true;
          bricks[i-12+1].destroyed = true;
          if (eraseBrick(&bricks[i-12])) {return 1;}
          if (eraseBrick(&bricks[i-12+1])) {return 1;}
          }
        }
      

        drawExplosion(&projectile);
        projectileLaunched = false;
        return 0;
      }
    }
  }
  if (projectile.y <= BIT(3)) {
    projectileLaunched = false;
  }
  projectile.oldy = projectile.y;
  projectile.y -= 5;
  return 0;
}

int move_bricks() {
  for (int i = 0; i < 72; i++) {
    if (!bricks[i].destroyed) {
      if (bricks[i].y + bricks[i].sprite->height <= paddle.y - BIT(4)) {
        bricks[i].y+= BIT(2);
      }
      else {
        gameState = LOST;
      }
    }
  }
  return 0;
}

int move_marker() {
  if (mouse_packet.delta_x > 0) {
    if (marker.x + marker.sprite->width < vmi_p.XResolution - BIT(4)) {
      marker.oldx = marker.x;
      marker.x += mouse_packet.delta_x;
    }
  }
  if (mouse_packet.delta_x < BIT(4)) {
    if (marker.x > 0) {
      marker.oldx = marker.x;
      marker.x += mouse_packet.delta_x;
    }
  }
  return 0;
}

int (run)() {
    int ipc_status,r=0;
    message msg;

    while (gameState!=EXIT) {
    
    if ((r=driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("Error");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & BIT(bit_no_timer)) {
            if (gameState == GAME) {
              frames++;
              if (frames % 60*tspan == 1)
                draw_init();
              if (frames % (60/fps) == 0) { 
                move_ball();
                draw_frame();
                if (moveBricks) {
                  if (frames % 60 == 0) {
                    move_bricks();
                  }
                }
                if (projectileLaunched) {
                  move_projectile();
                  if(eraseProjectile(&projectile)) {return 1;}
                  if(drawProjectile(&projectile)) {return 1;}
                }
                if (ballLaunched) {
                  move_extraball();
                  if(eraseBall(&extra_ball)) {return 1;}
                  if(drawBall(&extra_ball)) {return 1;}
                }
                if (ball.base) {
                  eraseMarker(&marker);
                  drawMarker(&marker);
                }
              }
              if (frames == 60*tstart) {
                moveBricks = true;
              }
            } else {
              draw_init();
            }
          }
          if (msg.m_notify.interrupts & BIT(bit_no_keyboard)){
            (kbc_ih)();
            handle_keyboard();
            
          } 
          if (msg.m_notify.interrupts & BIT(bit_no_mouse)){
            (mouse_ih)();
            move_marker();
            handle_mouse();
          }
        }
      }
      if (lives < 0) {
        lives = 3;
        destroyed = 0;
        gameState = LOST;     
      }
      destroyed = 0;
      for (int i = 0; i < 72; i++) {
        if(bricks[i].destroyed) {
          destroyed++;
        }
      }
      if (destroyed == 72) {
        lives = 3;
        destroyed = 0;
        gameState = WON;
      }
      if (ball_power) {
        initBall(&extra_ball, (background_sprite->width/2)-(ball_sprite->width/2), 530-ball_sprite->height, extra_ball_sprite);
        ballLaunched = true;
        ball_power = false;
      }
    }
  return 0;
}
