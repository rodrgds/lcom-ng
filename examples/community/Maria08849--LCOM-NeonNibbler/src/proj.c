#include <lcom/lcf.h>
#include <minix/sysutil.h>

#include "kbc.h"
#include "mouse.h"
#include "vg.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/doc/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/doc/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

/** @defgroup Project
 * @{
 *
 * Project's main functions
 */

extern uint16_t data;
extern uint8_t data_m;
extern int timerCounter;

// uint8_t pacman_score = 0;
static uint8_t ghost_score = 0;
static int pspeed = 2;
static int gspeed = 2;
static uint8_t pdir = STATIONARY;
static uint8_t gdir = STATIONARY;
// x coordinate for pacman
static uint16_t px = 562;
// y coordinate for pacman
static uint16_t py = 402;
// x coordinate for ghost
static uint16_t gx = 562;
// y coordinate for ghost
static uint16_t gy = 800;
static bool stop = false;
bool hasPowerup = false;
bool usedPowerup = false;
static uint8_t gameState = MENU_SPLAY;

/**
 * @brief Sets the initial variables for the start of the game.
 */
void(set_game_start_variables)() {
  pspeed = 2;
  gspeed = 2;
  pdir = STATIONARY;
  gdir = STATIONARY;
  px = 562;
  py = 402;
  gx = 562;
  gy = 800;
}
/**
 * @brief The main loop of the project.
 * @return 0 if the loop finishes successfully, 1 otherwise.
 */
