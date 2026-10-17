#include "guiDrawer.h"

/*** Sprites ***/
// Game Elements
#include "../sprites/xpms/bonecos_lcom.xpm"
#include "../sprites/xpms/braco_direito.xpm"
#include "../sprites/xpms/braco_esquerdo.xpm"
#include "../sprites/xpms/Untitled.xpm"
#include "../sprites/xpms/start_button.xpm"
#include "../sprites/xpms/quit_button.xpm"
#include "../sprites/xpms/bricks.xpm"
#include "../sprites/xpms/bg_mainroom.xpm"
#include "../sprites/xpms/minigotchi_normal.xpm"
#include "../sprites/xpms/minigotchi_happy.xpm"
#include "../sprites/xpms/hotbar.xpm"
#include "../sprites/xpms/hotbar_select.xpm"
#include "../sprites/xpms/feather.xpm"
#include "../sprites/xpms/nameMinigotchi_bg.xpm"
#include "../sprites/xpms/minigames_icon.xpm"
#include "../sprites/xpms/day_window.xpm"
#include "../sprites/xpms/evening_window.xpm"
#include "../sprites/xpms/night_window.xpm"
#include "../sprites/xpms/rps_background.xpm"
#include "../sprites/xpms/rock.xpm"
#include "../sprites/xpms/paper.xpm"
#include "../sprites/xpms/scissors.xpm"
#include "../sprites/xpms/rps_win.xpm"
#include "../sprites/xpms/rps_tie.xpm"
#include "../sprites/xpms/rps_lose.xpm"
#include "../sprites/xpms/bot_chose.xpm"
#include "../sprites/xpms/exitGame_mini.xpm"
#include "../sprites/xpms/bg_mainroom_hotbar.xpm"
#include "../sprites/xpms/main_menu_bg.xpm"
#include "../sprites/xpms/minigames_menu_bg.xpm"
#include "../sprites/xpms/tic_tac_toe_button.xpm"
#include "../sprites/xpms/rps_button.xpm"
#include "../sprites/xpms/exit_icon_small.xpm"

// Items
#include "../sprites/xpm_items/apple.xpm"
#include "../sprites/xpm_items/banana.xpm"
#include "../sprites/xpm_items/chocolate.xpm"
#include "../sprites/xpm_items/empty.xpm"

// Font
#include "../sprites/xpm_fonts/A.xpm"
#include "../sprites/xpm_fonts/B.xpm"
#include "../sprites/xpm_fonts/C.xpm"
#include "../sprites/xpm_fonts/D.xpm"
#include "../sprites/xpm_fonts/E.xpm"
#include "../sprites/xpm_fonts/F.xpm"
#include "../sprites/xpm_fonts/G.xpm"
#include "../sprites/xpm_fonts/H.xpm"
#include "../sprites/xpm_fonts/I.xpm"
#include "../sprites/xpm_fonts/J.xpm"
#include "../sprites/xpm_fonts/K.xpm"
#include "../sprites/xpm_fonts/L.xpm"
#include "../sprites/xpm_fonts/M.xpm"
#include "../sprites/xpm_fonts/N.xpm"
#include "../sprites/xpm_fonts/O.xpm"
#include "../sprites/xpm_fonts/P.xpm"
#include "../sprites/xpm_fonts/Q.xpm"
#include "../sprites/xpm_fonts/R.xpm"
#include "../sprites/xpm_fonts/S.xpm"
#include "../sprites/xpm_fonts/T.xpm"
#include "../sprites/xpm_fonts/U.xpm"
#include "../sprites/xpm_fonts/V.xpm"
#include "../sprites/xpm_fonts/W.xpm"
#include "../sprites/xpm_fonts/X.xpm"
#include "../sprites/xpm_fonts/Y.xpm"
#include "../sprites/xpm_fonts/Z.xpm"

// Happiness Bar
#include "../sprites/xpm_happiness_levels/happiness_bar_L0.xpm"
#include "../sprites/xpm_happiness_levels/happiness_bar_L1.xpm"
#include "../sprites/xpm_happiness_levels/happiness_bar_L2.xpm"
#include "../sprites/xpm_happiness_levels/happiness_bar_L3.xpm"
#include "../sprites/xpm_happiness_levels/happiness_bar_L4.xpm"
#include "../sprites/xpm_happiness_levels/happiness_bar_L5.xpm"
#include "../sprites/xpm_happiness_levels/happiness_bar_L6.xpm"
#include "../sprites/xpm_happiness_levels/happiness_bar_L7.xpm"
#include "../sprites/xpm_happiness_levels/happiness_bar_L8.xpm"
#include "../sprites/xpm_happiness_levels/happiness_bar_L9.xpm"
#include "../sprites/xpm_happiness_levels/happiness_bar_L10.xpm"
#include "../sprites/xpm_happiness_levels/happiness_bar_max.xpm"

