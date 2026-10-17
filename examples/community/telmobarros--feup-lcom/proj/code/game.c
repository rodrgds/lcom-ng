#include <stdint.h>
#include "game.h"
#include "video_gr.h"
#include "logic.h"
#include "bitmap.h"
#include "serial.h"

Game * new_game(unsigned int mode){
	Game * game = (Game*) malloc(sizeof(Game));
	game->game_mode = mode;

	unsigned int x_board, y_board, x_actual,y_actual,x_next,y_next;

	if (game->game_mode == 0){				//SINGLEPLAYER MODE
		x_board = ONE_PLAYER_BOARD_X;
		y_board = ONE_PLAYER_BOARD_Y;

		x_actual= x_board + BOARD_RELATIVE_MIDDLE_X;
		y_actual= y_board;
		x_next = x_board + ONE_PLAYER_RELATIVE_NEXT_PIECE_X;
		y_next = y_board + ONE_PLAYER_RELATIVE_NEXT_PIECE_Y;
	}
	else if (game->game_mode == 1 || game->game_mode == 2){						//2 PLAYERS MODE
		x_board = TWO_PLAYER_BOARD_X;
		y_board = TWO_PLAYER_BOARD_Y;

		x_actual= x_board + BOARD_RELATIVE_MIDDLE_X;
		y_actual= y_board;
		x_next = x_board + TWO_PLAYER_RELATIVE_NEXT_PIECE_X;
		y_next = y_board + TWO_PLAYER_RELATIVE_NEXT_PIECE_Y;
	}

	//Creating board
	game->board.map = map_Bitmap("/home/lcom/proj/code/img/board.bmp", &game->board.width, &game->board.height);
	game->board.x = x_board;
	game->board.y = y_board;

	//Creating first two pieces
	game->actual_piece = new_piece(x_actual,y_actual);
	game->next_piece = new_piece(x_next,y_next);

	//Correction in x to place piece in middle after knowing its width
	game->actual_piece->sprite.x -= (int)((game->actual_piece->sprite.width / 2) /FACE_LENGTH) * 30;  //error when using face_length

	//Initializing others variables of game
	game->fall_delay = INIT_FALL_DELAY;
	game->timer_event = NO_TICK;
	game->kbd_event = NOKEY;
	game->mouse_event = MOUSE_STOPPED;
	game->state = DO_NOTHING;
	game->pieces_already_swapped = 0;
	game->points = 0;
	game->lines = 0;
	game->lines_received = 0;
	return game;
}

void update_gamestate(Game * game, unsigned int event_type){
	if(event_type == 0 ){
		//handle keyboard events
		switch (game->kbd_event){

		case NOKEY:
			game->state = DO_NOTHING;
			break;

		case LEFTKEY_DOWN:
			if (can_piece_move_x(game->actual_piece,&game->board,0)== 0)
				game->state = MOVE_LEFT;
			else
				game->state = DO_NOTHING;
			break;

		case RIGHTKEY_DOWN:
			if (can_piece_move_x(game->actual_piece,&game->board,1)== 0)
				game->state = MOVE_RIGHT;
			else
				game->state = DO_NOTHING;
			break;

		case DOWNKEY_DOWN:
			if (can_piece_fall(game->actual_piece,&game->board)== 0)
				game->state = FALL;
			else if (game->actual_piece->sprite.y >= game->board.y + 2* 30)		//??
				game->state = REACH_END;
			else
				game->state = GAME_OVER;
			break;

		case UPKEY_DOWN:
			game->state = ROTATE;
			break;

		case SPACEBAR_DOWN:
			if (game->pieces_already_swapped == 0){
				game->state = SWAP_PIECES;
				game->pieces_already_swapped = 1;
			}
			break;

		default:
			break;
		}
	}
	else if(event_type == 1){
		//handle mouse events
		switch (game->mouse_event){
		case MOUSE_STOPPED:
			game->state = DO_NOTHING;
			break;
		case MOUSE_LEFT:
				if (can_piece_move_x(game->actual_piece,&game->board,0)== 0)
					game->state = MOVE_LEFT;
				else
					game->state = DO_NOTHING;
			break;

		case MOUSE_RIGHT:
				if (can_piece_move_x(game->actual_piece,&game->board,1)== 0)
					game->state = MOVE_RIGHT;
				else
					game->state = DO_NOTHING;
			break;

		case MOUSE_RIGHT_BTN:
			if (can_piece_fall(game->actual_piece,&game->board)== 0)
				game->state = FALL;
			else if (game->actual_piece->sprite.y >= game->board.y + 2* 30)		//??
				game->state = REACH_END;
			else
				game->state = GAME_OVER;
			break;

		case MOUSE_LEFT_BTN:
			game->state = ROTATE;
			break;

		case MOUSE_MIDDLE_BTN:
			if (game->pieces_already_swapped == 0){
				game->state = SWAP_PIECES;
				game->pieces_already_swapped = 1;
			}
			break;

		default:
			break;
		}
	}
	//else if(event_type == 2){
		//handle timer events to make actual piece fall
		if (game->timer_event == FALL_TICK){
			if(game->state != FALL){
				if (can_piece_fall(game->actual_piece,&game->board)== 0)
					game->state = FALL;
				else if (game->actual_piece->sprite.y >= game->board.y + 2* 30)		//??
					game->state = REACH_END;
				else
					game->state = GAME_OVER;
			}
			game->timer_event = NO_TICK;
		}
	//}
}

