#include <lcom/lcf.h>

#include "pen_pal.h"

bool drawn_promts[5] = {false};
bool first_letter = true;
bool words_drawn = false;
bool submitted_guess = false;
bool curr_word_drawn = false;
bool state_drawn = false;
bool dark;
bool eraser_on = false;

int drawing_time_left = DRAWING_TIME;
int analysis_time_left = ANALYSE_TIME;
int no_guesses = 0;
int old_index = 0;
int game_number = 1;
uint32_t selected_color;
uint32_t background_color;
char selected_thickness = 'S';
xpm_image_t frame;
int16_t prev_letter_width = 0;

extern int counter;
extern uint8_t mouse_scancode;
uint8_t mouse_bytes[3];
struct packet pp;
int packet_size = 1;

// From "mouse.c"
extern Cursor *cursor;

/* State Machine */
KeyboardState keyboard_state = KBD_GETTING_NICKNAME_1;
GameState game_state = GETTING_NICKNAME_1;
MouseState mouse_state = MOUSE_GETTING_NICKNAMES;

char player1_nickname[MAX_WORD_SIZE] = {'\0'};
char player2_nickname[MAX_WORD_SIZE] = {'\0'};
char guess[MAX_WORD_SIZE] = {'\0'};

char *word1, *word2, *word3;
char *curr_word;

static Sprite *light_screen;
static Sprite *dark_screen;
static Sprite *prompts[5];
Sprite *tick;
Sprite *eraser_sp;
Time curr_time;

uint32_t timer_irq_bit, keyboard_irq_bit, mouse_irq_bit;
extern xpm_image_t separator;

void(start_xpms)() {
  light_screen = create_sprite(light_screen_xpm, 0, 0, 0, 0);
  dark_screen = create_sprite(dark_screen_xpm, 0, 0, 0, 0);

  prompts[0] = create_sprite(player1_nickname_xpm, 404, 340, 0, 0);
  prompts[1] = create_sprite(player2_nickname_xpm, 404, 340, 0, 0);
  prompts[2] = create_sprite(word_selection_xpm, 404, 340, 0, 0);
  prompts[3] = create_sprite(guess_xpm, 404, 340, 0, 0);
  prompts[4] = create_sprite(play_again_exit_game_xpm, 404, 340, 0, 0);

  tick = create_sprite(cross_xpm, 0, 0, 0, 0);
  eraser_sp = create_sprite(eraser_xpm, 0, 0, 0, 0);

  load_alphabet();
  load_numbers();
  load_colors();
}

void(draw_initial_elements)() {
  if (dark) {
    draw_sprite(dark_screen, false);
    xpm_load(dark_screen_xpm, XPM_8_8_8_8, &frame);
    background_color = DARK_MODE_GREY;
  }
  else {
    draw_sprite(light_screen, false);
    xpm_load(light_screen_xpm, XPM_8_8_8_8, &frame);
    background_color = WHITE;
  }
  draw_colors();
  draw_cursor();

  draw_unfilled_square(50, 870, 20, BLACK, true);
  draw_letter('S', 80, 865);
  draw_pixmap(50, 870, &(tick->img), true);

  draw_unfilled_square(50, 910, 20, BLACK, true);
  draw_letter('M', 80, 905);

  draw_unfilled_square(50, 950, 20, BLACK, true);
  draw_letter('L', 80, 945);

  draw_pixmap(200, 872, &(eraser_sp->img), true);

  draw_game_number();
  return;
}

void(delete_elements)() {
  destroy_sprite(light_screen);

  for (int i = 0; i < 4; i++) {
    destroy_sprite(prompts[i]);
  }

  destroy_colors();

  return;
}

