#include "libraries.h"


										//WARNING//
			//IN THIS CODE THE PLAYER1 REFERENCES ARE REALLY REFERENCES TO THE PLAYER2 //
							//WE NEED TO CHANGE THIS THIS WEEKEND //



//AI definitions
int player1_square_width = 35;
int player1_square_height = 35;
int player1_board_xposition = 0; 		//equal to the final x position of player2 board
int player1_board_yposition = 0;		//equal to the final x position of player2 board


//player 1 definitions
int player2_square_width = 28;
int player2_square_height = 28;
int player2_board_xposition = 200;
int player2_board_yposition = 100;

//global color definitions
unsigned long unselected_color = 0;     //color of the squares outline on the matrix when they are unselected
unsigned long selected_color = 07;		//color of the squares outline on the matrix when they are selected
unsigned long square_color = 25;  		//color of the squares on the matrix
unsigned long player_ship_color = 7;	//color of the ships on the table
unsigned long ship_destroyed_color = 4; //color of the ships after been destroyed
unsigned long ship_hit = 36;			//color of the square on the table when a piece of a ship is hitted
unsigned long ship_miss = 8;			//color of the square on the table when the shot misses a ship
unsigned long ship_revived = 34 ;       //color for the square of the ship when revived
unsigned long ship_found = 	52;			//color for the square when a ship is found by using scout
unsigned long background_color = 11;    //color for the background of the game

//global variables definitions
int number_of_tries = 0;  				//for the interrupt_handler() condition || 1 if enter has been pressed in a available position
										//if pressed,terminate the interrupt_handler turn and begin the AI_turn()
int board_width = 10;
int board_height = 10;
unsigned long square_outline_gap = 5; 	//gap used to compensate the board outline
unsigned long player1_hits = 0; 		//number of hits made by the player 1 on the ships in the table - used to end he game
unsigned long AI_hits = 0;     			//number of hits made by the AI on the ships in the table        -used to end the game

int ships1[5] = {2,3,3,4,5};  			//vector made to check if any of the ships in the player1 table has been destroyed
int ships2[5] = {2,3,3,4,5};			//vector made to check if any of the ships in the player2 table has been destroyed
int ships_aux[5] = {2,3,3,4,5}; 		//vector made to help in the scout special attack.It is used to compare the actual ships2[5] values
										//with the pre-defined values

int volley_flag = 0;					//flag to know when the volley special attack has been made by the player1
int scout_flag = 0;						//flag to know when the scout special attack has been made by the player1
int bombardement_flag = 0;				//flag to know when the bombardement special attack has been made by the player1

int AI_volley_flag = 0;					//flag to know when the volley special attack has been made by the AI
int AI_scout_flag = 0;					//flag to know when the scout special attack has been made by the AI
int AI_bombardement_flag = 0;			//flag to know when the bombardement special attack has been made by the AI

int menu_flag = 0;             			 //flag to know if we are still in the menu || 1-not in the menu || 0-still in the menu
int play_flag = 0;						//flag to know if the player has choose the play option
int exit_flag = 0;						//flag to know if the player has choose the quit option

//square structure used to make a matrix that represents each table
typedef struct
{
	int x;
	int y;
	int content; //1 if has something || 0 is is empty
	int checked; //1 if has already been checked || 0 if not
}square;

//square_aux structure used to make the auxiliary table
//it contains the same positions checked that the normal table,bus in each square where is a ship
//it contains an ID that refers to that ship
typedef struct
{
	int id;
}square_aux;

//ID reference:
//0->Default Water
//1->2-square-ship
//2->1st 3-square-ship
//3->2nd 3-square-ship
//4->4-square-ship
//5->5-square-ship
//6->wreckage

//strucuture used to virtually represent our keyboard cursor of the game
typedef struct
{
	int x; //player1_board_xposition;
	int y; //player1_board_yposition;
	int row;
	int column;
}cursor;

//structure used to represent the AI_shot
//When it finds a ships the ship_found flag is raised and he tracks down that ship
//It keeps track of the previous hit shot in order to facilitate the moves
typedef struct
{
	int ship_found; 					//it indicates if the AI engine has find a ship
	int previous_row;					//it indicates the row of the previous shot
	int previous_column;				//it indicates the column of the previous shot

}AI_shot;

square player1_board[10][10];        	//structure that represents the player2 table
square_aux player1_board_aux[10][10];	//auxiliary structure that is used to save the ships position on the player2 table
									 	 //and associate an ID to each of the ship component - used to know when a ship
									 	 //has been destroyed and which ship has been destroyed on the player2 table

square player2_board[10][10];		 	//structure that represents the player1 table
square_aux player2_board_aux[10][10];	//auxiliary structure that is used to save the ships position on the player1 table
										//and associate an ID to each of the ship component - used to know when a ship
									 	 //has been destroyed and which ship has been destroyed on the player1 table

cursor keyboard_cursor;					//structure used to represent virtually the keyboard cursor

AI_shot ai_shot;						//structure used to represent the AI_shot

//draws a sprite *spr at (x,y) position of the screen
void draw_sprite(int x,int y,Sprite *spr)
{
	int i,j;

	spr->x = x;
	spr->y = y;

	for(i=0;i<spr->height;i++)
		for(j=0;j<spr->width;j++)
		{
			vg_set_pixel(spr->x+j,spr->y+i,*(spr->map+((i*spr->width)+j)));
		}
}

//draws an outline with a pixel thick of the color desired,starting at position (x,y)
void draw_outline(int x,int y,int width,int height,unsigned long color)
{
	int xf = x+width;
	int yf = y+height;

	vg_draw_line(x,y,xf+1,y,color);
	vg_draw_line(x,y,x,yf+1,color);
	vg_draw_line(x,yf,xf,yf,color);
	vg_draw_line(xf,y,xf,yf+1,color);
}

//draws a square at position (x,y),with the width,height and color specified as argument
void draw_square(int x,int y,int width,int height,unsigned color)
{
	int i,j;

	draw_outline(x,y,width,height,unselected_color); //draws the black outline

	for(i=1;i!=height;i++)
		for(j=1;j!=width;j++)
			vg_set_pixel(x+j,y+i,color);

}

//draws the AI board where the game is going be played
//it initializes every position of the AI_board structure
//it initializes every position of the AI_board_aux with the ID 0
void draw_player1_board()
{

	int i,j;
	int x = player1_board_xposition;
	int y = player1_board_yposition;

	for(i=0;i<board_height;i++)
	{
		if(i!=0)
			y+=player1_square_height;
		x = player1_board_xposition;

		for(j=0;j<board_width;j++)
		{
			draw_square(x,y,player1_square_width,player1_square_height,square_color);
			player1_board[j][i].x = x;
			player1_board[j][i].y = y;
			player1_board[j][i].content = 0;
			player1_board[j][i].checked = 0;
			player1_board_aux[j][i].id = 0;
			x+=player1_square_width;
		}
	}
}

//draws the player1 board where the AI is going do is shots
//it initializes every position of the player1_board structure
//it initializes every position of the player1_board_aux with the ID 0
void draw_player2_board()
{

	int i,j;
	int x = player2_board_xposition;
	int y = player2_board_yposition;

	for(i=0;i<board_height;i++)
	{
		if(i!=0)
			y+=player2_square_height;
		x = player2_board_xposition;

		for(j=0;j<board_width;j++)
		{
			draw_square(x,y,player2_square_width,player2_square_height,square_color);
			player2_board[j][i].x = x;
			player2_board[j][i].y = y;
			player2_board[j][i].content = 0;
			player2_board[j][i].checked = 0;
			x+=player2_square_width;
		}
	}

	player1_board_xposition = x+4;
	player1_board_yposition = (y+5)+player2_square_height;
}

//draws the black outline of 5 pixels surrounding the AI_board
void draw_player1_board_outline()
{
	int x = player1_board_xposition;
	int y = player1_board_yposition-player1_square_height-square_outline_gap;

	int i;

	//draw black border
	for(i=0;i<5;i++)
		draw_outline((x-5)+i,(player1_board_yposition-square_outline_gap)+i,350+(5-i)*2,350+(5-i)*2,0);

	for(i=0;i<board_width;i++)
	{
		draw_square(x,y,player1_square_width,player1_square_height,63);
		x+=player1_square_width;
	}

	x = player1_board_xposition-player1_square_width-square_outline_gap;
	y = player1_board_yposition;

	for(i=0;i<board_height;i++)
	{
		draw_square(x,y,player1_square_width,player1_square_height,63);
		y+=player1_square_height;
	}
}

//draws the black outline of 5 pixels surrounding the player1 board
void draw_player2_board_outline()
{
	int x = player2_board_xposition;
	int y = player2_board_yposition-player2_square_height-square_outline_gap;

	int i;


	//draw black border
	for(i=0;i<5;i++)
		draw_outline(195+i,95+i,280+(5-i)*2,280+(5-i)*2,0);

	for(i=0;i<board_width;i++)
	{
		draw_square(x,y,player2_square_width,player2_square_height,63);
		x+=player2_square_width;
	}

	x = player2_board_xposition-player2_square_width-square_outline_gap;
	y = player2_board_yposition;

	for(i=0;i<board_height;i++)
	{
		draw_square(x,y,player2_square_width,player2_square_height,63);
		y+=player2_square_height;
	}
}

//it fills all the pixels of the screen with the color black
void erase_screen()
{
	vg_fill(background_color);
}

//draws the ships that stand next to each table
void draw_ships()
{
	//AI ships
	draw_square(360,400,70,35,player_ship_color);
	draw_square(325,447,105,35,player_ship_color);
	draw_square(325,494,105,35,player_ship_color);
	draw_square(290,541,140,35,player_ship_color);
	draw_square(255,588,175,35,player_ship_color);

	//Player1 ships
	draw_square(500,100,56,28,player_ship_color);
	draw_square(500,140,84,28,player_ship_color);
	draw_square(500,180,84,28,player_ship_color);
	draw_square(500,220,112,28,player_ship_color);
	draw_square(500,260,140,28,player_ship_color);

}

//draws the avatar of each player
void draw_players()
{
	Sprite *player_1_in_game_sp = create_sprite(player1v3,0);
	Sprite *computer_sp = create_sprite(computer,0);
	draw_sprite(25,64,computer_sp);
	draw_sprite(865,345,player_1_in_game_sp);

	//draw_square(700,72,135,100,63);
	//draw_square(700,176,100,30,7);
	//draw_square(173,400,135,100,63);
	//draw_square(173,506,100,30,7);
}

