#include <minix/drivers.h>
#include <stdlib.h>
#include "GameController.h"

#include "kbd.h"
#include "Utilities.h"
#include "graphics.h"
#include "buffer.h"
#include "timer.h"
#include "mouse.h"
//#include "rtc.h"

#include "scope.h"
#include "sprite.h"
#include "shark.h"
#include "boat.h"
#include "keyStates.h"
#include "EndScreen.h"

#include "test7.h"
#include "multiplayerSync.h"

//VARS===============================
unsigned long gameSeed=0;
const unsigned short GAME_TIMER_FREQ=120;
int upd=0;///<aux var used to call draw
float mouseSumX=0;float mouseSumY=0;
const unsigned short thirdsUntilAnimation=20;
unsigned long game_previous_step_counter=0;
unsigned long game_steps_counter=0;
char previous_shootButtonState=0;
char shootButtonState=0;
char xBoatMov=0;
char yBoatMov=0;
char choice;///<choice selected from some MENU
//===================================

//interval in thirds
char updateOnInterval(unsigned short interval)
{
	if (game_previous_step_counter!=game_steps_counter&&
			!(game_steps_counter%(interval*GAME_TIMER_FREQ/60)  ) )
		return 1;
	return 0;
}

void initDevices()
{
	//subscriptions
	//Initialize Utilities Values
	initInterruptList();
	addToInterrupt(Mouse_int_handler, Mouse_subscribe_int, Mouse_unsubscribe_int);
	addToInterrupt(Kbd_int_handler, Kbd_subscribe_int, Kbd_unsubscribe_int);
	addToInterrupt(timer_int_handler, timer_subscribe_int, timer_unsubscribe_int);
	//Subscribe Interrupts
	subscribeInterrupts();

	Mouse_enable_stream();
}
void stopDevices()
{
	unsubscribeInterrupts();
	Mouse_sendCommand(MCMD_SET_STREAM_DATA);
	freeInterruptList();
}


char gameSeedInit()
{
	unsigned int seed=0;

	//SINGLE PLAYER
	if (multiplayer_getUserType()==USER_TYPE_NONE)
	{
		DEBUG_MSG("gameSeedInit label:s");//LOG_SDEC("gameSeedInit label:", 999 )
				seed = rtc_generate_seed();
		srand(seed);
		return 0;
	}

	//===HOST
	if (multiplayer_getUserType()==USER_TYPE_HOST)
	{
		DEBUG_MSG("gameSeedInit label:h");//LOG_SDEC("gameSeedInit label:", 11 );
				//initializar rand seed
			seed = rtc_generate_seed();
		srand(seed);
		if (multiplayer_syncSeed(&seed)!=OK) return -1;

		return 0;
	}

	//===CLIENT
	if (multiplayer_getUserType()==USER_TYPE_CLIENT)
	{
		DEBUG_MSG("gameSeedInit label:c1");//LOG_SDEC("gameSeedInit label:", 1 );

		timer_set_square(0,GAME_TIMER_FREQ);
		char multiplayer_syncSeed_answer=1;

		do{
			game_previous_step_counter = game_steps_counter;
			resetTick();

			//Call all Handlers
			interruptHandler();

			ks_updateKeyStates(Kbd_getLastScanCodeRead());
			game_steps_counter+=getTick();

			if (ks_keyPressed(KEY_ESCAPE)) return 1;

			if (updateOnInterval(2)) multiplayer_syncSeed_answer = multiplayer_syncSeed(&seed);

			//timout
			//if(multiplayer_syncSeed_answer==2) return -1;

		} while (multiplayer_syncSeed_answer!=OK) ;

		DEBUG_MSG("gameSeedInit label:c2");//LOG_SDEC("gameSeedInit label:", 2 );
		getSeed(&seed);

		srand(seed);
		DEBUG_MSG("gameSeedInit label:c3");//LOG_SDEC("gameSeedInit label:", 3 );
		return 0;
	}
	return 1;
}

GameController* initGameController(){
	GameController* game = (GameController*) malloc (sizeof(GameController));

	//reset timer freq
	timer_set_square(0,GAME_TIMER_FREQ);
	//reset keys (important when restarting game)
	ks_setKeyStates2Zero();

	//Init vars
	game_previous_step_counter = 0; game_steps_counter=0;
	previous_shootButtonState=0;shootButtonState=0;
	xBoatMov=0;	yBoatMov=0;
	//game->done = 0;
	game->state=GAME_INGAME;
	game->draw = 0;
	//game->gameover = 0;

	initSpritesDrawList();

	initScope();
	createSharks();
	game->playerBoat = createBoat();


	initEndScreen();

	if (multiplayer_getUserType()!=USER_TYPE_NONE)
		{
			game->multiplayerBoat = createBoat();
			game->state = GAME_SYNC;
		}


	return game;
}