int(play_game)() {
  /* Check whether or not dark mode needs to  be enabled */
  dark = is_dark_mode();

  selected_color = (dark) ? WHITE : BLACK;

  uint8_t timer_bit, keyboard_bit, mouse_bit;

  message msg;
  int r, ipc_status;

  if (timer_subscribe_int(&timer_bit)) {
    printf("Error: Failed to subscribe timer interruptions\n");
    return 1;
  }

  if (kbc_subscribe_int(&keyboard_bit)) {
    printf("Error: Failed to subscribe keyboard interruptions\n");
    return 1;
  }

  if (mouse_enable_data_reporting()) {
    printf("Error: Failed to enable mouse data reporting\n");
    return 1;
  }

  if (mouse_subscribe_int(&mouse_bit)) {
    printf("Error: Failed to subscribe mouse interruptions\n");
    return 1;
  }

  timer_irq_bit = BIT(timer_bit);
  keyboard_irq_bit = BIT(keyboard_bit);
  mouse_irq_bit = BIT(mouse_bit);

  /* Chooses 3 random works from a list */
  generate_words(word1, word2, word3);
  start_xpms();

  /* Create new cursor */
  new_cursor(500, 30);

  draw_initial_elements();

  draw_game_number();

  while (game_state != EXIT_GAME && get_scancode()[0] != ESC_BRK_CODE) {
    if ((r = driver_receive(ANY, &msg, &ipc_status))) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                      /* hardware interrupt notification */
          if (msg.m_notify.interrupts & keyboard_irq_bit) { /* Interrupt from the keyboard */
            keyboard_event_handler();
          }
          if (msg.m_notify.interrupts & timer_irq_bit) { /* Interrupt from the timer */
            timer_event_handler();

            /* Each second, print the time */
            if (counter % 60 == 0) {
              read_curr_time_from_rtc(&curr_time);
              print_time(curr_time);
              draw_game_number();
            }

            if (!state_drawn) {
              display_state(game_state);
              state_drawn = true;
            }
          }
          if (msg.m_notify.interrupts & mouse_irq_bit) {
            mouse_ih();

            mouse_bytes[packet_size - 1] = mouse_scancode;

            if (packet_size == 1 && (mouse_scancode & IS_FIRST_BYTE))
              packet_size++;

            else if (packet_size == 2)
              packet_size++;

            else if (packet_size == 3) {
              for (int i = 0; i < 3; i++) {
                pp.bytes[i] = mouse_bytes[i];
              }
              mouse_parse_packet(&pp);
              mouse_event_handler();

              packet_size = 1;
            }
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      }
    }
    else { /* received a standard message, not a notification */
           /* no standard messages expected: do nothing */
    }
  }

  /* if (mouse_disable_data_reporting()) {
    printf("Error: Failed to disable mouse data reporting\n");
    return 1;
  } */

  if (mouse_unsubscribe_int()) {
    printf("Error while unsubscribing mouse interrupts\n");
    return 1;
  }

  if (mouse_disable_data_reporting()) {
    return 1;
  }

  if (kbc_unsubscribe_int() != 0) {
    printf("Error while unsubscribing keyboard interrupts\n");
    return 1;
  }

  /* timer */
  if (timer_unsubscribe_int() != 0) {
    printf("Error while unsubscribing timer interrupts\n");
    return 1;
  }

  delete_elements();

  return 0;
}