//draws the letter and number on the game tables
void draw_letters_numbers()
{
	Sprite *a_27 = create_sprite(a27,0);
	Sprite *b_27 = create_sprite(b27,0);
	Sprite *c_27 = create_sprite(c27,0);
	Sprite *d_27 = create_sprite(d27,0);
	Sprite *e_27 = create_sprite(e27,0);
	Sprite *f_27 = create_sprite(f27,0);
	Sprite *g_27 = create_sprite(g27,0);
	Sprite *h_27 = create_sprite(h27,0);
	Sprite *i_27 = create_sprite(i27,0);
	Sprite *j_27 = create_sprite(j27,0);

	Sprite *a_34 = create_sprite(a34,0);
	Sprite *b_34 = create_sprite(b34,0);
	Sprite *c_34 = create_sprite(c34,0);
	Sprite *d_34 = create_sprite(d34,0);
	Sprite *e_34 = create_sprite(e34,0);
	Sprite *f_34 = create_sprite(f34,0);
	Sprite *g_34 = create_sprite(g34,0);
	Sprite *h_34 = create_sprite(h34,0);
	Sprite *i_34 = create_sprite(i34,0);
	Sprite *j_34 = create_sprite(j34,0);

	Sprite *n0_27 = create_sprite(num0_27,0);
	Sprite *n1_27 = create_sprite(num1_27,0);
	Sprite *n2_27 = create_sprite(num2_27,0);
	Sprite *n3_27 = create_sprite(num3_27,0);
	Sprite *n4_27 = create_sprite(num4_27,0);
	Sprite *n5_27 = create_sprite(num5_27,0);
	Sprite *n6_27 = create_sprite(num6_27,0);
	Sprite *n7_27 = create_sprite(num7_27,0);
	Sprite *n8_27 = create_sprite(num8_27,0);
	Sprite *n9_27 = create_sprite(num9_27,0);

	Sprite *n0_34 = create_sprite(num0_34,0);
	Sprite *n1_34 = create_sprite(num1_34,0);
	Sprite *n2_34 = create_sprite(num2_34,0);
	Sprite *n3_34 = create_sprite(num3_34,0);
	Sprite *n4_34 = create_sprite(num4_34,0);
	Sprite *n5_34 = create_sprite(num5_34,0);
	Sprite *n6_34 = create_sprite(num6_34,0);
	Sprite *n7_34 = create_sprite(num7_34,0);
	Sprite *n8_34 = create_sprite(num8_34,0);
	Sprite *n9_34 = create_sprite(num9_34,0);

	//draws player1 table letters
	draw_sprite(player1_board_xposition+1,player1_board_yposition-39,a_34);
	draw_sprite((player1_board_xposition+1)+35,player1_board_yposition-39,b_34);
	draw_sprite((player1_board_xposition+1)+70,player1_board_yposition-39,c_34);
	draw_sprite((player1_board_xposition+1)+105,player1_board_yposition-39,d_34);
	draw_sprite((player1_board_xposition+1)+140,player1_board_yposition-39,e_34);
	draw_sprite((player1_board_xposition+1)+175,player1_board_yposition-39,f_34);
	draw_sprite((player1_board_xposition+1)+210,player1_board_yposition-39,g_34);
	draw_sprite((player1_board_xposition+1)+245,player1_board_yposition-39,h_34);
	draw_sprite((player1_board_xposition+1)+280,player1_board_yposition-39,i_34);
	draw_sprite((player1_board_xposition+1)+315,player1_board_yposition-39,j_34);

	//draws AI table letters
	draw_sprite(201,68,a_27);
	draw_sprite(229,68,b_27);
	draw_sprite(257,68,c_27);
	draw_sprite(285,68,d_27);
	draw_sprite(313,68,e_27);
	draw_sprite(341,68,f_27);
	draw_sprite(369,68,g_27);
	draw_sprite(397,68,h_27);
	draw_sprite(425,68,i_27);
	draw_sprite(453,68,j_27);

	//draws player1 table numbers
	draw_sprite(player1_board_xposition-39,player1_board_yposition+1,n0_34);
	draw_sprite(player1_board_xposition-39,(player1_board_yposition+1)+35,n1_34);
	draw_sprite(player1_board_xposition-39,(player1_board_yposition+1)+70,n2_34);
	draw_sprite(player1_board_xposition-39,(player1_board_yposition+1)+105,n3_34);
	draw_sprite(player1_board_xposition-39,(player1_board_yposition+1)+140,n4_34);
	draw_sprite(player1_board_xposition-39,(player1_board_yposition+1)+175,n5_34);
	draw_sprite(player1_board_xposition-39,(player1_board_yposition+1)+210,n6_34);
	draw_sprite(player1_board_xposition-39,(player1_board_yposition+1)+245,n7_34);
	draw_sprite(player1_board_xposition-39,(player1_board_yposition+1)+280,n8_34);
	draw_sprite(player1_board_xposition-39,(player1_board_yposition+1)+315,n9_34);

	//draws AI table numbers
	draw_sprite(168,101,n0_27);
	draw_sprite(168,129,n1_27);
	draw_sprite(168,157,n2_27);
	draw_sprite(168,185,n3_27);
	draw_sprite(168,213,n4_27);
	draw_sprite(168,241,n5_27);
	draw_sprite(168,269,n6_27);
	draw_sprite(168,297,n7_27);
	draw_sprite(168,325,n8_27);
	draw_sprite(168,353,n9_27);






}

//draws the initial menu that appears before the game
void menu()
{
	//clear the previous screen
	vg_fill(background_color);

	//create the menu sprites
	Sprite *commodore_sp = create_sprite(commodorev3,0);
	Sprite *quit_sp = create_sprite(quit,0);
	Sprite *play_sp = create_sprite(play,0);
	//Sprite *rato_sp = create_sprite(rato,0);

	//draw the menu sprites
	draw_sprite(75,100,commodore_sp);
	draw_sprite(350,300,play_sp);
	draw_sprite(350,420,quit_sp);
	//draw_sprite(100,600,rato_sp);


}

//initializes the virtual keyboard cursor at the position (0,0) of the AI board
void initialize_keyboard_cursor()
{
	keyboard_cursor.x = player1_board_xposition;
	keyboard_cursor.y = player1_board_yposition;
	keyboard_cursor.column = 0;
	keyboard_cursor.row = 0;
	draw_outline(keyboard_cursor.x,keyboard_cursor.y,player1_square_width,player1_square_height,selected_color);
}

void initialize_keyboard_cursor_for_menu()
{
	keyboard_cursor.x = 330;
	keyboard_cursor.y = 290;
	keyboard_cursor.column = 0;
	keyboard_cursor.row = 0;
	draw_outline(330,290,340,110,0);
}


