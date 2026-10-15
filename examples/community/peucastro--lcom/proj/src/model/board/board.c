#include <lcom/lcf.h>

#include "model/board/board.h"

/**
 * @brief Converts a character from the board file to a board_element_t
 *
 * @param c Character to convert
 * @return Corresponding board_element_t
 */
static board_element_t(char_to_element)(char c) {
  switch (c) {
    case 'P': return PLAYER;
    case 'E': return ENEMY;
    case 'B': return BRICK;
    case '#': return WALL;
    case 'O': return BOMB;
    case 'U': return POWERUP;
    case 'D': return DOOR;
    case ' ': return EMPTY_SPACE;
    default:
      fprintf(stderr, "char_to_element: invalid char.");
      return EMPTY_SPACE;
  }
}

int(load_board)(GameBoard *board, const char *filename) {
  if (filename == NULL || board == NULL) {
    fprintf(stderr, "load_board: invalid arguments");
    return 1;
  }

  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    return 1;
  }

  reset_board(board);

  uint8_t width = 0;
  uint8_t height = 0;
  char line[MAX_LINE_LENGTH];

  while (fgets(line, MAX_LINE_LENGTH, file) != NULL && height < MAX_BOARD_HEIGHT) {
    size_t len = strlen(line);
    if (len > 0 && (line[len - 1] == '\n')) {
      line[--len] = '\0';
    }
    if (len > 0) {
      if (width == 0) {
        width = len;
        if (width > MAX_BOARD_WIDTH) {
          fprintf(stderr, "load_board: board width (%d) exceeds maximum (%d) in file %s", width, MAX_BOARD_WIDTH, filename);
          fclose(file);
          return 1;
        }
      }
      else if (len != width) {
        fprintf(stderr, "load_board: inconsistent line lengths in file %s", filename);
        fclose(file);
        return 1;
      }
      height++;
    }
  }

  if (width == 0 || height == 0) {
    fprintf(stderr, "load_board: empty or invalid board file %s", filename);
    fclose(file);
    return 1;
  }

  if (height > MAX_BOARD_HEIGHT) {
    fprintf(stderr, "load_board: board height (%d) exceeds maximum (%d) in file %s", height, MAX_BOARD_HEIGHT, filename);
    fclose(file);
    return 1;
  }

  board->width = width;
  board->height = height;

  rewind(file);
  uint8_t row = 0;

  while (fgets(line, MAX_LINE_LENGTH, file) != NULL && row < height) {
    size_t len = strlen(line);
    if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
      line[--len] = '\0';
    }
    if (len > 0) {
      for (uint8_t col = 0; col < width; col++) {
        board->elements[row][col] = char_to_element(line[col]);
      }
      row++;
    }
  }

  fclose(file);
  return 0;
}

int(reset_board)(GameBoard *board) {
  if (board == NULL) {
    fprintf(stderr, "reset_board: board pointer cannot be null");
    return 1;
  }

  board->width = 0;
  board->height = 0;

  for (uint8_t i = 0; i < MAX_BOARD_HEIGHT; i++) {
    for (uint8_t j = 0; j < MAX_BOARD_WIDTH; j++) {
      board->elements[i][j] = EMPTY_SPACE;
    }
  }

  return 0;
}
