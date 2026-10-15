#include "game.h"
#include <stdlib.h>

#define MAX_ALIENS 24
#define MAX_PROJECTILES 100
#define MAX_BARRIERS 4

int current_wave = 1;
bool wave_cleared = false;
int wave_spawn_delay = 90; 

int score = 0;

extern uint8_t scancode;
extern struct packet mouse_packet;
extern uint8_t mouse_byte_index;
extern unsigned int counter;

bool mouse_dirty;
bool need_redraw;
bool mouse_subscribed;
bool kbd_subscribed;

int direction = 0; // 0 - left, 1 - down, 2 - right, 3 - down || ALIEN MOVEMENT
int player_delta = 0;

Cursor *mouse_cursor;
Player *player;
Alien *aliens[MAX_ALIENS];
Barrier *barriers[MAX_BARRIERS];
Projectile *projectiles[MAX_PROJECTILES];
Entry *logo_entry, *play_entry, *leaderboard_entry, *exit_entry;

GameState game_state = GAME_STATE_MENU;

void exit_game() {
  destroyPlayer(player);
  for (int i = 0; i < MAX_ALIENS; i++) {
    destroyAlien(aliens[i]);
    aliens[i] = NULL;
  }
  for (int i = 0; i < 4; i++) {
    destroyBarrier(barriers[i]);
    barriers[i] = NULL;
  }
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    destroyProjectile(projectiles[i]);
    projectiles[i] = NULL;
  }
  game_state = EXIT_GAME;
}

void game_over() {
  destroyPlayer(player);
  for (int i = 0; i < MAX_ALIENS; i++) {
    destroyAlien(aliens[i]);
    aliens[i] = NULL;
  }
  for (int i = 0; i < 4; i++) {
    destroyBarrier(barriers[i]);
    barriers[i] = NULL;
  }
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    destroyProjectile(projectiles[i]);
    projectiles[i] = NULL;
  }
  game_state = GAME_STATE_GAME_OVER;
}

void spawnAlienWave(){
  for (int i = 0; i < MAX_ALIENS; i++) {
    if (aliens[i] != NULL) {
      destroyAlien(aliens[i]);
      aliens[i] = NULL;
    }
  }

  int alien_health = 1 + (current_wave / 3);
  int iterator = 0;
  if (current_wave < 4) {
    iterator = current_wave;
  } else iterator = 4;

  for (int i = 0; i < iterator; i++) {
    for (int j = 0; j < 6; j++) {
      if (i == 0)
        aliens[i * 6 + j] = createAlien(40 + j * 70, 10 + i * 50, alien_health, alien1);
      else if (i == 1)
        aliens[i * 6 + j] = createAlien(60 + j * 70, 10 + i * 50, alien_health, alien2);
      else if (i == 2)
        aliens[i * 6 + j] = createAlien(40 + j * 70, 10 + i * 50, alien_health, alien3);
      else if (i == 3)
        aliens[i * 6 + j] = createAlien(60 + j * 70, 10 + i * 50, alien_health, alien2);
    }
  }
}

void game_menu() {
  if (mouse_dirty) {
    if (drawEntry(logo_entry) != 0)
      return;
    if (drawEntry(play_entry) != 0)
      return;
    if (drawEntry(leaderboard_entry) != 0)
      return;
    if (drawEntry(exit_entry) != 0)
      return;
    if (drawCursor(mouse_cursor) != 0)
      return;
    video_swap_buffer();
    video_clear_buffer();
    mouse_dirty = false;
  }
}

void alien_tick() {
  for (int i = 0; i < MAX_ALIENS; i++) {
    if (aliens[i] != NULL) {
      alienMove(aliens[i], direction);
      int rnd = rand() % (101);
      if (rnd < 100 / MAX_ALIENS) {
        for (int j = 0; j < MAX_PROJECTILES; j++) {
          if (projectiles[j] == NULL) {
            projectiles[j] = createProjectile((int) aliens[i]->x + (aliens[i]->sprite->width / 2) - (a_projectile->width / 2), aliens[i]->y + aliens[i]->sprite->height, 'A', a_projectile);
            break;
          }
        }
      }
    }
  }
  direction = (direction + 1) % 4;
  need_redraw = true;
}

