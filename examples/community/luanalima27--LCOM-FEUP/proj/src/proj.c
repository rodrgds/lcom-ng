/**
 * @file proj.c
 * @brief This file contains the main implementation of the game project.
 */

#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

// Any header files included below this line should have been created by you
#include "alien.h"
#include "bg.h"
#include "buttons.h"
#include "cursor.h"
#include "i8042.h"
#include "i8254.h"
#include "keyboards.h"
#include "letters.h"
#include "mouse.h"
#include "player.h"
#include "projectiles.h"
#include "rtc.h"
#include "score.h"
#include "sprites.h"
#include "time.h"
#include "video.h"

extern int scancode;
extern vbe_mode_info_t vmi_p;
extern int hook_id;
extern int x_mouse;
extern int y_mouse;
extern struct packet packet0;
extern bool error_reading;
extern int counter_mouse;
int selected_option = 1;
extern Projectile *projectiles;
extern int projectile_count;
extern bool cooldown;
extern uint8_t hours;
int mouselock = 0;


/**
 * @brief Enum representing the different game states.
 */
typedef enum { MAINMENU,
               MODEMENU,
               PLAYING,
               QUIT,
               GAMEOVER,
               WIN } GameState;


typedef enum { EASY,
               MEDIUM,
               HARD } Mode;


/**
 * @brief Global variable representing the current game state.
 */
GameState gameState = MAINMENU;
Mode mode = MEDIUM;



/**
 * @brief Main function of the game.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return 0 upon success, non-zero otherwise.
 */
int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}



/**
 * @brief Initializes the game.
 * @return 0 upon success, non-zero otherwise.
 */
int init() {
  uint8_t bit_no = 0;

  if (timer_subscribe_int(&bit_no) != 0) {
    return 1;
  }

  if (kbc_subscribe_int() != 0) {
    return 1;
  }

  if (mouse_subscribe_int() != 0) {
    return 1;
  }

  if (vg_initi(0x115) != 0) {
    return 1;
  }

  return 0;
}



/**
 * @brief Cleans up resources and exits the game.
 * @return 0 upon success, non-zero otherwise.
 */
int leave() {
  if (timer_unsubscribe_int() != 0) {
    return 1;
  }

  if (kbc_unsubscribe_int() != 0) {
    return 1;
  }

  if (mouse_unsubscribe_int() != 0) {
    return 1;
  }

  if (vg_exit() != 0) {
    return 1;
  }

  return 0;
}



/**
 * @brief Renders the main menu screen and handles user input.
 * @param selected_option Pointer to the selected option.
 */
void render_screen(int *selected_option) {
  static bool w_key_handled = false;
  static bool s_key_handled = false;

  switch (scancode) {
    case ENTER:
      if (*selected_option == 1) {
        gameState = MODEMENU;
        if (delete_mmbg(hours)) {
          return;
        }
        if (delete_start(325, 250) != 0) {
          printf("xpm failed to load\n");
          return;
        }
        if (delete_quit(325, 300) != 0) {
          printf("xpm failed to load\n");
          return;
        }
      }
      else if (*selected_option == 0) {
        gameState = QUIT;
        scancode = ESC;
      }
      break;

    case WK:
      if (!w_key_handled) {
        w_key_handled = true;
        if (*selected_option == 1) {
          *selected_option = 0;
          if (draw_start(325, 250) != 0) {
            printf("xpm failed to load\n");
            return;
          }
          if (draw_selected_quit(325, 300) != 0) {
            printf("xpm failed to load\n");
            return;
          }
        }
        else if (*selected_option == 0) {
          *selected_option = 1;
          if (draw_selected_start(325, 250) != 0) {
            printf("xpm failed to load\n");
            return;
          }
          if (draw_quit(325, 300) != 0) {
            printf("xpm failed to load\n");
            return;
          }
        }
      }
      break;

    case SK:
      if (!s_key_handled) {
        s_key_handled = true;
        if (*selected_option == 1) {
          *selected_option = 0;
          if (draw_start(325, 250) != 0) {
            printf("xpm failed to load\n");
            return;
          }
          if (draw_selected_quit(325, 300) != 0) {
            printf("xpm failed to load\n");
            return;
          }
        }
        else if (*selected_option == 0) {
          *selected_option = 1;
          if (draw_selected_start(325, 250) != 0) {
            printf("xpm failed to load\n");
            return;
          }
          if (draw_quit(325, 300) != 0) {
            printf("xpm failed to load\n");
            return;
          }
        }
      }
      break;

    default:
      // Draw the current selection state
      if (*selected_option == 1) {
        draw_mmbg();
        if (draw_selected_start(325, 250) != 0) {
          printf("xpm failed to load\n");
          return;
        }
        if (draw_quit(325, 300) != 0) {
          printf("xpm failed to load\n");
          return;
        }
      }
      else if (*selected_option == 0) {
        // uint8_t * hours = 0;
        draw_mmbg();
        if (draw_start(325, 250) != 0) {
          printf("xpm failed to load\n");
          return;
        }
        if (draw_selected_quit(325, 300) != 0) {
          printf("xpm failed to load\n");
          return;
        }
      }
      break;
  }

  // Handle mouse interactions
  if (x_mouse > 325 && x_mouse < 453) {
    if (y_mouse > 223 && y_mouse < 263) {
      *selected_option = 1;
      if (packet0.lb == true) {
        mouselock = 10;
        gameState = MODEMENU;
        if (delete_mmbg(hours) != 0) {
          return;
        }
        if (delete_start(325, 250) != 0) {
          printf("xpm failed to load\n");
          return;
        }
        if (delete_quit(325, 300) != 0) {
          printf("xpm failed to load\n");
          return;
        }
      }
    }
    else if (y_mouse > 270 && y_mouse < 313) {
      *selected_option = 0;
      if (packet0.lb == true) {
        gameState = QUIT;
        scancode = ESC;
      }
    }
  }

  // Reset scancode and key handled flags
  if (scancode == WK)
    w_key_handled = false;
  if (scancode == SK)
    s_key_handled = false;
  scancode = 0; // Reset scancode after handling it
}


