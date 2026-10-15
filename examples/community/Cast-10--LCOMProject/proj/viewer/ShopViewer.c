#include "ShopViewer.h"


ShopViewer *shopViewerCreate() {
    ShopViewer *viewer = malloc(sizeof(ShopViewer));
    if (!viewer) return NULL;

    viewer->barViewer = NULL;

    viewer->barViewer = shopBarViewerCreate();

    if (!viewer->barViewer) {
        shopViewerDestroy(viewer);
        return NULL;
    }

    return viewer;
}

void shopViewerDestroy(ShopViewer *viewer) {
    if (!viewer) return;

    if (viewer->barViewer) {
        shopBarViewerDestroy(viewer->barViewer);
    }

    free(viewer);
}

/**
 * @brief Draws all three upgrade bars for a single player's shop column.
 * @param viewer ShopViewer providing the bar renderer.
 * @param playerShop PlayerShopState containing the bars to draw.
 */
static void playerShopViewerDraw(ShopViewer *viewer, PlayerShopState *playerShop,
                                 int points, uint32_t color) {
    if (!viewer || !playerShop) return;

    if (playerShop->attackBar) {
        shopBarViewerDraw(viewer->barViewer, playerShop->attackBar);
    }

    if (playerShop->speedBar) {
        shopBarViewerDraw(viewer->barViewer, playerShop->speedBar);
    }

    if (playerShop->hpBar) {
        shopBarViewerDraw(viewer->barViewer, playerShop->hpBar);
    }
}

void shopViewerDraw(ShopViewer *viewer, ShopState *shop, int p1_points, int p2_points) {
    if (!viewer || !shop) return;

    playerShopViewerDraw(viewer, shop->player1Shop, p1_points, 0x4444FF);
    playerShopViewerDraw(viewer, shop->player2Shop, p2_points, 0xFF4444);
}
