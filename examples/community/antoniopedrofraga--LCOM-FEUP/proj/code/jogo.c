#include "jogo.h"

int mouse_hover(Bitmap*bmp,Bitmap*bmp2,Mouse mouse, int x, int y){ //checks if mouse is hovering it
	if(mouse.x >= x && (mouse.x <= x + bmp->bitmapInfoHeader.width) && mouse.y >= y && (mouse.y <= y + bmp->bitmapInfoHeader.height)){
		if((drawBitmap_byPixel(bmp2, x, y, ALIGN_LEFT)) != 0)
			return -1;
		if(mouse.lb){
			return 1;
		}else{
			return 0;
		}
	}else {
		return 0;
	}
}

int mouse_hover_board(Bitmap*bmp,Mouse mouse, int x, int y,int hovered){ //checks if mouse is hovering board squares
	if(hovered){
	if(mouse.x >= x && (mouse.x <= x + bmp->bitmapInfoHeader.width) && mouse.y >= y && (mouse.y <= y + bmp->bitmapInfoHeader.height)){
		if((drawBitmap_byPixel(bmp, x, y, ALIGN_LEFT)) != 0)
			return -1;
		if(mouse.lb){
			return 1;
		}else{
			return 0;
		}
	}else {
		return 0;
	}
	}
	return 0;
}

int draw_mouse(Game*game, int array[3]){
	/* A clock repaint has no new complete mouse packet. */
	if(array){
		if(array[0] & BIT(0)) game->mouse.lb = 1;
		else game->mouse.lb = 0;
		int dx = (array[1] & 255) - ((array[0] & BIT(4)) ? 256 : 0);
		int dy = (array[2] & 255) - ((array[0] & BIT(5)) ? 256 : 0);
		game->mouse.x += 2*dx;
		game->mouse.y -= 2*dy;
	}
	game->mouse.x_max = game->mouse.x + game->mouse.w;
	game->mouse.y_max = game->mouse.y + game->mouse.h;
	if(game->mouse.x < 0) game->mouse.x = 0;
	if(game->mouse.y < 0) game->mouse.y = 0;
	game->mouse.x_max = game->mouse.x + game->mouse.w;
	game->mouse.y_max = game->mouse.y + game->mouse.h;
	if(game->mouse.x_max > 1023){
		game->mouse.x = game->mouse.x + (1023 - game->mouse.x_max);
		game->mouse.x_max = 1023;
	}
	if(game->mouse.y_max > 767){
		game->mouse.y = game->mouse.y + (767 - game->mouse.y_max);
		game->mouse.y_max = 767;
	}
	if((drawBitmap_byPixel(game->bmps.mouse, game->mouse.x, game->mouse.y, ALIGN_LEFT)) != 0){
		return 1;
	}
}

int update_highScores(Game*game){
	int a[3] = {0,0,0};
	FILE *file;
	file = fopen("//home//lcom1415-t6g09-proj//sudoku//highscores.txt", "r");
	if(file != NULL){
		int it = 0;
		while(it < 3 && fscanf(file,"%d",&a[it]) == 1){
			it++;
		}
	}
	int time = game->sudoku.time;
	if(a[0] > time || a[0] == 0){
		a[2] = a[1];
		a[1] = a[0];
		a[0] = time;
	}else if((a[1] > time && a[0] < time) || a[1] == 0){
		a[2] = a[1];
		a[1] = time;
	}else if((a[1] < time && a[2] > time) || a[2] == 0){
		a[2] = time;
	}
	file = fopen("//home//lcom1415-t6g09-proj//sudoku//highscores.txt", "w");
	if(file == NULL) return 0;
	int it2 = 0;
	for(it2; it2<3;it2++){
		if(a[it2]!=0){
		fprintf(file, "%d\n", a[it2]);
		}
	}
	fclose(file);
	return 0;
}

