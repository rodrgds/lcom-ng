#include "game.h"


extern uint32_t key_pressed;
extern int coordinate_x_mouse;
extern int coordinate_y_mouse;
extern struct Fruit fruit_map[20];
extern enum xpm_image_type type;

enum xpm_image_type type = XPM_5_6_5;

xpm_image_t img_zero;
xpm_image_t img_one;
xpm_image_t img_two;
xpm_image_t img_three;
xpm_image_t img_four;
xpm_image_t img_five;
xpm_image_t img_six;
xpm_image_t img_seven;
xpm_image_t img_eight;
xpm_image_t img_nine;

xpm_image_t img_a;
xpm_image_t img_b;
xpm_image_t img_c;
xpm_image_t img_d;
xpm_image_t img_e;
xpm_image_t img_f;
xpm_image_t img_g;
xpm_image_t img_h;
xpm_image_t img_i;
xpm_image_t img_j;
xpm_image_t img_k;
xpm_image_t img_l;
xpm_image_t img_m;
xpm_image_t img_n;
xpm_image_t img_o;
xpm_image_t img_p;
xpm_image_t img_q;
xpm_image_t img_r;
xpm_image_t img_s;
xpm_image_t img_t;
xpm_image_t img_u;
xpm_image_t img_v;
xpm_image_t img_w;
xpm_image_t img_x;
xpm_image_t img_y;
xpm_image_t img_z;


uint8_t* zero_adr;
uint8_t* one_adr;
uint8_t* two_adr;
uint8_t* three_adr;
uint8_t* four_adr;
uint8_t* five_adr;
uint8_t* six_adr;
uint8_t* seven_adr;
uint8_t* eight_adr;
uint8_t* nine_adr;

uint8_t* a_adr;
uint8_t* b_adr;
uint8_t* c_adr;
uint8_t* d_adr;
uint8_t* e_adr;
uint8_t* f_adr;
uint8_t* g_adr;
uint8_t* h_adr;
uint8_t* i_adr;
uint8_t* j_adr;
uint8_t* k_adr;
uint8_t* l_adr;
uint8_t* m_adr;
uint8_t* n_adr;
uint8_t* o_adr;
uint8_t* p_adr;
uint8_t* q_adr;
uint8_t* r_adr;
uint8_t* s_adr;
uint8_t* t_adr;
uint8_t* u_adr;
uint8_t* v_adr;
uint8_t* w_adr;
uint8_t* x_adr;
uint8_t* y_adr;
uint8_t* z_adr;

char name[100];