void update_game(Game * game){
	if (game->state == FALL){
		game->actual_piece->sprite.yspeed += FACE_LENGTH;

	} else if (game->state == MOVE_LEFT){
		game->actual_piece->sprite.xspeed -= FACE_LENGTH;

	} else if (game->state == MOVE_RIGHT){
		game->actual_piece->sprite.xspeed += FACE_LENGTH;

	} else if (game->state == ROTATE){
		rotate_piece(game->actual_piece, &game->board);

	} else if (game->state == DO_NOTHING){

	} else if (game->state == SWAP_PIECES){
		swap_pieces (game->actual_piece,game->next_piece, &game->board);

	} else if (game->state == REACH_END){
		unsigned int lines_removed;

		//add actual piece to board
		add_piece(game->actual_piece, &game->board);
		//remove finished lines
		lines_removed = remove_finished_lines(&game->board);
		//makes next piece the actual piece
		game->actual_piece = game->next_piece;
		game->actual_piece->sprite.x = game->board.x + BOARD_RELATIVE_MIDDLE_X;
		game->actual_piece->sprite.y = game->board.y;

		//generates next piece
		game->next_piece = new_piece((game->board.x + ONE_PLAYER_RELATIVE_NEXT_PIECE_X),
				(game->board.y + ONE_PLAYER_RELATIVE_NEXT_PIECE_Y));

		//reset of pieces already swapped flag to allow news swapping
		game->pieces_already_swapped = 0;
		//increment points
		game->points += 10;
		game->points += (lines_removed * 50);
		//update of lines completed
		game->lines += lines_removed;
		//update fall delay
		if (lines_removed != 0){
			if (game->lines < 60){
				game->fall_delay = INIT_FALL_DELAY - (unsigned int)(0.5 * game->lines);
			}
		}
		// serial sending lines completed, multiplayer mode only
		if(game->game_mode == 1){
			if (lines_removed == 0){
				add_lines_received(&game->board, game->lines_received);
			}else{
				serial_write_char(lines_removed + game->lines_received);
			}
			game->lines_received = 0;
		}
	} else if (game->state == GAME_OVER){
		//serial sending gameover info, multiplayer mode only
		if(game->game_mode == 1){
			serial_write_char('G');
		}
	}

	//updates actual piece position
	game->actual_piece->sprite.x += game->actual_piece->sprite.xspeed;
	game->actual_piece->sprite.y += game->actual_piece->sprite.yspeed;

	//sets actual piece speed to zero
	game->actual_piece->sprite.xspeed = 0;
	game->actual_piece->sprite.yspeed = 0;
}

void draw_game(Game * game){
	char temp[8];
	//draws black background
	vg_rectangle(0,0,1024,768,BLACK);
	//draws board
	vg_map(game->board.map,game->board.x,game->board.y,game->board.width,game->board.height);
	//draws actual piece
	vg_sprite(&game->actual_piece->sprite,0);

	//draws next piece
	vg_string("NEXT",game->board.x+ONE_PLAYER_RELATIVE_NEXT_PIECE_X,game->board.y+RELATIVE_NEXT_STRING_Y, 2, WHITE);
	vg_sprite(&game->next_piece->sprite,0);

	//draws points
	vg_string("POINTS",game->board.x+ONE_PLAYER_RELATIVE_NEXT_PIECE_X,game->board.y+RELATIVE_POINTS_STRING_Y, 2, WHITE);
	sprintf(temp, "%d", game->points);
	vg_string(temp,game->board.x+ONE_PLAYER_RELATIVE_NEXT_PIECE_X,game->board.y+RELATIVE_POINTS_Y, 2, WHITE);

	//draws lines completed
	vg_string("LINES",game->board.x+ONE_PLAYER_RELATIVE_NEXT_PIECE_X,game->board.y+RELATIVE_LINES_STRING_Y, 2, WHITE);
	sprintf(temp, "%d", game->lines);
	vg_string(temp,game->board.x+ONE_PLAYER_RELATIVE_NEXT_PIECE_X,game->board.y+RELATIVE_LINES_Y, 2, WHITE);

	//draws lines completed
	if (game->game_mode == 1){
		vg_string("WAITING LINES",game->board.x+ONE_PLAYER_RELATIVE_NEXT_PIECE_X,game->board.y+RELATIVE_WAITING_LINES_STRING_Y, 2, WHITE);
		sprintf(temp, "%d", game->lines_received);
		vg_string(temp,game->board.x+ONE_PLAYER_RELATIVE_NEXT_PIECE_X,game->board.y+RELATIVE_WAITING_LINES_Y, 2, WHITE);
	}
}

