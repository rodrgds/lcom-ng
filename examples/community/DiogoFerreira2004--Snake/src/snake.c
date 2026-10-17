#include "snake.h"
#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

extern vbe_mode_info_t vmi;
extern uint8_t scancode;

Game game;
uint8_t kbd_irq_set;
uint8_t timer_irq_set;

int(proj_main_loop)() {
  if (vg_init(INDEXED_MODE) == NULL) {
    return 1;
  }

  if (kbd_subscribe_int(&kbd_irq_set) != 0) {
    vg_exit();
    return 1;
  }

  if (timer_subscribe_int(&timer_irq_set) != 0) {
    kbd_unsubscribe_int();
    vg_exit();
    return 1;
  }

  if (snake_init() != 0) {
    timer_unsubscribe_int();
    kbd_unsubscribe_int();
    vg_exit();
    return 1;
  }

  int ipc_status;
  message msg;

  snake_draw_menu();

  while (game.state != EXIT) {
    int r = driver_receive(ANY, &msg, &ipc_status);
    if (r != 0) {
      continue;
    }

    switch (game.state) {
      case MENU:
        game.counter_end = 0;
        if (is_ipc_notify(ipc_status)) {
          if (msg.m_notify.interrupts & kbd_irq_set) {
            kbc_ih();
            bool menu_changed = false;

            if (scancode == BREAK_ESC) {
              game.state = EXIT;
            }
            else if (scancode == KEY_ENTER) {
              if (game.start_button.selected) {
                game.state = MODE_SELECT;
                draw_static_mode_select();
                snake_draw_mode_select();
              }
              else if (game.rules_button.selected) {
                game.state = RULES;
                snake_draw_rules();
              }
              else if (game.exit_button.selected) {
                game.state = EXIT;
              }
            }
            else if (scancode == KEY_UP) {
              menu_changed = true;
              if (game.start_button.selected) {
                game.start_button.selected = false;
                game.exit_button.selected = true;
              }
              else if (game.rules_button.selected) {
                game.rules_button.selected = false;
                game.start_button.selected = true;
              }
              else if (game.exit_button.selected) {
                game.exit_button.selected = false;
                game.rules_button.selected = true;
              }
            }
            else if (scancode == KEY_DOWN) {
              menu_changed = true;
              if (game.start_button.selected) {
                game.start_button.selected = false;
                game.rules_button.selected = true;
              }
              else if (game.rules_button.selected) {
                game.rules_button.selected = false;
                game.exit_button.selected = true;
              }
              else if (game.exit_button.selected) {
                game.exit_button.selected = false;
                game.start_button.selected = true;
              }
            }

            if (menu_changed) {
              snake_draw_menu();
            }
          }
          else if (msg.m_notify.interrupts & timer_irq_set) {
            timer_int_handler();
          }
        }
        break;
      case MODE_SELECT:
        if (is_ipc_notify(ipc_status)) {
          if (msg.m_notify.interrupts & kbd_irq_set) {
            kbc_ih();
            if (scancode == KEY_Q) {
              game.state = MENU;
              game.start_button.selected = true;
              game.rules_button.selected = false;
              game.exit_button.selected = false;
              draw_static_menu();
              snake_draw_menu();
            }
            else if (scancode == KEY_ENTER) {
              if (game.classic_mode_button.selected) {
                game.selected_modes = CLASSIC;
              }
              else if (game.walls_mode_button.selected) {
                game.selected_modes = WALLS;
              }
              else if (game.swap_arrows_button.selected) {
                game.selected_modes = SWAP_ARROWS;
              }
              else if (game.fruit_power_button.selected) {
                game.selected_modes = FRUIT_POWER;
              }
              snake_reset();
              game.state = PLAYING;
              snake_draw_board();
            }
            else if (scancode == KEY_UP) {
              if (game.classic_mode_button.selected) {
                game.classic_mode_button.selected = false;
                game.fruit_power_button.selected = true;
              }
              else if (game.walls_mode_button.selected) {
                game.walls_mode_button.selected = false;
                game.classic_mode_button.selected = true;
              }
              else if (game.swap_arrows_button.selected) {
                game.swap_arrows_button.selected = false;
                game.walls_mode_button.selected = true;
              }
              else if (game.fruit_power_button.selected) {
                game.fruit_power_button.selected = false;
                game.swap_arrows_button.selected = true;
              }
              snake_draw_mode_select();
            }
            else if (scancode == KEY_DOWN) {
              if (game.classic_mode_button.selected) {
                game.classic_mode_button.selected = false;
                game.walls_mode_button.selected = true;
              }
              else if (game.walls_mode_button.selected) {
                game.walls_mode_button.selected = false;
                game.swap_arrows_button.selected = true;
              }
              else if (game.swap_arrows_button.selected) {
                game.swap_arrows_button.selected = false;
                game.fruit_power_button.selected = true;
              }
              else if (game.fruit_power_button.selected) {
                game.fruit_power_button.selected = false;
                game.classic_mode_button.selected = true;
              }
              snake_draw_mode_select();
            }
          }
        }
        break;
      case PLAYING:
        if (is_ipc_notify(ipc_status)) {
          if (msg.m_notify.interrupts & kbd_irq_set) {
            kbc_ih();
            if (game.selected_modes == SWAP_ARROWS) {
              if ((scancode == KEY_UP || scancode == KEY_W) && game.direction != UP) {
                game.next_direction = DOWN;
              }
              else if ((scancode == KEY_DOWN || scancode == KEY_S) && game.direction != DOWN) {
                game.next_direction = UP;
                }
                else if ((scancode == KEY_LEFT || scancode == KEY_A) && game.direction != LEFT) {
                game.next_direction = RIGHT;
                  }
                  else if ((scancode == KEY_RIGHT || scancode == KEY_D) && game.direction != RIGHT) {
                game.next_direction = LEFT;
                    }
              else if (scancode == BREAK_ESC) {
                game.state = MENU;
                draw_static_menu();
                snake_draw_menu();
              }
            }
            else {
              if ((scancode == KEY_UP || scancode == KEY_W) && game.direction != DOWN) {
                game.next_direction = UP;
              }
              else if ((scancode == KEY_DOWN || scancode == KEY_S) && game.direction != UP) {
                game.next_direction = DOWN;
              }
              else if ((scancode == KEY_LEFT || scancode == KEY_A) && game.direction != RIGHT) {
                game.next_direction = LEFT;
              }
              else if ((scancode == KEY_RIGHT || scancode == KEY_D) && game.direction != LEFT) {
                game.next_direction = RIGHT;
              }
              else if (scancode == BREAK_ESC) {
                game.state = MENU;
                draw_static_menu();
                snake_draw_menu();
              }
            }
          }

          if (msg.m_notify.interrupts & timer_irq_set) {
            timer_int_handler();
            game.counter++;
            game.food_timer++;

            if (game.selected_modes == FRUIT_POWER && game.extra_speed_timer > 0) {
              game.extra_speed_timer--;
              if (game.extra_speed_timer == 0) {
                game.speed = GAME_SPEED;
              }
            }

            if (game.food_timer >= game.food_lifetime) {
              snake_spawn_food();
              game.food_timer = 0;
            }

            if (game.counter >= 60 / game.speed) {
              game.counter = 0;
              snake_move();
              bool game_over = snake_check_collision();

              if (game_over) {
                game.state = END;
                vg_clear();
                snake_draw_end();
                game.board_needs_full_redraw = true;
              }
              else {
                if (game.state == PLAYING) {
                  snake_draw_board();
                }
              }
            }
          }
        }
        break;
      case RULES:
        if (is_ipc_notify(ipc_status)) {
          if (msg.m_notify.interrupts & kbd_irq_set) {
            kbc_ih();
            if (scancode == KEY_Q) {
              game.state = MENU;
              draw_static_menu();
              snake_draw_menu();
            }
          }
        }
        break;
      case END:
        if (is_ipc_notify(ipc_status)) {
          if (msg.m_notify.interrupts & timer_irq_set) {
            timer_int_handler();
            game.counter_end++;
            if (game.counter_end >= 180) {
              game.state = MENU;
              game.start_button.selected = true;
              game.rules_button.selected = false;
              game.exit_button.selected = false;
              draw_static_menu();
              snake_draw_menu();
            }
          }
          if (msg.m_notify.interrupts & kbd_irq_set) {
            kbc_ih();
          }
        }
        break;

      case EXIT:
      break;
      default:
        break;
    }
  }
  vg_clear();
  timer_unsubscribe_int();
  kbd_unsubscribe_int();
  vg_exit();
  return 0;
}

