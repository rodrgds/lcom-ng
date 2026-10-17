#include "winScreenController.h"

bool twoByteScanWin = false;
option_win opt_win = PLAY_AGAIN;

void handleScanCodeWinScreen(SMine* mine, game_state* state) {
    if (scan_code == TWO_BYTES) {
        twoByteScanWin = true;
        return;
    }
    else {
        switch (opt_win)
        {
        case PLAY_AGAIN:
            if ((scan_code == M_S) || ((scan_code == M_DOWN) && twoByteScanWin)) {
                opt_win = EXIT_WIN;
                setSMinePosition(mine, SEL_EXIT_WIN_X, SEL_EXIT_WIN_Y);
            }
            break;

        case EXIT_WIN:
            if ((scan_code == M_W) || ((scan_code == M_UP) && twoByteScanWin)) {
                opt_win = PLAY_AGAIN;
                setSMinePosition(mine, SEL_PLAY_AGAIN_X, SEL_PLAY_AGAIN_Y);
            }     
        
        default:
            break;
        }
        twoByteScanWin = false;
    }
}
