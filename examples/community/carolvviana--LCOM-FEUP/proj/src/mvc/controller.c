#include "controller.h"



extern int sign_x;
extern int sign_y;

extern int counter;
extern uint8_t data;
extern uint8_t data_x_ball;
extern uint8_t data_y_ball;

extern struct Player player;
extern struct Player player2;
extern struct Player AIplayer;
extern struct Ball ball;
extern int actionLeftTimeout;

extern uint8_t scancode;
extern struct packet mouse_packet;
extern struct Cursor cursor;

extern int powerupActiveTimer;
extern int powerupCooldownTimer;

extern bool exit_game;

game_state_t game_state = MENU;
struct state game_states_list[] = {
  {MENU, menuController},
  {SINGLE_PLAYER_GAME, singlePlayerController},
  {MULTI_PLAYER_GAME, multiPlayerController},
  {WAITING_CONNECTION, waitingConnectionController},
  {CREDITS, creditsController},
  {GAME_OVER, gameOverController},
  {GAME_WIN, gameWinController},
  {BLOCKED, blockedController},
  {EXIT, NULL},
  {NONE_STATE, NULL},
  {0, NULL}
};

void(menuController)(int device) {

  if (device == TIMER) {
    if (counter % REFRESH_RATE == 0) {
      draw_menu();
      draw_mouse();
      draw_time();
      swap_buffer();
    }
  }

  else if (device == KEYBOARD) {
    if (scancode == UP_KEY) {
      DEBUG_MSG("UP\n");
      upMenuOption();
    }
    else if (scancode == DOWN_KEY) {
      DEBUG_MSG("DOWN\n");
      downMenuOption();
    }
    else if (scancode == ENTER_KEY) {
      DEBUG_MSG("ENTER\n");
      int menuOption = getMenuOption();
      switch (menuOption) {
        case MENU_SINGLEPLAYER_OPTION: {

          /* to check if user has permission to play regarding night time */
          if (rtc_check_permission() != 0) {
            game_state = BLOCKED;
            DEBUG_MSG("BLOCKED\n");
            clear_buffer();
            swap_buffer();
            break;
          }

          game_state = SINGLE_PLAYER_GAME;
          DEBUG_MSG("SINGLE PLAYER\n");
          resetGame();
          clear_buffer();
          swap_buffer();
          break;
        }
        case MENU_MULTIPLAYER_OPTION: {

          /* to check if user has permission to play regarding night time */
          if (rtc_check_permission() != 0) {
            game_state = BLOCKED;
            DEBUG_MSG("BLOCKED\n");
            clear_buffer();
            swap_buffer();
            break;
          }

          init_serialp();
          DEBUG_MSG("Waiting for connection...\n");

          // draw_xpm(images[WAITING], 0, 0); //precisamos de um menu para quando ainda etsás a conectar!!!!!!!!!!!!!!
          game_state = WAITING_CONNECTION;
          resetGame();
          clear_buffer();
          swap_buffer();
          break;
        }
        case MENU_CREDITS_OPTION: {

          game_state = CREDITS;
          clear_buffer();
          swap_buffer();
          break;
        }
      }

    }
    else if (scancode == ESC_KEY) {
      DEBUG_MSG("ESC\n");
      // game_state = EXIT;
      exit_game = true;
      // clear_buffer();
      // swap_buffer();
      DEBUG_MSG(game_state == EXIT ? "EXIT\n" : "NOT EXIT\n");
      return;
    }

  }

  else if(device == MOUSE){
    DEBUG_MSG("MOUSE\n");

      moveCursor(mouse_packet.delta_x, mouse_packet.delta_y);

      if (cursor.x >= 157 && cursor.x <= 643) {
        if (cursor.y >= 322 && cursor.y <= 372) {
          DEBUG_MSG("SINGLE PLAYER\n");
          setMenuOption(MENU_SINGLEPLAYER_OPTION - 1);
        }
        else if (cursor.y >= 391 && cursor.y <= 441) {
          DEBUG_MSG("MULTI PLAYER\n");
          setMenuOption(MENU_MULTIPLAYER_OPTION - 1);
        }
        else if (cursor.y >= 460 && cursor.y <= 510) {
          DEBUG_MSG("CREDITS\n");
          setMenuOption(MENU_CREDITS_OPTION - 1);
        }
      }

      if (mouse_packet.lb) {
        DEBUG_MSG("LB\n");
        switch (getMenuOption()) {
          case MENU_SINGLEPLAYER_OPTION:
            game_state = SINGLE_PLAYER_GAME;
            clear_buffer();
            swap_buffer();
            break;
          case MENU_MULTIPLAYER_OPTION:
            init_serialp();

            game_state = WAITING_CONNECTION;
            clear_buffer();
            swap_buffer();
            break;
          case MENU_CREDITS_OPTION:
            game_state = CREDITS;
            clear_buffer();
            swap_buffer();
            break;
        }
      }

  }
  
}