// Happiness Bar
#include "../sprites/xpm_hunger_levels/hunger_bar_L0.xpm"
#include "../sprites/xpm_hunger_levels/hunger_bar_L1.xpm"
#include "../sprites/xpm_hunger_levels/hunger_bar_L2.xpm"
#include "../sprites/xpm_hunger_levels/hunger_bar_L3.xpm"
#include "../sprites/xpm_hunger_levels/hunger_bar_L4.xpm"
#include "../sprites/xpm_hunger_levels/hunger_bar_L5.xpm"
#include "../sprites/xpm_hunger_levels/hunger_bar_L6.xpm"
#include "../sprites/xpm_hunger_levels/hunger_bar_L7.xpm"
#include "../sprites/xpm_hunger_levels/hunger_bar_L8.xpm"
#include "../sprites/xpm_hunger_levels/hunger_bar_L9.xpm"
#include "../sprites/xpm_hunger_levels/hunger_bar_L10.xpm"
#include "../sprites/xpm_hunger_levels/hunger_bar_max.xpm"

// Tic-Tac-Toe
#include "../sprites/xpms/guest_button.xpm"
#include "../sprites/xpms/host_button.xpm"
#include "../sprites/xpms/tictactoe_field.xpm"
#include "../sprites/xpms/tictactoe_wait.xpm"
#include "../sprites/xpms/tictactoe_menu_bg.xpm"
#include "../sprites/xpms/tictactoe_o.xpm"
#include "../sprites/xpms/tictactoe_x.xpm"



// Backgrounds
static Sprite* _bg_mainmenu_sprite = NULL;
static Sprite* _bg_mainroom_sprite = NULL;
static Sprite* _bg_mainroom_hotbar_sprite = NULL;
static Sprite* _bg_minigames_sprite = NULL;
static Sprite* _bg_nameMinigotchi_sprite = NULL;
static Sprite* _bg_rps_sprite = NULL;

// Other Game Elements
static Sprite* _cursor_sprite = NULL;
static Sprite* _start_button_sprite = NULL;
static Sprite* _quit_button_sprite = NULL;
static Sprite* _minigotchi_sprite = NULL;
static Sprite* _minigotchi_happy_sprite = NULL;
static Sprite* _minigames_icon_sprite = NULL;
static Sprite* _day_window = NULL;
static Sprite* _evening_window = NULL;
static Sprite* _night_window = NULL;
static Sprite* _rock_sprite = NULL;
static Sprite* _paper_sprite = NULL;
static Sprite* _scissors_sprite = NULL;
static Sprite* _rps_win_sprite = NULL;
static Sprite* _rps_tie_sprite = NULL;
static Sprite* _rps_lose_sprite = NULL;
static Sprite* _bot_chose_sprite = NULL;
static Sprite* _exit_game_mini_sprite = NULL;
static Sprite* _rps_button_sprite = NULL;
static Sprite* _exit_game_small_sprite = NULL;

// Items
static Sprite* _empty_sprite = NULL;
static Sprite* _apple_sprite = NULL;
static Sprite* _banana_sprite = NULL;
static Sprite* _chocolate_sprite = NULL;

// GUI Elements
static Sprite* _hotbar_sprite = NULL;
static Sprite* _hotbar_select_sprite = NULL;
	
/*** Buffers ***/
static char* _backgroundBuffer;
static char* _backgroundBufferAlternative;

/* Text */
static Sprite* _A_sprite;
static Sprite* _B_sprite;
static Sprite* _C_sprite;
static Sprite* _D_sprite;
static Sprite* _E_sprite;
static Sprite* _F_sprite;
static Sprite* _G_sprite;
static Sprite* _H_sprite;
static Sprite* _I_sprite;
static Sprite* _J_sprite;
static Sprite* _K_sprite;
static Sprite* _L_sprite;
static Sprite* _M_sprite;
static Sprite* _N_sprite;
static Sprite* _O_sprite;
static Sprite* _P_sprite;
static Sprite* _Q_sprite;
static Sprite* _R_sprite;
static Sprite* _S_sprite;
static Sprite* _T_sprite;
static Sprite* _U_sprite;
static Sprite* _V_sprite;
static Sprite* _W_sprite;
static Sprite* _X_sprite;
static Sprite* _Y_sprite;
static Sprite* _Z_sprite;

