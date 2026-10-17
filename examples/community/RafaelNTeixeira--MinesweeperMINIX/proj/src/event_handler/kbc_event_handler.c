#include "kbc_event_handler.h"

void handle_kbd_event(uint8_t scan_code[2]) {
  switch (state) {
    case MAIN_MENU: handle_kbd_mainmenu_event(scan_code); break;
    case BEGINNER: handle_kbd_beginner_event(scan_code); break;
    case MEDIUM: handle_kbd_medium_event(scan_code); break;
    case HARD: handle_kbd_hard_event(scan_code); break;
    case INSTRUCTIONS: handle_kbd_instructions_event(scan_code); break; 
    case WIN: handle_kbd_gamewin_event(scan_code); break; 
    case GAMEOVER: handle_kbd_gameover_event(scan_code); break;
    case EXIT: break;
  }
}

void handle_kbd_mainmenu_event(uint8_t scan_code[2]) {
 switch (scan_code[0]) {
    case 0x02: {
      start_game_16x16();
      state = BEGINNER;
      break;
    }

    case 0x03: {
      start_game_20x16();
      state = MEDIUM;
      break;
    }

    case 0x04: {
      start_game_28x20();
      state = HARD;
      break;
    }

    case 0x05: {
      state = INSTRUCTIONS;
      break;
    }

    case ESC_BREAK: {
      state = EXIT;
      break;
    }

    default: 
      break;
    }
}

void handle_kbd_beginner_event(uint8_t scan_code[2]) {
  switch (scan_code[0]) {
    case ESC_BREAK: {
      state = MAIN_MENU;
      break;
    }
    case W_MAKE: {
      check_keyboard_w();
      break;
    }
    case A_MAKE: {
      check_keyboard_a();
      break;
    }
    case S_MAKE: {
      check_keyboard_s_16x16();
      break;
    }
    case D_MAKE: {
      check_keyboard_d_16x16();
      break;
    }
    case ENTER_MAKE: {
      check_keyboard_enter_16x16();
      break;
    }
    case F_MAKE: {
      check_keyboard_f_16x16();
      break;
    }
    case R_MAKE: {
      start_game_16x16();
      state = BEGINNER;
      break;
    }
    default: 
      break;
    }
}

void handle_kbd_medium_event(uint8_t scan_code[2]) {
  switch (scan_code[0]) {
    case ESC_BREAK: {
      state = MAIN_MENU;
      break;
    }
    case W_MAKE: {
      check_keyboard_w();
      break;
    }
    case A_MAKE: {
      check_keyboard_a();
      break;
    }
    case S_MAKE: {
      check_keyboard_s_20x16();
      break;
    }
    case D_MAKE: {
      check_keyboard_d_20x16();
      break;
    }
    case ENTER_MAKE: {
      check_keyboard_enter_20x16();
      break;
    }
    case F_MAKE: {
      check_keyboard_f_20x16();
      break;
    }
    case R_MAKE: {
      start_game_20x16();
      state = MEDIUM;
      break;
    }
    default: 
      break;
    }
}

void handle_kbd_hard_event(uint8_t scan_code[2]) {
  switch (scan_code[0]) {
    case ESC_BREAK: {
      state = MAIN_MENU;
      break;
    }
    case W_MAKE: {
      check_keyboard_w();
      break;
    }
    case A_MAKE: {
      check_keyboard_a();
      break;
    }
    case S_MAKE: {
      check_keyboard_s_28x20();
      break;
    }
    case D_MAKE: {
      check_keyboard_d_28x20();
      break;
    }
    case ENTER_MAKE: {
      check_keyboard_enter_28x20();
      break;
    }
    case F_MAKE: {
      check_keyboard_f_28x20();
      break;
    }
    case R_MAKE: {
      start_game_28x20();
      state = HARD;
      break;
    }
    default: 
      break;
    }
}

void handle_kbd_instructions_event(uint8_t scan_code[2]) {
  switch (scan_code[0]) {
    case ESC_BREAK: {
      state = MAIN_MENU;
      break;
    }

    default: 
      break;
    }
}

void handle_kbd_gamewin_event(uint8_t scan_code[2]) {
  switch (scan_code[0]) {
      case ESC_BREAK: {
        state = MAIN_MENU;
        break;
      }

      default: {
        break;
      }
    }
}

void handle_kbd_gameover_event(uint8_t scan_code[2]) {
  switch (scan_code[0]) {
    case ESC_BREAK: {
      state = MAIN_MENU;
      break;
    }

    default: {
      break;
    }
  }
}
