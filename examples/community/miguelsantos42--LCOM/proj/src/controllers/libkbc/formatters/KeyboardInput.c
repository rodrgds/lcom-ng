#include "src/include/controllers/kbc/KeyboardInput.h"
#include "../i8042.h"
#include "src/include/Macros.h"
#include <string.h>
#include <stdlib.h>

struct KeyboardData{
    char *key;
    bool released;
};

typedef struct KeyboardData KeyboardData;

static void (destroyData)(KeyboardData *data){
    if(!data) return;
    if(data->key) free(data->key);
    free(data);
}

static void (destroy)(KeyboardInput *input){
    if(!input) return;
    KeyboardData *data = input->data;
    if(data) destroyData(data);
    free(input);
}

static const char* (getKey)(const KeyboardInput *input){
    KeyboardData *data;
    getData(input,data, NULL);
    return data->key;
}

static bool (wasReleased)(const KeyboardInput *input){
    KeyboardData *data;
    getData(input, data, true);
    return data->released;
}

static KeyboardData *(dataConstructor)(const char *key, bool released){
    if(!key) return NULL;
    KeyboardData *data = malloc(sizeof(KeyboardData));
    if(!data) return NULL;
    data->key = strdup(key);
    if(!data->key){
        free(data);
        return NULL;
    }
    data->released = released;
    return data;
}

static KeyboardInput *(linkInput)(const char *key, bool released){
    if(!key) return NULL;
    KeyboardData *data = dataConstructor(key,released);
    if(!data) return NULL;
    KeyboardInput *input = malloc(sizeof(KeyboardInput));
    if(!input) {
        destroyData(data);
        return NULL;
    }
    input->data = data;
    input->destroy = destroy;
    input->getKey = getKey;
    input->wasReleased = wasReleased;
    return input;
}

