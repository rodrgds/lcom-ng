#include "game_controller.h"

extern bool multi_player_mode;
Player_data_t *player_data;
int player_count;
extern application_state_t app_state;
card_t *deck;
game_action_state_t game_action_state;
int current_player = 1;
bool dealers_turn = false;
extern bool first_time;
int current_hand = 0;
bool payout = false;
bool won;
int current_bet_amount = 100;

//-------------------- DECK FUNCTIONS --------------------//
int game_controller_generate_deck() {
  // Generate a deck of cards
  deck = malloc(sizeof(card_t) * 52);
  if (deck == NULL) {
    return 1; // Memory allocation failed
  }

  char suits[4] = {'H', 'D', 'C', 'S'};

  for (int i = 0; i < 52; i++) {
    int suit_index = i / 13;
    int card_value = (i % 13) + 1;

    deck[i].number = card_value;
    deck[i].letter = suits[suit_index]; // H, D, C, S
  }
  return 0;
}

int game_controller_shuffle_deck() {
  if (deck == NULL) {
    return 1; // Invalid state
  }

  // Shuffle the deck using Fisher-Yates algorithm
  for (int i = 51; i > 0; i--) {
    int j = rand() % (i + 1);
    card_t temp = deck[i];
    deck[i] = deck[j];
    deck[j] = temp;
  }
  return 0;
}

int game_controller_reset_deck() {
  if (deck == NULL) {
    return 1; // Invalid state
  }

  char suits[4] = {'H', 'D', 'C', 'S'};

  // Reset the deck to its original state
  for (int i = 0; i < 52; i++) {
    int suit_index = i / 13;
    int card_value = (i % 13) + 1;

    deck[i].number = card_value;
    deck[i].letter = suits[suit_index]; // H, D, C, S
  }
  return 0;
}
//--------------------------------------------------------//

//-------------------- PLAYER ACTION FUNCTIONS --------------------//
int game_controller_split(int player_id, int hand_index) {
  if (player_data == NULL || player_id >= player_count) {
    printf("Invalid player ID\n");
    return 1;
  }

  Player_data_t *player = &player_data[player_id];
  hand_t *hand = &player->hands[hand_index];

  // Check if split is possible
  if (hand->num_cards != 2) {
    printf("Cannot split: hand doesn't have exactly 2 cards\n");
    player->action = WAITING_PLAYER_ACTION;
    return 0;
  }

  if (hand->cards[0].number != hand->cards[1].number) {
    printf("Cannot split: cards are not the same value\n");
    player->action = WAITING_PLAYER_ACTION;
    return 0;
  }

  if (player->money < hand->current_bet) {
    printf("Cannot split: not enough money\n");
    player->action = WAITING_PLAYER_ACTION;
    return 0;
  }

  // Create a new hand
  if (game_controller_add_hand(player_id) != 0) {
    printf("Failed to create new hand\n");
    return 1;
  }
  int new_hand_index = player->hands_count - 1;
  hand_t *new_hand = &player->hands[new_hand_index];

  // Deduct bet for new hand
  player->money -= hand->current_bet;
  new_hand->current_bet = hand->current_bet;

  // Assign a new position to the split hand (original position + 4)
  new_hand->place_on_table = hand->place_on_table + 4;

  // Move second card to new hand
  new_hand->cards = malloc(sizeof(card_t));
  if (new_hand->cards == NULL) {
    printf("Memory allocation failed\n");
    return 1;
  }
  new_hand->cards[0] = hand->cards[1];
  new_hand->num_cards = 1;

  // Remove second card from original hand by reducing count
  hand->num_cards = 1; // Only the first card remains

  // Deal one card to each hand
  if (game_controller_deal_card(player_id, hand_index) != 0) {
    printf("Failed to deal card to original hand\n");
    return 1;
  }
  if (game_controller_deal_card(player_id, new_hand_index) != 0) {
    printf("Failed to deal card to new hand\n");
    return 1;
  }

  printf("Split successful! Created new hand at index %d\n", new_hand_index);
  return 0;
}

int game_controller_stay(int player_id, int hand_index) {
  player_data[player_id].action = END_TURN;
  return 0;
}