void(timer_event_handler)() {
  counter++;
  switch (game_state) {
    case GETTING_NICKNAME_1: {
      if (!(drawn_promts[0])) {
        draw_sprite(prompts[0], true);
        drawn_promts[0] = true;
      }
      break;
    }

    case GETTING_NICKNAME_2: {
      if (!(drawn_promts[1])) {
        draw_sprite(prompts[1], true);
        drawn_promts[1] = true;
      }
      break;
    }

    case CHOOSING_WORD: {
      if (!(drawn_promts[2])) {
        draw_sprite(prompts[2], true);
        drawn_promts[2] = true;
      }
      if (!(words_drawn)) {
        draw_word(word1, 500, 474);
        draw_word(word2, 500, 564);
        draw_word(word3, 500, 654);
        words_drawn = true;
      }
      break;
    }

    case PLAYER1_DRAWING: {
      if (!(curr_word_drawn)) {
        draw_initial_elements();
        draw_word(curr_word, 150 - (strlen(curr_word) / 2) * 25, 280);
        curr_word_drawn = true;
      }
      if (counter % 60 == 0) {
        display_time_left(drawing_time_left);
        drawing_time_left--;
      }

      if (drawing_time_left <= -1) {
        game_state = ANALYSE_DRAWING;
        mouse_state = MOUSE_ANALYSING;
        state_drawn = false;
        counter = 0;
      }

    } break;

    case ANALYSE_DRAWING: {
      if (curr_word_drawn) {
        my_vg_draw_rectangle(10, 237, 290, 119, SIDEBAR_GREY, true);
        curr_word_drawn = false;
      }

      if (counter % 60 == 0) {
        display_time_left(analysis_time_left);
        analysis_time_left--;
      }

      if (analysis_time_left <= -1) {
        game_state = PLAYER2_GUESSING;
        counter = 0;
        mouse_state = MOUSE_GUESSING;
        keyboard_state = KBD_GETTING_GUESSES;
        state_drawn = false;
      }
      break;
    }

    case PLAYER2_GUESSING: {
      /* Hide word from guessing player */
      if (!drawn_promts[3]) {
        draw_sprite(prompts[3], true);
        drawn_promts[3] = true;
      }
      draw_word_with_color("GUESSES LEFT", 522, 665, BLACK);
      draw_number(MAX_GUESSES - no_guesses, 847, 650, GREEN);

      /* If the player has submitted a guess */
      if (submitted_guess) {

        /* Check if the user has guessed the answer */
        if (!(strcmp(guess, curr_word))) {
          game_state = CORRECT_GUESS;
          state_drawn = false;
        }
        else {
          no_guesses++;
          if (no_guesses == MAX_GUESSES) {
            game_state = OUT_OF_GUESSES;
          }
          else {
            game_state = WRONG_GUESS;
            memset(guess, '\0', sizeof guess);
            drawn_promts[3] = false;
          }
        }
        submitted_guess = false;
      }
      break;
    }

    case CORRECT_GUESS: {
      if (!drawn_promts[4]) {
        draw_sprite(prompts[4], true);
        drawn_promts[4] = true;
      }
      draw_word_with_color("CORRECT GUEESS YOU WIN", 414, 368, GREEN);

      mouse_state = MOUSE_PLAY_AGAIN;
      // game_state = PLAY_AGAIN;
      keyboard_state = KBD_IN_GAME;

      break;
    }

    case WRONG_GUESS: {
      if (!drawn_promts[3]) {
        draw_sprite(prompts[3], true);
        drawn_promts[3] = true;
      }
      uint32_t color = (MAX_GUESSES - no_guesses < 2) ? RED : YELLOW;
      draw_word_with_color("WRONG GUESS TRY AGAIN", 412, 610, color);
      draw_word_with_color("GUESSES LEFT", 522, 665, BLACK);
      draw_number(MAX_GUESSES - no_guesses, 847, 650, color);

      /* If the player has submitted a guess */
      if (submitted_guess) {
        /* Check if the user has guessed the answer */
        if (!(strcmp(guess, curr_word))) {
          game_state = CORRECT_GUESS;
          state_drawn = false;
        }
        else {
          no_guesses++;
          if (no_guesses == MAX_GUESSES) {
            game_state = OUT_OF_GUESSES;
          }
          else {
            game_state = WRONG_GUESS;
            memset(guess, 0, sizeof(guess));
            drawn_promts[3] = false;
          }
        }
        submitted_guess = false;
      }
      break;
    }
    case OUT_OF_GUESSES: {
      if (!drawn_promts[4]) {
        draw_sprite(prompts[4], true);
        drawn_promts[4] = true;
      }
      draw_word_with_color("OUT OF GUESSES YOU LOSE", 403, 368, RED);

      mouse_state = MOUSE_PLAY_AGAIN;
      // game_state = PLAY_AGAIN;
      keyboard_state = KBD_IN_GAME;

      break;
    }
    case PLAY_AGAIN: {
      game_state = CHOOSING_WORD;
      break;
    }

    case EXIT_GAME: return;
    default: break;
  }
}

