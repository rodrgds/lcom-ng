#include "player.h"


Player p1 = {"player", 500, 0, 0, 0, 0, 0, 0, 100};
Player p2 = {"player2", 500, 0, 0, 0, 0, 0, 0, 100};
Player pc = {"bbbb", 10000, 0, 0, 0, 0, 0, 0, 100};
GameCoins g_coins = {0, 0, 0, 0}; 

int get_bet_amount() {
  return g_coins.coin5 * 5 + g_coins.coin25 * 25 + g_coins.coin10 * 10 + g_coins.coin100 * 100;
}

int get_coin_index(int x, int y){
  if (x > 560 && x < 660 && y > 360 && y < 460) return 4;
  if (x > 560 && x < 660 && y > 480 && y < 580) return 5;
  if (x > 680 && x < 780 && y > 360 && y < 460) return 6;
  if (x > 680 && x < 780 && y > 480 && y < 580) return 7;
  return -1;
}

void take_coin(int id){
  switch (id) {
      case 4:
      if (5 <= p1.money) {
        g_coins.coin5++;
        p1.money -= 5;
      }
      break;
      case 5:
        if (25 <= p1.money) {
          g_coins.coin25++;
          p1.money -= 25;
        }
      break;
      case 6:
        if (10 <= p1.money) {
          g_coins.coin10++;
          p1.money -= 10;
        }
        break;
      case 7:
        if (100 <= p1.money) {
          g_coins.coin100++;
          p1.money -= 100;
        }
        break;
      default:
        break;
    }
}

int add_coin(int *carnum){
  int r = rand();
  int f = 0;
  if (p1.c1 == 0) {
    p1.a1 = (r % 32) + 1;
    *carnum = 1;
    f++;
  }
  else if (p1.c2 == 0) {
    p1.a2 = (r % 32) + 1;
    *carnum = 2;
    f++;
  }
  else if (p1.c3 == 0) {
    p1.a3 = (r % 32) + 1;
    *carnum = 3;
    f++;
  }
  return f;  
}

void end_game(){
  p1.seconds = 100;
  p1.c1 = p1.c2 = p1.c3 = pc.c1 = pc.c2 = p2.c1 = p2.c2 = p2.c3 = 0;
  g_coins.coin5 = g_coins.coin25 = g_coins.coin10 = g_coins.coin100 = 0;
  return;
}
