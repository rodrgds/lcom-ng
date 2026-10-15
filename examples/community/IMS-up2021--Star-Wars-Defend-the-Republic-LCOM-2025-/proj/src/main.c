#include "manager.h"
#include "gameState.h"
#include <lcom/lcf.h>

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/grupo_2leic18_2/proj/src/trace.txt");

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

/**
 * @brief Main project loop and entry point after LCF initialization.
 *
 * This function orchestrates the primary lifecycle of the application/game.
 * It first attempts to initialize all necessary graphics, input devices, and game assets
 * by calling `initialize_graphics()`.
 * If initialization is successful, it enters the main `gameLoop()`, which is expected
 * to handle game logic, rendering, and event processing until the game is meant to end.
 * Finally, regardless of how `gameLoop()` exits (normally or due to a quit condition),
 * `exit_game()` is called to perform cleanup operations, such as unsubscribing interrupts
 * and returning to text mode.
 *
 * @param argc The number of command-line arguments passed to the program.
 *             (Standard C main function argument, may not be used directly in this specific loop).
 * @param argv An array of strings representing the command-line arguments.
 *             (Standard C main function argument, may not be used directly in this specific loop).
 * @return int 0 if the game initializes, runs, and exits cleanly.
 *             1 if graphics initialization fails, preventing the game loop from starting.
 */
int(proj_main_loop)(int argc, char *argv[]) {
  // Attempt to initialize graphics, input devices, and game assets
  if (initialize_graphics() != 0) {
    printf("Failed to initialize graphics!\n");
    return 1; // Return error if initialization fails
  }

  // Enter the main game loop if initialization was successful
  // gameLoop() is expected to handle the game's runtime logic and event processing.
  gameLoop(); // Assuming gameLoop() is defined elsewhere and handles the main game execution.

  // Perform cleanup operations after the game loop finishes
  // This ensures resources are freed and the system is restored to a clean state.
  exit_game(); 
  
  return 0; // Return success
}
