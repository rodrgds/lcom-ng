#include "ShopController.h"

/**
 * @brief Processes mouse input for all three upgrade bars of a single player's shop column.
 * @param controller ShopController providing the bar controller.
 * @param playerShop PlayerShopState containing the bars to update.
 * @param mouse Current mouse state providing cursor position and click events.
 */
static void playerShopControllerUpdate(ShopController *controller,
                                       PlayerShopState *playerShop,
                                       MouseState *mouse,
                                       Champion *owner,
                                       bool mouse_pressed) {
    if (!controller || !playerShop || !mouse) return;

    if (playerShop->attackBar) {
        shopBarControllerUpdate(
            controller->barController,
            playerShop->attackBar,
            mouse->x,
            mouse->y,
            mouse_pressed,
            owner
        );
    }

    if (playerShop->speedBar) {
        shopBarControllerUpdate(
            controller->barController,
            playerShop->speedBar,
            mouse->x,
            mouse->y,
            mouse_pressed,
            owner
        );
    }

    if (playerShop->hpBar) {
        shopBarControllerUpdate(
            controller->barController,
            playerShop->hpBar,
            mouse->x,
            mouse->y,
            mouse_pressed,
            owner
        );
    }
}

ShopController *shopControllerCreate() {
    ShopController *controller = malloc(sizeof(ShopController));
    if (!controller) return NULL;

    controller->barController = shopBarControllerCreate();

    if (!controller->barController) {
        free(controller);
        return NULL;
    }

    return controller;
}

void shopControllerDestroy(ShopController *controller) {
    if (!controller) return;

    if (controller->barController) {
        shopBarControllerDestroy(controller->barController);
    }

    free(controller);
}

void shopControllerUpdate(ShopController *controller, ShopState *shop, MouseState *mouse, GameState *game) {
    if (!controller || !shop || !mouse || !game) return;

    playerShopControllerUpdate(controller, shop->player1Shop, mouse, game->p1, mouse->p1.attack);
    playerShopControllerUpdate(controller, shop->player2Shop, mouse, game->p2, mouse->p1.attack);
}