//Function to randomly place the AI ships in the AI table
//It updates the information of the AI_table with 0's and 1's corresponding 0 to water and 1 to a ship
//It updates the AI_table_aux with the ships ID as it puts the ships in the table
void place_AI_ships()
{
	//calculate random coordinates and orientation for the ship to be placed
	int orientation = rand() %4 ; //orientation used by the ship || 0-up || 1-left || 2-down || 3-right
	int position_x = rand() % 10;
	int position_y = rand() % 10;

	//used to control if the ship final position is greater or inferior than the table borders
	int final_position_x = 100;
	int final_position_y = 100;

	int i;

	//Calculate the initial position of the boat in pixels
	int initial_position_x = player1_board_xposition+(position_x*player1_square_width);
	int initial_position_y = player1_board_yposition+(position_y*player1_square_height);

	//Check if the initial position is greater or inferior than the table borders
	while(initial_position_x + 2*player1_square_width > (player1_board_xposition + (player1_square_width*9))|| initial_position_y + 2*player1_square_height > player1_board_yposition + (player1_square_height*9)||initial_position_x - 2*player1_square_width < player1_board_xposition||initial_position_y - 2*player1_square_height < player1_board_yposition)
	{
		//calculate new coordinates and orientation
		position_x = rand() % 10;
		position_y = rand() % 10;
		orientation = rand() %4;

		//variables recalculated to check in the while condition
		initial_position_x = player1_board_xposition+(position_x*player1_square_width);
		initial_position_y = player1_board_yposition+(position_y*player1_square_height);
	}

	//place the 1st 2-square ship
	for(i=0;i<2;i++)
	{
		if(i==0)
		{
			player1_board[position_x][position_y].content = 1;
			player1_board_aux[position_x][position_y].id = 1;
			//draw_square(initial_position_x,initial_position_y,player1_square_width,player1_square_height,0);
		}

		if(orientation == 0)
		{
			player1_board[position_x][position_y-i].content = 1;
			player1_board_aux[position_x][position_y-i].id = 1;
			//draw_square(initial_position_x,initial_position_y-(i*player1_square_height),player1_square_width,player1_square_height,0);
		}
		else
			if(orientation == 1)
			{
				player1_board[position_x-i][position_y].content = 1;
				player1_board_aux[position_x-i][position_y].id = 1;
				//draw_square(initial_position_x-(i*player1_square_width),initial_position_y,player1_square_width,player1_square_height,0);
			}
			else
				if(orientation == 2)
				{
					player1_board[position_x][position_y+i].content = 1;
					player1_board_aux[position_x][position_y+i].id = 1;
					//draw_square(initial_position_x,initial_position_y+(i*player1_square_width),player1_square_width,player1_square_height,0);
				}
				else
					if(orientation == 3)
					{
						player1_board[position_x+i][position_y].content = 1;
						player1_board_aux[position_x+i][position_y].id = 1;
						//draw_square(initial_position_x+(i*player1_square_width),initial_position_y,player1_square_width,player1_square_height,0);
					}
	}


	//place the 1st 3-square ship
	orientation = rand() %4 ; // 0-up // 1-left // 2-down // 3-right
	position_x = rand() % 10;
	position_y = rand() % 10;

	initial_position_x = player1_board_xposition+(position_x*player1_square_width);
	initial_position_y = player1_board_yposition+(position_y*player1_square_height);

	//Check if the initial position is greater or inferior than the table borders or if the possible positions of
	//the ship are already occupied by some other ship
	while(initial_position_x + 3*player1_square_width > (player1_board_xposition + (player1_square_width*9)) || initial_position_y + 3*player1_square_height > player1_board_yposition + (player1_square_height*9) ||initial_position_x - 3*player1_square_width < player1_board_xposition ||initial_position_y - 3*player1_square_height < player1_board_yposition
			|| player1_board[position_x][position_y].content == 1 || player1_board[position_x+1][position_y].content == 1 ||
			player1_board[position_x-1][position_y].content == 1 || player1_board[position_x][position_y+1].content == 1 ||
			player1_board[position_x][position_y-1].content == 1 || player1_board[position_x+2][position_y].content == 1 ||
			player1_board[position_x-2][position_y].content == 1 || player1_board[position_x][position_y+2].content == 1 ||
			player1_board[position_x][position_y-2].content == 1 )
	{
		position_x = rand() % 10;
		position_y = rand() % 10;
		orientation = rand() %4;
		initial_position_x = player1_board_xposition+(position_x*player1_square_width);
		initial_position_y = player1_board_yposition+(position_y*player1_square_height);
	}

	for(i=0;i<3;i++)
	{
		if(i==0)
		{
			player1_board[position_x][position_y].content = 1;
			player1_board_aux[position_x][position_y].id = 2;
			//draw_square(initial_position_x,initial_position_y,player1_square_width,player1_square_height,0);
		}

		if(orientation == 0)
		{
			player1_board[position_x][position_y-i].content = 1;
			player1_board_aux[position_x][position_y-i].id = 2;
			//draw_square(initial_position_x,initial_position_y-(i*player1_square_height),player1_square_width,player1_square_height,0);
		}
		else
			if(orientation == 1)
			{
				player1_board[position_x-i][position_y].content = 1;
				player1_board_aux[position_x-i][position_y].id = 2;
				//draw_square(initial_position_x-(i*player1_square_width),initial_position_y,player1_square_width,player1_square_height,0);
			}
			else
				if(orientation == 2)
				{
					player1_board[position_x][position_y+i].content = 1;
					player1_board_aux[position_x][position_y+i].id = 2;
					//draw_square(initial_position_x,initial_position_y+(i*player1_square_width),player1_square_width,player1_square_height,0);
				}
				else
					if(orientation == 3)
					{
						player1_board[position_x+i][position_y].content = 1;
						player1_board_aux[position_x+i][position_y].id = 2;
						//draw_square(initial_position_x+(i*player1_square_width),initial_position_y,player1_square_width,player1_square_height,0);
					}
	}


	//place the 2nd 3-square ship

	orientation = rand() %4 ; // 0-up // 1-left // 2-down // 3-right
	position_x = rand() % 10;
	position_y = rand() % 10;

	final_position_x = 100;
	final_position_y = 100;

	initial_position_x = player1_board_xposition+(position_x*player1_square_width);
	initial_position_y = player1_board_yposition+(position_y*player1_square_height);

	//Check if the initial position is greater or inferior than the table borders or if the possible positions of
	//the ship are already occupied by some other ship
	while(final_position_x > (player1_board_xposition + (player1_square_width*9)) || final_position_x < player1_board_xposition
			|| final_position_y > player1_board_yposition + (player1_square_height*9) || final_position_y < player1_board_yposition
			|| player1_board[position_x][position_y].content == 1 || player1_board[position_x+1][position_y].content == 1 ||
			player1_board[position_x-1][position_y].content == 1 || player1_board[position_x][position_y+1].content == 1 ||
			player1_board[position_x][position_y-1].content == 1 || player1_board[position_x+2][position_y].content == 1 ||
			player1_board[position_x-2][position_y].content == 1 || player1_board[position_x][position_y+2].content == 1 ||
			player1_board[position_x][position_y-2].content == 1 )
	{
		position_x = rand() % 10;
		position_y = rand() % 10;
		orientation = rand() %4;

		initial_position_x = player1_board_xposition+(position_x*player1_square_width);
		initial_position_y = player1_board_yposition+(position_y*player1_square_height);

		if(orientation == 0)
		{
			final_position_x = initial_position_x;
			final_position_y = initial_position_y - 3*player1_square_height;
		}else
			if(orientation == 1)
			{
				final_position_x = initial_position_x - 3*player1_square_width;
				final_position_y = initial_position_y;
			}else
				if(orientation == 2)
				{
					final_position_x = initial_position_x;
					final_position_y = initial_position_y  + 3*player1_square_height;
				}else
				{
					final_position_x = initial_position_x + 3*player1_square_width;
					final_position_y = initial_position_y;
				}
	}

	for(i=0;i<3;i++)
	{
		if(i==0)
		{
			player1_board[position_x][position_y].content = 1;
			player1_board_aux[position_x][position_y].id = 3;
			//draw_square(initial_position_x,initial_position_y,player1_square_width,player1_square_height,0);
		}

		if(orientation == 0)
		{
			player1_board[position_x][position_y-i].content = 1;
			player1_board_aux[position_x][position_y-i].id = 3;
			//draw_square(initial_position_x,initial_position_y-(i*player1_square_height),player1_square_width,player1_square_height,0);
		}
		else
			if(orientation == 1)
			{
				player1_board[position_x-i][position_y].content = 1;
				player1_board_aux[position_x-i][position_y].id = 3;
				//draw_square(initial_position_x-(i*player1_square_width),initial_position_y,player1_square_width,player1_square_height,0);
			}
			else
				if(orientation == 2)
				{
					player1_board[position_x][position_y+i].content = 1;
					player1_board_aux[position_x][position_y+i].id = 3;
					//draw_square(initial_position_x,initial_position_y+(i*player1_square_width),player1_square_width,player1_square_height,0);
				}
				else
					if(orientation == 3)
					{
						player1_board[position_x+i][position_y].content = 1;
						player1_board_aux[position_x+i][position_y].id = 3;
						//draw_square(initial_position_x+(i*player1_square_width),initial_position_y,player1_square_width,player1_square_height,0);
					}
	}
	//place the 4-square ship

	orientation = rand() %4 ; // 0-up // 1-left // 2-down // 3-right
	position_x = rand() % 10;
	position_y = rand() % 10;

	initial_position_x = player1_board_xposition+(position_x*player1_square_width);
	initial_position_y = player1_board_yposition+(position_y*player1_square_height);

	//Check if the initial position is greater or inferior than the table borders or if the possible positions of
	//the ship are already occupied by some other ship
	while(final_position_x > (player1_board_xposition + (player1_square_width*9)) || final_position_x < player1_board_xposition
			|| final_position_y > player1_board_yposition + (player1_square_height*9) || final_position_y < player1_board_yposition
			|| player1_board[position_x][position_y].content == 1 || player1_board[position_x+1][position_y].content == 1 ||
			player1_board[position_x-1][position_y].content == 1 || player1_board[position_x][position_y+1].content == 1 ||
			player1_board[position_x][position_y-1].content == 1 || player1_board[position_x+2][position_y].content == 1 ||
			player1_board[position_x-2][position_y].content == 1 || player1_board[position_x][position_y+2].content == 1 ||
			player1_board[position_x][position_y-2].content == 1 || player1_board[position_x+3][position_y].content == 1 ||
			player1_board[position_x-3][position_y].content == 1 || player1_board[position_x][position_y+3].content == 1 ||
			player1_board[position_x][position_y-3].content == 1)
	{
		position_x = rand() % 10;
		position_y = rand() % 10;
		orientation = rand() %4;

		initial_position_x = player1_board_xposition+(position_x*player1_square_width);
		initial_position_y = player1_board_yposition+(position_y*player1_square_height);

		if(orientation == 0)
		{
			final_position_x = initial_position_x;
			final_position_y = initial_position_y - 4*player1_square_height;
		}else
			if(orientation == 1)
			{
				final_position_x = initial_position_x - 4*player1_square_width;
				final_position_y = initial_position_y;
			}else
				if(orientation == 2)
				{
					final_position_x = initial_position_x;
					final_position_y = initial_position_y  + 4*player1_square_height;
				}else
				{
					final_position_x = initial_position_x + 4*player1_square_width;
					final_position_y = initial_position_y;
				}
	}



	for(i=0;i<4;i++)
	{
		if(i==0)
		{
			player1_board[position_x][position_y].content = 1;
			player1_board_aux[position_x][position_y].id = 4;
			//draw_square(initial_position_x,initial_position_y,player1_square_width,player1_square_height,0);
		}

		if(orientation == 0)
		{
			player1_board[position_x][position_y-i].content = 1;
			player1_board_aux[position_x][position_y-i].id = 4;
			//draw_square(initial_position_x,initial_position_y-(i*player1_square_height),player1_square_width,player1_square_height,0);
		}
		else
			if(orientation == 1)
			{
				player1_board[position_x-i][position_y].content = 1;
				player1_board_aux[position_x-i][position_y].id = 4;
				//draw_square(initial_position_x-(i*player1_square_width),initial_position_y,player1_square_width,player1_square_height,0);
			}
			else
				if(orientation == 2)
				{
					player1_board[position_x][position_y+i].content = 1;
					player1_board_aux[position_x][position_y+i].id = 4;
					//draw_square(initial_position_x,initial_position_y+(i*player1_square_width),player1_square_width,player1_square_height,0);
				}
				else
					if(orientation == 3)
					{
						player1_board[position_x+i][position_y].content = 1;
						player1_board_aux[position_x+i][position_y].id = 4;
						//draw_square(initial_position_x+(i*player1_square_width),initial_position_y,player1_square_width,player1_square_height,0);
					}
	}

	//place the 5-square ship

	orientation = rand() %4 ; // 0-up // 1-left // 2-down // 3-right
	position_x = rand() % 10;
	position_y = rand() % 10;

	final_position_x = 100;
	final_position_y = 100;

	initial_position_x = player1_board_xposition+(position_x*player1_square_width);
	initial_position_y = player1_board_yposition+(position_y*player1_square_height);

	//Check if the initial position is greater or inferior than the table borders or if the possible positions of
	//the ship are already occupied by some other ship
	while(final_position_x > (player1_board_xposition + (player1_square_width*9)) || final_position_x < player1_board_xposition
			|| final_position_y > player1_board_yposition + (player1_square_height*9) || final_position_y < player1_board_yposition
			|| player1_board[position_x][position_y].content == 1 || player1_board[position_x+1][position_y].content == 1 ||
			player1_board[position_x-1][position_y].content == 1 || player1_board[position_x][position_y+1].content == 1 ||
			player1_board[position_x][position_y-1].content == 1 || player1_board[position_x+2][position_y].content == 1 ||
			player1_board[position_x-2][position_y].content == 1 || player1_board[position_x][position_y+2].content == 1 ||
			player1_board[position_x][position_y-2].content == 1 || player1_board[position_x+3][position_y].content == 1 ||
			player1_board[position_x-3][position_y].content == 1 || player1_board[position_x][position_y+3].content == 1 ||
			player1_board[position_x][position_y-3].content == 1 || player1_board[position_x+4][position_y].content == 1 ||
			player1_board[position_x-4][position_y].content == 1 || player1_board[position_x][position_y+4].content == 1 ||
			player1_board[position_x][position_y-4].content == 1)
	{
		position_x = rand() % 10;
		position_y = rand() % 10;
		orientation = rand() %4;

		initial_position_x = player1_board_xposition+(position_x*player1_square_width);
		initial_position_y = player1_board_yposition+(position_y*player1_square_height);

		if(orientation == 0)
		{
			final_position_x = initial_position_x;
			final_position_y = initial_position_y - 5*player1_square_height;
		}else
			if(orientation == 1)
			{
				final_position_x = initial_position_x - 5*player1_square_width;
				final_position_y = initial_position_y;
			}else
				if(orientation == 2)
				{
					final_position_x = initial_position_x;
					final_position_y = initial_position_y  + 5*player1_square_height;
				}else
				{
					final_position_x = initial_position_x + 5*player1_square_width;
					final_position_y = initial_position_y;
				}
	}

	for(i=0;i<5;i++)
	{
		if(i==0)
		{
			player1_board[position_x][position_y].content = 1;
			player1_board_aux[position_x][position_y].id = 5;
			//draw_square(initial_position_x,initial_position_y,player1_square_width,player1_square_height,0);
		}

		if(orientation == 0)
		{
			player1_board[position_x][position_y-i].content = 1;
			player1_board_aux[position_x][position_y-i].id = 5;
			//draw_square(initial_position_x,initial_position_y-(i*player1_square_height),player1_square_width,player1_square_height,0);
		}
		else
			if(orientation == 1)
			{
				player1_board[position_x-i][position_y].content = 1;
				player1_board_aux[position_x-i][position_y].id = 5;
				//draw_square(initial_position_x-(i*player1_square_width),initial_position_y,player1_square_width,player1_square_height,0);
			}
			else
				if(orientation == 2)
				{
					player1_board[position_x][position_y+i].content = 1;
					player1_board_aux[position_x][position_y+i].id = 5;
					//draw_square(initial_position_x,initial_position_y+(i*player1_square_width),player1_square_width,player1_square_height,0);
				}
				else
					if(orientation == 3)
					{
						player1_board[position_x+i][position_y].content = 1;
						player1_board_aux[position_x+i][position_y].id = 5;
						//draw_square(initial_position_x+(i*player1_square_width),initial_position_y,player1_square_width,player1_square_height,0);
					}
	}

}