// Happiness Bar
static Sprite* _happiness_L0_sprite;
static Sprite* _happiness_L1_sprite;
static Sprite* _happiness_L2_sprite;
static Sprite* _happiness_L3_sprite;
static Sprite* _happiness_L4_sprite;
static Sprite* _happiness_L5_sprite;
static Sprite* _happiness_L6_sprite;
static Sprite* _happiness_L7_sprite;
static Sprite* _happiness_L8_sprite;
static Sprite* _happiness_L9_sprite;
static Sprite* _happiness_L10_sprite;
static Sprite* _happiness_max_sprite;

// Hunger
static Sprite* _hunger_L0_sprite;
static Sprite* _hunger_L1_sprite;
static Sprite* _hunger_L2_sprite;
static Sprite* _hunger_L3_sprite;
static Sprite* _hunger_L4_sprite;
static Sprite* _hunger_L5_sprite;
static Sprite* _hunger_L6_sprite;
static Sprite* _hunger_L7_sprite;
static Sprite* _hunger_L8_sprite;
static Sprite* _hunger_L9_sprite;
static Sprite* _hunger_L10_sprite;
static Sprite* _hunger_max_sprite;


// Elements for TicTacToe
static Sprite* _ttt_button_sprite;
static Sprite* _ttt_field;
static Sprite* _ttt_wait;
static Sprite* _ttt_menu_bg;
static Sprite* _guest_button_sprite;
static Sprite* _host_button_sprite;
static Sprite* _ttt_o;
static Sprite* _ttt_x;



// Sprite getters 

Sprite* get_cursor_sprite() {
    return _cursor_sprite;
}

/**
 * @brief Get the button sprite from an index
*/
Sprite* guiDrawer_get_button_sprite(int text_index){
	switch (text_index){
		case 0:
			return guiDrawer_get_start_button_sprite();
			break;
		case 1: 
			return guiDrawer_get_quit_button_sprite();
			break; 
		case 2:
			return guiDrawer_get_minigamesIcon_sprite();
			break;
        case 3:
            return guiDrawer_get_rock_sprite();
            break;
        case 4:
            return guiDrawer_get_paper_sprite();
            break;
        case 5:
            return guiDrawer_get_scissors_sprite();
            break;
        case 6:
            return guiDrawer_get_host_button_sprite();
            break;
        case 7:
            return guiDrawer_get_guest_button_sprite();
            break;
		case 8:
			return guiDrawer_get_exitGameIcon_sprite();
			break;
		case 9:
			return guiDrawer_get_ttt_button_sprite();
			break;
		case 10:
			return guiDrawer_get_rps_button_sprite();
			break;
		case 11:
			return guiDrawer_get_exitIconSmall_button_sprite();
		default: break;
	}
	return NULL;
}

Sprite* guiDrawer_get_start_button_sprite(){
	return _start_button_sprite;
}

Sprite* guiDrawer_get_quit_button_sprite(){
	return _quit_button_sprite;
}

Sprite* guiDrawer_get_minigotchi_sprite() {
	return _minigotchi_sprite;
}

Sprite* guiDrawer_get_minigotchi_cuddle_sprite(){
	return _minigotchi_happy_sprite;
}

Sprite* guiDrawer_get_minigotchi_normal_sprite() {
	return _minigotchi_sprite;
}

Sprite* guiDrawer_get_hotbar_sprite() {
	return _hotbar_sprite;
}

Sprite* guiDrawer_get_hotbar_select_sprite() {
	return _hotbar_select_sprite;
}

Sprite* guiDrawer_get_minigamesIcon_sprite() {
	return _minigames_icon_sprite;
}

Sprite* guiDrawer_get_apple_sprite() {
	return _apple_sprite;
}

Sprite* guiDrawer_get_rock_sprite(){
	return _rock_sprite;
}

Sprite* guiDrawer_get_paper_sprite(){
	return _paper_sprite;
}

Sprite* guiDrawer_get_scissors_sprite(){
	return _scissors_sprite;
}

Sprite* guiDrawer_get_rpsWin_sprite(){
	return _rps_win_sprite;
}

Sprite* guiDrawer_get_rpsTie_sprite(){
	return _rps_tie_sprite;
}

Sprite* guiDrawer_get_rpsLose_sprite(){
	return _rps_lose_sprite;
}

