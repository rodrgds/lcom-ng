#include "model.h"
int global_counter = 0;
int level_time = 0;
int changesprite = 0;
int spriteindex = 0;
uint8_t kbd_outbuf;
bool change = false;
bool completed = false;
bool redlever = true;
Sprite *boy;
Sprite *button[2];
Sprite *buttonpressed[2];
SpriteState boyState = NORMAL;
Sprite *boys[6];
SpriteState girlState = NORMAL;
Sprite *girls[6];
Sprite *cursor;
Sprite *start;
Sprite *exit_button;
Sprite *tryagain_button;
SystemState systemState = RUNNING;
MenuState menuState = START;
Sprite *doorblue;
Sprite *doorred;
Sprite *walls20[1200];
Sprite *arrow;
Sprite *lever;
Sprite *opendoor;
Sprite *leaderboard_button;
Sprite *num[12];
int *levelArray;
int level = 1;
extern struct packet packet;
extern int byte_counter;
extern rtc_values rtc;
LeaderboardEntry leaderboard[LEADERBOARD_SIZE];
void(timer_int_handler)() {
  global_counter++;
}

void load_sprites() {
  start = create_sprite((xpm_map_t) start_xpm, 200, 250, 0, 0);
  boys[0] = create_sprite((xpm_map_t) boy_xpm, 20, 520, 0, 0);
  boys[1] = create_sprite((xpm_map_t) boywalk1_xpm, 20, 520, 0, 0);
  boys[2] = create_sprite((xpm_map_t) boywalk1reverse_xpm, 20, 520, 0, 0);
  boys[3] = create_sprite((xpm_map_t) boywalk2_xpm, 20, 520, 0, 0);
  boys[4] = create_sprite((xpm_map_t) boywalk2reverse_xpm, 20, 520, 0, 0);
  boys[5] = create_sprite((xpm_map_t) boywin_xpm, 20, 520, 0, 0);
  girls[0] = create_sprite((xpm_map_t) girl_xpm, 738, 520, 0, 0);
  girls[1] = create_sprite((xpm_map_t) girlwalk1_xpm, 738, 520, 0, 0);
  girls[2] = create_sprite((xpm_map_t) girlwalk1reverse_xpm, 738, 520, 0, 0);
  girls[3] = create_sprite((xpm_map_t) girlwalk2_xpm, 738, 520, 0, 0);
  girls[4] = create_sprite((xpm_map_t) girlwalk2reverse_xpm, 738, 520, 0, 0);
  girls[5] = create_sprite((xpm_map_t) girlwin_xpm, 738, 520, 0, 0);
  cursor = create_sprite((xpm_map_t) hand_xpm, 0, 0, 0, 0);
  exit_button = create_sprite((xpm_map_t) exit_button_xpm, 200, 450, 0, 0);
  leaderboard_button = create_sprite((xpm_map_t) leaderboard_button_xpm, 200, 350, 0, 0);
  opendoor = create_sprite((xpm_map_t) opendoor_xpm, 100, 100, 0, 0);
  doorblue = create_sprite((xpm_map_t) doorblue_xpm, 500, 100, 0, 0);
  doorred = create_sprite((xpm_map_t) doorred_xpm, 600, 100, 0, 0);
  tryagain_button = create_sprite((xpm_map_t) tryagain_xpm, 200, 350, 0, 0);
  arrow = create_sprite((xpm_map_t) arrow_xpm, 20, 20, 0, 0);
  button[0] = create_sprite((xpm_map_t) buttonleft_xpm, 20, 20, 0, 0);
  button[1] = create_sprite((xpm_map_t) buttonright_xpm, 20, 20, 0, 0);
  buttonpressed[0] = create_sprite((xpm_map_t) buttonleftpressed_xpm, 20, 20, 0, 0);
  buttonpressed[1] = create_sprite((xpm_map_t) buttonrightpressed_xpm, 20, 20, 0, 0);
  /*
  greenleverright = create_sprite((xpm_map_t) greenleverright_xpm, 100, 100, 0, 0);
  redleverleft = create_sprite((xpm_map_t) redleverleft_xpm, 100, 100, 0, 0);

  leftToxic = create_sprite((xpm_map_t) leftToxic_xpm, 100, 100, 0, 0);
  centerToxic = create_sprite((xpm_map_t) centerToxic_xpm, 100, 100, 0, 0);
  rightToxic = create_sprite((xpm_map_t) rightToxic_xpm, 100, 100, 0, 0);
  leftFire = create_sprite((xpm_map_t) leftFire_xpm, 100, 100, 0, 0);
  centerFire = create_sprite((xpm_map_t) centerFire_xpm, 100, 100, 0, 0);
  rightFire = create_sprite((xpm_map_t) rightFire_xpm, 100, 100, 0, 0);
  leftWater = create_sprite((xpm_map_t) leftWater_xpm, 100, 100, 0, 0);
  centerWater = create_sprite((xpm_map_t) centerWater_xpm, 100, 100, 0, 0);
  rightWater = create_sprite((xpm_map_t) rightWater_xpm, 100, 100, 0, 0);
  */
  num[0] = create_sprite((xpm_map_t) num_0_xpm, 100, 100, 0, 0);
  num[1] = create_sprite((xpm_map_t) num_1_xpm, 100, 100, 0, 0);
  num[2] = create_sprite((xpm_map_t) num_2_xpm, 100, 100, 0, 0);
  num[3] = create_sprite((xpm_map_t) num_3_xpm, 100, 100, 0, 0);
  num[4] = create_sprite((xpm_map_t) num_4_xpm, 100, 100, 0, 0);
  num[5] = create_sprite((xpm_map_t) num_5_xpm, 100, 100, 0, 0);
  num[6] = create_sprite((xpm_map_t) num_6_xpm, 100, 100, 0, 0);
  num[7] = create_sprite((xpm_map_t) num_7_xpm, 100, 100, 0, 0);
  num[8] = create_sprite((xpm_map_t) num_8_xpm, 100, 100, 0, 0);
  num[9] = create_sprite((xpm_map_t) num_9_xpm, 100, 100, 0, 0);
  num[10] = create_sprite((xpm_map_t) bar_xpm, 100, 100, 0, 0);
  num[11] = create_sprite((xpm_map_t) dots_xpm, 100, 100, 0, 0);
  int i, x, y;
  for (i = 0; i < 1200; i++) {
    x = (i % 40) * 20;
    y = (i / 40) * 20;
    if (levelArray[i] == 1)
      walls20[i] = (create_sprite((xpm_map_t) wall20_20_xpm, x, y, 0, 0));

    else if (levelArray[i] == 2) {
      lever = create_sprite((xpm_map_t) redleverleft_xpm, x, y, 0, 0);
      walls20[i] = NULL;
    }

    else if (levelArray[i] == 5) {
      lever = create_sprite((xpm_map_t) greenleverright_xpm, x, y, 0, 0);
      walls20[i] = NULL;
    }

    else if (levelArray[i] == 6)
      walls20[i] = (create_sprite((xpm_map_t) leftToxic_xpm, x, y, 0, 0));

    else if (levelArray[i] == 7)
      walls20[i] = (create_sprite((xpm_map_t) centerToxic_xpm, x, y, 0, 0));

    else if (levelArray[i] == 8)
      walls20[i] = (create_sprite((xpm_map_t) rightToxic_xpm, x, y, 0, 0));

    else if (levelArray[i] == 9)
      walls20[i] = (create_sprite((xpm_map_t) leftFire_xpm, x, y, 0, 0));

    else if (levelArray[i] == 10)
      walls20[i] = (create_sprite((xpm_map_t) centerFire_xpm, x, y, 0, 0));

    else if (levelArray[i] == 11)
      walls20[i] = (create_sprite((xpm_map_t) rightFire_xpm, x, y, 0, 0));

    else if (levelArray[i] == 12)
      walls20[i] = (create_sprite((xpm_map_t) leftWater_xpm, x, y, 0, 0));

    else if (levelArray[i] == 13)
      walls20[i] = (create_sprite((xpm_map_t) centerWater_xpm, x, y, 0, 0));

    else if (levelArray[i] == 14)
      walls20[i] = (create_sprite((xpm_map_t) rightWater_xpm, x, y, 0, 0));

    else
      walls20[i] = NULL;
  }
}

