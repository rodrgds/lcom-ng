#include "model.h"

/** @defgroup model model
 * * @{
 */

extern uint8_t Kscancode;
extern vbe_mode_info_t modeInfos;
extern int count;
extern int byteCount;
extern struct packet pp;
extern int finalScore;
extern bool ableToSend;
uint8_t kbdIRQ;
uint8_t timerIRQ;
uint8_t mouseIRQ;
uint8_t serialIRQ;
int groundLevel = 700;

Player *player1;
Player *player2;

Planes *planeArray;
Bombs *bombsArray;
Bombs *bulletArray;

Sprite *mouseIcon;
Sprite *lifeBar1;
Sprite *lifeBar2;
Sprite *back;
Sprite *menu;
Sprite *multi;
Sprite *score;

int nLifes1 = 0;
int nLifes2 = 0;
int num = 0;
GameState gameState = MENU;
bool multiplayer = false;
uint8_t otherChoosedPlayer = 0;
uint8_t myPlayer = CHOOSE_P1;
bool planeLeft = true;
int endscore = 0;
int timerCounter = 0;
int planeFreq = 200;


/**
 * @brief Creates objects for moving elements and sprites for non moving ones.
 * @details Objects are sprites that can move.
 */
void (loadSprites)(){
  player1 = create_object((xpm_map_t) player_xpm, 0, 0);
  player2 = create_object((xpm_map_t) player2_xpm, 0, 0);
  back = create_sprite((xpm_map_t) background_xpm, 0, 0);
  multi = create_sprite((xpm_map_t) multi_xpm, 0, 0);
  score = create_sprite((xpm_map_t) score_xpm, 0, 0);
  planeArray = create_PlanesArray(30); 
  bombsArray = create_bombsArray(50);
  bulletArray = create_bombsArray(50);
  lifeBar1 = create_sprite((xpm_map_t) healthBar_xpm, 100, 50);
  lifeBar2 = create_sprite((xpm_map_t) healthBar_xpm, 800, 50);
  menu = create_sprite((xpm_map_t) menu_xpm, 0, 0);
  mouseIcon = create_sprite((xpm_map_t) mouse_xpm, 0, 0);
  loadScoreSprites();
}

/**
 * @brief Enables all interrupts.
 * @return 0 in case of success.
 */
int enableInterrupts(){
  if (keyboard_subscribe_int(&kbdIRQ) != 0) return 1;
  if (timer_subscribe_int(&timerIRQ) != 0) return 1;
  if (mouse_subscribe_int(&mouseIRQ) != 0) return 1;
  if (serial_subscribe(&serialIRQ) != 0) return 1;
  return 0;
}

/**
 * @brief Converts a scancode to its corresponding character representation.
 * 
 * @param scancode The scancode to be converted.
 * @return The corresponding character, or '\0' if the scancode is not recognized.
 */
char convertScancodeToChar(uint8_t scancode) {
    switch (scancode) {
        case MAKE_A_KEY:
            return 'A';
        case MAKE_D_KEY:
            return 'D';
        case BREAK_D_KEY:
            return 'd';
        case BREAK_A_KEY:
            return 'a';
        case SPACE_BAR_KEY:
            return ' ';
        default:
            return '\0'; 
    }
}

/**
 * @brief Maps a character to its corresponding scancode.
 * 
 * @param received_char The character to be mapped.
 * @return The corresponding scancode, or 0 if the character is not recognized.
 */
uint8_t mapCharToKey(char received_char) {
    switch (received_char) {
        case 'A':
            return MAKE_A_KEY;
        case 'D':
            return MAKE_D_KEY;
        case 'd':
            return BREAK_D_KEY;
        case 'a':
            return BREAK_A_KEY;    
        case ' ':
            return SPACE_BAR_KEY;
        default:
            return 0; // Indicates unmapped character
    }
}

/**
 * @brief Sends a scancode by converting it to a character and adding it to the send queue.
 * 
 * @param scancode The scancode to be sent.
 * @return 0 if the scancode is successfully converted and added to the send queue, 1 otherwise.
 */
int sendScancode(uint8_t scancode) {
    char character = convertScancodeToChar(scancode);
    if (character != '\0') {
        addToSend(character);
        return 0; 
    } else {
        return 1; 
    }
}