void mouse_event_handler(xpm_image_t mouse_img, uint8_t* mouse_adr, enum game_state* state, struct packet* pkt, void* background_game, uint8_t* apple_adr, xpm_image_t img_apple, uint8_t* xpm_game_background_adr, xpm_image_t img_game_background) {
	if (*state == MAIN_MENU)
		updateMousePosition(pkt);
	if (*state == PLAY_HIGHLIGHT)
		updateMousePosition(pkt);
	if (*state == EXIT_HIGHLIGHT)
		updateMousePosition(pkt);
	if (*state == LEADERBOARD_HIGHLIGHT)
		updateMousePosition(pkt);
	if (*state == PLAY){
		updateMousePosition(pkt);
		draw_xpm(mouse_adr, mouse_img, coordinate_x_mouse, coordinate_y_mouse);
		draw_buffer();
	}

	if (*state == MAIN_MENU && coordinate_x_mouse > 469 && coordinate_x_mouse < 790 && coordinate_y_mouse > 417 && coordinate_y_mouse < 522)
		*state = PLAY_HIGHLIGHT;

	if (*state == MAIN_MENU && coordinate_x_mouse > 496 && coordinate_x_mouse < 787 && coordinate_y_mouse > 831 && coordinate_y_mouse < 932)
		*state = EXIT_HIGHLIGHT;
	
	if (*state == MAIN_MENU && coordinate_x_mouse > 268 && coordinate_x_mouse < 1007 && coordinate_y_mouse > 630 && coordinate_y_mouse < 718)
		*state = LEADERBOARD_HIGHLIGHT;

	if (*state == PLAY_HIGHLIGHT && pkt->lb && coordinate_x_mouse > 469 && coordinate_x_mouse < 790 && coordinate_y_mouse > 417 && coordinate_y_mouse < 522)
		*state = PLAY_BEFORE_KEY;

	if (*state == PLAY_HIGHLIGHT && !(coordinate_x_mouse > 469 && coordinate_x_mouse < 790 && coordinate_y_mouse > 417 && coordinate_y_mouse < 522))
		*state = MAIN_MENU;

	if (*state == EXIT_HIGHLIGHT && pkt->lb && coordinate_x_mouse > 496 && coordinate_x_mouse < 787 && coordinate_y_mouse > 831 && coordinate_y_mouse < 932)
		*state = EXIT;

	if (*state == EXIT_HIGHLIGHT && !(coordinate_x_mouse > 496 && coordinate_x_mouse < 787 && coordinate_y_mouse > 831 && coordinate_y_mouse < 932))
		*state = MAIN_MENU;

	if (*state == LEADERBOARD_HIGHLIGHT && pkt->lb && coordinate_x_mouse > 268 && coordinate_x_mouse < 1007 && coordinate_y_mouse > 630 && coordinate_y_mouse < 718)
		*state = LEADERBOARD;

	if (*state == LEADERBOARD_HIGHLIGHT && !(coordinate_x_mouse > 268 && coordinate_x_mouse < 1007 && coordinate_y_mouse > 630 && coordinate_y_mouse < 718))
		*state = MAIN_MENU;

	// On mouse click, fruit gets switched to another place chosen randomly

	if (*state == PLAY) {
		for (size_t i = 0; i < 20; i++)
		{
			if (((coordinate_x_mouse >= fruit_map[i].x_coordinate * 25)) && (coordinate_x_mouse <= fruit_map[i].x_coordinate * 25 + 25) && (coordinate_y_mouse >= fruit_map[i].y_coordinate * 25) && (coordinate_y_mouse <= fruit_map[i].y_coordinate * 25 + 25) && (pkt->lb)) {

				fruit_map[i].x_coordinate = (rand() % (50 - 1 - 1)) + 1;
				fruit_map[i].y_coordinate = (rand() % (40 - 1 - 1)) + 1;
				
				clear_buffer();
				draw_xpm(xpm_game_background_adr, img_game_background, 0, 0);
					
				for (int z = 0; z < 20; z++)
				{
					memcpy(background_game, getBuffer(), getSize());
					draw_xpm(apple_adr, img_apple, fruit_map[z].x_coordinate * 25, fruit_map[z].y_coordinate * 25);
				}

			}
		}
	}
}