int game_controller_double(int player_id, int hand_index) {
  if (player_data == NULL || player_id >= player_count)
    return 1;
  if (hand_index >= player_data[player_id].hands_count)
    return 1;

  hand_t *hand = &player_data[player_id].hands[hand_index];

  // Double the bet
  if (player_data[player_id].money < hand->current_bet)
    return 1;
  player_data[player_id].money -= hand->current_bet;
  hand->current_bet *= 2;

  // Draw one card
  if (game_controller_deal_card(player_id, hand_index) != 0)
    return 1;

  // End turn after double
  player_data[player_id].action = END_TURN;
  return 0;
}

int game_controller_hit(int player_id, int hand_index) {
  if (player_data == NULL || player_id >= player_count) {
    printf("Failed player data error\n");
    return 1;
  }
  if (hand_index >= player_data[player_id].hands_count) {
    printf("Failedhand error\n");
    return 1;
  }

  if (game_controller_deal_card(player_id, hand_index) != 0) {
    printf("Failed to deal card to player\n");
    return 1;
  }

  int value = game_controller_calculate_hand_value(player_data[player_id].hands[hand_index]);

  if (value > 21) {
    player_data[player_id].action = END_TURN; // Bust end turn
  }
  else {
    player_data[player_id].action = WAITING_PLAYER_ACTION;
  }
  return 0;
}
//-----------------------------------------------------------------//

//--------------------- AUXILIARY FUNCTIONS ---------------------//
int game_controller_init() {
  printf("Game Controller init\n");
  player_data = NULL;
  player_count = ((app_state == MULTI_PLAYER) ? 2 : 1) + 1; // Players + Dealer
  deck = NULL;
  game_action_state = START;

  printf("Initializing %d players (including dealer)\n", player_count);

  // Allocate memory for players
  player_data = malloc(sizeof(Player_data_t) * player_count);
  if (player_data == NULL) {
    printf("Failed to allocate memory for players\n");
    return 1;
  }

  // Initialize each player
  for (int i = 0; i < player_count; i++) {
    player_data[i].ID = i + 1;
    player_data[i].money = PLAYER_INITIAL_MONEY;
    player_data[i].hands_count = 0;
    player_data[i].action = WAITING_PLAYER_ACTION;
    player_data[i].hands = NULL;

    printf("  Player %d initialized\n", i);
  }

  // Create initial hand for dealer (player 0)
  printf("Creating dealer's hand...\n");
  if (game_controller_add_hand(0) != 0) {
    printf("Failed to create dealer's hand\n");
    free(player_data);
    return 1;
  }
  printf("  Dealer hand created successfully\n");

  // Generate and shuffle deck
  printf("Generating deck...\n");
  if (game_controller_generate_deck() != 0) {
    printf("Failed to generate deck\n");
    free(player_data);
    return 1;
  }

  printf("Shuffling deck...\n");
  if (game_controller_shuffle_deck() != 0) {
    printf("Failed to shuffle deck\n");
    free(player_data);
    free(deck);
    return 1;
  }

  printf("Game controller initialized successfully\n");
  return 0;
}

int game_controller_calculate_hand_value(hand_t hand) {
  if (hand.cards == NULL || hand.num_cards <= 0) {
    printf("Invalid hand state\n");
    return -1;
  }
  int value = 0;
  int ace_count = 0;

  for (int i = 0; i < hand.num_cards; i++) {
    int card_num = hand.cards[i].number;
    if (card_num >= 2 && card_num <= 10) {
      value += card_num;
    }
    else if (card_num >= 11 && card_num <= 13) {
      value += 10; // Face cards
    }
    else if (card_num == 1) {
      value += 11; // Ace as 11 for now
      ace_count++;
    }
  }

  // Adjust for Aces if value > 21
  while (value > 21 && ace_count > 0) {
    value -= 10;
    ace_count--;
  }

  return value;
}

int game_controller_free() {
  if (player_data != NULL) {
    free(player_data);
    player_data = NULL;
  }
  if (deck != NULL) {
    free(deck);
    deck = NULL;
  }
  return 0;
}
//---------------------------------------------------------------//

