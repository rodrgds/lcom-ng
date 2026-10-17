//includes
#include <minix/drivers.h>
#include "lmlib.h"
#include "graphics.h"
#include "video_gr.h"
#include "buffer.h"
//#include "pixmap.h"

#include "Utilities.h"
#include "kbd.h"
#include "timer.h"
#include "mouse.h"
#include "rtc.h"

#include "GameController.h"

#include "rtc.h"
#include <stdlib.h>

#include "multiplayerSync.h"

int main(int argc, char **argv) {

  /* Initialize service */
  sef_startup();

  //init debug log
#ifdef DEBUG_LOG
  logfd=fopen(LOG_PATH,"w");
  LOG_SDEC("Init Log:",1);
#endif
  //subscribe devices needed for game
  initDevices();

  //GAME SEED AND MULTIPLAYER RELATED INITS
  GameController *game;
  if(argc>1)
  {
	  if(argv[1][0]=='1')
	  {
		  multiplayer_defineUserType(USER_TYPE_HOST,&game);
		  printf("\n-Started Host Mode-\n");
	  }
	  if(argv[1][0]=='2')
	  {
		  multiplayer_defineUserType(USER_TYPE_CLIENT,&game);
		  printf("\n-Started Client Mode-\n");
	  }

	  LOG_CHAR("User is Host(1) or Client(2)",argv[1][0] );
  }
  else  printf("Started Single Player Mode");//LOG_CHAR("SINGLE PLAYER",'!');
  if(gameSeedInit()==OK){ //only go into game if seed

	  //init graphic mode and buffer
	  set_vramAdress(vg_init(0x105));
	  initBuffer();

	  //=====GAME======
	  //  GameController *game;
	  while(1) {
		  //create a new game
		  game = initGameController();

		  //run game
		  while(game->state&GAME_RUNNING){
			  updateGame(game);

			  if(game->state&GAME_RUNNING)
				  if(game->draw)
					  drawGame(game);
		  }

		  //restart game or quit
		  if (game->state==GAME_RESTART)
		  {
			  /*heavy to compute but faster to implement than changing current code*/
			  /*should keep pixmaps for better performance later if possible*/
			  stopGame(game);
			  continue;
		  }else{
			  stopGame(game);
			  break;
		  }
	  }
	  //game terminated
	  //free stuff, unsubscribe devices and quit graphic mode

	 // while(multiplayer_receive()!=2);//leave no game info before quitting

	  freeBuffer();
	  vg_exit();
  }

 stopDevices();
 printf("\n-PROGRAM ENDED-\n");

  return 0;
}