int listen_kb(Game*game,int x,int y,int squares,unsigned char**double_buffer){
	int ipc_status;
	unsigned short key = 0;
	message msg;
	long packets;
	while(key != ESC) {
		int r = driver_receive(ANY, &msg, &ipc_status);
		if ( r != 0 ) {
			printf("dri\ver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { //Notified
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & game->irq_set_timer){ //timer notification
					game->sudoku.time++;
					if(game->sudoku.time%60==0){
					display_time(game,double_buffer); //keeps displaying time while square is selected
					draw_background(*double_buffer);
					mouse_hover_board(game->bmps.square, //keeps square selected (user friendly)
							game->mouse,
							game->sudoku.board[x][y][squares].x,
							game->sudoku.board[x][y][squares].y,
							game->sudoku.board[x][y][squares].hovered);
					}
				}
				if (msg.NOTIFY_ARG & game->irq_set_kb) {
					key = get_kb();
					if(key == ONE){
						return 1;
					}else if(key == TWO){
						return 2;
					}else if(key == THREE){
						return 3;
					}else if(key == FOUR){
						return 4;
					}else if(key == FIVE){
						return 5;
					}else if(key == SIX){
						return 6;
					}else if(key == SEVEN){
						return 7;
					}else if(key == EIGHT){
						return 8;
					}else if(key == NINE){
						return 9;
					}else if(key == BACKSPACE){
						return 0;
					}

				}
				if (msg.NOTIFY_ARG & game->irq_set_mouse) {
					packets = get_packets(); //Get mouse packets to avoid crashes
				}
				break;
			default:
				break;
			}
		} else { }
	}
	return -1;
}

void draw_background(unsigned char * double_buffer){
	char* bufferStartPos = vg_adress();
	memcpy(bufferStartPos, double_buffer,get_v_res()*get_h_res()*2);
}

int draw_on_board(Game*game,int x, int y, int choice,unsigned char **double_buffer, int correct){
	if(correct == 1){
		if(choice ==1){
			if((drawBitmap_onBoard(game->bmps.one, x, y, ALIGN_LEFT,double_buffer)) != 0)
				return -1;
		}else if(choice == 2){
			if((drawBitmap_onBoard(game->bmps.two, x, y, ALIGN_LEFT,double_buffer)) != 0)
				return -1;
		}else if(choice == 3){
			if((drawBitmap_onBoard(game->bmps.three, x, y, ALIGN_LEFT,double_buffer)) != 0)
				return -1;
		}else if(choice == 4){
			if((drawBitmap_onBoard(game->bmps.four, x, y, ALIGN_LEFT,double_buffer)) != 0)
				return -1;
		}else if(choice == 5){
			if((drawBitmap_onBoard(game->bmps.five, x, y, ALIGN_LEFT,double_buffer)) != 0)
				return -1;
		}else if(choice == 6){
			if((drawBitmap_onBoard(game->bmps.six, x, y, ALIGN_LEFT,double_buffer)) != 0)
				return -1;
		}else if(choice == 7){
			if((drawBitmap_onBoard(game->bmps.seven, x, y, ALIGN_LEFT,double_buffer)) != 0)
				return -1;
		}else if(choice == 8){
			if((drawBitmap_onBoard(game->bmps.eight, x, y, ALIGN_LEFT,double_buffer)) != 0)
				return -1;
		}else if(choice == 9){
			if((drawBitmap_onBoard(game->bmps.nine, x, y, ALIGN_LEFT,double_buffer)) != 0)
				return -1;
		}
	}else if (correct == 0){
		if(choice ==1){
			if((drawBitmap_onBoard(game->bmps.one_glow, x, y, ALIGN_LEFT,double_buffer)) != 0)
				return -1;
		}else if(choice == 2){
			if((drawBitmap_onBoard(game->bmps.two_glow, x, y, ALIGN_LEFT,double_buffer)) != 0)
				return -1;
		}else if(choice == 3){
			if((drawBitmap_onBoard(game->bmps.three_glow, x, y, ALIGN_LEFT,double_buffer)) != 0)
				return -1;
		}else if(choice == 4){
			if((drawBitmap_onBoard(game->bmps.four_glow, x, y, ALIGN_LEFT,double_buffer)) != 0)
				return -1;
		}else if(choice == 5){
			if((drawBitmap_onBoard(game->bmps.five_glow, x, y, ALIGN_LEFT,double_buffer)) != 0)
				return -1;
		}else if(choice == 6){
			if((drawBitmap_onBoard(game->bmps.six_glow, x, y, ALIGN_LEFT,double_buffer)) != 0)
				return -1;
		}else if(choice == 7){
			if((drawBitmap_onBoard(game->bmps.seven_glow, x, y, ALIGN_LEFT,double_buffer)) != 0)
				return -1;
		}else if(choice == 8){
			if((drawBitmap_onBoard(game->bmps.eight_glow, x, y, ALIGN_LEFT,double_buffer)) != 0)
				return -1;
		}else if(choice == 9){
			if((drawBitmap_onBoard(game->bmps.nine_glow, x, y, ALIGN_LEFT,double_buffer)) != 0)
				return -1;
		}
	}
	return 0;
}