int(proj_main_loop)(int argc, char **argv) {
  vg_init(DIRECT_COLOR_1152x864);

  uint8_t bit_no_t;
  uint8_t bit_no_k;
  uint8_t bit_no_m;
  int r;
  int ipc_status;
  message msg;
  int poweruptime = 0;
  struct packet pack;
  int counter = 1;

  if (vg_store_menu_background() != 0)
    return 1;
  if (vg_store_game_background() != 0)
    return 1;

  if (keyboard_subscribe_interrupts(&bit_no_k) != 0)
    return 1;
  if (timer_subscribe_int(&bit_no_t) != 0)
    return 1;
  if (mouse_subscribe_interrupts(&bit_no_m) != 0)
    return 1;
  if (timer_set_frequency(0, 60) != 0)
    return 1;

  while (!stop) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(bit_no_m)) {
            mouse_ih();
            process_mouse_data_t(data_m, &counter, &pack);
            if (gameState == GAME) {
              if (hasPowerup) {
                if (pack.lb == 1) {
                  pspeed = 4;
                  poweruptime = 5;
                  hasPowerup = false;
                  usedPowerup = true;
                }
                if (pack.rb == 1) {
                  pspeed = 4;
                  poweruptime = 5;
                  hasPowerup = false;
                  usedPowerup = true;
                }
              }
            }
          }
          if (msg.m_notify.interrupts & BIT(bit_no_k)) {
            kbc_ih();
            // process_and_print_keyboard_data(data);
            if (gameState == MENU_SPLAY) {
              switch (data) {
                case W_MAKECODE:
                  gameState = MENU_NPLAY;
                  break;
                case S_MAKECODE:
                  gameState = MENU_NPLAY;
                  break;
                case UP_MAKECODE:
                  gameState = MENU_NPLAY;
                  break;
                case DOWN_MAKECODE:
                  gameState = MENU_NPLAY;
                  break;
                case ENTER_MAKECODE:
                  gameState = GAME;
              }
            }
            else if (gameState == MENU_NPLAY) {
              switch (data) {
                case W_MAKECODE:
                  gameState = MENU_SPLAY;
                  break;
                case S_MAKECODE:
                  gameState = MENU_SPLAY;
                  break;
                case UP_MAKECODE:
                  gameState = MENU_SPLAY;
                  break;
                case DOWN_MAKECODE:
                  gameState = MENU_SPLAY;
                  break;
                case ENTER_MAKECODE:
                  gameState = EXIT;
              }
            }
            else if (gameState == GAME) {
              switch (data) {
                // pacman conditions
                case UP_MAKECODE:
                  pdir = UP;
                  break;
                case DOWN_MAKECODE:
                  pdir = DOWN;
                  break;
                case LEFT_MAKECODE:
                  pdir = LEFT;
                  break;
                case RIGHT_MAKECODE:
                  pdir = RIGHT;
                  break;

                // ghost conditions
                case W_MAKECODE:
                  gdir = UP;
                  break;
                case S_MAKECODE:
                  gdir = DOWN;
                  break;
                case A_MAKECODE:
                  gdir = LEFT;
                  break;
                case D_MAKECODE:
                  gdir = RIGHT;
                  break;

                // leave game
                case BREAK_ESC:
                  pdir = STATIONARY;
                  gdir = STATIONARY;
                  gameState = EXIT;
                  break;
              }
            }
            else if (gameState == EXIT) {
              switch (data) {
                case ENTER_MAKECODE:
                  stop = true;
                  break;
                case BREAK_ESC:
                  gameState = MENU_NPLAY;
                  break;
              }
            }
          }

          if (msg.m_notify.interrupts & BIT(bit_no_t)) {
            if (gameState == MENU_NPLAY) {
              if (vg_draw_menu_nplay() != 0) {
                printf("Error in vg_draw_menu_nplay\n");
                vg_exit();
                return 1;
              }
              if (vg_copy_back_buffer() != 0)
                return 1;
            }
            else if (gameState == MENU_SPLAY) {
              if (vg_draw_menu_splay() != 0) {
                printf("Error in vg_draw_menu_splay\n");
                vg_exit();
                return 1;
              }
              if (vg_copy_back_buffer() != 0)
                return 1;
            }
            else if (gameState == GAME) {
              if (vg_fill_screen(0x0000) != 0)
                return 1;
              if (vg_copy_game_background_buffer() != 0)
                return 1;

              if (vg_draw_ghost_score(300, 5, ghost_score) != 0)
                return 1;

              if (hasPowerup) {
                if (vg_draw_sprite(get_powerpacman_sprite(), px, py) != 0)
                  return 1;
              }
              if (!hasPowerup && !usedPowerup) {
                if (vg_draw_sprite(get_pacman_sprite(), px, py) != 0)
                  return 1;
              }
              if (usedPowerup) {
                if (vg_draw_sprite(get_upacman_sprite(), px, py) != 0)
                  return 1;
              }

              if (vg_draw_sprite(get_ghost_sprite(), gx, gy) != 0)
                return 1;
              if (vg_draw_collectibles() != 0)
                return 1;
              if (vg_draw_powerups() != 0)
                return 1;
              if (vg_copy_back_buffer() != 0)
                return 1;

              if (poweruptime > 0) {
                timer_int_handler();
                if (timerCounter % 60 == 0) {
                  poweruptime--;
                }
              }
              if (poweruptime == 0) {
                pspeed = 2;
                usedPowerup = false;
              }
              if (sprites_collided(get_pacman_sprite(), get_ghost_sprite(), px, py, gx, gy)) {
                ghost_score += 1;
                if (ghost_score == 3)
                  gameState = MENU_NPLAY;
                set_game_start_variables();
              }
              if (gameOver()) {
                gameState = MENU_SPLAY;
              }
              switch (pdir) {
                case STATIONARY:
                  break;
                case UP:
                  if (!will_collide_with_background(get_pacman_sprite(), px, py - pspeed))
                    py -= pspeed;
                  if ((will_collide_with_powerup) (get_pacman_sprite(), px + 15, py + 15)) {
                    hasPowerup = true;
                  }
                  if ((will_collide_with_collectible) (get_pacman_sprite(), px + 15, py + 15))
                    continue;
                  break;
                case DOWN:
                  if (!will_collide_with_background(get_pacman_sprite(), px, py + pspeed))
                    py += pspeed;
                  if ((will_collide_with_powerup) (get_pacman_sprite(), px + 15, py + 15)) {
                    hasPowerup = true;
                  }
                  if ((will_collide_with_collectible) (get_pacman_sprite(), px + 15, py + 15))
                    continue;
                  break;
                case LEFT:
                  if (!will_collide_with_background(get_pacman_sprite(), px - pspeed, py))
                    px -= pspeed;
                  if ((will_collide_with_powerup) (get_pacman_sprite(), px + 15, py + 15)) {
                    hasPowerup = true;
                  }
                  if ((will_collide_with_collectible) (get_pacman_sprite(), px + 15, py + 15))
                    continue;
                  break;
                case RIGHT:
                  if (!will_collide_with_background(get_pacman_sprite(), px + pspeed, py))
                    px += pspeed;
                  if ((will_collide_with_powerup) (get_pacman_sprite(), px + 15, py + 15)) {
                    hasPowerup = true;
                  }
                  if ((will_collide_with_collectible) (get_pacman_sprite(), px + 15, py + 15))
                    continue;
                  break;
              }

              switch (gdir) {
                case STATIONARY:
                  break;
                case UP:
                  if (!will_collide_with_background(get_ghost_sprite(), gx, gy - gspeed))
                    gy -= gspeed;
                  break;
                case DOWN:
                  if (!will_collide_with_background(get_ghost_sprite(), gx, gy + gspeed))
                    gy += gspeed;
                  break;
                case LEFT:
                  if (!will_collide_with_background(get_ghost_sprite(), gx - gspeed, gy))
                    gx -= gspeed;
                  break;
                case RIGHT:
                  if (!will_collide_with_background(get_ghost_sprite(), gx + gspeed, gy))
                    gx += gspeed;
                  break;
              }
            }
            else if (gameState == EXIT) {
              if (vg_fill_screen(0x0000) != 0)
                return 1;
              if (vg_copy_menu_background_buffer() != 0)
                return 1;
              if (vg_draw_sprite(get_cdoor_sprite(), 500, 400) != 0)
                return 1;
              if (vg_copy_back_buffer() != 0)
                return 1;
            }
          }
          break;
        default:
          break;
      }
    }
  }

  if (vg_exit() != 0)
    return 1;

  free_buffers();
  destroy_sprites();

  if (keyboard_unsubscribe_interrupts() != 0)
    return 1;
  if (timer_unsubscribe_int() != 0)
    return 1;
  if (mouse_unsubscribe_interrupts() != 0)
    return 1;

  return 0;
}
