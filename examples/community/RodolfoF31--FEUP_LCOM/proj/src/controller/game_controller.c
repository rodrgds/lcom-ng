#include "game_controller.h"
#include "../view/draw_game.h"
#include <stdlib.h>

char *get_random_easy_phrase() {
  static int seeded = 0;
  if (!seeded) {
    srand(time(NULL));
    seeded = 1;
  }
  int idx = rand() % EASY_PHRASES_COUNT;
  return easy_phrases[idx];
}

char *get_random_hard_phrase() {
  static int seeded = 0;
  if (!seeded) {
    srand(time(NULL));
    seeded = 1;
  }
  int idx = rand() % HARD_PHRASES_COUNT;
  return hard_phrase[idx];
}

int map_words_in_sentence(const char *sentence, int x, int y, WordPosition *word_positions, int max_words) {
  int x_res = x;
  int y_res = y;
  int fixed_spacing = 0;
  int line_spacing = 8;
  int screen_width = get_horizontal_resolution();

  char **words = split_phrase(sentence);
  if (words == NULL)
    return 0;

  int count = 0;
  for (int w = 0; words[w] != NULL && count < max_words; w++) {
    char *word = words[w];
    int word_width = 0;

    // Calculate the width of the word
    for (int idx = 0; word[idx] != '\0'; idx++) {
      const xpm_row_t *sprite = get_letter_sprite(word[idx]);
      if (sprite != NULL) {
        word_width += get_xpm_width(sprite) + fixed_spacing;
      }
    }

    // If the word doesn't fit, move to the next line
    if (x_res + word_width > screen_width) {
      x_res = x;
      y_res += line_spacing + get_xpm_height(get_letter_sprite('A'));
    }

    // Store word and its Pixel coordinates
    word_positions[count].word = strdup(word);
    word_positions[count].coordinates.x = x_res;
    word_positions[count].coordinates.y = y_res;
    count++;

    // Move x_res for next word (add space)
    const xpm_row_t *space_sprite = get_letter_sprite(' ');
    if (space_sprite != NULL) {
      x_res += word_width + get_xpm_width(space_sprite) + fixed_spacing;
    }
    else {
      x_res += word_width;
    }
  }

  for (int i = 0; words[i] != NULL; i++) free(words[i]);
  free(words);

  return count; // Number of words mapped
}

void trim_trailing_whitespace(char *str) {
  int len = strlen(str);
  while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\n' || str[len - 1] == '\r')) {
    str[--len] = '\0';
  }
}

void check_input(GameState *game_state) {
  if (game_state == NULL || game_state->input == NULL || game_state->input->word == NULL)
    return;

  printf("Checking input: %d\n", game_state->current_word_index);
  WordPosition curr_word = game_state->word_positions[game_state->current_word_index];
  if (curr_word.word == NULL) {
    printf("No word to check at index %d\n", game_state->current_word_index);
    return;
  }else {
    printf("Current word: %s\n", curr_word.word);
  }

  // Make local copies to trim
  char input_copy[128], word_copy[128];
  strncpy(input_copy, game_state->input->word, sizeof(input_copy));
  input_copy[sizeof(input_copy) - 1] = '\0';
  strncpy(word_copy, curr_word.word, sizeof(word_copy));
  word_copy[sizeof(word_copy) - 1] = '\0';

  trim_trailing_whitespace(input_copy);
  trim_trailing_whitespace(word_copy);

  if (strcmp(input_copy, word_copy) == 0) {
    draw_answer(game_state, true);
    game_state->correct_words++;
  }
  else {
    draw_answer(game_state, false);
    game_state->incorrect_words++;
  }
}

void game_loop(int8_t kbd_irq, int8_t timer_irq, GameConfig *config, GameState *game_state) {

  int r;
  message msg;
  int ipc_status;

  uint8_t data[2] = {0, 0};
  uint8_t err = 0;
  bool skip = false;
  int counter = 0;
  int timer = 0;
  char ascii_char = 0;
  bool isShiftPressed = false;

  while (data[0] != ESC_BREAK) {



    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_rget_eceive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & timer_irq) {
            refresh_frame_buffer();
            timer_int_handler();
            timer_get_counter(&counter);
            if (counter % 60 == 0) {
              timer++;
            }
            if (timer == config->seconds_per_round) {
              game_state->time_taken = timer;
              game_state->lost = true;
              return;
            }
            if (game_state->current_word_index >= game_state->word_count) {
              printf("Game finished, no more words to process.\n");
              game_state->time_taken = timer;
              game_state->lost = false;
              return;
            }
          }

          if (msg.m_notify.interrupts & kbd_irq) { // checks if interrupt comes from the subscribed device

            kbc_ih();
            get_kbc_data(data);
            get_kbc_error(&err);
            get_flag(&skip);

            if (skip) {
              continue;
            }

            if (err) {
              printf("Error\n");
              continue;
            }

            if(data[0] == SHIFT_CODE_BREAK) {
              printf("Shift key released\n");
              isShiftPressed = false; // Clear shift pressed state on break code
            }
          
            if(data[0] == SHIFT_CODE_MAKE) {
              printf("Shift key pressed\n");
              isShiftPressed = true; // Set shift pressed state on make code
            }

            ascii_char = breakcode_to_ascii(data[0]);

            if (data[0] == BACKSPACE_CODE) {
              int len = strlen(game_state->input->word);
              if (len > 0) {
                game_state->input->word[len - 1] = '\0'; // Remove last character
                draw_input(game_state);
              }
              continue;
            }
            else if (data[0] == SPACE_CODE) {
              check_input(game_state);
              game_state->input->word[0] = '\0'; // Clear input word
              game_state->current_word_index++;
              printf("Current word index: %d\n", game_state->current_word_index);
              printf("Word count: %d\n", game_state->word_count);
              clear_input(game_state);

              continue;
            }
            else if (ascii_char != 0) {

              if((ascii_char == '!' || ascii_char == '?') && !isShiftPressed) {
                continue;
              }

              int len = strlen(game_state->input->word);
              game_state->input->word[len] = ascii_char;
              game_state->input->word[len + 1] = '\0';

              draw_input(game_state);
              continue;
            }
          }
          break;
        default:
          break;
      }
    }
    else {
      // no standard messages expected: do nothing
    }
  }
}

void end_game(GameState *game_state, GameConfig *config, int8_t kbd_irq, int8_t timer_irq) {

  int r;
  message msg;
  int ipc_status;

  uint8_t data[2] = {0, 0};
  uint8_t err = 0;
  bool skip = false;
  int counter = 0;
  int timer = 0;

  if (game_state->lost) {
    draw_game_lost(game_state, config->seconds_per_round);
  }
  else {
    draw_game_stats(game_state, config->seconds_per_round);
  }

  while (data[0] != ESC_BREAK) {

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_rget_eceive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & timer_irq) {
            refresh_frame_buffer();
            timer_int_handler();
            timer_get_counter(&counter);
            if (counter % 60 == 0) {
              timer++;
            }
            if (timer == TIMEOUT) {
              game_state->time_taken = timer;

              return;
            }
          }

          if (msg.m_notify.interrupts & kbd_irq) { // checks if interrupt comes from the subscribed device

            kbc_ih();
            get_kbc_data(data);
            get_kbc_error(&err);
            get_flag(&skip);

            if (skip) {
              continue;
            }

            if (err) {
              printf("Error\n");
              continue;
            }

          }
          break;
        default:
          break;
      }
    }
    else {
      // no standard messages expected: do nothing
    }
  }
}
