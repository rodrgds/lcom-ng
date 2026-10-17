#include "ticTacToeController.h"

static TicTacToe *ttt = NULL;
static collision_type_ttt_t currentButtonEvent = NOP_TTT;
static bool currentPlayerPlayed = false;
static uint8_t amountReceived = 0;
static uint8_t lastKnownMove = 200;
static bool game_end_screen_flag = true;
static bool canClick = true;

#define SYNC_BYTE 0x01
#define ACK_BYTE 0x02

/**
 * @brief Flag to fix button overlapping (When a button before is clicked)
 */
void ttt_set_canClick(){
    canClick = true;
}

/**
 * @brief Load the Tic Tac Toe minigame (first time)
 */
void ticTacToeController_load_tictactoe() {
     if (ttt == NULL){
        ttt = new_ticTacToe();
        ticTacToeViewer_setTicTacToe(ttt);
    }
}

/**
 * @brief Restore a saved Tic Tac Toe game
 */
TicTacToe* ticTacToeController_get_saved_ttt() {
    currentButtonEvent = NOP_TTT;
    currentPlayerPlayed = false;
    amountReceived = 0;
    lastKnownMove = 200;
    game_end_screen_flag = true;
    return ttt;
}

/**
 * @brief Function that checks sprite collisions with the cursor
 */
bool ticTacToeController_checkCollisionWithSprites(Sprite* sprite, int x, int y) {
    int sprite_width = sprite_get_width(sprite);
    int sprite_height = sprite_get_height(sprite);

    return ticTacToe_checkCollision(x, y, sprite_width, sprite_height);
}

/**
 * @brief Function that checks collisions with the cursor
 */
bool ticTacToe_checkCollision(int x, int y, int width, int height) {
    Cursor* cursor = ticTacToe_get_cursor(ttt);

    int cursorX = cursor_get_x(cursor);
    int cursorY = cursor_get_y(cursor);
    bool beingPressed = get_buttonClicked(cursor);

    if (canClick && beingPressed) return false;
    else canClick = false;
    
    int left_upper_corner_x = x;
    int right_upper_corner_x = left_upper_corner_x + width;
    int right_upper_corner_y = y;
    int left_lower_corner_y = right_upper_corner_y + height;

    if ((cursorX >= left_upper_corner_x && cursorX <= right_upper_corner_x) && (cursorY >= right_upper_corner_y && cursorY <= left_lower_corner_y) && beingPressed) {
            return true;
    }

    return false;
}

collision_type_ttt_t ticTacToeController_getButtonEvent() {
    return currentButtonEvent;
}

void ticTacToeController_setButtonEvent(collision_type_ttt_t ct) {
    currentButtonEvent = ct;
}

/**
 * @brief Sets the board position of the current player
 */
void currentPlayerSetPosition(uint8_t index) {

    if (ticTacToe_get_otherPlayersMoves(ttt)[index]) return; // Player cannot set it's move in other player's move.

    if (ticTacToe_set_currentPlayersMoves(ttt, index)) {
        currentPlayerPlayed = true;
        lastKnownMove = index;

       
        for (uint8_t i = 0; i < 9; i++) {
            serial_send_byte(ticTacToe_get_currentPlayersMoves(ttt)[i]);
        }

        switchBackground(7);
        ticTacToe_set_state(ttt, WAIT_ACK);
    }
}

/**
 * @brief Controller function that is called once every frame and updates / checks every action in the Tic Tac Toe minigame
 */
