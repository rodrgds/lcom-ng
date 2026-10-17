#include "control_game.h"

AnimCard anim_card;
uint8_t multicard1 = DEF_CARD;
uint8_t multicard2 = DEF_CARD;
uint8_t multicard3 = DEF_CARD;
int id = -1;
bool is_digit = false;
uint8_t number_digits = 0;

void assignCard(int x, int y, int* a, int* c, bool f, AnimCard *card){
  AnimCard tmp = {x, y, a, c, f, 0};   
  *card = tmp;
}

int createAnimCard() {
  if (p1.a1) {
    assignCard(P1_CARD1_X, P1_CARD1_Y, &p1.a1, &p1.c1, false, &anim_card);
  }
  if (p1.a2) {
    assignCard(P1_CARD2_X, P1_CARD2_Y, &p1.a2, &p1.c2, false, &anim_card);
  }
  if (p1.a3) {
    assignCard(P1_CARD3_X, P1_CARD3_Y, &p1.a3, &p1.c3, false, &anim_card);
  }
  if (pc.a1) {
    assignCard(P2_CARD1_X, P2_CARD1_Y, &pc.a1, &pc.c1, true, &anim_card);
  }
  if (pc.a2) {
    assignCard(P2_CARD2_X, P2_CARD2_Y, &pc.a2, &pc.c2, true, &anim_card);
  }
  if (p2.a1) {
    assignCard(P2_CARD1_X, P2_CARD1_Y, &p2.a1, &p2.c1, true, &anim_card);
  }
  if (p2.a2) {
    assignCard(P2_CARD2_X, P2_CARD2_Y, &p2.a2, &p2.c2, true, &anim_card);
  }
  if (p2.a3) {
    assignCard(P2_CARD3_X, P2_CARD3_Y, &p2.a3, &p2.c3, true, &anim_card);
  }
  return 0;
}


int p2_step(int card_id) {
  if (anim_card.step != 0) return 0;
  if (card_id == 1 && multicard1 == DEF_CARD) return 0;
  if (card_id == 2 && multicard2 == DEF_CARD) return 0;
  if (card_id == 3 && multicard3 == DEF_CARD) return 0;
  if (card_id == 1) p2.a1 = multicard1;
  if (card_id == 2) p2.a2 = multicard2;
  if (card_id == 3) p2.a3 = multicard3;

  createAnimCard();

  return 0;
}

int pc_step(int card_id) {
  if(anim_card.step != 0) return 0;
  int r = rand();

  if (card_id == 1){
    pc.a1 = (r % 32) + 1;
    createAnimCard();
  } 
  else if (card_id == 2) {
    pc.a2 = (r % 32) + 1;
    createAnimCard();
  }

  return 0;
}

int checkTime() {
  if (counter % GAME_FREQUENCY == 0 && p1.c1 != 0) {
    p1.seconds--;
    if (p1.seconds == 0) state = STAND;
  }
  return 0;
}

void add_next_card(){
  if (isMulti){
    if (p2.c1 == 0) {
      p2_step(1);
    }
    else if (p2.c2 == 0) {
      p2_step(2);
    }
    else if (p2.c3 == 0) {
      p2_step(3);
    }
  }
  else{
    if (pc.c1 == 0) {
      pc_step(1);
    }
    else if (pc.c2 == 0) {
      pc_step(2);
    }
  }
}

