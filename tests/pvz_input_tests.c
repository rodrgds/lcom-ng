#include "input.h"
#include "state.h"
#include "plant_select.h"
#include "constants.h"
#include <stdio.h>
int main(void) {
    int failed = 0;
    set_game_state(STATE_MAIN_MENU);
    handle_keyboard(0x1C);
    handle_keyboard(ENTER_BREAKCODE);
    if (get_game_state() != STATE_PLANT_SELECTION) {
        fprintf(stderr, "First Enter did not open plant selection\n");
        failed = 1;
        handle_keyboard(0x1C);
        handle_keyboard(ENTER_BREAKCODE);
    }
    struct packet p = {.delta_x = SEL_GRID_X + 20 - 400, .delta_y = 300 - (SEL_GRID_Y + 20)};
    handle_mouse(&p);
    p.delta_x = p.delta_y = 0;
    p.lb = true; handle_mouse(&p);
    p.lb = false; handle_mouse(&p);
    if (get_selected_deck_size() != 1) {
        fprintf(stderr, "Clicking an inventory plant did not add it to the deck\n");
        failed = 1;
    }
    if (!failed) puts("First Enter and click-to-deck work");
    return failed;
}
