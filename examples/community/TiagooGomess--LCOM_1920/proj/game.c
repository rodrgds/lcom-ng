#include <lcom/lcf.h>
#include "game.h"
#include "time.h"
#include "mouse.h"
#include "keyboard.h"
#include "RTC.h"
#include "scancodes.h"
#include "words.h"
#include "Leaderboard.h"


extern struct packet pacote;
extern Sprite* spritePausa;
extern Sprite* spriteCursor;
extern Sprite* spriteMenu;
extern Sprite* spriteGameOver;
extern Sprite* spriteInstructions;
extern uint8_t* secondBuffer; // de video_card.c
extern int irq_set_keyboard, irq_set_timer, irq_set_rtc, irq_set_mouse;
extern uint8_t scancode; // de keyboard.c
int level = 1;
bool level_1 = true;
bool level_2 = false;
bool level_3 = false;
bool level_4 = false;
bool level_5 = false;
extern int y0_word;
extern int x0_word;
bool pausedGame = false;
bool game_over = false;
bool inGame = false;
bool askName = false;
bool inMenu = true;
bool inLeaderboard = false;
bool inInstructions = false;
extern bool exitGame;
extern int mouse_x,mouse_y; // de mouse.c
extern char* current_word; // de words.c
extern int counter;  // de timer.c

extern int numero_palavras_digitadas;
int num_seconds = 0;
extern Time time_rtc; // de rtc.c
extern Time previous_time_rtc; // de rtc.c

int subscribe_and_enable_everything() {
  uint8_t r;
  if(timer_subscribe_int(&r)!= 0) { //subscreve as notificações do timer
    vg_exit();
    return 1;
  }
  irq_set_timer= BIT(r);

  if (keyboard_subscribe_int(&r) != 0) {  // subscreve as notificações
    vg_exit();
    return 1;
  }
  irq_set_keyboard = BIT(r);

  if (mouse_subscribe_int(&r) != 0) {
      vg_exit();
      return 1;
  }
  irq_set_mouse = BIT(r);

  if (rtc_subscribe_int(&r) != 0) {
    vg_exit();
    return 1;
  }
  irq_set_rtc = BIT(r);

  if (our_mouse_enable_data_reporting() != 0)
        return 1;

  return 0;
}

int unsubscribe_everything() {
  if (keyboard_unsubscribe_int() != 0)  // cancela subscrição das notificações
       return 1;

  if(timer_unsubscribe_int() != 0)  // retira a subscrição das notificações do timer
      return 1;

  if (rtc_unsubscribe_int() != 0)
      return 1;

  if (mouse_reset_state() != 0)  // desativa o data reporting
      return 1;

  if (mouse_unsubscribe_int() != 0)
      return 1;
      
  return 0;
}

char* getFirstWord() {
    char *word = get_word();
    x0_word = rand() % 300;
    return word;
}

void set_level() {
  if ((num_seconds > 7) && (num_seconds <= 14)) {
      level = 2;
      level_2 = true;
      level_1 = false;
  }
  else if ((num_seconds > 14) && (num_seconds <= 21)) {
      level = 3;
      level_3 = true;
      level_2 = false;
  }
  else if ((num_seconds > 21) && (num_seconds <= 28)) {
      level = 4;
      level_4 = true;
      level_3 = false;
  }
  else if (num_seconds > 28) {
      level = 5;
      level_5 = true;
      level_4 = false;
  }
}

void level_handler() {
  if (level_1) {
      y0_word += 5;
  }
  else if (level_2) {
      y0_word += 10;
  }
  else if (level_3) {
      y0_word += 15;
  }
  else if (level_4) {
      y0_word += 20;
  }
  else if (level_5) {
      y0_word += 30;
  }
}

void pauseGameHandlerKeyboard() {
  if (scancode == ENTER_BREAK && (inGame || pausedGame)) {
      pausedGame = !pausedGame;
      inGame = !inGame;
  }
}

char* askNameIfRecordHandlerKeyboard(char* topPlayerName) {
  if (askName) {

      if (is_valid_scancode(scancode)) {
          char* c = scancode_to_char_pointer(scancode);
          strcat(topPlayerName, c);
      }

      if (scancode == BACKSPACE_BREAK && strlen(topPlayerName) > 0)
          topPlayerName[strlen(topPlayerName)-1] = 0; // remove the last character
  }
  return topPlayerName;
}

void gameWordsHandlerKeyboard() {
  if(inGame) 
      update_word(scancode_to_char(scancode));
}

