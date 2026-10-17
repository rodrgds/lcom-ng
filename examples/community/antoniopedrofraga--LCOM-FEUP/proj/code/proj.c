#include "proj.h"

int main(){
	/* Initialize service */
	sef_startup();
	Game game;
	set_mouse(&game);
	set_coordinates(&game);
	printf("\nLoading bitmaps...\n");
	if(load_bitmaps(&game)) return 1;
	vg_init(SUDOKU);
	if((game.irq_set_mouse = mouse_subscribe_int()) == 1){
		return 1;
	}
	if((game.irq_set_timer = timer_subscribe_int()) == 1){
		return 1;
	}
	if((game.irq_set_kb = kb_subscribe_int()) == 1){
		return 1;
	}
	set_rtc();
	if((game.irq_set_rtc = rtc_subscribe_int()) == -1){
		printf("\nIRQ ENABLE FAILED\n");
	}

	if(activate_mouse()==1){
		return 1;
	}

	menu1(&game);
	printf("\n\nExiting..\n\n");
	if(deactivate_mouse()==1){
		return 1;
	}
	if(mouse_unsubscribe_int() == 1){
		return 1;
	}
	rtc_unsubscribe_int();
	timer_unsubscribe_int();
	unsigned long stat;
	sys_inb(OUT_BUF,&stat);
	vg_exit();
	return 0;
}