Sprite* guiDrawer_get_botChose_sprite(){
	return _bot_chose_sprite;
}

Sprite* guiDrawer_get_exitGameIcon_sprite(){
	return _exit_game_mini_sprite;
}

Sprite* guiDrawer_get_ttt_button_sprite(){
	return _ttt_button_sprite;
}

Sprite* guiDrawer_get_rps_button_sprite(){
	return _rps_button_sprite;
}

Sprite* guiDrawer_get_exitIconSmall_button_sprite(){
	return _exit_game_small_sprite;
}

/**
 * @brief Get an item sprite from an id
*/
Sprite* guiDrawer_get_item_sprite(int id){
	switch (id){
		case 0:
			return _empty_sprite;
			break;
		case 1:
			return _apple_sprite;
			break;
		case 2:
			return _banana_sprite;
			break;
		case 3:
			return _chocolate_sprite;
			break;
		default:
			return _apple_sprite;
	}
}

Sprite* guiDrawer_get_day_window() {
	return _day_window;
}

Sprite* guiDrawer_get_evening_window() {
	return _evening_window;
}

Sprite* guiDrawer_get_night_window() {
	return _night_window;
}

Sprite* guiDrawer_get_guest_button_sprite() {
	return _guest_button_sprite;
}
Sprite* guiDrawer_get_host_button_sprite() {
	return _host_button_sprite;
}
Sprite* guiDrawer_get_ttt_field() {
	return _ttt_field;
}
Sprite* guiDrawer_get_ttt_wait() {
	return _ttt_wait;
}

Sprite* guiDrawer_get_ttt_o() {
	return _ttt_o;
}

Sprite* guiDrawer_get_ttt_x() {
	return _ttt_x;
}

/**
 * @brief Get a bar sprite from an index and level
 * @details This function gets the correct sprite based on a level which belongs to a certain range for which there is a sprite
*/
Sprite* guiDrawer_get_bar_sprite(int bar_type, int level){
	// There are 12 ranges of happiness levels
	int index = 0;
	if (level < 83) index = 0;
	else if ((level >= 83) && (level < 166)) index = 1; 
	else if ((level >= 166) && (level < 249)) index = 2; 
	else if ((level >= 249) && (level < 332)) index = 3; 
	else if ((level >= 332) && (level < 415)) index = 4; 
	else if ((level >= 415) && (level < 498)) index = 5; 
	else if ((level >= 498) && (level < 581)) index = 6; 
	else if ((level >= 581) && (level < 664)) index = 7; 
	else if ((level >= 664) && (level < 747)) index = 8; 
	else if ((level >= 747) && (level < 830)) index = 9; 
	else if ((level >= 830) && (level < 913)) index = 10; 
	else if ((level >= 913) && (level <= 1000)) index = 11; 
	else index = 0;

	if (bar_type == 0){
		return guiDrawer_get_happiness_level_sprite_from_index(index);
	} else {
		return guiDrawer_get_hunger_level_sprite_from_index(index);
	}
}


/* Buffer Getters */
char** guiDrawer_get_backgroundBuffer(){
	return &_backgroundBuffer;
}

/**
 * @brief Creates a sprite from a XPM
 */
Sprite *create_sprite_xpm(xpm_map_t sprite){
	Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
	if(sp == NULL){
		panic("Error allocating memory for sprite\n");
		return NULL;
	}

	xpm_image_t img;
	sp->colors = (uint32_t *)xpm_load(sprite, XPM_8_8_8_8, &img);
	sp->height = img.height;
	sp->width = img.width;

	if(sp->colors == NULL){
		panic("Error loading sprite\n");
		free(sp);
		return NULL;
	}
	return sp;
}

/**
 * @brief Get a text sprite from an index
*/
Sprite* get_text_sprite_from_index(uint8_t index) {
	switch (index) {
		case 0:
			return _A_sprite;
			break;
		case 1:
			return _B_sprite;
			break;
		case 2:
			return _C_sprite;
			break;
		case 3:
			return _D_sprite;
			break;
		case 4:
			return _E_sprite;
			break;
		case 5:
			return _F_sprite;
			break;
		case 6:
			return _G_sprite;
			break;
		case 7:
			return _H_sprite;
			break;
		case 8:
			return _I_sprite;
			break;
		case 9:
			return _J_sprite;
			break;
		case 10:
			return _K_sprite;
			break;
		case 11:
			return _L_sprite;
			break;
		case 12:
			return _M_sprite;
			break;
		case 13:
			return _N_sprite;
			break;
		case 14:
			return _O_sprite;
			break;
		case 15:
			return _P_sprite;
			break;
		case 16:
			return _Q_sprite;
			break;
		case 17:
			return _R_sprite;
			break;
		case 18:
			return _S_sprite;
			break;
		case 19:
			return _T_sprite;
			break;
		case 20:
			return _U_sprite;
			break;
		case 21:
			return _V_sprite;
			break;
		case 22:
			return _W_sprite;
			break;
		case 23:
			return _X_sprite;
			break;
		case 24:
			return _Y_sprite;
			break;
		case 25:
			return _Z_sprite;
			break;
		default: 
			return NULL;
			break;
	}
	return NULL;
}