int hover_squares(Game*game,unsigned char**double_buffer){
	int squares;
	int x;
	int y;
	int choice;
	for(squares = 0; squares < 9; squares++){
		for(x = 0; x < 3; x++){
			for(y = 0; y < 3; y++){
				if(mouse_hover_board(game->bmps.square,game->mouse,game->sudoku.board[x][y][squares].x,game->sudoku.board[x][y][squares].y,game->sudoku.board[x][y][squares].hovered)==1){ //[0][0][0]
					if((choice = listen_kb(game,x,y,squares,double_buffer))!=-1){
						game->sudoku.board[x][y][squares].value = choice;
					}
					return 1;
				}
			}
		}
	}

	return 0;
}

Bitmap*get_bitmap(Game*game,int choice){
	if(choice ==1){
		return game->bmps.one;
	}else if(choice == 2){
		return game->bmps.two;
	}else if(choice == 3){
		return game->bmps.three;
	}else if(choice == 4){
		return game->bmps.four;
	}else if(choice == 5){
		return game->bmps.five;
	}else if(choice == 6){
		return game->bmps.six;
	}else if(choice == 7){
		return game->bmps.seven;
	}else if(choice == 8){
		return game->bmps.eight;
	}else if(choice == 9){
		return game->bmps.nine;
	}else{
		return game->bmps.zero;
	}
}

int display_number(Game*game,int number, int x, int y,unsigned char**double_buffer){ //displays a two algarism number on pixels (x,y)
	Bitmap*number1;
	Bitmap*number2;
	int num;

	if(number < 10){
		number1 = game->bmps.zero;
		number2 = get_bitmap(game,number);
	}else{
		number1 = get_bitmap(game,number/10);
		number2 = get_bitmap(game,number%10);
	}
	erase_square(game->bmps.one,x,y,ALIGN_LEFT,double_buffer,game->sudoku.background);
	erase_square(game->bmps.one,x + 36,y,ALIGN_LEFT,double_buffer,game->sudoku.background);
	if((drawBitmap_onBoard(number1, x, y, ALIGN_LEFT,double_buffer)) != 0)
					return -1;
	if((drawBitmap_onBoard(number2, x+36, y, ALIGN_LEFT,double_buffer)) != 0)
					return -1;
	return 0;
}

void display_time(Game*game,unsigned char**double_buffer){
	int minutes = 0;
	int seconds;
	int temp = game->sudoku.time;
	while(temp >= 60 * 60){
		temp -= 60*60;
		minutes++;
	}
	seconds = temp / 60;
	display_number(game,minutes,700,200,double_buffer);
	display_number(game,seconds,775,200,double_buffer);
}



int load_sudoku(Game*game,int difficulty){
	srand(time(NULL)); // to get random numbers
	const char *dif;
	int r = rand() % 3 + 1;
	if(difficulty == 1) dif = "easy";
	else if(difficulty == 2) dif = "medi";
	else if(difficulty == 3) dif = "hard";
	else return 1;
	char dir[64];
	int length = snprintf(dir, sizeof(dir), "//home//lcom1415-t6g09-proj//sudoku//%s//%d.txt", dif, r);
	if(length < 0 || (size_t)length >= sizeof(dir)) return 1;
	printf("Dir: %s\n",dir);
	FILE *file;
	file = fopen(dir, "r");
	if(file != NULL){
	int x, y, squares, value, fields;
	while((fields = fscanf(file,"%d %d %d %d",&value,&x,&y,&squares)) == 4){
		if(x < 0 || x >= 3 || y < 0 || y >= 3 || squares < 0 || squares >= 9 || value < 0 || value > 9){
			fclose(file);
			return 1;
		}
		game->sudoku.board[x][y][squares].value = value;
		game->sudoku.board[x][y][squares].hovered = 0;
	}
	if(fields != EOF || ferror(file)){
		fclose(file);
		return 1;
	}
	fclose(file);
	return 0;
	}else{
		printf("\nNot opening\n");
		return 1;
	}
	return 0;
}

