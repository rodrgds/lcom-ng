/**
 * @file proj.c
 * @brief Main project file - Minesweeper Game implementation
 * 
 * This file contains the main game loop and state management for a Minesweeper game.
 * It handles interrupts from keyboard, mouse, and timer, manages game states,
 * and coordinates between different views and controllers.
 */

#include <lcom/lcf.h>
#include <stdio.h>

#include "Controller/Devices/macros.h"
#include "Controller/Devices/Timer/timer.h"
#include "Controller/Devices/Mouse_Keyboard/Mouse/mouse.h"
#include "Controller/Devices/Mouse_Keyboard/Keyboard/keyboard.h"
#include "Controller/Devices/VideoCard/video_graphics.h"
#include "Controller/MouseController/mouseController.h"
#include "Controller/MainMenuController/mainMenuController.h"
#include "Model/Mouse/mouseModel.h"
#include "View/MouseView/mouseView.h"
#include "View/MainMenuView/mainMenuView.h"
#include "Controller/GameOverController/gameOverController.h"
#include "Model/SelectorMine/selectorMine.h"
#include "View/GameView/gameView.h"
#include "Controller/GameController/gameController.h"
#include "View/InstructionsView/instructionsView.h"
#include "Controller/state.h"
#include "generalMacros.h"

// External variable declarations
extern bool packet_complete;       // Flag indicating mouse packet completion
extern uint8_t scan_code;          // Current keyboard scan code
extern uint8_t* background;        // Background buffer pointer
extern int game_counter;           // Global game timer counter
extern bool board_cleared;         // Flag indicating if minefield is cleared
extern bool startedExplosionTimer; // Flag for explosion animation timing
bool exploded = false;             // Flag indicating if explosion occurred
int explosion_timer = 0;           // Timer for explosion animation

// Game state variables
int game_time = 0;                 // Current game time in seconds
uint8_t mouse_mask;               // Mouse interrupt mask
uint8_t time_mask;                // Timer interrupt mask  
uint8_t keyboard_mask;            // Keyboard interrupt mask
bool stateChanged = false;        // Flag indicating game state transition
Mouse mouse;                      // Mouse model instance
SMine selector_mine;              // Selector mine sprite for UI navigation
game_state state = MAIN_MEU;      // Current game state


/**
 * @brief Main interrupt handling loop for the game
 * 
 * This function continuously handles interrupts from various devices (mouse, keyboard, timer)
 * and manages the game state transitions. It processes input events, updates game logic,
 * handles animations, and renders the appropriate screens based on the current state.
 * 
 * @return 0 on success, non-zero on error
 */
int (handle_interrupt_loop)() {
  int ipc_status;
  message msg;
  while( state != QUIT) { 
    // Receive interrupt messages
    if( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
      printf("Error");
      continue;
    }
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          // Handle mouse interrupts
          if (msg.m_notify.interrupts & mouse_mask) {
            mouse_ih();
            if (packet_complete) {
              handlePacket(&mouse);
            }
          }
          // Handle timer interrupts
          if (msg.m_notify.interrupts & time_mask) {
            timer_int_handler();
            // Update game time counter during gameplay
            if (state == GAME) {
              if (game_counter % 60 == 0) {
                game_time++;
              }
            }
            // Handle explosion timer for mine detonation animation
            if (state == GAME && startedExplosionTimer) {
              if (game_counter % 60 == 0) {
                explosion_timer++;
                if (explosion_timer >= 1) {
                  exploded = true;
                  startedExplosionTimer = false;
                }
              }
            }
            // Transition to game over state after explosion
            if (exploded) {
              state = GAME_OVER;
              stateChanged = true;
              exploded = false;
            }
            // Handle state transitions and load appropriate resources
            if (stateChanged) {
              stateChanged = false;
              switch (state) {
                case MAIN_MEU:
                  loadMainMenuSprites();
                  setMainMenuBackgorund();
                  break;
                case GAME_OVER:
                  loadGameOverSprites();
                  setGameOverBackgorund();
                  break;
                case GAME:
                  game_time = 0;
                  loadGameSprites();
                  buildGameBoard();
                  drawGameBoardToBackground();
                  drawGameSideBar();
                  break;
                case WIN:
                  // Win state handling (to be implemented)
                  break;                case INST:
                  printf("Inst\n");
                  createInstructionScreen();
                  printf("Inst2\n");
                  setInstructionBackgorund();
                  printf("Inst3\n");
                  break;
                default:
                  break;
              }
            }

            // Render appropriate screen based on current state
            if (state == GAME_OVER) {
              if (draw_background() != 0) return 1;
              drawSMine(selector_mine);
              if (drawMouse(mouse) != 0) return 1;
              if (flip_page() != 0) return 1;
            }
            else if (state == MAIN_MEU) {
              if (draw_background() != 0) return 1;
              drawSMine(selector_mine);
              if (flip_page() != 0) return 1;
            }
            else if (state == GAME) {
              if (draw_background() != 0) return 1;
              if (drawMouse(mouse) != 0) return 1;
              DrawTime(game_time);
              DrawFlagCounter();
              if (flip_page() != 0) return 1;
            }
            else if (state == WIN) {
              // Win screen rendering (to be implemented)
            }
            else if (state == INST) {
              if (draw_background() != 0) return 1;
              if (drawMouse(mouse) != 0) return 1;
              if (flip_page() != 0) return 1;
            }
          }
          // Handle keyboard interrupts
          if (msg.m_notify.interrupts & keyboard_mask) {
            kbc_ih();
            printf("Scancode: 0x%02x\n", scan_code);
            // ESC key - return to main menu from any state
            if (scan_code == ESC_BREAK) {
              state = MAIN_MEU;
              stateChanged = true;
            }
            // State-specific keyboard handling
            else if (state == GAME_OVER) {
              handleScanCodeGameOver(&selector_mine, &state, &stateChanged);
            }
            else if (state == MAIN_MEU) {
              handleScanCodeMainMenu(&selector_mine, &state, &stateChanged);
            }
          }
      }
    }
  }
  return 0;
}

