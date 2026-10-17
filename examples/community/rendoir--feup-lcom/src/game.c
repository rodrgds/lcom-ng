/**
	@file game.c
	@author Daniel Marques
	@author João Almeida

	Contains the Driver_Receive loop.
	Declaration of the functions that serve as the core of the game since all interrupts are handled here.
	It uses a state machine implementation to know what to do with each interrupt received.
*/

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "keyboard.h"
#include "mouse.h"
#include "graphics.h"
#include "game.h"

static unsigned long int highscore;

int runGame(game_t *game, unsigned int TIMER_BIT, unsigned int MOUSE_BIT,
		unsigned int KBD_BIT) {

	updateBuffer(game,2,0);
	memcpy((uint16_t*)getVideoMem() , game->buffer , getHRes()*getVRes()*sizeof(uint16_t));
	unsigned char two_bytes = 0, enter_pressed = 0, packet[3],
			lb_pressed = 0, cont = 0, button_pressed = 0;
	int ipc_status = 1, r = 1, byte_counter = 0;
	char full_path[200] = "";
	unsigned int timer_counter = 0 ;
	message msg;
	unsigned long int key;

	options_t current_option = START_GAME;
	game_status_t current_status = MAIN_MENU;
	unsigned int highScores[NUM_SCORES];



	strcat(full_path , game->current_dir);
	strcat(full_path , PATH_TO_SCORES);
	readHighScores(highScores, NUM_SCORES, full_path);
	printf("HIGH = %d \n", highScores[0]);
	highscore = highScores[0];
	printf("FIRST HIGH = %d \n", highScores[0] );\

	timerSubscribe(TIMER_BIT);
	mouseSubscribe(MOUSE_BIT);
	kbdSubscribe(KBD_BIT);

	unsigned long int temp;
	sys_outb(STAT_REG, EN_MOUSE);
	sys_inb(0x60, &temp);
	sys_outb(STAT_REG, MOUSE_CMD);
	sys_inb(0x60, &temp);
	sys_outb(IN_BUF, STREAM_MODE);
	sys_inb(0x60, &temp);
	receiveData(&temp);

	//INTERRUPT HANDLER
	while (1) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			if (_ENDPOINT_P(msg.m_source) == HARDWARE) {

				//KEYBOARD
				if (msg.NOTIFY_ARG & BIT(KBD_BIT)) {
					kbdReadByte(&key, &two_bytes);

					if (MAIN_MENU == current_status || REPLAY_MENU == current_status) { //AT MAIN MENU
						unsigned int opt = menuHandler(&current_option, &key, &enter_pressed);

						updateBuffer(game , ( (current_status == MAIN_MENU)   ? 2 :
						 										( (current_status == REPLAY_MENU) ? 4 : 0) ),
						 										(current_option == START_GAME) ? 0 : 1);
						memcpy( (uint16_t*)getVideoMem(), game->buffer , getHRes()*getVRes()*sizeof(uint16_t)) ;

						if (1 == opt) { //SELECTED START/RESUME
							if (game->score > highscore)
								highscore = game->score;
							if (current_status == REPLAY_MENU)
								setGame(game,0);
							current_status = IN_GAME;
							startGame(game);
							updateHighscore(game, (uint16_t*)getVideoMem() , HIGHSCORE_X , HIGHSCORE_Y, game->score);
						} else if (0 == opt) { //SELECTED EXIT GAME
							if (game->score > highscore)
								highscore = game->score;
							break;
						}
					}

				}

				//TIMER
				if (msg.NOTIFY_ARG & BIT(TIMER_BIT)) {
					timer_counter++;
					//RTC INNER WORKING
					if (timer_counter == 60) {
						timer_counter = 0;
						unsigned long int day, month, year, hour, minute, second;
						RTC_GetDate(&year, &month, &day);
						RTC_GetTime(&game->hour.hour, &game->hour.minute, &game->hour.second);
						updateBuffer(game, ( (current_status == WALKING)   ? 0 :
															 ( (current_status == IN_GAME)   ? 1 :
														   ( (current_status == MAIN_MENU) ? 2 :
														   ( (current_status == LOSE)      ? 3 : 4)))) , (current_option == START_GAME) ? 0 : 1 );
						memcpy( (uint16_t *)getVideoMem(),game->buffer, getVRes()*getHRes()*sizeof(uint16_t));
					}

					if (IN_GAME == current_status) {
						if (lb_pressed) {
							if (!button_pressed)
								game->stick.size = 0;
							button_pressed=1;
							if (!growStick(game)) { //IF MAXIMUM STICK SIZE REACHED
								current_status = WALKING;
								stickFallAnimation(game);
							}
						} else if (button_pressed && !lb_pressed){ //USER RELEASED LB
							current_status = WALKING;
							stickFallAnimation(game);
							button_pressed=0;
						}
					}

					else if (WALKING == current_status) {
						unsigned int move_result = moveMountains(game, cont);
						updateBuffer(game,0 , (current_option == START_GAME) ? 0 : 1);
						memcpy( (uint16_t*)getVideoMem() , game->buffer , getHRes()*getVRes()*sizeof(uint16_t));
						if (3 != move_result) { //ANIMATION OVER
							if (1 == move_result)
								current_status = LOSE;
							else if (2 == move_result)
								current_status = LOSE;
							else if (0 == move_result) { //NINJA STILL ON TOP OF MOUNTAIN
								game->score++;
								generateNewMountain(game);
								updateBuffer(game,1, (current_option == START_GAME) ? 0 : 1);
								memcpy( (uint16_t*)getVideoMem() , game->buffer , getHRes() * getVRes() * sizeof(uint16_t));
								current_status = IN_GAME;
							}
						} else { //ANIMATION NOT OVER
							if (10 == cont)
								cont = 0;
							else
								cont++;
						}
					}

					else if (LOSE == current_status){
						if (ninjaFallAnimation(game)){ //ANIMATION OVER
							current_status = REPLAY_MENU;
							updateBuffer(game, 4 , (current_option == START_GAME) ? 0 : 1 );
							memcpy((uint16_t*)getVideoMem(), game->buffer , getHRes()*getVRes()*sizeof(uint16_t));
							button_pressed=0;
						}
					}
				}

				//MOUSE
				if (msg.NOTIFY_ARG & BIT(MOUSE_BIT)) {
					if (readMouseByte(packet, byte_counter)){ //NOT YET SYNCED, KEEP GOING
						continue;
					}
					else {
						byte_counter++;
						//display_packet(packet);
						if (byte_counter == 3) { //PACKET PROCESSED
							if (((packet[0] & BIT(0)) != 0) && !lb_pressed){ //LEFT BUTTON PRESSED

								if (current_status == IN_GAME)
									lb_pressed = 	1;

									//IF USER PRESSED LEFT BUTTON
								if (current_status == MAIN_MENU || REPLAY_MENU == current_status){
									unsigned int button = mouseAboveButton(game, ( (current_status == MAIN_MENU || current_status == REPLAY_MENU) ? 1 : 0) );
									if (button == 0){ //START GAME
										if (game->score > highscore)
											highscore = game->score;
										if (current_status == REPLAY_MENU)
											setGame(game, 0);
										current_status = IN_GAME;
										startGame(game);
										updateHighscore(game, (uint16_t*)getVideoMem() , HIGHSCORE_X , HIGHSCORE_Y, game->score);
									}
									else if (button == 1){
										if (game->score > highscore)
											highscore = game->score;
										break;
									}
								}

							}
							else if (((packet[0] & BIT(0)) == 0) && lb_pressed) {
								lb_pressed = 0;
							}

							//WHEN USER MOVES MOUSE
							if (packet[1] != 0 || packet[2] != 0){
								updateMouseCoordinates(game, packet);

								if (MAIN_MENU == current_status || REPLAY_MENU == current_status){
									unsigned int button = mouseAboveButton(game,1);
									if (button != 2)
										current_option = (button == 0) ? START_GAME : EXIT_GAME;
								}

								moveMouse(game,( (current_status == WALKING)   ? 0 :
															 ( (current_status == IN_GAME)   ? 1 :
															 ( (current_status == MAIN_MENU) ? 2 :
															 ( (current_status == LOSE)      ? 3 : 4  )))) ,
													 (current_option == START_GAME) ? 0 : 1);
							}

							byte_counter = 0;
						}
					}
				}

			}
		}

	} //END OF WHILE LOOP
	printf("ENDED \n");
	sys_outb(STAT_REG, MOUSE_CMD);
	sys_inb(0x60, &temp);
	sys_outb(	IN_BUF, DATA_OFF);
	sys_inb(0x60, &temp);

	timerUnsubscribe(TIMER_BIT);
	kbdUnsubscribe(KBD_BIT);
	mouseUnsubscribe(MOUSE_BIT);

	updateHighScores(highScores, NUM_SCORES, highscore );
	memset(full_path,0,sizeof(full_path));
	strcat(full_path , game->current_dir);
	strcat(full_path , PATH_TO_SCORES);
	writeHighScores(highScores, NUM_SCORES, full_path);
}

