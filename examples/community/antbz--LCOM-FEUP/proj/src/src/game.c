#include "../headers/game.h"


void mainMenuHandler(int dev) {
  switch (dev) {
    case TIMER: {
      if (timer_counter % (sys_hz() / FRAME_R) == 0) {
        clear_screen_background(main_screen->background);

        for (unsigned i = 0; i < main_screen->fruit_basket.capacity; i++) {
          if (main_screen->fruit_basket.contents[i] != NULL) {
            move_fruit(current_screen->fruit_basket.contents[i]);
          }
        }

        size_t n = 0;

        if (cutting_state == COMP) {
          line_t line = make_line(cut_xi, cut_yi, cut_xf, cut_yf);
          draw_line(0xFFFFFF, &line);
          n = check_cut_fruit(&main_screen->fruit_basket, &line);
          destroy_line(&line);
          cutting_state = INIT;
        }

        if (n == 1) {
          if (main_screen->fruit_basket.contents[0]->is_chopped) {
            main_screen->fruit_basket.contents[0]->is_chopped = false;
            multiplayer_mode = false;
            current_screen = singleplayer_screen;
          } else if (main_screen->fruit_basket.contents[1]->is_chopped) {
            main_screen->fruit_basket.contents[1]->is_chopped = false;
            multiplayer_mode = true;
            current_screen = multiplayer_screen;
          } else if (main_screen->fruit_basket.contents[2]->is_chopped) {
            main_screen->fruit_basket.contents[2]->is_chopped = false;
            current_screen = help_screen;
          }
        } else if (n > 1) {
          main_screen->fruit_basket.contents[0]->is_chopped = false;
          main_screen->fruit_basket.contents[1]->is_chopped = false;
          main_screen->fruit_basket.contents[2]->is_chopped = false;
        }
        
        draw_fruit(mouse_pointer);
      }

      break;
    }
    case MOUSE: {
      if (mouse_counter % 3 == 0 && mouse_counter != 0) {
        assemble_packet(&pp);
        move_mouse(mouse_pointer, pp.delta_x, pp.delta_y, pp.lb);
        mouse_event ev = detect_mouse_event(&pp, mouse_pointer, &cut_slope, 10);
        check_cutting(ev, &cut_slope, &cut_xi, &cut_yi, &cut_xf, &cut_yf);
      }
      break;
    }
    default:
      break;
  }
  return;
}


void singlePlayerMenuHandler(int dev) {
  switch (dev) {
    case KEYBOARD: {
      if (data[0] == CKEY) {
        emptyFruitBasket(&singleplayer_screen->fruit_basket);
        current_screen = endsp_game_screen;
        return;
      }
      break;
    }
    case TIMER: {
      if (timer_counter % (sys_hz() / FRAME_R) == 0) {
        clear_screen_background(singleplayer_screen->background);

        for (unsigned i = 0; i < singleplayer_screen->fruit_basket.capacity; i++) {
          if (singleplayer_screen->fruit_basket.contents[i] != NULL) {
            if (singleplayer_screen->fruit_basket.contents[i]->is_bomb && singleplayer_screen->fruit_basket.contents[i]->is_chopped) {
              emptyFruitBasket(&singleplayer_screen->fruit_basket);
              current_screen = endsp_game_screen;
              return;
            } else if(fruit_out_of_screen(singleplayer_screen->fruit_basket.contents[i])){
              remove_fruit(&singleplayer_screen->fruit_basket, singleplayer_screen->fruit_basket.contents[i], i);
            }
            else
            {
              check_colliding_fruit(&singleplayer_screen->fruit_basket);
              move_fruit(singleplayer_screen->fruit_basket.contents[i]);
            }
          }
        }

        if (cutting_state == COMP) {
          line_t line = make_line(cut_xi, cut_yi, cut_xf, cut_yf);
          draw_line(0xFFFFFF, &line);
          combo = check_cut_fruit(&singleplayer_screen->fruit_basket, &line);
          score += combo * combo;
          destroy_line(&line);
          srand(rand_seed);
          if (combo > 3) {
            shoot_fruit(&singleplayer_screen->fruit_basket, 3+ rand() % 17);
          }
          cutting_state = INIT;
        }
        
        show_score();
        draw_fruit(mouse_pointer);
      }

      if (timer_counter % (sys_hz()/2) == 0) {
        shoot_fruit(&singleplayer_screen->fruit_basket, 1);
      }

      if (score != 0 && score % 100 == 0) {
        shoot_fruit(&singleplayer_screen->fruit_basket, 30);
      }
      break;
    }
    case MOUSE: {
     if (mouse_counter % 3 == 0 && mouse_counter != 0) {
        assemble_packet(&pp);
        move_mouse(mouse_pointer, pp.delta_x, pp.delta_y, pp.lb);
        mouse_event ev = detect_mouse_event(&pp, mouse_pointer, &cut_slope, 20);
        check_cutting(ev, &cut_slope, &cut_xi, &cut_yi, &cut_xf, &cut_yf);
      }
      break;
    }
    default:
      break;
  }
  return;
}

