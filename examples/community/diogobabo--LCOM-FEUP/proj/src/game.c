#include "game.h"

enum STATE GameState;
uint8_t bit_timer; // timer bit used for the mask
uint8_t bit_kb; // keyboard bit used for the mask
uint8_t bit_m; // mouse bit used for the mask
extern uint8_t scode; // scancode kb
extern uint8_t scancode; // scancode mouse
extern int flag; // flag to know if the data from keyboard is valid
extern int flag_m; // flag to know if the data from mouse is valid
enum KEY key; // key press
struct packet pack; // packet
extern int counter; //timer interrupt counter
mouseInfo mouse; // mouseStruct
Character letters[40]; //array to store the alphabet

// loads xpm
uint8_t *menuPlay;
xpm_image_t imgMenuPlay;

uint8_t *menuExit;
xpm_image_t imgMenuExit;


uint8_t *menuLeader;
xpm_image_t imgMenuLeader;

uint8_t *menu;
xpm_image_t imgMenu;

uint8_t *snakeDown;
xpm_image_t imgSnakeDown;

uint8_t *snakeUp;
xpm_image_t imgSnakeUp;

uint8_t *snakeLeft;
xpm_image_t imgSnakeLeft;


uint8_t *snakeRight;
xpm_image_t imgSnakeRight;

uint8_t *snakeTail;
xpm_image_t imgSnakeTail;

uint8_t *fruitI;
xpm_image_t imgFruit;

uint8_t *cursor;
xpm_image_t imgCursor;

uint8_t *resume;
xpm_image_t imgResume;

uint8_t *pauseExit;
xpm_image_t imgPauseExit;

uint8_t *gameBG;
xpm_image_t imgGameBG;

uint8_t *brickD;
xpm_image_t imgBrick;

uint8_t *broken;
xpm_image_t imgBroken;

uint8_t *font;
xpm_image_t imgFont;

uint8_t *BoardMenu;
xpm_image_t imgBoard;

/* Actual Functions */

void gameLoop() {
  int ipc_status;
  message msg;
  int r;
  int idx = 0;
  int numPacket = 0;
  bool firstTimeMenu = true;
  bool firstTimeBoard = true;
  bool firstTimeGame = true;
  uint8_t array[2];
  mouse.delta_x = 576;
  mouse.delta_y = 432;
  uint32_t mask_timer = BIT(bit_timer);
  uint32_t mask_kb = BIT(bit_kb);
  uint32_t mask_mouse = BIT(bit_m);
  int time = 0;

  loadAll();
  GameState = MENU;
  MenuStarter();
  printf("ASDSAd");
  while(GameState != EXIT) { 
        if( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
          printf("driver_receive failed with: %d", r);
          continue;
        }
        if (is_ipc_notify(ipc_status)) { 
            switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: 
                /* Timer Interrupts */
                if (msg.m_notify.interrupts & mask_timer) { 
                  if(firstTimeGame && GameState == PLAY_SOLO) {
                    drawBG();
                    firstTimeGame = false;
                    switchBuffer();
                  }
                  else if(firstTimeMenu && GameState == MENU) {
                    drawDefMenu();
                    firstTimeMenu = false;
                    switchBuffer();
                  }
                  else if(firstTimeBoard && GameState == LEADERBOARD) {
                    drawDefBoard();
                    firstTimeBoard = false;
                    firstTimeMenu = true;
                    switchBuffer();
                  }
                  if(GameState == PAUSE) {
                    firstTimeGame = true;
                  }
                  else if(GameState == LEADERBOARD) {
                    firstTimeMenu = true;
                    firstTimeGame = true;
                  }
                  else if(GameState == MENU) {
                    firstTimeBoard = true;
                    firstTimeGame = true;
                  }
                  else if(GameState == DEAD) {
                    MenuStarter();
                    firstTimeMenu = true;
                    GameState = MENU;
                    firstTimeBoard = true;
                  }
                  timer_int_handler();
                  switchBuffer();
                  InterruptRouter(TIMER);
                  if(counter % 60 == 0) {
                    time++;
                  }
                }
                /* Keyboard Interrupts */
                if(msg.m_notify.interrupts & mask_kb) {
                  kbc_ih();
                  if(flag == 0) {
                    if(idx == 0) {
                      array[idx] = scode;
                      if(scode == DEFSCAN) {
                        idx = 1;
                      }
                      else {
                        InterruptRouter(KBC);
                      }
                    }
                    else {
                      array[idx] = scode;
                      idx = 0;
                      InterruptRouter(KBC);
                    }
                  }

                }
                /* Mouse Interrupts */
                if (msg.m_notify.interrupts & mask_mouse) {
                  mouse_ih();
                  if(flag_m == 0) {
                    if (numPacket == 0 && (scancode & BIT(3))){
                      pack.bytes[0] = scancode;
                      numPacket++;
                    }
                    else if (numPacket == 1) {
                      pack.bytes[1] = scancode;
                      numPacket++;
                    }
                    else if (numPacket == 2) {
                      pack.bytes[2] = scancode;
                      numPacket = 0;
                      mouse_set_packet(&pack);
                      mouse.delta_x += pack.delta_x;
                      mouse.delta_y -= pack.delta_y;
                      mouse.lb = pack.lb;
                      mouse.rb = pack.rb;
                      mouse.mb = pack.mb;
                    }
                  }
                  InterruptRouter(MOUSE);
                }
                break;
              default:
                break; 
            }
        }
  }
}