void clean_board(Game*game){
	int squares;
	int x;
	int y;
	for(squares = 0; squares < 9; squares++){
		for(x = 0; x < 3; x++){
			for(y = 0; y < 3; y++){
				game->sudoku.board[x][y][squares].value = 0;
				game->sudoku.board[x][y][squares].hovered = 1;
			}
		}
	}
}

int draw_game(Game*game,int array[3], unsigned char*double_buffer){

	draw_background(double_buffer);

	if(array && hover_squares(game,&double_buffer)==1){
		if(!check_game(game,&double_buffer)) return 2;
		draw_background(double_buffer);
	}

	if(array && mouse_hover(game->bmps.back,game->bmps.back_glow,game->mouse,800,600)==1){
		return 1;
	}

	draw_mouse(game,array);
	return 0;

}

int winner_menu(Game*game){
	//Variable declaration
	int array[3] = {0,0,0};
	int array_pos = 0;
	int counter_timer = 0;
	int ipc_status;
	unsigned short key = 0;
	message msg;

	//Drawing winner menu background...

	if((drawBitmap_byLine(game->bmps.winmenu, 0, 0, ALIGN_LEFT)) != 0){
		return 1;
	}
	if((drawBitmap_byPixel(game->bmps.back, 800, 600, ALIGN_LEFT)) != 0)
		return 1;

	unsigned char * double_buffer = double_buffering();

	draw_mouse(game,array);

	while(key != ESC) {
		int r = driver_receive(ANY, &msg, &ipc_status);
		if ( r != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { //Notified
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & game->irq_set_timer){ //timer notification
					counter_timer++;
				}
				if (msg.NOTIFY_ARG & game->irq_set_mouse) {//mouse notification
					array[array_pos] = get_packets();
					if((array[0] & BIT(3)) != 0){
						if(array_pos !=2){
							array_pos++;
						}else{

							array_pos = 0;

							draw_background(double_buffer);
							if(mouse_hover(game->bmps.back,game->bmps.back_glow,game->mouse, 800, 600)==1){
								key = ESC;
							}
							draw_mouse(game,array);

						}
					}else{
						printf("\nnot a valid first byte\n");
					}
				}
				if (msg.NOTIFY_ARG & game->irq_set_kb) {
					key = get_kb();
				}

				break;
			default:
				break;
			}
		}
	}
	return 0;
}