int load_bitmaps(Game*game){

	//MENU1
	int error = 0;
	if((game->bmps.background = loadBitmap("//home//lcom1415-t6g09-proj//bmp//backgroundmenu.bmp"))==NULL){
		printf("\nBackground bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//backgroundmenu.bmp exist\n");
		error++;
	}
	if((game->bmps.mouse = loadBitmap("//home//lcom1415-t6g09-proj//bmp//mouse.bmp"))==NULL){
		printf("\nMouse bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//mouse.bmp exist\n");
		error++;
	}
	if((game->bmps.play = loadBitmap("//home//lcom1415-t6g09-proj//bmp//play.bmp"))==NULL){
		printf("\nPlay bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//play.bmp exist\n");
		error++;
	}
	if((game->bmps.quit = loadBitmap("//home//lcom1415-t6g09-proj//bmp//quit.bmp"))==NULL){
		printf("\nQuit bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//quit.bmp exist\n");
		error++;
	}
	if((game->bmps.quit_glow = loadBitmap("//home//lcom1415-t6g09-proj//bmp//quitglow.bmp"))==NULL){
		printf("\nQuit GLowing bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//quitglow.bmp exist\n");
		error++;
	}
	if((game->bmps.play_glow= loadBitmap("//home//lcom1415-t6g09-proj//bmp//playglow.bmp"))==NULL){
		printf("\nPlay Glowing bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//playglow.bmp exist\n");
		error++;
	}
	//MENU2
	if((game->bmps.easy = loadBitmap("//home//lcom1415-t6g09-proj//bmp//dificulty//easy.bmp"))==NULL){
		printf("\nEasy bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//dificulty//easy.bmp exist\n");
		error++;
	}
	if((game->bmps.easy_glow = loadBitmap("//home//lcom1415-t6g09-proj//bmp//dificulty//easyglow.bmp"))==NULL){
		printf("\nEasy Glowing bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//dificulty//easyglow.bmp exist\n");
		error++;
	}
	if((game->bmps.medium = loadBitmap("//home//lcom1415-t6g09-proj//bmp//dificulty//medium.bmp"))==NULL){
		printf("\nMedium bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//dificulty//medium.bmp exist\n");
		error++;
	}
	if((game->bmps.medium_glow = loadBitmap("//home//lcom1415-t6g09-proj//bmp//dificulty//mediumglow.bmp"))==NULL){
		printf("\nMedium Glowing bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//dificulty//mediumglow.bmp exist\n");
		error++;
	}
	if((game->bmps.hard = loadBitmap("//home//lcom1415-t6g09-proj//bmp//dificulty//hard.bmp"))==NULL){
		printf("\nHard bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//dificulty//hard.bmp exist\n");
		error++;
	}
	if((game->bmps.hard_glow = loadBitmap("//home//lcom1415-t6g09-proj//bmp//dificulty//hardglow.bmp"))==NULL){
		printf("\nHard Glowing bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//dificulty//hardglow.bmp exist\n");
		error++;
	}
	if((game->bmps.back = loadBitmap("//home//lcom1415-t6g09-proj//bmp//dificulty//back.bmp"))==NULL){
		printf("\nBack bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//dificulty//back.bmp exist\n");
		error++;
	}
	if((game->bmps.back_glow = loadBitmap("//home//lcom1415-t6g09-proj//bmp//dificulty//backglow.bmp"))==NULL){
		printf("\nBack Glowing bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//dificulty//backglow.bmp exist\n");
		error++;
	}
	if((game->bmps.game_background = loadBitmap("//home//lcom1415-t6g09-proj//bmp//board//gamebackground.bmp"))==NULL){
		printf("\nGame Background bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//gamebackground.bmp exist\n");
		error++;
	}
	if((game->bmps.board = loadBitmap("//home//lcom1415-t6g09-proj//bmp//board//board.bmp"))==NULL){
		printf("\nBoard bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//board.bmp exist\n");
		error++;
	}
	if((game->bmps.square = loadBitmap("//home//lcom1415-t6g09-proj//bmp//board//square.bmp"))==NULL){
		printf("\nSquare bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//square.bmp exist\n");
		error++;
	}
	if((game->bmps.zero = loadBitmap("//home//lcom1415-t6g09-proj//bmp//numbers//0.bmp"))==NULL){
		printf("\nZero bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//0.bmp exist\n");
		error++;
	}
	if((game->bmps.one = loadBitmap("//home//lcom1415-t6g09-proj//bmp//numbers//1.bmp"))==NULL){
		printf("\nOne bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//1.bmp exist\n");
		error++;
	}
	if((game->bmps.two = loadBitmap("//home//lcom1415-t6g09-proj//bmp//numbers//2.bmp"))==NULL){
		printf("\nTwo bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//2.bmp exist\n");
		error++;
	}
	if((game->bmps.three = loadBitmap("//home//lcom1415-t6g09-proj//bmp//numbers//3.bmp"))==NULL){
		printf("\nThree bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//3.bmp exist\n");
		error++;
	}
	if((game->bmps.four = loadBitmap("//home//lcom1415-t6g09-proj//bmp//numbers//4.bmp"))==NULL){
		printf("\nFour bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//4.bmp exist\n");
		error++;
	}
	if((game->bmps.five = loadBitmap("//home//lcom1415-t6g09-proj//bmp//numbers//5.bmp"))==NULL){
		printf("\nFive bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//5.bmp exist\n");
		error++;
	}
	if((game->bmps.six = loadBitmap("//home//lcom1415-t6g09-proj//bmp//numbers//6.bmp"))==NULL){
		printf("\nSix bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//6.bmp exist\n");
		error++;
	}
	if((game->bmps.seven = loadBitmap("//home//lcom1415-t6g09-proj//bmp//numbers//7.bmp"))==NULL){
		printf("\nSeven bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//7.bmp exist\n");
		error++;
	}
	if((game->bmps.eight = loadBitmap("//home//lcom1415-t6g09-proj//bmp//numbers//8.bmp"))==NULL){
		printf("\nEight bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//8.bmp exist\n");
		error++;
	}
	if((game->bmps.nine = loadBitmap("//home//lcom1415-t6g09-proj//bmp//numbers//9.bmp"))==NULL){
		printf("\nNine bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//9.bmp exist\n");
		error++;
	}
	if((game->bmps.one_glow = loadBitmap("//home//lcom1415-t6g09-proj//bmp//numbers//1glow.bmp"))==NULL){
		printf("\nOne Glowing bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//1glow.bmp exist\n");
		error++;
	}
	if((game->bmps.two_glow = loadBitmap("//home//lcom1415-t6g09-proj//bmp//numbers//2glow.bmp"))==NULL){
		printf("\nTwo Glowing bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//2glow.bmp exist\n");
		error++;
	}
	if((game->bmps.three_glow = loadBitmap("//home//lcom1415-t6g09-proj//bmp//numbers//3glow.bmp"))==NULL){
		printf("\nThree Glowing bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//3glow.bmp exist\n");
		error++;
	}
	if((game->bmps.four_glow = loadBitmap("//home//lcom1415-t6g09-proj//bmp//numbers//4glow.bmp"))==NULL){
		printf("\nFour Glowing bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//4glow.bmp exist\n");
		error++;
	}
	if((game->bmps.five_glow = loadBitmap("//home//lcom1415-t6g09-proj//bmp//numbers//5glow.bmp"))==NULL){
		printf("\nFive Glowing bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//5glow.bmp exist\n");
		error++;
	}
	if((game->bmps.six_glow = loadBitmap("//home//lcom1415-t6g09-proj//bmp//numbers//6glow.bmp"))==NULL){
		printf("\nSix Glowing bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//6glow.bmp exist\n");
		error++;
	}
	if((game->bmps.seven_glow = loadBitmap("//home//lcom1415-t6g09-proj//bmp//numbers//7glow.bmp"))==NULL){
		printf("\nSeven Glowing bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//7glow.bmp exist\n");
		error++;
	}
	if((game->bmps.eight_glow = loadBitmap("//home//lcom1415-t6g09-proj//bmp//numbers//8glow.bmp"))==NULL){
		printf("\nEight Glowing bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//8glow.bmp exist\n");
		error++;
	}
	if((game->bmps.nine_glow = loadBitmap("//home//lcom1415-t6g09-proj//bmp//numbers//9glow.bmp"))==NULL){
		printf("\nNine Glowing bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//9glow.bmp exist\n");
		error++;
	}
	if((game->bmps.dots = loadBitmap("//home//lcom1415-t6g09-proj//bmp//numbers//dots.bmp"))==NULL){
		printf("\nDots Glowing bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//dots.bmp exist\n");
		error++;
	}
	if((game->bmps.time = loadBitmap("//home//lcom1415-t6g09-proj//bmp//time.bmp"))==NULL){
		printf("\nTime bitmap couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//time.bmp exist\n");
		error++;
	}
	if((game->bmps.winmenu = loadBitmap("//home//lcom1415-t6g09-proj//bmp//winmenu.bmp"))==NULL){
		printf("\nWinner Menu couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//board//winmenu.bmp exist\n");
		error++;
	}
	if((game->bmps.slash = loadBitmap("//home//lcom1415-t6g09-proj//bmp//slash.bmp"))==NULL){
		printf("\nSlash couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//slash.bmp exist\n");
		error++;
	}
	if((game->bmps.highscores = loadBitmap("//home//lcom1415-t6g09-proj//bmp//highscores.bmp"))==NULL){
		printf("\nHigh Scores couldn't be loaded or does not exist\n"
				"Check if //home//lcom1415-t6g09-proj//bmp//highscores.bmp exist\n");
		error++;
	}
	return error;
}