KeyboardInput *(keyboardConstructor)(const uint8_t* scanCode, const uint8_t scanCodeSize){
    if(scanCodeSize != 1 && scanCodeSize != 2 && scanCodeSize != 4 && scanCodeSize != 6) return NULL;
    if(scanCodeSize == 2 && scanCode[0] != ADDITIONAL_BYTE) return NULL;
    bool released = true;
    char *key = NULL;
    if(scanCodeSize == 6){
        key = "Pause";
        released = false;
    }else if(scanCodeSize == 4){
        key = "Break";
        released = false;
    }else{
        released = IS_BREAK_CODE(scanCode[scanCodeSize-1]);
        if(scanCodeSize == 1){
            switch (CREATE_MAKE_CODE(*scanCode))
            {
            case MAKE_F1:
                key = "Function1";
                break;
            case MAKE_F2:
                key = "Function2";
                break;
            case MAKE_F3:
                key = "Function3";
                break;
            case MAKE_F4:
                key = "Function4";
                break;
            case MAKE_F5:
                key = "Function5";
                break;
            case MAKE_F6:
                key = "Function6";
                break;
            case MAKE_F7:
                key = "Function7";
                break;
            case MAKE_F8:
                key = "Function8";
                break;
            case MAKE_F9:
                key = "Function9";
                break;
            case MAKE_F10:
                key = "Function10";
                break;
            case MAKE_F11:
                key = "Function11";
                break;
            case MAKE_F12:
                key = "Function12";
                break;
            case MAKE_1:
                key = "Number1";
                break;
            case MAKE_2:
                key = "Number2";
                break;
            case MAKE_3:
                key = "Number3";
                break;
            case MAKE_4:
                key = "Number4";
                break;
            case MAKE_5:
                key = "Number5";
                break;
            case MAKE_6:
                key = "Number6";
                break;
            case MAKE_7:
                key = "Number7";
                break;
            case MAKE_8:
                key = "Number8";
                break;
            case MAKE_9:
                key = "Number9";
                break;
            case MAKE_0:
                key = "Number0";
                break;
            case MAKE_Q:
                key = "LetterQ";
                break;
            case MAKE_W:
                key = "LetterW";
                break;
            case MAKE_E:
                key = "LetterE";
                break;
            case MAKE_R:
                key = "LetterR";
                break;
            case MAKE_T:
                key = "LetterT";
                break;
            case MAKE_Y:
                key = "LetterY";
                break;
            case MAKE_U:
                key = "LetterU";
                break;
            case MAKE_I:
                key = "LetterI";
                break;
            case MAKE_O:
                key = "LetterO";
                break;
            case MAKE_P:
                key = "LetterP";
                break;
            case MAKE_A:
                key = "LetterA";
                break;
            case MAKE_S:
                key = "LetterS";
                break;
            case MAKE_D:
                key = "LetterD";
                break;
            case MAKE_F:
                key = "LetterF";
                break;
            case MAKE_G:
                key = "LetterG";
                break;
            case MAKE_H:
                key = "LetterH";
                break;
            case MAKE_J:
                key = "LetterJ";
                break;
            case MAKE_K:
                key = "LetterK";
                break;
            case MAKE_L:
                key = "LetterL";
                break;
            case MAKE_Z:
                key = "LetterZ";
                break;
            case MAKE_X:
                key = "LetterX";
                break;
            case MAKE_C:
                key = "LetterC";
                break;
            case MAKE_V:
                key = "LetterV";
                break;
            case MAKE_B:
                key = "LetterB";
                break;
            case MAKE_N:
                key = "LetterN";
                break;
            case MAKE_M:
                key = "LetterM";
                break;
            case MAKE_ESC:
                key = "Escape";
                break;
            case MAKE_BKSP:
                key = "Backspace";
                break;
            case MAKE_TAB:
                key = "Tab";
                break;
            case MAKE_ENTER:
                key = "Enter";
                break;
            case MAKE_LCTRL:
                key = "ControlLeft";
                break;
            case MAKE_LSHIFT:
                key = "ShiftLeft";
                break;
            case MAKE_RSHIFT:
                key = "ShiftRight";
                break;
            case MAKE_LALT:
                key = "AltLeft";
                break;
            case MAKE_SPACE:
                key = "Space";
                break;
            case MAKE_CAPSLOCK:
                key = "CapsLock";
                break;
            case MAKE_NUMLOCK:
                key = "NumpadLock";
                break;
            case MAKE_SCROLLLOCK:
                key = "ScrollLock";
                break;
            case MAKE_NP_0:
                key = "NumpedNumber0";
                break;
            case MAKE_NP_1:
                key = "NumpedNumber1";
                break;
            case MAKE_NP_2:
                key = "NumpedNumber2";
                break;
            case MAKE_NP_3:
                key = "NumpedNumber3";
                break;
            case MAKE_NP_4:
                key = "NumpedNumber4";
                break;
            case MAKE_NP_5:
                key = "NumpedNumber5";
                break;
            case MAKE_NP_6:
                key = "NumpedNumber6";
                break;
            case MAKE_NP_7:
                key = "NumpedNumber7";
                break;
            case MAKE_NP_8:
                key = "NumpedNumber8";
                break;
            case MAKE_NP_9:
                key = "NumpedNumber9";
                break;
            case MAKE_NP_MINUS:
                key = "NumpedMinus";
                break;
            case MAKE_NP_PLUS:
                key = "NumpedPlus";
                break;
            case MAKE_NP_PERIOD:
                key = "NumpedPeriod";
                break;
            case MAKE_NP_ASTERISK:
                key = "NumpedAsterisk";
                break;
            case MAKE_MINUS:
                key = "Minus";
                break;
            case MAKE_EQUAL:
                key = "Equal";
                break;
            case MAKE_LBRACK:
                key = "BrackLeft";
                break;
            case MAKE_RBRACK:
                key = "BrackRight";
                break;
            case MAKE_SEMI:
                key = "SemiColon";
                break;
            case MAKE_QUOTE:
                key = "SingleQuote";
                break;
            case MAKE_BACKTICK:
                key = "BackTick";
                break;
            case MAKE_BKSLASH:
                key = "SlashBackward";
                break;
            case MAKE_COMMA:
                key = "Comma";
                break;
            case MAKE_PERIOD:
                key = "Period";
                break;
            case MAKE_SLASH:
                key = "SlashFoward";
                break;
            default:
                key = "Error";
                break;
            }
        }else{
            switch (CREATE_MAKE_CODE(scanCode[1]))
            {
            case MAKE_PRTSC:
                key = "PrintScreen";
                break;
            case MAKE_INSERT:
                key = "Insert";
                break;
            case MAKE_HOME:
                key = "Home";
                break;
            case MAKE_PAGEU:
                key = "PageUp";
                break;
            case MAKE_DELETE:
                key = "Delete";
                break;
            case MAKE_END:
                key = "End";
                break;
            case MAKE_PAGED:
                key = "PageDown";
                break;
            case MAKE_RARROW:
                key = "ArrowRight";
                break;
            case MAKE_LARROW:
                key = "ArrowLeft";
                break;
            case MAKE_DARROW:
                key = "ArrowDown";
                break;
            case MAKE_UARROW:
                key = "ArrowUp";
                break;
            case MAKE_NP_SLASH:
                key = "NumpadSlash";
                break;
            case MAKE_NP_ENTER:
                key = "NumpadEnter";
                break;
            default:
                key = "Error";
                break;
            }
        }
    }
    KeyboardInput *input = linkInput(key, released);
    if(!input) return NULL;
    return input;
}

KeyboardInput *(keyboardCopyConstructor)(const KeyboardInput *originalInput){
    KeyboardData *originalData;
    getData(originalInput,originalData,NULL);
    return linkInput(originalData->key, originalData->released);
}