void destroy_sprites() {
  
  for (int i = 0; i < 6; i++){
    destroy_sprite(boys[i]);
    destroy_sprite(girls[i]);
  }
  destroy_sprite(start);
  destroy_sprite(exit_button);
  destroy_sprite(tryagain_button);
  destroy_sprite(arrow);
  destroy_sprite(lever);
  destroy_sprite(opendoor);
  destroy_sprite(doorblue);
  destroy_sprite(doorred);
  destroy_sprite(leaderboard_button);
  for (int i = 0; i < 12; i++)
    destroy_sprite(num[i]);

  destroy_sprite(cursor);
  for (int i = 0; i < 1200; i++)
    destroy_sprite(walls20[i]);
  
}

void initialize_leaderboard() {
  for (int i = 0; i < LEADERBOARD_SIZE; i++) {
    leaderboard[i].score = INT_MAX;
  }
}

void updateArrayWithLevel(int level) {
  char filename[64];
  snprintf(filename, sizeof(filename), "/home/lcom/labs/proj/src/visuals/levels/level%d.txt", level);

  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error opening file\n");
    return;
  }

  levelArray[0] = 5;

  int i = 0;
  while (i < ARRAY_SIZE) {
    if (fscanf(file, "%d", &levelArray[i]) != 1) {
      break;
    }
    i++;
  }

  fclose(file);
}

