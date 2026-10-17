#include "IO_controller.h"

extern application_state_t app_state;
application_state_t last_app_state;

bool changed=false; // Initialize to an invalid state

extern game_action_state_t game_action_state;
game_action_state_t last_game_action_state;
extern bool multi_player_mode;
extern Player_data_t *player_data;
extern int player_count;
extern key_identifier_t key;
extern int current_player;
extern bool multi_player_mode;
extern struct packet mouse_packet;
extern int current_hand;
bool first_time = true;
int last_money = PLAYER_INITIAL_MONEY;
buttons_active_list button_list;
extern bool payout;
extern bool won;
extern bool dealers_turn;
extern int current_bet_amount;
extern int mouse_x; // Mouse x coordinate
extern int mouse_y; // Mouse y coordinate
extern bool click_started; // Last left button state


int IO_controller_init() {
  if (set_graphic_mode(GRAPHIC_MODE) != 0) { // 1280x1024
    printf("set_graphic_mode failed\n");
    return 1;
  }

  if (map_VRAM(GRAPHIC_MODE) != 0) {
    printf("map_VRAM failed\n");
    return 1;
  }

  if (IO_controller_init_sprites() != 0) {
    printf("IO_controller_init_sprites failed\n");
    return 1;
  }

  button_list.count = 0;
  button_list.buttons[button_list.count++] = HIT_BT;
  button_list.buttons[button_list.count++] = STAY_BT;
  button_list.buttons[button_list.count++] = DOUBLE_BT;
  button_list.buttons[button_list.count++] = SPLIT_BT;

  last_app_state = app_state;
  last_game_action_state = game_action_state;
  return 0;
}

int IO_controller_display_frame() {
  bool money_updated = false;

  if (player_data != NULL) {
    money_updated = (last_money != player_data[current_player].money);
    if (money_updated) {
      printf("mudou %d-%d\n", last_money, player_data[current_player].money);
      printf("game state: %d\n", game_action_state);
      last_money = player_data[current_player].money;
    }
  }

  if (!((last_app_state == app_state) && (last_game_action_state == game_action_state) && (!first_time) && (!money_updated))) {
    if (first_time) {
      first_time = false;
    }
    last_app_state = app_state;
    last_game_action_state = game_action_state;
  }
  else {
    return 0;
  }
/*
  if(draw_sprite(MOUSE_CURSOR, mouse_x, mouse_y) != 0) {
    printf("Failed to draw mouse cursor\n");
    return 1;
  } */


  switch (app_state) {
    case MAIN_MENU:
      // Handle main menu display
      if (IO_controller_display_menu() != 0) {
        printf("Failed to Display MAIN MENU\n");
        return 1;
      }
      break;
    case SINGLE_PLAYER:
      // Handle single player display

      switch (game_action_state) {
        case START:
          if (IO_controller_display_table() != 0) {
            printf("Failed to Display MAIN MENU\n");
            return 1;
          }
          if (IO_controller_display_bet_buttons() != 0) {
            printf("Failed to draw bet buttons \n");
            return 1;
          }
          if (IO_controller_display_ready_button() != 0) {
            printf("Failed to draw bet buttons \n");
            return 1;
          }
          if (IO_controller_display_current_player_money() != 0) {
            printf("Failed to draw player monney \n");
            return 1;
          }
          if (IO_controller_display_player_hands() != 0) {
            printf("Could not draw Players hand\n");
            return 1;
          }
          if (IO_controller_display_dealer_hand() != 0) {
            printf("Could not draw Dealers hand\n");
            return 1;
          }
          if (IO_controller_display_bet_selector() != 0) {
            printf("Failed to display bet selector\n");
            return 1;
          }
          game_action_state = WAITING_PLAYER_ACTION_GAME;
          // Initialize game state
          break;
        case DEAL:
          // Deal cards to players and dealer
          break;
        case BET:
          // Handle betting display
          break;
        case DECISIONS:
          // Handle decision making display
          if (IO_controller_display_table() != 0) {
            printf("Failed to Display MAIN MENU\n");
            return 1;
          }
          if (IO_controller_display_player_hands() != 0) {
            printf("Could not draw Players hand\n");
            return 1;
          }
          if (IO_controller_display_dealer_hand() != 0) {
            printf("Could not draw Dealers hand\n");
            return 1;
          }
          if (IO_controller_display_turn_buttons() != 0) {
            printf("Failed to Display turn_buttons\n");
            return 1;
          }
          if (IO_controller_display_current_player_money() != 0) {
            printf("Failed to draw player monney \n");
            return 1;
          }
          break;
        case PAYOUT:
          // Handle payout display

          break;
        case END_GAME:
          // Handle end of game display
          if (IO_controller_display_wining_players() != 0) {
            printf("Failed to Display wining players\n");
            return 1;
          }
          break;
        case WAITING_PLAYER_ACTION_GAME:
          // WAITING_PLAYER_ACTION
          if (money_updated) {
            if (IO_controller_display_current_player_money() != 0) {
              printf("Failed to draw player monney \n");
              return 1;
            }
          }
          if (IO_controller_display_bet_selector() != 0) {
            printf("Failed to display bet selector\n");
            return 1;
          }
          break;
        default:
          printf("Invalid game action state");
          return 1; // Invalid action state
      }
      break;
    case MULTI_PLAYER:
      // Handle multi-player display
      if (IO_controller_display_table() != 0) {
        printf("Failed to Display MAIN MENU\n");
        return 1;
      }

      if (IO_controller_display_player_hands() != 0) {
        printf("Could not draw Players hand\n");
        return 1;
      }

      if (IO_controller_display_dealer_hand() != 0) {
        printf("Could not draw Dealers hand\n");
        return 1;
      }
      break;
    case EXIT:
      // Handle exit display
      break;
    default:
      printf("Invalid application state\n");
      break;
  }
  return 0;
}

