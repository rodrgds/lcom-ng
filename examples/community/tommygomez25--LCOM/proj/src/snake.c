#include "snake.h"

snakepart *snake;
int size = 3;
int previousmove = RIGHT;
static uint8_t requestedmove = RIGHT;
int speed = 20;
int score = 0;

extern uint8_t last; /* keyboard last scancode */

void(create_snake)() {
  size=3;
  previousmove=RIGHT;
  requestedmove=RIGHT;
  speed=20;
  score=0;
  snake = malloc(50 * sizeof *snake);
  xpm_load(snakebodyhorizontal, XPM_8_8_8, &snakebodyhorizontal_img);
  xpm_load(snakebodyvertical, XPM_8_8_8, &snakebodyvertical_img);
  xpm_load(snakeheaddown, XPM_8_8_8, &snakeheaddown_img);
  xpm_load(snakeheadright, XPM_8_8_8, &snakeheadright_img);
  xpm_load(snakeheadup, XPM_8_8_8, &snakeheadup_img);
  xpm_load(snakeheadleft, XPM_8_8_8, &snakeheadleft_img);
  xpm_load(snaketaildown, XPM_8_8_8, &snaketaildown_img);
  xpm_load(snaketailleft, XPM_8_8_8, &snaketailleft_img);
  xpm_load(snaketailright, XPM_8_8_8, &snaketailright_img);
  xpm_load(snaketailup, XPM_8_8_8, &snaketailup_img);
  xpm_load(snaketurndownleft, XPM_8_8_8, &snaketurndownleft_img);
  xpm_load(snaketurndownright, XPM_8_8_8, &snaketurndownright_img);
  xpm_load(snaketurnupleft, XPM_8_8_8, &snaketurnupleft_img);
  xpm_load(snaketurnupright, XPM_8_8_8, &snaketurnupright_img);
  create_snake_tail();
  create_snake_body();
  create_snake_head();
}

void(create_snake_tail)() {
  snakepart tail;
  tail.x = 360;
  tail.y = 300;
  tail.img = snaketailleft_img;
  snake[0] = tail;
}

void(create_snake_body)() {
  snakepart body;
  body.x = 380;
  body.y = 300;
  body.img = snakebodyhorizontal_img;
  snake[1] = body;
}

void(create_snake_head)() {
  snakepart head;
  head.x = 400;
  head.y = 300;
  head.img = snakeheadright_img;
  snake[2] = head;
}

void(draw_snake)() {
  for (int i = 0; i < size; i++) {
    draw_xpm(snake[i].img.bytes, &snake[i].img, snake[i].x, snake[i].y);

    /* if is body */
    if (i < size - 1) {
      snake[i].x = snake[i + 1].x;
      snake[i].y = snake[i + 1].y;
    }
  }
}

void(check_snake_apple_collision)(apple *apple1) {

  if (snake[size - 1].x == apple1->x && snake[size - 1].y == apple1->y) { /* if head of the snake coordenates are equal to apple coordenates */
    (score)++;
    printf("score incremented is: %d\n",score);
    snake[size] = snake[size - 1];
    if (last == UP) {
      snake[size].y -= 20;
      snake[size - 1].img = snakebodyvertical_img;
    }
    else if (last == DOWN) {
      snake[size].y += 20;
      snake[size - 1].img = snakebodyvertical_img;
    }
    else if (last == LEFT) {
      snake[size].x -= 20;
      snake[size - 1].img = snakebodyhorizontal_img;
    }
    else if (last == RIGHT){
      snake[size].x += 20;
      snake[size - 1].img = snakebodyhorizontal_img;
    }

    (size)++;
    apple1->x = (rand() % 40) * 20;
    apple1->y = ((rand() % 22) + 8) * 20;
    apple1->counter = 0;
  }
}

void(define_snake_tail_sprite)() {
  if (snake[0].y == snake[1].y) {  /* se y da tail e corpo são iguais , entao é movimento horizontal */
    if (snake[0].x > snake[1].x) { /* se a tail estiver à direita do corpo,  então é movimento para a esquerda */
      snake[0].img = snaketailleft_img;
    }
    else {
      snake[0].img = snaketailright_img;
    }
  }
  else {
    if (snake[0].y > snake[1].y) {
      snake[0].img = snaketailup_img;
    }
    else {
      snake[0].img = snaketaildown_img;
    }
  }
}