//Function to randomly place the player1 ships in the player1 table
//It updates the information of the player1_table with 0's and 1's corresponding 0 to water and 1 to a ship
//It updates the player1_table_aux with the ships ID as it puts the ships in the table
void place_player1_ships()
{
	//calculate random coordinates and orientation for the ship to be placed
	int orientation = rand() %4 ; // 0-up // 1-left // 2-down // 3-right
	int position_x = rand() % 10;
	int position_y = rand() % 10;

	//used to control if the ship final position is greater or inferior than the table borders
	int final_position_x = 0;
	int final_position_y = 0;

	int i;

	//Calculate the initial position of the boat in pixels
	int initial_position_x = player2_board_xposition+(position_x*player2_square_width);
	int initial_position_y = player2_board_yposition+(position_y*player2_square_height);

	//Check if the initial position is greater or inferior than the table borders or if the possible positions of
	//the ship are already occupied by some other ship
	while(initial_position_x + 2*player2_square_width > (player2_board_xposition + (player2_square_width*9)) || initial_position_y + 2*player2_square_height > player2_board_yposition + (player2_square_height*9) ||initial_position_x - 2*player2_square_width < player2_board_xposition ||initial_position_y - 2*player2_square_height < player2_board_yposition)
	{
		//calculate new coordinates and orientation
		position_x = rand() % 10;
		position_y = rand() % 10;
		orientation = rand() %4;

		//variables recalculated to check in the while condition
		initial_position_x = player2_board_xposition+(position_x*player2_square_width);
		initial_position_y = player2_board_yposition+(position_y*player2_square_height);
	}

	//place the 1st 2-square ship
	for(i=0;i<2;i++)
	{
		if(i==0)
		{
			player2_board[position_x][position_y].content = 1;
			player2_board_aux[position_x][position_y].id = 1;
			draw_square(initial_position_x,initial_position_y,player2_square_width,player2_square_height,player_ship_color);
		}

		if(orientation == 0)
		{
			player2_board[position_x][position_y-i].content = 1;
			player2_board_aux[position_x][position_y-i].id = 1;
			draw_square(initial_position_x,initial_position_y-(i*player2_square_height),player2_square_width,player2_square_height,player_ship_color);
		}
		else
			if(orientation == 1)
			{
				player2_board[position_x-i][position_y].content = 1;
				player2_board_aux[position_x-i][position_y].id = 1;
				draw_square(initial_position_x-(i*player2_square_width),initial_position_y,player2_square_width,player2_square_height,player_ship_color);
			}
			else
				if(orientation == 2)
				{
					player2_board[position_x][position_y+i].content = 1;
					player2_board_aux[position_x][position_y+i].id = 1;
					draw_square(initial_position_x,initial_position_y+(i*player2_square_width),player2_square_width,player2_square_height,player_ship_color);
				}
				else
					if(orientation == 3)
					{
						player2_board[position_x+i][position_y].content = 1;
						player2_board_aux[position_x+i][position_y].id = 1;
						draw_square(initial_position_x+(i*player2_square_width),initial_position_y,player2_square_width,player2_square_height,player_ship_color);
					}
	}


	//place the 1st 3-square ship
	orientation = rand() %4 ; // 0-up // 1-left // 2-down // 3-right
	position_x = rand() % 10;
	position_y = rand() % 10;

	initial_position_x = player2_board_xposition+(position_x*player2_square_width);
	initial_position_y = player2_board_yposition+(position_y*player2_square_height);

	//Check if the initial position is greater or inferior than the table borders or if the possible positions of
	//the ship are already occupied by some other ship
	while(initial_position_x + 3*player2_square_width > (player2_board_xposition + (player2_square_width*9)) || initial_position_y + 3*player2_square_height > player2_board_yposition + (player2_square_height*9) ||initial_position_x - 3*player2_square_width < player2_board_xposition ||initial_position_y - 3*player2_square_height < player2_board_yposition
			|| player2_board[position_x][position_y].content == 1 || player2_board[position_x+1][position_y].content == 1 ||
			player2_board[position_x-1][position_y].content == 1 || player2_board[position_x][position_y+1].content == 1 ||
			player2_board[position_x][position_y-1].content == 1 || player2_board[position_x+2][position_y].content == 1 ||
			player2_board[position_x-2][position_y].content == 1 || player2_board[position_x][position_y+2].content == 1 ||
			player2_board[position_x][position_y-2].content == 1 )
	{
		position_x = rand() % 10;
		position_y = rand() % 10;
		orientation = rand() %4;
		initial_position_x = player2_board_xposition+(position_x*player2_square_width);
		initial_position_y = player2_board_yposition+(position_y*player2_square_height);
	}

	for(i=0;i<3;i++)
	{
		if(i==0)
		{
			player2_board[position_x][position_y].content = 1;
			player2_board_aux[position_x][position_y].id = 2;
			draw_square(initial_position_x,initial_position_y,player2_square_width,player2_square_height,player_ship_color);
		}

		if(orientation == 0)
		{
			player2_board[position_x][position_y-i].content = 1;
			player2_board_aux[position_x][position_y-i].id = 2;
			draw_square(initial_position_x,initial_position_y-(i*player2_square_height),player2_square_width,player2_square_height,player_ship_color);
		}
		else
			if(orientation == 1)
			{
				player2_board[position_x-i][position_y].content = 1;
				player2_board_aux[position_x-i][position_y].id = 2;
				draw_square(initial_position_x-(i*player2_square_width),initial_position_y,player2_square_width,player2_square_height,player_ship_color);
			}
			else
				if(orientation == 2)
				{
					player2_board[position_x][position_y+i].content = 1;
					player2_board_aux[position_x][position_y+i].id = 2;
					draw_square(initial_position_x,initial_position_y+(i*player2_square_width),player2_square_width,player2_square_height,player_ship_color);
				}
				else
					if(orientation == 3)
					{
						player2_board[position_x+i][position_y].content = 1;
						player2_board_aux[position_x+i][position_y].id = 2;
						draw_square(initial_position_x+(i*player2_square_width),initial_position_y,player2_square_width,player2_square_height,player_ship_color);
					}
	}


	//place the 2nd 3-square ship

	orientation = rand() %4 ; // 0-up // 1-left // 2-down // 3-right
	position_x = rand() % 10;
	position_y = rand() % 10;

	initial_position_x = player2_board_xposition+(position_x*player2_square_width);
	initial_position_y = player2_board_yposition+(position_y*player2_square_height);

	//Check if the initial position is greater or inferior than the table borders or if the possible positions of
	//the ship are already occupied by some other ship
	while(final_position_x > (player2_board_xposition + (player2_square_width*9)) || final_position_x < player2_board_xposition
			|| final_position_y > player2_board_yposition + (player2_square_height*9) || final_position_y < player2_board_yposition
			|| player2_board[position_x][position_y].content == 1 || player2_board[position_x+1][position_y].content == 1 ||
			player2_board[position_x-1][position_y].content == 1 || player2_board[position_x][position_y+1].content == 1 ||
			player2_board[position_x][position_y-1].content == 1 || player2_board[position_x+2][position_y].content == 1 ||
			player2_board[position_x-2][position_y].content == 1 || player2_board[position_x][position_y+2].content == 1 ||
			player2_board[position_x][position_y-2].content == 1 )
	{
		position_x = rand() % 10;
		position_y = rand() % 10;
		orientation = rand() %4;

		initial_position_x = player2_board_xposition+(position_x*player2_square_width);
		initial_position_y = player2_board_yposition+(position_y*player2_square_height);

		if(orientation == 0)
		{
			final_position_x = initial_position_x;
			final_position_y = initial_position_y - 3*player2_square_height;
		}else
			if(orientation == 1)
			{
				final_position_x = initial_position_x - 3*player2_square_width;
				final_position_y = initial_position_y;
			}else
				if(orientation == 2)
				{
					final_position_x = initial_position_x;
					final_position_y = initial_position_y  + 3*player2_square_height;
				}else
				{
					final_position_x = initial_position_x + 3*player2_square_width;
					final_position_y = initial_position_y;
				}
	}



	for(i=0;i<3;i++)
	{
		if(i==0)
		{
			player2_board[position_x][position_y].content = 1;
			player2_board_aux[position_x][position_y].id = 3;
			draw_square(initial_position_x,initial_position_y,player2_square_width,player2_square_height,player_ship_color);
		}

		if(orientation == 0)
		{
			player2_board[position_x][position_y-i].content = 1;
			player2_board_aux[position_x][position_y-i].id = 3;
			draw_square(initial_position_x,initial_position_y-(i*player2_square_height),player2_square_width,player2_square_height,player_ship_color);
		}
		else
			if(orientation == 1)
			{
				player2_board[position_x-i][position_y].content = 1;
				player2_board_aux[position_x-i][position_y].id = 3;
				draw_square(initial_position_x-(i*player2_square_width),initial_position_y,player2_square_width,player2_square_height,player_ship_color);
			}
			else
				if(orientation == 2)
				{
					player2_board[position_x][position_y+i].content = 1;
					player2_board_aux[position_x][position_y+i].id = 3;
					draw_square(initial_position_x,initial_position_y+(i*player2_square_width),player2_square_width,player2_square_height,player_ship_color);
				}
				else
					if(orientation == 3)
					{
						player2_board[position_x+i][position_y].content = 1;
						player2_board_aux[position_x+i][position_y].id = 3;
						draw_square(initial_position_x+(i*player2_square_width),initial_position_y,player2_square_width,player2_square_height,player_ship_color);
					}
	}
	//place the 4-square ship

	orientation = rand() %4 ; // 0-up // 1-left // 2-down // 3-right
	position_x = rand() % 10;
	position_y = rand() % 10;

	final_position_x = 100;
	final_position_y = 100;

	initial_position_x = player2_board_xposition+(position_x*player2_square_width);
	initial_position_y = player2_board_yposition+(position_y*player2_square_height);

	//Check if the initial position is greater or inferior than the table borders or if the possible positions of
	//the ship are already occupied by some other ship
	while(final_position_x > (player2_board_xposition + (player2_square_width*9)) || final_position_x < player2_board_xposition
			|| final_position_y > player2_board_yposition + (player2_square_height*9) || final_position_y < player2_board_yposition
			|| player2_board[position_x][position_y].content == 1 || player2_board[position_x+1][position_y].content == 1 ||
			player2_board[position_x-1][position_y].content == 1 || player2_board[position_x][position_y+1].content == 1 ||
			player2_board[position_x][position_y-1].content == 1 || player2_board[position_x+2][position_y].content == 1 ||
			player2_board[position_x-2][position_y].content == 1 || player2_board[position_x][position_y+2].content == 1 ||
			player2_board[position_x][position_y-2].content == 1 || player2_board[position_x+3][position_y].content == 1 ||
			player2_board[position_x-3][position_y].content == 1 || player2_board[position_x][position_y+3].content == 1 ||
			player2_board[position_x][position_y-3].content == 1)
	{
		position_x = rand() % 10;
		position_y = rand() % 10;
		orientation = rand() %4;

		initial_position_x = player2_board_xposition+(position_x*player2_square_width);
		initial_position_y = player2_board_yposition+(position_y*player2_square_height);

		if(orientation == 0)
		{
			final_position_x = initial_position_x;
			final_position_y = initial_position_y - 4*player2_square_height;
		}else
			if(orientation == 1)
			{
				final_position_x = initial_position_x - 4*player2_square_width;
				final_position_y = initial_position_y;
			}else
				if(orientation == 2)
				{
					final_position_x = initial_position_x;
					final_position_y = initial_position_y  + 4*player2_square_height;
				}else
				{
					final_position_x = initial_position_x + 4*player2_square_width;
					final_position_y = initial_position_y;
				}
	}



	for(i=0;i<4;i++)
	{
		if(i==0)
		{
			player2_board[position_x][position_y].content = 1;
			player2_board_aux[position_x][position_y].id = 4;
			draw_square(initial_position_x,initial_position_y,player2_square_width,player2_square_height,0);
		}

		if(orientation == 0)
		{
			player2_board[position_x][position_y-i].content = 1;
			player2_board_aux[position_x][position_y-i].id = 4;
			draw_square(initial_position_x,initial_position_y-(i*player2_square_height),player2_square_width,player2_square_height,player_ship_color);
		}
		else
			if(orientation == 1)
			{
				player2_board[position_x-i][position_y].content = 1;
				player2_board_aux[position_x-i][position_y].id = 4;
				draw_square(initial_position_x-(i*player2_square_width),initial_position_y,player2_square_width,player2_square_height,player_ship_color);
			}
			else
				if(orientation == 2)
				{
					player2_board[position_x][position_y+i].content = 1;
					player2_board_aux[position_x][position_y+i].id = 4;
					draw_square(initial_position_x,initial_position_y+(i*player2_square_width),player2_square_width,player2_square_height,player_ship_color);
				}
				else
					if(orientation == 3)
					{
						player2_board[position_x+i][position_y].content = 1;
						player2_board_aux[position_x+i][position_y].id = 4;
						draw_square(initial_position_x+(i*player2_square_width),initial_position_y,player2_square_width,player2_square_height,player_ship_color);
					}
	}

	//place the 5-square ship

	orientation = rand() %4 ; // 0-up // 1-left // 2-down // 3-right
	position_x = rand() % 10;
	position_y = rand() % 10;

	final_position_x = 100;
	final_position_y = 100;

	initial_position_x = player2_board_xposition+(position_x*player2_square_width);
	initial_position_y = player2_board_yposition+(position_y*player2_square_height);

	//Check if the initial position is greater or inferior than the table borders or if the possible positions of
	//the ship are already occupied by some other ship
	while(final_position_x > (player2_board_xposition + (player2_square_width*9)) || final_position_x < player2_board_xposition
			|| final_position_y > player2_board_yposition + (player2_square_height*9) || final_position_y < player2_board_yposition
			|| player2_board[position_x][position_y].content == 1 || player2_board[position_x+1][position_y].content == 1 ||
			player2_board[position_x-1][position_y].content == 1 || player2_board[position_x][position_y+1].content == 1 ||
			player2_board[position_x][position_y-1].content == 1 || player2_board[position_x+2][position_y].content == 1 ||
			player2_board[position_x-2][position_y].content == 1 || player2_board[position_x][position_y+2].content == 1 ||
			player2_board[position_x][position_y-2].content == 1 || player2_board[position_x+3][position_y].content == 1 ||
			player2_board[position_x-3][position_y].content == 1 || player2_board[position_x][position_y+3].content == 1 ||
			player2_board[position_x][position_y-3].content == 1 || player2_board[position_x+4][position_y].content == 1 ||
			player2_board[position_x-4][position_y].content == 1 || player2_board[position_x][position_y+4].content == 1 ||
			player2_board[position_x][position_y-4].content == 1)
	{
		position_x = rand() % 10;
		position_y = rand() % 10;
		orientation = rand() %4;

		initial_position_x = player2_board_xposition+(position_x*player2_square_width);
		initial_position_y = player2_board_yposition+(position_y*player2_square_height);

		if(orientation == 0)
		{
			final_position_x = initial_position_x;
			final_position_y = initial_position_y - 5*player2_square_height;
		}else
			if(orientation == 1)
			{
				final_position_x = initial_position_x - 5*player2_square_width;
				final_position_y = initial_position_y;
			}else
				if(orientation == 2)
				{
					final_position_x = initial_position_x;
					final_position_y = initial_position_y  + 5*player2_square_height;
				}else
				{
					final_position_x = initial_position_x + 5*player2_square_width;
					final_position_y = initial_position_y;
				}
	}

	for(i=0;i<5;i++)
	{
		if(i==0)
		{
			player2_board[position_x][position_y].content = 1;
			player2_board_aux[position_x][position_y].id = 5;
			draw_square(initial_position_x,initial_position_y,player2_square_width,player2_square_height,0);
		}

		if(orientation == 0)
		{
			player2_board[position_x][position_y-i].content = 1;
			player2_board_aux[position_x][position_y-i].id = 5;
			draw_square(initial_position_x,initial_position_y-(i*player2_square_height),player2_square_width,player2_square_height,player_ship_color);
		}
		else
			if(orientation == 1)
			{
				player2_board[position_x-i][position_y].content = 1;
				player2_board_aux[position_x-i][position_y].id = 5;
				draw_square(initial_position_x-(i*player2_square_width),initial_position_y,player2_square_width,player2_square_height,player_ship_color);
			}
			else
				if(orientation == 2)
				{
					player2_board[position_x][position_y+i].content = 1;
					player2_board_aux[position_x][position_y+i].id = 5;
					draw_square(initial_position_x,initial_position_y+(i*player2_square_width),player2_square_width,player2_square_height,player_ship_color);
				}
				else
					if(orientation == 3)
					{
						player2_board[position_x+i][position_y].content = 1;
						player2_board_aux[position_x+i][position_y].id = 5;
						draw_square(initial_position_x+(i*player2_square_width),initial_position_y,player2_square_width,player2_square_height,player_ship_color);
					}
	}

}