Sprite *checkCollision(Sprite *sp, uint16_t x, uint16_t y) {
  for (int j = 0; j < 1200; j++) {
    if (levelArray[j] == 1 || (levelArray[j] >= 6 && levelArray[j] <= 14)) {    // if it's a wall or lake
      if (levelArray[j] >= 6 && levelArray[j] <= 14) {                          // if it's a lake
        if ((sp == boys[0] && (levelArray[j] >= 9 && levelArray[j] <= 11)) ||   // boy above fire
            (sp == girls[0] && (levelArray[j] >= 12 && levelArray[j] <= 14)) || // girl above water
            (levelArray[j] >= 6 && levelArray[j] <= 8)) {                       // anyone above toxic
          if (!(x >= (walls20[j]->x + walls20[j]->width) ||                     // boneco à direita do lago
                (x + sp->width) <= walls20[j]->x)) {                            // boneco à esquerda do lago
            if (y == walls20[j]->y - sp->height) {                              // boneco com os pés no lago
              change = true;
              menuState = GAMEOVER;
              level = 1;
              level_time = 0;
              kbc_ih();
              reset_states();
              return walls20[j];
            }
          }
        }
      }
      if (!(x >= (walls20[j]->x + walls20[j]->width) || // boneco à direita da parede
            (y + sp->height) <= walls20[j]->y ||        // boneco por cima da parede
            (x + sp->width) <= walls20[j]->x ||         // boneco à esquerda da parede
            y >= (walls20[j]->y + walls20[j]->height))) // boneco por baixo da parede
        return walls20[j];
    }
  }
  return NULL;
}
void update_timer() {
  timer_int_handler();
  if (menuState == GAME) {
    if (completed) {
      rtc_update_values();
      completed = false;
      add_to_leaderboard(rtc.day, rtc.month, rtc.year, rtc.hour, rtc.minute, level_time);
      level_time = 0;
      reset_states();
    }
    if (boyState == WINNING && girlState == WINNING) {
      change = true;
      if (level == 3) {
        level = 1;
        completed = true;
        menuState = WIN;
      }
      else {
        level++;
      }
      reset_states();
      kbc_ih();
    }
    else {
      if (global_counter % FRAME_RATE == 0)
        level_time++;
      if (update(boys[0]) != 0)
        change = true;
      if (update(girls[0]) != 0)
        change = true;
    }
  }
  if (menuState == START && completed) {
    rtc_update_values();
    completed = false;
    add_to_leaderboard(rtc.day, rtc.month, rtc.year, rtc.hour, rtc.minute, level_time);
    level_time = 0;
    reset_states();
  }
  if (change) {
    draw_frame();
    flip_screen();
    change = false;
  }
  else if (change == false && menuState == GAME) {
    boyState = NORMAL;
    girlState = NORMAL;
    draw_frame();
    flip_screen();
  }
}