int IO_controller_display_menu() {
  if (draw_sprite(MAIN_MENU_SPRITE, 0, 0) != 0) {
    printf("Could not draw MAIN MENU SPRITE\n");
    return 1;
  }
  return 0;
}

int IO_controller_display_player_hands() {
  // printf("Displaying player hands...\n");

  // Check if player_data is initialized
  if (player_data == NULL) {
    printf("player_data is NULL!\n");
    return 0;
  }

  // printf("player_count = %d\n", player_count);

  // Player positions [dealer, player1, player2, player3, player4]
  const int player_y[4] = {500, 600, 600, 500};
  const int player_x[4] = {30, 315, 650, 900};

  // Only display non-dealer players (players 1-4)
  for (int i = 1; i < player_count; i++) {

    // Add pointer check
    if (&player_data[i] == NULL) {
      printf("  ERROR: player_data[%d] is NULL!\n", i);
      continue;
    }

    // printf("  Player %d: hands_count=%d, hands_ptr=%p\n", i, player_data[i].hands_count, player_data[i].hands);

    // Skip players with no hands
    if (player_data[i].hands_count <= 0) {
      printf("  Player %d has no hands. Skipping.\n", i);
      continue;
    }

    for (int h = 0; h < player_data[i].hands_count; h++) {

      int base_x = player_x[player_data[i].hands[h].place_on_table - 1];
      int base_y = player_y[player_data[i].hands[h].place_on_table - 1];
      // Add hand pointer check
      if (&player_data[i].hands[h] == NULL) {
        printf("  ERROR: hand %d for player %d is NULL!\n", h, i);
        continue;
      }

      hand_t *hand = &player_data[i].hands[h];
      // printf("  Hand %d: num_cards=%d\n", h, hand->num_cards);

      // Skip hands with no cards
      if (hand->num_cards <= 0) {
        printf("    Hand %d has no cards. Skipping.\n", h);
        continue;
      }

      // Add cards pointer check
      if (hand->cards == NULL) {
        printf("  ERROR: cards array for hand %d (player %d) is NULL!\n", h, i);
        continue;
      }

      for (int c = 0; c < hand->num_cards; c++) {
        card_t card = hand->cards[c];
        Sprite *card_sprite = IO_get_card_sprite(card);

        if (card_sprite != NULL) {
          int x = base_x + c * 40; // 40px horizontal offset
          // printf("    Drawing card %d (number=%d, letter=%c) at (%d, %d)\n", c, card.number, card.letter, x, base_y);
          draw_sprite(card_sprite, x, base_y);
        }
        else {
          printf("    ERROR: No sprite for card %d (number=%d, letter=%c)\n", c, card.number, card.letter);
        }
      }
    }
  }
  // printf("Finished displaying player hands\n");
  return 0;
}

int IO_controller_display_dealer_hand() {
  // printf("Displaying dealer hand...\n");

  // Dealer position (player 0)
  const int dealer_y = 180;
  const int dealer_x = 460;

  // Check if player_data is initialized
  if (player_data == NULL) {
    printf("ERROR: player_data is NULL!\n");
    return 0;
  }

  // Skip if dealer has no hands
  if (player_data[0].hands_count <= 0) {
    printf("  Dealer has no hands (hands_count=%d). Skipping.\n", player_data[0].hands_count);
    return 0;
  }

  // printf("  Dealer has %d hands\n", player_data[0].hands_count);

  for (int h = 0; h < player_data[0].hands_count; h++) {
    // Check hand pointer
    if (&player_data[0].hands[h] == NULL) {
      printf("  ERROR: hand %d is NULL!\n", h);
      continue;
    }

    hand_t *hand = &player_data[0].hands[h];
    // printf("  Hand %d: num_cards=%d\n", h, hand->num_cards);

    // Skip hands with no cards
    if (hand->num_cards <= 0) {
      printf("    Hand %d has no cards. Skipping.\n", h);
      continue;
    }

    // Check cards array
    if (hand->cards == NULL) {
      printf("  ERROR: cards array for hand %d is NULL!\n", h);
      continue;
    }

    if (dealers_turn) {
      for (int c = 0; c < hand->num_cards; c++) {
        card_t card = hand->cards[c];
        // printf("    Card %d: number=%d, suit=%c\n", c, card.number, card.letter);

        Sprite *card_sprite = IO_get_card_sprite(card);

        if (card_sprite != NULL) {
          int x = dealer_x + c * 40; // 40px horizontal offset
          // printf("    Drawing card %d (number=%d, suit=%c) at (%d, %d)\n",c, card.number, card.letter, x, dealer_y);
          draw_sprite(card_sprite, x, dealer_y);
        }
        else {
          printf("    ERROR: No sprite for card %d (number=%d, suit=%c)\n", c, card.number, card.letter);
        }
      }
    }
    else {
      card_t card = hand->cards[0];
      Sprite *card_sprite = IO_get_card_sprite(card);
      if (card_sprite != NULL) {
        int x = dealer_x; // 40px horizontal offset
        draw_sprite(card_sprite, x, dealer_y);
      }
      else {
        printf("    ERROR: No sprite for card %d (number=%d, suit=%c)\n", 0, card.number, card.letter);
      }
      int x = dealer_x + 40; // 40px horizontal offset
      draw_sprite(BACK_RED, x, dealer_y);
    }
  }
  // printf("Finished displaying dealer hand\n");
  return 0;
}