/**
 * @brief Inside each interrupt, perform actions so the game makes sense.
 * @details Timer interrupt: creates bombs, planes, updates score and handles objects movements.
 * @details Mouse interrupt: moves mouse icon, perform mouse left click.
 * @details Keyboard interrupt: players movement, ESC in menu, send bytes to other player.
 */

void waitForInterrupt(){
  int ipc_status;
  message msg;
  while(gameState != CLOSE){
    sendByte();
    manageOutInput();
    if( driver_receive(ANY, &msg, &ipc_status) != 0 ){
      printf("Error");
      continue;
    }
    if(is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          if (msg.m_notify.interrupts & kbdIRQ) {
            kbd_ih();
            sendScancode(Kscancode);
             if (Kscancode == ESC_BREAK && IS_BREAK_CODE(Kscancode) && (gameState == SCORE || gameState == MULTI)){
                if (gameState == MULTI){
                  addToSend(CHOOSE_REMOVE);
                  multiplayer = false;
                  changeSprite(multi, (xpm_map_t) multi_xpm);
                }
                gameState = MENU;
                
             }
            if (myPlayer == CHOOSE_P1) manageInputKeyboard(player1, Kscancode, &gameState);
            else manageInputKeyboard(player2, Kscancode, &gameState);
            break;
          }
          if (msg.m_notify.interrupts & timerIRQ) {
            timer_int_handler();
            draws();
            if (rotateBuffer() != 0) printf("error in rotating the buffer");
            movements();
            freqActions();
          }
          if(msg.m_notify.interrupts & mouseIRQ) {
            mouse_ih();
            managePackages();
            mouseMovement();
            if (byteCount == 2 && pp.lb) leftMouseClick(true);
          }
          if (msg.m_notify.interrupts & serialIRQ) {
            serial_ih();
            break;
          }
        }
      }      
  }
}


/**
 * @brief Disables all interrupts and free the buffers.
 * @return 0 in case of success.
 */
int EndDevices(){
  if (keyboard_unsubscribe_int() != 0) return 1;
  if (timer_unsubscribe_int() != 0) return 1;
  if (mouse_unsubscribe_int() != 0) return 1;
  if (serial_unsubscribe() != 0) return 1;
  freeVideoBuffers();
  free(bombsArray->bombs);
  free(bombsArray);
  freePlanes(planeArray);
  return vg_exit();
}


/**
 * @brief Handles objects movement depending on the game state.
 */
void (movements)(){
  switch (gameState){
  case PLAY:
    PlanesMovement(planeArray);
    if (multiplayer){
      int r = 0;
      if (myPlayer == CHOOSE_P1){
        r = bombCollision(bombsArray, player1);
        nLifes1 += r;
      }else{
        r = bombCollision(bombsArray, player2);
        nLifes2 += r;
      }
      for (int i = 0 ; i < r ; i++){
          addToSend(LOST_LIFE);
        }
    }else{
      nLifes1 += bombCollision(bombsArray, player1);
    }
    if (nLifes1 < 11)playerMovement(player1);
    if (nLifes2 < 11)playerMovement(player2);
   
    bulletMovement(bulletArray, planeArray);
    bombMovement(bombsArray, count);
    if ((nLifes1 >= 11 && !multiplayer) || (multiplayer && nLifes1 >= 11 && nLifes2 >= 11)){
      gameState = OVER;
      update_time();
      updateGameResults();
      endscore = finalScore;
      multiplayer = false;
      otherChoosedPlayer = 0;
      changeSprite(multi, (xpm_map_t) multi_xpm);
    }
    break;
  case OVER:
    PlanesMovement(planeArray);
    bombMovement(bombsArray, count);
    break;
  default:
    break;  
  } 
}

/**
 * @brief Draws sprites and objects according to the game state.
 */
