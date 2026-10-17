#include "../headers/fruit.h"

extern int rand_seed;

FruitBasket createFruitBasket(size_t capacity) {
  FruitBasket fruit_basket;
  fruit_basket.capacity = capacity;
  fruit_basket.contents = (Fruit **) malloc(sizeof(Fruit*) * capacity);
  fruit_basket.next = 0;
  for (size_t i = 0; i < capacity; i++) {
    fruit_basket.contents[i] = NULL;
  }
  return fruit_basket;
}

void destroyFruitBasket(FruitBasket* fruit_basket) {
  if (fruit_basket == NULL) {
    return;
  }
  if (fruit_basket->contents) {
    for (size_t i = 0; i < fruit_basket->capacity; i++) {
      destroyFruit(fruit_basket->contents[i]);
    }
    free(fruit_basket->contents);
  }
}

void emptyFruitBasket(FruitBasket* fruit_basket) {
  for (size_t i = 0; i < fruit_basket->capacity; i++) {
    Fruit* rotten_fruit = fruit_basket->contents[i];
    fruit_basket->contents[i] = NULL;
    destroyFruit(rotten_fruit);
  }
  fruit_basket->next = 0;
}

int find_next(FruitBasket* fruit_basket) {
  for (size_t i = 0; i < fruit_basket->capacity; i++) {
    if (fruit_basket->contents[i] == NULL) {
      return i;
    }
  }
  return -1;
}

bool add_fruit(FruitBasket* fruit_basket, Fruit* new_fruit) {
  if (fruit_basket->next == -1) {
    return false;
  }
  fruit_basket->contents[fruit_basket->next] = new_fruit;
  fruit_basket->next = find_next(fruit_basket);
  return true;
}

bool remove_fruit(FruitBasket* fruit_basket, Fruit* rotten_fruit, size_t pos) {
  if (fruit_basket->contents[pos] != NULL && fruit_basket->contents[pos] == rotten_fruit) {
    fruit_basket->contents[pos] = NULL;
    if ((int)pos < fruit_basket->next || fruit_basket->next == -1) {
      fruit_basket->next = pos;
    }
    destroyFruit(rotten_fruit);
    return true;
  }
  return false;
}

Fruit* createFruit(xpm_image_t img_whole, xpm_image_t img_chopped, double x_init, double y_init, double x_speed, double y_speed, bool is_moving, bool is_chopped) {
  Fruit* fruit = (Fruit *) malloc(sizeof(Fruit));
  
  fruit->whole = img_whole;
  fruit->chopped = img_chopped;
  fruit->x_pos = x_init;
  fruit->y_pos = y_init;
  fruit->x_speed = x_speed;
  fruit->y_speed = y_speed;
  fruit->is_moving = is_moving;
  fruit->is_chopped = is_chopped;
  fruit->has_collided = false;
  fruit->is_bomb = false;

  //printf("Created fruit with bytes: %d and %d", img_whole.bytes, img_chopped.bytes);

  return fruit;
}

Fruit* createBomb(xpm_image_t img_whole, xpm_image_t img_chopped, double x_init, double y_init, double x_speed, double y_speed, bool is_moving, bool is_chopped) {
  Fruit* fruit = (Fruit *) malloc(sizeof(Fruit));
  
  fruit->whole = img_whole;
  fruit->chopped = img_chopped;
  fruit->x_pos = x_init;
  fruit->y_pos = y_init;
  fruit->x_speed = x_speed;
  fruit->y_speed = y_speed;
  fruit->is_moving = is_moving;
  fruit->is_chopped = is_chopped;
  fruit->has_collided = false;
  fruit->is_bomb = true;

  //printf("Created fruit with bytes: %d and %d", img_whole.bytes, img_chopped.bytes);

  return fruit;
}

void destroyFruit(Fruit* fruit) {
  if (fruit == NULL) {
    return;
  }
  free(fruit);
  fruit = NULL;
}

void draw_fruit(Fruit* fruit) {
  if (!fruit->is_chopped) {
    draw_xpm((uint16_t)fruit->x_pos, (uint16_t)fruit->y_pos, fruit->whole);
  } else
  {
    draw_xpm((uint16_t)fruit->x_pos, (uint16_t)fruit->y_pos, fruit->chopped);
  }
}