//-------------------- GAME AND TURN ACTION STATE HANDLER --------------------//
int game_controller_game_state_handler() {
  // printf("GAME Handler: ");
  // printf("game_action_state: %d\n", game_action_state);
  if (player_data == NULL) {
    printf("Player data is not initialized\n");
    return 1; // Invalid state
  }
  int place = 1;             // temporary comes from IO controller
  bool wants_to_play = true; // temporary comes from IO controller
  // Handle game state
  switch (game_action_state) {
    case START:
      // Initialize game state
      if (game_controller_shuffle_deck() != 0) {
        printf("Failed to Reset deck\n");
        return 1;
      }
      if (game_controller_redy_for_new_round() != 0) {
        printf("Failed to reddy for new round\n");
        return 1;
      }
      break;
    case DEAL:
      // Deal cards to players and dealer
      if (game_controller_deal_initial_cards() != 0) {
        printf("Failed to deal initial cards \n");
        return 1;
      }
      game_action_state = DECISIONS;
      break;
    case BET:
      // Handle betting logic
      for (int i = 1; i < player_count; i++) { // Skip player 0 Dealer
        if ((game_controller_place_bet(i, current_bet_amount, place) != 0) && wants_to_play) {
          printf("Failed to handle turn for player %d\n", i + 1);
          return 1;
        }
      }
      game_action_state = WAITING_PLAYER_ACTION_GAME;
      // game_action_state = DEAL; from IO
      break;
    case DECISIONS:
      // Handle decision making logic
      if (player_data[current_player].action != END_TURN) {
        if (game_controller_turn_state_handler(current_player, current_hand) != 0) {
          printf("Failed to handle player turn \n");
          return 1;
        }
      }
      else {
        if ((multi_player_mode) && (current_player < 2)) {
          current_player++;
        }
        else if (player_data[current_player].hands_count > current_hand) {
          current_hand++;
        }
        else {
          dealers_turn = true;
        }
      }

      if (dealers_turn) {
        if (player_data[0].action != END_TURN) {
          if (game_controller_Dealer_Turn() != 0) {
            printf("Failed during dealers play \n");
          }
        }
        else {
          printf("payout \n");
          game_action_state = PAYOUT;
        }
      }
      break;
    case PAYOUT:
      // Handle payout logic
      if (game_controller_payout(current_player) != 0) {
        printf("Failed to calculate player payout \n");
        return 1;
      }
      if (game_controller_reset_deck() != 0) {
        printf("Failed to Reset deck\n");
        return 1;
      }
      break;
    case END_GAME:
      // Handle end of game logic
      if (game_controller_reset_deck() != 0) {
        printf("Failed to Reset deck\n");
        return 1;
      }
      break;
    case WAITING_PLAYER_ACTION_GAME:
      // WAITING_PLAYER_ACTION
      break;
    default:
      printf("Invalid game action state");
      return 1; // Invalid action state
  }

  return 0; // Success
}

int game_controller_turn_state_handler(int player_id, int hand) {
  if (player_data == NULL || player_id >= player_count) {
    printf("Invalid player state\n");
    return 1;
  }

  // printf("TURN Handler: ");
  // printf("turn state: %d\n", player_data[player_id].action);

  switch (player_data[player_id].action) {
    case STAY:
      if (game_controller_stay(player_id, hand) != 0) {
        printf("Failed to double bet for player %d\n", player_id + 1);
        return 1;
      }
      break;
    case DOUBLE:
      if (game_controller_double(player_id, hand) != 0) {
        printf("Failed to double bet for player %d\n", player_id + 1);
        return 1;
      }
      break;
    case HIT:
      if (game_controller_hit(player_id, hand) != 0) {
        printf("Failed to hit for player %d\n", player_id + 1);
        return 1;
      }
      printf("HIT\n");
      break;
    case SPLIT:
      if (game_controller_split(player_id, hand) != 0) {
        printf("Failed to split for player %d\n", player_id + 1);
        return 1;
      }
      break;
    case WAITING_PLAYER_ACTION:
      // WAITING_PLAYER_ACTION
      break;
    case END_TURN:
      // Player has ended their turn
      game_action_state = WAITING_PLAYER_ACTION_GAME;
      break;
    default:
      printf("Invalid turn action state\n");
      return 1;
  }
  return 0;
}