/**
 * @brief Renders the mode selection screen and handles user interaction.
 * @details This function renders the mode selection screen and handles user input
 * to select a game mode. It also transitions to the PLAYING state when a mode
 * is selected, or back to the MAINMENU state if the back button is pressed.
 * @param selected_option Pointer to the selected option index.
 */
void render_mode_screen(int *selected_option) {
    static bool w_key_handled = false;
    static bool s_key_handled = false;

    switch (scancode) {
        case ENTER:
            switch (*selected_option) {
                case 0:
                case 1:
                case 2:
                    printf("Deleting all buttons and background for transition to PLAYING state...\n");
                    if (delete_mmbg(hours) != 0) {
                        printf("Failed to delete background\n");
                        return;
                    }
                    if (delete_easy(325, 250) != 0) {
                        printf("Failed to delete easy button\n");
                        return;
                    }
                    if (delete_medium(325, 300) != 0) {
                        printf("Failed to delete medium button\n");
                        return;
                    }
                    if (delete_hard(325, 350) != 0) {
                        printf("Failed to delete hard button\n");
                        return;
                    }
                    if (delete_back(325, 400) != 0) {
                        printf("Failed to delete back button\n");
                        return;
                    }

                    mode = (*selected_option == 0) ? EASY : 
                           (*selected_option == 1) ? MEDIUM : HARD;
                    gameState = PLAYING;
                    return;
                case 3:

                    gameState = MAINMENU;

                    printf("Deleting all buttons and background for transition to MAINMENU state...\n");
                    if (delete_easy(325, 250) != 0) {
                        printf("Failed to delete easy button\n");
                        return;
                    }
                    if (delete_medium(325, 300) != 0) {
                        printf("Failed to delete medium button\n");
                        return;
                    }
                    if (delete_hard(325, 350) != 0) {
                        printf("Failed to delete hard button\n");
                        return;
                    }
                    if (delete_back(325, 400) != 0) {
                        printf("Failed to delete back button\n");
                        return;
                    }
                    return;
            }
            break;

        case WK:
            if (!w_key_handled) {
                w_key_handled = true;
                *selected_option = (*selected_option - 1) < 0 ? 3 : (*selected_option - 1);
            }
            break;

        case SK:
            if (!s_key_handled) {
                s_key_handled = true;
                *selected_option = (*selected_option + 1) > 3 ? 0 : (*selected_option + 1);
            }
            break;

        default:
            break;
    }

    // Draw buttons based on selected option
    draw_mmbg();
    switch (*selected_option) {
        case 0:
            if (draw_selected_easy(325, 250) != 0 || draw_medium(325, 300) != 0 || 
                draw_hard(325, 350) != 0 || draw_back(325, 400) != 0) {
                printf("xpm failed to load\n");
                return;
            }
            break;
        case 1:
            if (draw_easy(325, 250) != 0 || draw_selected_medium(325, 300) != 0 || 
                draw_hard(325, 350) != 0 || draw_back(325, 400) != 0) {
                printf("xpm failed to load\n");
                return;
            }
            break;
        case 2:
            if (draw_easy(325, 250) != 0 || draw_medium(325, 300) != 0 || 
                draw_selected_hard(325, 350) != 0 || draw_back(325, 400) != 0) {
                printf("xpm failed to load\n");
                return;
            }
            break;
        case 3:
            if (draw_easy(325, 250) != 0 || draw_medium(325, 300) != 0 || 
                draw_hard(325, 350) != 0 || draw_selected_back(325, 400) != 0) {
                printf("xpm failed to load\n");
                return;
            }
            break;
        default:
            break;
    }

    // Handle mouse interactions
    if (x_mouse > 325 && x_mouse < 453) {
        if (y_mouse > 223 && y_mouse < 263) {
            *selected_option = 0;
            if (packet0.lb && mouselock == 0) {
                printf("Deleting all buttons and background for transition to EASY mode...\n");
                if (delete_mmbg(hours) != 0) {
                    printf("Failed to delete background\n");
                    return;
                }
                if (delete_easy(325, 250) != 0) {
                    printf("Failed to delete easy button\n");
                    return;
                }
                if (delete_medium(325, 300) != 0) {
                    printf("Failed to delete medium button\n");
                    return;
                }
                if (delete_hard(325, 350) != 0) {
                    printf("Failed to delete hard button\n");
                    return;
                }
                if (delete_back(325, 400) != 0) {
                    printf("Failed to delete back button\n");
                    return;
                }

                mode = EASY;
                gameState = PLAYING;
                return;
            }
        }
        else if (y_mouse > 270 && y_mouse < 313) {
            *selected_option = 1;
            if (packet0.lb) {
                printf("Deleting all buttons and background for transition to MAINMENU...\n");
                if (delete_mmbg(hours) != 0) {
                    printf("Failed to delete background\n");
                    return;
                }
                if (delete_easy(325, 250) != 0) {
                    printf("Failed to delete easy button\n");
                    return;
                }
                if (delete_medium(325, 300) != 0) {
                    printf("Failed to delete medium button\n");
                    return;
                }
                if (delete_hard(325, 350) != 0) {
                    printf("Failed to delete hard button\n");
                    return;
                }
                if (delete_back(325, 400) != 0) {
                    printf("Failed to delete back button\n");
                    return;
                }
                mode = MEDIUM;
                gameState = PLAYING;
                return;
            }
        }
        else if (y_mouse > 323 && y_mouse < 363) {
            *selected_option = 2;
            if (packet0.lb) {
                printf("Deleting all buttons and background for transition to MAINMENU...\n");
                if (delete_mmbg(hours) != 0) {
                    printf("Failed to delete background\n");
                    return;
                }
                if (delete_easy(325, 250) != 0) {
                    printf("Failed to delete easy button\n");
                    return;
                }
                if (delete_medium(325, 300) != 0) {
                    printf("Failed to delete medium button\n");
                    return;
                }
                if (delete_hard(325, 350) != 0) {
                    printf("Failed to delete hard button\n");
                    return;
                }
                if (delete_back(325, 400) != 0) {
                    printf("Failed to delete back button\n");
                    return;
                }
                mode = HARD;
                gameState = PLAYING;
                return;
            }
        }
        else if (y_mouse > 373 && y_mouse < 413) {
            *selected_option = 3;
            if (packet0.lb) {
                printf("Deleting all buttons and background for transition to MAINMENU...\n");
                if (delete_mmbg(hours) != 0) {
                    printf("Failed to delete background\n");
                    return;
                }
                if (delete_easy(325, 250) != 0) {
                    printf("Failed to delete easy button\n");
                    return;
                }
                if (delete_medium(325, 300) != 0) {
                    printf("Failed to delete medium button\n");
                    return;
                }
                if (delete_hard(325, 350) != 0) {
                    printf("Failed to delete hard button\n");
                    return;
                }
                if (delete_back(325, 400) != 0) {
                    printf("Failed to delete back button\n");
                    return;
                }
                
                gameState = MAINMENU;
                return;
            }
        }
    }

    // Reset scancode and key handled flags
    if (scancode == WK)
        w_key_handled = false;
    if (scancode == SK)
        s_key_handled = false;
    scancode = 0; // Reset scancode after handling it

    // If gameState has changed, immediately handle the new state
    if (gameState == MAINMENU) {
        *selected_option = 1;
        render_screen(selected_option);
    }
}