void(singlePlayerController)(int device) {
  // uint32_t color = 0x004B7D35;
  uint32_t color = 0x00404040;

  if (device == KEYBOARD) {
    if (scancode == UP_KEY) {
      movePlayerUp(&player);
    }
    else if (scancode == DOWN_KEY) {
      movePlayerDown(&player);
    }
    else if (scancode == LEFT_KEY) {
      movePlayerLeft(&player);
    }
    else if (scancode == RIGHT_KEY) {
      movePlayerRight(&player);
    }
    else if (scancode == ESC_KEY) {
      game_state = MENU;
      clear_buffer();
      swap_buffer();
    }
  }
  else if (device == MOUSE) {
    DEBUG_MSG("MOUSE\n");

    moveCursor(mouse_packet.delta_x, mouse_packet.delta_y);

    if (mouse_packet.lb && powerupCooldownTimer == 0) {
      activatePowerup1();
    }
    if (mouse_packet.rb && powerupCooldownTimer == 0) {
      activatePowerup2();
    }
  }
  else if (device == TIMER) {
    if (counter % REFRESH_RATE == 0) {
      moveBall();

      int missingTime = powerupCooldownTimer / (60 / REFRESH_RATE); // 60/REFRESH_RATE = 1 second

      if (powerupCooldownTimer > 0) {
        powerupCooldownTimer--;
      }

      moveAI(&AIplayer);
      draw_background(&color);
      draw_arena();
      draw_scores();
      draw_mouse();
      draw_player(&player);
      draw_player(&AIplayer);

      draw_ball(&ball);
      draw_time();

      if (missingTime == 60) {
        resetPowerup();
      }

      if (missingTime > 60) { // 80-61 secs
        draw_xpm(images[ACTIVATED], 580, 30, 0);
      }
      else if (missingTime > 45) { // 60-46 secs
        draw_xpm(images[COOLDOWN_60], 580, 30, 0);
      }
      else if (missingTime > 30) { // 45-31 secs
        draw_xpm(images[COOLDOWN_45], 580, 30, 0);
      }
      else if (missingTime > 15) { // 30-16 secs
        draw_xpm(images[COOLDOWN_30], 580, 30, 0);
      }
      else if (missingTime > 0) { // 15-1 secs
        draw_xpm(images[COOLDOWN_15], 580, 30, 0);
      }
      else if (missingTime == 0) { // 0  secs
        draw_xpm(images[COOLDOWN_0v3], 580, 30, 0);
      }

      enum Winner winner = checkScore();
      switch (winner) {
        case PLAYER1:
          clear_buffer();
          game_state = GAME_WIN;
          break;
        case PLAYER2:
          clear_buffer();
          game_state = GAME_OVER;
          break;
        default:
          break;
      }

      swap_buffer();
    }
  }
}

void(waitingConnectionController)(int device) {
  int ready;
  if (device == TIMER) {
    if (counter % REFRESH_RATE == 0) {
      draw_xpm(images[WAITING_SCREEN], 0, 0, 0);
      draw_time();
      swap_buffer();
    }
  }
  else if (device == SERIALPORT) {
    if (serialp_connection(&ready) != 0) {
      return;
    }
    game_state = MULTI_PLAYER_GAME;
    if (ready == SERIALP_STARTGAME) {
      DEBUG_MSG("Ready i think \n");
      game_state = MULTI_PLAYER_GAME;
      resetGame();
      switchPlayers();
      switchBall();
    }
  }
  else if (device == KEYBOARD) {
    if (scancode == ESC_KEY) {
      game_state = MENU;
      clear_buffer();
      swap_buffer();
    }
  }
}

