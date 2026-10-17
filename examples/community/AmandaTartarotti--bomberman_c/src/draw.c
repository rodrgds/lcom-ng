#include "../inc/draw.h"
#include "../inc/graphic.h"
#include "../inc/rtc.h"

//Variáveis globais
extern state_game game_state;

extern vbe_mode_info_t vbe;
extern rtc_time game_clock;
extern char map[15][20];
extern bool change_vram_mode;

char menu[4] = {'x', ' ', ' ', ' '};
extern GamePos arrow_position;
extern GamePos clock_position;
extern GamePos hero_position;
extern GamePos life_position;
extern GamePos bomb_position;
extern GamePos unlimited_bomb_position[10];
extern GamePos monster_position[5];
extern GamePos upgraded_monster_position[1];

int draw_map(){
  int linha = 15;
  int col = 20;
  int XSize = vbe.XResolution/col;
  int monster_count = 0;
  int up_monster_count = 0;

  for(int i = 0; i < col; i++){
    for(int j = 0; j < linha; j++){
      switch (map[j][i])
      {
      case '#':
        if(drawSpriteXPM(bloc, i*XSize, j*XSize) != 0) return 1;
      break;

      case '%':
        if(drawSpriteXPM(bbloc, i*XSize, j*XSize) != 0) return 1;
      break;
      case 'S':
        if(drawSpriteXPM(steven, i*XSize, j*XSize) != 0) return 1;
        hero_position.x = i;
        hero_position.y = j;
      break;
      case '-':
        if(drawSpriteXPM(iblock, i*XSize, j*XSize) != 0) return 1;
      break;
      case ' ':
      break;
      case 'B':
        if(drawSpriteXPM(bomb, i*XSize, j*XSize) != 0) return 1;
      break;
      case 'E':
        if(drawSpriteXPM(explosion, i*XSize, j*XSize) != 0) return 1;
      break;
      
      case 'M':
        if(drawSpriteXPM(monster, i*XSize, j*XSize) != 0) return 1;
        monster_position[monster_count].x = i;
        monster_position[monster_count].y = j;
        monster_count++;
      break;

      case 'U':
        if(drawSpriteXPM(up_monster, i*XSize, j*XSize) != 0) return 1;
        upgraded_monster_position[up_monster_count].x = i;
        upgraded_monster_position[up_monster_count].y = j;
        up_monster_count++;
      break;
      case 'H':
        if(drawSpriteXPM(life, i*XSize, j*XSize) != 0) return 1;
        break;

      case 'L':
        if(drawSpriteXPM(lost_life, i*XSize, j*XSize) != 0) return 1;
        break;
      
      case 'F':
        if(drawSpriteXPM(unlimited,i*XSize,j*XSize)!=0) return 1;
        break;
      
      default:
        break;
      }
    }
  }

  return 0;
}


int erase_screen(){
  int linha = 15;
  int col = 20;
  int xpmSize = vbe.XResolution/col;
  uint32_t color = 0x00;

  for(int i = 0; i < col*xpmSize; i++){
    for(int j = 0; j < linha*xpmSize; j++){
      if(vg_draw_pixel(i,j,color)) return 1;
    }
  }

  return 0;
}

int draw_menu(){
  int entries = 4;
  if(drawSpriteXPM(title, (vbe.XResolution/2)-title->width/2, 80) != 0) return 1;
  for(int i=0; i < entries; i++){
    switch (i)
    {
    case 0:
      if(menu[i] == 'x') {if(drawSpriteXPM(start_O, (vbe.XResolution/2)-start_O->width/2, vbe.YResolution/2-start_O->height/2) != 0) return 1;}
      else if(drawSpriteXPM(start_W, (vbe.XResolution/2)-start_W->width/2, vbe.YResolution/2-start_W->height/2) != 0) return 1;
      break;
    case 1:
      if(menu[i] == 'x') {if(drawSpriteXPM(ins_O, (vbe.XResolution/2)-ins_O->width/2, vbe.YResolution/2-ins_O->height/2 + start_W->height) != 0) return 1;}
      else if(drawSpriteXPM(ins_W, (vbe.XResolution/2)-ins_W->width/2, vbe.YResolution/2-ins_W->height/2 + start_W->height) != 0) return 1;
      break;
    case 2:
      if(menu[i] == 'x') {if(drawSpriteXPM(exit_O, (vbe.XResolution/2)-exit_O->width/2, vbe.YResolution/2-exit_O->height/2 + start_W->height + ins_W->height) != 0) return 1;}
      else if(drawSpriteXPM(exit_W, (vbe.XResolution/2)-exit_W->width/2, vbe.YResolution/2-ins_O->height/2 + start_W->height + ins_W ->height) != 0) return 1;
      break;
    default:
      break;
    }
  }
  return 0;
}

