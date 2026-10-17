### GAME CONTROLLER

#### DECK FUNCTIONS

| Function Name                        | Purpose                                 | Done |
|--------------------------------------|-----------------------------------------|------|
| game_controller_generate_deck        | Generates a new 52-card deck            | ✔️   |
| game_controller_shuffle_deck         | Shuffles the current deck               | ✔️   |
| game_controller_reset_deck           | Resets deck to initial unshuffled state | ✔️   |

#### PLAYER ACTION FUNCTIONS

| Function Name                        | Purpose                                 | Done |
|--------------------------------------|-----------------------------------------|------|
| game_controller_hit                  | Player requests a card                  | ✔️   |
| game_controller_stay                 | Player stands                           | ✔️   |
| game_controller_double               | Player doubles down                     | ✔️   |
| game_controller_split                | Player splits a pair                    | ✔️   |

#### AUXILIARY FUNCTIONS

| Function Name                        | Purpose                                 | Done |
|--------------------------------------|-----------------------------------------|------|
| game_controller_init                 | Initializes game state and allocates memory | ✔️   |
| game_controller_place_bet            | Place a bet                             | ✔️   |
| game_controller_free                 | Free all memory                         | ✔️   |
| game_controller_calculate_hand_value | Calculate hand value (handle Aces)      | ✔️   |

#### GAME AND TURN ACTION STATE HANDLER

| Function Name                        | Purpose                                 | Done |
|--------------------------------------|-----------------------------------------|------|
| game_controller_turn_state_handler   | Handles player action states (bet, hit, etc) | ✔️   |
| game_controller_game_state_handler   | Handles game state transitions          | ✔️   |

#### OTHER GAME FUNCTIONS

| Function Name                        | Purpose                                 | Done |
|--------------------------------------|-----------------------------------------|------|
| game_controller_deal_card            | Deals a card to a player or dealer      | ✔️   |
| game_controller_deal_initial_cards   | Deals initial cards to all players      | ✔️   |
| game_controller_payout               | Handle payouts                          | ✔️   |
| game_controller_wining_hand          | Determines if a hand beats the dealer   | ✔️   |
| game_controller_Dealer_Turn          | Dealer plays according to rules         | ✔️   |

### APP CONTROLLER FUNCTIONS


| Function Name                        | Purpose                                 | Done |
|--------------------------------------|-----------------------------------------|------|
| IO_controller_init                   | Initializes IO controller and graphics  | 🟡   |
| IO_controller_init_sprites           | Initializes all sprites                 | 🟡   |
| IO_controller_display_frame          | Displays the current frame based on state | 🟡   |
| IO_controller_display_menu           | Displays the main menu                  | ✔️   |
| IO_controller_display_table          | Displays the game table and buttons     | 🟡   |
| IO_controller_display_player_hands   | Displays all player hands               | 🟡   |
| IO_controller_display_dealer_hand    | Displays the dealer's hand              | 🟡   |
| IO_get_card_sprite                   | Returns the sprite for a given card     | ❌   |
| IO_controller_close                  | Cleans up and returns to text mode      | 🟡   |


### IO CONTROLLER FUNCTIONS

✔️ - Implemented  
❌ - Not implemented
🟡 - Not Finished