void update_keyboard() {
  kbc_ih();
  switch (menuState) {
    case GAME:
      if (kbd_outbuf == ESC_BREAK) {
        menuState = START;
        level = 1;
        change = true;
        level_time = 0;
        reset_states();
      }
      action_handler(boys[0]);
      action_handler(girls[0]);
      break;
    case START:
      if (kbd_outbuf == ESC_BREAK) {
        systemState = EXIT;
      }
      break;
    case LEADERBOARD:
      if (kbd_outbuf == ESC_BREAK) {
        menuState = START;
        change = true;
      }
      break;
    case GAMEOVER:
      if (kbd_outbuf == ESC_BREAK) {
        systemState = EXIT;
      }
    default:
      break;
  }
}

void update_mouse() {
  mouse_ih();
  bytes_to_packet();
  if (byte_counter == 3) {
    packet_parse();
    byte_counter = 0;
    change = true;
    move_cursor(&packet);
    check_mouse_click(packet);
  }
}
void update_rtc() {
  if (global_counter % FRAME_RATE == 0)
    rtc_update_values();
}
void move_cursor(struct packet *pp) {
  int16_t x = cursor->x + pp->delta_x;
  int16_t y = cursor->y - pp->delta_y;
  if (x < 0)
    x = 0;
  else if (x > CURSOR_MAX_X)
    x = CURSOR_MAX_X;
  if (y < 0)
    y = 0;
  else if (y > CURSOR_MAX_Y)
    y = CURSOR_MAX_Y;
  cursor->x = x;
  cursor->y = y;
}

void action_handler(Sprite *sp) {
  if (sp->width == 35) {
    if (kbd_outbuf == W_MAKE)
      jump(sp);
    else if (kbd_outbuf == A_MAKE || kbd_outbuf == D_BREAK)
      left(sp);
    else if (kbd_outbuf == D_MAKE || kbd_outbuf == A_BREAK)
      right(sp);
  }
  else if (sp->width == 42) {
    if (kbd_outbuf == UP_MAKE)
      jump(sp);
    else if (kbd_outbuf == LEFT_MAKE || kbd_outbuf == RIGHT_BREAK)
      left(sp);
    else if (kbd_outbuf == RIGHT_MAKE || kbd_outbuf == LEFT_BREAK)
      right(sp);
  }
}

int update(Sprite *sp) {
  updateSpeed(sp);
  return updatePosition(sp);
}

void updateSpeed(Sprite *sp) {
  if (sp->xaccell != 0) {
    if (sp->xspeed + sp->xaccell >= MAX_SPEED)
      sp->xspeed = MAX_SPEED;
    else if (sp->xspeed + sp->xaccell <= -MAX_SPEED)
      sp->xspeed = -MAX_SPEED;
    else
      sp->xspeed += sp->xaccell;
  }
  else {
    if (sp->xspeed > 0) {
      sp->xspeed--;
    }
    else if (sp->xspeed < 0) {
      sp->xspeed++;
    }
  }
  gravity(sp);
}

int updatePosition(Sprite *sp) {
  int change = move_y(sp);
  change += move_x(sp);
  return change;
}

int move_x(Sprite *sp) {
  int x = sp->x;

  if (sp->xspeed > 0) { // moving right
    if (sp->width == 35)
      boyState = WALKRIGHT;
    else if (sp->width == 42)
      girlState = WALKRIGHT;
    x += sp->xspeed;
    if (x > get_hres() - sp->width - 20)
      x = get_hres() - sp->width - 20;
    Sprite *wall = checkCollision(sp, x, sp->y);
    if (wall == NULL) {
      if (sp->x != x) {
        sp->x = x;
        return 1;
      }
    }
    else if (wall->y - (sp->y + sp->height) < -2) {
      x = wall->x - sp->width;
      if (sp->x != x) {
        sp->x = x;
        return 1;
      }
    }
    else {
      if (sp->x != x) {
        sp->x = x;
        return 1;
      }
    }
  }
  else if (sp->xspeed < 0) { // moving left
    if (sp->width == 35)
      boyState = WALKLEFT;
    else if (sp->width == 42)
      girlState = WALKLEFT;
    x += sp->xspeed;
    if (x < 20)
      x = 20;
    Sprite *wall = checkCollision(sp, x, sp->y);
    if (wall == NULL) {
      if (sp->x != x) {
        sp->x = x;
        return 1;
      }
    }
    else if (wall->y - (sp->y + sp->height) < -2) {
      x = wall->x + wall->width;
      if (sp->x != x) {
        sp->x = x;
        return 1;
      }
    }
    else {
      if (sp->x != x) {
        sp->x = x;
        return 1;
      }
    }
  }
  else {
    if (sp->width == 35)
      boyState = NORMAL;
    else if (sp->width == 42)
      girlState = NORMAL;
  }
  return 0;
}