void move_fruit(Fruit* fruit) {
  if (fruit->is_moving) {
    double t = 1.0 / (double)FRAME_R;
    fruit->y_speed += gravity * t;
    fruit->x_pos = fruit->x_pos + t * fruit->x_speed;
    fruit->y_pos = fruit->y_pos + t * fruit->y_speed;
  }
  draw_fruit(fruit);
}

bool fruit_out_of_screen(Fruit* fruit) {
  if (fruit->x_speed >= 0 && fruit->y_speed >= 0) {
    return (fruit->x_pos >= get_x_res() || fruit->y_pos >= get_y_res());
    //return in_screen(fruit->x_pos, fruit->y_pos);
  } else {
    uint16_t width, height;
    if (fruit->is_chopped) {
      width = fruit->chopped.width;
      height = fruit->chopped.height;
    } else {
      width = fruit->whole.width;
      height = fruit->whole.height;
    }

    if (fruit->x_speed >= 0 && fruit->y_speed < 0) {
      return (fruit->x_pos >= get_x_res() || fruit->y_pos + height < 0);
      //return in_screen(fruit->x_pos, fruit->y_pos + height);
    }
    if (fruit->x_speed < 0 && fruit->y_speed >= 0) {
      return (fruit->x_pos + width < 0 || fruit->y_pos >= get_y_res());
      //return in_screen(fruit->x_pos + width, fruit->y_pos);
    }
    if (fruit->x_speed < 0 && fruit->y_speed < 0) {
      return (fruit->x_pos + width < 0 || fruit->y_pos + height < 0);
      //return in_screen(fruit->x_pos + width, fruit->y_pos + height);
    }
  }

  return false;
}

Fruit* launch_fruit(xpm_image_t whole, xpm_image_t chopped, bool bomb) {
  Fruit* f1;
  srand(rand_seed);
  double x_pos = 200.0 + (double)(rand() % (get_x_res()-whole.width - 200));
  double direction = rand() % 2;
  double speed_factor = (double)(rand() % 100) + 20;
  if (bomb) {
    f1 = createBomb(whole, chopped, x_pos, (double)get_y_res(), pow(-1.0, direction) * speed_factor, -500.0 + pow(-1.0, direction) * speed_factor / 2.0, true, false);
  } else {
    f1 = createFruit(whole, chopped, x_pos, (double)get_y_res(), pow(-1.0, direction) * speed_factor, -500.0 + pow(-1.0, direction) * speed_factor / 2.0, true, false);
  }
  return f1;
}

bool intersect_line(Fruit* fruit, line_t* line) {
  uint32_t *map = (uint32_t*)fruit->whole.bytes;
  for (int h = 0; h < fruit->whole.height; h++) {
    uint16_t y = fruit->y_pos + h;
    for (int w = 0; w < fruit->whole.width; w++) {
      uint16_t x = fruit->x_pos + w;
      uint32_t color = map[h * fruit->whole.width + w];
      if (color != xpm_transparency_color(fruit->whole.type)) {
        //set_pixel(0xff0000, x, y);
        for (size_t j = 0; j < line->size; j += 2) {
          if (line->points[j] == x && line->points[j+1] == y) {
            //printf("intersected at %d - %d\n", x, y);
            return true;
          }
          
        }
      }
    }
  }
  //printf("No intersections\n");
  return false;
}

size_t check_cut_fruit(FruitBasket* fruit_basket, line_t* line) {
  size_t cnt = 0;
  uint16_t x_li, y_li, x_lf, y_lf;
  x_li = MIN(line->points[0], line->points[line->size - 2]);
  y_li = MIN(line->points[1], line->points[line->size - 1]);
  x_lf = MAX(line->points[0], line->points[line->size - 2]);
  y_lf = MAX(line->points[1], line->points[line->size - 1]);

  for (size_t i = 0; i < fruit_basket->capacity; i++) {
    Fruit* fruit = fruit_basket->contents[i];
    if (fruit != NULL && !fruit->is_chopped && fruit->x_pos + fruit->whole.width > x_li && fruit->x_pos < x_lf && fruit->y_pos + fruit->whole.height > y_li && fruit->y_pos < y_lf) {
      if (intersect_line(fruit, line)) {
        cnt++;
        cut_fruit(fruit);
      }
    }
  }

  return cnt;
}

