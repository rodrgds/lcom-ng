#include "tetris.h"

extern uint8_t scancode;
extern uint32_t time_counter;
extern uint32_t irq_set;
extern uint8_t irq_set_timer;
extern uint32_t irq_set_mouse;


tetris startGame(int speed) {
	tetris game;
	game.b1 = loadBitmap("/home/lcom/labs/proj/res/blue.bmp");
	game.b2 = loadBitmap("/home/lcom/labs/proj/res/green.bmp");
	game.b3 = loadBitmap("/home/lcom/labs/proj/res/light_blue.bmp");
	game.b4 = loadBitmap("/home/lcom/labs/proj/res/orange.bmp");
	game.b5 = loadBitmap("/home/lcom/labs/proj/res/purple.bmp");
	game.b6 = loadBitmap("/home/lcom/labs/proj/res/red.bmp");
	game.b7 = loadBitmap("/home/lcom/labs/proj/res/yellow.bmp");

	game.speed = speed;

	game.current_piece = rand() % 12 + 1;
	game.next_piece = rand() % 12 + 1;
	game.on_hold = 0;

	game.points = 0;

	game.background = loadBitmap("/home/lcom/labs/proj/res/background.bmp");

	numbers num;

	num.number0 = loadBitmap("/home/lcom/labs/proj/res/zero.bmp");
	num.number1 = loadBitmap("/home/lcom/labs/proj/res/one.bmp");
	num.number2 = loadBitmap("/home/lcom/labs/proj/res/two.bmp");
	num.number3 = loadBitmap("/home/lcom/labs/proj/res/three.bmp");
	num.number4 = loadBitmap("/home/lcom/labs/proj/res/four.bmp");
	num.number5 = loadBitmap("/home/lcom/labs/proj/res/five.bmp");
	num.number6 = loadBitmap("/home/lcom/labs/proj/res/six.bmp");
	num.number7 = loadBitmap("/home/lcom/labs/proj/res/seven.bmp");
	num.number8 = loadBitmap("/home/lcom/labs/proj/res/eight.bmp");
	num.number9 = loadBitmap("/home/lcom/labs/proj/res/nine.bmp");

	game.numbers = num;

	return game;
}


int fillWithZeros() {
	memset(board,0,sizeof(board));
	return 0;
}

int printBoard(tetris* game) {

	for (int i = 0; i < 22; i++) {
		for (int j = 0; j < 15; j++) {
			if (board[i][j] == 1) {
				drawBitmap(game->b1,j*32+270,i*32+32,getAuxBuffer());
			}
			else if (board[i][j] == 2) {
				drawBitmap(game->b2,j*32+270,i*32+32,getAuxBuffer());
			}
			else if (board[i][j] == 3) {
				drawBitmap(game->b3,j*32+270,i*32+32,getAuxBuffer());
			}
			else if (board[i][j] == 4) {
				drawBitmap(game->b4,j*32+270,i*32+32,getAuxBuffer());
			}
			else if (board[i][j] == 5) {
				drawBitmap(game->b5,j*32+270,i*32+32,getAuxBuffer());
			}
			else if (board[i][j] == 6) {
				drawBitmap(game->b6,j*32+270,i*32+32,getAuxBuffer());
			}
			else if (board[i][j] == 7) {
				drawBitmap(game->b7,j*32+270,i*32+32,getAuxBuffer());
			}
		}
	}

	return 0;
}