//moves the virtual keyboard cursor of the player2 table by redrawing the outline in the position desired
//and making the structure changes need to refresh the x and y saved
//this functions receives a "value" argument that indicates in witch position to move the cursor
//1-up || 2-left || 3-down || 4-right
void move_keyboard_cursor(int value)
{
	if(menu_flag == 1)
	{
		switch(value)
		{

		case 1: //move the keyboard cursor up
			if(keyboard_cursor.y - player1_square_height >= player1_board_yposition)
			{
				draw_outline(keyboard_cursor.x,keyboard_cursor.y,player1_square_width,player1_square_height,0);
				keyboard_cursor.y -= player1_square_height;
				keyboard_cursor.row -= 1;
				draw_outline(keyboard_cursor.x,keyboard_cursor.y,player1_square_width,player1_square_height,07);
			}
			break;
		case 2://move the keyboard cursor left
			if(keyboard_cursor.x - player1_square_width >= player1_board_xposition)
			{
				draw_outline(keyboard_cursor.x,keyboard_cursor.y,player1_square_width,player1_square_height,0);
				keyboard_cursor.x -= player1_square_width;
				keyboard_cursor.column -= 1;
				draw_outline(keyboard_cursor.x,keyboard_cursor.y,player1_square_width,player1_square_height,07);
			}
			break;
		case 3://move the keyboard cursor down
			if(keyboard_cursor.y + player1_square_height <= (player1_board_yposition + (player1_square_height*9)))
			{
				draw_outline(keyboard_cursor.x,keyboard_cursor.y,player1_square_width,player1_square_height,0);
				keyboard_cursor.y += player1_square_width;
				keyboard_cursor.row += 1;
				draw_outline(keyboard_cursor.x,keyboard_cursor.y,player1_square_width,player1_square_height,07);
			}
			break;
		case 4:	//move the keyboard cursor right
			if(keyboard_cursor.x + player1_square_width <= (player1_board_xposition + (player1_square_width*9)))
			{
				draw_outline(keyboard_cursor.x,keyboard_cursor.y,player1_square_width,player1_square_height,0);
				keyboard_cursor.x += player1_square_width;
				keyboard_cursor.column+=1;
				draw_outline(keyboard_cursor.x,keyboard_cursor.y,player1_square_width,player1_square_height,07);
			}
			break;
		default:
			break;

		}
	}else
	{
		switch(value)
		{
		case 1: //move the keyboard cursor up
			if(keyboard_cursor.y == 400)
			{
				draw_outline(keyboard_cursor.x,keyboard_cursor.y,340,110,background_color);
				keyboard_cursor.y -= 110;
				draw_outline(keyboard_cursor.x,keyboard_cursor.y,340,110,0);
			}
			break;
		case 3://move the keyboard cursor down
			if(keyboard_cursor.y == 290)
			{
				draw_outline(keyboard_cursor.x,keyboard_cursor.y,340,110,background_color);
				keyboard_cursor.y += 110;
				draw_outline(keyboard_cursor.x,keyboard_cursor.y,340,110,0);
			}
		}
	}
}

