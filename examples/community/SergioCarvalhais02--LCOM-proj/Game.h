#ifndef _GAME_H_
#define _GAME_H_


#include "i8042.h"
#include "elements.h"

/**
 * @brief Enumerated type for identifying the seven different game states
 */
typedef enum GameState {
    CLOSE, MENU, RUNNING, PONTUATION, LEADERBOARD, WAIT_MENU, WAIT_LB
} GameState;

/**
 * @brief Enumerated type for identifying the three different image states
 */
typedef enum Img_State {
    KEEP, CHANGE_ELEM, CHANGE_BG
} Img_State;

/**
 * @brief Struct for defining the game
 */
typedef struct Game {
    int lim_x; /*!< horizontal screen size */
    int lim_y; /*!< vertical screen size */
    uint16_t pontuation; /*!< pontuation according to the buttons scored */
    int16_t game_timer; /*!< countdown timer of the game */
    uint16_t wait_time; /*!< time in the waiting menu states (transtition states between the others) */
    Rectangle menu_elements[3]; /*!< rectangles that are invisible and above the menu options and are clickable */
    Rectangle pontuation_menu; /*!< rectangle representing the pontuation menu */
    Rectangle pontuation_elements[3]; /*!< rectangles that are invisible and above the pontuation menu options and are clickable */
    Rectangle leaderboard_menu; /*!< rectangle representing the leaderboard menu */
    Rectangle leaderboard_elements[1]; /*!< rectangles that are invisible and above the pontuation menu options and are clickable */
    Button buttons[9]; /*!< array containing the buttons that are present in the game */
    Mouse mouse; /*!< mouse that is used in the game */
    GameState g_state; /*!< state of the game */
    Img_State img_state; /*!< image state of the game */
    Video_Mode video_mode; /*!< video graphic mode where the game is running */
    Text text; /*!< text that is written when requesting the name to save the score */
    Score top_score_list[3]; /*!< list with the 3 highest scores */
    bool added_top_name; /*!< true if top_score_list is not empty, false otherwise */
} Game;

/**
 * function that processes data received from the mouse or keyboard based on the state the game is in
 * @param game address of the game whose current state is checked
 * @param mouse_data data containing mouse positioning and whose movement is to be processed
 * @param click true if the left mouse button was pressed, false otherwise
 * @param kb_data data received from the keyboard whose keys will be checked
 * @param kb_press true if a key form the keyboard was pressed, false otherwise
 * @return return 0 upon success and non-zero otherwise
*/
int (process_data)(Game * game, int mouse_data[2], bool click, int kb_data[3], bool kb_press);

/**
 * function that updates current mouse hitbox position
 * @param game address of the game that is running
 * @param mouse_data array containing the current coordenates of the mouse
 * @return return 0 upon success and non-zero otherwise
*/
int (process_mouse_data_movement)(Game * game, int mouse_data[2]);

/**
 * function that checks keys pressed based on breakcodes received
 * @param game address of the game that is running
 * @param kb_data array containing the breakcodes of the released keys
 * @return returns a defined keyboard key with a number associated
*/
Keyboard_Key (process_keyboard_data_input)(Game * game, int kb_data[3]);

/**
 * function that starts the game by filling in the necessary attributes of the respective game address based on the graphic mode
 * @param game address of the game to be started
 * @param width width of the screen based on the chosen video graphics mode
 * @param height height of the screen based on the chosen video graphics mode
 * @param vm chosen video graphics mode
*/
void (init_game)(Game * game, uint32_t width, uint32_t height, Video_Mode vm);

/**
 * function that starts the game mouse by filling in the necessary attributes like its hitbox position, size and color
 * @param game address of the game whose mouse is to be initiated
*/
void (init_mouse)(Game * game);

/**
 * function that starts the game menu by filling the list menu_elements with the rectangles and their attributes
 * @param game address of the game whose menu is to be initiated
*/
void (init_menu)(Game * game);

/**
 * function that starts the game buttons by filling the list buttons with the buttons and their specific attributes
 * @param game address of the game whose buttons are to be initiated
*/
void (init_buttons)(Game * game);

/**
 * function that starts the game pontuation by filling the lists pontuation_menu and pontuation_elements with the rectangles and their attributes
 * @param game address of the game whose pontation elements are to be initiated
*/
void (init_pontuation)(Game * game);

/**
 * function that starts the game leaderboard by filling in the lists leaderboard_menu and leaderboard_elements with the rectangles and their attributes
 * @param game address of the game whose leadreboard are to be initiated
*/
void (init_leaderboard)(Game * game);

/**
 * function that increases the game score (pontuation attribute) based on the type of the button scored
 * @param game address of the game whose pontuation is to be updated
 * @param mt type of the button (button_type) that was scored
*/
void (score_points)(Game * game, Button_Type mt);

/**
 * function that proccesses keyboard inputs and fills text game attribute
 * @param game game whose text attribute is being filled
*/
void (process_text_key)(Game * game, Keyboard_Key key);

/**
 * function that updates the top_score_list based on the pontuation of the current game
 * @param game game whose list top_score_list is being updated
 * @return return 0 upon success and non-zero otherwise
*/
int (process_pontuation)(Game * game);

/**
 * function that erases all the pontuation data of the current game when it ends
 * @param game game whose pontuation data is to be erased
*/
void (leave_pontuation)(Game * game);


#endif /* _GAME_H_ */