int snake_init() {
  game.state = MENU;
  draw_static_menu();
  game.score = 0;
  game.speed = GAME_SPEED;
  game.board_needs_full_redraw = true;

  game.start_button.x = vmi.XResolution / 2 - 100;
  game.start_button.y = vmi.YResolution / 2 - 50;
  game.start_button.width = 200;
  game.start_button.height = 40;
  game.start_button.text = "Start";
  game.start_button.selected = true;

  game.rules_button.x = vmi.XResolution / 2 - 100;
  game.rules_button.y = vmi.YResolution / 2 + 70;
  game.rules_button.width = 200;
  game.rules_button.height = 40;
  game.rules_button.text = "Rules";
  game.rules_button.selected = false;

  game.exit_button.x = vmi.XResolution / 2 - 100;
  game.exit_button.y = vmi.YResolution / 2 + 200;
  game.exit_button.width = 200;
  game.exit_button.height = 40;
  game.exit_button.text = "Exit";
  game.exit_button.selected = false;

  int mode_x = vmi.XResolution / 2 - 288/2;
  int mode_y = vmi.YResolution / 2 - 100;
  game.classic_mode_button.x = mode_x;
  game.classic_mode_button.y = mode_y;
  game.classic_mode_button.width = 200;
  game.classic_mode_button.height = 40;
  game.classic_mode_button.text = "Classic";
  game.classic_mode_button.selected = true;

  game.walls_mode_button.x = mode_x;
  game.walls_mode_button.y = mode_y + 100;
  game.walls_mode_button.width = 200;
  game.walls_mode_button.height = 40;
  game.walls_mode_button.text = "Walls";
  game.walls_mode_button.selected = false;

  game.swap_arrows_button.x = mode_x;
  game.swap_arrows_button.y = mode_y + 200;
  game.swap_arrows_button.width = 200;
  game.swap_arrows_button.height = 40;
  game.swap_arrows_button.text = "Swap Arrows";
  game.swap_arrows_button.selected = false;

  game.fruit_power_button.x = mode_x;
  game.fruit_power_button.y = mode_y + 300;
  game.fruit_power_button.width = 200;
  game.fruit_power_button.height = 40;
  game.fruit_power_button.text = "Fruit Power";
  game.fruit_power_button.selected = false;

  srand(time(NULL));

  game.food_lifetime = 500;
  game.food_timer = 0;
  game.wall_count = 0;
  game.fruit_type = NORMAL;
  game.extra_speed_timer = 0;
  game.counter_end = 0;

  snake_reset();
  return 0;
}