int IO_controller_display_table() {
  if (draw_sprite(BACKGROUND_SPRITE, 0, 0) != 0) {
    printf("Could not draw MAIN MENU SPRITE\n");
    return 1;
  }
  return 0;
}

int IO_controller_display_turn_buttons() {
  // Button display area: bottom-left corner (50px from left, 50px from bottom)
  const int start_x = 20;
  const int start_y = 770;
  const int spacing = 215;

  int current_x = start_x;

  for (int i = 0; i < button_list.count; i++) {
    switch (button_list.buttons[i]) {
      case STAY_BT:
        draw_sprite(BUTTON_STAND, current_x, start_y);
        current_x += spacing;
        break;
      case SPLIT_BT:
        draw_sprite(BUTTON_SPLIT, current_x, start_y);
        current_x += spacing;
        break;
      case HIT_BT:
        draw_sprite(BUTTON_HIT, current_x, start_y);
        current_x += spacing;
        break;
      case DOUBLE_BT:
        draw_sprite(BUTTON_DOUBLE, current_x, start_y);
        current_x += spacing;
        break;
      case STAY_BT_N:
        draw_sprite(BUTTON_STAND_N, current_x, start_y);
        current_x += spacing;
        break;
      case SPLIT_BT_N:
        draw_sprite(BUTTON_SPLIT_N, current_x, start_y);
        current_x += spacing;
        break;
      case HIT_BT_N:
        draw_sprite(BUTTON_HIT_N, current_x, start_y);
        current_x += spacing;
        break;
      case DOUBLE_BT_N:
        draw_sprite(BUTTON_DOUBLE_N, current_x, start_y);
        current_x += spacing;
        break;
      default:
        // Skip unsupported buttons
        break;
    }
  }
  return 0;
}

int IO_controller_close() {
  if (set_text_mode() != 0) {
    printf("set_text_mode failed\n");
    return 1;
  }
  return 0;
}