void cut_fruit(Fruit* fruit) {
  fruit->is_chopped = true;
  if (fruit->y_speed < 0) {
    fruit->y_speed = 0.0;
  }
}

void move_mouse(Fruit* mouse_pointer, int delta_x, int delta_y, bool lb) {
  if (lb) {
    mouse_pointer->is_chopped = true;
  } else {
    mouse_pointer->is_chopped = false;
  }

  int new_x = mouse_pointer->x_pos + delta_x;
  int new_y = mouse_pointer->y_pos - delta_y;
  
  if (new_x >= 0 && new_x <= (int)get_x_res()) {
    mouse_pointer->x_pos = new_x;
  } else if (new_x < 0) {
    mouse_pointer->x_pos = 0;
  } else if (new_x > (int)get_x_res()) {
    mouse_pointer->x_pos = get_x_res();
  }

  if (new_y >= 0 && new_y <= (int)get_y_res()) {
    mouse_pointer->y_pos = new_y;
  } else if (new_y < 0) {
    mouse_pointer->y_pos = 0;
  } else if (new_y > (int)get_y_res()) {
    mouse_pointer->y_pos = get_y_res();
  }     
}

bool collide_fruit(Fruit* f1, Fruit* f2) {
  if (f1->x_pos < f2->x_pos) {
    if (f2->x_pos > f1->x_pos + f1->whole.width) { return false; }
  } else if (f2->x_pos < f1->x_pos) {
    if (f1->x_pos > f2->x_pos + f2->whole.width) { return false; }
  }
  if (f1->y_pos < f2->y_pos) {
    if (f2->y_pos > f1->y_pos + f1->whole.height) { return false; }
  } else if (f2->y_pos < f1->y_pos) {
    if (f1->y_pos > f2->y_pos + f2->whole.height) { return false; }
  }

  //size_t w1 = 0, h1 = 0, w2 = 0, h2 = 0;
  uint32_t* map1 = (uint32_t*)f1->whole.bytes;
  uint32_t* map2 = (uint32_t*)f2->whole.bytes;
  size_t x_i = MAX(f1->x_pos, f2->x_pos);
  size_t x_f = MIN(f1->x_pos + f1->whole.width, f2->x_pos + f2->whole.width);
  size_t y_i = MAX(f1->y_pos, f2->y_pos);
  size_t y_f = MIN(f1->y_pos + f1->whole.height, f2->y_pos + f2->whole.height);
  size_t x = x_i;

  while (y_i < y_f) {
    while (x < x_f) {
      size_t w1 = x - f1->x_pos;
      size_t h1 = y_i - f1->y_pos;
      size_t w2 = x - f2->x_pos;
      size_t h2 = y_i - f2->y_pos;
      uint32_t c1 = map1[h1 * f1->whole.width + w1];
      uint32_t c2 = map2[h2 * f2->whole.width + w2];
      if (c1 != xpm_transparency_color(f1->whole.type) && c2 != xpm_transparency_color(f2->whole.type)) {
        // printf("COLIISION AT %d - %d!\n", x, y_i);
        // set_pixel(0x0000FF, f1->x_pos + w1, f1->y_pos + h1);
        return true;
      }
      x++;
    }
    x = x_i;
    y_i++;
  }

  return false;
}