void(keyboard_event_handler)() {
  switch (keyboard_state) {
    case KBD_IN_GAME: {
      kbc_ih();

      /* If an error has occurred, don't print scancode */
      if (is_error()) {
      }

      /* Increment size if we are dealing with a 2 byte scancode */
      if (two_byte_code())
        set_scancode_size(2);

      else
        set_scancode_size(1);

      break;
    }
    case KBD_GETTING_NICKNAME_1: {
      kbc_ih();
      /* If an error has occurred, don't print scancode */
      if (is_error())
        ;
      /* Increment size if we are dealing with a 2 byte scancode */
      if (two_byte_code())
        set_scancode_size(2);

      else {
        uint8_t code = get_scancode()[0];
        char c = get_character_from_code(code);

        if (strlen(player1_nickname) == MAX_WORD_SIZE) {
          keyboard_state = KBD_GETTING_NICKNAME_2;
          game_state = GETTING_NICKNAME_2;
          prev_letter_width = 0;
          first_letter = true;
          break;
        }
        if ((strlen(player1_nickname) > 0) && code == BACKSPACE_BRK_CODE) {
          char last_char = remove_character(player1_nickname);
          int last_char_width = get_width(last_char);
          prev_letter_width -= last_char_width;
          int position_to_draw = 503 + prev_letter_width - 2;
          if (strlen(player1_nickname) == 0) {
            prev_letter_width = 0;
          }
          erase_letter(last_char, position_to_draw, 590);
        }
        else {
          if (c != '\0') // Not recognized character
            add_character(player1_nickname, c);

          else if (code != ENTER_BRK_CODE)
            break;

          draw_letter(c, 503 + prev_letter_width - 2, 590);

          prev_letter_width += get_width(c);
        }
        set_scancode_size(1);
      }

      /* If we receive an enter, it means the player 1 has finished writting their name */
      if (get_scancode()[0] == ENTER_BRK_CODE && strlen(player1_nickname) > 0) {
        keyboard_state = KBD_GETTING_NICKNAME_2;
        game_state = GETTING_NICKNAME_2;
        prev_letter_width = 0;
        first_letter = true;
      }
      break;
    }

    case KBD_GETTING_NICKNAME_2: {
      kbc_ih();
      /* If an error has occurred, don't print scancode */
      if (is_error())
        ;
      /* Increment size if we are dealing with a 2 byte scancode */
      if (two_byte_code())
        set_scancode_size(2);

      else {
        uint8_t code = get_scancode()[0];
        char c = get_character_from_code(code);

        if ((strlen(player2_nickname) > 0) && code == BACKSPACE_BRK_CODE) {
          char last_char = remove_character(player2_nickname);
          int last_char_width = get_width(last_char);
          prev_letter_width -= last_char_width;
          int position_to_draw = 503 + prev_letter_width - 2;
          if (strlen(player2_nickname) == 0) {
            position_to_draw = 503;
            prev_letter_width = 0;
            first_letter = true;
          }
          erase_letter(last_char, position_to_draw, 590);
        }
        else {
          if (c != '\0') // Not recognized character
            add_character(player2_nickname, c);

          else if (code != ENTER_BRK_CODE)
            break;

          if (strlen(player1_nickname) == MAX_WORD_SIZE) {
            keyboard_state = KBD_CHOOSING_WORD;
            game_state = CHOOSING_WORD;
            mouse_state = MOUSE_CHOOSING_WORD;
            state_drawn = false;
            first_letter = true;
            prev_letter_width = 0;
            break;
          }

          if (first_letter) {
            draw_letter(c, 503, 590);
            first_letter = false;
          }
          else {
            if (strlen(player2_nickname) < MAX_WORD_SIZE)
              draw_letter(c, 503 + prev_letter_width - 2, 590);
          }
          prev_letter_width += get_width(c);
        }
        set_scancode_size(1);
      }

      /* If we receive an enter, it means the player 2 has finished writting their name */
      if (get_scancode()[0] == ENTER_BRK_CODE && strlen(player2_nickname) > 0) {
        keyboard_state = KBD_CHOOSING_WORD;
        game_state = CHOOSING_WORD;
        mouse_state = MOUSE_CHOOSING_WORD;
        state_drawn = false;
        first_letter = true;
        prev_letter_width = 0;
      }
      break;
    }

    case KBD_CHOOSING_WORD: {
      kbc_ih();
      /* If an error has occurred, don't print scancode */
      if (is_error())
        ;
      /* Increment size if we are dealing with a 2 byte scancode */
      if (two_byte_code())
        set_scancode_size(2);

      else {
        set_scancode_size(1);
      }

      uint8_t code = get_scancode()[0];

      if (code == NUMBER_1_BRK_CODE)
        curr_word = word1;
      else if (code == NUMBER_2_BRK_CODE)
        curr_word = word2;
      else if (code == NUMBER_3_BRK_CODE)
        curr_word = word3;

      if (curr_word != NULL) {
        keyboard_state = KBD_IN_GAME;
        game_state = PLAYER1_DRAWING;
        mouse_state = MOUSE_DRAWING;
        state_drawn = false;
      }
      break;
    }

    case KBD_GETTING_GUESSES: {
      kbc_ih();
      /* If an error has occurred, don't print scancode */
      if (is_error())
        ;
      /* Increment size if we are dealing with a 2 byte scancode */
      if (two_byte_code())
        set_scancode_size(2);

      else {
        uint8_t code = get_scancode()[0];
        char c = get_character_from_code(code);

        /* The user submitted a guess */
        if (code == ENTER_BRK_CODE && strlen(guess) > 0) {
          submitted_guess = true;
          first_letter = true;
          prev_letter_width = 0;
          break;
        }

        if (strlen(guess) == MAX_WORD_SIZE) {
          submitted_guess = true;
          first_letter = true;
          prev_letter_width = 0;
          break;
        }
        if ((strlen(guess) > 0) && code == BACKSPACE_BRK_CODE) {
          char last_char = remove_character(guess);
          int last_char_width = get_width(last_char);
          prev_letter_width -= last_char_width;
          int position_to_draw = 503 + prev_letter_width - 2;
          if (strlen(guess) == 0) {
            prev_letter_width = 0;
          }
          erase_letter(last_char, position_to_draw, 542);
        }
        else {
          if (c != '\0') // Not recognized character
            add_character(guess, c);

          draw_letter(c, 503 + prev_letter_width - 2, 542);

          prev_letter_width += get_width(c);
        }
        set_scancode_size(1);
      }
      break;
    }
    default: break;
  }
}