/**
 * @brief Get a happiness bar sprite from an index
 */
Sprite* guiDrawer_get_happiness_level_sprite_from_index(uint8_t index) {
	switch (index){
		case 0:
			return _happiness_L0_sprite;
			break;
		case 1:
			return _happiness_L1_sprite;
			break;
		case 2:
			return _happiness_L2_sprite;
			break;
		case 3:
			return _happiness_L3_sprite;
			break;
		case 4:
			return _happiness_L4_sprite;
			break;
		case 5:
			return _happiness_L5_sprite;
			break;
		case 6:
			return _happiness_L6_sprite;
			break;
		case 7:
			return _happiness_L7_sprite;
			break;
		case 8:
			return _happiness_L8_sprite;
			break;
		case 9:
			return _happiness_L9_sprite;
			break;
		case 10:
			return _happiness_L10_sprite;
			break;
		case 11:
			return _happiness_max_sprite;
			break;
	}
	return NULL;
}

/**
 * @brief Get a hunger bar sprite from an index
 */
Sprite* guiDrawer_get_hunger_level_sprite_from_index(uint8_t index){
	switch (index){
		case 0:
			return _hunger_L0_sprite;
			break;
		case 1:
			return _hunger_L1_sprite;
			break;
		case 2:
			return _hunger_L2_sprite;
			break;
		case 3:
			return _hunger_L3_sprite;
			break;
		case 4:
			return _hunger_L4_sprite;
			break;
		case 5:
			return _hunger_L5_sprite;
			break;
		case 6:
			return _hunger_L6_sprite;
			break;
		case 7:
			return _hunger_L7_sprite;
			break;
		case 8:
			return _hunger_L8_sprite;
			break;
		case 9:
			return _hunger_L9_sprite;
			break;
		case 10:
			return _hunger_L10_sprite;
			break;
		case 11:
			return _hunger_max_sprite;
			break;
	}
	return NULL;
}

/**
 * @brief Sets up the sprites used in the Tic Tac Toe minigame
 */
void setup_ttt_sprites() {
	_ttt_field = create_sprite_xpm((xpm_map_t) tictactoe_field_xpm);
	_ttt_wait = create_sprite_xpm((xpm_map_t) tictactoe_wait_xpm);
	_ttt_menu_bg = create_sprite_xpm((xpm_map_t) tictactoe_menu_bg_xpm);
	_guest_button_sprite = create_sprite_xpm((xpm_map_t) guest_button_xpm);
	_host_button_sprite = create_sprite_xpm((xpm_map_t) host_button_xpm);
	_ttt_o = create_sprite_xpm((xpm_map_t) tictactoe_o_xpm);
	_ttt_x = create_sprite_xpm((xpm_map_t) tictactoe_x_xpm);
}

/**
 * @brief Frees the sprites used in the Tic Tac Toe minigame
 */
void cleanup_ttt_sprites() {
	free(_ttt_field);
	free(_ttt_wait);
	free(_ttt_menu_bg);
	free(_guest_button_sprite);
	free(_host_button_sprite);
	free(_ttt_o);
	free(_ttt_x);
}

/**
 * @brief Sets up the letter sprites that make up the font
 */