void check_colliding_fruit(FruitBasket* fruit_basket) {
  for (size_t i = 0; i < fruit_basket->capacity; i++) {
    Fruit* f1 = fruit_basket->contents[i];
    if (f1 &&  !f1->is_chopped) {
      for (size_t j = i+1; j < fruit_basket->capacity; j++) {
        Fruit* f2 = fruit_basket->contents[j];
        if (f2 && !f2->is_chopped && !(f1->has_collided && f2->has_collided)) {
          if (f1->y_speed > 0 && f2->y_speed > 0 && collide_fruit(f1, f2)) {
            f1->has_collided = true;
            f2->has_collided = true;
            if (f1->y_speed > 0 && f2->y_speed > 0) {
              if ((f1->x_speed > 0 && f2->x_speed < 0) || (f1->x_speed < 0 && f2->x_speed > 0))  {
                f1->x_speed = -1.2 * f1->x_speed;
                f2->x_speed = -1.2 * f2->x_speed;
              } else if (f1->x_speed > 0 && f2->x_speed > 0) {
                if (f1->x_pos < f2->x_pos) {
                  f1->x_speed = -1.2 * f1->x_speed;
                } else {
                  f2->x_speed = -1.2 * f2->x_speed;
                }
              } else if (f1->x_speed < 0 && f2->x_speed < 0) {
                if (f1->x_pos > f2->x_pos) {
                  f1->x_speed = -1.2 * f1->x_speed;
                } else {
                  f2->x_speed = -1.2 * f2->x_speed;
                }
              }
            }
          }
        }
      }
    }
  }
}

void shoot_fruit(FruitBasket* fruit_basket, size_t num) {
  srand(rand_seed);
  for (size_t i = 0; i < num && fruit_basket->next != -1; i++) {
    int rnum = rand() % 3;
    int bnum = rand() % 15;
    if (bnum == 3) {
      add_fruit(fruit_basket, launch_fruit(game_images[BOMB_IMG], game_images[BOMB_IMG], true));
    } else {
      switch (rnum)
      {
      case 0:
        add_fruit(fruit_basket, launch_fruit(game_images[APPLE_IMG], game_images[CHOPPED_APPLE_IMG], false));
        break;
      case 1:
        add_fruit(fruit_basket, launch_fruit(game_images[PINEAPPLE_IMG], game_images[CHOPPED_PINEAPPLE_IMG], false));
        break;
      case 2:
        add_fruit(fruit_basket, launch_fruit(game_images[WATERMELON_IMG], game_images[CHOPPED_WATERMELON_IMG], false));
        break;
      default:
        break;
      }
    }
  }
}

void fire_fruit(FruitBasket* fruit_basket, Fruit* cannon) {
  srand(rand_seed);

  double direction = rand() % 2;
  double speed_factor = (double)(rand() % 100) + 20;

  int rnum = rand() % 3;
  int bnum = rand() % 15;
  if (bnum == 3) {
    add_fruit(fruit_basket, createBomb(game_images[BOMB_IMG], game_images[BOMB_IMG], cannon->x_pos + 60 - game_images[BOMB_IMG].width / 2, get_y_res(), pow(-1.0, direction) * speed_factor, -500.0 + pow(-1.0, direction) * speed_factor / 2.0, true, false));
  } else {
    switch (rnum)
    {
    case 0:
      add_fruit(fruit_basket, createFruit(game_images[APPLE_IMG], game_images[CHOPPED_APPLE_IMG], cannon->x_pos + 60 - game_images[APPLE_IMG].width / 2, get_y_res(), pow(-1.0, direction) * speed_factor, -500.0 + pow(-1.0, direction) * speed_factor / 2.0, true, false));
      break;
    case 1:
      add_fruit(fruit_basket, createFruit(game_images[PINEAPPLE_IMG], game_images[CHOPPED_PINEAPPLE_IMG], cannon->x_pos + 60 - game_images[PINEAPPLE_IMG].width / 2, get_y_res(), pow(-1.0, direction) * speed_factor, -500.0 + pow(-1.0, direction) * speed_factor / 2.0, true, false));
      break;
    case 2:
      add_fruit(fruit_basket, createFruit(game_images[WATERMELON_IMG], game_images[CHOPPED_WATERMELON_IMG], cannon->x_pos + 60 - game_images[WATERMELON_IMG].width / 2, get_y_res(), pow(-1.0, direction) * speed_factor, -500.0 + pow(-1.0, direction) * speed_factor / 2.0, true, false));
      break;
    default:
      break;
    }
  }

}
