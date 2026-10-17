#include <lcom/lcf.h>
#include "gameovermenu.h"

/** @defgroup Game-Over-Menu Game-Over-Menu
 * @{
 *
 * Contais tha code for the game over screen
 */

extern enum game_state game_state;
///If the program just entered this screen is false, then it turns true
bool drawn_ = false;
extern int score;
///Will record the user input for the score board
char str[99];
///The size of the user input
int size = 0;

extern int mouse_x;
extern int mouse_y;
extern int o_mouse_x;
extern int o_mouse_y;
extern xpm_image_t mouse_sprite;
extern xpm_image_t exit_sprite;
extern xpm_image_t game_over_sprite;
extern xpm_image_t score_sprite;
extern xpm_image_t try_again_sprite;
extern xpm_image_t score_board_sprite;
extern int highscores[SCORE_BOARD_SIZE];
extern char names[SCORE_BOARD_SIZE][255];

///Called once per frama, draws the menu and mouse cursor
void game_over_menu_frame()
{
	if(!drawn_)
	{

		size = 0;

		str[0] = '\0';

		drawn_ = true;
	}

	vg_draw_rectangle(o_mouse_x,o_mouse_y,mouse_sprite.width,mouse_sprite.height,0);

	draw_sprite(game_over_sprite, (SCREEN_WIDTH/2)-game_over_sprite.width/2, -30);
	
	draw_sprite(score_sprite, (SCREEN_WIDTH/2)-200, 230);

	load_scores();

	if(score > highscores[9])
	{
		draw_text("you got one of the highiest scores", 220, 350);
		draw_text("congratulations", 400, 370);
		draw_text("insert your name:", 200, 400);
	} 

    int nDigits = (score == 0) ? 1 : floor(log10(abs(score))) + 1;

	draw_number(score, (SCREEN_WIDTH/2)-(nDigits*10/2) + 20, 270);

	draw_sprite(score_board_sprite,(SCREEN_WIDTH/2)-75,450);

	draw_sprite(try_again_sprite, (SCREEN_WIDTH/2)-try_again_sprite.width/2, 500);

	draw_sprite(exit_sprite, (SCREEN_WIDTH/2)-exit_sprite.width/2, 550);

	draw_sprite(mouse_sprite,mouse_x,mouse_y);

	draw_frame();

	o_mouse_y = mouse_y;
	o_mouse_x = mouse_x;
	
}

///Adds the current score the the top 10 scores array and file
void update_score_board()
{
	int i = 0;
	for(; i < SCORE_BOARD_SIZE; i++)
	{
		if(highscores[i] < score)
		{
			break;
		}
	}

	for(int j = SCORE_BOARD_SIZE-1; j >+ i; j--)
	{
		highscores[j] = highscores[j-1];
		strcpy(names[j],names[j-1]);
	}

	highscores[i] = score;
	
	if(size <= 1)
	{
		strcpy(str, "unknown");
	}

	char* tmp_str = (char*)malloc((size+15)*sizeof(char));
	sprintf(tmp_str,"%s %s",get_formated_date(),str);
	
	strcpy(names[i],tmp_str);
	

	FILE *fp;	

		fp = fopen("/home/lcom/labs/proj/scores", "w");
	for(int i = 0; i < SCORE_BOARD_SIZE; i++)
	{
		fprintf(fp, "%s\n", names[i]);
		if(strlen(names[i]) > 1) fprintf(fp, "%d\n", highscores[i]);
	}
	fclose(fp);
}

///Called once per mouse interupt, manages exit and name insertion
void game_over_menu_keyboard(uint8_t data)
{
	if(data == ESCKEY)
	{
		change_game_state(&game_state, END);
        return;
	}
	if(score > highscores[9])
	{
		if(data == ENTER_KEY)
		{
			update_score_board();

			drawn_ = false;
			change_game_state(&game_state, SCORE_BOARD);
        	return;
		}
		draw_text_from_keyboard(str,data,&size,470,400);
	} 
}


///Called once per mouse interupt, manages mouse cursor and click
void game_over_menu_mouse(struct packet pckt)
{
	mouse_x += pckt.delta_x;
	mouse_y -= pckt.delta_y;

	Object mouseobj = newObject(mouse_x,mouse_y,mouse_sprite,OTHEROBJ);
	Object scoreboardobj = newObject((SCREEN_WIDTH/2)-exit_sprite.width/2, 450, exit_sprite, OTHEROBJ);
	Object try_againobj = newObject((SCREEN_WIDTH/2)-exit_sprite.width/2, 500, exit_sprite, OTHEROBJ);
	Object exitobj = newObject((SCREEN_WIDTH/2)-exit_sprite.width/2, 550, exit_sprite, OTHEROBJ);

	if((pckt.lb || pckt.mb || pckt.rb) && colides(mouseobj,try_againobj))
	{
		game_init();
		change_game_state(&game_state, GAME);
        return;
	}
	else if((pckt.lb || pckt.mb || pckt.rb) && colides(mouseobj,exitobj))
	{
		change_game_state(&game_state, END);
        return;
	}
	else if((pckt.lb || pckt.mb || pckt.rb) && colides(mouseobj,scoreboardobj))
	{
		if(score > highscores[9]) update_score_board();

		change_game_state(&game_state, SCORE_BOARD);
        return;
	}
}