void setup_text_sprite() {
	_A_sprite = create_sprite_xpm((xpm_map_t) A_xpm );
	_B_sprite = create_sprite_xpm((xpm_map_t) B_xpm );
	_C_sprite = create_sprite_xpm((xpm_map_t) C_xpm );
	_D_sprite = create_sprite_xpm((xpm_map_t) D_xpm );
	_E_sprite = create_sprite_xpm((xpm_map_t) E_xpm );
	_F_sprite = create_sprite_xpm((xpm_map_t) F_xpm );
	_G_sprite = create_sprite_xpm((xpm_map_t) G_xpm );
	_H_sprite = create_sprite_xpm((xpm_map_t) H_xpm );
	_I_sprite = create_sprite_xpm((xpm_map_t) I_xpm );
	_J_sprite = create_sprite_xpm((xpm_map_t) J_xpm );
	_K_sprite = create_sprite_xpm((xpm_map_t) K_xpm );
	_L_sprite = create_sprite_xpm((xpm_map_t) L_xpm );
	_M_sprite = create_sprite_xpm((xpm_map_t) M_xpm );
	_N_sprite = create_sprite_xpm((xpm_map_t) N_xpm );
	_O_sprite = create_sprite_xpm((xpm_map_t) O_xpm );
	_P_sprite = create_sprite_xpm((xpm_map_t) P_xpm );
	_Q_sprite = create_sprite_xpm((xpm_map_t) Q_xpm );
	_R_sprite = create_sprite_xpm((xpm_map_t) R_xpm );
	_S_sprite = create_sprite_xpm((xpm_map_t) S_xpm );
	_T_sprite = create_sprite_xpm((xpm_map_t) T_xpm );
	_U_sprite = create_sprite_xpm((xpm_map_t) U_xpm );
	_V_sprite = create_sprite_xpm((xpm_map_t) V_xpm );
	_W_sprite = create_sprite_xpm((xpm_map_t) W_xpm );
	_X_sprite = create_sprite_xpm((xpm_map_t) X_xpm );
	_Y_sprite = create_sprite_xpm((xpm_map_t) Y_xpm );
	_Z_sprite = create_sprite_xpm((xpm_map_t) Z_xpm );
}

/**
 * @brief Sets up the happiness bar sprites
 */
void setup_happiness_sprites(){
	_happiness_L0_sprite = create_sprite_xpm((xpm_map_t) happiness_bar_L0_xpm);
	_happiness_L1_sprite = create_sprite_xpm((xpm_map_t) happiness_bar_L1_xpm);
	_happiness_L2_sprite = create_sprite_xpm((xpm_map_t) happiness_bar_L2_xpm);
	_happiness_L3_sprite = create_sprite_xpm((xpm_map_t) happiness_bar_L3_xpm);
	_happiness_L4_sprite = create_sprite_xpm((xpm_map_t) happiness_bar_L4_xpm);
	_happiness_L5_sprite = create_sprite_xpm((xpm_map_t) happiness_bar_L5_xpm);
	_happiness_L6_sprite = create_sprite_xpm((xpm_map_t) happiness_bar_L6_xpm);
	_happiness_L7_sprite = create_sprite_xpm((xpm_map_t) happiness_bar_L7_xpm);
	_happiness_L8_sprite = create_sprite_xpm((xpm_map_t) happiness_bar_L8_xpm);
	_happiness_L9_sprite = create_sprite_xpm((xpm_map_t) happiness_bar_L9_xpm);
	_happiness_L10_sprite = create_sprite_xpm((xpm_map_t) happiness_bar_L10_xpm);
	_happiness_max_sprite = create_sprite_xpm((xpm_map_t) happiness_bar_max_xpm);
}

/**
 * @brief Sets up the hunger bar sprites
 */
void setup_hunger_sprites(){
	_hunger_L0_sprite = create_sprite_xpm((xpm_map_t) hunger_bar_L0_xpm);
	_hunger_L1_sprite = create_sprite_xpm((xpm_map_t) hunger_bar_L1_xpm);
	_hunger_L2_sprite = create_sprite_xpm((xpm_map_t) hunger_bar_L2_xpm);
	_hunger_L3_sprite = create_sprite_xpm((xpm_map_t) hunger_bar_L3_xpm);
	_hunger_L4_sprite = create_sprite_xpm((xpm_map_t) hunger_bar_L4_xpm);
	_hunger_L5_sprite = create_sprite_xpm((xpm_map_t) hunger_bar_L5_xpm);
	_hunger_L6_sprite = create_sprite_xpm((xpm_map_t) hunger_bar_L6_xpm);
	_hunger_L7_sprite = create_sprite_xpm((xpm_map_t) hunger_bar_L7_xpm);
	_hunger_L8_sprite = create_sprite_xpm((xpm_map_t) hunger_bar_L8_xpm);
	_hunger_L9_sprite = create_sprite_xpm((xpm_map_t) hunger_bar_L9_xpm);
	_hunger_L10_sprite = create_sprite_xpm((xpm_map_t) hunger_bar_L10_xpm);
	_hunger_max_sprite = create_sprite_xpm((xpm_map_t) hunger_bar_max_xpm);
}