int game_controller_Dealer_Turn() {
  int hand_value = 0;

  switch (player_data[0].action) {
    case STAY:
      if (game_controller_stay(0, 0) != 0) {
        printf("Failed to double bet for dealer\n");
        return 1;
      }
      first_time = true;
      break;
    case HIT:
      if (game_controller_hit(0, 0) != 0) {
        printf("Failed to hit for dealer\n");
        return 1;
      }
      first_time = true;
      break;
    case WAITING_PLAYER_ACTION:
      // WAITING_PLAYER_ACTION
      hand_value = game_controller_calculate_hand_value(player_data[0].hands[0]);
      if (hand_value < 0) {
        printf("Failed to calculate Dealers Hand Value \n");
        return 1;
      }
      // printf("DEALER HAND VALUE = %d\n",hand_value);
      // Dealer must hit until hand value is 17 or more
      if (hand_value < 17) {
        player_data[0].action = HIT;
      }
      else {
        player_data[0].action = STAY;
      }
      break;
    case END_TURN:
      // Player has ended their turn
      break;
    default:
      printf("Invalid turn action state\n");
      return 1;
  }
  return 0;
}

//-------------------------------------------------------------------------//

int game_controller_place_bet(int player_id, int bet, int place_on_table) {
  if (player_data == NULL || player_id >= player_count) {
    printf("Invalid player ID\n");
    return 1;
  }

  // Add first hand if none exists
  if (player_data[player_id].hands_count == 0) {
    if (game_controller_add_hand(player_id) != 0) {
      printf("Failed to create hand\n");
      return 1;
    }
  }

  // Validate hand exists
  if (player_data[player_id].hands_count == 0) {
    printf("No hands available\n");
    return 1;
  }

  hand_t *hand = &player_data[player_id].hands[0];
  if (bet > player_data[player_id].money) {
    printf("Bet exceeds available money\n");
    return 1;
  }

  if (place_on_table < 1 || place_on_table > 8) {
    printf("Invalid place on table\n");
    return 1;
  }

  hand->current_bet = bet;
  hand->place_on_table = place_on_table;
  player_data[player_id].money -= bet;
  printf("Bet placed successfully\n");
  return 0;
}

int game_controller_add_hand(int player_id) {
  if (player_id >= player_count || player_data == NULL) {
    return 1; // Invalid player
  }

  int new_count = player_data[player_id].hands_count + 1;
  hand_t *new_hands = realloc(player_data[player_id].hands, sizeof(hand_t) * new_count);
  if (!new_hands) {
    return 1; // Allocation failed
  }

  // Initialize new hand
  new_hands[new_count - 1].cards = NULL;
  new_hands[new_count - 1].num_cards = 0;
  new_hands[new_count - 1].current_bet = 0;
  new_hands[new_count - 1].place_on_table = 0;
  new_hands[new_count - 1].wining_hand = false;

  player_data[player_id].hands = new_hands;
  player_data[player_id].hands_count = new_count;
  return 0;
}

int game_controller_deal_card(int player_id, int hand_index) {
  static int current_card = 0; // Track position in deck
  printf("Dealing card to player %d, hand %d...\n", player_id, hand_index);

  if (deck == NULL) {
    printf("ERROR: deck is NULL!\n");
    return 1; // Invalid state
  }

  if (player_data == NULL) {
    printf("ERROR: player_data is NULL!\n");
    return 1; // Invalid state
  }

  if (player_id >= player_count) {
    printf("ERROR: Invalid player_id %d (player_count=%d)\n", player_id, player_count);
    return 1;
  }

  if (hand_index >= player_data[player_id].hands_count) {
    printf("ERROR: Invalid hand_index %d (player %d has %d hands)\n",
           hand_index, player_id, player_data[player_id].hands_count);
    return 1;
  }

  hand_t *hand = &player_data[player_id].hands[hand_index];
  printf("  Current hand: num_cards=%d\n", hand->num_cards);

  // Reallocate memory for new card
  card_t *new_cards = realloc(hand->cards, sizeof(card_t) * (hand->num_cards + 1));
  if (new_cards == NULL) {
    printf("ERROR: Memory allocation failed for new card\n");
    return 1;
  }

  hand->cards = new_cards;

  // Check if we have enough cards in the deck
  if (current_card >= 52) {
    printf("ERROR: No more cards in deck (current_card=%d)\n", current_card);
    return 1;
  }

  // Get card from deck
  card_t card = deck[current_card];
  printf("  Dealing card %d: number=%d, suit=%c\n", current_card, card.number, card.letter);

  // Add card to hand
  hand->cards[hand->num_cards] = card;
  hand->num_cards++;
  current_card++;

  printf("  Hand now has %d cards\n", hand->num_cards);
  return 0;
}