void projectile_tick() {
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    if (projectiles[i] != NULL && projectiles[i]->active && projectiles[i]->type == 'P') {
      projectileMove(projectiles[i]);
      for (int j = 0; j < MAX_ALIENS; j++) {
        if (aliens[j] != NULL && checkCollisionAlien(projectiles[i], aliens[j])) {
          if (damageAlien(aliens[j])) {
            draw_sprite(alien_explosion, aliens[j]->x, aliens[j]->y);
            destroyAlien(aliens[j]);
            aliens[j] = NULL;
            player->score += 10;
          }
          destroyProjectile(projectiles[i]);
          projectiles[i] = NULL;
          return;
        }
      }
    }
    else if (projectiles[i] != NULL && projectiles[i]->active && projectiles[i]->type == 'A') {
      projectileMove(projectiles[i]);
      if (checkCollisionPlayer(projectiles[i], player)) {
        if (damagePlayer(player)) {
          score = player->score;
          game_over();

          createGameOverSprites();

          play_entry = createEntry(200, 280, true, play_highlight);
          exit_entry = createEntry(500, 280, false, quit);

          if (vg_draw_rectangle(0, 0, 800, 600, 0x000000) != 0)
            return;
          draw_sprite(score_text, 250, 50); 
          draw_number(score, 400, 50); 

          video_swap_buffer();
          video_clear_buffer();

          need_redraw = true;
          return;
        }
        destroyProjectile(projectiles[i]);
        projectiles[i] = NULL;
        return;
      }
      for (int j = 0; j < MAX_BARRIERS; j++) {
        if (barriers[j] != NULL && checkCollisionBarrier(projectiles[i], barriers[j])) {
          if (damageBarrier(barriers[j])) {
            destroyBarrier(barriers[j]);
            barriers[j] = NULL;
          }
          destroyProjectile(projectiles[i]);
          projectiles[i] = NULL;
          return;
        }
      }
    }
  }
}

int game_redraw() { 
  if (vg_draw_rectangle(500, 0, 300, 600, 0x0A0E30) != 0)
    return 1;
  drawHud(player);
  drawWave(current_wave);
  drawPlayer(player);
  for (int i = 0; i < MAX_ALIENS; i++) {
    if (aliens[i] != NULL) {
      drawAlien(aliens[i]);
    }
  }
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    if (projectiles[i] != NULL) {
      if (!projectiles[i]->active) {
        destroyProjectile(projectiles[i]);
        projectiles[i] = NULL;
      }
      else
        drawProjectile(projectiles[i]);
    }
  }
  for (int i = 0; i < 4; i++) {
    if (barriers[i] != NULL) {
      drawBarrier(barriers[i]);
    }
  }
  video_swap_buffer();
  video_clear_buffer();
  need_redraw = false;
  return 0;
}

int unsubscribe() {
  if (mouse_subscribed) {
    if (mouse_unsubscribe_int() != 0)
      return 1;
    if (mouse_issue_cmd(MOUSE_DISABLE_DATA_REPORTING) != 0)
      return 1;
  }
  if (kbd_subscribed)
    if (kbd_unsubscribe_int() != 0)
      return 1;
  if (timer_unsubscribe_int() != 0)
    return 1;
  return 0;
}