int dropPiece(tetris* game, menus_t* menus) {
	
	int blocks = 0;
	int color;

	int pos[5][2];

	// menus->difficulty += 1;

	
	memset(status, 0, sizeof(status));
	for (int i = 0; i < 22; i++) {
		for (int j = 0; j < 15; j++) {
			// printf("%d",board[i][j]);
			
			if (board[i][j] != 0) {
				status[i][j] = true;
			}
			else {
				status[i][j] = false;
			}
		}
		// printf("\n");
	}

	int height = 0;
	int width = 0;
	
	if (game->current_piece == 1) {
		board[0][7] = 6;
		color = 6;
		blocks = 1;
		pos[0][0] = 0; pos[0][1] = 7;
	}
	else if (game->current_piece == 2) {
		board[0][6] = 1;
		board[0][7] = 1;
		color = 1;
		blocks = 2;
		pos[0][0] = 0; pos[0][1] = 6;
		pos[1][0] = 0; pos[1][1] = 7;
		
		width = 2;
		height = 1;
	}
	else if (game->current_piece == 3) {
		board[0][6] = 2;
		board[0][7] = 2;
		board[0][8] = 2;
		color = 2;
		blocks = 3;
		pos[0][0] = 0; pos[0][1] = 6;
		pos[1][0] = 0; pos[1][1] = 7;
		pos[2][0] = 0; pos[2][1] = 8;

		width = 3;
		height = 1;
	}
	else if (game->current_piece == 4) {
		board[0][5] = 4;
		board[0][6] = 4;
		board[0][7] = 4;
		board[0][8] = 4;
		color = 4;
		blocks = 4;
		pos[0][0] = 0; pos[0][1] = 5;
		pos[1][0] = 0; pos[1][1] = 6;
		pos[2][0] = 0; pos[2][1] = 7;
		pos[3][0] = 0; pos[3][1] = 8;
		
		width = 4;
		height = 1;
	}
	else if (game->current_piece == 5) {
		board[0][6] = 6;
		board[0][7] = 6;
		board[1][6] = 6;
		board[1][7] = 6;
		color = 6;
		blocks = 4;
		pos[0][0] = 0; pos[0][1] = 6;
		pos[1][0] = 0; pos[1][1] = 7;
		pos[2][0] = 1; pos[2][1] = 6;
		pos[3][0] = 1; pos[3][1] = 7;

		width = 2;
		height = 2;
	}
		else if (game->current_piece == 6) {
		board[0][6] = 1;
		board[0][7] = 1;
		board[0][8] = 1;
		board[1][6] = 1;
		color = 1;
		blocks = 4;
		pos[0][0] = 0; pos[0][1] = 6;
		pos[1][0] = 0; pos[1][1] = 7;
		pos[2][0] = 0; pos[2][1] = 8;
		pos[3][0] = 1; pos[3][1] = 6;

		width = 3;
		height = 2;
	}
	else if (game->current_piece == 7) {
		board[0][6] = 5;
		board[0][7] = 5;
		board[0][8] = 5;
		board[1][8] = 5;
		color = 5;
		blocks = 4;
		pos[0][0] = 0; pos[0][1] = 6;
		pos[1][0] = 0; pos[1][1] = 7;
		pos[2][0] = 0; pos[2][1] = 8;
		pos[3][0] = 1; pos[3][1] = 8;

		width = 3;
		height = 2;
	}
	else if (game->current_piece == 8) {
		board[0][6] = 2;
		board[1][6] = 2;
		board[1][7] = 2;
		board[2][7] = 2;
		color = 2;
		blocks = 4;
		pos[0][0] = 0; pos[0][1] = 6;
		pos[1][0] = 1; pos[1][1] = 6;
		pos[2][0] = 1; pos[2][1] = 7;
		pos[3][0] = 2; pos[3][1] = 7;

		width = 2;
		height = 3;
	}
	else if (game->current_piece == 9) {
		board[0][7] = 5;
		board[1][6] = 5;
		board[1][7] = 5;
		board[2][6] = 5;
		color = 5;
		blocks = 4;
		pos[0][0] = 0; pos[0][1] = 7;
		pos[1][0] = 1; pos[1][1] = 6;
		pos[2][0] = 1; pos[2][1] = 7;
		pos[3][0] = 2; pos[3][1] = 6;

		width = 2;
		height = 3;
	}
		else if (game->current_piece == 10) {
		board[0][7] = 7;
		board[1][6] = 7;
		board[1][7] = 7;
		board[1][8] = 7;
		color = 7;
		blocks = 4;
		pos[0][0] = 0; pos[0][1] = 7;
		pos[1][0] = 1; pos[1][1] = 6;
		pos[2][0] = 1; pos[2][1] = 7;
		pos[3][0] = 1; pos[3][1] = 8;

		width = 3;
		height = 2;
	}
	else if (game->current_piece == 11) {
		board[0][6] = 4;
		board[0][7] = 4;
		board[1][7] = 4;
		color = 4;
		blocks = 3;
		pos[0][0] = 0; pos[0][1] = 6;
		pos[1][0] = 0; pos[1][1] = 7;
		pos[2][0] = 1; pos[2][1] = 7;

		width = 2;
		height = 2;
	}
	else if (game->current_piece == 12) {
		board[1][6] = 3;
		board[1][7] = 3;
		board[1][8] = 3;
		board[0][7] = 3;
		board[2][7] = 3;
		color = 3;
		blocks = 5;
		pos[0][0] = 1; pos[0][1] = 6;
		pos[1][0] = 1; pos[1][1] = 7;
		pos[2][0] = 1; pos[2][1] = 8;
		pos[3][0] = 0; pos[3][1] = 7;
		pos[4][0] = 2; pos[4][1] = 7;
	}

	if (checkColision(game,blocks,pos) == 3) {
		return 2;
	}

	// Interruption Loop
	int ipc_status;
  message msg;
  uint16_t r;

  time_counter = 0;
  int fps = 30 * 1 / game->speed;

  bool is_two_byte = false;   
  uint8_t scancode_bytes[2];

	updateFrame(game);
	printf("dropping piece %d\n", game->current_piece);
	printBoard(game);
	doubleBuffering();

	scancode_bytes[0] = 0;
	scancode_bytes[1] = 0;
	scancode = 0;

	// bool colision = false;
	bool bound = false;
	bool wait = false;
	int rotation = 0;
	bool holded = false;

	bool end = false;


	while (!end) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */

        	if (msg.m_notify.interrupts & irq_set_timer) {
            timer_int_handler();
							if (time_counter % fps == 0) {
								if (wait) {
								tickdelay(micros_to_ticks(DELAY_US));
								}

								if (!checkNearColisionBottom(game,blocks,pos)) {
									for (int i = 0; i < blocks; i++) {
										board[pos[i][0]][pos[i][1]] = 0;
									}
									for (int i = 0; i < blocks; i++) {
										board[pos[i][0] + 1][pos[i][1]] = color;
										pos[i][0] += 1;
									}
									updateFrame(game);
									printBoard(game);
									doubleBuffering();
								}
								else {
									end = true;
								}
            	}
          	}
						if (msg.m_notify.interrupts & irq_set) {
							kbc_ih();
							// only reads one byte at a time
							if (scancode == TWO_BYTE_CODE) {
							is_two_byte = true;
							scancode_bytes[0] = scancode;
							} else {
								if (is_two_byte) {
									is_two_byte = false;
									scancode_bytes[1] = scancode;
								} else {
									scancode_bytes[0] = scancode;
								}
							}

							// PAUSE MENU
							if (scancode == ESC_MAKE) {
								char* temp = (char *) malloc(getVRAMSize());
								memcpy(temp, getAuxBuffer(), getVRAMSize());
								int p = pauseMenu(menus);
								
								if (p == 1) {
									return 1;
								}
								else if (p == 0) {
									updateFrame(game);
									//resetSecondBuffer();
									//memcpy(getGraphicsBuffer(),temp,getVRAMSize());
								}
							}

							// SIDE MOVEMENTS
							if (scancode_bytes[1] == LEFT_MAKE) {
								wait = true;

								if (!checkNearColisionLeft(game,blocks,pos)) {
									for (int i = 0; i < blocks; i++) {
										board[pos[i][0]][pos[i][1]] = 0;
									}
									for (int i = 0; i < blocks; i++) {
										board[pos[i][0]][pos[i][1]-1] = color;
										pos[i][1] -= 1;
									}
									updateFrame(game);
									printBoard(game);
									doubleBuffering();
								}
								scancode_bytes[1] = 0;
								bound = false; // reset value
							}
							if (scancode_bytes[1] == RIGHT_MAKE) {
								wait = true;

								if (!checkNearColisionRight(game,blocks,pos)) {
									for (int i = 0; i < blocks; i++) {
										board[pos[i][0]][pos[i][1]] = 0;
									}
									for (int i = 0; i < blocks; i++) {
										board[pos[i][0]][pos[i][1] + 1] = color;
										pos[i][1] += 1;
									}	
									updateFrame(game);
									printBoard(game);
									doubleBuffering();
								}
								bound = false; // reset value
							}

							// ROTATION MOVEMENTS
							if (scancode == D_MAKE) {
								wait = true;
								// printf("CLOCKWISE ROTATION - NOT YET IMPLEMENTED\n");

								rotation++;
								if (rotation == 4)
									rotation = 0;

								// no need to do it for piece 1,5,12
								for (int i = 0; i < blocks; i++) {
									board[pos[i][0]][pos[i][1]] = 0;
								}

								if (game->current_piece == 2) {
									int stand[2][2];
									for (int i = 0; i < blocks; i++) {
										stand[i][0] = pos[i][0];
										stand[i][1] = pos[i][1];
									}

									if (rotation == 1 || rotation == 3) {	
										pos[0][0] -= 1;
										pos[0][1] += 1;
									}
									else {
										pos[0][0] += 1;
										pos[0][1] -= 1;
									}

									while (checkColision(game,blocks,pos) != 0) {
										if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
											
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
											if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
												for (int i = 0; i < blocks; i++) {
														pos[i][0] = stand[i][0];
														pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 3) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {	
													pos[i][1] -= 1;
													pos[i][0] -= 1;
												}
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {
													pos[i][0] = stand[i][0];
													pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 4) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] -= 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 5) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
										}
									}
								}

								if (game->current_piece == 3) {
									for (int i = 0; i < blocks; i++) {
											board[pos[i][0]][pos[i][1]] = 0;
									}

									int stand[3][2];
									for (int i = 0; i < blocks; i++) {
										stand[i][0] = pos[i][0];
										stand[i][1] = pos[i][1];
									}

									if (rotation == 1 || rotation == 3) {	
										pos[0][0] -= 2;
										pos[0][1] += 1;
										pos[2][0] -= 1;
										pos[2][1] -= 1;
									}
									else {
										pos[0][0] += 2;
										pos[0][1] -= 1;
										pos[2][0] += 1;
										pos[2][1] += 1;
									}

									// colision handling
									while (checkColision(game,blocks,pos) != 0) {
										printf("colision: %d\n",checkColision(game,blocks,pos));

										if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
											
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
											if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
												for (int i = 0; i < blocks; i++) {
														pos[i][0] = stand[i][0];
														pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 3) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {	
													pos[i][1] += 1;
													// pos[i][0] -= 1;
												}
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {
													pos[i][0] = stand[i][0];
													pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 4) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] -= 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 5) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 6) {
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
										}
									}

								}

								if (game->current_piece == 4) {
									for (int i = 0; i < blocks; i++) {
											board[pos[i][0]][pos[i][1]] = 0;
									}

									int stand[4][2];
									for (int i = 0; i < blocks; i++) {
										stand[i][0] = pos[i][0];
										stand[i][1] = pos[i][1];
									}

									if (rotation == 1 || rotation == 3) {	
										pos[0][0] -= 2;
										pos[0][1] += 2;
										pos[1][0] -= 1;
										pos[1][1] += 1;
										pos[3][0] += 1;
										pos[3][1] -= 1;
									}
									else {
										pos[0][0] += 2;
										pos[0][1] -= 2;
										pos[1][0] += 1;
										pos[1][1] -= 1;
										pos[3][0] -= 1;
										pos[3][1] += 1;
									}

									// colision handling
									while (checkColision(game,blocks,pos) != 0) {
										printf("colision: %d\n",checkColision(game,blocks,pos));

										if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
											
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
											if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
												for (int i = 0; i < blocks; i++) {
														pos[i][0] = stand[i][0];
														pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 3) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {	
													pos[i][1] += 1;
													// pos[i][0] -= 1;
												}
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {
													pos[i][0] = stand[i][0];
													pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 4) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] -= 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 5) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 6) {
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
										}
									}
								}

								if (game->current_piece == 6) {
									for (int i = 0; i < blocks; i++) {
											board[pos[i][0]][pos[i][1]] = 0;
									}

									int stand[4][2];
									for (int i = 0; i < blocks; i++) {
										stand[i][0] = pos[i][0];
										stand[i][1] = pos[i][1];
									}

									if (rotation == 1) {	
										pos[0][0] -= 1;
										pos[0][1] += 1;
										pos[2][0] += 1;
										pos[2][1] -= 1;
										pos[3][0] -= 2;
									}
									else if (rotation == 2) {
										pos[0][0] += 1;
										pos[0][1] += 1;
										pos[2][0] -= 1;
										pos[2][1] -= 1;
										pos[3][1] += 2;
									}
									else if (rotation == 3) {
										pos[0][0] += 1;
										pos[0][1] -= 2;
										pos[1][1] -= 1;
										pos[2][0] -= 1;
										pos[3][0] += 2;
										pos[3][1] -= 1;
									}
									else {
										pos[0][0] -= 1;
										pos[1][1] += 1;
										pos[2][0] += 1;
										pos[2][1] += 2;
										pos[3][1] -= 1;
									}

									// colision handling
									while (checkColision(game,blocks,pos) != 0) {
										printf("colision: %d\n",checkColision(game,blocks,pos));

										if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
											
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
											if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
												for (int i = 0; i < blocks; i++) {
														pos[i][0] = stand[i][0];
														pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 3) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {	
													pos[i][1] += 1;
													// pos[i][0] -= 1;
												}
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {
													pos[i][0] = stand[i][0];
													pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 4) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] -= 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 5) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 6) {
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
										}
									}
								}

								if (game->current_piece == 7) {
									for (int i = 0; i < blocks; i++) {
											board[pos[i][0]][pos[i][1]] = 0;
									}

									int stand[4][2];
									for (int i = 0; i < blocks; i++) {
										stand[i][0] = pos[i][0];
										stand[i][1] = pos[i][1];
									}

									if (rotation == 1) {	
										pos[0][0] -= 1;
										pos[0][1] += 2;
										pos[1][1] += 1;
										pos[2][0] += 1;
										pos[3][1] -= 1;
									}
									else if (rotation == 2) {
										pos[0][0] += 1;
										pos[1][1] -= 1;
										pos[2][0] -= 1;
										pos[2][1] -= 2;
										pos[3][0] -= 2;
										pos[3][1] -= 1;
									}
									else if (rotation == 3) {
										pos[0][0] += 1;
										pos[0][1] -= 1;
										pos[2][0] -= 1;
										pos[2][1] += 1;
										pos[3][1] += 2;
									}
									else {
										pos[0][0] -= 1;
										pos[0][1] -= 1;
										pos[2][0] += 1;
										pos[2][1] += 1;
										pos[3][0] += 2;
									}

									// colision handling
									while (checkColision(game,blocks,pos) != 0) {
										printf("colision: %d\n",checkColision(game,blocks,pos));

										if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
											
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
											if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
												for (int i = 0; i < blocks; i++) {
														pos[i][0] = stand[i][0];
														pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 3) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {	
													pos[i][1] += 1;
													// pos[i][0] -= 1;
												}
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {
													pos[i][0] = stand[i][0];
													pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 4) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] -= 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 5) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 6) {
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
										}
									}
								}

								if (game->current_piece == 8) {
									for (int i = 0; i < blocks; i++) {
											board[pos[i][0]][pos[i][1]] = 0;
									}

									int stand[4][2];
									for (int i = 0; i < blocks; i++) {
										stand[i][0] = pos[i][0];
										stand[i][1] = pos[i][1];
									}

									if (rotation == 1 || rotation == 3) {	
										pos[0][1] += 2;
										pos[1][0] -= 1;
										pos[1][1] += 1;
										pos[3][0] -= 1;
										pos[3][1] -= 1;
									}
									else {
										pos[0][1] -= 2;
										pos[1][0] += 1;
										pos[1][1] -= 1;
										pos[3][0] += 1;
										pos[3][1] += 1;
									}

									// colision handling
									while (checkColision(game,blocks,pos) != 0) {
										printf("colision: %d\n",checkColision(game,blocks,pos));

										if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
											
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
											if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
												for (int i = 0; i < blocks; i++) {
														pos[i][0] = stand[i][0];
														pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 3) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {	
													pos[i][1] += 1;
													// pos[i][0] -= 1;
												}
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {
													pos[i][0] = stand[i][0];
													pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 4) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] -= 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 5) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 6) {
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
										}
									}
								}

								if (game->current_piece == 9) {
									for (int i = 0; i < blocks; i++) {
											board[pos[i][0]][pos[i][1]] = 0;
									}

									int stand[4][2];
									for (int i = 0; i < blocks; i++) {
										stand[i][0] = pos[i][0];
										stand[i][1] = pos[i][1];
									}

									if (rotation == 1 || rotation == 3) {	
										pos[0][0] += 1;
										pos[1][0] -= 1;
										pos[2][1] -= 1;
										pos[3][0] -= 2;
										pos[3][1] -= 1;
									}
									else {
										pos[0][0] -= 1;
										pos[1][0] += 1;
										pos[2][1] += 1;
										pos[3][0] += 2;
										pos[3][1] += 1;
									}

									// colision handling
									while (checkColision(game,blocks,pos) != 0) {
										printf("colision: %d\n",checkColision(game,blocks,pos));

										if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
											
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
											if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
												for (int i = 0; i < blocks; i++) {
														pos[i][0] = stand[i][0];
														pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 3) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {	
													pos[i][1] += 1;
													// pos[i][0] -= 1;
												}
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {
													pos[i][0] = stand[i][0];
													pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 4) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] -= 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 5) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 6) {
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
										}
									}
								}

								if (game->current_piece == 10) {
									for (int i = 0; i < blocks; i++) {
											board[pos[i][0]][pos[i][1]] = 0;
									}

									int stand[4][2];
									for (int i = 0; i < blocks; i++) {
										stand[i][0] = pos[i][0];
										stand[i][1] = pos[i][1];
									}

									if (rotation == 1) {	
										pos[0][0] += 1;
										pos[0][1] += 1;
										pos[1][0] -= 1;
										pos[1][1] += 1;
										pos[3][0] += 1;
										pos[3][1] -= 1;
									}
									else if (rotation == 2) {
										pos[0][0] += 1;
										pos[0][1] -= 1;
										pos[1][0] += 1;
										pos[1][1] += 1;
										pos[3][0] -= 1;
										pos[3][1] -= 1;
									}
									else if (rotation == 3) {
										pos[0][0] -= 1;
										pos[0][1] -= 1;
										pos[1][0] += 1;
										pos[1][1] -= 1;
										pos[3][0] -= 1;
										pos[3][1] += 1;
									}
									else {
										pos[0][0] -= 1;
										pos[0][1] += 1;
										pos[1][0] -= 1;
										pos[1][1] -= 1;
										pos[3][0] += 1;
										pos[3][1] += 1;
									}

									// colision handling
									while (checkColision(game,blocks,pos) != 0) {
										printf("colision: %d\n",checkColision(game,blocks,pos));

										if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
											
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
											if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
												for (int i = 0; i < blocks; i++) {
														pos[i][0] = stand[i][0];
														pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 3) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {	
													pos[i][1] += 1;
													// pos[i][0] -= 1;
												}
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {
													pos[i][0] = stand[i][0];
													pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 4) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] -= 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 5) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 6) {
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
										}
									}
								}

								if (game->current_piece == 11) {
									for (int i = 0; i < blocks; i++) {
											board[pos[i][0]][pos[i][1]] = 0;
									}

									int stand[3][2];
									for (int i = 0; i < blocks; i++) {
										stand[i][0] = pos[i][0];
										stand[i][1] = pos[i][1];
									}

									if (rotation == 1) {	
										pos[0][1] += 1;
										pos[1][0] += 1;
										pos[2][1] -= 1;
									}
									else if (rotation == 2) {
										pos[0][0] += 1;
										pos[1][1] -= 1;
										pos[2][0] -= 1;
									}
									else if (rotation == 3) {
										pos[0][1] -= 1;
										pos[1][0] -= 1;
										pos[2][1] += 1;
									}
									else {
										pos[0][0] -= 1;
										pos[1][1] += 1;
										pos[2][0] += 1;
									}

									// colision handling
									while (checkColision(game,blocks,pos) != 0) {
										printf("colision: %d\n",checkColision(game,blocks,pos));

										if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
											
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
											if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
												for (int i = 0; i < blocks; i++) {
														pos[i][0] = stand[i][0];
														pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 3) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {	
													pos[i][1] += 1;
													// pos[i][0] -= 1;
												}
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {
													pos[i][0] = stand[i][0];
													pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 4) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] -= 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 5) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 6) {
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
										}
									}
								}

								for (int i = 0; i < blocks; i++) {
									board[pos[i][0]][pos[i][1]] = color;
								}
								updateFrame(game);
								printBoard(game);
								doubleBuffering();
							}
							if (scancode == A_MAKE) {

								rotation--;
								if (rotation == -1)
									rotation = 3;

								wait = true;           

								// no need to do it for piece 1,5,12
								for (int i = 0; i < blocks; i++) {
									board[pos[i][0]][pos[i][1]] = 0;
								}

								if (game->current_piece == 2) {
									int stand[2][2];
									for (int i = 0; i < blocks; i++) {
										stand[i][0] = pos[i][0];
										stand[i][1] = pos[i][1];
									}

									if (rotation == 1 || rotation == 3) {	
										pos[0][0] -= 1;
										pos[0][1] += 1;
									}
									else {
										pos[0][0] += 1;
										pos[0][1] -= 1;
									}

									while (checkColision(game,blocks,pos) != 0) {
										if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
											
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
											if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
												for (int i = 0; i < blocks; i++) {
														pos[i][0] = stand[i][0];
														pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 3) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {	
													pos[i][1] -= 1;
													pos[i][0] -= 1;
												}
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {
													pos[i][0] = stand[i][0];
													pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 4) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] -= 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 5) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
										}
									}
								}

								if (game->current_piece == 3) {
									for (int i = 0; i < blocks; i++) {
											board[pos[i][0]][pos[i][1]] = 0;
									}

									int stand[3][2];
									for (int i = 0; i < blocks; i++) {
										stand[i][0] = pos[i][0];
										stand[i][1] = pos[i][1];
									}

									if (rotation == 1 || rotation == 3) {	
										pos[0][0] -= 2;
										pos[0][1] += 1;
										pos[2][0] -= 1;
										pos[2][1] -= 1;
									}
									else {
										pos[0][0] += 2;
										pos[0][1] -= 1;
										pos[2][0] += 1;
										pos[2][1] += 1;
									}

									// colision handling
									while (checkColision(game,blocks,pos) != 0) {
										printf("colision: %d\n",checkColision(game,blocks,pos));

										if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
											
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
											if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
												for (int i = 0; i < blocks; i++) {
														pos[i][0] = stand[i][0];
														pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 3) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {	
													pos[i][1] += 1;
													// pos[i][0] -= 1;
												}
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {
													pos[i][0] = stand[i][0];
													pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 4) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] -= 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 5) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 6) {
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
										}
									}

								}

								if (game->current_piece == 4) {
									for (int i = 0; i < blocks; i++) {
											board[pos[i][0]][pos[i][1]] = 0;
									}

									int stand[4][2];
									for (int i = 0; i < blocks; i++) {
										stand[i][0] = pos[i][0];
										stand[i][1] = pos[i][1];
									}

									if (rotation == 1 || rotation == 3) {	
										pos[0][0] -= 2;
										pos[0][1] += 2;
										pos[1][0] -= 1;
										pos[1][1] += 1;
										pos[3][0] += 1;
										pos[3][1] -= 1;
									}
									else {
										pos[0][0] += 2;
										pos[0][1] -= 2;
										pos[1][0] += 1;
										pos[1][1] -= 1;
										pos[3][0] -= 1;
										pos[3][1] += 1;
									}

									// colision handling
									while (checkColision(game,blocks,pos) != 0) {
										printf("colision: %d\n",checkColision(game,blocks,pos));

										if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
											
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
											if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
												for (int i = 0; i < blocks; i++) {
														pos[i][0] = stand[i][0];
														pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 3) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {	
													pos[i][1] += 1;
													// pos[i][0] -= 1;
												}
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {
													pos[i][0] = stand[i][0];
													pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 4) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] -= 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 5) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 6) {
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
										}
									}
								}

								if (game->current_piece == 6) {
									for (int i = 0; i < blocks; i++) {
											board[pos[i][0]][pos[i][1]] = 0;
									}

									int stand[4][2];
									for (int i = 0; i < blocks; i++) {
										stand[i][0] = pos[i][0];
										stand[i][1] = pos[i][1];
									}

									if (rotation == 1) {	
										pos[0][0] -= 1;
										pos[0][1] -= 1;
										pos[2][0] += 1;
										pos[2][1] += 1;
										pos[3][1] -= 2;
									}
									else if (rotation == 2) {
										pos[0][0] -= 1;
										pos[0][1] += 2;
										pos[1][1] += 1;
										pos[2][0] += 1;
										pos[3][0] -= 2;
										pos[3][1] += 1;
									}
									else if (rotation == 3) {
										pos[0][0] += 1;
										pos[1][1] -= 1;
										pos[2][0] -= 1;
										pos[2][1] -= 2;
										pos[3][1] += 1;
									}
									else {
										pos[0][0] += 1;
										pos[0][1] -= 1;
										pos[2][0] -= 1;
										pos[2][1] += 1;
										pos[3][0] += 2;
									}

									// colision handling
									while (checkColision(game,blocks,pos) != 0) {
										printf("colision: %d\n",checkColision(game,blocks,pos));

										if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
											
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
											if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
												for (int i = 0; i < blocks; i++) {
														pos[i][0] = stand[i][0];
														pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 3) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {	
													pos[i][1] += 1;
													// pos[i][0] -= 1;
												}
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {
													pos[i][0] = stand[i][0];
													pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 4) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] -= 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 5) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 6) {
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
										}
									}
								}

								if (game->current_piece == 7) {
									for (int i = 0; i < blocks; i++) {
											board[pos[i][0]][pos[i][1]] = 0;
									}

									int stand[4][2];
									for (int i = 0; i < blocks; i++) {
										stand[i][0] = pos[i][0];
										stand[i][1] = pos[i][1];
									}

									if (rotation == 1) {	
										pos[0][0] -= 1;
										pos[1][1] += 1;
										pos[2][0] += 1;
										pos[2][1] += 2;
										pos[3][0] += 2;
										pos[3][1] += 1;
									}
									else if (rotation == 2) {
										pos[0][0] -= 1;
										pos[0][1] += 1;
										pos[2][0] += 1;
										pos[2][1] -= 1;
										pos[3][1] -= 2;
									}
									else if (rotation == 3) {
										pos[0][0] += 1;
										pos[0][1] += 1;
										pos[2][0] -= 1;
										pos[2][1] -= 1;
										pos[3][0] -= 2;
									}
									else {
										pos[0][0] += 1;
										pos[0][1] -= 2;
										pos[1][1] -= 1;
										pos[2][0] -= 1;
										pos[3][1] += 1;
									}

									// colision handling
									while (checkColision(game,blocks,pos) != 0) {
										printf("colision: %d\n",checkColision(game,blocks,pos));

										if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
											
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
											if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
												for (int i = 0; i < blocks; i++) {
														pos[i][0] = stand[i][0];
														pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 3) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {	
													pos[i][1] += 1;
													// pos[i][0] -= 1;
												}
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {
													pos[i][0] = stand[i][0];
													pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 4) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] -= 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 5) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 6) {
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
										}
									}
								}

								if (game->current_piece == 8) {
									for (int i = 0; i < blocks; i++) {
											board[pos[i][0]][pos[i][1]] = 0;
									}

									int stand[4][2];
									for (int i = 0; i < blocks; i++) {
										stand[i][0] = pos[i][0];
										stand[i][1] = pos[i][1];
									}

									if (rotation == 1 || rotation == 3) {	
										pos[0][1] += 2;
										pos[1][0] -= 1;
										pos[1][1] += 1;
										pos[3][0] -= 1;
										pos[3][1] -= 1;
									}
									else {
										pos[0][1] -= 2;
										pos[1][0] += 1;
										pos[1][1] -= 1;
										pos[3][0] += 1;
										pos[3][1] += 1;
									}

									// colision handling
									while (checkColision(game,blocks,pos) != 0) {
										printf("colision: %d\n",checkColision(game,blocks,pos));

										if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
											
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
											if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
												for (int i = 0; i < blocks; i++) {
														pos[i][0] = stand[i][0];
														pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 3) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {	
													pos[i][1] += 1;
													// pos[i][0] -= 1;
												}
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {
													pos[i][0] = stand[i][0];
													pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 4) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] -= 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 5) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 6) {
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
										}
									}
								}

								if (game->current_piece == 9) {
									for (int i = 0; i < blocks; i++) {
											board[pos[i][0]][pos[i][1]] = 0;
									}

									int stand[4][2];
									for (int i = 0; i < blocks; i++) {
										stand[i][0] = pos[i][0];
										stand[i][1] = pos[i][1];
									}

									if (rotation == 1 || rotation == 3) {	
										pos[0][0] += 1;
										pos[1][0] -= 1;
										pos[2][1] -= 1;
										pos[3][0] -= 2;
										pos[3][1] -= 1;
									}
									else {
										pos[0][0] -= 1;
										pos[1][0] += 1;
										pos[2][1] += 1;
										pos[3][0] += 2;
										pos[3][1] += 1;
									}

									// colision handling
									while (checkColision(game,blocks,pos) != 0) {
										printf("colision: %d\n",checkColision(game,blocks,pos));

										if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
											
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
											if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
												for (int i = 0; i < blocks; i++) {
														pos[i][0] = stand[i][0];
														pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 3) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {	
													pos[i][1] += 1;
													// pos[i][0] -= 1;
												}
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {
													pos[i][0] = stand[i][0];
													pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 4) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] -= 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 5) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 6) {
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
										}
									}
								}

								if (game->current_piece == 10) {
									for (int i = 0; i < blocks; i++) {
											board[pos[i][0]][pos[i][1]] = 0;
									}

									int stand[4][2];
									for (int i = 0; i < blocks; i++) {
										stand[i][0] = pos[i][0];
										stand[i][1] = pos[i][1];
									}

									if (rotation == 1) {	
										pos[0][0] -= 1;
										pos[0][1] += 1;
										pos[1][0] -= 1;
										pos[1][1] -= 1;
										pos[3][0] += 1;
										pos[3][1] += 1;
									}
									else if (rotation == 2) {
										pos[0][0] += 1;
										pos[0][1] += 1;
										pos[1][0] -= 1;
										pos[1][1] += 1;
										pos[3][0] += 1;
										pos[3][1] -= 1;
									}
									else if (rotation == 3) {
										pos[0][0] += 1;
										pos[0][1] -= 1;
										pos[1][0] += 1;
										pos[1][1] += 1;
										pos[3][0] -= 1;
										pos[3][1] -= 1;
									}
									else {
										pos[0][0] -= 1;
										pos[0][1] -= 1;
										pos[1][0] += 1;
										pos[1][1] -= 1;
										pos[3][0] -= 1;
										pos[3][1] += 1;
									}

									// colision handling
									while (checkColision(game,blocks,pos) != 0) {
										printf("colision: %d\n",checkColision(game,blocks,pos));

										if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
											
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
											if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
												for (int i = 0; i < blocks; i++) {
														pos[i][0] = stand[i][0];
														pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 3) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {	
													pos[i][1] += 1;
													// pos[i][0] -= 1;
												}
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {
													pos[i][0] = stand[i][0];
													pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 4) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] -= 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 5) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 6) {
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
										}
									}
								}

								if (game->current_piece == 11) {
									for (int i = 0; i < blocks; i++) {
											board[pos[i][0]][pos[i][1]] = 0;
									}

									int stand[3][2];
									for (int i = 0; i < blocks; i++) {
										stand[i][0] = pos[i][0];
										stand[i][1] = pos[i][1];
									}

									if (rotation == 1) {	
										pos[0][0] -= 1;
										pos[1][1] += 1;
										pos[2][0] += 1;
									}
									else if (rotation == 2) {
										pos[0][1] += 1;
										pos[1][0] += 1;
										pos[2][1] -= 1;
									}
									else if (rotation == 3) {
										pos[0][0] += 1;
										pos[1][1] -= 1;
										pos[2][0] -= 1;
									}
									else {
										pos[0][1] -= 1;
										pos[1][0] -= 1;
										pos[2][1] += 1;
									}

									// colision handling
									while (checkColision(game,blocks,pos) != 0) {
										printf("colision: %d\n",checkColision(game,blocks,pos));

										if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
											
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
											if (checkColision(game,blocks,pos) == 1 || 
												checkColision(game,blocks,pos) == 2) {
												for (int i = 0; i < blocks; i++) {
														pos[i][0] = stand[i][0];
														pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 3) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {	
													pos[i][1] += 1;
													// pos[i][0] -= 1;
												}
											}
											if (checkColision(game,blocks,pos) == 3) {
												for (int i = 0; i < blocks; i++) {
													pos[i][0] = stand[i][0];
													pos[i][1] = stand[i][1];
												}
											}
										}
										else if (checkColision(game,blocks,pos) == 4) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] -= 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 5) {
											for (int i = 0; i < blocks; i++) {
												pos[i][1] += 1;
											}
										}
										else if (checkColision(game,blocks,pos) == 6) {
											for (int i = 0; i < blocks; i++) {
												pos[i][0] -= 1;
											}
										}
									}
								}

								for (int i = 0; i < blocks; i++) {
									board[pos[i][0]][pos[i][1]] = color;
								}
								updateFrame(game);
								printBoard(game);
								doubleBuffering();
							}

							// SPEED UP DOWN MOVEMENT
							if (scancode_bytes[1] == DOWN_MAKE) {
								if (!checkNearColisionBottom(game,blocks,pos)) {
								
									for (int i = 0; i < blocks; i++) {
										board[pos[i][0]][pos[i][1]] = 0;
									}
									for (int i = 0; i < blocks; i++) {
										board[pos[i][0] + 1][pos[i][1]] = color;
										pos[i][0] += 1;
									}

									updateFrame(game);
									printBoard(game);
									doubleBuffering();
								} else {
									end = true;
									break;
								}
							}

							// INSTANT DROP
							if (scancode_bytes[1] == UP_MAKE) {
								for (int i = 0; i < blocks; i++) {
										board[pos[i][0]][pos[i][1]] = 0;
								}
								while (!checkNearColisionBottom(game,blocks,pos)) {
									for (int i = 0; i < blocks; i++) {
										pos[i][0] += 1;
									}
								}
								for (int i = 0; i < blocks; i++) {
									board[pos[i][0]][pos[i][1]] = color;
								}
								updateFrame(game);
								printBoard(game);
								doubleBuffering();
								end = true;
								break;
							}

							// PIECE ON HOLD FEATURE
							if (scancode == SPACE_MAKE) {
								if (game->on_hold == 0) {
									game->on_hold = game->current_piece;
									game->current_piece = game->next_piece;
									holded = true;
								}
								else {
									int temp = game->current_piece;
									game->current_piece = game->on_hold;
									game->on_hold = temp;
									holded = true;
								}
								for (int i = 0; i < blocks; i++) {
									board[pos[i][0]][pos[i][1]] = 0;
								}
								end = true;
								break;
							}

							scancode_bytes[1] = 0;
							wait = false;
							}
							
						break;
									default:
											break; /* no other notifications expected: do nothing */
							}
					} else { /* received a standard message, not a notification */
							/* no standard messages expected: do nothing */
					}
		}
	scancode = 0;

	if (!holded) {
		game->current_piece = game->next_piece;
		game->next_piece = rand() % 12 + 1;
	}


	return 0;
}


