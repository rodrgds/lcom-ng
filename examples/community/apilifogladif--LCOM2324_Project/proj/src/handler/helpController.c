#include <lcom/lcf.h>
#include "helpController.h"


int checkKeyboardInputHelp(uint8_t scancode) {
    switch (scancode) {
        case ESC_BREAKCODE:
            return 1;
            break;
        default:
            return 0;
    }
}
