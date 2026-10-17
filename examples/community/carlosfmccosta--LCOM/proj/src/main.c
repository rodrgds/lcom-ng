/**
 * @file main.c
 * @brief Entry point and main loop for the game application.
 *
 * Handles initialization, main event loop, state transitions, and cleanup.
 * Manages global resources such as menus, game state, event queue, and cursor.
 */

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Drivers
#include <controller/KBC/keyboard/keyboard.h>
#include <controller/KBC/mouse/mouse.h>
#include <controller/timer/timer.h>
#include <controller/video_gr/video_gr.h>

// Others
#include "controller/game.h"
#include "controller/state.h"
#include "view/leaderboard.h"
#include "view/leaderboard_page.h"
#include "view/menu.h"
#include "view/write_name.h"
#include <controller/event.h>
#include <queue.h>

// Global vars
/**
 * @brief Current timer counter, incremented by timer interrupts.
 */
extern int counter;

/**
 * @brief Current application state.
 */
state_t state;

/**
 * @brief Pointer to the main menu structure.
 */
menu_t *menu = NULL;

/**
 * @brief Pointer to the current game instance.
 */
game_t *game = NULL;

/**
 * @brief Pointer to the event queue.
 */
queue_t *q = NULL;

/**
 * @brief Pointer to the cursor sprite.
 */
sprite_t *cursor_sprite = NULL;

/**
 * @brief Pointer to the write name page (for entering player names).
 */
write_name_t *page = NULL;

/**
 * @brief Pointer to the leaderboard page.
 */
lb_page_t *lb_page = NULL;

#define ANIM_DELAY 6
unsigned int anim_delay = 0;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int cleanup() {
  int status = 0; // Tracks if any cleanup step fails

  printf("\x1b[34mCleanup started...\x1b[0m\n");

  printf("\x1b[35mExiting graphics mode...\x1b[0m\n");
  vg_exit();

  printf("\x1b[35mUnsubscribing keyboard...\x1b[0m\n");
  if (kbd_unsubscribe_int() != 0) {
    printf("\x1b[0;31mError unsubscribing keyboard.\x1b[0m\n");
    status = 1;
  }

  printf("\x1b[35mUnsubscribing timer...\x1b[0m\n");
  if (timer_unsubscribe_int() != 0) {
    printf("\x1b[0;31mError unsubscribing timer.\x1b[0m\n");
    status = 1;
  }

  printf("\x1b[35mUnsubscribing mouse...\x1b[0m\n");
  if (mouse_unsubscribe_int() != 0) {
    printf("\x1b[0;31mError unsubscribing mouse.\x1b[0m\n");
    status = 1;
  }

  printf("\x1b[35mDisabling mouse data reporting...\x1b[0m\n");
  if (mouse_disable_data_reporting() != 0) {
    printf("\x1b[0;31mError disabling mouse.\x1b[0m\n");
    status = 1;
  }

  if (game) {
    printf("\x1b[35mDestroying game...\x1b[0m\n");
    destroy_game(game);
    game = NULL;
  }

  if (menu) {
    printf("\x1b[35mDestroying menu...\x1b[0m\n");
    destroy_menu(menu);
    menu = NULL;
  }

  if (page) {
    printf("\x1b[35mDestroying page...\x1b[0m\n");
    destroy_write_name_page(page);
    page = NULL;
  }

  if (lb_page) {
    printf("\x1b[35mDestroying leaderboard...\x1b[0m\n");
    destroy_lb_page(lb_page);
    lb_page = NULL;
  }

  if (q) {
    printf("\x1b[35mClearing event queue...\x1b[0m\n");
    while (is_not_empty_queue(q)) {
      event_t event_data;
      if (get_queue(q, &event_data) == 0) {
        if (event_data.k) {
          free(event_data.k);
          event_data.k = NULL;
        }
        if (event_data.pack) {
          free(event_data.pack);
          event_data.pack = NULL;
        }
      }
    }

    printf("\x1b[35mDeleting event queue...\x1b[0m\n");
    delete_queue(q);
    q = NULL;
  }

  if (cursor_sprite) {
    printf("\x1b[35mDestroying cursor sprite...\x1b[0m\n");
    destroy_sprite(cursor_sprite);
    cursor_sprite = NULL;
  }

  printf("\x1b[32mCleanup complete with status: %d\x1b[0m\n", status);
  return status;
}