void(multiPlayerController)(int device) {
  uint32_t color = 0x00404040;

  if (device == KEYBOARD) {
    switch (scancode) {
      case UP_KEY:
        // uint8_t dataToSend = 0;
        movePlayerUp(&player);
        // isto é necessário porque só conseguimos mandar 8 bits de cada vez -> ver último powerpoint do prof
        send_data_serialp(compressData(player.y_pos, player.y_velocity));
        break;
      case DOWN_KEY:
        // uint8_t dataToSend = 0;
        movePlayerDown(&player);
        // isto é necessário porque só conseguimos mandar 8 bits de cada vez -> ver último powerpoint do prof
        send_data_serialp(compressData(player.y_pos, player.y_velocity));
        break;
      case ESC_KEY:
        resetGame();
        game_state = MENU;
        send_data_serialp(ESC_KEY);
        clear_buffer();
        swap_buffer();
        break;
      default:
        break;
    }
  }
  else if (device == TIMER) {
    if (counter % REFRESH_RATE == 0) {
      moveBall();

      draw_background(&color);

      if(player.x_pos < 400 && player.color == 0xff00ff){
        draw_arena_multiplayer(); // arena com as cores invertidas horizontalmente
      }
      else {
        draw_arena(); // arena com as cores
      } 

      draw_scores();

      draw_ball(&ball);
      draw_player(&player);
      draw_player(&player2);

      draw_time();

      enum Winner winner = checkScore();
      switch (winner) {
        case PLAYER1:
          clear_buffer();
          game_state = GAME_WIN;
          break;
        case PLAYER2:
          clear_buffer();
          game_state = GAME_OVER;
          break;
        default:
          break;
      }

      swap_buffer();
    }
  }
  else if (device == SERIALPORT) {
    // uint8_t data = receive_data_serialp(); // receber dados da serial port
    // uint16_t y_pos = decompressData(player.y_pos, player.y_velocity); //descomprimir dados

    printf("Receiving data: %d\n", (int) data);
    if (data == BIT(7)) { // if goal
      player2.score++;
      reset();
      switchBall();
      return;
    }
    else if (data == BIT(6)) { // if goal against
      player.score++;
      reset();
      switchBall();
      return;
    }
    else if (data == ESC_KEY) {
      resetGame();
      game_state = MENU;
      clear_buffer();
      swap_buffer();
      return;
    }
    else if (data == BIT(0)) { // if lost
      clear_buffer();
      game_state = GAME_OVER;
      swap_buffer();
      return;
    }
    else if (data == BIT(1)) { // if win
      clear_buffer();
      game_state = GAME_WIN;
      swap_buffer();
      return;
    }
   

    int position = (uint8_t) data * player.y_velocity;
    printf("Decompressed data: %d\n", position);
    player2.y_pos = position;
    return;
    


    // tickdelay(micros_to_ticks(20000));

    // player2.y_pos = new_pos; //atualizar posição do player2
  }
}

void(creditsController)(int device) {
  if (device == TIMER) {
    if (counter % REFRESH_RATE == 0) {
      draw_credits();
      draw_mouse();
      draw_time();
      swap_buffer();
    }
  }
  else if (device == KEYBOARD) {
    if (scancode == ESC_KEY) {
      DEBUG_MSG("ESC\n");
      game_state = MENU;
    }
  }
  else if (device == MOUSE) {
    DEBUG_MSG("MOUSE\n");

    moveCursor(mouse_packet.delta_x, mouse_packet.delta_y);
  }
}

void(gameOverController)(int device) {
  if (device == TIMER) {
    if (counter % REFRESH_RATE == 0) {
      draw_game_over();
      draw_time();
      swap_buffer();
    }
  }
  else if (device == KEYBOARD) {
    if (scancode == ESC_KEY) {
      DEBUG_MSG("ESC\n");
      game_state = MENU;
    }
  }
}

void(gameWinController)(int device) {
  if (device == TIMER) {
    if (counter % REFRESH_RATE == 0) {
      draw_game_win();
      draw_time();
      swap_buffer();
    }
  }
  else if (device == KEYBOARD) {
    if (scancode == ESC_KEY) {
      DEBUG_MSG("ESC\n");
      game_state = MENU;
    }
  }
}

void(blockedController)(int device) {
  if (device == TIMER) {
    if (counter % REFRESH_RATE == 0) {


      draw_blocked();

      draw_time();
      draw_mouse();
      swap_buffer();
    }
  }
  else if (device == KEYBOARD) {
    if (scancode == ESC_KEY) {
      DEBUG_MSG("ESC\n");
      game_state = MENU;
      clear_buffer();
      swap_buffer();
    }
  }
  else if (device == MOUSE) {
    DEBUG_MSG("MOUSE\n");

    moveCursor(mouse_packet.delta_x, mouse_packet.delta_y);
  }
}