int game_controller_deal_initial_cards() {
  printf("Dealing initial cards...\n");

  if (player_data == NULL) {
    printf("ERROR: player_data is NULL\n");
    return 1;
  }

  if (deck == NULL) {
    printf("ERROR: deck is NULL\n");
    return 1;
  }
  printf("Dealing to %d players\n", player_count);

  for (int i = 0; i < player_count; i++) {
    printf("Player %d: hands_count=%d, hands_ptr=%p\n", i, player_data[i].hands_count, player_data[i].hands);
    if (player_data[i].hands != NULL) {
      // Deal two cards to the player's first hand
      printf("  Dealing to hand 0\n");
    }
    for (int j = 0; j < 2; j++) {
      printf("    Dealing card %d...\n", j + 1);
      int result = game_controller_deal_card(i, 0);

      if (result != 0) {
        printf("    ERROR: Failed to deal card %d to player %d (error=%d)\n", j + 1, i, result);
        return 1;
      }
    }

    printf("  Player %d now has %d cards in hand 0\n", i, player_data[i].hands[0].num_cards);
  }

  printf("Finished dealing initial cards\n");
  return 0;
}

int game_controller_wining_hand(hand_t *hand) {
  int player_value = game_controller_calculate_hand_value(*hand);
  int dealer_value = game_controller_calculate_hand_value(player_data[0].hands[0]);

  hand->wining_hand = false;
  if (player_value > 21)
    return 0;
  if (dealer_value > 21) {
    hand->wining_hand = true;
    won = true;
    printf("winner n\n");
    return 1;
  }
  if (player_value > dealer_value) {
    hand->wining_hand = true;
    won = true;
    printf("winner n\n");
    return 1;
  }
  return 0;
}

int game_controller_payout(int player_id) {
  for (int h = 0; h < player_data[player_id].hands_count; h++) {
    hand_t *hand = &player_data[player_id].hands[h]; // Change to pointer
    if (game_controller_wining_hand(hand)) {
      player_data[player_id].money += 2 * hand->current_bet;
    }
  }
  payout = true;
  game_action_state = WAITING_PLAYER_ACTION_GAME;
  return 0;
}

int game_controller_redy_for_new_round() {
  // Reset current player and hand
  current_player = 1;
  current_hand = 0;
  dealers_turn = false;

  // Reset deck index
  static int current_card = 0;
  current_card = 0;

  // Free all player hands
  for (int i = 0; i < player_count; i++) {
    if (player_data[i].hands != NULL) {
      // Free cards for each hand
      for (int h = 0; h < player_data[i].hands_count; h++) {
        if (player_data[i].hands[h].cards != NULL) {
          free(player_data[i].hands[h].cards);
          player_data[i].hands[h].cards = NULL;
        }
      }
      // Free hands array
      free(player_data[i].hands);
      player_data[i].hands = NULL;
      player_data[i].hands_count = 0;
    }
    // Reset player action state
    player_data[i].action = WAITING_PLAYER_ACTION;
  }

  // Create new hand for dealer
  if (game_controller_add_hand(0) != 0) {
    printf("Failed to create dealer hand for new round\n");
    return 1;
  }

  // Reset game action state
  game_action_state = START;
  won = false;

  printf("Game reset for new round\n");
  return 0;
}