void draw_static_menu(){
  vg_clear();
  int title_x = (vmi.XResolution / 2 - 336);
  int title_y = 100;
  our_vg_draw_rectangle_implementation(0, 0, vmi.XResolution, vmi.YResolution, 63);
  draw_xpm_image(snake_title, title_x, title_y);
}

void snake_draw_menu() {

  if (game.start_button.selected) {
    draw_xpm_image(start_button_selected, game.start_button.x, game.start_button.y);
  }
  else {
    draw_xpm_image(start_button, game.start_button.x, game.start_button.y);
  }

  if (game.rules_button.selected) {
    draw_xpm_image(rules_button_selected, game.rules_button.x, game.rules_button.y);
  }
  else {
    draw_xpm_image(rules_button, game.rules_button.x, game.rules_button.y);
  }

  if (game.exit_button.selected) {
    draw_xpm_image(exit_button_selected, game.exit_button.x, game.exit_button.y);
  }
  else {
    draw_xpm_image(exit_button, game.exit_button.x, game.exit_button.y);
  }
}

void draw_static_mode_select() {
  vg_clear();
  our_vg_draw_rectangle_implementation(0, 0, vmi.XResolution, vmi.YResolution, 63);
  int title_x = (vmi.XResolution / 2) - 288/2;
  int title_y = 100;
  draw_xpm_image(mode_select_title, title_x, title_y);
}