int IO_controller_mouse_handler() {

  switch (app_state) {
    case MAIN_MENU:

      printf("Mouse coordinates: (%d, %d)\n", mouse_x, mouse_y);

      if (mouse_x >= 0 &&  mouse_x<= 1152 && mouse_y>= 0 && mouse_y <= 864 ) {
        
        if (click_started) {
          click_started = false;
          if (mouse_x >= 190 && mouse_x <= 500 && mouse_y >= 185 && mouse_y <= 300 ) {
            //draw_sprite(MOUSE_CURSOR, mouse_x, mouse_y);
            printf("Single Player button clicked\n");

            app_state = SINGLE_PLAYER;
            changed = true;
            multi_player_mode = false;
          }
          else if (mouse_x  >= 555 && mouse_x  <= 830 && mouse_y >= 185 && mouse_y <= 300 ) {
            printf("Multi Player button clicked\n");
            //draw_sprite(MOUSE_CURSOR, mouse_x, mouse_y);

            app_state = MULTI_PLAYER;
            changed = true;
            multi_player_mode = true;
            
          }
          else {
            draw_sprite_and_restore(MOUSE_CURSOR, mouse_x, mouse_y);
            
          }
        }
        else {
          draw_sprite_and_restore(MOUSE_CURSOR, mouse_x, mouse_y);
          
        }
      }
      else {
        printf("Mouse coordinates out of bounds\n");
      }

      break;
    case SINGLE_PLAYER:
      printf("Mouse coordinates: (%d, %d)\n", mouse_x, mouse_y);
      if (mouse_x >= 0 &&  mouse_x<= 1152 && mouse_y>= 0 && mouse_y <= 864 ) {
        
        if (click_started) {
          click_started = false;
          //botões para apostar
          if((mouse_x>=60 && mouse_x<=220 && mouse_y>=530 && mouse_y<=590)||(mouse_x>=350 && mouse_x<=400 && mouse_y>=630 && mouse_y<=690)||(mouse_x>=650 && mouse_x<=700 && mouse_y>=630 && mouse_y<=690)||(mouse_x>=950 && mouse_x<=1010 && mouse_y>=530 && mouse_y<=590)){
              if (player_data[current_player].action == WAITING_PLAYER_ACTION) {
                game_action_state = BET;
              }
              break;
          }
          //botão para voltar ao menu
          if((mouse_x>=26 && mouse_x<=180 && mouse_y>=10 && mouse_y<=60)){
            app_state = MAIN_MENU;
            changed = true;
            break;
          }
          //botão começar o jogo/ENTER
          if(mouse_x>=420 && mouse_x<=620 && mouse_y>=190 && mouse_y<=250){
            if (game_action_state == END_GAME) {
              game_action_state = START;
            }
          }
          //botão do HIT
          if(mouse_x>=35 && mouse_x<=190 && mouse_y>=776 && mouse_y<=840){
            if (player_data[current_player].action == WAITING_PLAYER_ACTION) {
              player_data[current_player].action = HIT;
              first_time = true;
            }
          }
          //botão do STAY
          if(mouse_x>=250 && mouse_x<=405 && mouse_y>=776 && mouse_y<=840){
            if (player_data[current_player].action == WAITING_PLAYER_ACTION) {
              player_data[current_player].action = STAY;
              first_time = true;
            }
          }
          //botão do DOUBLE
          if(mouse_x>=465 && mouse_x<=620 && mouse_y>=776 && mouse_y<=840){
            if (player_data[current_player].action == WAITING_PLAYER_ACTION) {
              player_data[current_player].action = DOUBLE;
              first_time = true;
            }
          }
          //botão do SPLIT
          if(mouse_x>=680 && mouse_x<=835 && mouse_y>=776 && mouse_y<=840){
            if (player_data[current_player].action == WAITING_PLAYER_ACTION) {
              player_data[current_player].action = SPLIT;
              first_time = true;
            }
          }
          //botão de aumentar o tamanho da aposta
          if(mouse_x>=920 && mouse_x<=935 && mouse_y>=25 && mouse_y<=35){
              if (game_action_state == WAITING_PLAYER_ACTION_GAME) {
              if ((current_bet_amount + 10 < player_data[current_player].money) > 0) {
                current_bet_amount += 10;
                first_time = true;
              }
            }
          }
          //botão de diminuir o tamanho da aposta
          if(mouse_x>=920 && mouse_x<=935 && mouse_y>130 && mouse_y<=155){
              if ((current_bet_amount - 10) > 0) {
                current_bet_amount -= 10;
                first_time = true;
              }
          }
        }
        else {
          draw_sprite_and_restore(MOUSE_CURSOR, mouse_x, mouse_y);
          
        }
      }
      
    
      break;
    case MULTI_PLAYER:

      break;
    case EXIT:

      break;
    default:
      printf("Invalid application state\n");
      break;
  }
  return 0;
}

int IO_controller_keyboard_handler() {
  if (key < 1) { // break code
    return 0;
  }
  printf("Key pressed: %d\n", key);
  printf("app state: %d\n", app_state);
  printf("game state: %d\n", game_action_state);
  switch (app_state) {
    case MAIN_MENU:
      switch (key) {
        case KEY_ESCAPE:
          app_state = EXIT;
          break;
        case KEY_S:
          app_state = SINGLE_PLAYER;
          multi_player_mode = false;
          break;
        case KEY_M:
          app_state = MULTI_PLAYER;
          multi_player_mode = true;
          break;
        default:
          break;
      }
      break;
    case SINGLE_PLAYER:
      switch (key) {
        case KEY_K:
          if (player_data[current_player].action == WAITING_PLAYER_ACTION) {
            player_data[current_player].action = STAY;
            first_time = true;
          }
          break;
        case KEY_S:
          if (player_data[current_player].action == WAITING_PLAYER_ACTION) {
            player_data[current_player].action = SPLIT;
            first_time = true;
          }
          break;
        case KEY_H:
          if (player_data[current_player].action == WAITING_PLAYER_ACTION) {
            player_data[current_player].action = HIT;
            first_time = true;
          }
          break;
        case KEY_D:
          if (player_data[current_player].action == WAITING_PLAYER_ACTION) {
            player_data[current_player].action = DOUBLE;
            first_time = true;
          }
          break;
        case KEY_B:
          if (player_data[current_player].action == WAITING_PLAYER_ACTION) {
            game_action_state = BET;
          }
          break;
        case KEY_P:
          if (game_action_state == WAITING_PLAYER_ACTION_GAME) {
            game_action_state = DEAL;
          }
          break;
        case KEY_ESCAPE:
          app_state = MAIN_MENU;
          break;
        case KEY_ENTER:
          if (game_action_state == END_GAME) {
            game_action_state = START;
          }
          if ((game_action_state == WAITING_PLAYER_ACTION_GAME) && (payout)) {
            payout = false;
            game_action_state = END_GAME;
          }
        case KEY_UP:
          if (game_action_state == WAITING_PLAYER_ACTION_GAME) {
            if ((current_bet_amount + 10 < player_data[current_player].money) > 0) {
              current_bet_amount += 10;
              first_time = true;
            };
          }
          break;
        case KEY_DOWN:
          if (game_action_state == WAITING_PLAYER_ACTION_GAME) {
            if ((current_bet_amount - 10) > 0) {
              current_bet_amount -= 10;
              first_time = true;
            };
          }
          break;
        default:
          break;
      }
      break;
    case MULTI_PLAYER:
      switch (key) {
        case KEY_K:
          if (player_data[current_player].action == WAITING_PLAYER_ACTION) {
            first_time = true;
            player_data[current_player].action = STAY;
          }
          break;
        case KEY_S:
          if (player_data[current_player].action == WAITING_PLAYER_ACTION) {
            first_time = true;
            player_data[current_player].action = SPLIT;
          }
          break;
        case KEY_H:
          if (player_data[current_player].action == WAITING_PLAYER_ACTION) {
            first_time = true;
            player_data[current_player].action = HIT;
          }
          break;
        case KEY_D:
          if (player_data[current_player].action == WAITING_PLAYER_ACTION) {
            first_time = true;
            player_data[current_player].action = DOUBLE;
          }
          break;
        case KEY_B:
          if (game_action_state == WAITING_PLAYER_ACTION) {
            game_action_state = BET;
          }
          break;
        case KEY_R:
          if (game_action_state == WAITING_PLAYER_ACTION) {
            game_action_state = BET;
          }
          break;
        case KEY_ESCAPE:
          app_state = MAIN_MENU;
          break;
        default:
          break;
      }
      break;
    case EXIT:

      break;
    default:
      printf("Invalid application state\n");
      break;
  }
  return 0;
}