int ti_game(){
    switch (state) {
    case GAME:
      if (drawMesa()) return 1;
      if (drawCursor(1)) return 1;
      if (drawText(NAME_X, NAME_Y, p1.name)) return 1;
      checkTime();
      break;
    case ANIM:
      if (drawMesa()) return 1;
      if (drawCursor(0)) return 1;
      if (drawAnimCard(&anim_card) == ANIMATION_COMPLETE) state = GAME;
      if (drawText(NAME_X, NAME_Y, p1.name)) return 1;
      checkTime();
      break;
    case STAND:
      if (drawMesa()) return 1;
      if (drawCursor(0)) return 1;
      if (drawText(NAME_X, NAME_Y, p1.name)) return 1;
      add_next_card();

      if ((multicard1 != DEF_CARD && p2.c1==0) || 
          (multicard2 != DEF_CARD && p2.c2==0) || 
          (multicard3 != DEF_CARD && p2.c3==0) ||
          (pc.c2 == 0 && !isMulti)) drawAnimCard(&anim_card);
      
      break;
    case CHIP:
      if (drawMesa()) return 1;
      drawCoinSprite(0, 0, id, mouse_loc.x - 50, mouse_loc.y - 50, 1);
      if (drawCursor(2)) return 1;
      if (drawText(NAME_X, NAME_Y, p1.name)) return 1;
      break;
    case EDIT:
      if (drawMesa()) return 1;
      if (drawCursor(0)) return 1;
      if (drawText(NAME_X, NAME_Y, p1.name)) return 1;
      if (vg_draw_rectangle(NAME_X + strlen(p1.name) * 32, NAME_Y, 20, 20, 0xFFFFFF)) return 1;
      break;
    case TABLE:
      drawTable(); // TABLE
      drawCursor(0);
      if(is_digit) drawTableDigit();
      break;
    default:
      break;
  }
  return 0;
}

void handle_edit_kbd(){
  if (scancode == ENTER) state = GAME;
  int name_len = strlen(p1.name);
  if (scancode == BACK) {
    if (name_len > 0) {
      p1.name[name_len - 1] = '\0';
    }
  }
  char l = getLetter();
  if (isl(l) && name_len < 10) {
    p1.name[name_len] = l;
    p1.name[name_len + 1] = '\0';
  }
}

void handle_table_kbd(){
  if (scancode == ENTER) {
    if(number_digits > 10) add_game();
    to_save[0] = '\0'; number_digits = 0;
    is_digit = false;
  }

  int size_to_save = strlen(to_save);
  if (scancode == BACK && size_to_save > 0) {
    if(isd(to_save[size_to_save - 1])) number_digits--;
    to_save[size_to_save - 1] = '\0';
  }
  
  char l = getLetter();
  bool add_letter = false;

  add_letter =(size_to_save == 0 && isl(l)) || 
              (isl(l) && size_to_save < 10 && !isd(to_save[size_to_save - 1])) || 
              (isd(l) && number_digits < 13);

  if (isd(l) && number_digits < 13) {
    number_digits++;
  }

  if(add_letter){
    to_save[size_to_save] = l;
    to_save[size_to_save + 1] = '\0';
  }
}

int ki_game(){  
  if ((state == NO_WINNER || state == WIN || state == LOOSE) && !(scancode & BIT(7))) {
    printf("state will becom menu\n");
    state = MENU;
    isMulti=false;
    multicard1 = DEF_CARD;
    multicard2 = DEF_CARD;
    multicard3 = DEF_CARD;
  }

  if (state == EDIT) {
    handle_edit_kbd();
  }

  if(state == TABLE && is_digit){
    handle_table_kbd();
  }
  return 0;
}

int handleChipState(int x, int y) {
  if (!mouse_data.lb) {
    id = -1;
    if(isMulti)state = GAME;
    else state = GAME;
  }
  else if (mouse_loc.x < 400 && mouse_loc.y > 165 && mouse_loc.y < 330) {
    take_coin(id);
    id = -1;
    if(isMulti)state = GAME;
    else state = GAME;
  }
  return 0;
}

int handleEditState(int x, int y) {
  if (mouse_data.lb && !(x > 10 && x < 42 && y > 550 && y < 578)) {
    state = GAME;
  }
  return 0;
}