int move_y(Sprite *sp) {
  int16_t y = sp->y;
  if (sp->yspeed > 0) { // moving down
    y += sp->yspeed;
    if (y > get_vres() - sp->height - 20)
      y = get_vres() - sp->height - 20;
    Sprite *wall = checkCollision(sp, sp->x, y);
    if (wall == NULL) {
      if (sp->y != y) {
        sp->y = y;
        if (y == get_vres() - sp->height - 20) {
          set_ground(sp);
        }
        return 1;
      }
      set_ground(sp);
    }
    else if (wall->x - (sp->x + sp->width) < 0 || wall->x + wall->width - sp->x > 0) { // hits wall from above
      if (!sp->isOnGround) {
        y = wall->y - sp->height;
        if (sp->y != y) {
          sp->y = y;
          set_ground(sp);
          return 1;
        }
        set_ground(sp);
      }
      sp->yspeed = 0;
    }
    else {
      if (sp->y != y) {
        sp->y = y;
        return 1;
      }
    }
  }
  else if (sp->yspeed < 0) { // moving up
    y += sp->yspeed;
    if (y < 20)
      y = 20;
    Sprite *wall = checkCollision(sp, sp->x, y);
    if (wall == NULL) {
      if (sp->y != y) {
        sp->y = y;
        return 1;
      }
    }
    else {
      y = wall->y + wall->height;
      sp->y = y;
      sp->yspeed = 0;
      return 1;
    }
  }
  return 0;
}

void jump(Sprite *sp) {
  if (sp->isOnGround) {
    sp->yspeed = -15;
    sp->isOnGround = false;
  }
}

void set_ground(Sprite *sp) {
  sp->isOnGround = true;
  sp->yspeed = 0;
}

void right(Sprite *sp) {
  if (sp->xaccell <= 0)
    sp->xaccell += 2;
}

void left(Sprite *sp) {
  if (sp->xaccell >= 0)
    sp->xaccell -= 2;
}

void gravity(Sprite *sp) {
  sp->yspeed++;
}

