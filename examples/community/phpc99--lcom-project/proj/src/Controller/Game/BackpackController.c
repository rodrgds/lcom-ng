#include "BackpackController.h"

/**
 * @file BackpackController.c
 * @brief Implements logic for selecting backpack slots using keyboard input.
 */

void backpack_select_item(){
    switch (scan_code[0])
    {
    case 0x82:
        backpack_select(get_player()->backpack, 1);
        break;
    case 0x83:
        backpack_select(get_player()->backpack, 2);
        break;
    case 0x84:
        backpack_select(get_player()->backpack, 3);
        break;
    case 0x85:
        backpack_select(get_player()->backpack, 4);
        break;
    case 0x86:
        backpack_select(get_player()->backpack, 5);
        break;
    default:
        break;
    }
}
