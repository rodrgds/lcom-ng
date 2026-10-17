#include <stdint.h>
#include "character.h"
#include "video_gr.h"
#include "logic.h"
#include "bitmap.h"

Character * new_character(){
	Character * character = (Character *) malloc(sizeof(Character));

	// defines initial position
	character->x = CHAR_INIT_X;
	character->y = CHAR_INIT_Y;

	unsigned int temp_width,temp_height;

	character->right = map_Bitmap("/home/lcom/proj/code/img/char_right.bmp", &character->width, &character->height);
	character->left = map_Bitmap("/home/lcom/proj/code/img/char_left.bmp", &character->width, &character->height);
	character->current = character->left;
	character->height /= 4;

	character->xspeed = 0.0;
	character->yspeed = 0.0;
	character->state = STOPPED;
	character->falling = 0;
	character->jumping = 0;
	character->sprite_counting = 0;

	return character;
}

void update_character_state(Character * character){
	character->last_state = character->state;
	switch (character->kbd_event){

	//case NO_KEY:
	//case W_UP:
	//
	case A_UP:
	case S_UP:
	case D_UP:
		character->state = STOPPED;
		break;

	case A_DOWN:
		character->state = CHAR_MOVE_LEFT;
		break;

	case D_DOWN:
		character->state = CHAR_MOVE_RIGHT;
		break;
	case W_DOWN:
		if (character->jumping == 0 && character->falling == 0){
			//character->state = CHAR_JUMP;
			character->jumping = 1;
			character->falling = 1;
		}
		break;
	default:
		break;
	}

	switch(character->timer_event){
	case MOVE_FRAME_TICK:
		// increment of sprite counting or reset to 0
		if (character->sprite_counting == (NUMBER_OF_SPRITES -1)){
			character->sprite_counting = 0;
		} else {
			character->sprite_counting++;
		}

		// changes pointer to correct sprite
		if (character->xspeed < 0){
			character->current = character->left + character->width * character->height * character->sprite_counting;
		} else if(character->xspeed > 0){
			character->current = character->right + character->width * character->height * character->sprite_counting;
		} else{
			character->current = character->left;
		}
		break;
	}
}

void update_character(Character * character){
	//updates xspeed based on character state
	switch (character->state){
	case CHAR_FALL:
		break;
	case CHAR_MOVE_LEFT:
		if (can_char_move_x(character,0)== 0)
			character->xspeed = -CHAR_X_SPEED;
		else{
			character->state = STOPPED;
			character->xspeed = 0;
		}
		break;
	case CHAR_MOVE_RIGHT:
		if (can_char_move_x(character,1)== 0)
			character->xspeed = +CHAR_X_SPEED;
		else{
			character->state = STOPPED;
			character->xspeed = 0;
		}
		break;
	case STOPPED:
		character->xspeed = 0;
		break;
	default:
		break;
	}

	if (character->jumping == 1){
		character->yspeed = -CHAR_JUMP_SPEED;
		character->jumping = 0;
	}

	if (character->falling == 0){
		character->yspeed = 0;
	}

	// updates position adding speed components
	character->x += character->xspeed;
	character->y += character->yspeed;

	//updates yspeed after veryfing if character can jump or fall
	if (character->yspeed >= 0){
		if (can_char_fall(character) == 0){
			character->yspeed += GRAVITY_SPEED;
			character->falling = 1;
		} else {
			character->jumping = 0;
			character->falling = 0;
		}
	} else if (character->yspeed < 0){
		if (!can_char_jump(character) == 0){
					character->yspeed = 0;
				}
		character->yspeed += GRAVITY_SPEED;
	}

}

void draw_character(Character * character){
	vg_map_transparent(character->current, (unsigned int)character->x, (unsigned int)character->y, character->width, character->height, GREEN);

	// for debugging purposes
	/*char temp[26];
	sprintf(temp, "%f", character->x);
	vg_string(temp,0,0, 2, WHITE);
	sprintf(temp, "%f", character->y);
	vg_string(temp,0,50, 2, WHITE);
	sprintf(temp, "%d", character->width);
	vg_string(temp,0,100, 2, WHITE);
	sprintf(temp, "%d", character->height);
	vg_string(temp,0,150, 2, WHITE);
	if (character->falling == 1){
		vg_string("FALLING",0,200, 2, WHITE);
	}
	if (character->jumping == 1){
		vg_string("JUMPING",0,250, 2, WHITE);
	}
	sprintf(temp, "%f", character->yspeed);
	vg_string(temp,0,300, 2, WHITE);*/
}

void delete_character(Character * character){
	// releases allocated memory by character
	free(character);
}