void check_mouse_click(struct packet pp) {
  switch (menuState) {
    case START:
      if (pp.lb) {
        if (cursor->x >= start->x && cursor->x <= start->x + start->width &&
            cursor->y >= start->y && cursor->y <= start->y + start->height) {
          menuState = GAME;
        }
        if (cursor->x >= exit_button->x && cursor->x <= exit_button->x + exit_button->width &&
            cursor->y >= exit_button->y && cursor->y <= exit_button->y + exit_button->height) {
          systemState = EXIT;
        }
        if (cursor->x >= leaderboard_button->x && cursor->x <= leaderboard_button->x + leaderboard_button->width &&
            cursor->y >= leaderboard_button->y && cursor->y <= leaderboard_button->y + leaderboard_button->height) {
          menuState = LEADERBOARD;
        }
      }
      break;
    case GAME:
      if (pp.lb) {
        if (cursor->x >= lever->x && cursor->x <= lever->x + lever->width &&
            cursor->y >= lever->y && cursor->y <= lever->y + lever->height) {
          if (redlever) {
            redlever = false;
            updateArrayWithLevel(level + 10);
          }
          else {
            redlever = true;
            updateArrayWithLevel(level);
          }
          change = true;
          destroy_sprite(lever);
          lever = NULL;
          int i, x, y;
          for (i = 0; i < 1200; i++) {
            if (walls20[i] != NULL) {
              destroy_sprite(walls20[i]);
              walls20[i] = NULL;
            }
            x = (i % 40) * 20;
            y = (i / 40) * 20;
            if (levelArray[i] == 1)
              walls20[i] = (create_sprite((xpm_map_t) wall20_20_xpm, x, y, 0, 0));

            else if (levelArray[i] == 2) {
              lever = create_sprite((xpm_map_t) redleverleft_xpm, x, y, 0, 0);
              walls20[i] = NULL;
            }

            else if (levelArray[i] == 5) {
              lever = create_sprite((xpm_map_t) greenleverright_xpm, x, y, 0, 0);
              walls20[i] = NULL;
            }

            else if (levelArray[i] == 6)
              walls20[i] = (create_sprite((xpm_map_t) leftToxic_xpm, x, y, 0, 0));

            else if (levelArray[i] == 7)
              walls20[i] = (create_sprite((xpm_map_t) centerToxic_xpm, x, y, 0, 0));

            else if (levelArray[i] == 8)
              walls20[i] = (create_sprite((xpm_map_t) rightToxic_xpm, x, y, 0, 0));

            else if (levelArray[i] == 9)
              walls20[i] = (create_sprite((xpm_map_t) leftFire_xpm, x, y, 0, 0));

            else if (levelArray[i] == 10)
              walls20[i] = (create_sprite((xpm_map_t) centerFire_xpm, x, y, 0, 0));

            else if (levelArray[i] == 11)
              walls20[i] = (create_sprite((xpm_map_t) rightFire_xpm, x, y, 0, 0));

            else if (levelArray[i] == 12)
              walls20[i] = (create_sprite((xpm_map_t) leftWater_xpm, x, y, 0, 0));

            else if (levelArray[i] == 13)
              walls20[i] = (create_sprite((xpm_map_t) centerWater_xpm, x, y, 0, 0));

            else if (levelArray[i] == 14)
              walls20[i] = (create_sprite((xpm_map_t) rightWater_xpm, x, y, 0, 0));

            else
              walls20[i] = NULL;
          }
        }
      }
      break;
    case GAMEOVER:
      if (pp.lb) {
        if (cursor->x >= tryagain_button->x && cursor->x <= tryagain_button->x + tryagain_button->width &&
            cursor->y >= tryagain_button->y && cursor->y <= tryagain_button->y + tryagain_button->height) {
          menuState = GAME;
          change = true;
          level_time = 0;
          reset_states();
        }
        if (cursor->x >= exit_button->x && cursor->x <= exit_button->x + exit_button->width &&
            cursor->y >= exit_button->y && cursor->y <= exit_button->y + exit_button->height) {
          change = true;
          menuState = START;
        }
      }
      break;
    case WIN:
      if (pp.lb) {
        if (cursor->x >= tryagain_button->x && cursor->x <= tryagain_button->x + tryagain_button->width &&
            cursor->y >= tryagain_button->y && cursor->y <= tryagain_button->y + tryagain_button->height) {
          rtc_update_values();
          completed = false;
          add_to_leaderboard(rtc.day, rtc.month, rtc.year, rtc.hour, rtc.minute, level_time);
          menuState = GAME;
          change = true;
          level_time = 0;
          reset_states();
        }
        if (cursor->x >= exit_button->x && cursor->x <= exit_button->x + exit_button->width &&
            cursor->y >= exit_button->y && cursor->y <= exit_button->y + exit_button->height) {
          change = true;
          menuState = START;
        }
      }
      break;
    case LEADERBOARD:
      if (pp.lb) {
        if (cursor->x >= arrow->x && cursor->x <= arrow->x + arrow->width &&
            cursor->y >= arrow->y && cursor->y <= arrow->y + arrow->height) {
          change = true;
          menuState = START;
        }
      }
    default:
      break;
  }
}

