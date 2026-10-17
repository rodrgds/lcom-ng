#include <stdint.h>

#include "logic.h"
#include "logic.h"
#include "video_gr.h"

int can_piece_move_x(Piece * piece, Board * board, unsigned int dir){
	uint16_t * piece_ptr;
	uint16_t * board_ptr;

	int relative_x;
	unsigned int i;
	for(i=0; i<(piece->sprite.height / FACE_LENGTH);i++){
		piece_ptr = piece->sprite.map + piece->sprite.width * i * FACE_LENGTH;
		if (dir == 1){
			piece_ptr += piece->sprite.width -1;	//To get the last "pixel"of the line
		}

		relative_x = 0;								//relative x of block in piece
		while((*piece_ptr) == 0){
			if (dir == 0){
				relative_x += FACE_LENGTH;
			}else if (dir == 1){
				relative_x -= FACE_LENGTH;
			}
			piece_ptr += relative_x;
		}

		board_ptr = board->map;

		if (dir == 0){
			board_ptr +=
					((piece->sprite.x - board->x) + relative_x - 1) +
					(board->width * ((piece->sprite.y - board->y) + i * FACE_LENGTH));
		}else if (dir == 1){
			board_ptr +=
					((piece->sprite.x - board->x) + piece->sprite.width + relative_x) +
					(board->width * ((piece->sprite.y - board->y) + i * FACE_LENGTH));
		}
		if ((*board_ptr) != 0){			//Verifies the position is not black
			return 1;
		}
	}

	return 0;
}

int can_piece_fall(Piece * piece, Board * board){
	uint16_t * piece_ptr;
	uint16_t * board_ptr;

	int relative_y;
	unsigned int i;
	for(i=0; i<(piece->sprite.width / FACE_LENGTH);i++){
		piece_ptr = piece->sprite.map +
				(piece->sprite.width * (piece->sprite.height -1)) +
				i * FACE_LENGTH;

		relative_y = 0;
		while((*piece_ptr) == 0){							// "Climbs" piece sprite to find "pixel" not black
			relative_y -= FACE_LENGTH;
			piece_ptr -= piece->sprite.width * FACE_LENGTH;
		}

		board_ptr = board->map;

		board_ptr +=
				((piece->sprite.x - board->x) + i*FACE_LENGTH) +
				(board->width * ((piece->sprite.y - board->y) + piece->sprite.height + relative_y));

		if ((*board_ptr) != 0){			//Verifies the position is not black
			return 1;
		}
	}

	return 0;
}

int can_piece_be_placed(Piece * piece, Board * board){
	uint16_t * piece_ptr;
	uint16_t * board_ptr;

	//verifies if all blocks that piece will be are black blocks (empty space)
	unsigned int i, j;
	for(i=0; i<(piece->sprite.height / FACE_LENGTH);i++){
		for(j=0; j<(piece->sprite.width / FACE_LENGTH);j++){
			piece_ptr = piece->sprite.map + (j* FACE_LENGTH)+ (i *piece->sprite.width * FACE_LENGTH);
			board_ptr = board->map + ((piece->sprite.x - board->x) + (j*FACE_LENGTH)) +
					(board->width * ((piece->sprite.y - board->y) + (i * FACE_LENGTH)));
			if(*piece_ptr != 0x00 && *board_ptr != BLACK)
				return 1;
		}
	}
	return 0;
}

int can_char_move_x(Character * character, unsigned int dir){
	if (dir == 0){
		if (vg_get_pixel((unsigned int)(character->x + character->xspeed - 1), (unsigned int)(character->y + 1)) == BLACK)
			if (vg_get_pixel((unsigned int)(character->x + character->xspeed - 1), (unsigned int)(character->y + character->height - 1)) == BLACK)
				return 0;
			else
				return 1;
		else
			return 1;
	}else{
		if (vg_get_pixel((unsigned int)(character->x + character->width + character->xspeed + 1), (unsigned int)(character->y + 1)) == BLACK)
			if (vg_get_pixel((unsigned int)(character->x + character->width + character->xspeed + 1), (unsigned int)(character->y + character->height - 1)) == BLACK)
				return 0;
			else
				return 1;
		else
			return 1;
	}
}

int can_char_fall(Character * character){
	if (vg_get_pixel((unsigned int)(character->x), (unsigned int)(character->y + character->height + character->yspeed)) == BLACK && vg_get_pixel((unsigned int)(character->x + character->width - 1), (unsigned int)(character->y + character->height + character->yspeed)) == BLACK)
		return 0;
	else
		return 1;
}
int can_char_jump(Character * character){
	if (vg_get_pixel((unsigned int)(character->x), (unsigned int)(character->y + character->yspeed)) == BLACK && vg_get_pixel((unsigned int)(character->x + character->width - 1), (unsigned int)(character->y + character->yspeed)) == BLACK)
		return 0;
	else
		return 1;
}

int char_piece_collision(Character * character,Piece * piece){
	if ((character->x + character->width) > piece->sprite.x && character->x < (piece->sprite.x + piece->sprite.width))
		if (character->y > piece->sprite.y && (character->y + character->yspeed) < (piece->sprite.y + piece->sprite.height))
			if (vg_get_pixel((unsigned int)(character->x), (unsigned int)(character->y + character->yspeed)) != BLACK || vg_get_pixel((unsigned int)(character->x + character->width - 1), (unsigned int)(character->y + character->yspeed)) != BLACK)
				return 0;
			else
				return 1;
		else
			return 1;
	else
		return 1;
}
