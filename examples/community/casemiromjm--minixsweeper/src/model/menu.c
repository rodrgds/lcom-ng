#include "menu.h"
#include "../view/graphics.h"
#include <stdlib.h>

Menu* menu = NULL;

Menu* create_menu() {
    Menu* menu = (Menu*)malloc(sizeof(Menu));
    if (!menu) return NULL;

    // Initialize buttons
    int button_x = (SCREEN_WIDTH / 2) - (BUTTON_WIDTH / 2);

    int total_height = 4 * BUTTON_HEIGHT + 3 * BUTTON_SPACING;
    int first_button_y = (SCREEN_HEIGHT - total_height) / 2 + 50;
    
    menu->start_button      = (Button){button_x, first_button_y, BUTTON_WIDTH, BUTTON_HEIGHT, false};

    menu->mode_button = (Button){button_x, first_button_y + (BUTTON_HEIGHT + BUTTON_SPACING) * 1, BUTTON_WIDTH, BUTTON_HEIGHT, false};
    menu->left_mode_arrow  = (Button){button_x - ARROW_WIDTH - 10, first_button_y + (BUTTON_HEIGHT + BUTTON_SPACING) * 1 + (BUTTON_HEIGHT - ARROW_HEIGHT) / 2, ARROW_WIDTH, ARROW_HEIGHT, false};
    menu->right_mode_arrow = (Button){button_x + BUTTON_WIDTH + 10, first_button_y + (BUTTON_HEIGHT + BUTTON_SPACING) * 1 + (BUTTON_HEIGHT - ARROW_HEIGHT) / 2, ARROW_WIDTH, ARROW_HEIGHT, false};
    
    menu->difficulty_button       = (Button){button_x, first_button_y + (BUTTON_HEIGHT + BUTTON_SPACING) * 2, BUTTON_WIDTH, BUTTON_HEIGHT, false};
    menu->left_difficulty_arrow  = (Button){button_x - ARROW_WIDTH - 10, first_button_y + (BUTTON_HEIGHT + BUTTON_SPACING) * 2 + (BUTTON_HEIGHT - ARROW_HEIGHT) / 2, ARROW_WIDTH, ARROW_HEIGHT, false};
    menu->right_difficulty_arrow = (Button){button_x + BUTTON_WIDTH + 10, first_button_y + (BUTTON_HEIGHT + BUTTON_SPACING) * 2 + (BUTTON_HEIGHT - ARROW_HEIGHT) / 2, ARROW_WIDTH, ARROW_HEIGHT, false};
    
    menu->exit_button       = (Button){button_x, first_button_y + (BUTTON_HEIGHT + BUTTON_SPACING) * 3, BUTTON_WIDTH, BUTTON_HEIGHT, false};

    menu->selected_difficulty = EASY;
    menu->selected_mode = NORMAL;

    return menu;
}

void destroy_menu(Menu* menu) {
    if (menu) {
        free(menu);
    }
}

void reset_menu_buttons(Menu* menu) {
    if (!menu) return;
    menu->start_button.hovered = false;
    menu->left_mode_arrow.hovered = false;
    menu->right_mode_arrow.hovered = false;
    menu->left_difficulty_arrow.hovered = false;
    menu->right_difficulty_arrow.hovered = false;
    menu->exit_button.hovered = false;
}