void singlePlayerEndHandler(int dev) {
  switch (dev) {
    case KEYBOARD: {
      break;
    }
    case TIMER: {
      if (timer_counter % (sys_hz() / FRAME_R) == 0) {
        clear_screen_background(endsp_game_screen->background);
        draw_number(score, 780, 220, true, digits);

        for (unsigned i = 0; i < endsp_game_screen->fruit_basket.capacity; i++) {
          if (endsp_game_screen->fruit_basket.contents[i] != NULL) {
            move_fruit(endsp_game_screen->fruit_basket.contents[i]);
          }
        }

        size_t n = 0;

        if (cutting_state == COMP) {
          line_t line = make_line(cut_xi, cut_yi, cut_xf, cut_yf);
          draw_line(0xFFFFFF, &line);
          n = check_cut_fruit(&endsp_game_screen->fruit_basket, &line);
          destroy_line(&line);
          cutting_state = INIT;
        }

        if (n == 1) {
          if (endsp_game_screen->fruit_basket.contents[0]->is_chopped) {
            score = 0;
            combo = 0;
            endsp_game_screen->fruit_basket.contents[0]->is_chopped = false;
            if (multiplayer_mode) {
              current_screen = multiplayer_screen;
            } else {
              current_screen = singleplayer_screen;
            }
          } else if (endsp_game_screen->fruit_basket.contents[1]->is_chopped) {
            score = 0;
            combo = 0;
            endsp_game_screen->fruit_basket.contents[1]->is_chopped = false;
            multiplayer_mode = false;
            current_screen = main_screen;
          }
        } else if (n > 1) {
          endsp_game_screen->fruit_basket.contents[0]->is_chopped = false;
          endsp_game_screen->fruit_basket.contents[1]->is_chopped = false;
        }
        
        draw_fruit(mouse_pointer);
      }
      break;
    }
    case MOUSE: {
     if (mouse_counter % 3 == 0 && mouse_counter != 0) {
        assemble_packet(&pp);
        move_mouse(mouse_pointer, pp.delta_x, pp.delta_y, pp.lb);
        mouse_event ev = detect_mouse_event(&pp, mouse_pointer, &cut_slope, 10);
        check_cutting(ev, &cut_slope, &cut_xi, &cut_yi, &cut_xf, &cut_yf);
      }
      break;
    }
    default:
      break;
  }
  return;
}

