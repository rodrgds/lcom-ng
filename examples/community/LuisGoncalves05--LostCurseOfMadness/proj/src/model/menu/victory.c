#include "victory.h"

/**
 * @brief Represents a single Victory menu instance.
 *
 * The Victory struct is opaque.
 */
struct Victory {
    Button *nextButton; /**< Pointer to the next button */
    Button *exitButton; /**< Pointer to the exit button */
    Sprite *title;      /**< Pointer to the menu text sprite */
};

Victory *create_victory() {
    Victory *victory = (Victory *) malloc(sizeof(Victory));
    if (victory == NULL) {
        printf("create_victory: malloc failed\n");
        return NULL;
    }

    victory->nextButton = create_button((xpm_map_t) next_button, (xpm_map_t) next_button_selected, BUTTON_NEXT_START_X, VICTORY_BUTTON_START_Y);
    if (victory->nextButton == NULL) {
        printf("create_victory: create_button (nextButton) failed\n");
        free(victory);
        return NULL;
    }

    victory->exitButton = create_button((xpm_map_t) victory_exit_button, (xpm_map_t) victory_exit_button_selected, BUTTON_EXIT_START_X, VICTORY_BUTTON_START_Y);
    if (victory->exitButton == NULL) {
        printf("create_victory: create_button (exitButton) failed\n");
        if (destroy_button(victory->nextButton)) {
            printf("create_victory: destroy_button failed\n");
            return NULL;
        }
        free(victory);
        return NULL;
    }

    victory->title = create_sprite(victory_xpm, x_res / 2 - 715 / 2, y_res / 2 - 129, 0, 0);
    if (!victory->title) {
        printf("create_victory: create_sprite failed\n");
        if (destroy_button(victory->nextButton)) {
            printf("create_victory: destroy_button failed\n");
            return NULL;
        }
        if (destroy_button(victory->exitButton)) {
            printf("create_victory: destroy_button failed\n");
            return NULL;
        }
        free(victory);
        return NULL;
    }

    return victory;
}

int destroy_victory(Victory *victory) {
    if (victory == NULL) {
        printf("destroy_victory: NULL pointer provided\n");
        return 1;
    }
    if (destroy_sprite(victory->title)) {
        printf("destroy_victory: destroy_sprite failed\n");
        return 1;
    }
    if (destroy_button(victory->nextButton)) {
            printf("create_victory: destroy_button failed\n");
            return 1;
        }
        if (destroy_button(victory->exitButton)) {
            printf("create_victory: destroy_button failed\n");
            return 1;
        }
    free(victory);
    return 0;
}

ButtonType victory_get_button(Victory *victory) {
    if (victory == NULL) {
        return BUTTON_NONE;
    }
    if (button_get_selected(victory->nextButton)) {
        return BUTTON_NEXT;
    }
    if (button_get_selected(victory->exitButton)) {
        return BUTTON_EXIT;
    }
    return BUTTON_NONE;
}

int victory_change_button(Victory *victory) {
    if (victory == NULL) {
        printf("victory_change_button: NULL pointer provided\n");
        return 1;
    }
    bool next_selected = button_get_selected(victory->nextButton);
    bool exit_selected = button_get_selected(victory->exitButton);
    if (next_selected == exit_selected) { // no button selected
        button_set_selected(victory->nextButton, true);
        button_set_selected(victory->exitButton, false);
    } else {
        button_set_selected(victory->nextButton, !next_selected);
        button_set_selected(victory->exitButton, !exit_selected);
    }
    return 0;
}

ButtonType victory_click_handler(Victory *victory, uint16_t x, uint16_t y) {
    if (victory == NULL) {
        printf("victory_click_handler: NULL pointer provided\n");
        return BUTTON_NONE;
    }
    if (button_is_selected(victory->nextButton, x, y)) {
        return BUTTON_NEXT;
    }
    if (button_is_selected(victory->exitButton, x, y)) {
        return BUTTON_EXIT;
    }
    return BUTTON_NONE;
}

int draw_victory(Victory *victory, uint8_t *frame_buffer) {
    if (victory == NULL) {
        printf("draw_victory: NULL pointer provided\n");
        return 1;
    }
    if (draw_sprite(victory->title, frame_buffer)) {
        printf("draw_victory: draw_sprite (title) failed\n");
        return 1;
    }
    if (draw_button(victory->nextButton, frame_buffer)) {
        printf("draw_victory: draw_button (nextButton) failed\n");
        return 1;
    }
    if (draw_button(victory->exitButton, frame_buffer)) {
        printf("draw_victory: draw_button (exitButton) failed\n");
        return 1;
    }
    return 0;
}