/**
 * @brief Moves the cursor sprite according to mouse input, with boundary checks.
 *
 * Updates the cursor's position based on mouse movement, ensuring it stays within screen bounds.
 * @param h_res Horizontal resolution of the screen.
 * @param v_res Vertical resolution of the screen.
 * @param current_mouse_pack Mouse packet with movement data.
 */
void move_cursor(unsigned h_res, unsigned v_res, struct packet current_mouse_pack) {
  position_t *cursor_pos = get_sprite_pos(cursor_sprite);

  // Update cursor position
  cursor_pos->x += current_mouse_pack.delta_x;
  cursor_pos->y -= current_mouse_pack.delta_y;

  // Cursor boundary checks
  uint16_t c_width = get_sprite_width(cursor_sprite);
  uint16_t c_height = get_sprite_height(cursor_sprite);
  if (cursor_pos->x < 0)
    cursor_pos->x = 0;
  if (cursor_pos->y < 0)
    cursor_pos->y = 0;
  if ((unsigned) cursor_pos->x + c_width > h_res)
    cursor_pos->x = h_res - c_width;
  if ((unsigned) cursor_pos->y + c_height > v_res)
    cursor_pos->y = v_res - c_height;
}

/**
 * @brief Sets the game state to START_GAME.
 */
void start_game() {
  state = START_GAME;
}

/**
 * @brief Sets the game state to CONTINUE_GAME.
 */
void continue_game() {
  state = CONTINUE_GAME;
}

/**
 * @brief Sets the game state to EXIT.
 */
void exit_app() {
  state = EXIT;
}

/**
 * @brief Sets the game state to GO_TO_MAIN_MENU.
 */
void main_menu() {
  state = GO_TO_MAIN_MENU;
}

/**
 * @brief Saves the player's score and name to the leaderboard and transitions to the leaderboard state.
 */
void leaderboard_save() {
  char* str = get_player_name(page);
  const char* name = (str && str[0] != '\0') ? str : "UNKNOWN";
  main_leaderboard(get_game_score(game), name);
  state = GO_TO_LEADERBOARD;
}

/**
 * @brief Enables writing mode for the leaderboard name entry and updates the cursor position.
 */
void leaderboard_edit() {
  set_page_writing(page, true);
  set_page_opt(page, -1);
  unsigned int len = get_name_lenght(page) > 0 ? get_name_lenght(page) - 1 : 0;
  set_name_lenght(page, len);

  text_t *text = get_page_text(page);
  sprite_t *txt = get_text_sprite(text);
  uint16_t char_width = get_sprite_width(txt);

  position_t *cur_pos = get_page_cur_pos(page);
  position_t *pos = get_sprite_pos(cursor_sprite);
  pos->x = cur_pos->x + char_width * 1.5 * len;
  pos->y = cur_pos->y;
  set_sprite_curr(cursor_sprite, 1);
}

/**
 * @brief Main event loop for the game.
 *
 * Initializes graphics mode, sets up menus, event queue, and cursor.
 * Handles hardware interrupts, state transitions, event processing, and drawing.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return 0 on success, non-zero on error.
 */
