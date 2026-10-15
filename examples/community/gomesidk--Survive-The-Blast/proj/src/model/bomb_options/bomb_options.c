#include "bomb_options.h"

struct bomb_options_imp {
  Sprite* options[3];
  Sprite* selected_option[3];
  uint16_t options_x_initial;
  uint16_t options_x_final;
  uint16_t options_y_initial[3]; 
  uint16_t options_y_final[3];
  uint8_t number_of_options;
  uint8_t cooldown_counter[3]; 
  uint8_t spawn_counter;
  uint8_t spawn_rate;
  uint16_t decrease_spawn_rate_counter; // Counter to decrease spawn rate
  BombType selectedBomb;
};

uint16_t get_options_x_initial(BombOptions *b) {
  if (b == NULL) {
    return 0;
  }
  return b->options_x_initial;
}

uint16_t get_options_x_final(BombOptions *b) {
  if (b == NULL) {
    return 0;
  }
  return b->options_x_final;
}

uint16_t get_options_y_initial(BombOptions *b, int index) {
  if (b == NULL || index < 0 || index > b->number_of_options-1) {
    return 0;
  }
  return b->options_y_initial[index];
}

uint16_t get_options_y_final(BombOptions *b, int index) {
  if (b == NULL || index < 0 || index > b->number_of_options-1) {
    return 0;
  }
  return b->options_y_final[index];
}

uint8_t get_number_of_options(BombOptions *b) {
  if (b == NULL) {
    return 0;
  }
  return b->number_of_options;
}

uint8_t get_spawn_rate(BombOptions *b) {
  if (b == NULL) {
    return 0;
  }
  return b->spawn_rate;
}

uint16_t get_decrease_spawn_rate_counter(BombOptions *b) {
  if (b == NULL) {
    return 0;
  }
  return b->decrease_spawn_rate_counter;
} 

BombOptions *create_bomb_options(Sprite** options, Sprite** selected_option) {

  BombOptions *bomb_options = (BombOptions *) malloc(sizeof(BombOptions));
  if (bomb_options == NULL) {
    return NULL;
  }

  for (int i = 0; i < 3; i++) {
    bomb_options->options[i] = options[i];
    bomb_options->selected_option[i] = selected_option[i];
  }
  uint16_t sprite_width = get_sprite_width(options[0]);
  uint16_t sprite_height = get_sprite_height(options[0]);

  bomb_options->options_x_initial = 10; // Initial x position
  bomb_options->options_x_final = 10 + sprite_width; // Final x position

  bomb_options->options_y_initial[0] = 200; // Initial y position
  bomb_options->options_y_initial[1] = 400;
  bomb_options->options_y_initial[2] = 600;
  
  bomb_options->options_y_final[0] = 200 + sprite_height; // Final y position
  bomb_options->options_y_final[1] = 400 + sprite_height;
  bomb_options->options_y_final[2] = 600 + sprite_height;

  bomb_options->number_of_options = 3; // Number of options

  for (int i = 0; i < bomb_options->number_of_options; i++) {
    bomb_options->cooldown_counter[i] = 0; // Initialize cooldown counter
  }

  bomb_options->spawn_counter = 0; // Initialize spawn counter

  bomb_options->spawn_rate = 60; // Set spawn rate (e.g., every 60 frames)

  bomb_options->selectedBomb = NORMAL; // Default selected bomb type

  bomb_options->decrease_spawn_rate_counter = 300; // Initialize decrease spawn rate counter

  return bomb_options;
}

void destroy_bomb_options(BombOptions *b) {
  if (b == NULL) {
    return;
  }

  free(b);
  b = NULL;
}

void draw_bomb_options(BombOptions *b, int selected_option) {
  if (b == NULL || selected_option < 0 || selected_option > b->number_of_options-1) {
    return;
  }

  for (int i = 0; i < b->number_of_options; i++) {
    if (i == selected_option) {
      draw_sprite(b->selected_option[i], b->options_x_initial, b->options_y_initial[i]);
    } else {
      draw_sprite(b->options[i], b->options_x_initial, b->options_y_initial[i]);
    }
  }
}

void set_bomb_on_cooldown(BombOptions *b, BombType type) {
  if (b == NULL) {
    return;
  }
  switch (type) {
    case NORMAL:
      b->cooldown_counter[0] = 150;
      break;
    case CONSTRUCTIVE:
      b->cooldown_counter[1] = 150;
      break;
    case FULL_LINE:
      b->cooldown_counter[2] = 150;
      break;
    default:
      break;
  }
}

bool is_bomb_off_cooldown(BombOptions *b, BombType type) {
  if (b == NULL) {
    return false;
  }

  switch (type) {
    case NORMAL:
      return b->cooldown_counter[0] == 0;
    case CONSTRUCTIVE:
      return b->cooldown_counter[1] == 0;
    case FULL_LINE:
      return b->cooldown_counter[2] == 0;
    default:
      break;
  }

  return false; // Default case
}

void decrease_cooldown_time(BombOptions *b) {
  if (b == NULL) {
    return;
  }
  for (int i = 0; i < b->number_of_options; i++) {
    if (b->cooldown_counter[i] > 0) {
      b->cooldown_counter[i]--;
    }
  }
}

void decrease_time_until_spawn(BombOptions *b) {
  if (b == NULL) {
    return;
  }

  if (b->spawn_counter > 0) {
    b->spawn_counter--;
  }
}

bool is_spawning(BombOptions *b) {
  if (b == NULL) {
    return false;
  }
  return b->spawn_counter == 0;
}

void reset_time_until_spawn(BombOptions *b) {
  if (b == NULL) {
    return;
  }
  b->spawn_counter = b->spawn_rate; // Reset the spawn rate
}

void decrease_spawn_rate(BombOptions *b) {
  if (b == NULL) {
    return;
  }
  if (b->spawn_rate > 10) { // Minimum spawn rate
    b->spawn_rate -= 5; // Decrease spawn rate
    return;
  }
}

void decrease_decrease_spawn_rate_counter(BombOptions *b) {
  if (b == NULL) {
    return;
  }
  if (b->decrease_spawn_rate_counter > 0) {
    b->decrease_spawn_rate_counter--;
  }
  else {
    b->decrease_spawn_rate_counter = 300; // Reset the counter
    decrease_spawn_rate(b); // Decrease the spawn rate
  }
}

BombType get_random_bomb_type() {
  int r = rand() % 3; // generates 0, 1, or 2
  switch (r) {
    case 0: return NORMAL;
    case 1: return FULL_LINE;
    case 2: return CONSTRUCTIVE;
    default: return NORMAL; // fallback, should not happen
  }
}