int IO_controller_init_sprites() {
  MONEY_BACKGROUND = create_sprite_rectangle(200, 50, xpm_transparency_color(XPM_8_8_8_8));

  BETER_BACKGROUND = create_sprite_rectangle(200, 50, xpm_transparency_color(XPM_8_8_8_8));

  MAIN_MENU_SPRITE = create_sprite((const char **) menu_xpm);
  BACKGROUND_SPRITE = create_sprite((const char **) table);

  // Card sprites
  SA_SPRITE = create_sprite((const char **) Suit_Spades__Number_Ace);     // Ace of Spades
  SK_SPRITE = create_sprite((const char **) Suit_Spades__Number_Queen);   // King of Spades
  SQ_SPRITE = create_sprite((const char **) Suit_Spades__Number_King);    // Queen of Spades
  SJ_SPRITE = create_sprite((const char **) Suit_Spades__Number_Jack);    // Jack of Spades
  ST_SPRITE = create_sprite((const char **) Suit_Spades__Number_10);      // Ten of Spades
  S9_SPRITE = create_sprite((const char **) Suit_Spades__Number_9);       // 9 of Spades
  S8_SPRITE = create_sprite((const char **) Suit_Spades__Number_8);       // 8 of Spades
  S7_SPRITE = create_sprite((const char **) Suit_Spades__Number_7);       // 7 of Spades
  S6_SPRITE = create_sprite((const char **) Suit_Spades__Number_6);       // 6 of Spades
  S5_SPRITE = create_sprite((const char **) Suit_Spades__Number_5);       // 5 of Spades
  S4_SPRITE = create_sprite((const char **) Suit_Spades__Number_4);       // 4 of Spades
  S3_SPRITE = create_sprite((const char **) Suit_Spades__Number_3);       // 3 of Spades
  S2_SPRITE = create_sprite((const char **) Suit_Spades__Number_2);       // 2 of Spades
  HA_SPRITE = create_sprite((const char **) Suit_Hearts__Number_Ace);     // Ace of Hearts
  HK_SPRITE = create_sprite((const char **) Suit_Hearts__Number_King);    // King of Hearts
  HQ_SPRITE = create_sprite((const char **) Suit_Hearts__Number_Queen);   // Queen of Hearts
  HJ_SPRITE = create_sprite((const char **) Suit_Hearts__Number_Jack);    // Jack of Hearts
  HT_SPRITE = create_sprite((const char **) Suit_Hearts__Number_10);      // Ten of Hearts
  H9_SPRITE = create_sprite((const char **) Suit_Hearts__Number_9);       // 9 of Hearts
  H8_SPRITE = create_sprite((const char **) Suit_Hearts__Number_8);       // 8 of Hearts
  H7_SPRITE = create_sprite((const char **) Suit_Hearts__Number_7);       // 7 of Hearts
  H6_SPRITE = create_sprite((const char **) Suit_Hearts__Number_6);       // 6 of Hearts
  H5_SPRITE = create_sprite((const char **) Suit_Hearts__Number_5);       // 5 of Hearts
  H4_SPRITE = create_sprite((const char **) Suit_Hearts__Number_4);       // 4 of Hearts
  H3_SPRITE = create_sprite((const char **) Suit_Hearts__Number_3);       // 3 of Hearts
  H2_SPRITE = create_sprite((const char **) Suit_Hearts__Number_2);       // 2 of Hearts
  DA_SPRITE = create_sprite((const char **) Suit_Diamonds__Number_Ace);   // Ace of Diamonds
  DK_SPRITE = create_sprite((const char **) Suit_Diamonds__Number_King);  // King of Diamonds
  DQ_SPRITE = create_sprite((const char **) Suit_Diamonds__Number_Queen); // Queen of Diamonds
  DJ_SPRITE = create_sprite((const char **) Suit_Diamonds__Number_Jack);  // Jack of Diamonds
  DT_SPRITE = create_sprite((const char **) Suit_Diamonds__Number_10);    // Ten of Diamonds
  D9_SPRITE = create_sprite((const char **) Suit_Diamonds__Number_9);     // 9 of Diamonds
  D8_SPRITE = create_sprite((const char **) Suit_Diamonds__Number_8);     // 8 of Diamonds
  D7_SPRITE = create_sprite((const char **) Suit_Diamonds__Number_7);     // 7 of Diamonds
  D6_SPRITE = create_sprite((const char **) Suit_Diamonds__Number_6);     // 6 of Diamonds
  D5_SPRITE = create_sprite((const char **) Suit_Diamonds__Number_5);     // 5 of Diamonds
  D4_SPRITE = create_sprite((const char **) Suit_Diamonds__Number_4);     // 4 of Diamonds
  D3_SPRITE = create_sprite((const char **) Suit_Diamonds__Number_3);     // 3 of Diamonds
  D2_SPRITE = create_sprite((const char **) Suit_Diamonds__Number_2);     // 2 of Diamonds
  CA_SPRITE = create_sprite((const char **) Suit_Clubs__Number_Ace);      // Ace of Clubs
  CK_SPRITE = create_sprite((const char **) Suit_Clubs__Number_King);     // King of Clubs
  CQ_SPRITE = create_sprite((const char **) Suit_Clubs__Number_Queen);    // Queen of Clubs
  CJ_SPRITE = create_sprite((const char **) Suit_Clubs__Number_Jack);     // Jack of Clubs
  CT_SPRITE = create_sprite((const char **) Suit_Clubs__Number_10);       // Ten of Clubs
  C9_SPRITE = create_sprite((const char **) Suit_Clubs__Number_9);        // 9 of Clubs
  C8_SPRITE = create_sprite((const char **) Suit_Clubs__Number_8);        // 8 of Clubs
  C7_SPRITE = create_sprite((const char **) Suit_Clubs__Number_7);        // 7 of Clubs
  C6_SPRITE = create_sprite((const char **) Suit_Clubs__Number_6);        // 6 of Clubs
  C5_SPRITE = create_sprite((const char **) Suit_Clubs__Number_5);        // 5 of Clubs
  C4_SPRITE = create_sprite((const char **) Suit_Clubs__Number_4);        // 4 of Clubs
  C3_SPRITE = create_sprite((const char **) Suit_Clubs__Number_3);        // 3 of Clubs
  C2_SPRITE = create_sprite((const char **) Suit_Clubs__Number_2);        // 2 of Clubs

  BACK_RED = create_sprite((const char **) BackRed_xpm); // Red Back

  BUTTON_PLAY = create_sprite((const char **) redy);
  BUTTON_HIT = create_sprite((const char **) hit);
  BUTTON_STAND = create_sprite((const char **) stay);
  BUTTON_DOUBLE = create_sprite((const char **) doublebt);
  BUTTON_SPLIT = create_sprite((const char **) split);
  BUTTON_BET = create_sprite((const char **) bet);
  ARROW_UP = create_sprite((const char **) up);
  ARROW_DOWN = create_sprite((const char **) down);

  BUTTON_PLAY_N = create_sprite_neutral((const char **) redy);
  BUTTON_PLAY_N = create_sprite_neutral((const char **) hit);
  BUTTON_PLAY_N = create_sprite_neutral((const char **) stay);
  BUTTON_PLAY_N = create_sprite_neutral((const char **) doublebt);
  BUTTON_PLAY_N = create_sprite_neutral((const char **) split);
  BUTTON_PLAY_N = create_sprite_neutral((const char **) bet);
  BUTTON_PLAY_N = create_sprite_neutral((const char **) up);
  BUTTON_PLAY_N = create_sprite_neutral((const char **) down);

  NUMBER_ZERO = create_sprite((const char **) zero);
  NUMBER_ONE = create_sprite((const char **) one);
  NUMBER_TWO = create_sprite((const char **) two);
  NUMBER_THREE = create_sprite((const char **) three);
  NUMBER_FOUR = create_sprite((const char **) four);
  NUMBER_FIVE = create_sprite((const char **) five);
  NUMBER_SIX = create_sprite((const char **) six);
  NUMBER_SEVEN = create_sprite((const char **) seven);
  NUMBER_EIGHT = create_sprite((const char **) eight);
  NUMBER_NINE = create_sprite((const char **) nine);

  LOSER = create_sprite((const char **) loser);
  WINNER = create_sprite((const char **) win);

  MOUSE_CURSOR= create_sprite((const char **) casino_chip);

  return 0;
}