/**
 * @brief Draws the cursor on top of the game screen based on the current game state.
 * @details This function draws the cursor at the specified coordinates on the game screen
 * based on the current game state. It is called during gameplay to display the cursor
 * or target for the player's interaction.
 */
void draw_cursor_on_top() {
  if (gameState == MAINMENU) {
    if (draw_cursor(x_mouse, y_mouse) != 0) {
      printf("xpm failed to load\n");
    }
  }
  if (gameState == MODEMENU) {
    if (draw_cursor(x_mouse, y_mouse) != 0) {
      printf("xpm failed to load\n");
    }
  }
  if (gameState == PLAYING) {
    if (draw_target(x_mouse, y_mouse) != 0) {
      printf("xpm failed to load\n");
    }
  }
  if (gameState == WIN) {
    if (draw_cursor(x_mouse, y_mouse) != 0) {
      printf("xpm failed to load\n");
    }
  }
  if (gameState == GAMEOVER) {
    if (draw_cursor(x_mouse, y_mouse) != 0) {
      printf("xpm failed to load\n");
    }
  }
}




/**
 * @brief Handles cursor movement based on the current game state.
 * @details This function handles the movement of the cursor based on the current game state.
 * It updates the cursor position and redraws it on the screen accordingly.
 */
void handle_cursor_movement() {
  if (gameState == MAINMENU) {
    if (counter_mouse == 0) {
      if (delete_cursor(x_mouse, y_mouse) != 0) {
        printf("failed to delete cursor\n");
        return;
      }
      move_cursor();
    }
  }
  if (gameState == MODEMENU) {
    if (counter_mouse == 0) {
      if (delete_cursor(x_mouse, y_mouse) != 0) {
        printf("failed to delete cursor\n");
        return;
      }
      move_cursor();
    }
  }
  if (gameState == PLAYING) {
    if (counter_mouse == 0) {
      if (delete_target(x_mouse, y_mouse) != 0) {
        printf("failed to delete cursor\n");
        return;
      }
      move_cursor();
    }
  }
  if (gameState == GAMEOVER) {
    if (counter_mouse == 0) {
      if (delete_cursor(x_mouse, y_mouse) != 0) {
        printf("failed to delete cursor\n");
        return;
      }
      move_cursor();
    }
  }
  if (gameState == WIN) {
    if (counter_mouse == 0) {
      if (delete_cursor(x_mouse, y_mouse) != 0) {
        printf("failed to delete cursor\n");
        return;
      }
      move_cursor();
    }
  }
}