void helpHandler(int dev) {
  switch (dev) {
    case TIMER: {
      if (timer_counter % (sys_hz() / FRAME_R) == 0) {
        clear_screen_background(help_screen->background);

        for (unsigned i = 0; i < help_screen->fruit_basket.capacity; i++) {
          if (help_screen->fruit_basket.contents[i] != NULL) {
            move_fruit(help_screen->fruit_basket.contents[i]);
          }
        }

        size_t n = 0;

        if (cutting_state == COMP) {
          line_t line = make_line(cut_xi, cut_yi, cut_xf, cut_yf);
          draw_line(0xFFFFFF, &line);
          n = check_cut_fruit(&help_screen->fruit_basket, &line);
          destroy_line(&line);
          cutting_state = INIT;
        }

        if (n == 1) {
          if (help_screen->fruit_basket.contents[0]->is_chopped) {
            help_screen->fruit_basket.contents[0]->is_chopped = false;
            help_screen->background = game_images[HELP1_BACKGROUND_IMG];
            if (!help_screen->fruit_basket.contents[1]) {
              help_screen->fruit_basket.contents[1] = createFruit(game_images[PINEAPPLE_IMG], game_images[CHOPPED_PINEAPPLE_IMG], 780, 650, 0, 0, false, false);
            }
            current_screen = main_screen;
          } else if (help_screen->fruit_basket.contents[1] && help_screen->fruit_basket.contents[1]->is_chopped) {
            remove_fruit(&help_screen->fruit_basket, help_screen->fruit_basket.contents[1], 1);
            help_screen->background = game_images[HELP2_BACKGROUND_IMG];
          }
        } else if (n > 1) {
          help_screen->fruit_basket.contents[0]->is_chopped = false;
          help_screen->fruit_basket.contents[1]->is_chopped = false;
        }
        
        draw_fruit(mouse_pointer);
      }
      break;
    }
    case MOUSE: {
      if (mouse_counter % 3 == 0 && mouse_counter != 0) {
        assemble_packet(&pp);
        move_mouse(mouse_pointer, pp.delta_x, pp.delta_y, pp.lb);
        mouse_event ev = detect_mouse_event(&pp, mouse_pointer, &cut_slope, 10);
        check_cutting(ev, &cut_slope, &cut_xi, &cut_yi, &cut_xf, &cut_yf);
      }
      break;
    }
    default:
      break;
  }
}