int desintegrateLines(tetris* game) {

	int temp = game->points;
	for (int i = 0; i < 22; i++) {
		bool full = true;
		for (int j = 0; j < 15; j++) {
			if (board[i][j] == 0) {
				full = false;
			}
		}

		int k = i;
		if (i == 0 && full)
			printf("GAMEOVER!\n");
		
		if (full) {
			game->points += 10;
			while (k != 0) {
				memcpy(board[k],board[k-1],sizeof(board[k]));
				k--;
			}
		}
	}
	if (game->points != temp) {
		return 1;
	}

	return 0;
}

int printNext(tetris* game) {

	if (game->next_piece == 1) {
		drawBitmap(game->b6,810+32*2,164,getAuxBuffer());
	}
	if (game->next_piece == 2) {
		drawBitmap(game->b1,810+32*2-16,164,getAuxBuffer());
		drawBitmap(game->b1,810+32*2+16,164,getAuxBuffer());
	}
	if (game->next_piece == 3) {
		drawBitmap(game->b2,810+32,164,getAuxBuffer());
		drawBitmap(game->b2,810+32*2,164,getAuxBuffer());
		drawBitmap(game->b2,810+32*3,164,getAuxBuffer());
	}
	if (game->next_piece == 4) {
		drawBitmap(game->b4,810+32-16,164,getAuxBuffer());
		drawBitmap(game->b4,810+32+16,164,getAuxBuffer());
		drawBitmap(game->b4,810+64+16,164,getAuxBuffer());
		drawBitmap(game->b4,810+96+16,164,getAuxBuffer());
	}
	if (game->next_piece == 5) {
		drawBitmap(game->b6,810+32*2-16,164-16,getAuxBuffer());
		drawBitmap(game->b6,810+32*2+16,164-16,getAuxBuffer());
		drawBitmap(game->b6,810+32*2-16,164+16,getAuxBuffer());
		drawBitmap(game->b6,810+32*2+16,164+16,getAuxBuffer());
	}
	if (game->next_piece == 6) {
		drawBitmap(game->b1,810+32,164-16,getAuxBuffer());
		drawBitmap(game->b1,810+32*2,164-16,getAuxBuffer());
		drawBitmap(game->b1,810+32*3,164-16,getAuxBuffer());
		drawBitmap(game->b1,810+32,164+16,getAuxBuffer());
	}
	if (game->next_piece == 7) {
		drawBitmap(game->b5,810+32,164-16,getAuxBuffer());
		drawBitmap(game->b5,810+32*2,164-16,getAuxBuffer());
		drawBitmap(game->b5,810+32*3,164-16,getAuxBuffer());
		drawBitmap(game->b5,810+32*3,164+16,getAuxBuffer());
	}
	if (game->next_piece == 8) {
		drawBitmap(game->b2,810+32*2-16,164-32,getAuxBuffer());
		drawBitmap(game->b2,810+32*2-16,164,getAuxBuffer());
		drawBitmap(game->b2,810+32*2+16,164,getAuxBuffer());
		drawBitmap(game->b2,810+32*2+16,164+32,getAuxBuffer());
	}
	if (game->next_piece == 9) {
		drawBitmap(game->b5,810+32*2+16,164-32,getAuxBuffer());
		drawBitmap(game->b5,810+32*2-16,164,getAuxBuffer());
		drawBitmap(game->b5,810+32*2+16,164,getAuxBuffer());
		drawBitmap(game->b5,810+32*2-16,164+32,getAuxBuffer());
	}
	if (game->next_piece == 10) {
		drawBitmap(game->b7,810+32,164+16,getAuxBuffer());
		drawBitmap(game->b7,810+32*2,164+16,getAuxBuffer());
		drawBitmap(game->b7,810+32*3,164+16,getAuxBuffer());
		drawBitmap(game->b7,810+32*2,164-16,getAuxBuffer());
	}
	if (game->next_piece == 11) {
		drawBitmap(game->b4,810+32*2-16,164-16,getAuxBuffer());
		drawBitmap(game->b4,810+32*2+16,164-16,getAuxBuffer());
		drawBitmap(game->b4,810+32*2+16,164+16,getAuxBuffer());
	}
	if (game->next_piece == 12) {
		drawBitmap(game->b3,810+32*1,164,getAuxBuffer());
		drawBitmap(game->b3,810+32*2,164,getAuxBuffer());
		drawBitmap(game->b3,810+32*3,164,getAuxBuffer());
		drawBitmap(game->b3,810+32*2,164-32,getAuxBuffer());
		drawBitmap(game->b3,810+32*2,164+32,getAuxBuffer());
	}

	return 0;
}