void delete_game(Game * game){
	//release the allocated memory
	free(game);
}

void add_piece(Piece * piece, Board * board){
	uint16_t * piece_ptr = piece->sprite.map;
	uint16_t * board_ptr = board->map;

	board_ptr += (piece->sprite.x - board->x) + (board->width * (piece->sprite.y - board->y));

	//copies actual piece map to board map at certain position
	unsigned int i,j;
	for(i = 0; i< piece->sprite.height; i++){
		for(j = 0; j < piece->sprite.width;j++){
			if ((*piece_ptr) != 0){
				*board_ptr = *piece_ptr;
			}
			piece_ptr++;
			board_ptr++;
		}
		board_ptr += (board->width - piece->sprite.width);
	}
}

int remove_finished_lines(Board * board){
	int lines_removed = 0;

	uint16_t * board_ptr = board->map;

	board_ptr += (board->width * FACE_LENGTH * 21) + FACE_LENGTH;				//??

	unsigned int i,j, counter;
	for (i = 20; i > 0; i--){
		for (j = 0; j< 10; j++){
			//counts the number of pieces in line
			if (*(board_ptr + j *30) != BLACK && *(board_ptr + j *30) != 0x2965)
				counter++;
		}
		// remove the line by pushing upper lines down if it has 10 pieces
		if(counter == 10){
			uint16_t * tmp_ptr = board_ptr - (board->width * 30);

			unsigned int lines_above;
			for(lines_above = 0; lines_above < i; lines_above++){
				for (j = 0; j< 30; j++){
					memcpy(board_ptr - (lines_above * board->width*30) + (j*board->width),tmp_ptr - (lines_above * board->width*30) + (j*board->width), 10*30*2);	//??
				}
			}
			i++;
			//increment lines removed
			lines_removed++;
			//} else if(counter == 0){
			//break;
		} else{
			//moves to upper line
			board_ptr -= (board->width * FACE_LENGTH);
		}
		counter = 0;
	}
	//returns total lines removed
	return lines_removed;
}

void rotate_piece(Piece * piece, Board * board){
	uint16_t * temp_ptr;
	temp_ptr = (uint16_t *)malloc(piece->sprite.height*piece->sprite.width * 2);
	uint16_t * piece_ptr = piece->sprite.map;

	//rotates piece map
	unsigned int i,j;
	for(i = 0; i < piece->sprite.width; i++){
		piece_ptr = piece->sprite.map +(piece->sprite.width * (piece->sprite.height-1))+i;
		for(j=0; j < piece->sprite.height;j++){
			*temp_ptr = *piece_ptr;
			temp_ptr++;
			piece_ptr -= piece->sprite.width;
		}
	}

	//dimensions reverse
	unsigned int width = piece->sprite.width;
	unsigned int height = piece->sprite.height;
	piece->sprite.height = width;
	piece->sprite.width = height;

	piece_ptr = piece->sprite.map;
	temp_ptr -= (width * height);
	piece->sprite.map = temp_ptr;

	// if piece can be placed transformations are accepted, if not we discard changes
	if (can_piece_be_placed(piece, board) == 1){
		piece->sprite.height = height;
		piece->sprite.width = width;
		piece->sprite.map = piece_ptr;
		free (temp_ptr);
	}
	else
	{
		free (piece_ptr);
	}
}

void swap_pieces(Piece * actual, Piece * next, Board * board){
	unsigned int actual_x = actual->sprite.x;
	unsigned int actual_y = actual->sprite.y;
	unsigned int width = actual->sprite.width;
	unsigned int height = actual->sprite.height;

	uint16_t *actual_ptr = actual->sprite.map;

	// makes actual piece sprite the next piece sprite
	actual->sprite = next->sprite;

	// places new actual piece in top of board
	actual->sprite.x = board->x + BOARD_RELATIVE_MIDDLE_X;
	actual->sprite.y = board->y;

	// changes variables of next piece to certain coordinates
	next->sprite.width = width;
	next->sprite.height = height;
	next->sprite.map = actual_ptr;
}

int add_lines_received(Board * board, unsigned int lines){
	uint16_t * board_ptr = board->map;
	uint16_t * temp_ptr = board->map;
	temp_ptr += (board->width * FACE_LENGTH * lines);

	unsigned int i;
	for(i= 0; i < (22 - lines);i++){
		//copies downer line that is always full of grey blocks
		memcpy(board_ptr, temp_ptr, board->width * FACE_LENGTH * 2);
		board_ptr += board->width * FACE_LENGTH;
		temp_ptr += board->width * FACE_LENGTH;
	}
	temp_ptr = board->map + board->width * FACE_LENGTH * 22;
	for(i= 0; i < lines;i++){
		//moves downer lines up by copying them up
		memcpy(board_ptr, temp_ptr, board->width * FACE_LENGTH * 2);
		board_ptr += board->width * FACE_LENGTH;
	}
}