void keyboard_event_handler(enum game_state* state, uint32_t* key_pressed, enum direction* dir) {
	// Menu inputs

	if (*state == DODGE && *key_pressed == ESC_KEY)
		*state = MAIN_MENU;
	if (*state == PLAY && *key_pressed == ESC_KEY)
		*state = MAIN_MENU;
	if (*state == GAME_OVER && *key_pressed == ESC_KEY){
		*state = USERNAME;
		*key_pressed=0;
	}
	if (*state == LEADERBOARD && *key_pressed ==ESC_KEY)
		*state = MAIN_MENU;
	if (*state == USERNAME && *key_pressed ==ESC_KEY)
		*state = MAIN_MENU;

	// Starting direction
	if (*state == DODGE && *key_pressed == UP_ARROW) {
		*state = PLAY;
		*dir = UP;
	}

	if (*state == DODGE && *key_pressed == DOWN_ARROW) {
		*state = PLAY;
		*dir = DOWN;
	}

	if (*state == DODGE && *key_pressed == RIGHT_ARROW) {
		*state = PLAY;
		*dir = RIGHT;
	}

	if (*state == DODGE && *key_pressed == LEFT_ARROW) {
		*state = PLAY;
		*dir = LEFT;
	}

	// Direction changes

	if (*state == PLAY && *dir == RIGHT) {
		if (*key_pressed == UP_ARROW) {
			*dir = UP;
		}
		else if (*key_pressed == DOWN_ARROW) {
			*dir = DOWN;
		}
	}

	if (*state == PLAY && *dir == LEFT) {
		if (*key_pressed == UP_ARROW) {
			*dir = UP;
		}
		else if (*key_pressed == DOWN_ARROW) {
			*dir = DOWN;
		}
	}

	if (*state == PLAY && *dir == UP) {
		if (*key_pressed == RIGHT_ARROW) {
			*dir = RIGHT;
		}
		else if (*key_pressed == LEFT_ARROW) {
			*dir = LEFT;
		}
	}

	if (*state == PLAY && *dir == DOWN) {
		if (*key_pressed == RIGHT_ARROW) {
			*dir = RIGHT;
		}
		else if (*key_pressed == LEFT_ARROW) {
			*dir = LEFT;
		}
	}

	if (*state == USERNAME) {
		char to_input[50];
		if (*key_pressed == A_KEY) {
			strcpy(to_input, "A");
			strcat(name, to_input);
		}
		if (*key_pressed == B_KEY) {
			strcpy(to_input, "B");
			strcat(name, to_input);
		}
		if (*key_pressed == C_KEY) {
			strcpy(to_input, "C");
			strcat(name, to_input);
		}
		if (*key_pressed == D_KEY) {
			strcpy(to_input, "D");
			strcat(name, to_input);
		}
		if (*key_pressed == E_KEY) {
			strcpy(to_input, "E");
			strcat(name, to_input);
		}
		if (*key_pressed == F_KEY) {
			strcpy(to_input, "F");
			strcat(name, to_input);
		}
		if (*key_pressed == G_KEY) {
			strcpy(to_input, "G");
			strcat(name, to_input);
		}
		if (*key_pressed == H_KEY) {
			strcpy(to_input, "H");
			strcat(name, to_input);
		}
		if (*key_pressed == I_KEY) {
			strcpy(to_input, "I");
			strcat(name, to_input);
		}
		if (*key_pressed == J_KEY) {
			strcpy(to_input, "J");
			strcat(name, to_input);
		}
		if (*key_pressed == K_KEY) {
			strcpy(to_input, "K");
			strcat(name, to_input);
		}
		if (*key_pressed == L_KEY) {
			strcpy(to_input, "L");
			strcat(name, to_input);
		}
		if (*key_pressed == M_KEY) {
			strcpy(to_input, "M");
			strcat(name, to_input);
		}
		if (*key_pressed == N_KEY) {
			strcpy(to_input, "N");
			strcat(name, to_input);
		}
		if (*key_pressed == O_KEY) {
			strcpy(to_input, "O");
			strcat(name, to_input);
		}
		if (*key_pressed == P_KEY) {
			strcpy(to_input, "P");
			strcat(name, to_input);
		}
		if (*key_pressed == Q_KEY) {
			strcpy(to_input, "Q");
			strcat(name, to_input);
		}
		if (*key_pressed == R_KEY) {
			strcpy(to_input, "R");
			strcat(name, to_input);
		}
		if (*key_pressed == S_KEY) {
			strcpy(to_input, "S");
			strcat(name, to_input);
		}
		if (*key_pressed == T_KEY) {
			strcpy(to_input, "T");
			strcat(name, to_input);
		}
		if (*key_pressed == U_KEY) {
			strcpy(to_input, "U");
			strcat(name, to_input);
		}
		if (*key_pressed == V_KEY) {
			strcpy(to_input, "V");
			strcat(name, to_input);
		}
		if (*key_pressed == W_KEY) {
			strcpy(to_input, "W");
			strcat(name, to_input);
		}
		if (*key_pressed == X_KEY) {
			strcpy(to_input, "X");
			strcat(name, to_input);
		}
		if (*key_pressed == Y_KEY) {
			strcpy(to_input, "Y");
			strcat(name, to_input);
		}
		if (*key_pressed == Z_KEY) {
			strcpy(to_input, "Z");
			strcat(name, to_input);
		}
		if (*key_pressed == SPACE_KEY) {
			strcpy(to_input, " ");
			strcat(name, to_input);
		}
		if (*key_pressed == BACKSPACE_KEY) {
			name[strlen(name) - 1] = '\0';
		}
		if (strlen(name) > 8) {
			name[strlen(name) - 1] = '\0';
		}
	}
}