int printOnHold(tetris* game) {
	if (game->on_hold == 1) {
		drawBitmap(game->b6,55+32*2,164,getAuxBuffer());
	}
	if (game->on_hold == 2) {
		drawBitmap(game->b1,55+32*2-16,164,getAuxBuffer());
		drawBitmap(game->b1,55+32*2+16,164,getAuxBuffer());
	}
	if (game->on_hold == 3) {
		drawBitmap(game->b2,55+32,164,getAuxBuffer());
		drawBitmap(game->b2,55+32*2,164,getAuxBuffer());
		drawBitmap(game->b2,55+32*3,164,getAuxBuffer());
	}
	if (game->on_hold == 4) {
		drawBitmap(game->b4,55+32-16,164,getAuxBuffer());
		drawBitmap(game->b4,55+32+16,164,getAuxBuffer());
		drawBitmap(game->b4,55+64+16,164,getAuxBuffer());
		drawBitmap(game->b4,55+96+16,164,getAuxBuffer());
	}
	if (game->on_hold == 5) {
		drawBitmap(game->b6,55+32*2-16,164-16,getAuxBuffer());
		drawBitmap(game->b6,55+32*2+16,164-16,getAuxBuffer());
		drawBitmap(game->b6,55+32*2-16,164+16,getAuxBuffer());
		drawBitmap(game->b6,55+32*2+16,164+16,getAuxBuffer());
	}
	if (game->on_hold == 6) {
		drawBitmap(game->b1,55+32,164-16,getAuxBuffer());
		drawBitmap(game->b1,55+32*2,164-16,getAuxBuffer());
		drawBitmap(game->b1,55+32*3,164-16,getAuxBuffer());
		drawBitmap(game->b1,55+32,164+16,getAuxBuffer());
	}
	if (game->on_hold == 7) {
		drawBitmap(game->b5,55+32,164-16,getAuxBuffer());
		drawBitmap(game->b5,55+32*2,164-16,getAuxBuffer());
		drawBitmap(game->b5,55+32*3,164-16,getAuxBuffer());
		drawBitmap(game->b5,55+32*3,164+16,getAuxBuffer());
	}
	if (game->on_hold == 8) {
		drawBitmap(game->b2,55+32*2-16,164-32,getAuxBuffer());
		drawBitmap(game->b2,55+32*2-16,164,getAuxBuffer());
		drawBitmap(game->b2,55+32*2+16,164,getAuxBuffer());
		drawBitmap(game->b2,55+32*2+16,164+32,getAuxBuffer());
	}
	if (game->on_hold == 9) {
		drawBitmap(game->b5,55+32*2+16,164-32,getAuxBuffer());
		drawBitmap(game->b5,55+32*2-16,164,getAuxBuffer());
		drawBitmap(game->b5,55+32*2+16,164,getAuxBuffer());
		drawBitmap(game->b5,55+32*2-16,164+32,getAuxBuffer());
	}
	if (game->on_hold == 10) {
		drawBitmap(game->b7,55+32,164+16,getAuxBuffer());
		drawBitmap(game->b7,55+32*2,164+16,getAuxBuffer());
		drawBitmap(game->b7,55+32*3,164+16,getAuxBuffer());
		drawBitmap(game->b7,55+32*2,164-16,getAuxBuffer());
	}
	if (game->on_hold == 11) {
		drawBitmap(game->b4,55+32*2-16,164-16,getAuxBuffer());
		drawBitmap(game->b4,55+32*2+16,164-16,getAuxBuffer());
		drawBitmap(game->b4,55+32*2+16,164+16,getAuxBuffer());
	}
	if (game->on_hold == 12) {
		drawBitmap(game->b3,55+32*1,164,getAuxBuffer());
		drawBitmap(game->b3,55+32*2,164,getAuxBuffer());
		drawBitmap(game->b3,55+32*3,164,getAuxBuffer());
		drawBitmap(game->b3,55+32*2,164-32,getAuxBuffer());
		drawBitmap(game->b3,55+32*2,164+32,getAuxBuffer());
	}

	return 0;
}