/**
 * @brief Creates projectiles fired by the spaceship.
 * @details This function creates projectiles fired by the spaceship when the left mouse button
 * is pressed. It checks if the game state is PLAYING, if the left mouse button is pressed,
 * if the direction of the projectile is upwards, and if the cooldown for firing is inactive.
 * If all conditions are met, a projectile is created with the specified coordinates, speed,
 * and type (PLAYER_PROJECTILE).
 * @param xi Initial x-coordinate of the projectile.
 * @param yi Initial y-coordinate of the projectile.
 * @param xf Final x-coordinate of the projectile.
 * @param yf Final y-coordinate of the projectile.
 * @param speed Speed of the projectile.
 */
void create_spaceship_projectiles(int xi, int yi, int xf, int yf, int speed) {
  if (gameState == PLAYING) {
    if (packet0.lb == true && yf < yi && !cooldown) {
      cooldown = true;
      packet0.lb = false;
      create_projectile(xi, yi, xf, yf, speed, PLAYER_PROJECTILE);
    }
  }
}



/**
 * @brief Renders the game over screen and handles user interaction.
 * @details This function renders the game over screen and handles user input to select options
 * such as restarting the game or quitting. It transitions to the PLAYING state when
 * the restart option is selected, or to the QUIT state when the quit option is selected.
 * @param selected_option Pointer to the selected option index.
 */
void render_gameover_screen(int *selected_option) {
  static bool w_key_handled = false;
  static bool s_key_handled = false;

  switch (scancode) {
    case ENTER:
      if (*selected_option == 1) {
        gameState = PLAYING;
        if (delete_mmbg(hours)) {
          return;
        }
        if (delete_start(325, 400) != 0) {
          printf("xpm failed to load\n");
          return;
        }
        if (delete_quit(325, 450) != 0) {
          printf("xpm failed to load\n");
          return;
        }
        if (delete_gameover(270, 135) != 0) {
          printf("xpm failed to load\n");
          return;
        }
        init_score(&game_score, 3);
      }
      else if (*selected_option == 0) {
        gameState = QUIT;
        scancode = ESC;
      }
      break;

    case WK:
      if (!w_key_handled) {
        w_key_handled = true;
        if (*selected_option == 1) {
          *selected_option = 0;
          if (draw_start(325, 400) != 0) {
            printf("xpm failed to load\n");
            return;
          }
          if (draw_selected_quit(325, 450) != 0) {
            printf("xpm failed to load\n");
            return;
          }
        }
        else if (*selected_option == 0) {
          *selected_option = 1;
          if (draw_selected_start(325, 400) != 0) {
            printf("xpm failed to load\n");
            return;
          }
          if (draw_quit(325, 450) != 0) {
            printf("xpm failed to load\n");
            return;
          }
        }
      }
      break;

    case SK:
      if (!s_key_handled) {
        s_key_handled = true;
        if (*selected_option == 1) {
          *selected_option = 0;
          if (draw_start(325, 400) != 0) {
            printf("xpm failed to load\n");
            return;
          }
          if (draw_selected_quit(325, 450) != 0) {
            printf("xpm failed to load\n");
            return;
          }
        }
        else if (*selected_option == 0) {
          *selected_option = 1;
          if (draw_selected_start(325, 400) != 0) {
            printf("xpm failed to load\n");
            return;
          }
          if (draw_quit(325, 450) != 0) {
            printf("xpm failed to load\n");
            return;
          }
        }
      }
      break;

    default:
      // Draw the current selection state
      if (*selected_option == 1) {
        draw_mmbg();
        if (draw_game_over(270, 135) != 0) {
          printf("xpm failed to load\n");
        }
        draw_final_score(&game_score);
        draw_best_score(&game_score);
        if (draw_selected_start(325, 400) != 0) {
          printf("xpm failed to load\n");
          return;
        }
        if (draw_quit(325, 450) != 0) {
          printf("xpm failed to load\n");
          return;
        }
      }
      else if (*selected_option == 0) {
        // uint8_t * hours = 0;
        draw_mmbg();
        if (draw_game_over(270, 135) != 0) {
          printf("xpm failed to load\n");
        }
        draw_final_score(&game_score);
        draw_best_score(&game_score);
        if (draw_start(325, 400) != 0) {
          printf("xpm failed to load\n");
          return;
        }
        if (draw_selected_quit(325, 450) != 0) {
          printf("xpm failed to load\n");
          return;
        }
      }
      break;
  }

  // vg_draw_rect(0, 0, vmi_p.XResolution, vmi_p.YResolution, 0);
  //  Handle mouse interactions
  if (x_mouse > 325 && x_mouse < 453) {
    if (y_mouse > 400 && y_mouse < 440) {
      *selected_option = 1;
      if (packet0.lb == true) {
        
        gameState = PLAYING;
        if (delete_mmbg(hours) != 0) {
          return;
        }
        if (delete_start(325, 400) != 0) {
          printf("xpm failed to load\n");
          return;
        }
        if (delete_quit(325, 450) != 0) {
          printf("xpm failed to load\n");
          return;
        }
        init_score(&game_score, 3);
      }
    }
    else if (y_mouse > 450 && y_mouse < 493) {
      *selected_option = 0;
      if (packet0.lb == true) {
        gameState = QUIT;
        scancode = ESC;
      }
    }
  }

  // Reset scancode and key handled flags
  if (scancode == WK)
    w_key_handled = false;
  if (scancode == SK)
    s_key_handled = false;
  scancode = 0; // Reset scancode after handling it
}



