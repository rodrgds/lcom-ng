#include <lcom/lcf.h>
#include "gameovermenu.h"

extern enum game_state game_state;

extern int highscores[SCORE_BOARD_SIZE];
extern char names[SCORE_BOARD_SIZE][255];

extern int mouse_x;
extern int mouse_y;
extern int o_mouse_x;
extern int o_mouse_y;
extern xpm_image_t mouse_sprite;
extern xpm_image_t start_sprite;
extern xpm_image_t exit_sprite;
extern xpm_image_t top10_sprite;
extern xpm_image_t title_screen_sprite;

///Called once per frame
void score_board_menu_frame()
{
	
	vg_draw_rectangle(o_mouse_x,o_mouse_y,mouse_sprite.width,mouse_sprite.height,0);

	draw_sprite(top10_sprite,(SCREEN_WIDTH/2)-top10_sprite.width/2,30);

	for(int i = 0; i < SCORE_BOARD_SIZE; i++)
	{
		draw_text(names[i],(SCREEN_WIDTH/2)-strlen(names[i])*LETTERS_WIDTH, 150 + 20*i);
		if(strlen(names[i]) > 1) draw_number(highscores[i],(SCREEN_WIDTH/2)+strlen(names[i])*LETTERS_WIDTH/2, 150 + 20*i);
	}		

	draw_sprite(title_screen_sprite, (SCREEN_WIDTH/2)-title_screen_sprite.width/2, 400);

	draw_sprite(start_sprite, (SCREEN_WIDTH/2)-start_sprite.width/2, 500);

	draw_sprite(exit_sprite, (SCREEN_WIDTH/2)-exit_sprite.width/2, 600);

	draw_sprite(mouse_sprite,mouse_x,mouse_y);

	draw_frame();

	o_mouse_y = mouse_y;
	o_mouse_x = mouse_x;
	
}

///Called once per keyboard interupt, manages exit
void score_board_menu_keyboard(uint8_t data)
{
	if(data == ESCKEY)
	{
		change_game_state(&game_state, END);
        return;
	}
}

///Called once per mouse interupt, manages position and click
void score_board_menu_mouse(struct packet pckt)
{
	mouse_x += pckt.delta_x;
	mouse_y -= pckt.delta_y;

	Object mouseobj = newObject(mouse_x,mouse_y,mouse_sprite,OTHEROBJ);
	Object title_screenobj = newObject((SCREEN_WIDTH/2)-exit_sprite.width/2, 400, exit_sprite, OTHEROBJ);
	Object try_againobj = newObject((SCREEN_WIDTH/2)-exit_sprite.width/2, 500, exit_sprite, OTHEROBJ);
	Object exitobj = newObject((SCREEN_WIDTH/2)-exit_sprite.width/2, 600, exit_sprite, OTHEROBJ);

	if((pckt.lb || pckt.mb || pckt.rb) && colides(mouseobj,try_againobj))
	{
		game_init();
		change_game_state(&game_state, GAME);
        return;
	}
	else if(pckt.lb && colides(mouseobj,exitobj))
	{
		change_game_state(&game_state, END);
        return;
	}
	else if(pckt.lb && colides(mouseobj,title_screenobj))
	{
		change_game_state(&game_state, MAIN_MENU);
        return;
	}
}