/**
 * @brief Frees the letter sprites that make up the font
 */
void cleanup_text_sprite() {
	free(_A_sprite);
	free(_B_sprite);
	free(_C_sprite);
	free(_D_sprite);
	free(_E_sprite);
	free(_F_sprite);
	free(_G_sprite);
	free(_H_sprite);
	free(_I_sprite);
	free(_J_sprite);
	free(_K_sprite);
	free(_L_sprite);
	free(_M_sprite);
	free(_N_sprite);
	free(_O_sprite);
	free(_P_sprite);
	free(_Q_sprite);
	free(_R_sprite);
	free(_S_sprite);
	free(_T_sprite);
	free(_U_sprite);
	free(_V_sprite);
	free(_W_sprite);
	free(_X_sprite);
	free(_Y_sprite);
	free(_Z_sprite);
}

/**
 * @brief Frees the happiness bar sprites
 */
void cleanup_happiness_sprites(){
	free(_happiness_L0_sprite);
	free(_happiness_L1_sprite);
	free(_happiness_L2_sprite);
	free(_happiness_L3_sprite);
	free(_happiness_L4_sprite);
	free(_happiness_L5_sprite);
	free(_happiness_L6_sprite);
	free(_happiness_L7_sprite);
	free(_happiness_L8_sprite);
	free(_happiness_L9_sprite);
	free(_happiness_L10_sprite); 
	free(_happiness_max_sprite); 
}

/**
 * @brief Frees the hunger bar sprites
*/
void cleanup_hunger_sprites(){
	free(_hunger_L0_sprite);
	free(_hunger_L1_sprite);
	free(_hunger_L2_sprite);
	free(_hunger_L3_sprite);
	free(_hunger_L4_sprite);
	free(_hunger_L5_sprite);
	free(_hunger_L6_sprite);
	free(_hunger_L7_sprite);
	free(_hunger_L8_sprite);
	free(_hunger_L9_sprite);
	free(_hunger_L10_sprite); 
	free(_hunger_max_sprite); 
}

/**
 * @brief Sets up all the main sprites
*/
void setup_sprites(){
	// Backgrounds
	_bg_mainmenu_sprite = create_sprite_xpm((xpm_map_t) main_menu_bg_xpm);
	_bg_mainroom_sprite = create_sprite_xpm((xpm_map_t) bg_mainroom_xpm);
	_bg_mainroom_hotbar_sprite = create_sprite_xpm((xpm_map_t) bg_mainroom_hotbar_xpm);
	_bg_minigames_sprite = create_sprite_xpm((xpm_map_t) minigames_menu_bg_xpm);
	_bg_nameMinigotchi_sprite = create_sprite_xpm((xpm_map_t) nameMinigotchi_bg_xpm);
	_bg_rps_sprite = create_sprite_xpm((xpm_map_t) rps_background_xpm);
	_day_window = create_sprite_xpm((xpm_map_t) day_window_xpm);
	_evening_window = create_sprite_xpm((xpm_map_t) evening_window_xpm);
	_night_window = create_sprite_xpm((xpm_map_t) night_window_xpm);
	_backgroundBuffer = malloc(video_get_vram_size());
	_backgroundBufferAlternative = malloc(video_get_vram_size());

	// Icons
	_minigames_icon_sprite =  create_sprite_xpm((xpm_map_t) minigames_icon_xpm);
	_exit_game_mini_sprite = create_sprite_xpm((xpm_map_t) exitGame_mini_xpm);
	_exit_game_small_sprite = create_sprite_xpm((xpm_map_t) exit_icon_small_xpm);

	// Rock Paper Scissors
	_rps_win_sprite =  create_sprite_xpm((xpm_map_t) rps_win_xpm);
	_rps_tie_sprite =  create_sprite_xpm((xpm_map_t) rps_tie_xpm);
	_rps_lose_sprite =  create_sprite_xpm((xpm_map_t) rps_lose_xpm);
	_bot_chose_sprite =  create_sprite_xpm((xpm_map_t) bot_chose_xpm);

	// Items
	_empty_sprite =  create_sprite_xpm((xpm_map_t) empty_xpm);
	_apple_sprite =  create_sprite_xpm((xpm_map_t) apple_xpm);
	_banana_sprite =  create_sprite_xpm((xpm_map_t) banana_xpm);
	_chocolate_sprite =  create_sprite_xpm((xpm_map_t) chocolate_xpm);

	// Cursor
	_cursor_sprite = create_sprite_xpm((xpm_map_t) feather_xpm);

	// Buttons
	_start_button_sprite = create_sprite_xpm((xpm_map_t) start_button_xpm);
	_quit_button_sprite = create_sprite_xpm((xpm_map_t) quit_button_xpm);
	_rock_sprite = create_sprite_xpm((xpm_map_t) rock_xpm);
	_paper_sprite = create_sprite_xpm((xpm_map_t) paper_xpm);
	_scissors_sprite = create_sprite_xpm((xpm_map_t) scissors_xpm);
	_rps_button_sprite =  create_sprite_xpm((xpm_map_t) rps_button_xpm);
	_ttt_button_sprite =  create_sprite_xpm((xpm_map_t) tic_tac_toe_button_xpm);


	// Minigotchi
	_minigotchi_sprite = create_sprite_xpm((xpm_map_t) minigotchi_normal_xpm);
	_minigotchi_happy_sprite = create_sprite_xpm((xpm_map_t) minigotchi_happy_xpm);

	// Hotbar / hotbar items
	_hotbar_sprite = create_sprite_xpm((xpm_map_t) hotbar_xpm);
	_hotbar_select_sprite = create_sprite_xpm((xpm_map_t) hotbar_select_xpm);
}

