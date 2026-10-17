
#include "states.h"
#include "../view/draw_game.h"

MenuResult menu_state(uint8_t kbd_irq, uint8_t timer_irq) {

  int r;
  message msg;
  int ipc_status;

  uint8_t data[2] = {0, 0};
  uint8_t err = 0;
  bool skip = false;
  GameConfig config = {.difficulty = "", .seconds_per_round = 0};

  MenuState menu;
  init_menu(&menu);
  draw_menu(&menu);

  MenuResult result = {.next_state = STATE_MENU};
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

            if (data[0] == ENTER_CODE) {
              config = select_menu_item(&menu);
            }

            if (strcmp(config.difficulty, "") != 0) {
              if (strcmp(config.difficulty, "easy") == 0 || strcmp(config.difficulty, "hard") == 0) {
                result.next_state = STATE_GAME;
                result.config = config;
                return result;
              }
              else if (strcmp(config.difficulty, "exit") == 0) {
                result.next_state = STATE_EXIT;
                return result;
              }
            }

            if (data[1] == FLAG_BREAK) {
              switch (data[0]) {
                case ARROW_UP_CODE:

                  update_menu(&menu, -1);
                  draw_update(&menu);
                  break;
                case ARROW_DOWN_CODE:

                  update_menu(&menu, 1);
                  draw_update(&menu);
                  break;
                default:
                  break;
              }
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
    result.next_state = STATE_EXIT;
  }
  return result;
}

// timer / kbc
int game_state(GameConfig config, uint8_t kbd_irq, uint8_t timer_irq) {

  char *sentence = NULL;

  if (strcmp(config.difficulty, "easy") == 0) {
    sentence = get_random_easy_phrase();
  }
  else if (strcmp(config.difficulty, "hard") == 0) {
    sentence = get_random_hard_phrase();
  }
  else {
    printf("Invalid difficulty selected.\n");
    return ERROR;
  }

  WordPosition word_positions[MAX_WORDS];
  int word_count = map_words_in_sentence(sentence, 50, 100, word_positions, MAX_WORDS);

  Rectangle writing_rectangle = {
    .position = {260, 650},
    .width = 500,
    .height = 100,
    .color = 15};

  WordPosition *input_word = malloc(sizeof(WordPosition));
  if (!input_word) {
    printf("Failed to allocate memory for input_word\n");
    return ERROR;
  }
  input_word->word = calloc(128, sizeof(char)); 
  if (!input_word->word) {
    printf("Failed to allocate memory for input_word->word\n");
    free(input_word);
    return ERROR;
  }
  input_word->coordinates.x = writing_rectangle.position.x/2;
  input_word->coordinates.y = writing_rectangle.position.y/2;

  GameState game_state;
  game_state.writting_rectangle_position = writing_rectangle;
  game_state.word_count = word_count;
  game_state.current_word_index = 0;
  game_state.input = input_word;
  memcpy(game_state.word_positions, word_positions, sizeof(WordPosition) * word_count);

  game_state.correct_words = 0;
  game_state.incorrect_words = 0;
  game_state.time_taken = 0;

  clearScreen();
  refresh_frame_buffer();
  draw_sentence(sentence, 50, 100, "white");
  vg_draw_rectangle(260, 650, 500, 100, 15);

  game_loop(kbd_irq, timer_irq, &config, &game_state);

  end_game(&game_state,&config,kbd_irq, timer_irq);
  
  return OK;
}
