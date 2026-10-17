#include "event_handler.h"
#include "devices/graphics/graphics.h"
#include "devices/uart/uart.h"
#include "game/sprite.h"

bool isDay; /** @brief indicates if the game is in day mode or night mode */

State(handle_timer)(State state, rtc_time *timeRTC) {
  isDay = timeRTC->hours >= 7 && timeRTC->hours < 19;
  switch (state) {
    case GAME:
      updateTimes();
      updateTargets();
      updateDynamite();
      draw_game(isDay);
      if (endTime()) {
        ser_reset_queues();
        return GAMEOVER;
      }
      break;
    case MENU:
      if (readTime(timeRTC) != 0) {
        printf("Error in checkTime inside: %s\n", __func__);
        return ENDGAME;
      }
      draw_menu(isDay);
      break;
    case ENDGAME:
      return ENDGAME;
    case GAMEOVER:
      draw_gameover(isDay);
      return GAMEOVER;
    case WAIT:
      if (ser_get_player2_ready()) {
        ser_set_player2_ready(false);
        initGame(true);
        return GAME;
      }
      else {
        draw_wait_menu(isDay);
        return WAIT;
      }
    default:
      break;
  }
  return state;
}

State(handle_keyboard)(State state, uint8_t *keyboardBytes) {
  switch (state) {
    case GAME:
      if (keyboardBytes[0] == 0x81) {
        if (isMultiplayer()) {
          ser_reset_queues();
        }
        return MENU;
      }
      break;
    case MENU:
      if (keyboardBytes[0] == 0x1C) {
        switch (getCurrentOption()) {
          case SINGLEPLAYER:
            initGame(false);
            return GAME;
          case MULTIPLAYER:
            ser_send_waiting_to_txqueue();
            return WAIT;
          case QUIT:
            return ENDGAME;
          default:
            break;
        }
      }
      if (keyboardBytes[0] == 0xE0 && keyboardBytes[1] == 0x50) {
        increaseCurrentOption();
      }
      if (keyboardBytes[0] == 0xE0 && keyboardBytes[1] == 0x48) {
        decreaseCurrentOption();
      }
      return MENU;
    case GAMEOVER:
      if (keyboardBytes[0] == 0x81) {
        if (isMultiplayer()) {
          ser_reset_queues();
        }
        return MENU;
      }
      return GAMEOVER;

    case WAIT:
      if (keyboardBytes[0] == 0x81) {
        ser_reset_queues();
        return MENU;
      }
      break;
    default:
      break;
  }
  return state;
}

State(handle_mouse)(State state, struct mousePacket *pp) {
  int8_t target_index = -1;
  switch (state) {
    case GAME:
      addToX(getPlayer1(), pp->delta_x);
      addToY(getPlayer1(), pp->delta_y);

      if (pp->lb) {
        if (getPlayerCanShoot(getPlayer1())) {
          target_index = checkAllCollisions(getPlayer1());

          setPlayerCanShoot(getPlayer1(), false);
        }
      }
      else {
        setPlayerCanShoot(getPlayer1(), true);
      }

      if (pp->delta_scroll > 1 && !isMultiplayer()) {
        setSlowTime();
      }

      if (isMultiplayer()) {
        ser_send_player2_info_to_txqueue(getPlayerX(getPlayer1()), getPlayerY(getPlayer1()), target_index, getPlayer1()->score);
      }
      return GAME;
    case MENU:
      return MENU;
    case ENDGAME:
      return ENDGAME;
    case GAMEOVER:
      return GAMEOVER;
    case WAIT:
      return WAIT;
    default:
      break;
  }
  return state;
}

State(handle_serial)(State state) {
  player2_info_t pp;
  switch (state) {
    case WAIT:
      ser_handle_start();
      break;
    case GAME:
      ser_read_data_from_rx_queue();
      if (ser_get_player2_info_is_done()) {
        ser_get_player2_info(&pp);
        if (isMultiplayer()) {
          setPlayerX(getPlayer2(), pp.x);
          setPlayerY(getPlayer2(), pp.y);
          setPlayerScore(getPlayer2(), pp.score);
          if (pp.target != -1) {
            if (isActiveTarget(pp.target)) {
              setActiveTarget(pp.target, false);
            }
          }
        }
      }
      break;
    default:
      break;
  }

  return state;
}