/**
 * @brief Renders the win screen and handles user interaction.
 * @details This function renders the win screen and handles user input to select options
 * such as restarting the game or quitting. It transitions to the PLAYING state when
 * the restart option is selected, or to the QUIT state when the quit option is selected.
 * @param selected_option Pointer to the selected option index.
 */
void render_win_screen(int *selected_option) {
  static bool w_key_handled = false;
  static bool s_key_handled = false;

  switch (scancode) {
    case ENTER:
      if (*selected_option == 1) {
        gameState = PLAYING;
        if (delete_mmbg(hours)) {
          return;
        }
        if (delete_start(325, 400) != 0) {
          printf("xpm failed to load\n");
          return;
        }
        if (delete_quit(325, 450) != 0) {
          printf("xpm failed to load\n");
          return;
        }
        if (delete_win(270, 135) != 0) {
          printf("xpm failed to load\n");
          return;
        }
        init_score(&game_score, 3);
      }
      else if (*selected_option == 0) {
        gameState = QUIT;
        scancode = ESC;
      }
      break;

    case WK:
      if (!w_key_handled) {
        w_key_handled = true;
        if (*selected_option == 1) {
          *selected_option = 0;
          if (draw_start(325, 400) != 0) {
            printf("xpm failed to load\n");
            return;
          }
          if (draw_selected_quit(325, 450) != 0) {
            printf("xpm failed to load\n");
            return;
          }
        }
        else if (*selected_option == 0) {
          *selected_option = 1;
          if (draw_selected_start(325, 400) != 0) {
            printf("xpm failed to load\n");
            return;
          }
          if (draw_quit(325, 450) != 0) {
            printf("xpm failed to load\n");
            return;
          }
        }
      }
      break;

    case SK:
      if (!s_key_handled) {
        s_key_handled = true;
        if (*selected_option == 1) {
          *selected_option = 0;
          if (draw_start(325, 400) != 0) {
            printf("xpm failed to load\n");
            return;
          }
          if (draw_selected_quit(325, 450) != 0) {
            printf("xpm failed to load\n");
            return;
          }
        }
        else if (*selected_option == 0) {
          *selected_option = 1;
          if (draw_selected_start(325, 400) != 0) {
            printf("xpm failed to load\n");
            return;
          }
          if (draw_quit(325, 450) != 0) {
            printf("xpm failed to load\n");
            return;
          }
        }
      }
      break;

    default:
      // Draw the current selection state
      if (*selected_option == 1) {
        draw_mmbg();
        if (draw_win(225, 150) != 0) {
          printf("xpm failed to load\n");
        }
        draw_final_score(&game_score);
        draw_best_score(&game_score);
        if (draw_selected_start(325, 400) != 0) {
          printf("xpm failed to load\n");
          return;
        }
        if (draw_quit(325, 450) != 0) {
          printf("xpm failed to load\n");
          return;
        }
      }
      else if (*selected_option == 0) {
        // uint8_t * hours = 0;
        draw_mmbg();
        if (draw_win(225, 150) != 0) {
          printf("xpm failed to load\n");
        }
        draw_final_score(&game_score);
        draw_best_score(&game_score);
        if (draw_start(325, 400) != 0) {
          printf("xpm failed to load\n");
          return;
        }
        if (draw_selected_quit(325, 450) != 0) {
          printf("xpm failed to load\n");
          return;
        }
      }
      break;
  }

  // vg_draw_rect(0, 0, vmi_p.XResolution, vmi_p.YResolution, 0);
  //  Handle mouse interactions
  if (x_mouse > 325 && x_mouse < 453) {
    if (y_mouse > 400 && y_mouse < 440) {
      *selected_option = 1;
      if (packet0.lb == true && mouselock == 0) {
        gameState = PLAYING;
        if (delete_mmbg(hours) != 0) {
          return;
        }
        if (delete_start(325, 400) != 0) {
          printf("xpm failed to load\n");
          return;
        }
        if (delete_quit(325, 450) != 0) {
          printf("xpm failed to load\n");
          return;
        }
        init_score(&game_score, 3);
      }
    }
    else if (y_mouse > 450 && y_mouse < 493) {
      *selected_option = 0;
      if (packet0.lb == true) {
        gameState = QUIT;
        scancode = ESC;
      }
    }
  }

  // Reset scancode and key handled flags
  if (scancode == WK)
    w_key_handled = false;
  if (scancode == SK)
    s_key_handled = false;
  scancode = 0; // Reset scancode after handling it
}



/**
 * @brief Main loop of the project, responsible for handling game states, user input, and rendering.
 * @details This function serves as the main loop of the project, where the game states are managed,
 * user input is handled, and the rendering of the game screen is performed. It includes
 * sub-loops for each game state (MAINMENU, MODEMENU, PLAYING, GAMEOVER, WIN), each of
 * which processes different types of user input and performs specific actions accordingly.
 * The function also utilizes interrupt handlers to respond to mouse, keyboard, and timer
 * events. Depending on the game state and difficulty mode, the function updates the game
 * state, player position, alien movement, projectile creation, and screen rendering. It
 * continuously loops until the user presses the ESC key or the game state transitions to
 * the QUIT state. Upon exiting the loop, it performs cleanup tasks before returning.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return Returns 0 upon successful completion, non-zero otherwise.
 */
