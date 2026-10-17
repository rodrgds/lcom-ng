#include "menu.h"
#include "video_gr.h"

Menu * new_main_menu(){
	// allocates memory and loads background image
	Menu * main_menu = (Menu*) malloc(sizeof(Menu));
	main_menu->buttons = (Button*)malloc(sizeof(Button)*6);
	main_menu->background = loadBitmap("/home/lcom/proj/code/img/background.bmp");

	// loads all buttons images when mouse is and is not above the button
	main_menu->buttons[0].null = map_Bitmap("/home/lcom/proj/code/img/button0_null.bmp", &main_menu->buttons[0].width, &main_menu->buttons[0].height);
	main_menu->buttons[0].above = map_Bitmap("/home/lcom/proj/code/img/button0_above.bmp", &main_menu->buttons[0].width, &main_menu->buttons[0].height);
	main_menu->buttons[1].null = map_Bitmap("/home/lcom/proj/code/img/button1_null.bmp", &main_menu->buttons[1].width, &main_menu->buttons[1].height);
	main_menu->buttons[1].above = map_Bitmap("/home/lcom/proj/code/img/button1_above.bmp", &main_menu->buttons[1].width, &main_menu->buttons[1].height);
	main_menu->buttons[2].null = map_Bitmap("/home/lcom/proj/code/img/button2_null.bmp", &main_menu->buttons[2].width, &main_menu->buttons[2].height);
	main_menu->buttons[2].above = map_Bitmap("/home/lcom/proj/code/img/button2_above.bmp", &main_menu->buttons[2].width, &main_menu->buttons[2].height);
	main_menu->buttons[3].null = map_Bitmap("/home/lcom/proj/code/img/button3_null.bmp", &main_menu->buttons[3].width, &main_menu->buttons[3].height);
	main_menu->buttons[3].above = map_Bitmap("/home/lcom/proj/code/img/button3_above.bmp", &main_menu->buttons[3].width, &main_menu->buttons[3].height);
	main_menu->buttons[4].null = map_Bitmap("/home/lcom/proj/code/img/button4_null.bmp", &main_menu->buttons[4].width, &main_menu->buttons[4].height);
	main_menu->buttons[4].above = map_Bitmap("/home/lcom/proj/code/img/button4_above.bmp", &main_menu->buttons[4].width, &main_menu->buttons[4].height);
	main_menu->buttons[5].null = map_Bitmap("/home/lcom/proj/code/img/button5_null.bmp", &main_menu->buttons[5].width, &main_menu->buttons[5].height);
	main_menu->buttons[5].above = map_Bitmap("/home/lcom/proj/code/img/button5_above.bmp", &main_menu->buttons[5].width, &main_menu->buttons[5].height);

	unsigned int i;
	for(i = 0; i < 6;i++)
	{
		// sets buttons position
		main_menu->buttons[i].x = (getH_res()/2) - (main_menu->buttons[i].width/2);
		main_menu->buttons[i].y =200 + i * 75;
	}


	main_menu->event = NOACTION;
	main_menu->state = DONOTHING;

	return main_menu;
}

void update_main_menu_state(Menu * menu){
	switch (menu->event){
	case LEFT_CLICK:
		switch (menu->state){
		case BUTTON0_SELECT:
			menu->state = SINGLEPLAYER;
			break;
		case BUTTON1_SELECT:
			menu->state = MULTIPLAYER;
			break;
		case BUTTON2_SELECT:
			menu->state = BATTLE;
			break;
		case BUTTON3_SELECT:
			menu->state = HIGHSCORES;
			break;
		case BUTTON4_SELECT:
			menu->state = INSTRUCTIONS;
			break;
		case BUTTON5_SELECT:
			menu->state = SHUTDOWN;
			break;
		default:
			menu->state = DONOTHING;
			break;
		}
		break;
		case BUTTON0_ISABOVE:
			menu->state = BUTTON0_SELECT;
			break;
		case BUTTON1_ISABOVE:
			menu->state = BUTTON1_SELECT;
			break;
		case BUTTON2_ISABOVE:
			menu->state = BUTTON2_SELECT;
			break;
		case BUTTON3_ISABOVE:
			menu->state = BUTTON3_SELECT;
			break;
		case BUTTON4_ISABOVE:
			menu->state = BUTTON4_SELECT;
			break;
		case BUTTON5_ISABOVE:
			menu->state = BUTTON5_SELECT;
			break;
		default:
			menu->state = DONOTHING;
			break;
	}
}

void update_main_menu(Menu * menu){
	// all buttons flag are reseted to avoid "ghost effect" when passing through all buttons fast
	unsigned int i;
	for(i=0; i < 6; i++){
		//resets all buttons is above flag
		menu->buttons[i].isAbove = 0;
	}

	switch (menu->state){
	case BUTTON0_SELECT:
		menu->buttons[0].isAbove = 1;
		break;
	case BUTTON1_SELECT:
		menu->buttons[1].isAbove = 1;
		break;
	case BUTTON2_SELECT:
		menu->buttons[2].isAbove = 1;
		break;
	case BUTTON3_SELECT:
		menu->buttons[3].isAbove = 1;
		break;
	case BUTTON4_SELECT:
		menu->buttons[4].isAbove = 1;
		break;
	case BUTTON5_SELECT:
		menu->buttons[5].isAbove = 1;
		break;
	default:
		break;
	}
}

void draw_main_menu(Menu * menu){
	//draws background image
	drawBitmap(menu->background,0,0,ALIGN_LEFT);
	//draws all buttons
	unsigned int i;
	for(i = 0; i < 6; i++){
		if(menu->buttons[i].isAbove == 1){
			vg_map(menu->buttons[i].above, menu->buttons[i].x, menu->buttons[i].y, menu->buttons[i].width, menu->buttons[i].height);
		}
		else{
			vg_map(menu->buttons[i].null, menu->buttons[i].x, menu->buttons[i].y, menu->buttons[i].width, menu->buttons[i].height);
		}
	}
}

void delete_main_menu(Menu * menu){
	//releases menu allocated memory
	free(menu);
}