int setGame(game_t *game , unsigned int load_images){
	bit_map_info_header_t temp;
	game->score=0;
	char full_path[200] = "";
	//BACKGROUND
	if (load_images){
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_BACKGROUND);
		game->background_img = LoadBitmapFile(full_path, &temp);

		if (game->background_img == NULL){
			printf("Error Reading Background Img \n");
			return 1;
		}
		game->buffer = malloc( temp.width * temp.height * sizeof(uint16_t) );
	}
	//NINJA
	game->ninja.x = 30;
	game->ninja.y = 459;
	game->ninja.size_x = 40;
	game->ninja.size_y = 40;
	game->ninja.status = 0;
	if (load_images){
		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_STAND);
		game->ninja.stand_img = LoadBitmapFile(full_path,&temp);
	}
	if (game->ninja.stand_img == NULL){
		printf("Error Reading Ninja Stand Img \n");
		return 1;
	}
	if (load_images){
		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_WALK);
		game->ninja.walk_img = LoadBitmapFile(full_path,&temp);
	}
	if (game->ninja.walk_img == NULL){
		printf("Error Reading Ninja Walk Img \n");
		return 1;
	}


	if (load_images){

		//HIGHSCORE NUMBERS
		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_ONE);
		game->h_numbers.one  = LoadBitmapFile(full_path,&temp);
		if (game->h_numbers.one == NULL){
			printf("Error Reading Highscore One Img \n");
			return 1;
		}
		game->h_numbers.width_of_1 = temp.width;

		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_TWO);
		game->h_numbers.two  = LoadBitmapFile(full_path,&temp);
		if (game->h_numbers.two == NULL){
			printf("Error Reading Highscore Two Img \n");
			return 1;
		}

		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_THREE);
		game->h_numbers.three= LoadBitmapFile(full_path,&temp);
		if (game->h_numbers.three == NULL){
			printf("Error Reading Highscore Three Img \n");
			return 1;
		}

		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_FOUR);
		game->h_numbers.four = LoadBitmapFile(full_path,&temp);
		if (game->h_numbers.four == NULL){
			printf("Error Reading Highscore Four Img \n");
			return 1;
		}

		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_FIVE);
		game->h_numbers.five = LoadBitmapFile(full_path,&temp);
		if (game->h_numbers.five == NULL){
			printf("Error Reading Highscore Five Img \n");
			return 1;
		}

		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_SIX);
		game->h_numbers.six  = LoadBitmapFile(full_path,&temp);
		if (game->h_numbers.six == NULL){
			printf("Error Reading Highscore Six Img \n");
			return 1;
		}

		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_SEVEN);
		game->h_numbers.seven= LoadBitmapFile(full_path,&temp);
		if (game->h_numbers.seven == NULL){
			printf("Error Reading Highscore Seven Img \n");
			return 1;
		}

		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_EIGHT);
		game->h_numbers.eight= LoadBitmapFile(full_path,&temp);
		if (game->h_numbers.eight == NULL){
			printf("Error Reading Highscore Eight Img \n");
			return 1;
		}

		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_NINE);
		game->h_numbers.nine = LoadBitmapFile(full_path,&temp);
		if (game->h_numbers.nine == NULL){
			printf("Error Reading Highscore Nine Img \n");
			return 1;
		}

		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_ZERO);
		game->h_numbers.zero = LoadBitmapFile(full_path,&temp);
		if (game->h_numbers.zero == NULL){
			printf("Error Reading Highscore Zero Img \n");
			return 1;
		}
		game->h_numbers.width = temp.width;
		game->h_numbers.height= temp.height;

		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_NEWHIGH);
		game->h_numbers.new_high = LoadBitmapFile(full_path,&temp);
		if (game->h_numbers.new_high == NULL){
			printf("Error Reading New Highscore Img \n");
			return 1;
		}
		game->h_numbers.n_width = temp.width;
		game->h_numbers.n_height= temp.height;

		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_SCORE);
		game->h_numbers.score = LoadBitmapFile(full_path	,&temp);
		if (game->h_numbers.score == NULL){
			printf("Error Reading Score Text Img \n");
			return 1;
		}
		game->h_numbers.s_width = temp.width;
		game->h_numbers.s_height= temp.height;

		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_HSCORE);
		game->h_numbers.high_score = LoadBitmapFile(full_path	,&temp);
		if (game->h_numbers.high_score == NULL){
			printf("Error Reading Highscore Text Img \n");
			return 1;
		}
		game->h_numbers.h_width = temp.width;
		game->h_numbers.h_height= temp.height;


		//HOUR
		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_HOUR_ZERO);
		game->hour.zero = LoadBitmapFile(full_path,&temp);
		if (game->hour.zero == NULL){
			printf("Error Reading Hour Zero Img \n");
			return 1;
		}
		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_HOUR_ONE);
		game->hour.one = LoadBitmapFile(full_path,&temp);
		if (game->hour.one == NULL){
			printf("Error Reading Hour One Img \n");
			return 1;
		}
		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_HOUR_TWO);
		game->hour.two = LoadBitmapFile(full_path,&temp);
		if (game->hour.two == NULL){
			printf("Error Reading Hour Two Img \n");
			return 1;
		}
		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_HOUR_THREE);
		game->hour.three = LoadBitmapFile(full_path,&temp);
		if (game->hour.three == NULL){
			printf("Error Reading Hour Three Img \n");
			return 1;
		}
		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_HOUR_FOUR);
		game->hour.four = LoadBitmapFile(full_path,&temp);
		if (game->hour.four == NULL){
			printf("Error Reading Hour Four Img \n");
			return 1;
		}
		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_HOUR_FIVE);
		game->hour.five = LoadBitmapFile(full_path,&temp);
		if (game->hour.five == NULL){
			printf("Error Reading Hour Five Img \n");
			return 1;
		}
		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_HOUR_SIX);
		game->hour.six = LoadBitmapFile(full_path,&temp);
		if (game->hour.six == NULL){
			printf("Error Reading Hour Six Img \n");
			return 1;
		}
		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_HOUR_SEVEN);
		game->hour.seven = LoadBitmapFile(full_path,&temp);
		if (game->hour.seven == NULL){
			printf("Error Reading Hour Seven Img \n");
			return 1;
		}
		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_HOUR_EIGHT);
		game->hour.eight = LoadBitmapFile(full_path,&temp);
		if (game->hour.eight == NULL){
			printf("Error Reading Hour Eight Img \n");
			return 1;
		}
		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_HOUR_NINE);
		game->hour.nine = LoadBitmapFile(full_path,&temp);
		if (game->hour.nine == NULL){
			printf("Error Reading Hour Nine Img \n");
			return 1;
		}
		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_HOUR_SEPARATOR);
		game->hour.two_points = LoadBitmapFile(full_path,&temp);
		if (game->hour.two_points == NULL){
			printf("Error Reading Hour Separator Img \n");
			return 1;
		}
		game->hour.width = temp.width;
		game->hour.height= temp.height;



		//MENUS
		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_PLAYS);
		game->menus.play_s = LoadBitmapFile(full_path, &temp);
		if (game->menus.play_s == NULL){
			printf("Error Reading Play Selected Img \n");
			return 1;
		}
		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_PLAYUN);
		game->menus.play_un= LoadBitmapFile(full_path, &temp);
		if (game->menus.play_un == NULL){
			printf("Error Reading Play Unselected Img \n");
			return 1;
		}
		game->menus.play_w = temp.width;
		game->menus.play_h = temp.height;

		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_EXITS);
		game->menus.exit_s = LoadBitmapFile(full_path, &temp);
		if (game->menus.exit_s == NULL){
			printf("Error Reading Exit Selected Img \n");
			return 1;
		}
		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_EXITUN);
		game->menus.exit_un= LoadBitmapFile(full_path, &temp);
		if (game->menus.exit_un == NULL){
			printf("Error Reading Exit Unselected Img \n");
			return 1;
		}
		game->menus.exit_w = temp.width;
		game->menus.exit_h = temp.height;
		game->menus.playx = 314 ;
		game->menus.playy = 200;
		game->menus.exitx = 330;
		game->menus.exity = 279;


		//MOUSE
		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_MOUSE);
		game->mouse.cursor = LoadBitmapFile(full_path, &temp);
		if (game->mouse.cursor == NULL){
			printf("Error Reading Cursor Img \n");
			return 1;
		}
		game->mouse.x = 400;
		game->mouse.y = 150;
		game->mouse.width = temp.width;
		game->mouse.height = temp.height;

		memset(full_path,0,sizeof(full_path));
		strcat(full_path , game->current_dir);
		strcat(full_path , PATH_TO_LOGO);
		game->logo = LoadBitmapFile(full_path, &temp);
		if (game->logo == NULL){
			printf("Error Reading Logo Img \n");
			return 1;
		}
		game->logo_w = temp.width;
		game->logo_h = temp.height;

	}

	//STICK
	game->stick.width = 5;
	game->stick.x = 75;
	game->stick.y = 499;
	game->stick.size = 0;
	game->stick.max_size = STICK_MAX_SIZE;
	game->stick.status = 0;
	game->stick.color = STICK_COLOR;

	//MOUNTAIN 1
	game->mountain1.color = MOUNTAIN_COLOR;
	game->mountain1.size_x = 80;
	game->mountain1.size_y = 0;
	game->mountain1.x = 0;
	game->mountain1.y = MOUNTAIN_Y;


	//MOUNTAIN 2
	game->mountain2.color = MOUNTAIN_COLOR;
	game->mountain2.size_x = rand() % 80 + 20;
	game->mountain2.size_y = 0;
	game->mountain2.x = rand() % (600 - game->mountain2.size_x) + 180;
	game->mountain2.y = MOUNTAIN_Y;

	return 0;
}

void generateNewMountain(game_t *game){
	game->mountain1 = game->mountain2;
	game->mountain2.color = game->mountain1.color;
	game->mountain2.size_x = rand() % 80 + 20;
	game->mountain2.size_y = 0;
	game->mountain2.x = rand() % (600 - game->mountain2.size_x) + 180;
	game->mountain2.y = game->mountain1.y;
}

unsigned long int getHighscore(){
	return highscore;
}

void setHighscore(unsigned long int score){
	highscore = score;
}