void text_to_screen(char text[], unsigned int newline, unsigned int x, unsigned int y) {
	unsigned int newx = x;
	unsigned int newy = y;
	
	unsigned int x_original = x;
	unsigned int total_sum = 0;
		
	for (int i = 0; text[i] != '\0'; i++) {		
		if ((text[i] == 'a') || (text[i] == 'A')) {
			draw_xpm(a_adr, img_a, newx, newy);
		}
		else if ((text[i] == 'b') || (text[i] == 'B')) {
			draw_xpm(b_adr, img_b, newx, newy);
		}
		else if ((text[i] == 'c') || (text[i] == 'C')) {
			draw_xpm(c_adr, img_c, newx, newy);
		}
		else if ((text[i] == 'd') || (text[i] == 'D')) {
			draw_xpm(d_adr, img_d, newx, newy);
		}
		else if ((text[i] == 'e') || (text[i] == 'E')) {
			draw_xpm(e_adr, img_e, newx, newy);
		}
		else if ((text[i] == 'f') || (text[i] == 'F')) {
			draw_xpm(f_adr, img_f, newx, newy);
		}
		else if ((text[i] == 'g') || (text[i] == 'G')) {
			draw_xpm(g_adr, img_g, newx, newy);
		}
		else if ((text[i] == 'h') || (text[i] == 'H')) {
			draw_xpm(h_adr, img_h, newx, newy);
		}
		else if ((text[i] == 'i') || (text[i] == 'I')) {
			draw_xpm(i_adr, img_i, newx, newy);
		}
		else if ((text[i] == 'j') || (text[i] == 'J')) {
			draw_xpm(j_adr, img_j, newx, newy);
		}
		else if ((text[i] == 'k') || (text[i] == 'K')) {
			draw_xpm(k_adr, img_k, newx, newy);
		}
		else if ((text[i] == 'l') || (text[i] == 'L')) {
			draw_xpm(l_adr, img_l, newx, newy);
		}
		else if ((text[i] == 'm') || (text[i] == 'M')) {
			draw_xpm(m_adr, img_m, newx, newy);
		}
		else if ((text[i] == 'n') || (text[i] == 'N')) {
			draw_xpm(n_adr, img_n, newx, newy);
		}
		else if ((text[i] == 'o') || (text[i] == 'O')) {
			draw_xpm(o_adr, img_o, newx, newy);
		}
		else if ((text[i] == 'p') || (text[i] == 'P')) {
			draw_xpm(p_adr, img_p, newx, newy);
		}
		else if ((text[i] == 'q') || (text[i] == 'Q')) {
			draw_xpm(q_adr, img_q, newx, newy);
		}
		else if ((text[i] == 'r') || (text[i] == 'R')) {
			draw_xpm(r_adr, img_r, newx, newy);
		}
		else if ((text[i] == 's') || (text[i] == 'S')) {
			draw_xpm(s_adr, img_s, newx, newy);
		}
		else if ((text[i] == 't') || (text[i] == 'T')) {
			draw_xpm(t_adr, img_t, newx, newy);
		}
		else if ((text[i] == 'u') || (text[i] == 'U')) {
			draw_xpm(u_adr, img_u, newx, newy);
		}
		else if ((text[i] == 'v') || (text[i] == 'V')) {
			draw_xpm(v_adr, img_v, newx, newy);
		}
		else if ((text[i] == 'w') || (text[i] == 'W')) {
			draw_xpm(w_adr, img_w, newx, newy);
		}
		else if ((text[i] == 'x') || (text[i] == 'X')) {
			draw_xpm(x_adr, img_x, newx, newy);
		}
		else if ((text[i] == 'y') || (text[i] == 'Y')) {
			draw_xpm(y_adr, img_y, newx, newy);
		}
		else if ((text[i] == 'z') || (text[i] == 'Z')) {
			draw_xpm(z_adr, img_z, newx, newy);
		}
		else if (text[i] == '0') {
			draw_xpm(zero_adr, img_zero, newx, newy);
		}
		else if (text[i] == '1') {
			draw_xpm(one_adr, img_one, newx, newy);
		}
		else if (text[i] == '2') {
			draw_xpm(two_adr, img_two, newx, newy);
		}
		else if (text[i] == '3') {
			draw_xpm(three_adr, img_three, newx, newy);
		}
		else if (text[i] == '4') {
			draw_xpm(four_adr, img_four, newx, newy);
		}
		else if (text[i] == '5') {
			draw_xpm(five_adr, img_five, newx, newy);
		}
		else if (text[i] == '6') {
			draw_xpm(six_adr, img_six, newx, newy);
		}
		else if (text[i] == '7') {
			draw_xpm(seven_adr, img_seven, newx, newy);
		}
		else if (text[i] == '8') {
			draw_xpm(eight_adr, img_eight, newx, newy);
		}
		else if (text[i] == '9') {
			draw_xpm(nine_adr, img_nine, newx, newy);
		}
		else if (text[i] == ' ')
			newx += 5;

		total_sum++;

		if (total_sum == newline) { // If time to go to line below
			total_sum = 0;
			newx = x_original;
			newy += 30;
		}
		else
			newx += 35; // 35 pixels between characters - change to fit the text best

	}
	draw_buffer();
}

