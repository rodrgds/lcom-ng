#include "maze.h"

/**
 * @brief Represents a single Maze instance.
 *
 * The Maze struct is opaque.
 */
struct Maze {
    uint8_t width;       /**< Width of the maze in cells */
    uint8_t height;      /**< Height of the maze in cells */
    maze_entity **cells; /**< 2D array of maze entities representing the maze structure */
    uint8_t mob_count;   /**< Number of active mobs in the maze */
    AnimatedSprite *key; /**< Pointer to the key sprite */
};

/* Create and destroy section */

static int shuffle(int *arr, int n) {
    if (arr == NULL) {
        printf("shuffle: NULL pointer provided\n");
        return 1;
    }

    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
    return 0;
}

static int dfs(Maze *maze, int x, int y) {
    if (maze == NULL) {
        printf("dfs: NULL pointer provided\n");
        return 1;
    }

    maze->cells[y][x] = EMPTY;

    int dx[] = {2, -2, 0, 0};
    int dy[] = {0, 0, 2, -2};
    int dirs[] = {0, 1, 2, 3};
    if (shuffle(dirs, 4)) {
        printf("maze dfs: suffle failed\n");
        return 1;
    }

    for (int i = 0; i < 4; i++) {
        int dir = dirs[i];
        int new_x = x + dx[dir];
        int new_y = y + dy[dir];

        if (new_x > 0 && new_x < maze->width - 1 && new_y > 0 && new_y < maze->height - 1 && maze->cells[new_y][new_x] == WALL) { // Position inside the maze and there is a wall
            maze->cells[y + dy[dir] / 2][x + dx[dir] / 2] = EMPTY;                                                                // Remove wall
            if (dfs(maze, new_x, new_y)) {
                printf("maze dfs: failed to find path\n");
                return 1;
            }
        }
    }
    return 0;
}

static int initialize_maze(Maze *maze, uint8_t width, uint8_t height) {
    if (maze == NULL) {
        printf("dfs: NULL pointer provided\n");
        return 1;
    }

    uint8_t w = width > x_res / CELL_SIZE ? x_res / CELL_SIZE : width;
    uint8_t h = height > y_res / CELL_SIZE ? y_res / CELL_SIZE : height;

    w -= (w % 2 == 0);
    h -= (h % 2 == 0);

    maze->width = w;
    maze->height = h;

    maze_entity **mz = (maze_entity **) malloc(maze->height * sizeof(maze_entity *));
    if (mz == NULL) {
        printf("initialize_maze: malloc failed\n");
        return 1;
    }

    for (uint16_t i = 0; i < maze->height; i++) {
        mz[i] = (maze_entity *) malloc(maze->width * sizeof(maze_entity));

        if (mz[i] == NULL) {
            printf("initialize_maze: malloc failed\n");
            for (int16_t j = i - 1; j >= 0; j--) {
                free(mz[j]);
            }
            free(mz);
            return 1;
        }

        for (int j = 0; j < maze->width; j++) {
            mz[i][j] = WALL;
        }
    }

    maze->cells = mz;
    if (dfs(maze, 1, 1)) {
        printf("initialize_maze: dfs failed\n");
        return 1;
    }
    mz[maze->height - 2][maze->width - 2] = WIN;

    return 0;
}

static int generate_mob_positions(Maze *maze, uint8_t mob_count) {
    if (maze == NULL) {
        printf("generate_mob_positions: NULL pointer provided\n");
        return 1;
    }

    int positions = 0, attempts = 0;
    while (positions < mob_count && attempts < MAX_ATTEMPTS) {
        attempts++;
        int i = rand() % maze->width;
        int j = rand() % maze->height;
        int manhattan_distance = i - 1 + j - 1;
        if (i != 1 && j != 1 && maze->cells[j][i] == EMPTY && manhattan_distance > NO_MONSTER_SQUARE) {
            positions++;
            maze->cells[j][i] = MOB;
        }
    }
    maze->mob_count = mob_count;

    return 0;
}

