#include "instructionmenu.h"
#include "../assets/menus/instructions/menuinstructions_xpm.h"
#include "../assets/menus/instructions/pressanykey_xpm.h"
#include "../assets/menus/instructions/instruction_xpm.h"
#include "../assets/menus/instructions/gameplayinstruction_xpm.h"
#include "../assets/menus/instructionBIGGER_xpm.h"

instruction_menu_t* instruction_menu_init() {
    instruction_menu_t* instruction_menu = (instruction_menu_t *) malloc(sizeof (instruction_menu_t));

    if (instruction_menu == NULL) {
        printf("Error alocating memory for instruction menu structure\n");
        return NULL;
    }

    instruction_menu->instruction_sprite = NULL;
    instruction_menu->gameplay_instruction_sprite = NULL;

    instruction_menu->instruction_sprite = create_sprite((xpm_map_t) instructionBIGGER_xpm);
    if (instruction_menu->instruction_sprite == NULL) {
        destroy_instruction_menu(instruction_menu);
        return NULL;
    }

    instruction_menu->gameplay_instruction_sprite = create_sprite((xpm_map_t) gameplayinstruction_xpm);
    if (instruction_menu->gameplay_instruction_sprite == NULL) {
        destroy_instruction_menu(instruction_menu);
        return NULL;
    }
    return instruction_menu;
}

menu_action_t instructionmenu_process_input(instruction_menu_t *instruction_menu, uint8_t scancode) {
    if (instruction_menu == NULL) {
        return MENU_ACTION_NONE;
    }
    bool is_release = (scancode & 0x80) != 0;
    
    if (!is_release) {
        return MENU_ACTION_MAIN_MENU;
    }
    return MENU_ACTION_NONE;
}

menu_action_t instructionmenu_process_mouse_click(instruction_menu_t *instruction_menu, int mouse_x, int mouse_y) {
    if (instruction_menu == NULL) {
        return MENU_ACTION_NONE;
    }
    
    return MENU_ACTION_MAIN_MENU;
}

void draw_instruction_menu(instruction_menu_t *instruction_menu) {
    if (instruction_menu == NULL) {
        return;
    }

    if (instruction_menu->instruction_sprite != NULL) {
        draw_sprite(instruction_menu->instruction_sprite, 250, 0);
    }
    if (instruction_menu->gameplay_instruction_sprite != NULL) {
        draw_sprite(instruction_menu->gameplay_instruction_sprite, 100, 200);
    }
}

void destroy_instruction_menu(instruction_menu_t *instruction_menu) {
    if (instruction_menu == NULL) {
        return;
    }

    if (instruction_menu->instruction_sprite != NULL) {
        destroy_sprite(instruction_menu->instruction_sprite);
        instruction_menu->instruction_sprite = NULL;
    }

    if (instruction_menu->instruction_sprite != NULL) {
        destroy_sprite(instruction_menu->gameplay_instruction_sprite);
        instruction_menu->gameplay_instruction_sprite = NULL;
    }

    free(instruction_menu);
}
