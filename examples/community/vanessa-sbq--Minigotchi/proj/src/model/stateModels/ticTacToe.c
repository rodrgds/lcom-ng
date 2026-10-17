#include "ticTacToe.h"

/**
 * @brief Creates a new TicTacToe "object" and returns a pointer to it.
 */
TicTacToe *new_ticTacToe(){
    // Allocate TTT object
    TicTacToe *ttt = malloc(sizeof(*ttt));
    if (ttt == NULL) return NULL;

    // Initialize TTT
    ttt->quitButton = new_button(990, 50, 8);
    ttt->hostButton = new_button(388, 272, 6);
    ttt->guestButton = new_button(388, 462, 7);
    ttt->cursor = new_cursor(400, 600);
    set_buttonClicked(true, ttt->cursor);
    ttt->ticTacToeState = TX_RX_DECISION_TTT;
    
    srand(time(NULL));
    int reward_id = (rand() % NUM_ITEMS) + 1;
    ttt->reward = new_item(400, 500, reward_id, 1, 100);
    
    for (uint8_t i = 0; i < 9; i++) {
        ttt->otherPlayersMoves[i] = false;
        ttt->currentPlayersMoves[i] = false;
    }

    return ttt;
}

// Getters and Setters

bool* ticTacToe_get_otherPlayersMoves(TicTacToe* ttt) {
    return ttt->otherPlayersMoves;
}

bool* ticTacToe_get_currentPlayersMoves(TicTacToe* ttt) {
    return ttt->currentPlayersMoves;
}

bool ticTacToe_set_otherPlayersMoves(TicTacToe* ttt, uint8_t pos) {
    if (pos >= 9) {
        return false;
    }

    if (!ttt->otherPlayersMoves[pos]) {
        ttt->otherPlayersMoves[pos] = true;
        return true;
    }

    return false;
}

bool ticTacToe_set_currentPlayersMoves(TicTacToe* ttt, uint8_t pos) {
    if (pos >= 9) {
        return false;
    }

    if (!ttt->currentPlayersMoves[pos]) {
        ttt->currentPlayersMoves[pos] = true;
        return true;
    }

    return false;
}

state_ttt_t ticTacToe_get_state(TicTacToe* ttt) {
    return ttt->ticTacToeState;
}

void ticTacToe_set_state(TicTacToe* ttt, state_ttt_t st) {
    ttt->ticTacToeState = st;
}

Item* ticTacToe_get_reward(TicTacToe* ttt){
    return ttt->reward;
}

void ticTacToe_set_reward(TicTacToe* ttt, Item* item){
    ttt->reward = item;
}

void undoChangeInCurrentPlayer(TicTacToe* ttt, uint8_t pos) {
    ttt->currentPlayersMoves[pos] = false;
}

Button* ticTacToe_get_quitButton(TicTacToe *ttt){
    return ttt->quitButton;
}

Button* ticTacToe_get_guestButton(TicTacToe *ttt) {
    return ttt->guestButton;
}
Button* ticTacToe_get_hostButton(TicTacToe *ttt) {
    return ttt->hostButton;
}

Cursor* ticTacToe_get_cursor(TicTacToe *ttt){
    return ttt->cursor;
}


/**
 * @brief Resets the TicTacToe game 
*/
void reset_ticTacToe(TicTacToe *ttt) {
    ttt->ticTacToeState = TX_RX_DECISION_TTT;

    for (uint8_t i = 0; i < 9; i++) {
        ttt->otherPlayersMoves[i] = false;
        ttt->currentPlayersMoves[i] = false;
    }

    srand(time(NULL));
    int reward_id = (rand() % NUM_ITEMS) + 1;
    ttt->reward = new_item(400, 500, reward_id, 1, 100);
}

/**
 * @brief TicTacToe destructor
 */
void delete_ticTacToe(TicTacToe *ttt){ 
    button_delete(ttt->quitButton);
    button_delete(ttt->guestButton);
    button_delete(ttt->hostButton);
    if (ttt->cursor != NULL) {
        free(ttt->cursor);
    }
    if (ttt->reward != NULL) {
        free(ttt->reward);
    }
    free(ttt);
}