int menu3(Game*game,int difficulty){
	//Variable declaration
	game->sudoku.time = 0;
	int array[3] = {0,0,0}; //initialize array to 0 to draw_mouse() function first run
	int array_pos = 0;
	int ipc_status;
	unsigned short key = 0;
	message msg;
	//---------------------
	if(load_sudoku(game,difficulty)==1){
		return 1;
	}
	//Drawing game environment
	if((drawBitmap_byLine(game->bmps.game_background, 0, 0, ALIGN_LEFT)) != 0){
		return 1;
	}
	if((drawBitmap_byPixel(game->bmps.board, 50, 50, ALIGN_LEFT)) != 0)
		return -1;
	if((drawBitmap_byPixel(game->bmps.back, 800, 600, ALIGN_LEFT)) != 0)
		return -1;

	if((drawBitmap_byPixel(game->bmps.time, 700, 125, ALIGN_LEFT)) != 0)
		return -1;

	if((drawBitmap_byPixel(game->bmps.dots, 755, 200, ALIGN_LEFT)) != 0)
		return -1;

	unsigned char * double_buffer = double_buffering();//saving video memory for performance purposes
	unsigned char * background2 = double_buffering();
	game->sudoku.background = double_buffering();

	check_game(game,&double_buffer);

	display_time(game,&double_buffer);
	//ALL SET

	draw_background(double_buffer);

	draw_mouse(game,array);

	while(key != ESC) {
		int r = driver_receive(ANY, &msg, &ipc_status);
		if ( r != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { //Notified
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & game->irq_set_timer){ //timer notification
					game->sudoku.time++;
					if(game->sudoku.time % 60 == 0){
						display_time(game,&double_buffer);
						if(draw_game(game,NULL,double_buffer)==1) key = ESC;
					}
				}
				if (msg.NOTIFY_ARG & game->irq_set_mouse) {//mouse notification
					array[array_pos] = get_packets();
					if((array[0] & BIT(3)) != 0){
						if(array_pos !=2){
							array_pos++;
						}else{
							array_pos = 0;
							int status = draw_game(game,array,double_buffer);
							if(status==1) key = ESC;
							else if(status==2){
								update_highScores(game);
								winner_menu(game);
								key = ESC;
							}
							int pos = 2;
							for(pos; pos != -1; pos--){
								array[pos] = 0;
							}
						}
					}else{
						printf("\nnot a valid first byte\n");
					}
				}
				if (msg.NOTIFY_ARG & game->irq_set_kb) {
					key = get_kb();
				}

				break;
			default:
				break;
			}
		}
	}
	clean_board(game);
	return 0;
}

int menu2(Game*game){
	//Variable declaration
	int counter_timer = 0;
	int array[3] = {0,0,0}; //initialize array to 0 to draw_mouse() function first run
	int array_pos = 0;
	int ipc_status;
	unsigned short key = 0;
	message msg;
	//---------------------

	//Drawing second menu background...
	if((drawBitmap_byLine(game->bmps.background, 0, 0, ALIGN_LEFT)) != 0){
		return 1;
	}
	if((drawBitmap_byPixel(game->bmps.easy, 200, 300, ALIGN_LEFT)) != 0)
		return -1;
	if((drawBitmap_byPixel(game->bmps.medium, 200, 400, ALIGN_LEFT)) != 0)
		return -1;
	if((drawBitmap_byPixel(game->bmps.hard, 200, 500, ALIGN_LEFT)) != 0)
			return -1;
	if((drawBitmap_byPixel(game->bmps.back, 200, 600, ALIGN_LEFT)) != 0)
			return -1;
	unsigned char * double_buffer = double_buffering(); //saving video memory for performance purposes
	draw_mouse(game,array);

	while(key != ESC) {
		int r = driver_receive(ANY, &msg, &ipc_status);
		if ( r != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { //Notified
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & game->irq_set_timer){ //timer notification
					counter_timer++;
				}
				if (msg.NOTIFY_ARG & game->irq_set_mouse) {//mouse notification
					array[array_pos] = get_packets();
					if((array[0] & BIT(3)) != 0){
						if(array_pos !=2){
							array_pos++;
						}else{
							array_pos = 0;
							draw_background(double_buffer);
							if(mouse_hover(game->bmps.easy,game->bmps.easy_glow,game->mouse, 200, 300)==1){
								menu3(game,1);
								draw_background(double_buffer);
							}
							if(mouse_hover(game->bmps.medium,game->bmps.medium_glow,game->mouse,200,400)==1){
								menu3(game,2);
								draw_background(double_buffer);
							}
							if(mouse_hover(game->bmps.hard,game->bmps.hard_glow,game->mouse,200,500) == 1){
								menu3(game,3);
								draw_background(double_buffer);
							}
							if(mouse_hover(game->bmps.back,game->bmps.back_glow,game->mouse,200,600)==1) key = ESC;
							draw_mouse(game,array);

						}
					}else{
						printf("\nnot a valid first byte\n");
					}
				}
				if (msg.NOTIFY_ARG & game->irq_set_kb) {
					key = get_kb();
				}

				break;
			default:
				break;
			}
		}
	}
	return 0;

}

int display_date(Game*game,unsigned char*double_buffer){
	clean_buffer();
	display_number(game,get_day(), 50, 700,&double_buffer);
	display_number(game,get_month(), 125, 700,&double_buffer);
	display_number(game,get_year(), 200, 700,&double_buffer);
	display_number(game,get_hour(), 350, 700,&double_buffer);
	display_number(game,get_minutes(), 425, 700,&double_buffer);
	return 0;
}