void loadAlphabet() {
	zero_adr = xpm_load(zero_xpm, type, &img_zero);
	one_adr = xpm_load(one_xpm, type, &img_one);
	two_adr = xpm_load(two_xpm, type, &img_two);
	three_adr = xpm_load(three_xpm, type, &img_three);
	four_adr = xpm_load(four_xpm, type, &img_four);
	five_adr = xpm_load(five_xpm, type, &img_five);
	six_adr = xpm_load(six_xpm, type, &img_six);
	seven_adr = xpm_load(seven_xpm, type, &img_seven);
	eight_adr = xpm_load(eight_xpm, type, &img_eight);
	nine_adr = xpm_load(nine_xpm, type, &img_nine);

	a_adr = xpm_load(a_xpm, type, &img_a);
	b_adr = xpm_load(b_xpm, type, &img_b);
	c_adr = xpm_load(c_xpm, type, &img_c);
	d_adr = xpm_load(d_xpm, type, &img_d);
	e_adr = xpm_load(e_xpm, type, &img_e);
	f_adr = xpm_load(f_xpm, type, &img_f);
	g_adr = xpm_load(g_xpm, type, &img_g);
	h_adr = xpm_load(h_xpm, type, &img_h);
	i_adr = xpm_load(i_xpm, type, &img_i);
	j_adr = xpm_load(j_xpm, type, &img_j);
	k_adr = xpm_load(k_xpm, type, &img_k);
	l_adr = xpm_load(l_xpm, type, &img_l);
	m_adr = xpm_load(m_xpm, type, &img_m);
	n_adr = xpm_load(n_xpm, type, &img_n);
	o_adr = xpm_load(o_xpm, type, &img_o);
	p_adr = xpm_load(p_xpm, type, &img_p);
	q_adr = xpm_load(q_xpm, type, &img_q);
	r_adr = xpm_load(r_xpm, type, &img_r);
	s_adr = xpm_load(s_xpm, type, &img_s);
	t_adr = xpm_load(t_xpm, type, &img_t);
	u_adr = xpm_load(u_xpm, type, &img_u);
	v_adr = xpm_load(v_xpm, type, &img_v);
	w_adr = xpm_load(w_xpm, type, &img_w);
	x_adr = xpm_load(x_xpm, type, &img_x);
	y_adr = xpm_load(y_xpm, type, &img_y);
	z_adr = xpm_load(z_xpm, type, &img_z);

}