int(proj_main_loop)(int argc, char *argv[]) {
  uint8_t bit_no_mouse = IRQ2;
  uint8_t bit_no_kbd = IRQ1;
  uint8_t bit_no_timer = IRQ0;

  uint8_t irq_set_mouse = BIT(bit_no_mouse);
  uint8_t irq_set_kbd = BIT(bit_no_kbd);
  uint8_t irq_set_timer = BIT(bit_no_timer);

  rtc_getTime();

  // initialize timer frequency to 60 Hz
  if (timer_set_frequency(0, 60) != OK) {
    printf("Failed to set timer frequency\n");
    return 1;
  }

  if (init() != 0) {
    printf("Couldn't init\n");
    leave();
    return 1;
  }

  load_xpm();

  // Initialize aliens with screen resolution
  if (vbe_get_mode_info(0x115, &vmi_p) != OK) {
    printf("Failed to get VBE mode info\n");
    return 1;
  }

  init_score(&game_score, 3);
  init_aliens(vmi_p.XResolution, vmi_p.YResolution);

  int ipc_status;
  message msg;
  double r = 0;
  int timer_counter = 0;
  int alien_shoot_timer = 0;

  while (scancode != ESC && gameState != QUIT) {
    while (gameState == MAINMENU && scancode != ESC) {
      if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with: %d", r);
        continue;
      }

      if (is_ipc_notify(ipc_status)) {
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:
            if (msg.m_notify.interrupts & irq_set_mouse) {
              mouse_ih();
              if (error_reading && counter_mouse == 0) {
                error_reading = false;
              }
              else if (error_reading && counter_mouse != 0) {
              }
              else {
                get_mouse_coordinates();
                handle_cursor_movement();
              }
            }
            if (msg.m_notify.interrupts & irq_set_kbd) {
              kbc_ih();
            }
            if (msg.m_notify.interrupts & irq_set_timer) {
              timer_int_handler();
              render_screen(&selected_option);
              draw_cursor_on_top();
              double_buffer();
              
            }
            break;
          default:
            break;
        }
      }
    }

    while (gameState == MODEMENU && scancode != ESC) {
      if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with: %d", r);
        continue;
      }

      if (is_ipc_notify(ipc_status)) {
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:
            if (msg.m_notify.interrupts & irq_set_mouse) {
              mouse_ih();
              if (error_reading && counter_mouse == 0) {
                error_reading = false;
              }
              else if (error_reading && counter_mouse != 0) {
              }
              else {
                get_mouse_coordinates();
                handle_cursor_movement();
              }
            }
            if (msg.m_notify.interrupts & irq_set_kbd) {
              kbc_ih();
            }
            if (msg.m_notify.interrupts & irq_set_timer) {
              timer_int_handler();
              render_mode_screen(&selected_option);
              draw_cursor_on_top();
              double_buffer();
              if (mouselock != 0) {mouselock -= 1;}
            }
            break;
          default:
            break;
        }
      }
    }

    while (gameState == PLAYING && scancode != ESC) {
      if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with: %d", r);
        continue;
      }

      if (is_ipc_notify(ipc_status)) {
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:
            if (msg.m_notify.interrupts & irq_set_mouse) {
              mouse_ih();
              if (error_reading && counter_mouse == 0) {
                error_reading = false;
              }
              else if (error_reading && counter_mouse != 0) {
              }
              else {
                get_mouse_coordinates();
                handle_cursor_movement();
              }
            }
            if (msg.m_notify.interrupts & irq_set_kbd) {
              kbc_ih();
              if (draw_player(player_x, player_y) != 0) {
                printf("xpm failed to load\n");
                return 1;
              }
              switch (scancode) {
                case WK:
                  if (player_y - 15 >= 425) {
                    if (delete_player(player_x, player_y) != 0) {
                      printf("failed to delete player\n");
                      return 1;
                    }
                    player_y -= 15;
                    if (draw_player(player_x, player_y) != 0) {
                      printf("xpm failed to load\n");
                      return 1;
                    }
                  }
                  break;
                case AK:
                  if (player_x - 15 >= 5) {
                    if (delete_player(player_x, player_y) != 0) {
                      printf("failed to delete player\n");
                      return 1;
                    }
                    player_x -= 15;
                    if (draw_player(player_x, player_y) != 0) {
                      printf("xpm failed to load\n");
                      return 1;
                    }
                  }
                  break;
                case DK:
                  if (player_x + 15 <= vmi_p.XResolution - 132) {
                    if (delete_player(player_x, player_y) != 0) {
                      printf("failed to delete player\n");
                      return 1;
                    }
                    player_x += 15;
                    if (draw_player(player_x, player_y) != 0) {
                      printf("xpm failed to load\n");
                      return 1;
                    }
                  }
                  break;
                case SK:
                  if (player_y + 15 < vmi_p.YResolution - 125) {
                    if (delete_player(player_x, player_y) != 0) {
                      printf("failed to delete player\n");
                      return 1;
                    }
                    player_y += 15;
                    if (draw_player(player_x, player_y) != 0) {
                      printf("xpm failed to load\n");
                      return 1;
                    }
                  }
                  break;
                default:
                  break;
              }
            }
            if (msg.m_notify.interrupts & irq_set_timer) {
              if (mode == EASY) {
                timer_int_handler();
                timer_counter++;
                alien_shoot_timer++;

                if (timer_counter % 50 == 0) { // moves aliens after 20 60hz ticks
                  move_aliens();
                }

                if (timer_counter % 60 == 0) {
                  cooldown = false;
                  // timer_counter = 0;
                }

                if (cooldown == true) {
                  draw_Rw(590, 563);
                  draw_lew(606, 564);
                  draw_llw(622, 560);
                  draw_low(638, 564);
                  draw_law(654, 564);
                  draw_ldw(670, 560);
                  draw_liw(686, 560);
                  draw_lnw(702, 564);
                  draw_lgw(718, 564);
                  draw_dotw(734, 572);
                  draw_dotw(750, 572);
                  draw_dotw(768, 572);
                }

                if (cooldown == false) {
                  erasewhiteletters(590, 563);
                  erasewhiteletters(606, 564);
                  erasewhiteletters(622, 560);
                  erasewhiteletters(638, 564);
                  erasewhiteletters(654, 564);
                  erasewhiteletters(670, 560);
                  erasewhiteletters(686, 560);
                  erasewhiteletters(702, 564);
                  erasewhiteletters(718, 564);
                  erasewhiteletters(734, 572);
                  erasewhiteletters(750, 572);
                  erasewhiteletters(768, 572);
                }

                if (alien_shoot_timer >= alien_shoot_interval) { // aliens shoot every 3 seconds, decrease alien_shoot_interval for harder difficulty
                  for (int i = 0; i < ALIEN_COUNT; i++) {
                    if (aliens[i].alive) {
                      create_projectile(aliens[i].x + 30, aliens[i].y + 45, player_x + 30, player_y + 22, 1, ALIEN_PROJECTILE);
                      alien_shoot_timer = 0; // reset the timer after shooting
                      break;
                    }
                  }
                }

                move_projectiles();

                if (game_score.lives == 0) { // end game if player has no lives left
                  gameState = GAMEOVER;
                  break;
                }

                if (check_all_aliens_dead()) {
                  init_aliens(vmi_p.XResolution, vmi_p.YResolution); // respawn aliens after the last one was killed
                }

                if (timer_counter == 3600) {
                  gameState = WIN;
                  game_score.score += 1000 * game_score.lives;
                  break;
                }

                draw_score(&game_score);
                draw_lives(&game_score);
                if (timer_counter % 60 == 0){
                  draw_time_remaining(60 - (timer_counter/60));
                }
                create_spaceship_projectiles(player_x + 56, player_y - 10, x_mouse, y_mouse, 30);
                draw_aliens();
                draw_player(player_x, player_y);
                draw_cursor_on_top();
                double_buffer();
              }
              if (mode == MEDIUM) {
                timer_int_handler();
                timer_counter++;
                alien_shoot_timer++;

                if (timer_counter % 20 == 0) { // moves aliens after 20 60hz ticks
                  move_aliens();
                }

                if (timer_counter % 60 == 0) {
                  cooldown = false;
                  // timer_counter = 0;
                }

                if (cooldown == true) {
                  draw_Rw(590, 563);
                  draw_lew(606, 564);
                  draw_llw(622, 560);
                  draw_low(638, 564);
                  draw_law(654, 564);
                  draw_ldw(670, 560);
                  draw_liw(686, 560);
                  draw_lnw(702, 564);
                  draw_lgw(718, 564);
                  draw_dotw(734, 572);
                  draw_dotw(750, 572);
                  draw_dotw(768, 572);
                }

                if (cooldown == false) {
                  erasewhiteletters(590, 563);
                  erasewhiteletters(606, 564);
                  erasewhiteletters(622, 560);
                  erasewhiteletters(638, 564);
                  erasewhiteletters(654, 564);
                  erasewhiteletters(670, 560);
                  erasewhiteletters(686, 560);
                  erasewhiteletters(702, 564);
                  erasewhiteletters(718, 564);
                  erasewhiteletters(734, 572);
                  erasewhiteletters(750, 572);
                  erasewhiteletters(768, 572);
                }

                if (alien_shoot_timer >= alien_shoot_interval) { // aliens shoot every 3 seconds, decrease alien_shoot_interval for harder difficulty
                  for (int i = 0; i < ALIEN_COUNT; i++) {
                    if (aliens[i].alive) {
                      create_projectile(aliens[i].x + 30, aliens[i].y + 45, player_x + 30, player_y + 22, 3, ALIEN_PROJECTILE);
                      alien_shoot_timer = 0; // reset the timer after shooting
                      break;
                    }
                  }
                }

                move_projectiles();

                if (game_score.lives == 0) { // end game if player has no lives left
                  gameState = GAMEOVER;
                  break;
                }

                if (check_all_aliens_dead()) {
                  init_aliens(vmi_p.XResolution, vmi_p.YResolution); // respawn aliens after the last one was killed
                }

                if (timer_counter == 5400) {
                  gameState = WIN;
                  game_score.score += 1000 * game_score.lives;
                  break;
                }

                draw_score(&game_score);
                draw_lives(&game_score);
                if (timer_counter % 60 == 0){
                  draw_time_remaining(90 - (timer_counter/60));
                }
                create_spaceship_projectiles(player_x + 56, player_y - 10, x_mouse, y_mouse, 20);
                draw_aliens();
                draw_player(player_x, player_y);
                draw_cursor_on_top();
                double_buffer();
              }
              if (mode == HARD) {
                timer_int_handler();
                timer_counter++;
                alien_shoot_timer++;

                if (timer_counter % 5 == 0) { // moves aliens after 20 60hz ticks
                  move_aliens();
                }

                if (timer_counter % 60 == 0) {
                  cooldown = false;
                  // timer_counter = 0;
                }

                if (cooldown == true) {
                  draw_Rw(590, 563);
                  draw_lew(606, 564);
                  draw_llw(622, 560);
                  draw_low(638, 564);
                  draw_law(654, 564);
                  draw_ldw(670, 560);
                  draw_liw(686, 560);
                  draw_lnw(702, 564);
                  draw_lgw(718, 564);
                  draw_dotw(734, 572);
                  draw_dotw(750, 572);
                  draw_dotw(768, 572);
                }

                if (cooldown == false) {
                  erasewhiteletters(590, 563);
                  erasewhiteletters(606, 564);
                  erasewhiteletters(622, 560);
                  erasewhiteletters(638, 564);
                  erasewhiteletters(654, 564);
                  erasewhiteletters(670, 560);
                  erasewhiteletters(686, 560);
                  erasewhiteletters(702, 564);
                  erasewhiteletters(718, 564);
                  erasewhiteletters(734, 572);
                  erasewhiteletters(750, 572);
                  erasewhiteletters(768, 572);
                }

                if (alien_shoot_timer >= alien_shoot_interval) { // aliens shoot every 3 seconds, decrease alien_shoot_interval for harder difficulty
                  for (int i = 0; i < ALIEN_COUNT; i++) {
                    if (aliens[i].alive) {
                      create_projectile(aliens[i].x + 30, aliens[i].y + 45, player_x + 30, player_y + 22, 5, ALIEN_PROJECTILE);
                      alien_shoot_timer = 0; // reset the timer after shooting
                      break;
                    }
                  }
                }

                move_projectiles();

                if (game_score.lives == 0) { // end game if player has no lives left
                  gameState = GAMEOVER;
                  break;
                }

                if (check_all_aliens_dead()) {
                  init_aliens(vmi_p.XResolution, vmi_p.YResolution); // respawn aliens after the last one was killed
                }

                if (timer_counter == 9000) {
                  gameState = WIN;
                  game_score.score += 1000 * game_score.lives;
                  break;
                }

                draw_score(&game_score);
                draw_lives(&game_score);
                if (timer_counter % 60 == 0){
                  draw_time_remaining(150 - (timer_counter/60));
                }
                create_spaceship_projectiles(player_x + 56, player_y - 10, x_mouse, y_mouse, 15);
                draw_aliens();
                draw_player(player_x, player_y);
                draw_cursor_on_top();
                double_buffer();
              }
            }
            break;
          default:
            break;
        }
      }
    }

    while (gameState == GAMEOVER) {
      if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with: %d", r);
        continue;
      }

      if (is_ipc_notify(ipc_status)) {
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:
            if (msg.m_notify.interrupts & irq_set_mouse) {
              mouse_ih();
              if (error_reading && counter_mouse == 0) {
                error_reading = false;
              }
              else if (error_reading && counter_mouse != 0) {
              }
              else {
                get_mouse_coordinates();
                handle_cursor_movement();
              }
            }
            if (msg.m_notify.interrupts & irq_set_kbd) {
              kbc_ih();
              if (scancode == ESC) {
                gameState = QUIT;
              }
            }
            if (msg.m_notify.interrupts & irq_set_timer) {
              timer_int_handler();
              render_gameover_screen(&selected_option);
              draw_cursor_on_top();
              timer_counter = 0;
              init_aliens(vmi_p.XResolution, vmi_p.YResolution);
              double_buffer();
            }
            break;
          default:
            break;
        }
      }
    }

    while (gameState == WIN) {
      if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with: %d", r);
        continue;
      }

      if (is_ipc_notify(ipc_status)) {
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:
            if (msg.m_notify.interrupts & irq_set_mouse) {
              mouse_ih();
              if (error_reading && counter_mouse == 0) {
                error_reading = false;
              }
              else if (error_reading && counter_mouse != 0) {
              }
              else {
                get_mouse_coordinates();
                handle_cursor_movement();
              }
            }
            if (msg.m_notify.interrupts & irq_set_kbd) {
              kbc_ih();
              if (scancode == ESC) {
                gameState = QUIT;
              }
            }
            if (msg.m_notify.interrupts & irq_set_timer) {
              timer_int_handler();
              render_win_screen(&selected_option);
              draw_cursor_on_top();
              timer_counter = 0;
              init_aliens(vmi_p.XResolution, vmi_p.YResolution);
              double_buffer();
            }
            break;
          default:
            break;
        }
      }
    }
  }

  if (leave() != 0) {
    return 1;
  }

  return 0;
}