int display_hs(Game*game,unsigned char ** double_buffer){
	int a[3] = {0,0,0};
	FILE *file;
	file = fopen("//home//lcom1415-t6g09-proj//sudoku//highscores.txt", "r");
	if(file != NULL){
		char it = 0;
		while(it < 3 && fscanf(file,"%d",&a[it]) == 1){
			it++;
		}
	}else{
		file = fopen("//home//lcom1415-t6g09-proj//sudoku//highscores.txt", "w");
	}

	if(a[0] != 0){
		drawBitmap_onBoard(game->bmps.highscores, 580, 25, ALIGN_LEFT,double_buffer);
		drawBitmap_onBoard(game->bmps.one, 640, 100, ALIGN_LEFT,double_buffer);
		drawBitmap_onBoard(game->bmps.dots, 665, 100, ALIGN_LEFT,double_buffer);
		int minutes = 0;
		int seconds;
		int temp = a[0];
		while(temp >= 60 * 60){
			temp -= 60*60;
			minutes++;
		}
		seconds = temp / 60;
		display_number(game,minutes,665 + 64,100,double_buffer);
		display_number(game,seconds,665 + 64 + 75,100,double_buffer);
		drawBitmap_onBoard(game->bmps.dots, 665 + 64 + (75 -20), 100, ALIGN_LEFT,double_buffer);
	}
	if(a[1] != 0){
		drawBitmap_onBoard(game->bmps.two, 640, 150, ALIGN_LEFT,double_buffer);
		drawBitmap_onBoard(game->bmps.dots, 665, 150, ALIGN_LEFT,double_buffer);
		int minutes = 0;
		int seconds;
		int temp = a[1];
		while(temp >= 60 * 60){
			temp -= 60*60;
			minutes++;
		}
		seconds = temp / 60;
		display_number(game,minutes,665 + 64,150,double_buffer);
		display_number(game,seconds,665 + 64 + 75,150,double_buffer);
		drawBitmap_onBoard(game->bmps.dots, 665 + 64 + (75 -20), 150, ALIGN_LEFT,double_buffer);
	}
	if(a[2] != 0){
		drawBitmap_onBoard(game->bmps.three, 640, 200, ALIGN_LEFT,double_buffer);
		drawBitmap_onBoard(game->bmps.dots, 665, 200, ALIGN_LEFT,double_buffer);
		int minutes = 0;
		int seconds;
		int temp = a[2];
		while(temp >= 60 * 60){
			temp -= 60*60;
			minutes++;
		}
		seconds = temp / 60;
		display_number(game,minutes,665 + 64,200,double_buffer);
		display_number(game,seconds,665 + 64 + 75,200,double_buffer);
		drawBitmap_onBoard(game->bmps.dots, 665 + 64 + (75 -20), 200, ALIGN_LEFT,double_buffer);
	}
	if(file != NULL) fclose(file);
	return 0;
}