static int open_maze(Maze *maze, uint8_t percentage) {
    if (maze == NULL) {
        printf("open_maze: NULL pointer provided\n");
        return 1;
    }

    uint16_t wall_area = 2 * (maze->width / 2 + 1) * (maze->height / 2 + 1) - 1; // 2 times O - 1 (V + E on spanning tree on a graph that passes through every other position)
    uint16_t visited = 0;
    uint16_t attempts = 0;

    while (visited < wall_area * percentage / 100.0 && attempts < MAX_ATTEMPTS) {
        int random_x = 1 + rand() % (maze->width - 2);  // Leave borders untouched
        int random_y = 1 + rand() % (maze->height - 2); // Leave borders untouched

        if (maze->cells[random_y][random_x] == WALL) {
            maze->cells[random_y][random_x] = EMPTY;
            visited++;
        }

        attempts++;
    }

    return 0;
}

Maze *create_maze(uint8_t width, uint8_t height, uint8_t mob_count) {
    Maze *maze = (Maze *) malloc(sizeof(Maze));
    if (maze == NULL) {
        printf("create_maze: malloc failed\n");
        return NULL;
    }

    if (initialize_maze(maze, width, height)) {
        printf("create_maze: initialize_maze failed");
        free(maze);
        return NULL;
    }

    if (open_maze(maze, MAZE_OPENING_PERCENTAGE)) {
        printf("create_maze: open_maze failed");
        free(maze);
        return NULL;
    }

    if (generate_mob_positions(maze, mob_count)) {
        printf("create_maze: generate_mob_positions failed");
        free(maze);
        return NULL;
    }

    Sprite *key_sprite = create_sprite((xpm_map_t) key_1, 0, 0, 0, 0);
    maze->key = create_animated_sprite(key_sprite, 25, 4,
                                       (xpm_map_t) key_2,
                                       (xpm_map_t) key_1,
                                       (xpm_map_t) key_3);

    return maze;
}

int destroy_maze(Maze *maze) {
    if (maze == NULL) {
        printf("destroy_maze: NULL pointer provided\n");
        return 1;
    }

    for (int i = 0; i < maze->height; i++) {
        free(maze->cells[i]);
    }
    free(maze->cells);

    free(maze);
    return 0;
}

/* Getter and setter section */

uint8_t get_width(Maze *maze) {
    if (maze == NULL) {
        printf("get_width: NULL pointer provided\n");
        return 0;
    }

    return maze->width;
}

uint8_t get_height(Maze *maze) {
    if (maze == NULL) {
        printf("get_height: NULL pointer provided\n");
        return 0;
    }

    return maze->height;
}

uint8_t get_mob_count(Maze *maze) {
    if (maze == NULL) {
        printf("get_mob_count: NULL pointer provided\n");
        return 0;
    }

    return maze->mob_count;
}

void set_mob_count(Maze *maze, uint8_t mob_count) {
    if (maze == NULL) {
        printf("set_mob_count: NULL pointer provided\n");
        return;
    }

    maze->mob_count = mob_count;
}

Sprite *get_key_sprite(Maze *maze) {
    if (maze == NULL) {
        printf("get_key_sprite: NULL pointer provided\n");
        return NULL;
    }

    return maze->key->sprite;
}

/* Statics section */

/* Others section */

Point **get_mob_positions(Maze *maze) {
    if (maze == NULL) {
        printf("get_mob_positions: NULL pointer provided\n");
        return NULL;
    }

    int point_no = 0;
    Point **points = malloc(sizeof(Point *) * maze->mob_count);
    if (points == NULL) {
        printf("get_mob_positions: malloc failed");
        return NULL;
    }

    for (int j = 0; j < maze->height; j++) {
        for (int i = 0; i < maze->width; i++) {
            if (maze->cells[j][i] == MOB) {
                Point *p = malloc(sizeof(Point));
                if (p == NULL) {
                    printf("get_mob_positions: malloc failed");
                    for (int k = 0; k < point_no; k++) {
                        free(points[k]);
                    }
                    return NULL;
                }

                p->x = i;
                p->y = j;
                points[point_no] = p;
                point_no++;
            }
        }
    }

    return points;
}

