

#include "playingController.h"
#include "../labs/graphics.h"
#include "../labs/scancodes.h"
#include "../model/button.h"
#include "../model/player.h"
#include "../physics/simulate.h"
#include "../viewer/cueViewer.h"
#include "../viewer/lineViewer.h"
#include "../xpms/ball.xpm"
#include "labs/rtc.h"

STATE playingControllerHandle(Table *table, DEVICE interruptType, const struct packet *packet, uint8_t scanCode, unsigned elapsed) {
  switch (interruptType) {
    case TIMER:
      if (elapsed % (sys_hz() / 30) == 0) {

        if (table->state == SIMULATING) {
          if (!updatePhysics(table, 1.0 / 30.0)) {
            printf("1\n");
            if (table->firstBallHit == NULL) { // no ball was hit
              switchTurn(table);
              resetRound(table);
              table->state = ADVANTAGE;
              printf("2\n");
              
            }
            else {
              printf("3\n");
              printf("Ball type: %d\n", getPlayingPlayer(table)->ballType);
              if (getPlayingPlayer(table)->ballType != PLAYERBALLNONE) {
                printf("4\n");
              if (((table->firstBallHit->type != STRIPED) && (getPlayingPlayer(table)->ballType == PLAYERSTRIPED)) || ((table->firstBallHit->type != SOLID) && (getPlayingPlayer(table)->ballType == PLAYERSOLID))) { // player hit the wrong ball
              printf("5\n");
                if (table->balls[1]->state == POCKETED) {
                  printf("6\n");
                  Player* notPlaying = getNotPlayingPlayer(table);
                  table->winningPlayerName = notPlaying->name;
                  return MENU; // player lose
                }
                else {
                  printf("7\n");
                  switchTurn(table);
                  resetRound(table);
                  table->state = ADVANTAGE;
                  printf("Player %d hit the wrong ball\n", getPlayingPlayer(table)->isPlaying);
                }
              }
              else {
                printf("8\n");
                if (table->balls[1]->state == POCKETED) {
                  printf("9\n");
                  for (int i = 0; i < table->ballNumber; i++) {
                    if (table->balls[i]->state != POCKETED) {
                      if (((table->balls[i]->type == STRIPED) && (getPlayingPlayer(table)->ballType == PLAYERSTRIPED)) || ((table->balls[i]->type == SOLID) && (getPlayingPlayer(table)->ballType == PLAYERSOLID))) {
                        printf("10\n");
                        Player* oppenent = getNotPlayingPlayer(table);
                        table->winningPlayerName = oppenent->name;
                        return MENU; // player lose
                      }
                    }
                  }
                  printf("11\n");
                  Player* player = getPlayingPlayer(table);
                  table->winningPlayerName = player->name;
                  return MENU; // player win
                }
                else {
                  printf("12\n");
                  if (table->balls[0]->state == POCKETED) {
                    printf("13\n");
                    switchTurn(table);
                    resetRound(table);                    
                  table->state = ADVANTAGE;
                  }
                  else {
                    printf("14\n");
                    printf("Pocketed own ball: %d\n", table->pocketedOwnBall);
                    if (!table->pocketedOwnBall) {
                      printf("15\n");
                      switchTurn(table);
                    }
                    resetRound(table);                    
                  }
                }
              }
            }
            else {
              printf("16\n");
              if (table->balls[1]->state == POCKETED) {
                printf("17\n");
                Player* notPlaying = getNotPlayingPlayer(table);
                table->winningPlayerName = notPlaying->name;

                return MENU; // player lose
              }
              else {
                printf("18\n");
                if (table->balls[0]->state == POCKETED) {
                  printf("19\n");
                  switchTurn(table);
                    resetRound(table);                    
                  table->state = ADVANTAGE;
                }
                else {
                  printf("20\n");
                  int solidBalls = 0;
                  int stripedBalls = 0;
                  for (int i = 0; i < table->ballNumber; i++) {
                    if (table->balls[i]->state == POCKETED) {
                      if (table->balls[i]->type == SOLID) {
                        solidBalls++;
                      }
                      else if (table->balls[i]->type == STRIPED) {
                        stripedBalls++;
                      }
                    }
                  }
                  if (solidBalls > 0 || stripedBalls > 0) {
                    printf("21\n");
                    if (solidBalls > stripedBalls) {
                      printf("22\n");
                      getPlayingPlayer(table)->ballType = PLAYERSOLID;
                      getNotPlayingPlayer(table)->ballType = PLAYERSTRIPED;
                    }
                    else if (stripedBalls >= solidBalls) {
                      printf("23\n");
                      getPlayingPlayer(table)->ballType = PLAYERSTRIPED;
                      getNotPlayingPlayer(table)->ballType = PLAYERSOLID;
                    }
                  }
                  else{
                    printf("24\n");
                    switchTurn(table);
                    resetRound(table);                    
                  }
                }
              }
            }
            }
            
            if(table->state != ADVANTAGE){
              table->state = AIMING;
            }
          }
        }

        if (drawTable(table, get_game_time(), get_round_time())) {
          return OVER;
        }
        if (swap_buffers())
          return 1;
      }
      break;
    case MOUSE:
      // Update mouse position
      table->mouse->pos.x += packet->delta_x;
      table->mouse->pos.y -= packet->delta_y;

      table->exitSelected=isMouseOverButton(table->exitButton, table->mouse->pos);

      switch (table->state) {
        case AIMING:
          updateCueState(table, false);
          if (packet->lb) {

            if (updateSpin(table))
              break;
            if (table->exitSelected) {
              printf("Exiting game\n");
              Player* opponent = getNotPlayingPlayer(table);
              table->winningPlayerName = opponent->name;
              return MENU;
            }
            table->mouse->savedPos = table->mouse->pos;
            table->state = SHOOTING;
          }
          break;
        case SHOOTING:
          // Update how much mouse moved
          table->mouse->delta.x = packet->delta_x;
          table->mouse->delta.y = -packet->delta_y;
          updateCueState(table, true);
          // Reset mouse movement because its not automatically reseted
          table->mouse->delta.x = 0;
          table->mouse->delta.y = 0;

          if (!packet->lb) {
            if (table->cue->charge) {
              printf("\n\n\n\nStart simuilation\n\n\n\n\n\n\n");
              table->state = SIMULATING;
              processShot(table);
            }
            else {
              table->state = AIMING;
              table->cue->charge = 0;
            }
          }
          break;
        case ADVANTAGE:

          glueBall(table);
          if (packet->lb && canDropBall(table)) {

            table->state = AIMING;
          }
        default:
          break;
      }
      break;
    case KEYBOARD:
      printf("Detected keyboard with scanCode: %x\n", scanCode);
      printf("The comparation is %d", scanCode == 0x81);
      if (scanCode == SCANCODE_ESC) {
        return OVER;
      }
      break;

    case RTC:
      if (get_round_time() == 0 && table->state != SIMULATING) {
        resetRound(table);                    
        switchTurn(table);
      }

      break;

    case SP:
      break;

    default:
      break;
  }
  return PLAYING;
}


void resetRound(Table * table){
  set_round_time(40);
  table->cue->charge = 0;
  table->cue->sideEnglish = 0;
  table->cue->verticalEnglish = 0;
  table->firstBallHit = NULL;
  table->pocketedOwnBall = 0;
}