void multiPlayerHandler(int dev) {
  switch (dev) {
    case KEYBOARD: {
      if (data[0] == CKEY) {
        
        emptyFruitBasket(&multiplayer_screen->fruit_basket);
        add_fruit(&multiplayer_screen->fruit_basket, createFruit(game_images[CANON_IMG], game_images[CANON_IMG], 500, get_y_res() - 150, 0, 0, false, false));
        current_screen = endsp_game_screen;
        serial_transmit_force(M_GAME_END);
        return;

      }
      break;
    }
    case TIMER: {
      if (timer_counter % (sys_hz() / FRAME_R) == 0) {
        clear_screen_background(multiplayer_screen->background);

        for (unsigned i = 0; i < multiplayer_screen->fruit_basket.capacity; i++) {
          if (multiplayer_screen->fruit_basket.contents[i] != NULL) {
            if (multiplayer_screen->fruit_basket.contents[i]->is_bomb && multiplayer_screen->fruit_basket.contents[i]->is_chopped) {
              
              emptyFruitBasket(&multiplayer_screen->fruit_basket);
              add_fruit(&multiplayer_screen->fruit_basket, createFruit(game_images[CANON_IMG], game_images[CANON_IMG], 500, get_y_res() - 150, 0, 0, false, false));
              current_screen = endsp_game_screen;
              return;

            } else if(fruit_out_of_screen(multiplayer_screen->fruit_basket.contents[i])){
              remove_fruit(&multiplayer_screen->fruit_basket, multiplayer_screen->fruit_basket.contents[i], i);
            }
            else
            {
              check_colliding_fruit(&multiplayer_screen->fruit_basket);
              move_fruit(multiplayer_screen->fruit_basket.contents[i]);
            }
          }
        }

        if (cutting_state == COMP) {
          line_t line = make_line(cut_xi, cut_yi, cut_xf, cut_yf);
          draw_line(0xFFFFFF, &line);
          combo = check_cut_fruit(&multiplayer_screen->fruit_basket, &line);
          score += combo * combo;
          destroy_line(&line);
          cutting_state = INIT;
        }

        show_score();
        draw_fruit(mouse_pointer);
      }
      break;
    }
    case MOUSE: {
      if (mouse_counter % 3 == 0 && mouse_counter != 0) {
        assemble_packet(&pp);
        move_mouse(mouse_pointer, pp.delta_x, pp.delta_y, pp.lb);
        mouse_event ev = detect_mouse_event(&pp, mouse_pointer, &cut_slope, 10);
        check_cutting(ev, &cut_slope, &cut_xi, &cut_yi, &cut_xf, &cut_yf);
      }
      break;
    }
    case SERIAL: {
      switch (ser_in)
      {
      case M_GAME_END: {
        emptyFruitBasket(&multiplayer_screen->fruit_basket);
        add_fruit(&multiplayer_screen->fruit_basket, createFruit(game_images[CANON_IMG], game_images[CANON_IMG], 500, get_y_res() - 150, 0, 0, false, false));
        current_screen = endsp_game_screen;
        return;
      }
      case SPACE: {
        fire_fruit(&multiplayer_screen->fruit_basket, multiplayer_screen->fruit_basket.contents[0]);
        break;
      }
      case AKEY: {
        double x_new = multiplayer_screen->fruit_basket.contents[0]->x_pos - 20;
        if (x_new < 0) {
          multiplayer_screen->fruit_basket.contents[0]->x_pos = 0;
        } else {
          multiplayer_screen->fruit_basket.contents[0]->x_pos = x_new;
        }
        break;
      }
      case DKEY: {
        double x_new = multiplayer_screen->fruit_basket.contents[0]->x_pos + 20;
        if (x_new + multiplayer_screen->fruit_basket.contents[0]->whole.width > get_x_res()) {
          multiplayer_screen->fruit_basket.contents[0]->x_pos = get_x_res() - multiplayer_screen->fruit_basket.contents[0]->whole.width;
        } else {
          multiplayer_screen->fruit_basket.contents[0]->x_pos = x_new;
        }
        break;
      }
      default:
        break;
      }
    }
    default:
      break;
  }
}


void spectatorHandler(int dev) {
  switch (dev) {
    case KEYBOARD: {
      if (data[0] == CKEY) {
        emptyFruitBasket(&multiplayer_screen->fruit_basket);
        add_fruit(&multiplayer_screen->fruit_basket, createFruit(game_images[CANON_IMG], game_images[CANON_IMG], 500, get_y_res() - 150, 0, 0, false, false));
        current_screen = endsp_game_screen;
        serial_transmit_force(M_GAME_END);
        return;
      }

      if (data[0] == SPACE) {
        serial_transmit_force(SPACE);
      }

      if (data[0] == AKEY) {
        double x_new = multiplayer_screen->fruit_basket.contents[0]->x_pos - 20;
        if (x_new < 0) {
          multiplayer_screen->fruit_basket.contents[0]->x_pos = 0;
        } else {
          multiplayer_screen->fruit_basket.contents[0]->x_pos = x_new;
        }
        serial_transmit_force(AKEY);
      }

      if (data[0] == DKEY) {
        double x_new = multiplayer_screen->fruit_basket.contents[0]->x_pos + 20;
        if (x_new + multiplayer_screen->fruit_basket.contents[0]->whole.width > get_x_res()) {
          multiplayer_screen->fruit_basket.contents[0]->x_pos = get_x_res() - multiplayer_screen->fruit_basket.contents[0]->whole.width;
        } else {
          multiplayer_screen->fruit_basket.contents[0]->x_pos = x_new;
        }
        serial_transmit_force(DKEY);
      }

      break;
    }
    case TIMER: {
      clear_screen_background(multiplayer_screen->background);

      for (unsigned i = 0; i < multiplayer_screen->fruit_basket.capacity; i++) {
        if (multiplayer_screen->fruit_basket.contents[i] != NULL) {
          move_fruit(multiplayer_screen->fruit_basket.contents[i]);
        }
      }

      break;
    }
    case SERIAL: {
      switch (ser_in)
      {
      case M_GAME_END: {
        emptyFruitBasket(&multiplayer_screen->fruit_basket);
        add_fruit(&multiplayer_screen->fruit_basket, createFruit(game_images[CANON_IMG], game_images[CANON_IMG], 500, get_y_res() - 150, 0, 0, false, false));
        current_screen = endsp_game_screen;
        return;
      }
      default:
        break;
      }
      break;
    }
    default:
      break;
  }
}