bool check_rectangle_collision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2) {
    return !(
        x1 + width1 < x2 ||  // a to the left of b
        x1 > x2 + width2 ||  // b to the left of a
        y1 + height1 < y2 || // in normal people coordinates, a above b
        y1 > y2 + height2    // in normal people coordinates, b above a
    );
}

bool check_sprite_collision(Sprite *a, Sprite *b) {
    if (a == NULL || b == NULL) {
        printf("check_sprite_collision: NULL pointer provided\n");
        return false;
    }

    return check_rectangle_collision(a->x, a->y, a->width, a->height, b->x, b->y, b->width, b->height);
}

bool check_wall_collision(Maze *maze, Sprite *sprite) {
    if (maze == NULL || sprite == NULL) {
        printf("check_wall_collision: NULL pointer provided.\n");
        return false;
    }

    double scaled_down_x = sprite->x / CELL_SIZE;
    double scaled_down_y = sprite->y / CELL_SIZE;

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            int x = scaled_down_x + i;
            int y = scaled_down_y + j;
            if (x >= 0 && x < maze->width &&
                y >= 0 && y < maze->height &&
                maze->cells[y][x] == WALL) {
                if (check_rectangle_collision(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, sprite->x, sprite->y, sprite->width, sprite->height)) {
                    return true;
                }
            }
        }
    }

    return false;
}

/* Draw section */

int draw_maze(Maze *maze, uint8_t *frame_buffer) {
    if (maze == NULL || frame_buffer == NULL) {
        printf("Error: Maze or frame buffer is NULL\n");
        return 1;
    }

    xpm_image_t wall;
    uint8_t *data = xpm_load(wall_xpm, XPM_INDEXED, &wall);
    if (data == NULL) {
        printf("draw_maze: xpm_load failed\n");
        return 1;
    }

    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            if (maze->cells[y][x] == WALL) {
                if (vga_draw_loaded_xpm(data, x * CELL_SIZE, y * CELL_SIZE, wall.width, wall.height, frame_buffer)) {
                    printf("draw_maze: vga_draw_loaded_xpm failed\n");
                    return 1;
                }
            } else if (maze->cells[y][x] == WIN) {
                maze->key->sprite->x = x * CELL_SIZE + (CELL_SIZE - maze->key->sprite->width) / 2;
                maze->key->sprite->y = y * CELL_SIZE + (CELL_SIZE - maze->key->sprite->height) / 2;
                if (draw_animated_sprite(maze->key, frame_buffer)) {
                    printf("draw_maze: draw_animated_sprite failed\n");
                    return 1;
                }
            }
        }
    }

    free(data);
    return 0;
}

int draw_maze_outer(Maze *maze, uint8_t *frame_buffer) {
    if (maze == NULL || frame_buffer == NULL) {
        printf("draw_maze_outer: NULL pointer provided\n");
        return 1;
    }

    xpm_image_t wall;
    uint8_t *data = xpm_load(wall_xpm, XPM_INDEXED, &wall);
    if (data == NULL) {
        printf("draw_maze_outer: xpm_load failed\n");
        return 1;
    }

    for (int y = 0; y < y_res / CELL_SIZE; y++) {
        for (int x = 0; x < x_res / CELL_SIZE; x++) {
            if (y < maze->height && x < maze->width) {
                continue; // Inside maze, already drawn
            }
            if (vga_draw_loaded_xpm(data, x * CELL_SIZE, y * CELL_SIZE, wall.width, wall.height, frame_buffer)) {
                printf("draw_maze_outer: vga_draw_loaded_xpm failed\n");
                return 1;
            }
        }
    }

    free(data);

    return 0;
}
