#include "score.h"

struct score_imp {
  uint16_t score; // Current score
  uint8_t frames_until_next_increment; // Frames until the next score increment
  Sprite *number_sprites[10]; // Array of sprites for digits 0-9
};

Score *create_score(SpriteLoader *loader) {
  Score *score = (Score *) malloc(sizeof(Score));
  if (score == NULL) {
    return NULL; // Memory allocation failed
  }

  score->score = 0; // Initialize score to 0
  score->frames_until_next_increment = 0; // Initialize frames until next increment to 0
  for (int i = 0; i < 10; i++) {
    score->number_sprites[i] = get_number_sprite(loader, i); // Load sprites for digits 0-9
    if (score->number_sprites[i] == NULL) {
      destroy_score(score); // Clean up if sprite loading fails
      return NULL;
    }
  }

  return score;
}

void destroy_score(Score *score) {
  if (score == NULL) {
    return; // Nothing to destroy
  }

  free(score); // Free the allocated memory for the score
}

void increment_score(Score *score) {
  if (score == NULL) {
    return ;
  }

  if (score->frames_until_next_increment > 0) {
    score->frames_until_next_increment--; 
    return; 
  }

  score->frames_until_next_increment = 30;
  score->score += 1;
  return;
}

uint16_t get_score_value(Score *score) {
  if (score == NULL) {
    return 0;
  }

  return score->score;
}

void draw_score(Score *score, int x, int y) {
  if (score == NULL) {
    return; 
  }

  uint16_t value = score->score;
  if (value == 0) {
    draw_sprite(score->number_sprites[0], x, y);
    return;
  }

  int digits[10];
  int num_digits = 0;
  while (value > 0) {
    digits[num_digits++] = value % 10;
    value /= 10;
  }

  for (int i = 0; i <= num_digits - 1; i++) {
    draw_sprite(score->number_sprites[digits[i]], x, y);
    x -= get_sprite_width(score->number_sprites[digits[i]]);
  }
}