//it checks if any of the AI ships has been destroyed and if so draws the ship sprite next to the table as red
void check_player2_destroyed_ships()
{
	int i;
	for(i=0;i<5;i++)
	{
		if(ships1[i] == 0)
		{
			switch(i)
			{
			case 0:	draw_square(360,400,70,35,ship_destroyed_color);
			break;
			case 1:	draw_square(325,447,105,35,ship_destroyed_color);
			break;
			case 2:	draw_square(325,494,105,35,ship_destroyed_color);
			break;
			case 3:	draw_square(290,541,140,35,ship_destroyed_color);
			break;
			case 4:	draw_square(255,588,175,35,ship_destroyed_color);
			break;
			}
		}else
		{
			switch(i)
			{
			case 0:	draw_square(360,400,70,35,player_ship_color);
			break;
			case 1:	draw_square(325,447,105,35,player_ship_color);
			break;
			case 2:	draw_square(325,494,105,35,player_ship_color);
			break;
			case 3:	draw_square(290,541,140,35,player_ship_color);
			break;
			case 4:	draw_square(255,588,175,35,player_ship_color);
			break;
			}
		}
	}
}

//it checks if any of the player1 ships has been destroyed and if so draws the ship sprite next to the table as red
void check_player1_destroyed_ships()
{
	int i;
	for(i=0;i<5;i++)
	{
		if(ships2[i] == 0)
		{
			switch(i)
			{
			case 0:	draw_square(500,100,56,28,ship_destroyed_color);
			break;
			case 1:	draw_square(500,140,84,28,ship_destroyed_color);
			break;
			case 2:	draw_square(500,180,84,28,ship_destroyed_color);
			break;
			case 3:	draw_square(500,220,112,28,ship_destroyed_color);
			break;
			case 4:	draw_square(500,260,140,28,ship_destroyed_color);
			break;
			}
		}else
		{
			switch(i)
			{
			case 0:	draw_square(500,100,56,28,player_ship_color);
			break;
			case 1:	draw_square(500,140,84,28,player_ship_color);
			break;
			case 2:	draw_square(500,180,84,28,player_ship_color);
			break;
			case 3:	draw_square(500,220,112,28,player_ship_color);
			break;
			case 4:	draw_square(500,260,140,28,player_ship_color);
			break;
			}
		}
	}
}

//indicates that the volley special attack was made by the player1 - number 1 or 2 key pressed
void set_volley_flag()
{
	volley_flag = 1;
}

//indicates that the scout special attack was made by the player1 - number 3 key pressed
void set_scout_flag()
{
	scout_flag = 1;
}

//indicates that the revive special attack was made by the player1 - number 3 key pressed
void set_bombardement_flag()
{
	bombardement_flag = 1;
}

//indicates that the volley special attack was made by the AI
void set_AI_volley_flag()
{
	AI_volley_flag = 1;
}

//indicates that the scout special attack was made by the AI
void set_AI_scout_flag()
{
	AI_scout_flag = 1;
}

//indicates that the revive special attack was made by the AI
void set_AI_bombardement_flag()
{
	AI_bombardement_flag = 1;
}

//checks the square where the virtual keyboard cursor is located after receiving a "enter" scancode
void check_square_keyboard()
{
	int row,column;
	int i;

	row = keyboard_cursor.row;
	column = keyboard_cursor.column;

	//terminate the player1 turn by ending the interrupt_handler condition
	number_of_tries = 1;

	if(menu_flag == 0)
	{
		if(keyboard_cursor.y == 290)
			play_flag = 1;
		else
			exit_flag = 1;

		//the first time a enter is pressed it means it has leaved the menu
		menu_flag = 1;
	}
	else{

		if(player1_board[column][row].checked == 1)
		{
			//if the square has already been checked,make the player repeat the move
			number_of_tries = 0;
		}else
			if(player1_board[column][row].content == 1) //it has found a piece of a ship
			{
				//check the matrix position to guarantee that it isnt checked again
				player1_board[column][row].checked = 1;

				//sum the player1 total of hits by 1
				player1_hits += 1;

				//Create the sprites
				Sprite *explosion1= create_sprite(explosion_1,0);
				Sprite *explosion2 = create_sprite(explosion_2,0);
				Sprite *explosion3 = create_sprite(explosion_3,0);
				Sprite *red_cross = create_sprite(redcross,0);

				//run an explosion sound

				//Draw the animated sprite of a explosion
				draw_sprite(player1_board_xposition+(column*player1_square_width),player1_board_yposition+(row*player1_square_height),explosion1);
				tickdelay(micros_to_ticks(100000));
				draw_sprite(player1_board_xposition+(column*player1_square_width),player1_board_yposition+(row*player1_square_height),explosion2);
				tickdelay(micros_to_ticks(100000));
				draw_sprite(player1_board_xposition+(column*player1_square_width),player1_board_yposition+(row*player1_square_height),explosion3);
				tickdelay(micros_to_ticks(100000));
				draw_sprite(player1_board_xposition+(column*player1_square_width),player1_board_yposition+(row*player1_square_height),red_cross);
				//draw_outline(keyboard_cursor.x,keyboard_cursor.y,player1_square_width,player1_square_height,0);


				//check the ships1 vector for the id of the ship that has been hit and update that information
				ships1[(player1_board_aux[column][row].id)-1] -= 1;
				draw_square(player1_board_xposition+(column*player1_square_width),player1_board_yposition+(row*player1_square_height),player1_square_width,player1_square_height,ship_hit);

				//search in the ships1 vector if any ship has been destroyed
				for(i=0;i<5;i++)
				{
					if(ships1[i] == 0)
					{
						switch(i)
						{
						case 0:	draw_square(360,400,70,35,ship_destroyed_color);
								break;
						case 1:	draw_square(325,447,105,35,ship_destroyed_color);
								break;
						case 2:	draw_square(325,494,105,35,ship_destroyed_color);
								break;
						case 3:	draw_square(290,541,140,35,ship_destroyed_color);
								break;
						case 4:	draw_square(255,588,175,35,ship_destroyed_color);
								break;
						}
					}
				}


			}else //it has found water
			{
				//check the matrix position to guarantee that it isnt checked again
				player1_board[column][row].checked = 1;

				//Create the sprites
				Sprite *splash1= create_sprite(splash_1,0);
				Sprite *splash2 = create_sprite(splash_2,0);
				Sprite *splash3 = create_sprite(splash_3,0);
				Sprite *blue_cross = create_sprite(bluecross,0);

				//run an miss target sound


				//Draw the animated sprite of a miss
				draw_sprite(player1_board_xposition+(column*player1_square_width),player1_board_yposition+(row*player1_square_height),splash1);
				tickdelay(micros_to_ticks(100000));
				draw_sprite(player1_board_xposition+(column*player1_square_width),player1_board_yposition+(row*player1_square_height),splash2);
				tickdelay(micros_to_ticks(100000));
				draw_sprite(player1_board_xposition+(column*player1_square_width),player1_board_yposition+(row*player1_square_height),splash3);
				tickdelay(micros_to_ticks(100000));
				draw_sprite(player1_board_xposition+(column*player1_square_width),player1_board_yposition+(row*player1_square_height),blue_cross);
				draw_outline(keyboard_cursor.x,keyboard_cursor.y,player1_square_width,player1_square_height,0);

				draw_square(player1_board_xposition+(column*player1_square_width),player1_board_yposition+(row*player1_square_height),player1_square_width,player1_square_height,ship_miss);

			}
	}

}

//To use as the interrupt_handler condition terminator
int check_number_of_tries()
{
	return number_of_tries;
}

//initializes the AI_shot variable
void initialize_AI_shot()
{
	 ai_shot.ship_found= 0;
	 ai_shot.previous_row = 0;
	 ai_shot.previous_column = 0;
}