void test_display_all_cards() {
  Sprite *cards[] = {
    DA_SPRITE, DK_SPRITE, DQ_SPRITE, DJ_SPRITE, DT_SPRITE, D9_SPRITE, D8_SPRITE, D7_SPRITE, D6_SPRITE, D5_SPRITE, D4_SPRITE, D3_SPRITE, D2_SPRITE,
    CA_SPRITE, CK_SPRITE, CQ_SPRITE, CJ_SPRITE, CT_SPRITE, C9_SPRITE, C8_SPRITE, C7_SPRITE, C6_SPRITE, C5_SPRITE, C4_SPRITE, C3_SPRITE, C2_SPRITE, BACK_RED};
  int num_cards = sizeof(cards) / sizeof(cards[0]);
  int cards_per_row = 9;
  int x_start = 0, y_start = 0;
  int x_gap = 128; // no gap, tight grid
  int y_gap = 180;

  for (int i = 0; i < num_cards; i++) {
    int row = i / cards_per_row;
    int col = i % cards_per_row;
    int x = x_start + col * x_gap;
    int y = y_start + row * y_gap;
    if (cards[i]) {
      draw_sprite(cards[i], x, y);
    }
  }
}

Sprite *IO_get_card_sprite(card_t card) {
  switch (card.letter) {
    case 'S':
      switch (card.number) {
        case 2:
          return S2_SPRITE;
          break;
        case 3:
          return S3_SPRITE;
          break;
        case 4:
          return S4_SPRITE;
          break;
        case 5:
          return S5_SPRITE;
          break;
        case 6:
          return S6_SPRITE;
          break;
        case 7:
          return S7_SPRITE;
          break;
        case 8:
          return S8_SPRITE;
          break;
        case 9:
          return S9_SPRITE;
          break;
        case 10:
          return ST_SPRITE;
          break;
        case 11:
          return SJ_SPRITE;
          break;
        case 12:
          return SQ_SPRITE;
          break;
        case 13:
          return SK_SPRITE;
          break;
        case 1:
          return SA_SPRITE;
          break;
        default:
          break;
      }
      break;
    case 'H':
      switch (card.number) {
        case 2:
          return H2_SPRITE;
          break;
        case 3:
          return H3_SPRITE;
          break;
        case 4:
          return H4_SPRITE;
          break;
        case 5:
          return H5_SPRITE;
          break;
        case 6:
          return H6_SPRITE;
          break;
        case 7:
          return H7_SPRITE;
          break;
        case 8:
          return H8_SPRITE;
          break;
        case 9:
          return H9_SPRITE;
          break;
        case 10:
          return HT_SPRITE;
          break;
        case 11:
          return HJ_SPRITE;
          break;
        case 12:
          return HQ_SPRITE;
          break;
        case 13:
          return HK_SPRITE;
          break;
        case 1:
          return HA_SPRITE;
          break;
        default:
          break;
      }
      break;
    case 'C':
      switch (card.number) {
        case 2:
          return C2_SPRITE;
          break;
        case 3:
          return C3_SPRITE;
          break;
        case 4:
          return C4_SPRITE;
          break;
        case 5:
          return C5_SPRITE;
          break;
        case 6:
          return C6_SPRITE;
          break;
        case 7:
          return C7_SPRITE;
          break;
        case 8:
          return C8_SPRITE;
          break;
        case 9:
          return C9_SPRITE;
          break;
        case 10:
          return CT_SPRITE;
          break;
        case 11:
          return CJ_SPRITE;
          break;
        case 12:
          return CQ_SPRITE;
          break;
        case 13:
          return CK_SPRITE;
          break;
        case 1:
          return CA_SPRITE;
          break;
        default:
          break;
      }
      break;
    case 'D':
      switch (card.number) {
        case 2:
          return D2_SPRITE;
          break;
        case 3:
          return D3_SPRITE;
          break;
        case 4:
          return D4_SPRITE;
          break;
        case 5:
          return D5_SPRITE;
          break;
        case 6:
          return D6_SPRITE;
          break;
        case 7:
          return D7_SPRITE;
          break;
        case 8:
          return D8_SPRITE;
          break;
        case 9:
          return D9_SPRITE;
          break;
        case 10:
          return DT_SPRITE;
          break;
        case 11:
          return DJ_SPRITE;
          break;
        case 12:
          return DQ_SPRITE;
          break;
        case 13:
          return DK_SPRITE;
          break;
        case 1:
          return DA_SPRITE;
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  return NULL;
}

int IO_controller_display_bet_buttons() {
  // Button positions for each player (excluding dealer)
  const int player_y[4] = {530, 630, 630, 530}; // Y positions for players 1-4
  const int player_x[4] = {60, 350, 650, 950};  // X positions for players 1-4

  for (int i = 0; i < 4; i++) {

    if (draw_sprite(BUTTON_BET, player_x[i], player_y[i]) != 0) {
      printf("Failed to draw bet button/n");
      return 1;
    }
  }

  return 0;
}

int IO_controller_display_ready_button() {
  const int ready_y = 180; // Y position for ready button
  const int ready_x = 420; // X position for ready button

  // Draw the ready button at the specified position
  if (draw_sprite(BUTTON_PLAY, ready_x, ready_y) != 0) {
    printf("Failed to draw ready button\n");
    return 1;
  }

  return 0;
}

int IO_controller_display_wining_players() {
  printf("current player %d\n", current_player);
  printf("current player %d\n", player_data[current_player].hands[current_hand].wining_hand);
  if (won) {
    printf("winner \n");
    // Draw WINNER sprite above the hand
    if (draw_sprite(WINNER, 0, 0) != 0) {
      printf("Failed to draw win sprite for player %d, hand %d\n", current_player, current_hand);
      return 1;
    }
  }
  else {
    if (draw_sprite(LOSER, 0, 0) != 0) {
      printf("Failed to draw win sprite for player %d, hand %d\n", current_player, current_hand);
      return 1;
    }
  }
  return 0;
}

int IO_controller_display_current_player_money() {
  // Check if player_data is initialized
  if (player_data == NULL) {
    printf("player_data is NULL!\n");
    return 1;
  }

  // Only display for non-dealer players
  if (current_player == 0) {
    return 0; // Skip dealer
  }

  // Position in bottom right corner
  const int x = 900; // 1000px from left
  const int y = 800; // 800px from top

  // Get the current player's money
  int money = player_data[current_player].money;

  // Convert money to string
  char money_str[20];
  snprintf(money_str, sizeof(money_str), "$%d", money);

  if (draw_sprite_and_restore(MONEY_BACKGROUND, x, y) != 0) {
    printf("Failed to draw digit sprite\n");
  }

  // Display each character
  int current_x = x;
  for (int i = 0; money_str[i] != '\0'; i++) {
    Sprite *sprite = NULL;

    switch (money_str[i]) {
      case '0': sprite = NUMBER_ZERO; break;
      case '1': sprite = NUMBER_ONE; break;
      case '2': sprite = NUMBER_TWO; break;
      case '3': sprite = NUMBER_THREE; break;
      case '4': sprite = NUMBER_FOUR; break;
      case '5': sprite = NUMBER_FIVE; break;
      case '6': sprite = NUMBER_SIX; break;
      case '7': sprite = NUMBER_SEVEN; break;
      case '8': sprite = NUMBER_EIGHT; break;
      case '9': sprite = NUMBER_NINE; break;
      default: break;
    }

    if (sprite != NULL) {
      if (draw_sprite(sprite, current_x, y) != 0) {
        printf("Failed to draw digit sprite\n");
      }
      current_x += 40; // Move right for next digit
    }
  }

  return 0;
}


int IO_controller_display_bet_selector() {
  // Position for bet selector (centered)
  const int selector_x = 950;
  const int selector_y = 100;
  const int arrow_offset = 40; // Vertical offset for arrows

  // Draw up arrow
  if (draw_sprite(ARROW_UP, selector_x - 20, selector_y - arrow_offset - 40) != 0) {
    printf("Failed to draw up arrow\n");
    return 1;
  }

  // Draw down arrow
  if (draw_sprite(ARROW_DOWN, selector_x - 20, selector_y + arrow_offset) != 0) {
    printf("Failed to draw down arrow\n");
    return 1;
  }

  // Convert current bet to string
  char bet_str[20];
  snprintf(bet_str, sizeof(bet_str), "%d", current_bet_amount);

  // Draw each digit of the bet amount
  int num_digits = strlen(bet_str);
  int start_x = selector_x - (num_digits * 40) / 2; // Center the number

  if (draw_sprite_and_restore(BETER_BACKGROUND, selector_x - 100, selector_y) != 0) {
    printf("Failed to draw digit sprite\n");
  }

  for (int i = 0; i < num_digits; i++) {
    Sprite *digit_sprite = NULL;

    switch (bet_str[i]) {
      case '0': digit_sprite = NUMBER_ZERO; break;
      case '1': digit_sprite = NUMBER_ONE; break;
      case '2': digit_sprite = NUMBER_TWO; break;
      case '3': digit_sprite = NUMBER_THREE; break;
      case '4': digit_sprite = NUMBER_FOUR; break;
      case '5': digit_sprite = NUMBER_FIVE; break;
      case '6': digit_sprite = NUMBER_SIX; break;
      case '7': digit_sprite = NUMBER_SEVEN; break;
      case '8': digit_sprite = NUMBER_EIGHT; break;
      case '9': digit_sprite = NUMBER_NINE; break;
      default: break;
    }

    if (digit_sprite != NULL) {
      if (draw_sprite(digit_sprite, start_x + i * 40, selector_y) != 0) {
        printf("Failed to draw digit sprite\n");
      }
    }
  }

  return 0;
}
