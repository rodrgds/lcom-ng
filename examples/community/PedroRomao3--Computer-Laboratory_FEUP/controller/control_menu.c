#include "control_menu.h"

bool stand_allowed = false;

int allow_stand(){
  stand_allowed = true;
  return 0;
}

int point(int x) {
  return (x == 0) ? (0) : ((x - 1) / 4 + 6);
}

int check_winner() {
  int my_score = point(p1.c1) + point(p1.c2) + point(p1.c3);
  int pc_score = isMulti ? (point(p2.c1) + point(p2.c2) + point(p2.c3)) : (point(pc.c1) + point(pc.c2)) ;
  int bet_amount = get_bet_amount();

  if ((my_score > MAX_SCORE && pc_score > MAX_SCORE) || my_score == pc_score) {
    p1.money += bet_amount;
    state = NO_WINNER;
  } else if (my_score > MAX_SCORE || (pc_score <= MAX_SCORE && pc_score > my_score)) {
    state = LOOSE;
  } else {
    p1.money += 2 * bet_amount;
    state = WIN;
  }
  
  stand_allowed = false;
  end_game();

  return 0;
}

int ti_menu(){
  if (state == STAND && (isMulti ? stand_allowed : (pc.c1 && pc.c2)))
    check_winner();
  switch (state) {
    case (MENU):
      if (drawMainMenu()) return 1;
      break;
    case (OPRDY):
      if (drawMainMenu()) return 1;
      break;
    case WAIT:
      if (draw_wait()) return 1;
      break;
    case NO_WINNER:
      if (drawNoWinner()) return 1;
      break;
    case WIN:
      if (drawWin()) return 1;
      break;
    case LOOSE:
      if (drawLoose()) return 1;
      break;
    default:
      break;
  }
  return 0;
}

int ki_menu(){
  if(state == WAIT && scancode == Ql){
    state = MENU;
    send_char(QUIT);
    isMulti = false;
  }

  if (scancode == BREAK_ESC) state = END;


  return 0;
}

int handleMenuState(int x, int y) {
  if (mouse_data.lb && x > EXIT_BUTTON_X && x < 452 && y < START_BUTTON_X && y > 310) state = END;  //EXIT
  if (mouse_data.lb && x > START_BUTTON_X && x < 465 && y < 290 && y > START_BUTTON_Y) state = GAME; //START

  if (mouse_data.lb && x > 340 && x < 340 + 157 && y < 394 && y > 370) {
    state = TABLE;
    read_file();
  }
  if (mouse_data.lb && x > MULTIPLAYER_BUTTON_X && x < 450 && y < 460 && y > MULTIPLAYER_BUTTON_Y) {
    if (state != OPRDY){
      state = WAIT;
      send_char(RDY);//send duel
    }
    else{
      accept_duel();
    }
  }
  return 0;
}

int mouseaction_menu(){
  int x = mouse_loc.x;
  int y = mouse_loc.y;

  if(state == MENU || state == OPRDY) handleMenuState(x, y);

  return 0;
}