//AI engine
void AI_turn()
{
	int i,j;
	int random_x_factor = rand()%2;
	int random_y_factor = rand()%2;

	//resets the interrupt handler variable so that the player1 can play again
	if(player1_hits < 17 && AI_hits < 17)
		number_of_tries = 0;
	else
		number_of_tries = 1;

	//generates a random position to hit
	int column = rand() % 10;
	int row = rand() % 10;

	//if the generated position is not valid,make another one until it is
	while(player2_board[column][row].checked == 1)
	{
		column = rand() % 10;
		row = rand() % 10;
	}

	//AI method 1
	//when the AI hits a ship it searches the entire map for the same id as the piece of the ship that was hit until the ship is destroyed
	if(ships2[(player2_board_aux[ai_shot.previous_column][ai_shot.previous_row].id)-1] == 0)
		initialize_AI_shot(); //if the ship that was initially found is destroyed resets the AI_shot variable
	else
	{
		for(j=0;j<10;j++)
			for(i=0;i<10;i++)
			{
				if((player2_board_aux[i][j].id == player2_board_aux[ai_shot.previous_column][ai_shot.previous_row].id) && player2_board[i][j].checked == 0)
				{
					/*while(player2_board[i+random_x_factor][j+random_y_factor].checked == 1 || i+random_x_factor > 9 || j+random_y_factor > 9)
					{
						random_x_factor = rand()%2;
						random_y_factor = rand()%2;
					}*/

					if((player2_board_aux[i+random_x_factor][j+random_y_factor].id == player2_board_aux[ai_shot.previous_column][ai_shot.previous_row].id))
						{
							ai_shot.previous_row = j; //+random_y_factor;
							ai_shot.previous_column = i; //+random_x_factor;
						}

					row = j;// + random_y_factor;
					column = i;// + random_x_factor;


					break;
				}
			}
	}


	//AI method 2
	//when the AI hits a ship it searches for other ships pieces in the cardinal positions unti the ship has been destroyed
	//it has a bug when it hits the ship in a middle piece
		//it indicates if the ships that was found has already been destroyed
		/*if(ships2[(player2_board_aux[ai_shot.previous_column][ai_shot.previous_row].id)-1] == 0)
			initialize_AI_shot();
		else
		{
			//it verifies for all of the cardinal positions where is the next ship piece

			//the next ship piece is east
			if((((player2_board_aux[ai_shot.previous_column+1][ai_shot.previous_row].id)) == ((player2_board_aux[ai_shot.previous_column][ai_shot.previous_row].id))))
				ai_shot.previous_column +=1;
			else
				//the next ship piece is south
				if((((player2_board_aux[ai_shot.previous_column][ai_shot.previous_row+1].id)) == ((player2_board_aux[ai_shot.previous_column][ai_shot.previous_row].id))))
					ai_shot.previous_row += 1;
				else
					//the next ship piece is west
					if((((player2_board_aux[ai_shot.previous_column-1][ai_shot.previous_row].id)) == ((player2_board_aux[ai_shot.previous_column][ai_shot.previous_row].id))))
						ai_shot.previous_column -=1;
					else
						//the next ship piece is north
						if( (((player2_board_aux[ai_shot.previous_column][ai_shot.previous_row-1].id)) == ((player2_board_aux[ai_shot.previous_column][ai_shot.previous_row].id))))
							ai_shot.previous_row -= 1;

			row = ai_shot.previous_row;
			column = ai_shot.previous_column;

		}*/

	if(player2_board[column][row].content == 1)
	{
		//check the matrix position to guarantee that it isnt checked again
		player2_board[column][row].checked = 1;

		//sum the AI total of hits by 1
		AI_hits += 1;

		//run an explosion sound

		//Create the sprites
		Sprite *explosion1= create_sprite(explosion_1_27,0);
		Sprite *explosion2 = create_sprite(explosion_2_27,0);
		Sprite *explosion3 = create_sprite(explosion_3_27,0);
		Sprite *red_cross = create_sprite(redcross_27,0);

		//run an explosion sound

		//Draw the animated sprite of a explosion
		draw_sprite(player2_board_xposition+(column*player2_square_width),player2_board_yposition+(row*player2_square_height),explosion1);
		tickdelay(micros_to_ticks(100000));
		draw_sprite(player2_board_xposition+(column*player2_square_width),player2_board_yposition+(row*player2_square_height),explosion2);
		tickdelay(micros_to_ticks(100000));
		draw_sprite(player2_board_xposition+(column*player2_square_width),player2_board_yposition+(row*player2_square_height),explosion3);
		tickdelay(micros_to_ticks(100000));
		draw_sprite(player2_board_xposition+(column*player2_square_width),player2_board_yposition+(row*player2_square_height),red_cross);
		draw_square(player2_board_xposition+(column*player2_square_width),player2_board_yposition+(row*player2_square_height),player2_square_width,player2_square_height,ship_hit);

		//check the ships2 vector for the id of the ship that has been hit and update that information
		ships2[(player2_board_aux[column][row].id)-1] -= 1;

		//search in the ships2 vector if any ship has been destroyed - element of the vector is 0
		check_player1_destroyed_ships();

		//check the ship_found flag to know that the ai has found a ship
		//refresh the row and column of the last hit
		if(ai_shot.ship_found == 0)
		{
			ai_shot.ship_found = 1;
			ai_shot.previous_row = row;
			ai_shot.previous_column = column;
		}
	}else
	{
		//desenhar um remoinho
		player2_board[column][row].checked = 1;
		//Create the sprites
		Sprite *splash1= create_sprite(splash_1_27,0);
		Sprite *splash2 = create_sprite(splash_2_27,0);
		Sprite *splash3 = create_sprite(splash_3_27,0);
		Sprite *blue_cross = create_sprite(bluecross_27,0);

		//run an miss target sound


		//Draw the animated sprite of a miss
		draw_sprite(player2_board_xposition+(column*player2_square_width),player2_board_yposition+(row*player2_square_height),splash1);
		tickdelay(micros_to_ticks(100000));
		draw_sprite(player2_board_xposition+(column*player2_square_width),player2_board_yposition+(row*player2_square_height),splash2);
		tickdelay(micros_to_ticks(100000));
		draw_sprite(player2_board_xposition+(column*player2_square_width),player2_board_yposition+(row*player2_square_height),splash3);
		tickdelay(micros_to_ticks(100000));
		draw_sprite(player2_board_xposition+(column*player2_square_width),player2_board_yposition+(row*player2_square_height),blue_cross);
		draw_square(player2_board_xposition+(column*player2_square_width),player2_board_yposition+(row*player2_square_height),player2_square_width,player2_square_height,ship_miss);
		//som de miss
	}
}

//it calls the the functions to draw the player1 and AI table
void draw_tables()
{
	vg_fill(background_color);
	draw_player2_board();
	draw_player1_board();
	draw_player1_board_outline();
	draw_player2_board_outline();
}

//draws all of the graphical interface of the game by calling other functions
int draw_graphical_interface()
{
	menu();
	initialize_keyboard_cursor_for_menu();
	interupt_handler(60);
	if(play_flag == 1)
	{
		erase_screen();
		//draw_background();
		vg_fill(35);
		draw_tables();
		draw_letters_numbers();
		initialize_keyboard_cursor();
		draw_players();
		draw_ships();
		place_AI_ships();
		place_player1_ships();
		return 1;
	}
	else
	{
		return 0;
	}
}

//it starts the interrupt handler loop
void player1_turn()
{
	interupt_handler(20);
}

//1st special attack
//it hits all of the squares in a randomly generated column or row
void AI_volley()
{
	//checks if the volley special attack was already used by the AI
	if(AI_volley_flag==1)
		AI_volley_flag = 1;
	else
	{
		//resets the interrupt handler variable so that the player1 can play again
		number_of_tries = 0;

		//sets the AI_volley_flag so the AI cant use the special attack again
		set_AI_volley_flag();

		//it calculates a random position and orientation for the volley
		int volley_position = rand()%10;
		int orientation = rand()%2;

		int i;

		for(i=0;i<10;i++)
		{

			if(orientation == 0) //horizontal volley
			{
				//its checks if the position is already checked
				if(player2_board[i][volley_position].checked == 1)
				{
					i=i;
				}
				else //if it isnt checked
				{
					//checks the uncheked position
					player2_board[i][volley_position].checked = 1;

					//checks if the square has a piece of a ship
					if(player2_board[i][volley_position].content == 1)
					{
						//draws a explosion
						draw_square(player2_board_xposition+(i*player2_square_width),player2_board_yposition+(volley_position*player2_square_height),player2_square_width,player2_square_height,ship_hit);
						//sum the AI total of hits by 1
						AI_hits += 1;
						//check the ships2 vector for the id of the ship that has been hit and update that information
						ships2[(player2_board_aux[i][volley_position].id)-1] -= 1;
						//updates the ai_shot information with the coordinates of the new shot
						ai_shot.previous_row = volley_position;
						ai_shot.previous_column = i;
						//checks if any new ship has been destroyed
						check_player1_destroyed_ships();

					}
					else //the square is empty
						draw_square(player2_board_xposition+(i*player2_square_width),player2_board_yposition+(volley_position*player2_square_height),player2_square_width,player2_square_height,ship_miss);
				}
			}

			else //vertical volley
			{
				if(player2_board[volley_position][i].checked == 1)
				{
					i=i;
				}
				else //if it isnt checked
				{
					player2_board[volley_position][i].checked = 1;

					//checks if the square has a piece of a ship
					if(player2_board[volley_position][i].content == 1)
					{
						//draws a explosion
						draw_square(player2_board_xposition+(volley_position*player2_square_width),player2_board_yposition+(i*player2_square_height),player2_square_width,player2_square_height,ship_hit);
						//sum the AI total of hits by 1
						AI_hits += 1;
						//check the ships2 vector for the id of the ship that has been hit and update that information
						ships2[(player2_board_aux[volley_position][i].id)-1] -= 1;
						//updates the ai_shot information with the coordinates of the new shot
						ai_shot.previous_row =i;
						ai_shot.previous_column = volley_position;
						//checks if any new ship has been destroyed
						check_player1_destroyed_ships();

					}
					else //the square is empty
						draw_square(player2_board_xposition+(volley_position*player2_square_width),player2_board_yposition+(i*player2_square_height),player2_square_width,player2_square_height,ship_miss);
				}

			}
		}

	}
}

void player1_horizontal_volley()
{

	//checks if the volley special attack was already used by the player1
	if(volley_flag==1)
		volley_flag = 1;
	else
	{
		//it sets the interrupt handler variable so that the player1 cant play again this turn
		number_of_tries = 1;

		//sets the player1_volley_flag so the player1 cant use the special attack again
		set_volley_flag();

		int i;

		for(i=0;i<10;i++)
		{
			//its checks if the position is already checked
			if(player1_board[i][keyboard_cursor.row].checked == 1)
			{
				i=i;
			}
			else //if it isnt checked
			{
				//checks the unchecked position
				player1_board[i][keyboard_cursor.row].checked = 1;

				//checks if the square has a piece of a ship
				if(player1_board[i][keyboard_cursor.row].content == 1)
				{
					//draws a explosion
					draw_square(player1_board_xposition+(i*player1_square_width),player1_board_yposition+(keyboard_cursor.row*player1_square_height),player1_square_width,player1_square_height,ship_hit);
					//sum the AI total of hits by 1
					player1_hits += 1;
					//check the ships2 vector for the id of the ship that has been hit and update that information
					ships1[(player1_board_aux[i][keyboard_cursor.row].id)-1] -= 1;
					//checks if any new ship has been destroyed
					check_player2_destroyed_ships();

				}
				else //the square is empty
					draw_square(player1_board_xposition+(i*player1_square_width),player1_board_yposition+(keyboard_cursor.row*player1_square_height),player1_square_width,player1_square_height,ship_miss);
			}

		}
	}
}