void(mouse_event_handler)() {
  update_cursor(&pp);
  Collision colis;

  switch (mouse_state) {
    case MOUSE_GETTING_NICKNAMES: break;
    case MOUSE_CHOOSING_WORD: {
      colis = cursor_collisions_in_word_selection();
      if (!pp.lb)
        break;
      switch (colis) {
        case OVER_WORD_1: {
          curr_word = word1;
          break;
        }
        case OVER_WORD_2: {
          curr_word = word2;
          break;
        }
        case OVER_WORD_3: {
          curr_word = word3;
          break;
        }
        default:
          break;
      }
      if (curr_word != NULL) {
        keyboard_state = KBD_IN_GAME;
        game_state = PLAYER1_DRAWING;
        mouse_state = MOUSE_DRAWING;
        state_drawn = false;
      }
      break;
    }
    case MOUSE_DRAWING: {
      colis = cursor_collisions_in_game();

      if (!pp.lb)
        break;

      if (colis > 12 && colis < 16) { /* boxes */
        int index = colis - 13;
        my_vg_draw_rectangle(51, 871 + old_index * 40, 18, 18, SIDEBAR_GREY, true);
        draw_pixmap(50, 870 + index * 40, &(tick->img), true);
        old_index = index;
        update_width(index);
      }
      else {
        uint32_t new_color;
        handle_mouse_collision_with_colors(&new_color, colis);

        if (new_color == background_color)
          eraser_on = true;

        else
          eraser_on = false;

        selected_color = new_color;
      }
      mouse_draw_pixel(selected_color, selected_thickness, true);
      break;
    }
    case MOUSE_ANALYSING: break;
    case MOUSE_GUESSING: break;

    case MOUSE_PLAY_AGAIN:
      colis = cursor_collisions_option_selection();
      if (!pp.lb)
        break;
      switch (colis) {
        case OVER_PLAY_AGAIN:
          prepare_restart();
          break;

        case OVER_PLAY_SWITCH: {
          switch_nicknames();
          prepare_restart();
          break;
        }

        case OVER_EXIT_GAME:
          game_state = EXIT_GAME;
        default:
          break;
      }

    default:
      break;
  }
}

