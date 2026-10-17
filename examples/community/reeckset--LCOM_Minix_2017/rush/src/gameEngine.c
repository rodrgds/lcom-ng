#include <stdio.h>
#include <unistd.h> //[DEBUG] for sleep() function - for testing
#include <minix/drivers.h>
#include <minix/driver.h>
#include <minix/syslib.h>
#include <minix/sysutil.h>
#include "gameEngine.h"
#include "video_gr.h"
#include "macros.h"
#include "keyboard.h"
#include "mouse.h"
#include "timer.h"
#include "bitmap.h"
#include "mainGame.h"

int startEngine() {

  //enter Graphic Mode 0x117;
  vg_init(0x117);

  /*[DEBUG]
  Bitmap* test = loadBitmap("/home/rush/res/test.bmp");
  //Draws the test bitmap;
  drawBitmap(test, 0, 0, ALIGN_LEFT);*/

  //[DEBUG]
  //Draws a pixel with color 0xF81F (R5G6B5) on position (100,100)
  //video_draw_pixel(100, 100, 0xF81F);

  mainGameLoop();

  return 0;


}

bool mainGameLoop(){
  //INITIALIZE GAME OBJECT LIST AND EVENT QUEUE
  ev_queue_st* gameEvents = ev_queue_create();

  //subscribe mouse interrupts
  int initialHookID_MOUSE = mouse_subscribe_int();
  if(mouse_enable_stream_mode() != 0) return false;
  if(initialHookID_MOUSE == -1) return false;
  unsigned short mousePacketsReceived = 0;

  //subscribe keyboard interrupts
	int initialHookID_KBD = kbd_subscribe_int();
	if(initialHookID_KBD == -1) return false;

	//subscribe Timer interrupts
	int initialHookID_TIMER = timer_subscribe_int();
	if(initialHookID_TIMER == -1) return false;

	int ipc_status, r;
	message msg;


  startStates(gameEvents, MAX_HP);


  gameState_st* currentState = getCurrentState();


  bool stopflag = false;
	while(!stopflag) {

		// Get a request message.
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d\n", r);
			continue;//continue while loop
		}
		if (is_ipc_notify(ipc_status)) { // received notification
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE: // hardware interrupt notification
          if (msg.NOTIFY_ARG & BIT(initialHookID_TIMER)) { // subscribed interrupt
            //printf("0\n\n");
            //printf("1\n\n");
            currentState = processEvents(gameEvents, currentState);
            //printf("2\n\n");
            if(currentState->state == GAME_EXIT){ stopflag = true; break;}
          }
					if (msg.NOTIFY_ARG & BIT(initialHookID_KBD)) { // subscribed interrupt
            keyboardHandler(gameEvents, kbd_get_pressed_key());
					}
          if (msg.NOTIFY_ARG & BIT(initialHookID_MOUSE)) { // subscribed interrupt
						unsigned long mouseBufferData;
						//read the byte form out_buf
						sys_inb(MOUSE_OUT_BUF, &mouseBufferData);
						unsigned char packet[3];
						if(mouse_movement_handler(mouseBufferData, &mousePacketsReceived, &(packet[0])) == 1){ //Just finished receiving mouse packet
              mouseHandler(gameEvents, packet);
            }
          }

					break;
				default:
					break; // no other notifications expected: do nothing
			}
		} else { // received a standard message, not a notification
			// no standard messages expected: do nothing
		}
	}
  mouse_unsubscribe_int();
  kbd_unsubscribe_int();
  timer_unsubscribe_int();
	vg_exit();
  ev_queue_free(gameEvents);
  stateMachine_free();
	return true;
}

gameState_st* processEvents(ev_queue_st* events, gameState_st* currentState){
  ev_queue_elem_st* currentEvent = ev_queue_begin(events);
  //int ev_counter = 0;
  //printf("ev_queue size: %d\n",events->size);

  while(events->size > 0){
    currentEvent = ev_queue_begin(events);

    //ev_counter++;
    //printf("Processing ev no. %d\n", ev_counter);
    currentState = eventHandler(currentState, currentEvent->data, events);
    //printf("Current state after ev handler is (%d)\n", currentState->state);
    ev_queue_pop(events);

  }
  update(events, currentState);
  drawFrame(currentState);
  return currentState;
}

void drawFrame(gameState_st* currentState){
    clearScreen();
    unsigned int z;
    //printf("Drawing state(%d)\n", currentState->state);
    ObjLayer* objects = currentState->objects;
    for(z = 0; z <= Z_INDEX_MAX; z++){
      unsigned int obj;
      for(obj = 0; obj < objects[z].size; obj++){
        drawObject(&objects[z].gameObjects[obj]);
      }
    }
    memcpy(getGraphicsBuffer(), getTmpBuffer(), getVramSize());
}

//Returns true if cursor (GameObject) is inside of another object (only the upper left corner necessary)
bool cursorIsInside(GameObject* cursor, GameObject* obj2) {
  if(cursor->x >= obj2->x && cursor->x <= obj2->x + obj2->sizeX) { // upperLeftCorner x is inside
      if(cursor->y >= obj2->y && cursor->y <= obj2->y + obj2->sizeY) { // upperLeftCorner y is inside
        //upper left corner is inside
        //printf("cursor inside of %s\n", obj2->label);
        return true;
      }
  }

  return false;
}