void ticTacToeController_step() {
    ticTacToeController_load_tictactoe();

    state_ttt_t currentTicTacToe = ticTacToe_get_state(ttt);
    Button* quit_button;
    Button* host_button;
    Button* guest_button;
    uint8_t other_player_index = 200;
    bool errorOccoured = false;

   // Check if "Quit" clicked
    quit_button = ticTacToe_get_quitButton(ttt);
    if (ticTacToeController_checkCollisionWithSprites(button_get_sprite(quit_button), button_get_x(quit_button), button_get_y(quit_button))){
        currentButtonEvent = QUIT_TTT;
    }

    switch (currentTicTacToe) {
        case TX_RX_DECISION_TTT:
            // Check if "Host" clicked
            host_button = ticTacToe_get_hostButton(ttt);
            if (ticTacToeController_checkCollisionWithSprites(button_get_sprite(host_button), button_get_x(host_button), button_get_y(host_button))){
                currentButtonEvent = HOST_TTT;
                switchBackground(6);
                ticTacToe_set_state(ttt, SERIAL_WAIT_HOST_TTT);
            }

            // Check if "Guest" clicked
            guest_button = ticTacToe_get_guestButton(ttt);
            if (ticTacToeController_checkCollisionWithSprites(button_get_sprite(guest_button), button_get_x(guest_button), button_get_y(guest_button))){
                currentButtonEvent = GUEST_TTT;
                switchBackground(7);
                ticTacToe_set_state(ttt, SERIAL_WAIT_GUEST_TTT);
            }

            break;
        case SERIAL_WAIT_HOST_TTT:
            serial_send_byte(SYNC_BYTE);
            ticTacToe_set_state(ttt, WAIT_FOR_GUEST_SYNC_TTT);
            break;
        case SERIAL_WAIT_GUEST_TTT:
            if (isEmpty(serial_get_receive_queue())) break;
            if (dequeue(serial_get_receive_queue()) == SYNC_BYTE) {
                serial_send_byte(ACK_BYTE);
                ticTacToe_set_state(ttt, OTHER_PLAYER_TURN_TTT);
            }
            break;
        case WAIT_FOR_GUEST_SYNC_TTT:
            if (isEmpty(serial_get_receive_queue())) break;
            if (dequeue(serial_get_receive_queue()) == ACK_BYTE) {
                ticTacToe_set_state(ttt, CURRENT_PLAYER_TURN_TTT);
            }
            break;
        case CURRENT_PLAYER_TURN_TTT:
            if (!currentPlayerPlayed) {

                if (ticTacToe_checkCollision(246, 110, 213, 213)) currentPlayerSetPosition(0);
                else if (ticTacToe_checkCollision(464, 110, 213, 213)) currentPlayerSetPosition(1);
                else if (ticTacToe_checkCollision(682, 110, 213, 213)) currentPlayerSetPosition(2);
                else if (ticTacToe_checkCollision(246, 328, 213, 213)) currentPlayerSetPosition(3);
                else if (ticTacToe_checkCollision(464, 328, 213, 213)) currentPlayerSetPosition(4);
                else if (ticTacToe_checkCollision(682, 328, 213, 213)) currentPlayerSetPosition(5);
                else if (ticTacToe_checkCollision(246, 546, 213, 213)) currentPlayerSetPosition(6);
                else if (ticTacToe_checkCollision(464, 546, 213, 213)) currentPlayerSetPosition(7);
                else if (ticTacToe_checkCollision(682, 546, 213, 213)) currentPlayerSetPosition(8);
                else currentPlayerPlayed = false;

            } else {
                ticTacToe_set_state(ttt, OTHER_PLAYER_TURN_TTT);
            }
            
            break;
        case OTHER_PLAYER_TURN_TTT:

            other_player_index = 200;
            if (isEmpty(serial_get_receive_queue())) break;

            bool val = dequeue(serial_get_receive_queue());

            if (val) {
                ticTacToe_set_otherPlayersMoves(ttt, amountReceived);
            }
            amountReceived++;

            if (amountReceived == 8) {
                val = dequeue(serial_get_receive_queue());
                
                if (val) {
                    ticTacToe_set_otherPlayersMoves(ttt, amountReceived);
                }

                // Check if position is valid and acknoledge
                errorOccoured = false;
                for (uint8_t i = 0; i < 9; i++) {
                    if (!ticTacToe_get_currentPlayersMoves(ttt)[i] || !ticTacToe_get_otherPlayersMoves(ttt)[i]) continue;
                    if (ticTacToe_get_currentPlayersMoves(ttt)[i] == ticTacToe_get_otherPlayersMoves(ttt)[i]) {
                        serial_send_byte(0);
                        errorOccoured = true;
                    }
                }

                if (errorOccoured) {
                    errorOccoured = false;
                    break;
                }
                
                serial_send_byte(1);
                switchBackground(6);
                currentPlayerPlayed = false;
                ticTacToe_set_state(ttt, CURRENT_PLAYER_TURN_TTT);
                amountReceived = 0;
            }
            
            break;
        case EXIT_TTT:
            ticTacToeMayBeResetNow();
            canClick = true;
            break;
        case WAIT_ACK:
            if (isEmpty(serial_get_receive_queue())) return;

            // Check the integrity of data
            // Condition is true if integrity not met.
            if (!(bool)dequeue(serial_get_receive_queue())) {
                // Undo last change.
                if (lastKnownMove >= 8) ticTacToe_set_state(ttt, EXIT_TTT);
                undoChangeInCurrentPlayer(ttt, lastKnownMove);
                currentPlayerPlayed = false;
                ticTacToe_set_state(ttt, CURRENT_PLAYER_TURN_TTT);
                return;
            }

            ticTacToe_set_state(ttt, OTHER_PLAYER_TURN_TTT);
            break;
        default:
            break;
    }
    
    if (currentTicTacToe == CPLAYER_LOST || currentTicTacToe == CPLAYER_WON || currentTicTacToe == TIE_TTT){
        if (game_end_screen_flag){
            game_end_screen_flag = false;
            switchBackground(5); 
            
            if (currentTicTacToe == CPLAYER_WON){
                Item* reward = ticTacToe_get_reward(ttt);
                database_add_food_to_array(getDatabase(), item_get_id(reward));
            }
        }
        return;
    }

    bool playerWon = false;
    
    if (ticTacToe_get_otherPlayersMoves(ttt)[0] && ticTacToe_get_otherPlayersMoves(ttt)[1] && ticTacToe_get_otherPlayersMoves(ttt)[2]) playerWon = true;
    if (ticTacToe_get_otherPlayersMoves(ttt)[3] && ticTacToe_get_otherPlayersMoves(ttt)[4] && ticTacToe_get_otherPlayersMoves(ttt)[5]) playerWon = true;
    if (ticTacToe_get_otherPlayersMoves(ttt)[6] && ticTacToe_get_otherPlayersMoves(ttt)[7] && ticTacToe_get_otherPlayersMoves(ttt)[8]) playerWon = true;
    if (ticTacToe_get_otherPlayersMoves(ttt)[0] && ticTacToe_get_otherPlayersMoves(ttt)[3] && ticTacToe_get_otherPlayersMoves(ttt)[6]) playerWon = true;
    if (ticTacToe_get_otherPlayersMoves(ttt)[1] && ticTacToe_get_otherPlayersMoves(ttt)[4] && ticTacToe_get_otherPlayersMoves(ttt)[7]) playerWon = true;
    if (ticTacToe_get_otherPlayersMoves(ttt)[2] && ticTacToe_get_otherPlayersMoves(ttt)[5] && ticTacToe_get_otherPlayersMoves(ttt)[8]) playerWon = true;
    if (ticTacToe_get_otherPlayersMoves(ttt)[0] && ticTacToe_get_otherPlayersMoves(ttt)[4] && ticTacToe_get_otherPlayersMoves(ttt)[8]) playerWon = true;
    if (ticTacToe_get_otherPlayersMoves(ttt)[2] && ticTacToe_get_otherPlayersMoves(ttt)[4] && ticTacToe_get_otherPlayersMoves(ttt)[6]) playerWon = true;
    

    if (playerWon) {
        ticTacToe_set_state(ttt, CPLAYER_LOST);
        return;
    }

    if (ticTacToe_get_currentPlayersMoves(ttt)[0] && ticTacToe_get_currentPlayersMoves(ttt)[1] && ticTacToe_get_currentPlayersMoves(ttt)[2]) playerWon = true;
    if (ticTacToe_get_currentPlayersMoves(ttt)[3] && ticTacToe_get_currentPlayersMoves(ttt)[4] && ticTacToe_get_currentPlayersMoves(ttt)[5]) playerWon = true;
    if (ticTacToe_get_currentPlayersMoves(ttt)[6] && ticTacToe_get_currentPlayersMoves(ttt)[7] && ticTacToe_get_currentPlayersMoves(ttt)[8]) playerWon = true;
    if (ticTacToe_get_currentPlayersMoves(ttt)[0] && ticTacToe_get_currentPlayersMoves(ttt)[3] && ticTacToe_get_currentPlayersMoves(ttt)[6]) playerWon = true;
    if (ticTacToe_get_currentPlayersMoves(ttt)[1] && ticTacToe_get_currentPlayersMoves(ttt)[4] && ticTacToe_get_currentPlayersMoves(ttt)[7]) playerWon = true;
    if (ticTacToe_get_currentPlayersMoves(ttt)[2] && ticTacToe_get_currentPlayersMoves(ttt)[5] && ticTacToe_get_currentPlayersMoves(ttt)[8]) playerWon = true;
    if (ticTacToe_get_currentPlayersMoves(ttt)[0] && ticTacToe_get_currentPlayersMoves(ttt)[4] && ticTacToe_get_currentPlayersMoves(ttt)[8]) playerWon = true;
    if (ticTacToe_get_currentPlayersMoves(ttt)[2] && ticTacToe_get_currentPlayersMoves(ttt)[4] && ticTacToe_get_currentPlayersMoves(ttt)[6]) playerWon = true;
    
   
    if (playerWon) {
        ticTacToe_set_state(ttt, CPLAYER_WON);
        return;
    }

    
    uint8_t counter = 0;
    for (uint8_t i = 0; i < 9; i++){
        if (ticTacToe_get_currentPlayersMoves(ttt)[i] || ticTacToe_get_otherPlayersMoves(ttt)[i]) counter++;
    } 
    
    if (!playerWon && counter == 9){ // Tie
        ticTacToe_set_state(ttt, TIE_TTT);
        return;
    } 
    
}

/**
 * @brief Function that calls the Tic Tac Toe destructor
 */
void ticTacToeController_delete_tictactoe() {
    delete_ticTacToe(ttt);
}


Cursor* getTicTacToeCursor() {
    currentButtonEvent = NOP_TTT;
    return ttt->cursor;
}

void setTicTacToeCursor(Cursor* cursor)  {
    ttt->cursor = cursor;
}

/**
 * @brief Function that randomizes the reward (after each game)
 */
void ticTacToeController_randomize_reward(){
    srand(time(NULL));
    int reward_id = (rand() % NUM_ITEMS) + 1;
    Item* next_reward = new_item(700, 500, reward_id, 1, 100);
    ticTacToe_set_reward(ttt, next_reward);
}