void InterruptRouter(enum DEVICE device){
  switch (GameState){
    case MENU:
      MenuIH(device);
      break;
    case PAUSE:
      PauseIH(device);
      break;
    case PLAY_SOLO:
      PlaySoloIH(device);
      break;
    case PLAY_MULTIPLAYER:
      PlayMultiplayerIH(device);
      break;
    case DEAD:
      DeadIH(device);
      break;
    case LEADERBOARD:
      BoardIH(device);
      break;
    case EXIT:
      break;
  }
}

void MenuIH(enum DEVICE device){
  switch (device)
  {
  case KBC:
    break;
  
  case TIMER:
    MenuTimerHandler();
    break;

  case MOUSE:
    MenuMouseHandler();
    break;
  default:
    break;
  }
  return;
}
void PauseIH(enum DEVICE device){
  switch (device)
  {
  case TIMER:
    PauseTimerHandler();
    break;
  
  case KBC:
    updateKBC();
    PauseHandlerKBC(key);
    break;
  default:
    break;
  }
  return;
}

void BoardIH(enum DEVICE device){
  switch (device)
  {
  case TIMER:
    break;
  
  case KBC:
    updateKBC();
    BoardHandlerKBC(key);
    break;
  default:
    break;
  }
  return;
}

void PlaySoloIH(enum DEVICE device){
  switch (device) {
    case TIMER:
      InterruptHandlerTimer();
      break;
    case KBC:
      changePosition();
      break;
    case MOUSE:
      InterruptHandlerMouse();
      break;
    default:
      break;
  }
}

void PlayMultiplayerIH(enum DEVICE device){
  return;
}
void DeadIH(enum DEVICE device){
  return;
}

void changePosition() {
  updateKBC(scode);
  InterruptHandlerKBC(key);
}

void updateKBC(){
  switch (scode)
  {
  case 0x50: //ARROW DOWN
    key = DOWN;
    break;
  
  case 0x48: //ARROW UP
    key = UP;
    break;

  case 0x4B: //ARROW LEFT
    key = LEFT;
    break;

  case 0x4D: //ARROW RIGHT
    key = RIGHT;
    break;

  case 0x11: //W
    key = UP;
    break;

  case 0x1F: //S
    key = DOWN;
    break;

  case 0x1E: //A
    key = LEFT;
    break;

  case 0X20: //D
    key = RIGHT;
    break;

  case 0x1C: //ENTER
    key = ENTER;
    break;
  case 0x01: //ESC
    key = ESC;
    break;
    
  default:
    break;
  }
}

void loadAll() {
  menuPlay = xpm_load(play_menu,XPM_8_8_8_8,&imgMenuPlay);
  menuExit = xpm_load(exit_menu,XPM_8_8_8_8,&imgMenuExit);
  menu = xpm_load(first_menu,XPM_8_8_8_8,&imgMenu);
  snakeUp = xpm_load(snake_up,XPM_8_8_8_8,&imgSnakeUp);
  snakeDown = xpm_load(snake_down,XPM_8_8_8_8,&imgSnakeDown);
  snakeLeft = xpm_load(snake_left,XPM_8_8_8_8,&imgSnakeLeft);
  snakeTail = xpm_load(body_snake,XPM_8_8_8_8,&imgSnakeTail);
  snakeRight = xpm_load(snake_right,XPM_8_8_8_8,&imgSnakeRight);
  fruitI = xpm_load(game_fruit,XPM_8_8_8_8,&imgFruit);
  cursor = xpm_load(mouse_xpm,XPM_8_8_8_8,&imgCursor);
  pauseExit = xpm_load(exit_pause,XPM_8_8_8_8,&imgPauseExit);
  resume = xpm_load(resume_game,XPM_8_8_8_8,&imgResume);
  gameBG = xpm_load(game_bg,XPM_8_8_8_8,&imgGameBG);
  brickD = xpm_load(brick,XPM_8_8_8_8,&imgBrick);
  broken = xpm_load(broken_xpm,XPM_8_8_8_8,&imgBroken);
  font = xpm_load(font_xpm,XPM_8_8_8_8,&imgFont);
  menuLeader = xpm_load(leader_part,XPM_8_8_8_8,&imgMenuLeader);
  BoardMenu = xpm_load(leader_menu,XPM_8_8_8_8,&imgBoard);
  initLetters();
}

void initLetters() {
  int x = 0;
  int y = 2;
  int numL = 0;
  int asci = 65;
  for(int i = 0; i < 26; i++) {
    Character tmp;
    tmp.x = x;
    tmp.y = y;
    tmp.letter = asci;
    x+=37;
    letters[numL] = tmp;
    asci++;
    numL++;
  }

  x = 0;
  y = 42;
  int num = 0;
  for(int i = 0; i < 10; i++) {
    Character tmp;
    tmp.x = x;
    tmp.y = y;
    tmp.letter = num;
    x+=37;
    letters[numL] = tmp;
    num++;
    numL++;
  }
}
