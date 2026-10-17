#include "mainRoomViewer.h"

static MainRoom *mainRoom;
static Sprite* currentWindow;
static Vector *_spriteVector = NULL;

void setRTCWindow(Sprite* window) {
    currentWindow = window;
}

void mainRoomViewer_setMainRoom(MainRoom* mr){
    mainRoom = mr;
}

Vector* getSpriteVector() {
    return _spriteVector;
}

void setSpriteVector(Vector* vec) {
    _spriteVector = vec;
}

/**
 * @brief Calls draw function of each element the main room contains
 */
void mainRoomViewer_draw(){
    vg_clear_screen();

    // Background elements
    if (!mainRoom_get_hotbar(mainRoom)->hidden) {
        wrapper_draw_background(1);
    } else {
        wrapper_draw_background(0);
    }
    
    drawSprite(currentWindow, 879, 285);
    draw_minigotchi(mainRoom_get_minigotchi(mainRoom));

    // Foreground elements
    draw_button(mainRoom_get_miniGamesButton(mainRoom));
    draw_button(mainRoom_get_quitButton(mainRoom));
    draw_bar(mainRoom_get_happinessBar(mainRoom));
    draw_bar(mainRoom_get_hungerBar(mainRoom));

    if (!mainRoom_get_hotbar(mainRoom)->hidden) {
        drawHotbar(mainRoom_get_hotbar(mainRoom));
        draw_slider(mainRoom_get_hotbar(mainRoom));
    }

     if (_spriteVector != NULL) {
        uint16_t currentOffset = 0;
        for (size_t i = 0; i < _spriteVector->size; i++) {
            Sprite* sss = get_text_sprite_from_index(_spriteVector->data[i]);

            if (sss == NULL) continue;
            drawSprite(sss, 550 + currentOffset, 10);
            currentOffset += 50;
        }
    }

    draw_cursor(mainRoom_get_cursor(mainRoom));

    vg_page_flip();
}

