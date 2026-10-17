#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>

/** @defgroup game game
 * @{
 * Function that has the game
 */
#include "timer.h"
#include "kbd.h"
#include "mouse.h"

#include "i8042.h"
#include "bitmap.h"

#include "utils.h"

#include "snake.h"
#include "fruit.h"

#define mode 0x117

/**
 * @brief Initializes the game
 * @return 0 if succeed
 */
int game();

void printSnakePosition(Snake* snake);

void moveSnake(FILE* fp, Snake* snake, Fruit* fruit, Fruit* specialF);