int(proj_main_loop)(int argc, char *argv[]) {
  // Set Graphics Mode
  vg_init(0x105);

  // Dimensions of screen
  unsigned h_res = get_hres();
  unsigned v_res = get_vres();

  printf("\x1b[32mResolution: %ux%u\x1b[0m\n", h_res, v_res);

  //------ Start Application ------
  void (*functions[])(void) = {
    start_game,
    exit_app};

  const char *title_str = "Eye for an eye";
  const char *options[] = {"Start", "Exit"};
  unsigned int size = 2;

  menu = create_menu(title_str, options, functions, size, h_res, v_res);
  if (!menu) {
    printf("\x1b[0;31mError creating menu.\x1b[0m\n");
    vg_exit();
    return 1;
  }
  state = MAIN_MENU;

  // Create an event queue
  q = new_queue(10, sizeof(event_t));
  if (!q) {
    destroy_menu(menu);
    printf("\x1b[0;31mFailed to create queue\x1b[0m\n");
    vg_exit();
    return 1;
  }

  /***********************/
  /******* CURSOR ********/
  /***********************/
  xpm_map_t cursor_arr[3] = {cursor_xpm, cursor_xpm_2, cursor_xpm_3};
  cursor_sprite = create_sprite(cursor_arr, 3, h_res / 4, v_res / 4);
  if (cursor_sprite == NULL) {
    destroy_menu(menu);
    delete_queue(q);
    destroy_game(game);
    printf("\x1b[0;31mError creating cursor sprite\x1b[0m\n");
    vg_exit();
    return 1;
  }

  extern int num_byte; // num_byte from mouse.c

  /* Interruptions */
  int ipc_status;
  message msg;
  uint8_t bit_no_timer = 2;
  int irq_set_t = BIT(bit_no_timer);
  if (timer_subscribe_int(&bit_no_timer) != 0) {
    printf("\x1b[0;31mError subscribing timer\x1b[0m\n"); // Corrected typo
    return 1;
  }

  // Keyboard
  uint8_t bit_no_kbd = 5;
  int irq_set_k = BIT(bit_no_kbd);
  struct key k;
  k.size = 0;
  k.bytes[0] = 0;
  k.bytes[1] = 0;
  uint8_t current_scancode_byte = 0;
  if (kbd_subscribe_int(&bit_no_kbd) != 0) {
    cleanup();
    printf("\x1b[0;31mError subscribing keyboard\x1b[0m\n");
    return 1;
  }

  // Mouse
  uint8_t bit_no_mouse = 3;
  unsigned int prev_btn = 0;
  if (mouse_nable_data_reporting() != 0) {
    cleanup();
    printf("\x1b[0;31mError failed to enable mouse data reporting.\x1b[0m\n");
    return 1;
  }
  int irq_set_m = BIT(bit_no_mouse);
  if (mouse_subscribe_int(&bit_no_mouse)) {
    cleanup();
    printf("\x1b[0;31mError subscribing mouse.\x1b[0m\n");
    return 1;
  }

  uint8_t kbd_scancode_bytes[2];
  int kbd_scancode_idx = 0;

  while (1) {
    int r = driver_receive(ANY, &msg, &ipc_status);

    // CHECK STATES ========================================
    if (state == START_GAME) {
      if (menu) {
        destroy_menu(menu);
        menu = NULL;
      }
      if (game) {
        destroy_game(game);
        game = NULL;
      }
      if (page) {
        destroy_write_name_page(page);
        page = NULL;
      }
      // Create game
      game = create_game(v_res, h_res);
      if (!game) {
        cleanup();
        printf("\x1b[0;31mFailed to create game\x1b[0m\n");
        return 1;
      }
      state = IN_GAME;
    }
    else if (state == CONTINUE_GAME) {
      state = IN_GAME;
    }
    else if (state == GO_TO_PAUSE_MENU) {
      if (menu) {
        destroy_menu(menu);
        menu = NULL;
      }
      void (*functions[])(void) = {continue_game, start_game, exit_app};

      const char *title_str = "Eye for an eye";
      const char *options[] = {"Resume", "New", "Exit"};
      unsigned int size = 3;

      menu = create_menu(title_str, options, functions, size, h_res, v_res);
      if (!menu) {
        printf("\x1b[0;31mError creating menu.\x1b[0m\n");
        cleanup();
        return 1;
      }
      state = PAUSE_MENU;
    }
    else if (state == GAME_OVER) {
      void (*functions[])(void) = {leaderboard_save, leaderboard_edit, start_game, exit_app};

      const char *title_str = "Game Over";
      const char *cue_str = "Write Your name";
      const char *options[] = {"Save", "Edit", "New", "Exit"};
      unsigned int size = 4;
      page = create_write_name_page(title_str, cue_str, options, functions, size, h_res, v_res);
      state = WRITE_NAME;
      // Set cursor properly!
      if (is_writing(page)) {
        position_t *cur_pos = get_page_cur_pos(page);
        position_t *pos = get_sprite_pos(cursor_sprite);
        pos->x = cur_pos->x;
        pos->y = cur_pos->y;
        set_sprite_curr(cursor_sprite, 1);
      }
    }
    else if (state == GO_TO_LEADERBOARD) {
      if (game) {
        destroy_game(game);
        game = NULL;
      }
      if (menu) {
        destroy_menu(menu);
        menu = NULL;
      }
      lb_page = create_lb_page(h_res, v_res, main_menu);
      if (!lb_page) {
        printf("\x1b[0;31mError creating leaderboard.\x1b[0m\n");
        cleanup();
        return 1;
      }
      state = LEADERBOARD;
    }
    else if (state == GO_TO_MAIN_MENU) {
      if (lb_page) {
        destroy_lb_page(lb_page);
        lb_page = NULL;
      }
      if (menu) {
        destroy_menu(menu);
        menu = NULL;
      }
      void (*functions[])(void) = {
        start_game,
        exit_app};

      const char *title_str = "Eye for an eye";
      const char *options[] = {"Start", "Exit"};
      unsigned int size = 2;

      menu = create_menu(title_str, options, functions, size, h_res, v_res);
      if (!menu) {
        printf("\x1b[0;31mError creating menu.\x1b[0m\n");
        vg_exit();
        return 1;
      }
      state = MAIN_MENU;
    }
    else if (state == EXIT) {
      break;
    }
    //========================================================

    if (r != 0) {
      printf("\x1b[34mdriver_receive failed with: %d\n\x1b[0m", r);
      continue;
    }

    // receive notifications
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          // TIMER
          if (msg.m_notify.interrupts & irq_set_t) {
            timer_int_handler(); // Increment timer counter

            if (state == IN_GAME) {
              if (!game) {
                printf("\x1b[0;31mIn_Game but no game found\x1b[0m\n");
                cleanup();
                return 1;
              }
              if (timer_update_game(game, h_res, v_res, counter)) {
                destroy_game(game);
                cleanup();
                return 1;
              }
            }

            // --- EVENT HANDLING LOGIC ---
            while (is_not_empty_queue(q)) {
              event_t dequeued_event;
              if (get_queue(q, &dequeued_event) == 0) { // Check return value of get_queue
                if (state == IN_GAME) {
                  if (!game) {
                    printf("\x1b[0;31mIn_Game but no game found\x1b[0m\n");
                    cleanup();
                    return 1;
                  }
                  handle_event_game(&dequeued_event, game, cursor_sprite);
                }
                else if (state == MAIN_MENU || state == PAUSE_MENU) {
                  if (!menu) {
                    printf("\x1b[0;31mIn menu but no menu found\x1b[0m\n");
                    cleanup();
                    return 1;
                  }
                  handle_event_menu(&dequeued_event, menu, cursor_sprite);
                }
                else if (state == WRITE_NAME) {
                  if (!page) {
                    printf("\x1b[0;31mIn page but no page found\x1b[0m\n");
                    cleanup();
                    return 1;
                  }
                  handle_event_page(&dequeued_event, page, cursor_sprite);
                }
                else if (state == LEADERBOARD) {
                  if (!lb_page) {
                    printf("\x1b[0;31mIn leaderboard but no leaderboard found\x1b[0m\n");
                    cleanup();
                    return 1;
                  }
                  handle_event_lb(&dequeued_event, lb_page, cursor_sprite);
                }

                if (dequeued_event.k) {
                  free(dequeued_event.k);
                }
                if (dequeued_event.pack) {
                  free(dequeued_event.pack);
                }
              }
              else {
                break;
              }
            }
            // --- END EVENT HANDLING LOGIC ---

            if (state == IN_GAME) {
              if (!game) {
                printf("In_Game but no game found\n");
                cleanup();
                return 1;
              }
              if (is_empty_queue(q)) { // Nothing Pressed/Moved
                game_idle(game);
              }

              timer_update_post_event_game(game);
            }

            // --- DRAWING PHASE ---
            vg_fill(0);

            switch (state) {
              case IN_GAME:
                if (draw_game(game)) {
                  printf("\x1b[0;31mError drawing game\x1b[0m\n");
                  cleanup();
                  return 1;
                }
                break;
              case MAIN_MENU:
                if (draw_menu(menu)) {
                  printf("\x1b[0;31mError drawing menu\x1b[0m\n");
                  cleanup();
                  return 1;
                }
                break;
              case PAUSE_MENU:
                if (draw_menu(menu)) {
                  printf("\x1b[0;31mError drawing pause menu\x1b[0m\n");
                  cleanup();
                  return 1;
                }
                break;
              case LEADERBOARD:
                if (draw_leaderboard(lb_page)) {
                  printf("\x1b[0;31mError drawing lb name page\x1b[0m\n");
                  cleanup();
                  return 1;
                }
                break;
              case WRITE_NAME:
                if (draw_write_name_page(page)) {
                  printf("\x1b[0;31mError drawing write name page\x1b[0m\n");
                  cleanup();
                  return 1;
                }
                break;
              default:
                break;
            }

            if (draw_sprite(cursor_sprite)) {
              printf("\x1b[0;31mError drawing cursor\x1b[0m\n");
              return 1;
            }
            if (page) {
              if (is_writing(page)) {
                unsigned int curr = get_sprite_curr(cursor_sprite);
                anim_delay++;
                if (anim_delay >= ANIM_DELAY) {
                  if (curr == 1) {
                    curr = 2;
                  }
                  else if (curr == 2) {
                    curr = 1;
                  }

                  anim_delay = 0;
                }
                set_sprite_curr(cursor_sprite, curr);
              }
            }
            vg_copy_buffer();
          } // End of Timer interrupt

          // KEYBOARD INTERRUPT
          if (msg.m_notify.interrupts & irq_set_k) {
            kbd_int_handler(); // Read scancode byte
            uint8_t read_byte = get_scan_code();
            current_scancode_byte = read_byte;

            if (read_byte == 0)
              continue;

            if (kbd_scancode_idx == 0) {
              kbd_scancode_bytes[0] = read_byte;
              if (read_byte == FIRST_BYTE_SCANCODE) {
                kbd_scancode_idx = 1;
              }
              else {
                k.bytes[0] = kbd_scancode_bytes[0];
                k.bytes[1] = 0;
                k.size = 1;
                kbd_scancode_idx = 0;

                event_t *e = new_keyboard_event(&k);
                if (e) {
                  if (put_queue(q, e) != 0) {
                    if (e->k) {
                      free(e->k);
                      free(e);
                    }
                  }
                }
              }
            }
            else {
              kbd_scancode_bytes[1] = read_byte;
              k.bytes[0] = kbd_scancode_bytes[0];
              k.bytes[1] = kbd_scancode_bytes[1];
              k.size = 2;
              kbd_scancode_idx = 0;

              // Qeueu Keyboard Event
              event_t *e = new_keyboard_event(&k);
              if (e) {
                if (put_queue(q, e) != 0) {
                  if (e->k) {
                    free(e->k);
                    free(e);
                  }
                }
              }
            }
          }

          // MOUSE INTERRUPT
          if (msg.m_notify.interrupts & irq_set_m) {
            mouse_ih();
            if (num_byte == 3) {
              struct packet current_mouse_pack = get_mouse_packet();
              num_byte = 0; // Reset for next packet

              if (page) {
                if (!is_writing(page)) {
                  move_cursor(h_res, v_res, current_mouse_pack);
                }
              }
              else {
                move_cursor(h_res, v_res, current_mouse_pack);
              }

              // Queue mouse event (if needed for other things)
              event_t *e = new_mouse_event(&current_mouse_pack, prev_btn);
              if (e) {
                if (put_queue(q, e) != 0) {
                  if (e->pack) {
                    free(e->pack);
                    free(e);
                  }
                }
              }

              // Set new previous
              if (current_mouse_pack.lb) {
                prev_btn = 2;
              }
              else if (current_mouse_pack.rb) {
                prev_btn = 1;
              }
              else if (current_mouse_pack.mb) {
                prev_btn = 3;
              }
              else {
                prev_btn = 0;
              }
            }
          } // End Mouse Interrupt
          break; // End of HARDWARE case
        default:
          break;
      }
    }
  } // End of while loop

  if (cleanup()) {
    printf("\x1b[0;31mProgram execution Failed\x1b[0m\n");
    return 1;
  }

  printf("\x1b[32mProgram executed with success!\x1b[0m");
  return 0;
}