void snake_draw_mode_select() {

  if (game.classic_mode_button.selected) {
    draw_xpm_image(classic_button_selected, game.classic_mode_button.x, game.classic_mode_button.y);
  }
  else {
    draw_xpm_image(classic_button, game.classic_mode_button.x, game.classic_mode_button.y);
  }

  if (game.walls_mode_button.selected) {
    draw_xpm_image(walls_button_selected, game.walls_mode_button.x, game.walls_mode_button.y);
  }
  else {
    draw_xpm_image(walls_button, game.walls_mode_button.x, game.walls_mode_button.y);
  }

  if (game.swap_arrows_button.selected) {
    draw_xpm_image(swap_arrows_button_selected, game.swap_arrows_button.x, game.swap_arrows_button.y);
  }
  else {
    draw_xpm_image(swap_arrows_button, game.swap_arrows_button.x, game.swap_arrows_button.y);
  }

  if (game.fruit_power_button.selected) {
    draw_xpm_image(fruit_power_button_selected, game.fruit_power_button.x, game.fruit_power_button.y);
  }
  else {
    draw_xpm_image(fruit_power_button, game.fruit_power_button.x, game.fruit_power_button.y);
  }
}

void snake_reset() {
  game.speed = GAME_SPEED;
  game.snake_size = SNAKE_INITIAL_SIZE;
  game.direction = RIGHT;
  game.next_direction = RIGHT;
  game.score = 0;
  game.counter = 0;
  game.board_needs_full_redraw = true;
  game.fruit_type = NORMAL;
  game.extra_speed_timer = 0;

  for (int i = 0; i < SNAKE_INITIAL_SIZE; i++) {
    game.snake[i].position.x = GRID_WIDTH / 2 - i;
    game.snake[i].position.y = GRID_HEIGHT / 2;
  }

  game.food_timer = 0;
  game.wall_count = 0;
  snake_spawn_food();
}

void snake_spawn_food() {
  bool valid_position;
  do {
    valid_position = true;
    game.food.x = rand() % GRID_WIDTH;
    game.food.y = rand() % GRID_HEIGHT;

    for (int i = 0; i < game.snake_size; i++) {
      if (game.snake[i].position.x == game.food.x &&
          game.snake[i].position.y == game.food.y) {
        valid_position = false;
        break;
      }
    }
    if (!valid_position)
      continue;

    for (int i = 0; i < game.wall_count; i++) {
      if (game.walls[i].x == game.food.x &&
          game.walls[i].y == game.food.y) {
        valid_position = false;
        break;
      }
    }
  } while (!valid_position);

  if (game.selected_modes == FRUIT_POWER) {
    int r = rand() % 10;
    if (r < 7)
      game.fruit_type = NORMAL;
    else if (r < 9)
      game.fruit_type = SPEED;
    else
      game.fruit_type = SIZE;
  }
  else {
    game.fruit_type = NORMAL;
  }
}

void snake_draw_rules() {
  vg_clear();
  our_vg_draw_rectangle_implementation(0, 0, vmi.XResolution, vmi.YResolution, 63);
  draw_xpm_image(rules_page, 100, 256);
}

void snake_draw_background(int board_x, int board_y) {
  our_vg_draw_rectangle_implementation(
    0, 0,
    vmi.XResolution,
    vmi.YResolution,
    62);

  int border_width = CELL_SIZE * 1.3;
  int grid_width_px = GRID_WIDTH * CELL_SIZE;
  int grid_height_px = GRID_HEIGHT * CELL_SIZE;

  our_vg_draw_rectangle_implementation(
    board_x - border_width,
    board_y - border_width,
    grid_width_px + border_width * 2,
    border_width,
    12);

  our_vg_draw_rectangle_implementation(
    board_x - border_width,
    board_y + grid_height_px,
    grid_width_px + border_width * 2,
    border_width,
    12);

  our_vg_draw_rectangle_implementation(
    board_x - border_width,
    board_y - border_width,
    border_width,
    grid_height_px + border_width * 2,
    12);

  our_vg_draw_rectangle_implementation(
    board_x + grid_width_px,
    board_y - border_width,
    border_width,
    grid_height_px + border_width * 2,
    12);
}

