#include "game_core_st.h"

//Global variables
state_t current_state;
player_t player;
rtc_t time_start;

char filepath[200];

//Initializer
void linkStart(char *file) {

	current_state = MENU1;
	time_start = checkClock();

	strcpy(filepath, file);
}

unsigned int timePassed() {

	rtc_t currentTime = checkClock();

	unsigned int playTime = (currentTime.h - time_start.h) * 60 + (currentTime.m - time_start.m);

	return playTime;
}

state_t getCurrentState() {
	return current_state;
}

void setCurrentState(state_t s)
{
	current_state = s;
}

player_t getPlayer() {
	return player;
}

//Event handler
void mosu_handler(event_t *evt) {

	switch (current_state) {
	case MENU1:
		if (evt->type == ACTIVATE) {

			//Shows the options of the MENU1
			vg_sprite(mosu, 362, 200, 1);
			vg_sprite(play, 488, 365, 1);
			vg_sprite(exit_menu, 488, 455, 1);

			//Play
			//Checks the x
			if(evt->mouse->x > 488 && evt->mouse->x < 567)
			{
				//Checks the y
				if(evt->mouse->y > 363 && evt->mouse->y < 389)
				{
					vg_circle(528, 376, 40, 63);
				}
			}

			//Exit
			//Checks the x
			if(evt->mouse->x > 488 && evt->mouse->x < 567)
			{
				//Checks the y
				if(evt->mouse->y > 453 && evt->mouse->y < 479)
				{
					vg_circle(528, 466, 40, 63);
				}
			}
		}
		else if (evt->type == LEFT_B) {

			//Play
			//Checks the x
			if(evt->mouse->x > 488 && evt->mouse->x < 567)
				{
					//Checks the y
					if(evt->mouse->y > 363 && evt->mouse->y < 389)
					{
						current_state = MENU2;

						//Alters the position of the cursor so that you don't accidently click any button
						evt->mouse->x = evt->mouse->x - 100;
					}
				}

			//Exit
			//Checks the x
			if(evt->mouse->x > 488 && evt->mouse->x < 567)
				{
					//Checks the y
					if(evt->mouse->y > 453 && evt->mouse->y < 479)
					{
						current_state = END_GAME;
					}
				}
		}
		break;
	case MENU2:
		if (evt->type == ACTIVATE) {

			//Shows the options of the MENU2
			vg_sprite(mosu, 362, 200, 1);
			vg_sprite(easy, 486, 365, 1);
			vg_sprite(hard, 484, 455, 1);

			//Play
			//Checks the x
			if(evt->mouse->x > 486 && evt->mouse->x < 565)
			{
				//Checks the y
				if(evt->mouse->y > 363 && evt->mouse->y < 388)
				{
					vg_circle(528, 376, 40, 63);
				}
			}

			//Exit
			//Checks the x
			if(evt->mouse->x > 486 && evt->mouse->x < 565)
			{
				//Checks the y
				if(evt->mouse->y > 453 && evt->mouse->y < 475)
				{
					vg_circle(528, 466, 40, 63);
				}
			}
		}
		else if (evt->type == LEFT_B) {

			//Easy
			//Checks the x
			if(evt->mouse->x > 486 && evt->mouse->x < 565)
			{
				//Checks the y
				if(evt->mouse->y > 363 && evt->mouse->y < 388)
				{
					current_state = IN_GAME_EASY;
					player = (player_t ) { 0, 1, 1, 100 };
				}
			}

			//Hard
			//Checks the x
			if(evt->mouse->x > 486 && evt->mouse->x < 565)
			{
				//Checks the y
				if(evt->mouse->y > 453 && evt->mouse->y < 475)
				{
					current_state = IN_GAME_HARD;
					player = (player_t ) { 0, 1, 1, 100 };
				}
			}
		}
		break;
	case IN_GAME_EASY:

		if (evt->type == KEY_PRESS) {

			//Bonus multiplier for playing more than half an hour
			unsigned int mult_to_add;
			if(timePassed() >= 30)
				mult_to_add = 2;
			else
				mult_to_add = 1;

			//Time may change during this loop, so let's save it
			unsigned int time_left = evt->circ->t;

			//Points to add to the score and life to add/remove from player
			unsigned int points_to_add;
			int life_changes;

			if (time_left <= 10) {
				points_to_add = 300 * player.multi;
				life_changes = 15;
			} else if (time_left <= 20) {
				points_to_add = 100 * player.multi;
				life_changes = 5;
			} else if (time_left <= 30) {
				points_to_add = 50 * player.multi;
				life_changes = -5;
			} else {
				points_to_add = 0;
				life_changes = -15;
			}

			//Checks if the cursor if within the circle
			//Through the equation: (x - center)² + (y - center)² <= radius²
			unsigned int cx = evt->circ->x;
			unsigned int cy = evt->circ->y;
			unsigned int radius = evt->circ->r;
			radius = radius * radius;

			unsigned int mx = evt->mouse->x;
			unsigned int my = evt->mouse->y;

			unsigned int x = (mx - cx) * (mx - cx);
			unsigned int y = (my - cy) * (my - cy);

			//True only if the mouse is within circle, the key is correct and the timing is acceptable
			if ((x + y) <= radius && points_to_add != 0) {
				evt->circ->l = Z;
				evt->circ->c = 2;

				player.life += life_changes;
				if (player.life > 100)
					player.life = 100;

				player.score += points_to_add;
				player.multi += mult_to_add;
			} else {
				evt->circ->l = X;
				evt->circ->c = 4;

				player.life += life_changes;

				//Reset multiplier
				if (player.multi > player.max_multi)
					player.max_multi = player.multi;

				player.multi = 1;
			}

			evt->circ->t = 10;
		}
		else if (evt->type == MISSED) {
			evt->circ->l = X;
			evt->circ->c = 4;

			//Reset multiplier
			if (player.multi > player.max_multi)
				player.max_multi = player.multi;

			player.life -= 15;
			player.multi = 1;

			evt->circ->t = 10;
		}

		if (player.life < 0)
			current_state = END_GAME;
		break;
	case IN_GAME_HARD:

		if (evt->type == KEY_PRESS) {

			//Bonus multiplier for playing more than half an hour
			unsigned int mult_to_add;
			if(timePassed() >= 30)
				mult_to_add = 2;
			else
				mult_to_add = 1;

			//Time may change during this loop, so let's save it
			unsigned int time_left = evt->circ->t;

			//Points to add to the score and life to add/remove from player
			unsigned int points_to_add;
			int life_changes;

			if (time_left <= 10) {
				points_to_add = 300 * player.multi;
				life_changes = 15;
			}
			else if (time_left <= 20) {
				points_to_add = 100 * player.multi;
				life_changes = 5;
			}
			else if (time_left <= 30) {
				points_to_add = 50 * player.multi;
				life_changes = -5;
			}
			else {
				points_to_add = 0;
				life_changes = -15;
			}

			unsigned long correct_key = evt->circ->l;

			//Checks if the cursor if within the circle
			//Through the equation: (x - center)² + (y - center)² <= radius²
			unsigned int cx = evt->circ->x;
			unsigned int cy = evt->circ->y;
			unsigned int radius = evt->circ->r;
			radius = radius * radius;

			unsigned int mx = evt->mouse->x;
			unsigned int my = evt->mouse->y;

			unsigned int x = (mx - cx) * (mx - cx);
			unsigned int y = (my - cy) * (my - cy);

			//True only if the mouse is within circle, the key is correct and the timing is acceptable
			if ((x + y) <= radius && evt->key_code == correct_key && points_to_add != 0) {
				evt->circ->l = Z;
				evt->circ->c = 2;

				player.life += life_changes;
				if (player.life > 100)
					player.life = 100;

				player.score += points_to_add;
				player.multi += mult_to_add;
			}
			else {
				evt->circ->l = X;
				evt->circ->c = 4;

				player.life += life_changes;

				//Reset multiplier
				if (player.multi > player.max_multi)
					player.max_multi = player.multi;

				player.multi = 1;
			}

			evt->circ->t = 10;
		}
		else if (evt->type == MISSED) {
			evt->circ->l = X;
			evt->circ->c = 4;

			//Reset multiplier
			if (player.multi > player.max_multi)
				player.max_multi = player.multi;

			player.life -= 15;
			player.multi = 1;

			evt->circ->t = 10;
		}

		if (player.life < 0)
			current_state = END_GAME;
		break;
	case END_GAME:

		if(evt->type == SAVE) {

			FILE *read = fopen(filepath, "r");

			if (!read)
				return;

			//First player
			unsigned int score_p1, multi_p1, h1, m1, s1;

			//Second player
			unsigned int score_p2, multi_p2, h2, m2, s2;

			//Third player
			unsigned int score_p3, multi_p3 = 0, h3, m3, s3;

			char line[1000];

			int cont = 1;
			while (fgets(line, 1000, read)!= NULL) {

				if(cont == 1)
					sscanf(line, "Score: %d, Multiplier: %d (%d:%d:%d)", &score_p1, &multi_p1, &h1, &m1, &s1);
				else if(cont == 2)
					sscanf(line, "Score: %d, Multiplier: %d (%d:%d:%d)", &score_p2, &multi_p2, &h2, &m2, &s2);
				else if(cont == 3)
					sscanf(line, "Score: %d, Multiplier: %d (%d:%d:%d)", &score_p3, &multi_p3, &h3, &m3, &s3);

				cont++;
			}

			fclose(read);

			FILE *write = fopen(filepath, "w");

			rtc_t now = checkClock();

			if(player.score > score_p1) {
				fprintf(write, "Score: %d, Multiplier: %d (%d:%d:%d)\n", player.score, player.max_multi, now.h, now.min, now.s);
				fprintf(write, "Score: %d, Multiplier: %d (%d:%d:%d)\n", score_p1, multi_p1, h1, m1, s1);
				fprintf(write, "Score: %d, Multiplier: %d (%d:%d:%d)\n", score_p2, multi_p2, h2, m2, s2);
			}
			else if(player.score > score_p2) {
				fprintf(write, "Score: %d, Multiplier: %d (%d:%d:%d)\n", score_p1, multi_p1, h1, m1, s1);
				fprintf(write, "Score: %d, Multiplier: %d (%d:%d:%d)\n", player.score, player.max_multi, now.h, now.min, now.s);
				fprintf(write, "Score: %d, Multiplier: %d (%d:%d:%d)\n", score_p2, multi_p2, h2, m2, s2);
			}
			else if(player.score > score_p3) {
				fprintf(write, "Score: %d, Multiplier: %d (%d:%d:%d)\n", score_p1, multi_p1, h1, m1, s1);
				fprintf(write, "Score: %d, Multiplier: %d (%d:%d:%d)\n", score_p2, multi_p2, h2, m2, s2);
				fprintf(write, "Score: %d, Multiplier: %d (%d:%d:%d)\n", player.score, player.max_multi, now.h, now.min, now.s);
			}
			else {
				fprintf(write, "Score: %d, Multiplier: %d (%d:%d:%d)\n", score_p1, multi_p1, h1, m1, s1);
				fprintf(write, "Score: %d, Multiplier: %d (%d:%d:%d)\n", score_p2, multi_p2, h2, m2, s2);
				fprintf(write, "Score: %d, Multiplier: %d (%d:%d:%d)\n", score_p3, multi_p3, h3, m3, s3);
			}

			fclose(write);

			current_state = MENU1;
		}
		break;
	}
}
