#ifndef _TICTACTOE_H_
#define _TICTACTOE_H_

#include "../../viewer/guiDrawer.h"

#include "../button.h"
#include "../cursor.h"
#include "../item.h"

typedef enum {TX_RX_DECISION_TTT, CURRENT_PLAYER_TURN_TTT, OTHER_PLAYER_TURN_TTT, CPLAYER_WON, CPLAYER_LOST, EXIT_TTT, WAIT_ACK, TIE_TTT, SERIAL_WAIT_HOST_TTT, SERIAL_WAIT_GUEST_TTT, WAIT_FOR_GUEST_SYNC_TTT} state_ttt_t;

typedef struct{
    Button *guestButton;
    Button *hostButton;
    Button *quitButton;
    Cursor *cursor;
    state_ttt_t ticTacToeState;
    Item* reward;
    bool otherPlayersMoves[9];
    bool currentPlayersMoves[9];
} TicTacToe;

TicTacToe *new_ticTacToe();

void reset_ticTacToe();

void delete_ticTacToe(TicTacToe *ttt);

state_ttt_t ticTacToe_get_state(TicTacToe* ttt);
void ticTacToe_set_state(TicTacToe* ttt, state_ttt_t st);

Item* ticTacToe_get_reward(TicTacToe* ttt);
void ticTacToe_set_reward(TicTacToe* ttt, Item* item);

bool* ticTacToe_get_otherPlayersMoves(TicTacToe* ttt);
bool* ticTacToe_get_currentPlayersMoves(TicTacToe* ttt);

bool ticTacToe_set_otherPlayersMoves(TicTacToe* ttt, uint8_t pos);
bool ticTacToe_set_currentPlayersMoves(TicTacToe* ttt, uint8_t pos);

void undoChangeInCurrentPlayer(TicTacToe* ttt, uint8_t pos);

Button* ticTacToe_get_quitButton(TicTacToe *ttt);
Button* ticTacToe_get_guestButton(TicTacToe *ttt);
Button* ticTacToe_get_hostButton(TicTacToe *ttt);
Cursor* ticTacToe_get_cursor(TicTacToe *ttt);

#endif /* _TICTACTOE_H */
