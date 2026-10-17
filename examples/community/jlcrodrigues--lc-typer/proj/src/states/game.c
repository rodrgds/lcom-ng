#include "game.h"

static uint32_t primary_color = PRIMARY_COLOR;
static uint32_t secondary_color = SECONDARY_COLOR;
static uint32_t accent_color = ACCENT_COLOR;

static int base_y;
static int next_y;
static int last_line;

static Button exit_button;
static Button next_button;

static char phrases[LINESIZE];

static int cursor_on = 1;

void game_create(Game* game) {
  char * phrase = phrase_select("/home/lcom/labs/proj/src/assets/phrases/phrases.txt");
  game->player_position = 0;
  game->text = phrase;
  game->text_size = strlen(phrase);
  game->time_elapsed = 0;
  game->typo_offset = 0;
  game->typo_count = 0;
  base_y = TEXT_Y_START;
  last_line = 0;
  exit_button = button_create(video_get_h_res() - FONT_WIDTH, PADDING, "x");
  next_button = button_create(video_get_h_res() -  2 * (FONT_WIDTH + PADDING), PADDING, "n");
}

void game_restart(Game* game) {
  game->player_position = 0;
  game->time_elapsed = 0;
  game->typo_offset = 0;
  game->typo_count = 0;
  base_y = TEXT_Y_START;
  last_line = 0;
}

void game_draw(Game* game) {
  draw_text(game);
  if (game->player_position >= 1)
    draw_wpm(game);
}

void game_handle_event(Game* game, Event* event) {
  if (exit_button.clicked) {
    exit_button.clicked = 0;
    proj_set_state(MENU);
  }
  if (next_button.clicked) {
    next_button.clicked = 0;
    proj_set_state(GAME);
  }
  if (event->type == KEYBOARD) {
    if (game->text[game->player_position] == event->info.keyboard.character
        && !(game->typo_offset)) {
      if (game->player_position == 0) {
        rtc_start_counter();
      }
      game->player_position++;
      if (game->player_position == game->text_size) {
        game->time_elapsed = rtc_get_time_elapsed();
        proj_set_state(GAME_OVER);
      }
    }
    else if (test_letter(event->info.keyboard.character)) {
      game->typo_offset++;
      game->typo_count++;
    }
    else if (event->info.keyboard.buff == MAKECODE_BACKSPACE) {
      game->typo_offset--;
      game->typo_offset = (game->typo_offset < 0) ? 0 : game->typo_offset;
    }
    if (event->info.keyboard.buff == BREAKCODE_ENTER)
      proj_set_state(GAME);
    if (event->info.keyboard.buff == BREAKCODE_ESC)
      proj_set_state(MENU);
    if (event->info.keyboard.buff == BREAKCODE_TAB)
      game_restart(game);
  }
  if (event->type == TIMER) {
    if (!(event->info.timer.count_interrupts % 30)) {
      cursor_on = (cursor_on) ? 0 : 1;
    }
  }
}

void game_step(Game* game, Event* event) {
  button_step(&exit_button, event);
  button_step(&next_button, event);
  game_handle_event(game, event);
  game_draw(game);
}

void draw_text(Game* game) {
  int x_pos = MARGIN;
  if (game->player_position > next_y && !last_line)
    base_y -= LINE_HEIGHT;
  int y_pos = base_y;
  char* text_copy = malloc(strlen(game->text) + 1);
  if (text_copy == NULL) return;
  for (int i = 0; i < game->text_size; i++) {
    if (y_pos >= TEXT_Y_START && y_pos < TEXT_Y_MAX) {
      if (i < game->player_position) {
        draw_char(game->text[i], x_pos, y_pos, primary_color);
      }
      else if (i < game->player_position + game->typo_offset) {
        draw_char(game->text[i], x_pos, y_pos, accent_color);
      }
      else {
        draw_char(game->text[i], x_pos, y_pos, secondary_color);
      }
      
      if (cursor_on) {
        if ((i == game->player_position && game->typo_offset == 0)
          || i == (game->player_position + game->typo_offset)) {
          vg_draw_rectangle(x_pos, y_pos + 5, 5, FONT_HEIGHT - 10, primary_color);
        }
      }
    }
    x_pos += PADDING + FONT_WIDTH;
    if (game->text[i] == ' ') {
      strcpy(text_copy, game->text);
      char* next = strtok(text_copy + i, " ");
      if (x_pos + MARGIN + get_sentence_width(next) > video_get_h_res()) {
        x_pos = MARGIN;
        y_pos += LINE_HEIGHT;
        if (y_pos == TEXT_Y_START + (LINE_HEIGHT << 1))
          next_y = i;
      }
    }
  }
  if (y_pos == TEXT_Y_MAX - LINE_HEIGHT) last_line = 1;
  free(text_copy);
}

void draw_wpm(Game* game) {
  if (rtc_get_time_elapsed() == 0) return;
  int wpm = (int)(
    ((double)(game->player_position) / 5)
     / ((double)(rtc_get_time_elapsed()) / 60) 
    );
  game->wpm = wpm;
  char wpm_str[16];
  snprintf(wpm_str, sizeof(wpm_str), "%d wpm", wpm);
  draw_sentence(wpm_str, MARGIN, MARGIN, PRIMARY_COLOR);
}

char* phrase_select(char* infile){
    FILE *file;
    if((file = fopen(infile,"r"))== NULL){
        perror("cannot open file");
        return NULL;
    }
    int n = rand() % NUMBER_OF_STRINGS;
    size_t str_size = 0;
    char* str = NULL;
    int i = 0;
    while(getline(&str, &str_size, file)> 0){
        if(i==n){
            for (size_t j=0;j<str_size;j++){
                if(str[j]=='\n'){
                    phrases[j] = '\0';
                    break;
                }
                phrases[j] = str[j];
            }
            break;
        }
        i++;
    }
    fclose(file);
    return phrases;
}
