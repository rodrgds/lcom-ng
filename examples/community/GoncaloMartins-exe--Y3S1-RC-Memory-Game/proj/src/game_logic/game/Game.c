#include "Game.h"

// Initializes the game: player, board, cards, and their fruits
void game_init(Game* game) {
  srand(time(NULL));  //Seed the random generator with the current time

  player_init(&game->player); // Initialize player position
  board_init(&game->board, BOARD_WIDTH, BOARD_HEIGHT); // Initialize game board with dimensions

  const int numPairs = 8;
  int totalCards = numPairs * 2;
  Fruta fruitPairs[totalCards];

  // Fill fruitPairs with each fruit twice
  for (int i = 0; i < numPairs; i++) {
    fruitPairs[i * 2] = (Fruta)i;
    fruitPairs[i * 2 + 1] = (Fruta)i;
  }

  // Shuffle the fruit pairs using Fisher-Yates algorithm
  for (int i = totalCards - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    Fruta temp = fruitPairs[i];
    fruitPairs[i] = fruitPairs[j];
    fruitPairs[j] = temp;
  }

 // Distribute the shuffled fruits onto the board's cards with their positions
  int fruitIndex = 0;
  for (int i = 0; i < game->board.height; i++) {
    for (int j = 0; j < game->board.width; j++) {
      Card* card = &game->board.cards[i][j];
      card_init(card, fruitPairs[fruitIndex++], 50 + (280 * j), 10 + (210 * i));
    }
  }
}

// Updates all currently animating cards on the board
void board_update_animations(Board* board) {
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      Card* card = &board->cards[i][j];
      if (card->isAnimating) {
        animation_update(card);  // Progress the card's animation state
      }
    }
  }
}

// Handles keyboard input for player movement and actions
void input_handler(Game* game){
  switch (last_keycode){
  case 0x48: // UP_ARROW
    player_move_up(&game->player);
    break;
  case 0x50: // DOWN_ARROW
    player_move_down(&game->player);
    break;
  case 0x4B: // LEFT_ARROW
    player_move_left(&game->player);
    break;
  case 0x4D: // RIGHT_ARROW
    player_move_right(&game->player);
    break;
  case 0x9C:  // ENTER key
    // Only allow flipping if no animations are in progress
    if (check_game_status(game) == 0) {
      Card* selected = &game->board.cards[game->player.yPos][game->player.xPos];
      if (!selected->isAnimating && !selected->isMatched && !selected->isFaceUp) {
        animation_start(selected, 34, true); // flip face up
      }
    }
    break;
  case 0x01: // ESC
    current_screen = SCREEN_MENU;
    printf("ESC pressed: Returning to main menu from game screen\n");
    break;
  
  default:
    return;
  }
}

// Checks current game state: ongoing animations or matched cards
int check_game_status(Game* game){
  int animCounter = 0;
  int faceUpCounter = 0;
  Card* cards[2];   // To hold face-up cards for comparison
  
  // Count animations and collect non-matched, face-up cards
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      Card* card = &game->board.cards[i][j];
      if (card->isAnimating) {
        animCounter++;
      } else if (card->isFaceUp && !card->isMatched) {
        if (faceUpCounter < 2) {
          cards[faceUpCounter++] = card;
        }
      }
    }
  }
  
  // If any animation is running, don't proceed
  if (animCounter > 0) return 1;
  
  // If two cards are face-up, check for a match
  if (faceUpCounter == 2) {
    if (cards[0]->fruit != cards[1]->fruit) {
      animation_start(cards[0], 34, false);
      animation_start(cards[1], 34, false);
    } else {
      // If matched, mark them as such
      cards[0]->isMatched = true;
      cards[1]->isMatched = true;
    }
    return 1;
  }

  return 0;  // No actions taken
}

// Checks whether all cards have been matched (win condition)
bool game_is_finished(Game* game) {
  for (int i = 0; i < game->board.height; i++) {
    for (int j = 0; j < game->board.width; j++) {
      if (!game->board.cards[i][j].isMatched) return false; // At least one unmatched card
    }
  }
  return true; // All cards matched
}

// Main loop handling game flow, input, rendering, and victory screen
void game_run(Game* game, uint8_t* kbd_irq){
  int ipc_status;
  message msg;

  timer_running = true;   // Start timer
  counter = 0;             // Reset frame counter
  
  game_init(game);         // Set up game state
  
  // Run the game loop until ESC key is pressed
  while (last_keycode != BREAK_ESC){
    last_keycode = 0;
    if (driver_receive(ANY, &msg, &ipc_status) == 0) {
      if (is_ipc_notify(ipc_status)) {
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:
          // Timer interrupt: advance game state and render
          if (msg.m_notify.interrupts & BIT(0)) {
            check_game_status(game);
            board_update_animations(&game->board);
            board_draw(&game->board, &game->player);
          
            desenhar_timer(counter / 60, 10, 10); // Display time in seconds
            counter++;
            swap_buffers();  // Refresh frame
            
            // If the game is complete, stop timer and show win screen
            if (game_is_finished(game)) {
              timer_running = false;
          
              desenhar_parabens(counter / 60); // Show congratulations with time
              int ipc_status_parabens;
              message msg_parabens;
              
              // Wait for ESC key to exit win screen
              while (true) {
                if (driver_receive(ANY, &msg_parabens, &ipc_status_parabens) == 0) {
                  if (is_ipc_notify(ipc_status_parabens)) {
                    if (_ENDPOINT_P(msg_parabens.m_source) == HARDWARE) {
                      if (msg_parabens.m_notify.interrupts & *kbd_irq) {
                        kbd_ih();
                        if (last_keycode == BREAK_ESC) {
                          current_screen = SCREEN_MENU;
                          return;
                        }
                      }
                    }
                  }
                }
              }

            }
          }
          
           // Keyboard interrupt: handle player input
          if (msg.m_notify.interrupts & *kbd_irq) {
            kbd_ih();
            input_handler(game);
          }

          break;
        }
      }
    }
  }
  timer_running = false;  // Stop the timer when loop ends
}
