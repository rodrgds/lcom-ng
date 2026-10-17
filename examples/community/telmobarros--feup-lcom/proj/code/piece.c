#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "piece.h"
#include "video_gr.h"

Piece * new_piece(unsigned int x, unsigned int y){
	// randomizes a number between 0 and 6 that are equivalent to the 7 pieces possible
	int random = rand() % 7;
	Piece * piece = (Piece*) malloc(sizeof(Piece));
	int tempwidth;
	int tempheight;
	piece->sprite.x = x;
	piece->sprite.y = y;
	piece->sprite.xspeed = 0;
	piece->sprite.yspeed = 0;
	// loads image of correspondent piece given random number
	if(random == 0){
		piece->sprite.map = (uint16_t *)map_Bitmap("/home/lcom/proj/code/img/i.bmp", &tempwidth, &tempheight);
	}
	else if(random == 1){
		piece->sprite.map = (uint16_t *)map_Bitmap("/home/lcom/proj/code/img/j.bmp", &tempwidth, &tempheight);
	}
	else if(random == 2){
		piece->sprite.map = (uint16_t *)map_Bitmap("/home/lcom/proj/code/img/l.bmp", &tempwidth, &tempheight);
	}
	else if(random == 3){
		piece->sprite.map = (uint16_t *)map_Bitmap("/home/lcom/proj/code/img/o.bmp", &tempwidth, &tempheight);
	}
	else if(random == 4){
		piece->sprite.map = (uint16_t *)map_Bitmap("/home/lcom/proj/code/img/s.bmp", &tempwidth, &tempheight);
	}
	else if(random == 5){
		piece->sprite.map = (uint16_t *)map_Bitmap("/home/lcom/proj/code/img/t.bmp", &tempwidth, &tempheight);
	}
	else if(random == 6){
		piece->sprite.map = (uint16_t *)map_Bitmap("/home/lcom/proj/code/img/z.bmp", &tempwidth, &tempheight);
	}

	piece->sprite.width = tempwidth;
	piece->sprite.height = tempheight;

	return piece;
};

void draw_piece(Piece * piece){
	vg_sprite(&piece->sprite, BLACK);
}

void delete_piece(Piece * piece){
	// releases the allocated memory for piece structure
	free(piece);
}
