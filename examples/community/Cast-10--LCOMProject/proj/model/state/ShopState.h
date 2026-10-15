#pragma once

#include <stdlib.h>

#include "shopBarState.h"

/**
 * @brief Upgrade bars for a single player in the shop.
 */
typedef struct {
    ShopBarState *attackBar; /**< Attack damage upgrade bar. */
    ShopBarState *speedBar; /**< Movement speed upgrade bar. */
    ShopBarState *hpBar; /**< Maximum HP upgrade bar. */
} PlayerShopState;

/**
 * @brief Shop state containing upgrade bars for both players.
 */
typedef struct {
    PlayerShopState *player1Shop; /**< Upgrade bars for player 1. */
    PlayerShopState *player2Shop; /**< Upgrade bars for player 2. */
} ShopState;

/**
 * @brief Allocates and positions all upgrade bars for both players.
 * @param player1X Left edge of the player-1 shop column in pixels.
 * @param player2X Left edge of the player-2 shop column in pixels.
 * @param attackY Y position of the attack bar row.
 * @param defenseY Y position of the defense bar row.
 * @param hpY Y position of the HP bar row.
 * @param barWidth Width of each upgrade bar in pixels.
 * @param barHeight Height of each upgrade bar in pixels.
 * @param buyButtonWidth Width of each buy button in pixels.
 * @param buyButtonHeight Height of each buy button in pixels.
 * @param buyButtonGap Gap between the bar and the buy button in pixels.
 * @return Pointer to the new ShopState, or NULL on allocation failure.
 */
ShopState *shopStateCreate(int player1X,
                           int player2X,
                           int attackY,
                           int defenseY,
                           int hpY,
                           int barWidth,
                           int barHeight,
                           int buyButtonWidth,
                           int buyButtonHeight,
                           int buyButtonGap);

/**
 * @brief Frees a ShopState and all upgrade bars it owns.
 * @param shop ShopState to destroy.
 */
void shopStateDestroy(ShopState *shop);

/**
 * @brief Clears the one-frame click flag on every buy button.
 * @param shop ShopState to reset.
 */
void shopStateResetClicks(ShopState *shop);