void player1_vertical_volley()
{
	//checks if the volley special attack was already used by the player1
	if(volley_flag==1)
		volley_flag = 1;
	else
	{
		//it sets the interrupt handler variable so that the player1 cant play again this turn
		number_of_tries = 1;

		//sets the player1_volley_flag so the player1 cant use the special attack again
		set_volley_flag();

		int i;

		for(i=0;i<10;i++)
		{
			if(player1_board[keyboard_cursor.column][i].checked == 1)
			{
				i=i;
			}
			else //if it isnt checked
			{
				//its checks if the position is already checked
				player1_board[keyboard_cursor.column][i].checked = 1;

				//checks if the square has a piece of a ship
				if(player1_board[keyboard_cursor.column][i].content == 1)
				{
					//draws a explosion
					draw_square(player1_board_xposition+(keyboard_cursor.column*player1_square_width),player1_board_yposition+(i*player1_square_height),player1_square_width,player1_square_height,ship_hit);
					//sum the AI total of hits by 1
					player1_hits += 1;
					//check the ships2 vector for the id of the ship that has been hit and update that information
					ships1[(player1_board_aux[keyboard_cursor.column][i].id)-1] -= 1;
					//checks if any new ship has been destroyed
					check_player2_destroyed_ships();

				}
				else //the square is empty
					draw_square(player1_board_xposition+(keyboard_cursor.column*player1_square_width),player1_board_yposition+(i*player1_square_height),player1_square_width,player1_square_height,ship_miss);
			}
		}
	}
}

//2nd special attack
//it searches in the table for a piece of a ship with a random id that wasnt hit before and makes it visible
void AI_scout()
{
	//checks if the scout special attack was already used by the AI
	if(AI_scout_flag==1)
		AI_scout_flag = 1;
	else
	{
		//resets the interrupt handler variable so that the player1 can play again
		number_of_tries = 0;
		//sets the AI_scout_flag so the AI cant use the special attack again
		set_AI_scout_flag();
		//generates a random ID for the scout
		int random_id = random()%5+1;
		//it indicates that a piece of a ship was already found using the for loop
		int piece_found = 0;

		int i,j;

		//if the ID generated is of a ship that was already destroyed or of a ship that was already found
		//generate a new ID
		while(ships2[(random_id)-1] == 0 && ships2[(random_id)-1] != ships_aux[(random_id)-1])
			random_id = random()%5+1;

		for(j=0;j<10;j++)
			for(i=0;i<10;i++)
			{
				if(player2_board_aux[i][j].id == random_id && player2_board[i][j].checked == 0 && piece_found == 0)
				{
					draw_square(player2_board_xposition+(i*player2_square_width),player2_board_yposition+(j*player2_square_height),player2_square_width,player2_square_height,ship_found);

					//it refreshes the ai_shot variable with the piece of ship information that was found
					ai_shot.ship_found = 1;
					ai_shot.previous_row = j;
					ai_shot.previous_column = i;
					//sets the piece_found variable
					piece_found = 1;
					break;
				}
			}
	}
}

void player1_scout()
{
	//checks if the scout special attack was already used by the player1
	if(scout_flag==1)
		scout_flag = 1;
	else
	{
		//it sets the interrupt handler variable so that the player1 cant play again this turn
		number_of_tries = 1;
		//sets the player1_scout_flag so the player1 cant use the special attack again
		set_scout_flag();
		//generates a random ID for the scout
		int random_id = random()%5+1;
		//it indicates that a piece of a ship was already found using the for loop
		int piece_found = 0;

		int i,j;

		//if the ID generated is of a ship that was already destroyed or of a ship that was already found
		//generate a new ID
		while(ships1[(random_id)-1] == 0 || ships1[(random_id)-1] != ships_aux[(random_id)-1])
			random_id = random()%5+1;

		for(j=0;j<10;j++)
			for(i=0;i<10;i++)
			{
				if(player1_board_aux[i][j].id == random_id && player1_board[i][j].checked == 0 && piece_found == 0)
				{
					draw_square(player1_board_xposition+(i*player1_square_width),player1_board_yposition+(j*player1_square_height),player1_square_width,player1_square_height,ship_found);
					//sets the piece_found variable
					piece_found = 1;
					break;
				}
			}
	}
}

//3rd special attack
/*void AI_revive()
{
	number_of_tries = 0;
	//it revives a ship based in a randomly generated id
	int random_id = random()%5+1;
	int i,j;

	while(ships1[(random_id)-1] != 0)
		random_id = random()%5+1;

	for(i=0;i<10;i++)
			for(j=0;j<10;j++)
			{
				if(player1_board_aux[i][j].id == random_id)
				{
					draw_square(player1_board_xposition+(i*player1_square_width),player1_board_yposition+(j*player1_square_height),player1_square_width,player1_square_height,ship_revived);
					player1_board[i][j].checked = 0;
					ships1[(player1_board_aux[i][j].id)-1] += 1;
					player1_hits -= 1;
				}
			}

	check_player2_destroyed_ships();
}*/

//3rd special attack
//it makes a explosion with the size of 3x3 square using the actual square as a middle point
void AI_bombardement()
{
	//checks if the bombardement special attack was already used by the AI
	if(AI_bombardement_flag==1)
		AI_bombardement_flag = 1;
	else
	{
		//resets the interrupt handler variable so that the player1 can play again
		number_of_tries = 0;
		//sets the AI_bombardement_flag so the AI cant use the special attack again
		set_AI_bombardement_flag();

		//generates a random position to use the bombardement
		int column = random()%10;
		int row = random()%10;

		int i,j;

		//It generates a new position if the random position previously generated was already checked
		while(player2_board[column][row].checked == 1)
		{
			column = random()%10;
			row = random()%10;
		}

		for(j=-1;j<=1;j++)
			for(i=-1;i<=1;i++)
			{
				//if the square is already checked or exceeds the table border limits - do nothing
				if(player2_board[column+i][row+j].checked == 1|| column+i < 0 || column+i > 9|| row+j < 0 || row+j > 9)
				{
					i=i;
				}
				else //if it isnt checked
				{
					//checks the unchecked square
					player2_board[column+i][row+j].checked = 1;

					//checks if the square has a piece of a ship
					if(player2_board[column+i][row+j].content == 1)
					{
						//draws a explosion
						draw_square(player2_board_xposition+((column+i)*player2_square_width),player2_board_yposition+((row+j)*player2_square_height),player2_square_width,player2_square_height,ship_hit);
						//sum the AI total of hits by 1
						AI_hits += 1;
						//check the ships2 vector for the id of the ship that has been hit and update that information
						ships2[(player2_board_aux[column+i][row+j].id)-1] -= 1;
						//checks if any new ship has been destroyed
						check_player1_destroyed_ships();

					}
					else //the square is empty
						draw_square(player2_board_xposition+((column+i)*player2_square_width),player2_board_yposition+((row+j)*player2_square_height),player2_square_width,player2_square_height,ship_miss);
				}
			}
	}
}

void player1_bombardement()
{
	//checks if the bombardement special attack was already used by the player1
	if(bombardement_flag==1)
		bombardement_flag = 1;
	else
	{
		//it sets the interrupt handler variable so that the player1 cant play again this turn
		number_of_tries = 1;
		//sets the player1_bombardement_flag so the player1 cant use the special attack again
		set_bombardement_flag();

		int i,j;
		for(j=-1;j<=1;j++)
			for(i=-1;i<=1;i++)
			{
				//if the square is already checked or exceeds the table border limits - do nothing
				if(player1_board[keyboard_cursor.column+i][keyboard_cursor.row+j].checked == 1
						|| keyboard_cursor.column+i < 0 || keyboard_cursor.column+i > 9
						|| keyboard_cursor.row+j < 0 || keyboard_cursor.row+j > 9)
				{
					i=i;
				}
				else //if it isnt checked
				{
					//checks the unchecked square
					player1_board[keyboard_cursor.column+i][keyboard_cursor.row+j].checked = 1;

					//checks if the square has a piece of a ship
					if(player1_board[keyboard_cursor.column+i][keyboard_cursor.row+j].content == 1)
					{
						//draws a explosion
						draw_square(player1_board_xposition+((keyboard_cursor.column+i)*player1_square_width),player1_board_yposition+((keyboard_cursor.row+j)*player1_square_height),player1_square_width,player1_square_height,ship_hit);
						//sum the AI total of hits by 1
						player1_hits += 1;
						//check the ships2 vector for the id of the ship that has been hit and update that information
						ships1[(player1_board_aux[keyboard_cursor.column+i][keyboard_cursor.row+j].id)-1] -= 1;
						//checks if any new ship has been destroyed
						check_player2_destroyed_ships();

					}
					else //the square is empty
						draw_square(player1_board_xposition+((keyboard_cursor.column+i)*player1_square_width),player1_board_yposition+((keyboard_cursor.row+j)*player1_square_height),player1_square_width,player1_square_height,ship_miss);
				}
			}
	}
}

//draws the "you win" or "you lose" message and the time of the game
void draw_end_of_game(unsigned int start,unsigned int end)
{
	print_game_duration(start,end);

	if(player1_hits == 17)
	{
		Sprite *you_win_sp = create_sprite(you_win,0);
		draw_sprite(295,150,you_win_sp);
	}
	else
	{
		Sprite *you_lose_sp = create_sprite(you_lose,0);
		draw_sprite(245,150,you_lose_sp);
	}

	sleep(10);
}

//starts the game
void start_commodore()
{
	//variables used to keep track of the game time
	unsigned int time_start=0;
	unsigned int time_end=0;

	//variables used to make the AI randomly use the special attacks
	int random_volley = rand () % 10;
	int random_scout = rand() % 10;
	int random_bombardement = rand() %10;

	//if any of the special attacks has the same time value - generates new values
	while(random_volley == random_scout || random_scout == random_bombardement || random_volley == random_bombardement)
	{
		random_volley = rand () % 10;
		random_scout = rand() % 10;
		random_bombardement = rand() %10 ;
	}

	//initializes the graphical mode in the 1024x786 resolution
	vg_init(0x105);

	//subscribe to all of the devices used in interrupt_handler()
	subscribe_interrupts();

	//draws the initial menu and the game interface next
	if(draw_graphical_interface() == 0)
	{
		unsubscribe_interrupts();
		vg_exit();
		return;
	}
	else
	{

	//gets the RTC time before the game starts
	time_start = getTime();

	//only ends when all of the ships of one of the tables have been destroyed
	while(player1_hits < 17 && AI_hits < 17)
	{
		player1_turn();
		
		if(player1_hits == 17)
			break;

		if(player1_hits == random_volley && AI_volley_flag==0)
			AI_volley();
		else
			if(player1_hits == random_scout && AI_scout_flag==0)
				AI_scout();
			else
				if(player1_hits == random_bombardement && AI_bombardement_flag==0)
					AI_bombardement();
				else
					AI_turn();
	}
	//gets the RTC time after the game end
	time_end = getTime();

	//prints the game duration
	draw_end_of_game(time_start,time_end);

	//unsubscribe to all of the devices used in interrupt_handler()
	unsubscribe_interrupts();

	//exits the graphical mode
	vg_exit();

	return;
	}
}