Screen* loadMainScreen() { 
  FruitBasket fruit_basket = createFruitBasket(3);

  fruit_basket.contents[0] = createFruit(game_images[SINGLEPLAYER_IMG], game_images[CHOPPED_SINGLEPLAYER_IMG], 131, 290, 0, 0, false, false);
  fruit_basket.contents[1] = createFruit(game_images[MULTIPLAYER_IMG], game_images[CHOPPED_MULTIPLAYER_IMG], 643, 290, 0, 0, false, false);
  fruit_basket.contents[2] = createFruit(game_images[WATERMELON_IMG], game_images[CHOPPED_WATERMELON_IMG], 400, 620, 0, 0, false, false);
  
  return createScreen(&mainMenuHandler, game_images[MENU_BACKGROUND_IMG], fruit_basket);
}

Screen* loadSinglePlayerScreen() {
  FruitBasket fruit_basket = createFruitBasket(100);

  return createScreen(&singlePlayerMenuHandler, game_images[BACKGROUND_IMG], fruit_basket);
}

Screen* loadSinglePlayerEndScreen() {
  FruitBasket fruit_basket = createFruitBasket(2);

  fruit_basket.contents[0] = createFruit(game_images[PINEAPPLE_IMG], game_images[CHOPPED_PINEAPPLE_IMG], 210, 410, 0, 0, false, false);
  fruit_basket.contents[1] = createFruit(game_images[APPLE_IMG], game_images[CHOPPED_APPLE_IMG], 215, 520, 0, 0, false, false);

  return createScreen(&singlePlayerEndHandler, game_images[SP_END_BACKGROUND_IMG], fruit_basket);
}

Screen* loadHelpScreen() {
  FruitBasket fruit_basket = createFruitBasket(2);

  fruit_basket.contents[0] = createFruit(game_images[APPLE_IMG], game_images[CHOPPED_APPLE_IMG], 180, 680, 0, 0, false, false);
  fruit_basket.contents[1] = createFruit(game_images[PINEAPPLE_IMG], game_images[CHOPPED_PINEAPPLE_IMG], 780, 650, 0, 0, false, false);

  return createScreen(&helpHandler, game_images[HELP1_BACKGROUND_IMG], fruit_basket);
}

Screen* loadMultiPlayerScreen() {
  FruitBasket fruit_basket = createFruitBasket(21);

  add_fruit(&fruit_basket, createFruit(game_images[CANON_IMG], game_images[CANON_IMG], 500, get_y_res() - 150, 0, 0, false, false));

  return createScreen(&multiPlayerHandler, game_images[BACKGROUND_IMG], fruit_basket);
}

Screen* loadSpectatorScreen() {
  FruitBasket fruit_basket = createFruitBasket(1);

  add_fruit(&fruit_basket, createFruit(game_images[CANON_IMG], game_images[CANON_IMG], 500, get_y_res() - 150, 0, 0, false, false));

  return createScreen(&spectatorHandler, game_images[BACKGROUND_IMG], fruit_basket);
}

void show_score() {
  draw_number(score, get_x_res(), 0, true, digits);
  if (combo > 1) {
    draw_number(combo, get_x_res() - game_images[COMBO_IMG].width - 10, 60, true, digits);
    draw_xpm(get_x_res() - game_images[COMBO_IMG].width, 80, game_images[COMBO_IMG]);
  }
}
