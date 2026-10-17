#include "nameMinigotchiViewer.h"

static NameMinigotchi *nameMinigotchi;
static Vector* spriteVector;

void nameMinigotchiViewer_setNameMinigotchi(NameMinigotchi* nm){
    nameMinigotchi = nm;
    spriteVector = init_vector(1);
}

Vector* nameMinigotchiViewer_get_spriteVector() {
    return spriteVector;
}

/**
 * @brief Calls draw function of each element the nameMinigotchi contains
 */
void nameMinigotchiViewer_draw(){
    vg_clear_screen();

    // Background elements
    wrapper_draw_background(0);

    // Foreground elements
    draw_button(nameMinigotchi_get_continueButton(nameMinigotchi));
    draw_button(nameMinigotchi_get_quitButton(nameMinigotchi));

    uint16_t currentOffset = 0;
    for (size_t i = 0; i < spriteVector->size; i++) {
        drawSprite(get_text_sprite_from_index(spriteVector->data[i]), 418 + currentOffset, 226);
        currentOffset += 55;
    }

    draw_cursor(nameMinigotchi_get_cursor(nameMinigotchi));

    vg_page_flip();
}