int game_loop() {
  int ipc_status;
  message msg;
  uint8_t mouse_bit_no, timer_bit_no, keyboard_bit_no;

  if (mouse_issue_cmd(MOUSE_SET_STREAM_MODE) != 0)
    return 1;
  if (mouse_issue_cmd(MOUSE_ENABLE_DATA_REPORTING) != 0)
    return 1;
  if (mouse_subscribe_int(&mouse_bit_no) != 0)
    return 1;

  if (timer_subscribe_int(&timer_bit_no) != 0)
    return 1;
  if (timer_set_frequency(0, 30) != 0)
    return 1;

  mouse_dirty = true;
  need_redraw = true;
  int cooldown = 0;

  bool kbd_subscribed = false, mouse_subscribed = true;

  createMenuSprites();
  mouse_cursor = createCursor(cursor);
  logo_entry = createEntry(100, 100, false, logo);
  play_entry = createEntry(500, 200, false, play);
  leaderboard_entry = createEntry(500, 300, false, leaderboard);
  exit_entry = createEntry(500, 400, false, quit);

  while (game_state != EXIT_GAME) {
    if ((driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("Error");
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & timer_bit_no) {
            timer_int_handler();
            if (game_state == GAME_STATE_MENU) {
              game_menu();
            }
            else if (game_state == GAME_STATE_PLAYING) {
              if (counter % 45 == 0) {
                alien_tick();
              }
              if (cooldown > 0) {
                cooldown--;
              }
              if (counter % 2 == 0) { 
                if (player != NULL) {
                  playerMove(player, player_delta);
                }
                projectile_tick();
                if (game_state == GAME_STATE_GAME_OVER) break;
                need_redraw = true;
              }
              int aliens_alive = 0;
              for (int i = 0; i < MAX_ALIENS; i++) {
                if (aliens[i] != NULL) {
                  aliens_alive++;
                }
              }
              if (aliens_alive == 0 && !wave_cleared) {
                wave_cleared = true;
                current_wave++;
                wave_spawn_delay = 90;
                need_redraw = true;
              }
              if (wave_cleared && wave_spawn_delay > 0) {
                wave_spawn_delay--;
                if (wave_spawn_delay <= 0) {
                  spawnAlienWave();
                  wave_cleared = false;
                  wave_spawn_delay = 90;
                }
              }
              printf("Checkpoint 4\n");
              if (need_redraw) {
                if (game_redraw() != 0) return 1;
              }
            }
            else if (game_state == GAME_STATE_GAME_OVER) {
              if (need_redraw) {
                if (vg_draw_rectangle(0, 0, 800, 600, 0x000000) != 0)
                  return 1;
                draw_sprite(score_text, 250, 50);
                draw_number(score, 400, 50);    
                drawEntry(play_entry);
                drawEntry(exit_entry);
                video_swap_buffer();
                video_clear_buffer();
                need_redraw = false;
              }
            }
          }
          if ((msg.m_notify.interrupts & mouse_bit_no)) {
            if (game_state == GAME_STATE_MENU) {
              mouse_ih();
              mouse_place_byte();
              if (mouse_byte_index == 3) {
                mouse_create_packet();
                mouse_byte_index = 0;
                cursor_update_location(mouse_cursor, &mouse_packet);
                mouse_dirty = true;

                if ((play_entry->selected == false && 
                    mouse_cursor->x >= play_entry->x && mouse_cursor->x <= play_entry->x + play_entry->sprite->width &&
                    mouse_cursor->y >= play_entry->y && mouse_cursor->y <= play_entry->y + play_entry->sprite->height) ||
                    (play_entry->selected == true &&
                    (mouse_cursor->x < play_entry->x || mouse_cursor->x > play_entry->x + play_entry->sprite->width ||
                    mouse_cursor->y < play_entry->y || mouse_cursor->y > play_entry->y + play_entry->sprite->height))) {
                  toggleEntry(play_entry, play, play_highlight);
                } 

                if ((leaderboard_entry->selected == false && 
                    mouse_cursor->x >= leaderboard_entry->x && mouse_cursor->x <= leaderboard_entry->x + leaderboard_entry->sprite->width &&
                    mouse_cursor->y >= leaderboard_entry->y && mouse_cursor->y <= leaderboard_entry->y + leaderboard_entry->sprite->height) ||
                    (leaderboard_entry->selected == true &&
                    (mouse_cursor->x < leaderboard_entry->x || mouse_cursor->x > leaderboard_entry->x + leaderboard_entry->sprite->width ||
                    mouse_cursor->y < leaderboard_entry->y || mouse_cursor->y > leaderboard_entry->y + leaderboard_entry->sprite->height))) {
                  toggleEntry(leaderboard_entry, leaderboard, leaderboard_highlight);
                }

                if ((exit_entry->selected == false && 
                    mouse_cursor->x >= exit_entry->x && mouse_cursor->x <= exit_entry->x + exit_entry->sprite->width &&
                    mouse_cursor->y >= exit_entry->y && mouse_cursor->y <= exit_entry->y + exit_entry->sprite->height) ||
                    (exit_entry->selected == true &&
                    (mouse_cursor->x < exit_entry->x || mouse_cursor->x > exit_entry->x + exit_entry->sprite->width ||
                    mouse_cursor->y < exit_entry->y || mouse_cursor->y > exit_entry->y + exit_entry->sprite->height))) {
                  toggleEntry(exit_entry, quit, quit_highlight);
                }

                if (mouse_packet.lb && play_entry->selected) {
                  destroyCursor(mouse_cursor);
                  destroyEntry(play_entry);
                  destroyEntry(leaderboard_entry);
                  destroyEntry(exit_entry);
                  mouse_cursor = NULL;
                  mouse_dirty = false;
                  play_entry = NULL;
                  leaderboard_entry = NULL;
                  exit_entry = NULL;

                  game_state = GAME_STATE_PLAYING;

                  if (vg_draw_rectangle(0, 0, 800, 600, 0x000000) != 0)
                    return 1;
                  video_swap_buffer();
                  video_clear_buffer();

                  if (mouse_unsubscribe_int() != 0)
                    return 1;
                  if (mouse_issue_cmd(MOUSE_DISABLE_DATA_REPORTING) != 0)
                    return 1;
                  mouse_subscribed = false;

                  if (kbd_subscribe_int(&keyboard_bit_no) != 0)
                    return 1;
                  kbd_subscribed = true;

                  createGameSprites();
                  player = createPlayer(54, 550, 3, 0, airship);

                  spawnAlienWave();

                  for (int i = 0; i < 4; i++) {
                    barriers[i] = createBarrier(40 + i * 125, 475);
                  }

                  need_redraw = true;
                  break;
                }
                else if (mouse_packet.lb && exit_entry->selected) {
                  destroyCursor(mouse_cursor);
                  destroyEntry(play_entry);
                  destroyEntry(leaderboard_entry);
                  destroyEntry(exit_entry);
                  mouse_cursor = NULL;
                  mouse_dirty = false;
                  play_entry = NULL;
                  leaderboard_entry = NULL;
                  exit_entry = NULL;

                  game_state = EXIT_GAME;
                  break;
                }
              }
            }
          }
          if ((msg.m_notify.interrupts & keyboard_bit_no) ) {
            if (game_state == GAME_STATE_PLAYING) {
              kbc_ih();
              if (scancode == MAKE_A) {
                player_delta = -8;
                need_redraw = true;
              }
              else if (scancode == MAKE_D) {
                player_delta = 8;
                need_redraw = true;
              }
              else if (scancode == BREAK_A || scancode == BREAK_D) {
                player_delta = 0;
                need_redraw = true;
              }
              else if (scancode == MAKE_SPACE && cooldown == 0) {
                for (int i = 0; i < MAX_PROJECTILES; i++) {
                  if (projectiles[i] == NULL) {
                    projectiles[i] = createProjectile((int) player->x + (player->sprite->width / 2) - (p_projectile->width / 2), player->y - player->sprite->height, 'P', p_projectile); // later on, add velocity
                    cooldown = 30;
                    need_redraw = true;
                    break;
                  }
                }
              }
              else if (scancode == BREAK_ESC) {
                exit_game();
                if (unsubscribe() != 0) return 1;
              }
            }
            else if (game_state == GAME_STATE_GAME_OVER) {
              kbc_ih();
              if (scancode == MAKE_A || scancode == MAKE_D) {
                  toggleEntry(play_entry, play, play_highlight);
                  toggleEntry(exit_entry, quit, quit_highlight);

                  need_redraw = true;
                }
              else if (scancode == MAKE_SPACE) {
                if (play_entry->selected == true) {
                  game_state = GAME_STATE_PLAYING;

                  if (vg_draw_rectangle(0, 0, 800, 600, 0x000000) != 0)
                    return 1;
                
                  video_swap_buffer();
                  video_clear_buffer();

                  createGameSprites();
                  player = createPlayer(54, 550, 3, 0, airship);

                  spawnAlienWave();

                  for (int i = 0; i < 4; i++) {
                    barriers[i] = createBarrier(40 + i * 125, 475);
                  }

                  need_redraw = true;
                }
                else if (exit_entry->selected == true) {
                  destroyEntry(play_entry);
                  destroyEntry(exit_entry);
                  play_entry = NULL;
                  exit_entry = NULL;
                  if (unsubscribe() != 0) return 1;
                  game_state = EXIT_GAME;
                }
              }
            }
          }
          break;
      }
    }
  }
  return 0;
}
