#ifndef INPUT_H
#define INPUT_H

#include "pvz_platform.h"
#include <stdbool.h>

#include "plant.h"
#include "system.h"
#include "input.h"
#include "state.h"
#include "timer.h"
#include "constants.h"
#include "scene.h"
#include "video.h"
#include "render.h"
#include "zombie.h"
#include "wave.h"
#include "world.h"
#include "board.h"
#include "lawnmower.h"
#include "plant_select.h"
#include "rtc.h"

typedef enum {
    MOUSE_STATE_IDLE,
    MOUSE_STATE_HOLDING_PLANT
} MouseState;

/* Called on every keyboard interrupt */
void handle_keyboard_interrupt(void);

/* Called on every mouse interrupt */
void handle_mouse_interrupt(void);

/* Handles a decoded keyboard scancode */
void handle_keyboard(uint8_t scancode);

/* Handles a complete 3-byte mouse packet */
void handle_mouse(struct packet *p);

int get_mouse_x(void);
int get_mouse_y(void);

/* Drag & drop no ecrã de seleção de plantas */
bool is_dragging_plant(void);
PlantType get_dragged_plant_type(void);

bool is_shovel_mode_active(void);
bool is_keyboard_grid_active(void);
bool is_keyboard_input_mode_active(void);
int get_keyboard_selected_row(void);
int get_keyboard_selected_col(void);
PlantType get_selected_seed_type(void);

MouseState get_mouse_state(void);
PlantType get_selected_plant_type(void);

#endif /* INPUT_H */
