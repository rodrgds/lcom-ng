#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tetriminos.h"

#define kTetriminosNum 10

static tetrimino t[kTetriminosNum];

static tetrimino next;

coordinate _makeCoordinate(int x, int y) {
    coordinate coord;
    
    coord.x = x;
    coord.y = y;
    
    return coord;
}

void tetriminos_init() {
    srand(time(NULL));

    /*  Tetrimino O */

    tetrimino t_o = {
        .color = /*yellow*/2,
        .width = 2,
        .ymax = 1,
        .blocks = 4,
        .coords = {
            {.x = 0, .y = 0},
            {.x = 0, .y = 1},
            {.x = 1, .y = 0},
            {.x = 1, .y = 1}
        }
    };

    /*  Tetrimino I */

    tetrimino t_i = {
        .color = /*light blue*/3,
        .width = 1,
        .ymax = 3,
        .blocks = 4,
        .coords = {
            {.x = 0, .y = 0},
            {.x = 0, .y = 1},
            {.x = 0, .y = 2},
            {.x = 0, .y = 3}
        }
    };

    /*  Tetrimino S */

    tetrimino t_s = {
        .color = /*red*/4,
        .width = 3,
        .ymax = 1,
        .blocks = 4,
        .coords = {
            {.x = 0, .y = 0},
            {.x = 1, .y = 0},
            {.x = 1, .y = 1},
            {.x = 2, .y = 1}
        }
    };

    /*  Tetrimino Z */

    tetrimino t_z = {
        .color = /*dark green*/5,
        .width = 3,
        .ymax = 1,
        .blocks = 4,
        .coords = {
            {.x = 0, .y = 1},
            {.x = 1, .y = 1},
            {.x = 1, .y = 0},
            {.x = 2, .y = 0}
        }
    };

    /*  Tetrimino L */

    tetrimino t_l = {
        .color = /*orange*/6,
        .width = 2,
        .ymax = 2,
        .blocks = 4,
        .coords = {
            {.x = 0, .y = 2},
            {.x = 0, .y = 1},
            {.x = 0, .y = 0},
            {.x = 1, .y = 0}
        }
    };

    /*  Tetrimino J */

    tetrimino t_j = {
        .color = /*pink*/7,
        .width = 2,
        .ymax = 2,
        .blocks = 4,
        .coords = {
            {.x = 0, .y = 0},
            {.x = 1, .y = 0},
            {.x = 1, .y = 1},
            {.x = 1, .y = 2}
        }
    };

    /*  Tetrimino T */

    tetrimino t_t = {
        .color = /*purple*/8,
        .width = 3,
        .ymax = 1,
        .blocks = 4,
        .coords = {
            {.x = 0, .y = 1},
            {.x = 1, .y = 1},
            {.x = 2, .y = 1},
            {.x = 1, .y = 0}
        }
    };
    
    /*  Tetromino Block */
    
    tetrimino t_b = {
        .color = /*something*/9,
        .width = 1,
        .ymax = 0,
        .blocks = 1,
        .coords = {
            {.x = 0, .y = 0}
        }
    };
    
    tetrimino t_f = {
        .color = 10,
        .width = 3,
        .ymax = 2,
        .blocks = 5,
        .coords = {
            {.x = 0, .y = 1},
            {.x = 1, .y = 0},
            {.x = 1, .y = 1},
            {.x = 1, .y = 2},
            {.x = 2, .y = 2}
        }
    };
    
    tetrimino t_p = {
        .color = 11,
        .width = 2,
        .ymax = 2,
        .blocks = 5,
        .coords = {
            {.x = 0, .y = 0},
            {.x = 0, .y = 1},
            {.x = 1, .y = 0},
            {.x = 1, .y = 1},
            {.x = 1, .y = 2}
        }
    };
    
    t[0] = t_o;
    t[1] = t_i;
    t[2] = t_s;
    t[3] = t_z;
    t[4] = t_l;
    t[5] = t_j;
    t[6] = t_t;
    t[7] = t_b;
    t[8] = t_f;
    t[9] = t_p;
    
    next = tetriminos_get();
}

tetrimino tetriminos_get() {
    tetrimino ret_hold = next;
    
    next = t[rand() % kTetriminosNum];
    
    return ret_hold;
}

tetrimino tetriminos_next() {
    return next;
}
