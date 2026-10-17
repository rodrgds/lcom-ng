#pragma once
#include <lcom/lcf.h>

typedef enum INT_TYPE{
    KEYBOARD,
    MOUSE,
    TIMER,
} INT_TYPE;


typedef enum STATE {
    PLAYING,
    MENU,
    RULES,
    SETUP,
    WAIT,
    EXIT,
    END
} STATE;
