#pragma once

#include "rtc.h"


/**
 * @brief struct used to save the LearderBoard information
 * 
 */
struct Point
{   
    /** @brief date and hour when the score have been make */
    Date_t date;
    /** @brief score make in that run */
    long long unsigned int score; 
};

/**
 * @brief - enum used as state machine that represents the actual state of the screen 
 */
typedef enum {MENU, /**< @brief menu screen */ 
              GAME, /**< @brief game screen */ 
              MULTIPLAYER, /**< @brief multiplayer screen */ 
              SCORE, /**< @brief score screen */ 
              MULTI_SCORE, /**< @brief multi_score screen */ 
              LEADERBOARD, /**< @brief leaderboard screen */ 
              LOADING, /**< @brief loading screen */ 
              EXIT /**< @brief exit screen */ 
            } screen_state_t;

//game var
/**
 * @brief enum used as state machine that represents the actual state of game
 */
typedef enum { CATCH, /**< @brief singleplayer: catch state   -   multiplayer: both catch state*/ 
               DROP, /**< @brief singleplayer: drop state   -    multiplayer: both drop state*/  
               DROP1, /**< @brief singleplayer: nothing   -    multiplayer: player1 drop state, player2 catch state*/  
               DROP2 /**< @brief singleplayer: nothing    -    multiplayer: player2 drop state, player1 catch state*/  
               } game_state_t;


//keyboard var
/**
 * @brief - enum used to analise the interrupt received from the keyboard
 */
typedef enum {KBC_INVALID,/**< @brief invalid data */ 
              A_DOWN, /**< @brief A key down*/
              A_UP, /**< @brief A key up*/
              D_DOWN, /**< @brief D key down*/
              D_UP, /**< @brief D key up*/
              W_DOWN, /**< @brief W key down*/
              W_UP, /**< @brief W key up*/
              S_DOWN, /**< @brief S key down*/
              S_UP, /**< @brief S key up*/
              UP_DOWN, /**< @brief arrowUp key down or W key down received from the uart*/
              UP_UP, /**< @brief arrowUp key up or W key up received from the uart*/
              LEFT_DOWN, /**< @brief arrowLeft key down or A key down received from the uart*/
              LEFT_UP, /**< @brief arrowLeft key up or A key up received from the uart*/
              RIGHT_DOWN, /**< @brief arrowRight key down or D key down received from the uart*/
              RIGHT_UP, /**< @brief arrowRight key up or D key up received from the uart*/
              DOWN_DOWN, /**< @brief arrowDown key down or S key down received from the uart*/
              DOWN_UP, /**< @brief arrowDown key up or S key up received from the uart*/
              ESC_DOWN, /**< @brief ESC key down*/
              ENTER_DOWN /**< @brief ENTER key down*/
              } key_event;

//mouse var
/**
 * @brief - enum used to analise the interrupt recived by the mouse
 */
typedef enum {INVALID, /**< @brief invalid data */ 
              RDOWN, /**< @brief right botton down*/
              RUP, /**< @brief right botton up*/
              LDOWN, /**< @brief letf botton down*/
              LUP, /**< @brief left botton up*/
              MDOWN,  /**< @brief middle botton down*/
              MUP, /**< @brief middle botton up*/
              MOVE /**< @brief moving mouse*/
              } mouse_event_type;

//KBC
/**
 * @brief  - validate the interrupt recived from the keyboard
 * 
 * @param size - current size of the bytes array
 * @param bytes - pointer to the first elemente of the array that contain all teh info
 */
void kbc_interrupt_validation(unsigned size ,uint8_t bytes[]);

/**
 * @brief - will generate the key event related to the key pressed
 * 
 * @param code - bytes last readed code
 * @param event - variavel that will have the event epecified
 * @return int - 0 up on success
 */
int kbc_get_event(uint8_t code,key_event * event);

/**
 * @brief - make all the changes in the game related to the key event received
 * 
 * @param event - event related to the key pressed
 * @return int 
 */
int kbc_event_handler(key_event event);

// MOUSE
/**
 * @brief - generate mouse event related to the received packet from the mouse
 * 
 * @param pp - packet that has been generated
 * @param event - event that come from the packet received
 * @return int - 0 up on success
 */
int mouse_get_event_type(struct packet *pp,mouse_event_type * event);

/**
 * @brief - make all the changes in the game related to the mouse event received
 * 
 * @param pp - packet that has been generated
 * @param event - event that come from the packet received
 * @return int - 0 up on success
 */
int mouse_event_handler(struct packet *pp,mouse_event_type event);

/**
 * @brief - depending of the game state the function will make there changes
 * 
 * @param score - pointer to the game score
 * @param clock_counter - pointer to the game clock_counter
 */
void handle_game_changes(long long unsigned int * score, int * clock_counter);

/**
 * @brief Set game drop state 
 * 
 * @param type - type of the drop: 0-> indefined __  1-> gold  __  2-> clock
 * @param idx - pos of the obj in the array
 */
void set_drop_state(int type, int idx);

/**
 * @brief - Set game drop state 
 * 
 */
void set_catch_state();

/**
 * @brief - used to handle changes(clicks) in menu screen
 * 
 * @param date - date given by the rtc
 */
void handle_menu_screen(Date_t * date);

/**
 * @brief Set the multiplayer drop state 
 * 
 * @param player - player that whill be set drop state
 * @param type - cathed object tipe
 * @param idx - idx of the catched obj
 */
void set_multiplayer_drop_state(int player,int type, int idx);

/**
 * @brief Set the multiplayer catch state  
 * 
 * @param player - player that whill be set catch state
 */
void set_multiplayer_catch_state(int player);

/**
 * @brief - handles multiplayer game changes
 * 
 * @param score - score of player1
 * @param clock_counter - clock counter of player1
 * @param score2 - score of player2
 * @param clock_counter2 - clock counter of player2
 */
void handle_multiplayer_changes(long long unsigned int * score,int * clock_counter,long long unsigned int * score2,int * clock_counter2);

/**
 * @brief - transforme key_byte in keyboard event
 * 
 * @param code - key_byte code
 * @param event - correspondent event 
 * @return int - 1 if fail other wise 0
 */
int key_byte_get_event(uint8_t code,key_event * event);