void snake_set_direction(uint8_t key) {
  if (key == UP || key == DOWN || key == LEFT || key == RIGHT)
    requestedmove = key;
}

void(snake_update_movement)() {

  if (requestedmove == UP && previousmove != DOWN) {
    snake[size - 1].img = snakeheadup_img;
    snake[size - 1].y -= speed;
    previousmove = UP;
  }
  else if (requestedmove == DOWN && previousmove != UP) {
    snake[size - 1].img = snakeheaddown_img;
    snake[size - 1].y += speed;
    previousmove = DOWN;
  }
  else if (requestedmove == RIGHT && previousmove != LEFT) {
    snake[size - 1].img = snakeheadright_img;
    snake[size - 1].x += speed;
    previousmove = RIGHT;
  }
  else if (requestedmove == LEFT && previousmove != RIGHT) {
    snake[size - 1].img = snakeheadleft_img;
    snake[size - 1].x -= speed;
    previousmove = LEFT;
  }
  else {
    if (previousmove == UP) {
      snake[size - 1].img = snakeheadup_img;
      snake[size - 1].y -= speed;
      previousmove = UP;
    }
    else if (previousmove == DOWN) {
      snake[size - 1].img = snakeheaddown_img;
      snake[size - 1].y += speed;
      previousmove = DOWN;
    }
    else if (previousmove == RIGHT) {
      snake[size - 1].img = snakeheadright_img;
      snake[size - 1].x += speed;
      previousmove = RIGHT;
    }
    else if (previousmove == LEFT) {
      snake[size - 1].img = snakeheadleft_img;
      snake[size - 1].x -= speed;
      previousmove = LEFT;
    }
  }
}

void (define_snake_body_sprite)() {
        for (int i = 1; i < size - 1; i++) {
        if ((snake[i - 1].x > snake[i].x && snake[i].x > snake[i + 1].x) || (snake[i - 1].x < snake[i].x && snake[i].x < snake[i + 1].x)) {
          snake[i].img = snakebodyhorizontal_img;
        }
        else if ((snake[i - 1].y > snake[i].y && snake[i].y > snake[i + 1].y) || (snake[i - 1].y < snake[i].y && snake[i].y < snake[i + 1].y)) {
          snake[i].img = snakebodyvertical_img;
        }
        else if ((snake[i - 1].y > snake[i].y && snake[i - 1].x == snake[i].x && snake[i + 1].y == snake[i].y && snake[i + 1].x > snake[i].x) || (snake[i + 1].y > snake[i].y && snake[i + 1].x == snake[i].x && snake[i - 1].y == snake[i].y && snake[i - 1].x > snake[i].x)) {
          snake[i].img = snaketurnupleft_img;
        }
        else if ((snake[i - 1].y < snake[i].y && snake[i - 1].x == snake[i].x && snake[i + 1].y == snake[i].y && snake[i + 1].x < snake[i].x) || (snake[i + 1].y < snake[i].y && snake[i + 1].x == snake[i].x && snake[i - 1].y == snake[i].y && snake[i - 1].x < snake[i].x)) {
          snake[i].img = snaketurndownright_img;
        }
        else if ((snake[i - 1].y > snake[i].y && snake[i - 1].x == snake[i].x && snake[i + 1].y == snake[i].y && snake[i + 1].x < snake[i].x) || (snake[i + 1].y > snake[i].y && snake[i + 1].x == snake[i].x && snake[i - 1].y == snake[i].y && snake[i - 1].x < snake[i].x)) {
          snake[i].img = snaketurnupright_img;
        }
        else if ((snake[i - 1].y < snake[i].y && snake[i - 1].x == snake[i].x && snake[i + 1].y == snake[i].y && snake[i + 1].x > snake[i].x) || (snake[i + 1].y < snake[i].y && snake[i + 1].x == snake[i].x && snake[i - 1].y == snake[i].y && snake[i - 1].x > snake[i].x)) {
          snake[i].img = snaketurndownleft_img;
        }
      }
}