int printPoints(tetris* game) {

	int n = 5;
	int points = game->points;
	while (n != 0) {
		int i = points % 10;
		if (i == 0) {
			drawBitmap(game->numbers.number0, 820+27*(n-1), 45, getAuxBuffer());
		}
		if (i == 1) {
			drawBitmap(game->numbers.number1, 820+27*(n-1), 45, getAuxBuffer());
		}
		if (i == 2) {
			drawBitmap(game->numbers.number2, 820+27*(n-1), 45, getAuxBuffer());
		}
		if (i == 3) {
			drawBitmap(game->numbers.number3, 820+27*(n-1), 45, getAuxBuffer());
		}
		if (i == 4) {
			drawBitmap(game->numbers.number4, 820+27*(n-1), 45, getAuxBuffer());
		}
		if (i == 5) {
			drawBitmap(game->numbers.number5, 820+27*(n-1), 45, getAuxBuffer());
		}
		if (i == 6) {
			drawBitmap(game->numbers.number6, 820+27*(n-1), 45, getAuxBuffer());
		}
		if (i == 7) {
			drawBitmap(game->numbers.number7, 820+27*(n-1), 45, getAuxBuffer());
		}
		if (i == 8) {
			drawBitmap(game->numbers.number8, 820+27*(n-1), 45, getAuxBuffer());
		}
		if (i == 9) {
			drawBitmap(game->numbers.number9, 820+27*(n-1), 45, getAuxBuffer());
		}
		points /= 10;
		n--;
	}
	return 0;
}