void snake_draw_board() {
  static Position prev_snake_tail = {-1, -1};
  static Position prev_food = {-1, -1};

  int board_x = (vmi.XResolution - (GRID_WIDTH * CELL_SIZE)) / 2;
  int board_y = (vmi.YResolution - (GRID_HEIGHT * CELL_SIZE)) / 2;

  if (game.board_needs_full_redraw) {
    vg_clear();
    snake_draw_background(board_x, board_y);
    for (int y = 0; y < GRID_HEIGHT; y++) {
      for (int x = 0; x < GRID_WIDTH; x++) {
        uint32_t color = ((x + y) % 2 == 0) ? 10 : 18;
        our_vg_draw_rectangle_implementation(
          board_x + x * CELL_SIZE,
          board_y + y * CELL_SIZE,
          CELL_SIZE,
          CELL_SIZE,
          color);
      }
    }
    game.board_needs_full_redraw = false;
    prev_snake_tail = game.snake[game.snake_size - 1].position;
    prev_food = game.food;                                     
  }

  if (prev_snake_tail.x != -1 && game.snake_size > 0 &&
      (prev_snake_tail.x != game.snake[game.snake_size - 1].position.x || prev_snake_tail.y != game.snake[game.snake_size - 1].position.y || game.board_needs_full_redraw == false)) {
    uint32_t bg_color = ((prev_snake_tail.x + prev_snake_tail.y) % 2 == 0) ? 10 : 18;
    our_vg_draw_rectangle_implementation(
      board_x + prev_snake_tail.x * CELL_SIZE,
      board_y + prev_snake_tail.y * CELL_SIZE,
      CELL_SIZE,
      CELL_SIZE,
      bg_color);
  }

  if (prev_food.x != -1 && (prev_food.x != game.food.x || prev_food.y != game.food.y)) {
    uint32_t bg_color = ((prev_food.x + prev_food.y) % 2 == 0) ? 10 : 18;
    our_vg_draw_rectangle_implementation(
      board_x + prev_food.x * CELL_SIZE,
      board_y + prev_food.y * CELL_SIZE,
      CELL_SIZE,
      CELL_SIZE,
      bg_color);
  }

  for (int i = 0; i < game.snake_size; i++) {
    uint32_t snake_color = (i == 0) ? 17 : 25;
    if (game.snake[i].position.x == 0 && game.snake[i].position.y == 0 && i == game.snake_size - 1 && game.snake_size > 1) {
      continue;
    }
    our_vg_draw_rectangle_implementation(
      board_x + game.snake[i].position.x * CELL_SIZE,
      board_y + game.snake[i].position.y * CELL_SIZE,
      CELL_SIZE,
      CELL_SIZE,
      snake_color);
  }

  int fruit_color = 4;
  if (game.selected_modes == FRUIT_POWER) {
    if (game.fruit_type == SPEED)
      fruit_color = 62;
    else if (game.fruit_type == SIZE)
      fruit_color = 0;
    else
      fruit_color = 4;
  }
  our_vg_draw_rectangle_implementation(
    board_x + game.food.x * CELL_SIZE,
    board_y + game.food.y * CELL_SIZE,
    CELL_SIZE,
    CELL_SIZE,
    fruit_color);

  if (game.selected_modes == WALLS && game.wall_count > 0) {
    for (int i = 0; i < game.wall_count; i++) {
      our_vg_draw_rectangle_implementation(
        board_x + game.walls[i].x * CELL_SIZE,
        board_y + game.walls[i].y * CELL_SIZE,
        CELL_SIZE,
        CELL_SIZE,
        12);
    }
  }

  snake_draw_points(40, 20);

  if (game.snake_size > 0) {
    prev_snake_tail = game.snake[game.snake_size - 1].position;
  }
  else {
    prev_snake_tail = (Position) {-1, -1};
  }
  prev_food = game.food;
}