void reset_states() {
  changesprite = 0;
  spriteindex = 0;
  boyState = NORMAL;
  girlState = NORMAL;
  redlever = true;
  for (int i = 0; i < 6; i++) {
    boys[i]->x = 20;
    boys[i]->y = 520;
    boys[i]->xspeed = 0;
    boys[i]->yspeed = 0;
    boys[i]->xaccell = 0;
    girls[i]->x = 738;
    girls[i]->y = 520;
    girls[i]->xspeed = 0;
    girls[i]->yspeed = 0;
    girls[i]->xaccell = 0;
  }
  updateArrayWithLevel(level);
  destroy_sprite(lever);
  lever = NULL;
  int i, x, y;
  for (i = 0; i < 1200; i++) {
    if (walls20[i] != NULL) {
      destroy_sprite(walls20[i]);
      walls20[i] = NULL;
    }
    x = (i % 40) * 20;
    y = (i / 40) * 20;
    if (levelArray[i] == 1)
      walls20[i] = (create_sprite((xpm_map_t) wall20_20_xpm, x, y, 0, 0));

    else if (levelArray[i] == 2) {
      lever = create_sprite((xpm_map_t) redleverleft_xpm, x, y, 0, 0);
      walls20[i] = NULL;
    }

    else if (levelArray[i] == 5) {
      lever = create_sprite((xpm_map_t) greenleverright_xpm, x, y, 0, 0);
      walls20[i] = NULL;
    }

    else if (levelArray[i] == 6)
      walls20[i] = (create_sprite((xpm_map_t) leftToxic_xpm, x, y, 0, 0));

    else if (levelArray[i] == 7)
      walls20[i] = (create_sprite((xpm_map_t) centerToxic_xpm, x, y, 0, 0));

    else if (levelArray[i] == 8)
      walls20[i] = (create_sprite((xpm_map_t) rightToxic_xpm, x, y, 0, 0));

    else if (levelArray[i] == 9)
      walls20[i] = (create_sprite((xpm_map_t) leftFire_xpm, x, y, 0, 0));

    else if (levelArray[i] == 10)
      walls20[i] = (create_sprite((xpm_map_t) centerFire_xpm, x, y, 0, 0));

    else if (levelArray[i] == 11)
      walls20[i] = (create_sprite((xpm_map_t) rightFire_xpm, x, y, 0, 0));

    else if (levelArray[i] == 12)
      walls20[i] = (create_sprite((xpm_map_t) leftWater_xpm, x, y, 0, 0));

    else if (levelArray[i] == 13)
      walls20[i] = (create_sprite((xpm_map_t) centerWater_xpm, x, y, 0, 0));

    else if (levelArray[i] == 14)
      walls20[i] = (create_sprite((xpm_map_t) rightWater_xpm, x, y, 0, 0));

    else
      walls20[i] = NULL;
  }
}

void add_to_leaderboard(uint8_t day, uint8_t month, uint8_t year, uint8_t hour, uint8_t minute, int score) {
  int insert = -1;
  for (int i = 0; i < LEADERBOARD_SIZE; i++) {
    if (score < leaderboard[i].score || leaderboard[i].year == 0) {
      insert = i;
      break;
    }
  }

  if (insert >= 0) {
    for (int j = LEADERBOARD_SIZE - 1; j > insert; j--) {
      leaderboard[j] = leaderboard[j - 1];
    }
    leaderboard[insert].day = day;
    leaderboard[insert].month = month;
    leaderboard[insert].year = year;
    leaderboard[insert].hour = hour;
    leaderboard[insert].minute = minute;
    leaderboard[insert].score = score;
  }
}

void write_leaderboard_data() {
  FILE *file = fopen("/home/lcom/labs/proj/src/visuals/leaderboard_data.txt", "w");
  if (file == NULL) {
    printf("error writing leaderboard data\n");
    return;
  }

  for (int i = 0; i < LEADERBOARD_SIZE; i++) {
    fprintf(file, "%hhu %hhu %hhu %hhu %hhu %d\n", leaderboard[i].day, leaderboard[i].month, leaderboard[i].year, leaderboard[i].hour, leaderboard[i].minute, leaderboard[i].score);
  }

  fclose(file);
}

void read_leaderboard_data() {
  FILE *file = fopen("/home/lcom/labs/proj/src/visuals/leaderboard_data.txt", "r");
  if (file == NULL) {
    printf("error reading leaderboard data!\n");
    return;
  }

  for (int i = 0; i < LEADERBOARD_SIZE; i++) {
    fscanf(file, "%hhu %hhu %hhu %hhu %hhu %d", &leaderboard[i].day, &leaderboard[i].month, &leaderboard[i].year, &leaderboard[i].hour, &leaderboard[i].minute, &leaderboard[i].score);
  }

  fclose(file);
}