int updateFrame(tetris* game) {
  resetSecondBuffer();
  drawBitmap(game->background,0,0,getAuxBuffer());
	printPoints(game);
	printNext(game);
	printOnHold(game);
  return 0;
}

bool checkNearColisionLeft(tetris* game, int blocks, int pos[][2]) {
	for (int i = 0; i < blocks; i++) {
		if (pos[i][1] == 0)
			return true;
	}

	for (int i = 0; i < blocks; i++) {
		if (status[pos[i][0]][pos[i][1] - 1] == true) {
			return true;
		}
	}
	return false;
}

bool checkNearColisionRight(tetris* game, int blocks, int pos[][2]) {
	for (int i = 0; i < blocks; i++) {
		if (pos[i][1] == 14)
			return true;
	}

	for (int i = 0; i < blocks; i++) {
		if (status[pos[i][0]][pos[i][1] + 1] == true) {
			return true;
		}
	}
	return false;
}

bool checkNearColisionBottom(tetris* game, int blocks, int pos[][2]) {
	for (int i = 0; i < blocks; i++) {
		if (pos[i][0] + 1 > 21)
			return true;
	}

	for (int i = 0; i < blocks; i++) {
		if (status[pos[i][0] + 1][pos[i][1]] == true) {
			return true;
		}
	}

	return false;
}

