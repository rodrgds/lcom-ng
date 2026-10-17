#include "nameMinigotchiController.h"

#define MAX_AMOUNT_OF_CHARS 10

static NameMinigotchi *nameMinigotchi = NULL;
static collision_type_nameMinigotchi_t currentButtonEvent = NOP_NAMEMINIGOTCHI;
static bool keyReleased = true;

/**
 * @brief Load the minigotchi naming screen (first time)
 */
void nameMinigotchiController_load_nameMinigotchi() {
    if (nameMinigotchi == NULL){
        nameMinigotchi = new_nameMinigotchi();
        nameMinigotchiViewer_setNameMinigotchi(nameMinigotchi);
        setup_text_sprite();
    }
}

/**
 * @brief Function that checks sprite collisions with the cursor
 */
bool nameMinigotchiController_checkCollision(Sprite* sprite, int x, int y) {
    Cursor* cursor = nameMinigotchi_get_cursor(nameMinigotchi);

    int cursorX = cursor_get_x(cursor);
    int cursorY = cursor_get_y(cursor);
    bool beingPressed = get_buttonClicked(cursor);

    int sprite_width = sprite_get_width(sprite);
    int sprite_height = sprite_get_height(sprite);

    int left_upper_corner_x = x;

    int right_upper_corner_x = left_upper_corner_x + sprite_width;

    int right_upper_corner_y = y;

    int left_lower_corner_y = right_upper_corner_y + sprite_height;

    if ((cursorX >= left_upper_corner_x && cursorX <= right_upper_corner_x) && (cursorY >= right_upper_corner_y && cursorY <= left_lower_corner_y) && beingPressed) {
            return true;
    }

    return false;
}

collision_type_nameMinigotchi_t nameMinigotchiController_getButtonEvent() {
    return currentButtonEvent;
}

void nameMinigotchiController_setButtonEvent(collision_type_nameMinigotchi_t ct) {
    currentButtonEvent = ct;
}

void nameMinigotchi_setKeyReleased() {
    keyReleased = true;
}

/**
 * @brief Controller function that is called once every frame and updates / checks every action in the minigotchi naming screen
 */
void nameMinigotchiController_step(uint8_t keyPress) {

    nameMinigotchiController_load_nameMinigotchi();

    if (keyPress != 0x00 && ((keyPress & 0x80) == 0) && keyReleased) {
        keyReleased = false;
        switch (keyPress) {
            case A: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 0);
                break;
            case B:
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 1);
                break;
            case C:
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 2);
                break;
            case D:
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 3);
                break;
            case E:
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 4);
                break;
            case F: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 5);
                break;
            case G: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 6);
                break;
            case H: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 7);
                break;
            case I: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 8);
                break;
            case J: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 9);
                break;
            case K: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 10);
                break;
            case L: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 11);
                break;
            case M: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 12);
                break;
            case N: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 13);
                break;
            case O: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 14);
                break;
            case P: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 15);
                break;
            case Q: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 16);
                break;
            case R: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 17);
                break;
            case S: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 18);
                break;
            case T: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 19);
                break;
            case U: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 20);
                break;
            case V: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 21);
                break;
            case W: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 22);
                break;
            case X: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 23);
                break;
            case Y: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 24);
                break;
            case Z: 
                push_back(nameMinigotchiViewer_get_spriteVector(nameMinigotchi), 25);
                break;
            case BACKSPACE:
                remove_at(nameMinigotchiViewer_get_spriteVector(), nameMinigotchiViewer_get_spriteVector()->size - 1);
            default:
                break;
        }

        if (nameMinigotchiViewer_get_spriteVector()->size >= MAX_AMOUNT_OF_CHARS) {
            remove_at(nameMinigotchiViewer_get_spriteVector(), 10);
        }

    }
    
    Button* continue_button = nameMinigotchi_get_continueButton(nameMinigotchi);
    if (nameMinigotchiController_checkCollision(button_get_sprite(continue_button), button_get_x(continue_button), button_get_y(continue_button))){
        currentButtonEvent = CONTINUE_NAMEMINIGOTCHI;
    }

    Button* quit_button = nameMinigotchi_get_quitButton(nameMinigotchi);
    if (nameMinigotchiController_checkCollision(button_get_sprite(quit_button), button_get_x(quit_button), button_get_y(quit_button))){
        currentButtonEvent = QUIT_NAMEMINIGOTCHI;
    }

}

Cursor* getNameMinigotchiCursor() {
    currentButtonEvent = NOP_NAMEMINIGOTCHI;
    return nameMinigotchi->cursor;
}

void setNameMinigotchiCursor(Cursor* cursor) {
    nameMinigotchi->cursor = cursor;
}

/**
 * @brief Calls the minigotchi naming screen
 */
void nameMinigotchiController_delete_nameMinigotchi() {
    delete_nameMinigotchi(nameMinigotchi);
}
