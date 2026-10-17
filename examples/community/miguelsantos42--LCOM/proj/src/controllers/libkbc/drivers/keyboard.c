#include "src/include/controllers/kbc/keyboard.h"
#include "../i8042.h"
#include "src/include/Macros.h"
#include "src/include/controllers/kbc/KBC.h"


static int hookId = 1;
static uint8_t scanCodes[] = {0,0,0,0,0,0};
static bool readingPause = false;
static bool readingBreak = false;
static uint8_t scanCodeSize = 0;
KeyboardInput *keyboardFormattedInput = NULL;

int (keyboardSubscribeInt)(uint8_t *bit_no){
    *bit_no = BIT(hookId);
    CHECK_RETURN(sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookId));
    return 0;
}

int (keyboardUnsubscribeInt)(){
    CHECK_RETURN(sys_irqrmpolicy(&hookId));
    return 0;
}

static uint8_t (pauseScanCode)(uint8_t n){
    switch (n)
    {
    case 0:
        return PAUSE_FIRST;
    case 1:
        return PAUSE_SECOND;
    case 2:
        return PAUSE_THIRD;
    case 3:
        return PAUSE_FOURTH;
    case 4:
        return PAUSE_FIFTH;
    case 5:
        return PAUSE_SIXTH;
    default:
        return 0;
    }
}
static uint8_t (breakScanCode)(uint8_t n){
    switch (n)
    {
    case 0:
        return ADDITIONAL_BYTE;
    case 1:
        return BREAK_SECOND;
    case 2:
        return BREAK_THIRD;
    case 3:
        return BREAK_FOURTH;
    default:
        return 0;
    }
}

static void (resetScanCode)(){
    scanCodeSize = 0;
    readingPause = false;
    readingBreak = false;
}

static int (validateScanCode)(uint8_t scanCode){
    bool keyboardScanCodeRead = false;
    if(readingBreak){ if(scanCode != breakScanCode(scanCodeSize)){
        resetScanCode();
        validateScanCode(scanCode);
        return 1;
    }}
    else if(readingPause) if(scanCode != pauseScanCode(scanCodeSize)){
        resetScanCode();
        validateScanCode(scanCode);
        return 1;
    }
    scanCodes[scanCodeSize++] = scanCode;
    if(scanCodeSize == 1){
        if(scanCode == pauseScanCode(0)) readingPause = true;
        else if(scanCode != ADDITIONAL_BYTE) keyboardScanCodeRead = true;
    }else if(scanCodeSize == 2){
        if(scanCode == breakScanCode(1)) readingBreak = true;
        else if(!readingPause) keyboardScanCodeRead = true;
    }else if(scanCodeSize == 4 && readingBreak) keyboardScanCodeRead = true;
    else if(scanCodeSize == 6) keyboardScanCodeRead = true;
    if(keyboardScanCodeRead) {
        keyboardFormattedInput = keyboardConstructor(scanCodes, scanCodeSize);
        bool reRead = (strcmp(keyboardFormattedInput->getKey(keyboardFormattedInput),"Error") == 0) && (scanCodeSize == 2);
        resetScanCode();
        if(reRead) return validateScanCode(scanCode);
    }
    return 0;
}

int (readScanCode)(){
    if(keyboardFormattedInput) {
        keyboardFormattedInput->destroy(keyboardFormattedInput);
        keyboardFormattedInput = NULL;
    }
    uint8_t scanCode = 0;
    CHECK_RETURN(kBCReadKeyboard(&scanCode));
    return validateScanCode(scanCode);
}

void (kbc_ih)(){ 
    CHECK_ERROR(readScanCode(),"Unable to read scan code.\n","");
}

bool (isMakeCode)(){
    if(scanCodeSize == 0) return false;
    if(scanCodeSize > 2) return true;
    return IS_MAKE_CODE(scanCodes[scanCodeSize-1]);
}

int (allowKeyboardInterrupts)(){
    uint8_t cmd;
    CHECK_RETURN(kBCCommand(KBC_READ_CMD));
    CHECK_RETURN(kBCReadKeyboard(&cmd));
    cmd |= KBC_INT;
    CHECK_RETURN(kBCCommand(KBC_WRITE_CMD));
    CHECK_RETURN(kBCArgument(cmd));
    return 0;
}