bool mainMenuHandlerMouse() {
  bool shoud_I_break = false;
  if (inMenu) {
      if (mouse_x > 401 && mouse_x < 875 && mouse_y > 344 && mouse_y < 438 && pacote.lb) { // botão PLAY
          inGame = true;
          inMenu = false;
          shoud_I_break = true;
      }
      if (mouse_x > 401 && mouse_x < 875 && mouse_y > 488 && mouse_y < 580 && pacote.lb) { // botão INSTRUCTIONS
          inInstructions = true;
          inMenu = false;
          shoud_I_break = true;
      }
      if (mouse_x > 401 && mouse_x < 875 && mouse_y > 636 && mouse_y < 735 && pacote.lb) { // botão LEADERBOARD
          inLeaderboard = true;
          inMenu = false;
          shoud_I_break = true;
      }
      if (mouse_x > 401 && mouse_x < 875 && mouse_y > 785 && mouse_y < 880 && pacote.lb) { // botão EXIT
          exitGame = true;
          shoud_I_break = true;
      }
  }
    return shoud_I_break;
}

bool leaderboardHandlerMouse() {
  if (inLeaderboard) {
      if (mouse_x > 1122 && mouse_x < 1240 && mouse_y > 857 && mouse_y < 964 && pacote.lb) { // botão BACK
          inLeaderboard = false;
          inMenu = true;
          return true;
      }
  }
  return false;
}

bool instructionsHandlerMouse() {
    if (inInstructions) {
        if (mouse_x > 1046 && mouse_x < 1209 && mouse_y > 824 && mouse_y < 964 && pacote.lb) { // botão BACK
          inInstructions = false;
          inMenu = true;
          return true;
      }
  }
  return false;
}

bool askNameHandlerMouse(char* topPlayerName) {
  if (askName) {
      if (mouse_x > 1055 && mouse_x < 1165 && mouse_y > 683 && mouse_y < 776 && pacote.lb) { // botão CONFIRMAR
          if (new_record_handler(numero_palavras_digitadas, topPlayerName)) {
              write_leaderboard_file();
          }
          game_over = true;
          askName = false;
          return true;
      }
  }
  return false;
}

bool gamerOverHandlerMouse() {
  if (game_over) {
      if (mouse_x > 393 && mouse_x < 912 && mouse_y > 479 && mouse_y < 568 && pacote.lb) { // botão PLAY AGAIN
          inGame = true;
          game_over = false;
          num_seconds = 0;
          numero_palavras_digitadas = 0;
          level = 1;
          level_1 = true;
          level_2 = false;
          level_3 = false;
          level_4 = false;
          level_5 = false;
          
          return true;
      }
      if (mouse_x > 393 && mouse_x < 912 && mouse_y > 618 && mouse_y < 704 && pacote.lb) { // botão BACK TO MENU
          inMenu = true;
          game_over = false;
          num_seconds = 0;
          numero_palavras_digitadas = 0;
          level = 1;
          level_1 = true;
          level_2 = false;
          level_3 = false;
          level_4 = false;
          level_5 = false;
          
          return true;
      }
      if (mouse_x > 393 && mouse_x < 912 && mouse_y > 741 && mouse_y < 830 && pacote.lb) { // botão EXIT
          exitGame = true;
          return true;
      }

  }
  return false;
}

bool gameHandler() {
    if (inGame) {

      set_level();
      level_handler();

      if (y0_word > 880) {
          inGame = false;
          askName = true;
          if (!is_new_record(numero_palavras_digitadas)) {
              game_over = true;
              askName = false;
              return true;
          } 
      }

      read_time();
      if (a_second_passed()) // counting the number of seconds using rtc
          num_seconds++;

      if (counter % INTERRUPTS_PER_FRAME_GAME == 0) {
          draw_everything_in_game(current_word, numero_palavras_digitadas, secondBuffer, y0_word);
      }
  }
  return false;
}

void pausedGameHanler() {
  if (pausedGame)
      draw_backgroud(spritePausa, secondBuffer);
}

void askNameHandler(char* topPlayerName) {
  if (askName) {
    if (counter % INTERRRUPTS_PER_FRAME_MOUSE == 0) {
        askForName(secondBuffer, topPlayerName);
        draw_cursor(spriteCursor, secondBuffer);
    }
  }
}

void menuHandler() {
  if (inMenu) {
    if (counter % INTERRRUPTS_PER_FRAME_MOUSE == 0) {
        draw_backgroud(spriteMenu, secondBuffer);
        draw_cursor(spriteCursor,secondBuffer);
    }
  }
}

void leaderboardHandler() {
  if (inLeaderboard) {
      if (counter % INTERRRUPTS_PER_FRAME_MOUSE == 0) {
          display_leaderboard(secondBuffer);
          draw_cursor(spriteCursor,secondBuffer);
      }
  }
}

void instructionsHandler() {
    if (inInstructions) {
        if (counter % INTERRRUPTS_PER_FRAME_MOUSE == 0) {
            draw_backgroud(spriteInstructions, secondBuffer);
            draw_cursor(spriteCursor, secondBuffer);
        }
    }
}

void gameOverHandler() {
  if (game_over) {
      if (counter % INTERRRUPTS_PER_FRAME_MOUSE == 0) {
          draw_backgroud(spriteGameOver, secondBuffer);
          displayNumber(655,280,numero_palavras_digitadas,secondBuffer); // score
          draw_cursor(spriteCursor,secondBuffer);
      }
  }
}