void snake_move() {
  game.direction = game.next_direction;

  if (game.snake_size > 0) {
    for (int i = game.snake_size - 1; i > 0; i--) {
      game.snake[i].position = game.snake[i - 1].position;
    }

    switch (game.direction) {
      case UP:
        game.snake[0].position.y--;
        break;
      case DOWN:
        game.snake[0].position.y++;
        break;
      case LEFT:
        game.snake[0].position.x--;
        break;
      case RIGHT:
        game.snake[0].position.x++;
        break;
    }
  }
}

bool snake_check_collision() {
  if (game.snake_size == 0)
    return false;

  if (game.snake[0].position.x < 0 || game.snake[0].position.x >= GRID_WIDTH ||
      game.snake[0].position.y < 0 || game.snake[0].position.y >= GRID_HEIGHT) {
    return true;
  }

  if (game.selected_modes == WALLS && game.wall_count > 0) {
    for (int i = 0; i < game.wall_count; i++) {
      if (game.snake[0].position.x == game.walls[i].x &&
          game.snake[0].position.y == game.walls[i].y) {
        return true;
      }
    }
  }

  for (int i = 1; i < game.snake_size; i++) {
    if (game.snake[0].position.x == game.snake[i].position.x &&
        game.snake[0].position.y == game.snake[i].position.y) {
      return true;
    }
  }

  if (game.snake[0].position.x == game.food.x &&
      game.snake[0].position.y == game.food.y) {
    int growth_amount = 1;

    if (game.selected_modes == FRUIT_POWER) {
      if (game.fruit_type == SPEED) {
        game.speed = EXTRA_GAME_SPEED;
        game.extra_speed_timer = 500;
        game.score += 20;
      }
      else if (game.fruit_type == SIZE) {
        game.score += 40;
        growth_amount = 5;
      }
      else {
        game.score += 10;
      }
    }
    else {
      game.score += 10;
    }

    Position last_segment_pos = game.snake[game.snake_size - 1].position;
    for (int g = 0; g < growth_amount && game.snake_size < SNAKE_MAX_SIZE; g++) {
      game.snake[game.snake_size].position = last_segment_pos;
      game.snake_size++;
    }

    if (game.selected_modes == WALLS) {
      snake_spawn_wall();
    }

    snake_spawn_food();
    game.food_timer = 0;
  }
  return false;
}

void snake_spawn_wall() {
  if (game.wall_count >= MAX_WALLS)
    return;

  int wx, wy;
  bool valid_position;
  do {
    valid_position = true;
    wx = rand() % GRID_WIDTH;
    wy = rand() % GRID_HEIGHT;

    for (int i = 0; i < game.snake_size; i++) {
      if (game.snake[i].position.x == wx && game.snake[i].position.y == wy) {
        valid_position = false;
        break;
      }
    }
    if (!valid_position)
      continue;

    if (game.food.x == wx && game.food.y == wy) {
      valid_position = false;
      continue;
    }

    for (int i = 0; i < game.wall_count; i++) {
      if (game.walls[i].x == wx && game.walls[i].y == wy) {
        valid_position = false;
        break;
      }
    }
  } while (!valid_position);

  game.walls[game.wall_count].x = wx;
  game.walls[game.wall_count].y = wy;
  game.wall_count++;
}

void snake_draw_points(int x, int y) {
  int points = game.score;
  if (points == 0) {
    draw_xpm_image(numeros[0], x, y);
    return;
  }

  int numeros_points[10];
  int i = 0;
  if (points == 0) {
    numeros_points[0] = 0;
    i = 1;
  }
  else {
    while (points > 0 && i < 10) {
      numeros_points[i] = points % 10;
      points = points / 10;
      i++;
    }
  }

  for (int h = i - 1; h >= 0; h--) {
    draw_xpm_image(numeros[numeros_points[h]], x, y);
    x += 25;
  }
}

void snake_draw_end() {
  vg_clear();
  our_vg_draw_rectangle_implementation(0, 0, vmi.XResolution, vmi.YResolution, 62);
  int x_pos = vmi.XResolution / 2 - 400;
  int y_pos = vmi.YResolution / 2 - 40;
  draw_xpm_image(points_label, x_pos, y_pos);
  x_pos += 400;
  y_pos += 8;
  snake_draw_points(x_pos, y_pos);
}

