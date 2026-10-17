#include "mainMenuController.h"

bool twoByteScan = false;
option_menu opt = PLAY;

void handleScanCodeMainMenu(SMine* mine, game_state* state) {
    if (scan_code == TWO_BYTES) {
        twoByteScan = true;
        return;
    }
    else {
        switch (opt)
        {
        case PLAY:
            if ((scan_code == M_S) || ((scan_code == M_DOWN) && twoByteScan)) {
                opt = INSTRUCTIONS;
                setSMinePosition(mine, SEL_INST_X, SEL_INST_Y);
            }
            break;

        case INSTRUCTIONS:
            if ((scan_code == M_S) || ((scan_code == M_DOWN) && twoByteScan)) {
                opt = EXIT;
                setSMinePosition(mine, SEL_EXIT_X, SEL_EXIT_Y);
            }

            else if ((scan_code == M_W) || ((scan_code == M_UP) && twoByteScan)) {
                opt = PLAY;
                setSMinePosition(mine, SEL_PLAY_X, SEL_PLAY_Y);
            }
            break;

        case EXIT:
            if ((scan_code == M_W) || ((scan_code == M_UP) && twoByteScan)) {
                opt = INSTRUCTIONS;
                setSMinePosition(mine, SEL_INST_X, SEL_INST_Y);
            }
            break;        
        
        default:
            break;
        }
        twoByteScan = false;
    }
}
