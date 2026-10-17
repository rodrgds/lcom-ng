#include <lcom/lcf.h>
#include "mainmenu.h"

/** @defgroup Main-Menu Main-Menu
 * @{
 *
 * Contais tha code for the main menu screen
 */

extern enum game_state game_state;

///Mouse x position
int mouse_x = SCREEN_WIDTH/2;

///Mouse y position 
int mouse_y = SCREEN_HEIGHT/2;

///Mouse x position in last frame
int o_mouse_x = 0;

///Mouse y position in last frame
int o_mouse_y = 0;

///Mouse cursor sprite
xpm_image_t mouse_sprite;

///Alien player sprite
xpm_image_t alien_sprite;

///Asteroid sprite
xpm_image_t asteroid_sprite;

///Broken asteroid sprite
xpm_image_t broken_asteroid_sprite;

///Background sprite
xpm_image_t background_sprite;

///Title name sprite
xpm_image_t title_sprite;

///How to play text sprite
xpm_image_t how_to_play_sprite;

///Start button sprite
xpm_image_t start_sprite;

///Exit button sprite
xpm_image_t exit_sprite;

///Game over text sprite
xpm_image_t game_over_sprite;

///Score text sprite
xpm_image_t score_sprite;

///try again button sprite
xpm_image_t try_again_sprite;


///score board button sprite
xpm_image_t score_board_sprite;

///top 10 text sprite
xpm_image_t top10_sprite;

///title screen button sprite
xpm_image_t title_screen_sprite;

///Loads the XPM into xpm_image_t for further use
void load_sprites()
{
    load_numbers();
    xpm_load(ALIEN,XPM_INDEXED,&alien_sprite);
    xpm_load(ASTEROIDE,XPM_INDEXED,&asteroid_sprite);
    xpm_load(ASTEROIDE_BROKEN,XPM_INDEXED,&broken_asteroid_sprite);
    xpm_load(BACKGROUND,XPM_INDEXED,&background_sprite);
    xpm_load(TITLE,XPM_INDEXED,&title_sprite);
    xpm_load(HOW_TO_PLAY,XPM_INDEXED,&how_to_play_sprite);
    xpm_load(START,XPM_INDEXED,&start_sprite);
    xpm_load(EXIT,XPM_INDEXED,&exit_sprite);
	xpm_load(MOUSE_CURSOR,XPM_INDEXED,&mouse_sprite);
	xpm_load(GAME_OVER_XPM,XPM_INDEXED,&game_over_sprite);
	xpm_load(TRY_AGAIN,XPM_INDEXED,&try_again_sprite);
	xpm_load(SCORE,XPM_INDEXED,&score_sprite);
	xpm_load(SCOREBOARDXPM,XPM_INDEXED,&score_board_sprite);
	xpm_load(TOP10,XPM_INDEXED,&top10_sprite);
	xpm_load(TITLE_SCREEN,XPM_INDEXED,&title_screen_sprite);

}

///Called once per frame on main menu
void main_menu_frame()
{

	vg_draw_rectangle(o_mouse_x,o_mouse_y,mouse_sprite.width,mouse_sprite.height,0);
	
	draw_sprite(background_sprite,0,0);

	draw_sprite(title_sprite, (SCREEN_WIDTH/2)-title_sprite.width/2, 50);
	
	draw_sprite(alien_sprite, 800, 50);

	draw_sprite(how_to_play_sprite, 700, 500);

	draw_sprite(start_sprite, (SCREEN_WIDTH/2)-start_sprite.width/2, 350);

	draw_sprite(score_board_sprite, (SCREEN_WIDTH/2)-score_board_sprite.width/2, 450);

	draw_sprite(exit_sprite, (SCREEN_WIDTH/2)-exit_sprite.width/2, 550);

	draw_sprite(mouse_sprite,mouse_x,mouse_y);

	draw_frame();

	o_mouse_y = mouse_y;
	o_mouse_x = mouse_x;
}

///Called once per keyboard interupt, manages exit
void main_menu_keyboard(uint8_t data)
{
	if(data == ESCKEY)
	{
		change_game_state(&game_state, END);
        return;
	}
}


///Called once per mouse interupt, manages mouse position and click
void main_menu_mouse(struct packet pckt)
{
	mouse_x += pckt.delta_x;
	mouse_y -= pckt.delta_y;

	Object mouseobj = newObject(mouse_x,mouse_y,mouse_sprite,OTHEROBJ);
	Object startobj = newObject((SCREEN_WIDTH/2)-start_sprite.width/2, 350,start_sprite,OTHEROBJ);
	Object scoreboardobj = newObject((SCREEN_WIDTH/2)-exit_sprite.width/2, 450,exit_sprite,OTHEROBJ);
	Object exitobj = newObject((SCREEN_WIDTH/2)-exit_sprite.width/2, 550,exit_sprite,OTHEROBJ);

	if((pckt.lb || pckt.mb || pckt.rb) && colides(mouseobj,startobj))
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
		change_game_state(&game_state, SCORE_BOARD);
        return;
	}
}