void (draws)(){
  switch (gameState){
  case MENU:
    fillBackground(menu->map);
    drawSprite(mouseIcon);
    break;
  case MULTI:
    fillBackground(multi->map);
    drawSprite(mouseIcon);
    break;   
  case PLAY:
    fillBackground(back->map);
    drawBombs(bombsArray);
    drawBullets(bulletArray);
    drawPlanes(planeArray);
    drawMultipleSprite(nLifes1, 3, 4, lifeBar1);
    if (multiplayer)drawMultipleSprite(nLifes2, 3, 4, lifeBar2);
    if (multiplayer){
      drawObject(player1);
      drawObject(player2);
    }else{
      if (myPlayer == CHOOSE_P1)drawObject(player1);
      else drawObject(player2);
    }
    drawScoreGamePlay();
    break;
  case SCORE:
    fillBackground(score->map);
    drawScoreGameScore();
    break;
  case OVER:
    fillBackground(back->map);
    drawBombs(bombsArray);
    if (multiplayer){
      drawObject(player1);
      drawObject(player2);
    }else{
      if (myPlayer == CHOOSE_P1)drawObject(player1);
      else drawObject(player2);
    }
    drawPlanes(planeArray);
    drawMultipleSprite(nLifes1, 3, 4, lifeBar1);
    if (multiplayer)drawMultipleSprite(nLifes2, 3, 4, lifeBar2);
    drawScoreGameOver(endscore);
    break;  
  default:
    break;  
  }
}

/**
 * @brief Handles mouse movement for non playable screens.
 */
 void (mouseMovement)(){
  if (gameState == MENU || gameState == SCORE || gameState == MULTI){
    mouseIcon->x += pp.delta_x;
    mouseIcon->y -= pp.delta_y;
    if (mouseIcon->x < 0) mouseIcon->x = 0;
    if (mouseIcon->x + mouseIcon->width > modeInfos.XResolution) 
        mouseIcon->x = modeInfos.XResolution - mouseIcon->width;
    if (mouseIcon->y < 0) mouseIcon->y = 0;
    if (mouseIcon->y + mouseIcon->height > modeInfos.YResolution) 
        mouseIcon->y = modeInfos.YResolution - mouseIcon->height;  
    }
 }

/**
 * @brief Creates actions for the left mouse button clicking depending on the game state.
 * @param mine Says if the click is of our own player or the other otherwise.
 */
void (leftMouseClick)(bool mine){
  if (gameState == PLAY){
    if (mine){
      if (myPlayer == CHOOSE_P1) createBomb(bulletArray, player1->sprite->x + player1->sprite->width/2, player1->sprite->y - 15, false);
      else createBomb(bulletArray, player2->sprite->x + player2->sprite->width/2, player2->sprite->y - 15, false);
      addToSend(MOUSE_CODE_SERIAL);
    }else{
      if (myPlayer == CHOOSE_P2) createBomb(bulletArray, player1->sprite->x + player1->sprite->width/2, player1->sprite->y - 15, false);
      else createBomb(bulletArray, player2->sprite->x + player2->sprite->width/2, player2->sprite->y - 15, false);
    }
    
  }
  else if (gameState == MENU && mine){
    changeSprite(mouseIcon, (xpm_map_t) mouseClick_xpm);
    if (mouseIcon->x > 290 && mouseIcon->x < 834 && mouseIcon->y > 229 && mouseIcon->y < 317) prepareGame(true);
    else if (mouseIcon->x > 290 && mouseIcon->x < 834 && mouseIcon->y > 358 && mouseIcon->y < 446) gameState = MULTI;
    else if (mouseIcon->x > 290 && mouseIcon->x < 834 && mouseIcon->y > 495 && mouseIcon->y < 583) gameState = SCORE;
    else if (mouseIcon->x > 290 && mouseIcon->x < 834 && mouseIcon->y > 639 && mouseIcon->y < 727) gameState = CLOSE;
    addToSend(MOUSE_CODE_SERIAL);
  }
  else if (gameState == MULTI && mine){
    if (mouseIcon->x > 268 && mouseIcon->x < 884 && mouseIcon->y > 282 && mouseIcon->y < 376 && otherChoosedPlayer != CHOOSE_P1){
      printf("Player 1 clicked");
      addToSend(CHOOSE_P1);
      changeSprite(multi, (xpm_map_t) multiP1_xpm);
      multiplayer = true;
    }else if (mouseIcon->x > 268 && mouseIcon->x < 884 && mouseIcon->y > 460 && mouseIcon->y < 554 && otherChoosedPlayer != CHOOSE_P2){
      printf("Player 2 clicked");
      addToSend(CHOOSE_P2);
      changeSprite(multi, (xpm_map_t) multiP2_xpm);
      multiplayer = true;
    }
    if (otherChoosedPlayer != 0 && multiplayer){
      addToSend(START_GAME);
      sendByte();
      usleep(500000); // TO try to start the game at the same time in the other vm
      prepareGame(false);
    } 
  }
}
/**
 * @brief Manage the bytes sended from the other player
 */