/**
 * @brief Switches the background from an index (We are using buffers to make the game faster)
 */
void switchBackground(uint8_t bg) {
	switch (bg) {
		case 0: // MainMenu
			getBufferFromSprite(_bg_mainmenu_sprite->height, _bg_mainmenu_sprite->width, 0, 0, _bg_mainmenu_sprite->colors, &_backgroundBuffer);
			break;
		case 1: // MainRoom
			getBufferFromSprite(_bg_mainroom_sprite->height, _bg_mainroom_sprite->width, 0, 0, _bg_mainroom_sprite->colors, &_backgroundBuffer);
			getBufferFromSprite(_bg_mainroom_hotbar_sprite->height, _bg_mainroom_hotbar_sprite->width, 0, 0, _bg_mainroom_hotbar_sprite->colors, &_backgroundBufferAlternative);
			break;
		case 2: // MinigamesMenu
			getBufferFromSprite(_bg_minigames_sprite->height, _bg_minigames_sprite->width, 0, 0, _bg_minigames_sprite->colors, &_backgroundBuffer);
			break;
		case 3: // NameMinigotchi
			getBufferFromSprite(_bg_nameMinigotchi_sprite->height, _bg_nameMinigotchi_sprite->width, 0, 0, _bg_nameMinigotchi_sprite->colors, &_backgroundBuffer);
			break;
		case 4: // RockPaperScissors
			getBufferFromSprite(_bg_rps_sprite->height, _bg_rps_sprite->width, 0, 0, _bg_rps_sprite->colors, &_backgroundBuffer);
			break;
		case 5: // Tic-Tac-Toe Menu
			getBufferFromSprite(_ttt_menu_bg->height, _ttt_menu_bg->width, 0, 0, _ttt_menu_bg->colors, &_backgroundBuffer);
			break;
		case 6: // Tic-Tac-Toe Game Field
			getBufferFromSprite(_ttt_field->height, _ttt_field->width, 0, 0, _ttt_field->colors, &_backgroundBuffer);
			break;
		case 7: // Tic-Tac-Toe Game Field
			getBufferFromSprite(_ttt_wait->height, _ttt_wait->width, 0, 0, _ttt_wait->colors, &_backgroundBuffer);
			break;
		default:
			break;
	}
}

/**
 * @brief Function responsible for drawing the sprites on the screen
 */
int drawSprite(Sprite *sprite, int x, int y){
	uint16_t height = sprite->height;
	uint16_t width = sprite->width;
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			if(drawPixel(j + x, i + y, sprite->colors[j + i * width]) != 0){
				return 1;
			}
		}
	}
	return 0;
}

uint16_t sprite_get_width(Sprite* sprite){
	return sprite->width;
}

uint16_t sprite_get_height(Sprite* sprite){
	return sprite->height;
}


/*** Wrappers ***/
/**
 * @brief Draws the backfround (wrapper function)
*/
void wrapper_draw_background(uint8_t bufferIndex) {
	switch (bufferIndex) {
		case 0:
			setBackgroundFromBuffer(_backgroundBuffer);
			break;
		case 1:
			setBackgroundFromBuffer(_backgroundBufferAlternative);
			break;
		default:
			break;
	}
}

