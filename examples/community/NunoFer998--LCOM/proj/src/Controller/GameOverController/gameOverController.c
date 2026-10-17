#include "gameOverController.h"

bool twoByteScanOver = false;
option_game_over opt_over = RETRY;

void handleScanCodeGameOver(SMine* mine, game_state* state, bool* stateChanged) {
    if (scan_code == TWO_BYTES) {
        twoByteScanOver = true;
        return;
    }
    else {
        switch (opt_over)
        {
        case RETRY:
            if ((scan_code == M_S) || ((scan_code == M_DOWN) && twoByteScanOver)) {
                opt_over = EXIT_OVER;
                setSMinePosition(mine, SEL_OVER_EXIT_X, SEL_OVER_EXIT_Y);
            }
            else if (scan_code == B_ENTER) {
                *state = GAME;
                *stateChanged = true;
                return;
            }
            break;

        case EXIT_OVER:
            if ((scan_code == M_W) || ((scan_code == M_UP) && twoByteScanOver)) {
                opt_over = RETRY;
                setSMinePosition(mine, SEL_RETRY_X, SEL_RETRY_Y);
            }     
            else if (scan_code == B_ENTER) {
                *state = QUIT;
                *stateChanged = true;
                return;
            }
        
        default:
            break;
        }
        twoByteScanOver = false;
    }
}