void (manageOutInput)(){
  char byte = receiveTop();
  if (byte != 0){
    if (byte == CHOOSE_P1){
      otherChoosedPlayer = CHOOSE_P1;
      changeSprite(multi, (xpm_map_t) multiP1_xpm);
    }else if (byte == CHOOSE_P2){
      otherChoosedPlayer = CHOOSE_P2;
      changeSprite(multi, (xpm_map_t) multiP2_xpm);
    }else if (byte == CHOOSE_REMOVE && otherChoosedPlayer != 0){
      otherChoosedPlayer = 0;
      changeSprite(multi, (xpm_map_t) multi_xpm);
    }else if (byte == START_GAME){
      gameState = PLAY;
      if (otherChoosedPlayer != 0 && multiplayer){
        prepareGame(false);
       }
    }else if (byte == LOST_LIFE){
      if (myPlayer == CHOOSE_P1) nLifes2++;
      else nLifes1++;
    }
    else if (byte == MOUSE_CODE_SERIAL && gameState == PLAY && multiplayer){
      leftMouseClick(false);
    }
    else if (gameState == PLAY){
      uint8_t scan = mapCharToKey(byte);
        if (myPlayer == CHOOSE_P1) manageInputKeyboard(player2, scan, &gameState);
        else manageInputKeyboard(player1, scan, &gameState);
      }
      
    }
  }
  
/**
 * @brief Actions to prepare a gameplay
 * @param solo True if is solo game, false if is a multiplayer
 */
void (prepareGame)(bool solo){
  freePlanes(planeArray);
  free(bombsArray->bombs);
  free(bombsArray);
  free(bulletArray->bombs);
  free(bulletArray);
  planeArray = create_PlanesArray(30);
  bombsArray = create_bombsArray(50);
  bulletArray = create_bombsArray(50);
  nLifes1 = 0;
  nLifes2 = 0;
  count = 0;
  timerCounter = 0;
  player1->xspeed = 0;
  player1->yspeed = 0;
  player1->sprite->x = 0;
  player1->sprite->y = groundLevel - player1->sprite->height;
  player2->xspeed = 0;
  player2->yspeed = 0;
  player2->sprite->x = 0;
  player2->sprite->y = groundLevel - player2->sprite->height;
  gameState = PLAY;
  if (!solo){
     if (otherChoosedPlayer == CHOOSE_P2) myPlayer = CHOOSE_P1;
    else myPlayer = CHOOSE_P2;
  }else myPlayer = CHOOSE_P1;
}

/**
 * @brief Actions that take place taking into consideration the count
 */
void (freqActions)(){
  if (count % 7 == 0){
    for (int i = 0; i < planeArray->sizePlanes; i++){
            if (planeArray->plane[i]->state == 2) continue;
                planeArray->plane[i]->state += 1;
                if (planeArray->plane[i]->state == 2) planeArray->plane[i]->state = 0;
              }
  }
  if (planeFreq > 70)planeFreq -- ; 
  if (count % planeFreq == 0 && gameState == PLAY){
        createPlane(planeArray, planeLeft);
        planeLeft = !planeLeft;
    } 
  if (count % 50 == 0){
      for (int i = 0; i < planeArray->sizePlanes; i++){
          if ( planeArray->plane[i]->state == 2 || planeArray->plane[i]->planeObject->sprite->x < 0 ||
          planeArray->plane[i]->planeObject->sprite->x + planeArray->plane[i]->planeObject->sprite->width > WindowWidth) continue; // No bombs when crashing
              createBomb(bombsArray, planeArray->plane[i]->planeObject->sprite->x + (planeArray->plane[i]->planeObject->sprite->width/3)/2, 
                planeArray->plane[i]->planeObject->sprite->y + planeArray->plane[i]->planeObject->sprite->height, true);
            }
   }
  if (count % 50 == 0 && (gameState == MENU || gameState == SCORE)){
      changeSprite(mouseIcon, (xpm_map_t) mouse_xpm);
  } 
    if (gameState == OVER){
      if (timerCounter == 0){
        timerCounter = count;
      }else if((count - timerCounter) / 60 >= 3){ //3 sec 
        timerCounter = 0;
        gameState = MENU;
      } 
    }
}




