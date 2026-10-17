#ifndef TETRIMINOS_H_
#define TETRIMINOS_H_

#define kMaxPieceSize 6

/** @defgroup tetriminos tetriminos
 * @{
 * Functions for getting tetriminos.
 */

/// Struct representing a coordinate.
typedef struct kCoord {
    int x;
    int y;
} coordinate;

/// Struct representing a tetrimino.
typedef struct kTetrimino {
    unsigned int color;
    unsigned int width;
    unsigned int ymax;
    unsigned int blocks;
    coordinate coords[kMaxPieceSize];
} tetrimino;

/**
 * @brief Initializes the tetriminos.
 */

void tetriminos_init();

/**
 * @brief Gets the next tetrimino and draws a new one.
 *
 * @return The next tetrimino.
 */

tetrimino tetriminos_get();

/**
 * @brief Gets the next tetrimino without drawing a new one.
 *
 * @return The next tetrimino.
 */

tetrimino tetriminos_next();

/**@}*/

#endif /* TETRIMINOS_H_ */