void(prepare_restart)() {
  /* generate new words */
  generate_words();

  /* Reset timers */
  drawing_time_left = DRAWING_TIME;
  analysis_time_left = ANALYSE_TIME;

  /* Reset # of guesses and the guess itself*/
  no_guesses = 0;
  memset(guess, 0, sizeof(guess));

  /* Do this in order to write in the correct spot */
  prev_letter_width = 0;

  /* Reset some state variables */
  words_drawn = false;
  submitted_guess = false;
  curr_word_drawn = false;
  state_drawn = false;

  /* Reset state variables of prompts not yet drawn */
  for (int i = 2; i < 5; i++) {
    drawn_promts[i] = false;
  }

  my_vg_draw_rectangle(51, 871 + old_index * 40, 18, 18, SIDEBAR_GREY, true);
  draw_pixmap(50, 870, &(tick->img), true);

  old_index = 0;
  game_number++;
  selected_thickness = 'S';

  /* Change game, mouse and keyboard states */
  game_state = PLAY_AGAIN;
  keyboard_state = KBD_CHOOSING_WORD;
  mouse_state = MOUSE_CHOOSING_WORD;
}

void(display_state)(GameState state) {
  int xpos = 50, ypos = 450;
  my_vg_draw_rectangle(0, 410, 330, 170, SIDEBAR_GREY, true);
  char str[40] = {'\0'};

  switch (state) {
    case 0: {
      strcat(str, "GETTING NICKNAMES");
      draw_big_word(str, xpos, ypos);
      break;
    }
    case 1: break;
    case 2: {
      strcat(str, player1_nickname);
      strcat(str, " ");
      strcat(str, "CHOOSING WORD");
      draw_big_word(str, xpos, ypos);
      break;
    }
    case 3: {
      strcat(str, player1_nickname);
      strcat(str, " ");
      strcat(str, "DRAWING");
      draw_big_word(str, xpos, ypos);
      break;
    }
    case 4: {
      strcat(str, player2_nickname);
      strcat(str, " ");
      strcat(str, "ANALYSING DRAWING");
      draw_big_word(str, xpos, ypos);
      break;
    }
    case 5: {
      strcat(str, player2_nickname);
      strcat(str, " ");
      strcat(str, "GUESSING");
      draw_big_word(str, xpos, ypos);
      break;
    }

    case 6: break;
    case 7: break;
    case 8: break;
    case 9: break;
    default: break;
  }
}

void(generate_words)() {
  char *tmp;
  word1 = get_random_word();

  /* Make sure we generate a different word */
  do {
    tmp = get_random_word();
  } while (!(strcmp(tmp, word1)));

  word2 = tmp;
  tmp = NULL;

  /* Make sure we generate a different word */
  do {
    tmp = get_random_word();
  } while (!strcmp(tmp, word1) || !strcmp(tmp, word2));

  word3 = tmp;
}

void(update_width)(int index) {
  switch (index) {
    case 0:
      selected_thickness = 'S';
      break;

    case 1:
      selected_thickness = 'M';
      break;

    case 2:
      selected_thickness = 'L';
      break;

    default:
      break;
  }
}

void(switch_nicknames)() {
  char tmp[MAX_WORD_SIZE] = {'\0'};

  strcpy(tmp, player1_nickname);
  strcpy(player1_nickname, player2_nickname);
  strcpy(player2_nickname, tmp);
}

void(draw_game_number)() {
  uint32_t color = (dark) ? WHITE : BLACK;
  draw_word_with_color("GAME ", 350, 964, color);
  draw_word_with_color("NUMBER", 470, 964, color);
  draw_pixmap_fixed_color(625, 964, &separator, color, true);
  draw_number(game_number, 655, 950, color);
}