int draw_instructions(){
  game_state = INSTRUCTIONS;
  if(drawSpriteXPM(insGen, (vbe.XResolution/2)-insGen->width/2, 0)) return 1;
  if(drawSpriteXPM(exit_O, (vbe.XResolution/2)-exit_O->width/2, insGen->height)) return 1;

  return 0;
}

int draw_gameover(){
  gameover = create_sprite((xpm_map_t) gameoverofc_xpm);
  if(drawSpriteXPM(gameover, vbe.XResolution/2-gameover->width/2, vbe.YResolution/2-gameover->height/2 )) return 1;
  if(drawSpriteXPM(exit_O, (vbe.XResolution/2)-exit_O->width/2, vbe.YResolution/2-gameover->height/2 + gameover->height + 100)) return 1;

  return 0;
}

int draw_gamewin(){
  gameWin= create_sprite((xpm_map_t) you_win_xpm);
  if(drawSpriteXPM(gameWin, vbe.XResolution/2-gameWin->width/2, vbe.YResolution/2-gameWin->height/2 )) return 1;
  if(drawSpriteXPM(exit_O, (vbe.XResolution/2)-exit_O->width/2, vbe.YResolution/2-gameWin->height/2 + gameWin->height + 100)) return 1;

  return 0;
}

int arrow_draw(){
    if(drawSpriteXPM(arrow_mouse,arrow_position.x, arrow_position.y)) return 1;

    return 0;
}


int print_num(int n, GamePos position){
  switch (n)
  {
  case 0:
    if(drawSpriteXPM(zero,position.x*30,position.y)) return 1;
    break;
  case 1:
    if(drawSpriteXPM(one,position.x*30,position.y)) return 1;
    break;
  case 2:
    if(drawSpriteXPM(two,position.x*30,position.y)) return 1;
    break;
  case 3:
    if(drawSpriteXPM(three,position.x*30,position.y)) return 1;
    break;
  case 4:
    if(drawSpriteXPM(four,position.x*30,position.y)) return 1;
    break;
  case 5:
    if(drawSpriteXPM(five,position.x*30,position.y)) return 1;
    break;
  case 6:
    if(drawSpriteXPM(six,position.x*30,position.y)) return 1;
    break;
  case 7:
    if(drawSpriteXPM(seven,position.x*30,position.y)) return 1;
    break;
  case 8:
    if(drawSpriteXPM(eight,position.x*30,position.y)) return 1;
    break;
  case 9:
    if(drawSpriteXPM(nine,position.x*30,position.y)) return 1;
    break;
  
  default:
  
    break;
  }

  return 0;
}


int create_clock(){
  int sec = game_clock.seconds;
  GamePos sec_pos = {18,5};
  while(sec>0){
    int aux = sec % 10;
    sec = sec / 10;
    if(print_num(aux,sec_pos)) return 1;
    sec_pos.x--;
  } if(sec_pos.x == 17){
    int aux = 0;
    if(print_num(aux,sec_pos)) return 1;

  }else if(sec_pos.x == 18){
    int aux = 0;
    if(print_num(aux,sec_pos)) return 1;
    sec_pos.x--;
    if(print_num(aux,sec_pos)) return 1;
  }

  if(drawSpriteXPM(bar,16*30,5)) return 1;
  if(drawSpriteXPM(black,15*30,5)) return 1;
  

  int min = game_clock.minutes;
  GamePos min_pos = {14,5};
  while(min>0){
    int aux = min % 10;
    min = min / 10;
    if(print_num(aux,min_pos)) return 1;
    min_pos.x--;
  }if(min_pos.x == 13){
    int aux = 0;
    if(print_num(aux,min_pos)) return 1;
  }else if(min_pos.x == 14){
    int aux = 0;
    if(print_num(aux,min_pos)) return 1;
    min_pos.x--;
    if(print_num(aux,min_pos)) return 1;
  }

  if(drawSpriteXPM(bar,12*30,5)) return 1;
  if(drawSpriteXPM(black,11*30,5)) return 1;

  int hour = game_clock.hours;
  GamePos hour_pos = {10,5};
  while(hour>0){
    int aux = hour % 10;
    hour = hour / 10;
    if(print_num(aux,hour_pos)) return 1;
    hour_pos.x--;
  }if(hour_pos.x == 9){
    int aux = 0;
    if(print_num(aux,hour_pos)) return 1;
  }else if(hour_pos.x == 10){
    int aux = 0;
    if(print_num(aux,hour_pos)) return 1;
    hour_pos.x--;
    if(print_num(aux,hour_pos)) return 1;
  }

  return 0;
}
