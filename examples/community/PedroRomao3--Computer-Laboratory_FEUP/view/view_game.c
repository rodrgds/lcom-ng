#include "view_game.h"

int drawCoinSprite(int x, int y, int coinIndex, int s1, int s2, bool f) {
  bool selected = ((x > s1) && (x < s1 + COIN_WIDTH) && (y > s2) && (y < s2 + COIN_HEIGHT)) || f;
  size_t index = selected ? coinIndex : coinIndex - 4;
  if (drawSprite(coins[index], s1, s2)) return 1;
  return 0;
}

int drawMesa() {
  //clearBuffer();
  if (drawSprite(background, SCREEN_START_X, SCREEN_START_Y)) return 1;
  if (drawCard(p1.c1, P1_1CARD)) return 1;
  if (drawCard(p1.c2, P1_2CARD)) return 1;
  if (drawCard(p1.c3, P1_3CARD)) return 1;
  if (drawCard(pc.c1, P2_1CARD)) return 1;
  if (drawCard(pc.c2, P2_2CARD)) return 1;
  if (drawCard(p2.c1, P2_1CARD)) return 1;
  if (drawCard(p2.c2, P2_2CARD)) return 1;
  if (drawCard(p2.c3, P2_3CARD)) return 1;

  // Buttons
  if (drawButton(Hit, HitPressed, HIT_BUTTON_X, HIT_BUTTON_Y)) return 1;
  if (drawButton(Stand, StandPressed, STAND_BUTTON_X, STAND_BUTTON_Y)) return 1;
  if (drawSprite(edit_icon, EDIT_ICON_X, EDIT_ICON_Y_GAME)) return 1;

  // daraw coins menu
  int x = mouse_loc.x;
  int y = mouse_loc.y;

  if (drawCoinSprite(x, y, 0 + 4, COINS_AREA_LEFT, COINS_AREA_TOP, 0)) return 1;
  if (drawCoinSprite(x, y, 1 + 4, COINS_AREA_LEFT, COINS_AREA_BOTTOM - COIN_HEIGHT, 0)) return 1;
  if (drawCoinSprite(x, y, 2 + 4, COINS_AREA_RIGHT - COIN_WIDTH, COINS_AREA_TOP, 0)) return 1;
  if (drawCoinSprite(x, y, 3 + 4, COINS_AREA_RIGHT - COIN_WIDTH, COINS_AREA_BOTTOM - COIN_HEIGHT, 0)) return 1;

  // draw coins on table
  int list_coins[] = {g_coins.coin5, g_coins.coin25, g_coins.coin10, g_coins.coin100};
  y = START_COINS_AREA_Y;
  for (int c = 0; c < 4; ++c) {
    x = START_COINS_AREA_X;
    for (int i = 0; i < list_coins[c]; ++i) {
      if (drawCoinSprite(0, 0, c + 4, x, y, 0)) return 1;
      x += SPACE;
    }
    y += SPACE + 5; // +5 just more space for letters
  }

  // draw bet money
  int sum_money = g_coins.coin5 * 5 + g_coins.coin25 * 25 + g_coins.coin10 * 10 + g_coins.coin100 * 100;
  char sum[5];
  if (sum_money < 10000) {
    snprintf(sum, sizeof(sum), "%04d", sum_money);
    if (drawText(BET_AMOUNT_X, BET_AMOUNT_Y, sum)) return 1;
  }

  // Time
  if (drawTime(TIME_X, TIME_Y, p1.seconds)) return 1;

  // Draw player money
  snprintf(sum, sizeof(sum), "%04d", p1.money);
  if (drawText(MONEY_X, MONEY_Y, sum)) return 1;
  return 0;
}

int drawCard(int card, int pos) {
  if(card == 0) return 0;
  int positions[][2] = {
    {P1_CARD1_X, P1_CARD1_Y},
    {P1_CARD2_X, P1_CARD2_Y},
    {P1_CARD3_X, P1_CARD3_Y},
    {P2_CARD1_X, P2_CARD1_Y},
    {P2_CARD2_X, P2_CARD2_Y},
    {P2_CARD3_X, P2_CARD3_Y}};

  if (drawSprite(cards[card], positions[pos][0], positions[pos][1])) return 1;
  return 0;
}

int drawAnimCard(AnimCard *card){

  int deltaX = (float) (card->final_x - X_POS) / (GAME_FREQUENCY - 1);
  int deltaY = (float) (card->final_y - Y_POS) / (GAME_FREQUENCY - 1);

  int x_pos = X_POS + (int) (deltaX * card->step + 0.5);
  int y_pos = Y_POS + (int) (deltaY * card->step + 0.5);

  size_t index = card->virada ? 0 : (*(card->current_card_id));
  if (drawSprite(cards[index], x_pos, y_pos)) return 1;
  
  card->step++;

  if(card->step == GAME_FREQUENCY){
    *(card->final_card_id) = *(card->current_card_id);
    *(card->current_card_id) = 0;
    card->step = 0;
    return ANIMATION_COMPLETE;
  }

  return 0;
}