int menu1(Game*game){
	//Variable declaration
	int array[3] = {0,0,0};
	int array_pos = 0;
	int counter_timer = 0;
	int ipc_status;
	unsigned short key = 0;
	message msg;
	//Drawing first menu background...

	if((drawBitmap_byLine(game->bmps.background, 0, 0, ALIGN_LEFT)) != 0){
		return 1;
	}
	if((drawBitmap_byPixel(game->bmps.play, 200, 400, ALIGN_LEFT)) != 0)
		return 1;
	if((drawBitmap_byPixel(game->bmps.quit, 200, 500, ALIGN_LEFT)) != 0)
		return 1;
	if((drawBitmap_byPixel(game->bmps.slash, 105, 700, ALIGN_LEFT)) != 0)
		return -1;
	if((drawBitmap_byPixel(game->bmps.slash, 180, 700, ALIGN_LEFT)) != 0)
		return -1;
	if((drawBitmap_byPixel(game->bmps.dots, 405, 700, ALIGN_LEFT)) != 0)
		return -1;

	unsigned char * double_buffer = double_buffering();
	unsigned char * background2 = double_buffering();
	game->sudoku.background = double_buffering();
	display_hs(game,&double_buffer);
	display_date(game,double_buffer);
	draw_mouse(game,array);
	draw_background(double_buffer);
	while(key != ESC) {
		int r = driver_receive(ANY, &msg, &ipc_status);
		if ( r != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) { //Notified
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & game->irq_set_timer){ //timer notification
					counter_timer++;
					//-------------------------------------
					//--------------------------------------
				}
				if (msg.NOTIFY_ARG & game->irq_set_mouse) {//mouse notification
					array[array_pos] = get_packets();
					if((array[0] & BIT(3)) != 0){
						if(array_pos !=2){
							array_pos++;
						}else{

							array_pos = 0;

							draw_background(double_buffer);

							if(mouse_hover(game->bmps.play,game->bmps.play_glow,game->mouse, 200, 400)==1){
								menu2(game);
								game->sudoku.background = background2;
								display_hs(game,&double_buffer);
								draw_background(double_buffer); //Get my background right, don't have to wait for mouse notification to get it right now
							}

							if(mouse_hover(game->bmps.quit,game->bmps.quit_glow,game->mouse,200,500) == 1) key = ESC;

							draw_mouse(game,array);

							int i = 0;
							for(i; i < 3; i++){
								array[i] = 0;
							}

						}
					}else{
						printf("\nnot a valid first byte\n");
					}
				}
				if (msg.NOTIFY_ARG & game->irq_set_kb) {
					key = get_kb();
				}

				if (msg.NOTIFY_ARG & game->irq_set_rtc) {
					display_date(game,double_buffer);
					draw_background(double_buffer);
					draw_mouse(game,NULL);
				}

				break;
			default:
				break;
			}
		}
	}
	return 0;
}

int according_rulles(Game*game,int x, int y, int squares){
	int i1;		//itterators
	int i2;
	int i3;
	int i4;
	//Checking if there's no more than one number on same square
	if(game->sudoku.board[x][y][squares].value != 0){
		for (i1 = 0; i1 < 3; i1++){
			for (i2 = 0; i2 < 3; i2++){
				if(!(i2 == x && i1 == y)){
					if(game->sudoku.board[x][y][squares].value == game->sudoku.board[i2][i1][squares].value){
						return 0;
					}
				}
			}
		}

	//Checking if there's same values on vertical lines
		int squares2 = squares;
		while(squares2 >= 3){
			squares2 -= 3;
		}
		for(i1 = squares2; i1 < 9; i1 += 3){
			for(i2 = 0; i2 < 3; i2++){
				if(!(y == i2 && squares == i1))
				if(game->sudoku.board[x][y][squares].value == game->sudoku.board[x][i2][i1].value){
					return 0;
				}
			}
		}
	}
	//Checking if there's same values on horizontal lines
	int squares3 = squares;
	while(squares3 != 0 && squares3 != 3 && squares3 != 6){
		squares3--;
	}
	int it;
	for(it = 3; it != 0; it--){
		for(i2 = 0; i2 < 3; i2++){
			if(!(x == i2 && squares == squares3))
				if(game->sudoku.board[x][y][squares].value == game->sudoku.board[i2][y][squares3].value){
					return 0;
			}
		}
		squares3++;
	}
	return 1;
}

int check_game(Game*game,unsigned char**double_buffer){
	int error = 0;
	int squares = 0;
	int x = 0;
	int y = 0;
	for(squares = 0; squares < 9; squares++){
		for(x = 0; x < 3; x++){
			for(y = 0; y < 3;y++){
				if(!according_rulles(game,x, y,squares)){
					error++;
				}
				if(!game->sudoku.board[x][y][squares].value){
					error ++;
				}
				erase_square(game->bmps.one,game->sudoku.board[x][y][squares].x,game->sudoku.board[x][y][squares].y,ALIGN_LEFT,double_buffer,game->sudoku.background);
				draw_on_board(game,game->sudoku.board[x][y][squares].x,
						game->sudoku.board[x][y][squares].y,
						game->sudoku.board[x][y][squares].value,double_buffer,
						according_rulles(game,x, y,squares));
			}
		}
	}
	return error;
}

