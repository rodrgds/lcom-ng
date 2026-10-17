#include <stdio.h>
#include "stateMachine.h"
#include "mainGame.h"
#include "macros.h"

//setting the default current state (aka the first one)
static gameState_st* currentState;
static gameState_st* existentStates[5];

gameState_st* newGameState(state_st state, int score, int hp) {
  gameState_st* stateptr = malloc(sizeof(gameState_st));

  stateptr->state = state;
  stateptr->currentScore = score;
  stateptr->currentHP = hp;
  ObjLayer* objects = calloc(Z_INDEX_MAX + 1, sizeof(ObjLayer)); //Create array of ObjLayer (each ObjLayer is a z-index)
  createActiveObjectsList(objects, Z_INDEX_MAX + 1); //Populate with Z_INDEX_MAX + 1 z-indexes. Each z-index is an ObjLayer
  stateptr->objects = objects;
  stateptr->isKeyDown_SPACE = false;
  return stateptr;

}

void startStateMachine(gameState_st* state) {
  printf("Starting StateMachine..\n");
  int i;
  for(i = 0; i < sizeof(existentStates)/sizeof(gameState_st*); i++) {
    printf("State %d: %d\n", i, existentStates[i]->state);
    printf("--nObjects: %d\n", existentStates[i]->objects->size);
    //printObjectArray(existentStates[i]->objects);
  }
  currentState = state;
}

void stateMachine_free(){
  int i;
  for(i = 0; i < N_STATES ; i++) {
    gameState_free(existentStates[i]);
  }

  printf("\n\n-----THANK YOU FOR PLAYING-----\n\n\n");

}

void gameState_free(gameState_st* state) {
  objLayer_free(state->objects);
  free(state);
}

void updateCurrentGameState(state_st updatedGameState) {
  //printf("updating state...from (%d) to (%d)\n", currentState->state, updatedGameState);

  currentState = getState(updatedGameState);
}

gameState_st* getState(state_st s){
    int i;
    for(i = 0; i < sizeof(existentStates)/sizeof(gameState_st*);i++){
      if(existentStates[i]->state == s) {
        //printf("getState:: return ptr: %p\n");
        return existentStates[i];
      }
    }
    //printf("getState::return null ptr\n");
    return NULL;
}

void startStates(ev_queue_st* gameEvents, int maxHP){
  printf("Starting states \n");
  existentStates[0] = newGameState(MAIN_MENU, 0, maxHP);
  existentStates[1] = newGameState(GAME_PLAY, 0, maxHP);
  existentStates[2] = newGameState(PAUSE_MENU, 0, maxHP);
  existentStates[3] = newGameState(GAME_OVER, 0, maxHP);
  existentStates[4] = newGameState(GAME_EXIT, 0, maxHP);
  printf("Started states -- \n");
  int i;
  for(i = 0; i < sizeof(existentStates)/sizeof(gameState_st*); i++) {
    printf("State %d: %d\n", i, existentStates[i]->state);
    printf("--nObjects: %d\n", existentStates[i]->objects->size);
    //printObjectArray(existentStates[i]->objects);
  }
  startup(gameEvents, *existentStates);
  startStateMachine(*existentStates);
}

gameState_st* getCurrentState() {
  //printf("Current State: %d\n", currentState->state);
  return currentState;
}

gameState_st* eventHandler(gameState_st* currentState, event_st event, ev_queue_st* events) {
    switch (event.eventType) {
        case KEY_PRESS:
          //printf("HANDLING KEY PRESS EVENT\n");
          onKbdEvent(currentState, event, events);
          break;
        case MOUSE_CHANGE:
          onMouseEvent(currentState, event, events);
          break;
        case NO_HP:
          updateCurrentGameState(GAME_OVER);
          break;
        default:
          break;
    }
  return getCurrentState();
}
