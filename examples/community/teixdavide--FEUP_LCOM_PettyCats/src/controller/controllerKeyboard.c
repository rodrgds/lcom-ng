#include "controllerKeyboard.h"
#include "utils/utils.h"


// check when held down
bool wIsDown = false;
bool aIsDown = false;
bool sIsDown = false;
bool dIsDown = false;

// check single press (true on break code; to be turned back to false by reader function)
bool escWasPressed = false;
bool eWasPressed = false;

int update_keys(uint8_t scan_code) {

    switch (scan_code)
    {

    //held down
    case W_MAKE:
        wIsDown = true;
        break;
    case W_BREAK:
        wIsDown = false;
        break;

    case A_MAKE:
        aIsDown = true;
        break;
    case A_BREAK:
        aIsDown = false;
        break;

    case S_MAKE:
        sIsDown = true;
        break;
    case S_BREAK:
        sIsDown = false;
        break;
    
    case D_MAKE:
        dIsDown = true;
        break;
    case D_BREAK:
        dIsDown = false;
        break;

    //single press
    case ESC_BREAK:
        escWasPressed = true;
        break;
    case E_BREAK:
        eWasPressed = true;
        break;
    
    default:
        break;
    }

    return 0;
}