int checkColision(tetris* game, int blocks, int pos[][2]) {
	for (int i = 0; i < blocks; i++) {
		if (status[pos[i][0]][pos[i][1]]) {
			for (int j = 0; j < blocks; j++) {
				if (pos[j][1] + 1 > 14) {
					return 1;	// entalado à direita
				}
				if (pos[j][1] - 1 < 0) {
					return 2;	// entalado à esquerda
				}
			}
			return 3;			// colisão com outras peças
		}
	}

	for (int i = 0; i < blocks; i++) {
		if (pos[i][1] > 14)
			return 4;			// colisão com direita
		if (pos[i][1] < 0)
			return 5;			// colisão com esquerda
	}

	for (int i = 0; i < blocks; i++) {
		if (pos[i][0] > 21)
			return 6;			// colisão com o fundo
	}

	return 0;					// nenhuma colisão detetada
}

int tetris_start(tetris* game,  menus_t* menus) {
	fillWithZeros();
	drawBitmap(game->background,0,0,getAuxBuffer());
	doubleBuffering();

	int drop = 0;
	while (drop == 0) {
		printNext(game);
		printOnHold(game);
		/*
		if (dropPiece(game, menus) == 1) {
			break;
		}
		*/
		drop = dropPiece(game,menus);
		if (drop != 0) {
			break;
		}

		if (desintegrateLines(game) == 1)
			printf("points: %d\n", game->points);
		updateFrame(game);
		printBoard(game);
	}

	if (drop == 1) {
		// go to main menu;
		return 1;
	}

	else if (drop == 2) {
		gameOverMenu(game,menus);
		return 2;
	}

	return 0;	// never returns 0 (just here so that the program compiles)
}

