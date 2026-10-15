#include "MenuController.h"

void menuControllerUpdate(MenuState *menu, KeyboardState *kbd, MouseState *mouse) {
    if (!menu || !menu->playButton) return;

    if (kbd && kbd->p1.up) {
        menu->playButton->clicked = true;
        return;
    }

    buttonControllerUpdate(menu->playButton, mouse);
}