boat* boatToCheck;
char sharkColidesWithBoat(shark* s)
{
	return circlesColide(s->position , s->colRad
			,boatToCheck->position, boatToCheck->colRad);
}

char checkGameOver(GameController *game)
{
	shark_SV* sharks = getSharks();
	boatToCheck = &(game->playerBoat);
	return shark_SV_find(sharks,sharkColidesWithBoat)!=-1;
}


inline void update_syncGameInit(GameController *game)
{
	if (multiplayer_syncGameInit()==OK)
	game->state = GAME_INGAME;
}

inline void update_ingame(GameController *game)
{
	if (multiplayer_update()!=OK)
	{
		//failed something in uart
	}


	updateSharks(updateOnInterval(thirdsUntilAnimation)
			,previous_shootButtonState==0&&shootButtonState?
					getScopePosition():Vector2(-500,-500));

	updateBoat( &(game->playerBoat)
			,xBoatMov
			,yBoatMov
	);
	if(checkGameOver(game)) game->state = GAME_GAMEOVER;
}

inline void update_gameover(GameController *game)
{
	updateSharks(updateOnInterval(thirdsUntilAnimation), Vector2(-500,-500) );
	choice = endGameChoice(previous_shootButtonState==0&&shootButtonState?
			getScopePosition(): Vector2(-500,-500) );
	if (choice==QUIT_GAME) game->state = GAME_QUIT;
	if (choice==RESTART_GAME) game->state = GAME_RESTART;
}

void updateGame(GameController *game){

		game_previous_step_counter = game_steps_counter;
		//Reset DeltaX and DeltaY from mouse for scope Draw
		Mouse_resetDeltas();
		//Reset TimerTick
		resetTick();
		//game->scancode = 0;
		game->draw = 0;


		//Call all Handlers
		interruptHandler();

		//===GET INPUTS==================================
		//<<<KEYBOARD<<<
		//game->scancode = Kbd_getLastScanCodeRead();
		ks_updateKeyStates(Kbd_getLastScanCodeRead());
		xBoatMov += -ks_keyPressed(KEY_D) + ks_keyReleased(KEY_D) -ks_keyReleased(KEY_A) + ks_keyPressed(KEY_A) ;
		yBoatMov += ks_keyPressed(KEY_W) - ks_keyReleased(KEY_W) +ks_keyReleased(KEY_S) - ks_keyPressed(KEY_S) ;
		//<<<MOUSE<<<
		game->shootButtonState = Mouse_getLbuttonState();
		mouseSumX+=Mouse_getXdelta()*0.55f;//so arredonda no final
		mouseSumY+=-Mouse_getYdelta()*0.55f;
		//game steps - timer ticks
		game_steps_counter+=getTick();
		//===END GET INPUTS==============================

		//"emergency exit"
		if (ks_keyPressed(KEY_ESCAPE))
		{ /* ESC release */
			game->state = GAME_QUIT;
		}

		//================================================
		//===SINC WITH TIMER UPDATES======================
		//UPDATE GAME STUFF
		if (game_steps_counter!=game_previous_step_counter){
			//ALWAYS UPDATE --------------------------------
			previous_shootButtonState=shootButtonState;
			shootButtonState=game->shootButtonState;
			//UPDATE DEPENDING ON STATE --------------------
			switch(game->state){
			case GAME_SYNC:
				update_syncGameInit(game);
				break;
			case GAME_INGAME:
				update_ingame(game);
				break;
			case GAME_GAMEOVER:
				update_gameover(game);
			break;
			default: break;
			}
		}//END UPDATE ======================================

		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		//CALL DRAW 60x per second (cant use timerFreq<30)
		//if (upd==GAME_TIMER_FREQ/60){
		if (game->state!=GAME_SYNC &&
				updateOnInterval(1)){
			//upd=0;
			updateScope(mouseSumX,mouseSumY);
			mouseSumX=0;mouseSumY=0;
			game->draw = 1 ;
		}

}

void drawGame(GameController *game){
	fillBuffer(1);//fill screen
	drawSprites();
	if(game->state==GAME_GAMEOVER)	drawEndScreen();
	//if(game->state==GAME_SYNC)	drawSyncScreen();
	drawScope();
	printBuffer(getVramAdress());
}

void stopGame(GameController *game){
	clearBuffer();

	//free mem
	freeEndScreen();
	freeBoat(&game->playerBoat);
	if (multiplayer_getUserType()!=USER_TYPE_NONE) freeBoat(&game->multiplayerBoat);
	deleteSharks();
	freeScope();
	pixmap_cleanMemory();
	freeDrawList();
	free(game);
}