int pauseMenu(menus_t* menus) {
	drawBitmap(menus->pauseMenu,0,0,getAuxBuffer());
	doubleBuffering();

	int ipc_status;
  message msg;
  uint16_t r;

	while (true) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();
						if (scancode == ESC_MAKE) {
							scancode = 0;
							return 1; // 1 to exit game
						}
						else if (scancode == ENTER_MAKE) {
							scancode = 0;
							return 0;	// 0 to keep playing
						}
          }
					if (msg.m_notify.interrupts & irq_set_mouse) {
						continue;
					}

	
        break;
      default:
        break; /* no other notifications expected: do nothing */
      }
    } else { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
      }
    }

	return 2;
}

int gameOverMenu(tetris* game, menus_t* menus) {
	drawBitmap(menus->gameOverMenu,0,0,getAuxBuffer());
	int n = 5;
	int points = game->points;
	while (n != 0) {
		int i = points % 10;
		if (i == 0) {
			drawBitmap(game->numbers.number0, 440+27*(n-1), 400, getAuxBuffer());
		}
		if (i == 1) {
			drawBitmap(game->numbers.number1, 440+27*(n-1), 400, getAuxBuffer());
		}
		if (i == 2) {
			drawBitmap(game->numbers.number2, 440+27*(n-1), 400, getAuxBuffer());
		}
		if (i == 3) {
			drawBitmap(game->numbers.number3, 440+27*(n-1), 400, getAuxBuffer());
		}
		if (i == 4) {
			drawBitmap(game->numbers.number4, 440+27*(n-1), 400, getAuxBuffer());
		}
		if (i == 5) {
			drawBitmap(game->numbers.number5, 440+27*(n-1), 400, getAuxBuffer());
		}
		if (i == 6) {
			drawBitmap(game->numbers.number6, 440+27*(n-1), 400, getAuxBuffer());
		}
		if (i == 7) {
			drawBitmap(game->numbers.number7, 440+27*(n-1), 400, getAuxBuffer());
		}
		if (i == 8) {
			drawBitmap(game->numbers.number8, 440+27*(n-1), 400, getAuxBuffer());
		}
		if (i == 9) {
			drawBitmap(game->numbers.number9, 440+27*(n-1), 400, getAuxBuffer());
		}
		points /= 10;
		n--;
	}
	doubleBuffering();

	int ipc_status;
  message msg;
  uint16_t r;

	while (true) {
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();
						if (scancode == ENTER_MAKE) {
							scancode = 0;
							return 0;	// exit
						}
          }
					if (msg.m_notify.interrupts & irq_set_mouse) {
						continue;
					}

	
        break;
      default:
        break; /* no other notifications expected: do nothing */
      }
    } else { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
      }
    }

	return 0;
}
