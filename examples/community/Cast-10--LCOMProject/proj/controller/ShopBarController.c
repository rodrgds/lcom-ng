#include "ShopBarController.h"

ShopBarController *shopBarControllerCreate() {
    ShopBarController *controller = malloc(sizeof(ShopBarController));
    if (!controller) return NULL;

    controller->buyButton = buttonStateCreate();
    if (!controller->buyButton) {
        free(controller);
        return NULL;
    }

    return controller;
}

void shopBarControllerDestroy(ShopBarController *controller) {
    if (!controller) return;

    if (controller->buyButton) {
        buttonStateDestroy(controller->buyButton);
    }

    free(controller);
}

/**
 * @brief Tests whether a screen coordinate is inside a button's bounding rectangle.
 * @param button Button to test (NULL returns false).
 * @param mouse_x Cursor X coordinate in pixels.
 * @param mouse_y Cursor Y coordinate in pixels.
 * @return true if (mouse_x, mouse_y) is within the button bounds.
 */
static bool isInsideButton(ButtonState *button, int mouse_x, int mouse_y) {
    if (!button) return false;

    return mouse_x >= button->x &&
           mouse_x < button->x + button->width &&
           mouse_y >= button->y &&
           mouse_y < button->y + button->height;
}

bool shopBarControllerUpdate(ShopBarController *controller,
                             ShopBarState *bar,
                             int mouse_x,
                             int mouse_y,
                             bool mouse_pressed,
                             Champion *owner) {
    if (!controller || !bar || !bar->buyButton) {
        return false;
    }

    ButtonState *buyButton = controller->buyButton;

    if (!buyButton) {
        buyButton = bar->buyButton;
    }

    buyButton->x = bar->buyButton->x;
    buyButton->y = bar->buyButton->y;
    buyButton->width = bar->buyButton->width;
    buyButton->height = bar->buyButton->height;

    buyButton->hovered = isInsideButton(buyButton, mouse_x, mouse_y);
    bar->buyButton->hovered = buyButton->hovered;

    if (buyButton->hovered && mouse_pressed && !shopBarStateIsMaxed(bar) && owner && championCurrentPoints(owner) >= 2) {
        buyButton->clicked = true;
        championUpdatePoints(owner, -2);
        shopBarStateBuy(bar);

        switch (bar->upgrade_type) {
            case UPGRADE_DAMAGE: championAddAttackDmg(owner, 5);  break;
            case UPGRADE_SPEED:  championAddSpeed(owner, 1);      break;
            case UPGRADE_HP:
                championAddMaxHp(owner, 20);
                championAddHp(owner, 20);
                break;
        }

        return true;
    }

    buyButton->clicked = false;
    return false;
}