int handleTableState(int x, int y) {
  if (mouse_data.lb && x > 725 && x < 769 && y > 60 && y < 540){ // red cross
    uint8_t index = (y - 60) / 30;
    delete_game(index);
  }
  if (mouse_data.lb && x > 10 && x < 78 && y > 530 && y < 590){
    save_file();
    state = MENU;
  }
  if (mouse_data.lb && x > 10 && x < 42 && y > 500 && y < 528){
    is_digit = true;
  }
  return 0;
}

int handleGameState(int x, int y) {
  if (mouse_data.lb && x > 679 && x < 744 && y < 41 && y > 20) { // HIT
    int carnum = 0;
    int f = add_coin(&carnum);
    if(isMulti){
      switch (carnum)
    {
    case 1:
      send_char((p1.a1 & 0xFF) | 0x40);// create defines later (estou a usar os 2 msb para passar o numero da carta XX00 0000)
      break;
    case 2:
      send_char((p1.a2 & 0xFF) | 0x80);
      break;
    case 3:
      send_char((p1.a3 & 0xFF) | 0xC0);
      break;
    default:
      break;
    }
    }
    if (f) state = ANIM;
    createAnimCard();
    //if (point(p1.c1) + point(p1.c2) + point(p1.c3) > 21) state = STAND; //PERDEU
  }
  if (mouse_data.lb && x > 679 && x < 780 && y < 84 && y > 62){
    state = STAND;
    send_char(STD);//allow stand on the other pc, ou seja, mudar variavel allow_stand
  } 
  if (mouse_data.lb && ((x > COINS_AREA_LEFT && x < (COINS_AREA_LEFT + COIN_WIDTH) && y > COINS_AREA_TOP && y < COINS_AREA_TOP + COIN_HEIGHT) ||
                        (x > COINS_AREA_LEFT && x < COINS_AREA_LEFT + COIN_WIDTH && y > COINS_AREA_BOTTOM - COIN_HEIGHT && y < COINS_AREA_BOTTOM) ||
                        (x > COINS_AREA_RIGHT - COIN_HEIGHT && x < COINS_AREA_RIGHT && y > COINS_AREA_TOP && y < COINS_AREA_TOP + COIN_HEIGHT) ||
                        (x > COINS_AREA_RIGHT - COIN_HEIGHT && x < COINS_AREA_RIGHT && y > COINS_AREA_BOTTOM - COIN_HEIGHT && y < COINS_AREA_BOTTOM))) { 
    if (!(p1.c1 || p1.c2 || p1.c3)) {
      id = get_coin_index(mouse_loc.x, mouse_loc.y);
      state = CHIP;
    }
  }
  if (mouse_data.lb && x > 10 && x < 42 && y > 550 && y < 578 && p1.c1 == 0 && p1.a1 == 0) {
    state = EDIT;
  }
  return 0;
}

int mouseaction_game(){
  int x = mouse_loc.x;
  int y = mouse_loc.y;

  switch (state) {
    case GAME:
      return handleGameState(x, y);
    case CHIP:
      return handleChipState(x, y);
    case EDIT:
      return handleEditState(x, y);
    case TABLE:
      return handleTableState(x, y);
    default:
      return 0;
  }
}

int serp_card_attribution(uint8_t *char_rec){
  uint8_t char_rec_2msb = *char_rec;
  char_rec_2msb = (char_rec_2msb & 0xC0) >> 6;
  switch (char_rec_2msb)
  {
  case 1:  
    multicard1 = *char_rec & 0x3F;
    printf("card1 received %d \n",multicard1);
    break;
  case 2:  
    multicard2 = *char_rec & 0x3F;
    printf("card2 received %d\n",multicard2);
    break;
  case 3:  
    multicard3 = *char_rec & 0x3F;
    printf("card3 received %d\n",multicard3);
    break;
  default:
    break;
  }
  return 0;
}

int received_quit(){

  if(state == OPRDY) state = MENU;

  return 0;
}

int received_duel(){
  if(!isMulti){
    if(state == WAIT){
      accept_duel();
    }
    else if(state == MENU){
      state = OPRDY;//opponent ready
    }
  }
  return 0;
}