void set_mouse(Game*game){
	game->mouse.x = 100;
	game->mouse.y = 100;
	game->mouse.h = 51;
	game->mouse.w = 64;
}

void set_coordinates(Game*game){
	//First Square
	game->sudoku.board[0][0][0].x = 169;
	game->sudoku.board[0][0][0].y = 202;
	game->sudoku.board[0][0][0].value = 0;

	game->sudoku.board[1][0][0].x = 208;
	game->sudoku.board[1][0][0].y = 202;
	game->sudoku.board[1][0][0].value = 0;

	game->sudoku.board[2][0][0].x = 246;
	game->sudoku.board[2][0][0].y = 202;
	game->sudoku.board[2][0][0].value = 0;

	game->sudoku.board[0][1][0].x = 169;
	game->sudoku.board[0][1][0].y = 239;
	game->sudoku.board[0][1][0].value = 0;

	game->sudoku.board[1][1][0].x = 208;
	game->sudoku.board[1][1][0].y = 239;
	game->sudoku.board[1][1][0].value = 0;

	game->sudoku.board[2][1][0].x = 246;
	game->sudoku.board[2][1][0].y = 239;
	game->sudoku.board[2][1][0].value = 0;

	game->sudoku.board[0][2][0].x = 169;
	game->sudoku.board[0][2][0].y = 279;
	game->sudoku.board[0][2][0].value = 0;

	game->sudoku.board[1][2][0].x = 208;
	game->sudoku.board[1][2][0].y = 278;
	game->sudoku.board[1][2][0].value = 0;

	game->sudoku.board[2][2][0].x = 246;
	game->sudoku.board[2][2][0].y = 278;
	game->sudoku.board[2][2][0].value = 0;

	//Second square
	game->sudoku.board[0][0][1].x = 292;
	game->sudoku.board[0][0][1].y = 202;
	game->sudoku.board[0][0][1].value = 0;

	game->sudoku.board[1][0][1].x = 331;
	game->sudoku.board[1][0][1].y = 202;
	game->sudoku.board[1][0][1].value = 0;

	game->sudoku.board[2][0][1].x = 369;
	game->sudoku.board[2][0][1].y = 202;
	game->sudoku.board[2][0][1].value = 0;

	game->sudoku.board[0][1][1].x = 292;
	game->sudoku.board[0][1][1].y = 239;
	game->sudoku.board[0][1][1].value = 0;

	game->sudoku.board[1][1][1].x = 331;
	game->sudoku.board[1][1][1].y = 239;
	game->sudoku.board[1][1][1].value = 0;

	game->sudoku.board[2][1][1].x = 369;
	game->sudoku.board[2][1][1].y = 239;
	game->sudoku.board[2][1][1].value = 0;

	game->sudoku.board[0][2][1].x = 292;
	game->sudoku.board[0][2][1].y = 278;
	game->sudoku.board[0][2][1].value = 0;

	game->sudoku.board[1][2][1].x = 331;
	game->sudoku.board[1][2][1].y = 278;
	game->sudoku.board[1][2][1].value = 0;

	game->sudoku.board[2][2][1].x = 369;
	game->sudoku.board[2][2][1].y = 278;
	game->sudoku.board[2][2][1].value = 0;

	//Third square
	game->sudoku.board[0][0][2].x = 415;
	game->sudoku.board[0][0][2].y = 202;
	game->sudoku.board[0][0][2].value = 0;

	game->sudoku.board[1][0][2].x = 454;
	game->sudoku.board[1][0][2].y = 202;
	game->sudoku.board[1][0][2].value = 0;

	game->sudoku.board[2][0][2].x = 492;
	game->sudoku.board[2][0][2].y = 202;
	game->sudoku.board[2][0][2].value = 0;

	game->sudoku.board[0][1][2].x = 415;
	game->sudoku.board[0][1][2].y = 239;
	game->sudoku.board[0][1][2].value = 0;

	game->sudoku.board[1][1][2].x = 454;
	game->sudoku.board[1][1][2].y = 239;
	game->sudoku.board[1][1][2].value = 0;

	game->sudoku.board[2][1][2].x = 492;
	game->sudoku.board[2][1][2].y = 239;
	game->sudoku.board[2][1][2].value = 0;

	game->sudoku.board[0][2][2].x = 415;
	game->sudoku.board[0][2][2].y = 278;
	game->sudoku.board[0][2][2].value = 0;

	game->sudoku.board[1][2][2].x = 454;
	game->sudoku.board[1][2][2].y = 278;
	game->sudoku.board[1][2][2].value = 0;

	game->sudoku.board[2][2][2].x = 492;
	game->sudoku.board[2][2][2].y = 278;
	game->sudoku.board[2][2][2].value = 0;

	//Fourth Square
	game->sudoku.board[0][0][3].x = 169;
	game->sudoku.board[0][0][3].y = 322;
	game->sudoku.board[0][0][3].value = 0;

	game->sudoku.board[1][0][3].x = 208;
	game->sudoku.board[1][0][3].y = 322;
	game->sudoku.board[1][0][3].value = 0;

	game->sudoku.board[2][0][3].x = 246;
	game->sudoku.board[2][0][3].y = 322;
	game->sudoku.board[2][0][3].value = 0;

	game->sudoku.board[0][1][3].x = 169;
	game->sudoku.board[0][1][3].y = 360;
	game->sudoku.board[0][1][3].value = 0;

	game->sudoku.board[1][1][3].x = 208;
	game->sudoku.board[1][1][3].y = 360;
	game->sudoku.board[1][1][3].value = 0;

	game->sudoku.board[2][1][3].x = 246;
	game->sudoku.board[2][1][3].y = 360;
	game->sudoku.board[2][1][3].value = 0;

	game->sudoku.board[0][2][3].x = 169;
	game->sudoku.board[0][2][3].y = 399;
	game->sudoku.board[0][2][3].value = 0;

	game->sudoku.board[1][2][3].x = 208;
	game->sudoku.board[1][2][3].y = 399;
	game->sudoku.board[1][2][3].value = 0;

	game->sudoku.board[2][2][3].x = 246;
	game->sudoku.board[2][2][3].y = 399;
	game->sudoku.board[2][2][3].value = 0;

	//Fifth square
	game->sudoku.board[0][0][4].x = 292;
	game->sudoku.board[0][0][4].y = 322;
	game->sudoku.board[0][0][4].value = 0;

	game->sudoku.board[1][0][4].x = 331;
	game->sudoku.board[1][0][4].y = 322;
	game->sudoku.board[1][0][4].value = 0;

	game->sudoku.board[2][0][4].x = 369;
	game->sudoku.board[2][0][4].y = 322;
	game->sudoku.board[2][0][4].value = 0;

	game->sudoku.board[0][1][4].x = 292;
	game->sudoku.board[0][1][4].y = 360;
	game->sudoku.board[0][1][4].value = 0;

	game->sudoku.board[1][1][4].x = 331;
	game->sudoku.board[1][1][4].y = 360;
	game->sudoku.board[1][1][4].value = 0;

	game->sudoku.board[2][1][4].x = 369;
	game->sudoku.board[2][1][4].y = 360;
	game->sudoku.board[2][1][4].value = 0;

	game->sudoku.board[0][2][4].x = 292;
	game->sudoku.board[0][2][4].y = 399;
	game->sudoku.board[0][2][4].value = 0;

	game->sudoku.board[1][2][4].x = 331;
	game->sudoku.board[1][2][4].y = 399;
	game->sudoku.board[1][2][4].value = 0;

	game->sudoku.board[2][2][4].x = 369;
	game->sudoku.board[2][2][4].y = 399;
	game->sudoku.board[2][2][4].value = 0;

	//Sixth square
	game->sudoku.board[0][0][5].x = 415;
	game->sudoku.board[0][0][5].y = 322;
	game->sudoku.board[0][0][5].value = 0;

	game->sudoku.board[1][0][5].x = 454;
	game->sudoku.board[1][0][5].y = 322;
	game->sudoku.board[1][0][5].value = 0;

	game->sudoku.board[2][0][5].x = 492;
	game->sudoku.board[2][0][5].y = 322;
	game->sudoku.board[2][0][5].value = 0;

	game->sudoku.board[0][1][5].x = 415;
	game->sudoku.board[0][1][5].y = 360;
	game->sudoku.board[0][1][5].value = 0;

	game->sudoku.board[1][1][5].x = 454;
	game->sudoku.board[1][1][5].y = 360;
	game->sudoku.board[1][1][5].value = 0;

	game->sudoku.board[2][1][5].x = 492;
	game->sudoku.board[2][1][5].y = 360;
	game->sudoku.board[2][1][5].value = 0;

	game->sudoku.board[0][2][5].x = 415;
	game->sudoku.board[0][2][5].y = 399;
	game->sudoku.board[0][2][5].value = 0;

	game->sudoku.board[1][2][5].x = 454;
	game->sudoku.board[1][2][5].y = 399;
	game->sudoku.board[1][2][5].value = 0;

	game->sudoku.board[2][2][5].x = 492;
	game->sudoku.board[2][2][5].y = 399;
	game->sudoku.board[2][2][5].value = 0;

	//Seventh square
	game->sudoku.board[0][0][6].x = 169;
	game->sudoku.board[0][0][6].y = 443;
	game->sudoku.board[0][0][6].value = 0;

	game->sudoku.board[1][0][6].x = 208;
	game->sudoku.board[1][0][6].y = 443;
	game->sudoku.board[1][0][6].value = 0;

	game->sudoku.board[2][0][6].x = 246;
	game->sudoku.board[2][0][6].y = 443;
	game->sudoku.board[2][0][6].value = 0;

	game->sudoku.board[0][1][6].x = 169;
	game->sudoku.board[0][1][6].y = 481;
	game->sudoku.board[0][1][6].value = 0;

	game->sudoku.board[1][1][6].x = 208;
	game->sudoku.board[1][1][6].y = 481;
	game->sudoku.board[1][1][6].value = 0;

	game->sudoku.board[2][1][6].x = 246;
	game->sudoku.board[2][1][6].y = 481;
	game->sudoku.board[2][1][6].value = 0;

	game->sudoku.board[0][2][6].x = 169;
	game->sudoku.board[0][2][6].y = 520;
	game->sudoku.board[0][2][6].value = 0;

	game->sudoku.board[1][2][6].x = 208;
	game->sudoku.board[1][2][6].y = 520;
	game->sudoku.board[1][2][6].value = 0;

	game->sudoku.board[2][2][6].x = 246;
	game->sudoku.board[2][2][6].y = 520;
	game->sudoku.board[2][2][6].value = 0;

	//Eighth square
	game->sudoku.board[0][0][7].x = 292;
	game->sudoku.board[0][0][7].y = 443;
	game->sudoku.board[0][0][7].value = 0;

	game->sudoku.board[1][0][7].x = 331;
	game->sudoku.board[1][0][7].y = 443;
	game->sudoku.board[1][0][7].value = 0;

	game->sudoku.board[2][0][7].x = 369;
	game->sudoku.board[2][0][7].y = 443;
	game->sudoku.board[2][0][7].value = 0;

	game->sudoku.board[0][1][7].x = 292;
	game->sudoku.board[0][1][7].y = 481;
	game->sudoku.board[0][1][7].value = 0;

	game->sudoku.board[1][1][7].x = 331;
	game->sudoku.board[1][1][7].y = 481;
	game->sudoku.board[1][1][7].value = 0;

	game->sudoku.board[2][1][7].x = 369;
	game->sudoku.board[2][1][7].y = 481;
	game->sudoku.board[2][1][7].value = 0;

	game->sudoku.board[0][2][7].x = 292;
	game->sudoku.board[0][2][7].y = 520;
	game->sudoku.board[0][2][7].value = 0;

	game->sudoku.board[1][2][7].x = 331;
	game->sudoku.board[1][2][7].y = 520;
	game->sudoku.board[1][2][7].value = 0;

	game->sudoku.board[2][2][7].x = 369;
	game->sudoku.board[2][2][7].y = 520;
	game->sudoku.board[2][2][7].value = 0;

	//Ninth square

	game->sudoku.board[0][0][8].x = 415;
	game->sudoku.board[0][0][8].y = 443;
	game->sudoku.board[0][0][8].value = 0;

	game->sudoku.board[1][0][8].x = 454;
	game->sudoku.board[1][0][8].y = 443;
	game->sudoku.board[1][0][8].value = 0;

	game->sudoku.board[2][0][8].x = 492;
	game->sudoku.board[2][0][8].y = 443;
	game->sudoku.board[2][0][8].value = 0;

	game->sudoku.board[0][1][8].x = 415;
	game->sudoku.board[0][1][8].y = 481;
	game->sudoku.board[0][1][8].value = 0;

	game->sudoku.board[1][1][8].x = 454;
	game->sudoku.board[1][1][8].y = 481;
	game->sudoku.board[1][1][8].value = 0;

	game->sudoku.board[2][1][8].x = 492;
	game->sudoku.board[2][1][8].y = 481;
	game->sudoku.board[2][1][8].value = 0;

	game->sudoku.board[0][2][8].x = 415;
	game->sudoku.board[0][2][8].y = 520;
	game->sudoku.board[0][2][8].value = 0;

	game->sudoku.board[1][2][8].x = 454;
	game->sudoku.board[1][2][8].y = 520;
	game->sudoku.board[1][2][8].value = 0;

	game->sudoku.board[2][2][8].x = 492;
	game->sudoku.board[2][2][8].y = 520;
	game->sudoku.board[2][2][8].value = 0;

	int squares;
	int x;
	int y;
	int choice;
	for(squares = 0; squares < 9; squares++){
		for(x = 0; x < 3; x++){
			for(y = 0; y < 3; y++){
				game->sudoku.board[x][y][squares].hovered = 1;
			}
		}
	}
}



