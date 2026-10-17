#ifndef __INPUT_H
#define __INPUT_H

#include <lcom/lcf.h>
#include "../../model/note.h"
#include "i8042.h"

typedef enum e_input_scheme {
    INPUT_ARROWS,
    INPUT_WASD,
    INPUT_DFJK
} t_input_scheme;

void set_input_scheme(t_input_scheme scheme);
note_lane get_pressed_lane(uint8_t scancode);

#endif