/**
 * @brief Subscribes to all necessary interrupts
 * 
 * This function subscribes to timer, mouse, and keyboard interrupts
 * to enable the game to respond to user input and timing events.
 * 
 * @return 0 on success, non-zero on error
 */
int subscribe_int() {
  if (timer_subscribe_int(&time_mask) != 0) return 1;
  if (sub_mouse_int(&mouse_mask) != 0) return 1;
  if (kb_sub_int(&keyboard_mask) != 0) return 1;
  if (write_mouse_command(ENABLE_DR) != 0) return 1;
  return 0;
}

/**
 * @brief Unsubscribes from all interrupts
 * 
 * This function unsubscribes from timer, mouse, and keyboard interrupts
 * to clean up resources when the game exits.
 * 
 * @return 0 on success, non-zero on error
 */
int unsubscribe_int() {
  if (timer_unsubscribe_int() != 0) return 1;
  if (unsub_mouse_int() != 0) return 1;
  if (kb_unsub_int() != 0) return 1;
  return 0; 
}

/**
 * @brief Initializes video mode and graphics
 * 
 * This function sets up the video graphics system by configuring
 * the display mode, setting up double buffering, and initializing
 * the video system for 800x600 resolution.
 * 
 * @return 0 on success, non-zero on error
 */
int start_video() {
  if (get_mode_conf(DIRECT_800X600_MODE)!= 0) return 1;
  if (set_double_frame_buffer() != 0) return 1;
  if (init_video(DIRECT_800X600_MODE) != 0) return 1;
  return 0;
}

/**
 * @brief Creates and initializes game models
 * 
 * This function initializes the mouse model with its starting position.
 */
void createModels() {
  mouse = createMouse(MOUSE_INI_X, MOUSE_INI_Y);
}

/**
 * @brief Main function - entry point for the program
 * 
 * This is the standard main function that sets up LCF (LCOM Framework)
 * environment and initiates the program execution.
 * 
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @return 0 on success, non-zero on error
 */
int (main)(int argc, char *argv[]) {

  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/g8/proj/trace.txt");
  lcf_log_output("/home/lcom/labs/g8/proj/output.txt");
  if (lcf_start(argc, argv)) return 1;

  lcf_cleanup();

  return 0;
}

/**
 * @brief Main project loop function
 * 
 * This function is the main entry point for the game execution.
 * It initializes all necessary components, sets up the game environment,
 * runs the main game loop, and handles cleanup when the game exits.
 * 
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @return 0 on success, non-zero on error
 */
int (proj_main_loop)(int argc, char **argv) {  
  createModels();                   // Initialize game models
  if(subscribe_int() != 0) return 1; // Subscribe to interrupts
  if(start_video() != 0) return 1;   // Initialize video system
  selector_mine = createSMine();     // Create selector mine sprite
  createBackground();               // Initialize background system
  loadMainMenuSprites();            // Load main menu graphics
  setMainMenuBackgorund();          // Set initial background
  if(handle_interrupt_loop() != 0) return 1; // Run main game loop
  if(vg_exit() != 0) return 1;      // Exit video mode
  if(unsubscribe_int() != 0) return 1; // Unsubscribe from interrupts
  free_buffers();                   // Free allocated memory
  destroyMouse(mouse);              // Clean up mouse model
  return 0;
}

