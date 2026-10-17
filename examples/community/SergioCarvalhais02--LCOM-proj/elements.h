#ifndef _ELEMENTS_H_
#define _ELEMENTS_H_


#include <lcom/lcf.h>

#include <stdint.h>

/**
 * @brief Enumerated type for identifying the three different video graphics mode supported
 */
typedef enum Video_Mode {
    INDEXED, DIRECT_8_8_8, DIRECT_8_8_8_8
} Video_Mode;

/**
 * @brief Enumerated type for identifying the four different directions
 */
typedef enum Direction{
    RIGHT, LEFT, UP, DOWN
} Direction;

/**
 * @brief Enumerated type for identifying the four different mouse states
 */
typedef enum Mouse_State{
    M_IDLE, M_MOVING, LEFT_CLICK, RIGHT_CLICK
} Mouse_State;

/**
 * @brief Enumerated type for identifying the keyboard keys
 */
typedef enum Keyboard_Key{
    KB_IDLE, NO_KEY, A_KEY, B_KEY, C_KEY,
    D_KEY, E_KEY, F_KEY, G_KEY, H_KEY, 
    I_KEY, J_KEY, K_KEY, L_KEY, M_KEY, 
    N_KEY, O_KEY, P_KEY, Q_KEY, R_KEY, 
    S_KEY, T_KEY, U_KEY, V_KEY, W_KEY,
    X_KEY, Y_KEY, Z_KEY, SPACE_KEY, ESC_KEY, UP_KEY,
    LEFT_KEY, DOWN_KEY, RIGHT_KEY,
    ENTER_KEY, BKSPC_KEY
} Keyboard_Key;

/**
 * @brief Enumerated type for identifying button state
 */
typedef enum Button_State{
    ALIVE, DEAD, HIT
} Button_State;

/**
 * @brief Enumerated type for identifying type of the button
 */
typedef enum Button_Type{
    RED_MOLE, GREEN_MOLE, BLUE_MOLE, YELLOW_MOLE, BLACK_MOLE, WHITE_MOLE
} Button_Type;

/**
 * @brief Struct for defining a position
 */
typedef struct Position{
    uint16_t x; /*!< x coordinate */
    uint16_t y; /*!< y coordinate */
} Position;

/**
 * @brief Struct for defining a rectangle
 */
typedef struct Rectangle {
    Position pos; /*!< position of the first rectangle coordinate */
    uint16_t width; /*!< width of the rectangle */
    uint16_t height; /*!< height of the rectangle */
    uint32_t color; /*!< color of the rectangle */
    bool active; /*!< true if the rectangle is to be displayed, false otherwise */
} Rectangle;

/**
 * @brief Struct for defining the mouse
 */
typedef struct Mouse {
    Rectangle hitbox; /*!< rectangle corresponding to the mouse */
    Mouse_State mouse_state; /*!< state of the mouse (alive, dead or hit) */
} Mouse;

/**
 * @brief Struct for defining a button
 */
typedef struct Button {
    uint32_t counting; /*!< incrementing/decrementing value used to keep track of how much time has passed */
    uint32_t counting_value; /*!< time for the button to appear since it disapeared */
    uint32_t time_to_appear[10]; /*!< time for each type of button of the next_type_list to appear since it disapeared */
    uint16_t next_idx; /*!< index of the next type of button to appear in the next_type_list */
    Rectangle hitbox; /*!< rectangle corresponding to the button */
    Button_State button_state; /*!< time left for the button to appear */
    Button_Type button_type; /*!< type of the button */
    Button_Type next_type_list[10]; /*!< list of the type of the buttons to appear after the current be disabled */
    uint8_t idx_type; /*!< type of the button in this index */
} Button;

/**
 * @brief Struct for defining text
 */
typedef struct Text {
    Keyboard_Key key_list[10]; /*!< list of keyboard keys in the text */
    uint8_t cur_key; /*!< current key index */
} Text;

/**
 * @brief Struct for defining a score saved
 */
typedef struct Score {
    Keyboard_Key name[10]; /*!< list of keyboard keys containing the name saved in the leaderboard */
    uint16_t score; /*!< actual number corresponding to the pontuation */
} Score;

/**
 * function responsible for moving the rectangle corresponding to the mouse
 * @param r memory address of the rectangle that will be filled with its new position
 * @param d direction in which the rectangle moves
 * @return return 0 upon success and non-zero otherwise
*/
int (move_rectangle)(Rectangle * r, Direction d);

/**
 * function that checks if the position of a rectangle collides with the position of another (to be used after mouse clicks)
 * @param hitbox1 address of one of the rectangles whose positioning is to be checked
 * @param hitbox2 address of the other rectangle whose positioning is to be checked
 * @return returns true if the rectangles intersect, false otherwise
*/
bool (hit)(Rectangle * hitbox1, Rectangle * hitbox2);

/**
 * function that checks if, after a mouse click or the recession of a specific keyboard key based on the type of the button, the mouse rectangle intersects the rectangle of the button
 * @param hitbox1 address of one of the rectangles whose positioning is to be checked
 * @param hitbox2 address of the other rectangle whose positioning is to be checked
 * @param button_type address corresponding to the type (color) of button based on which the character that is to be received from the keyboard will be defined
 * @param click true if the left mouse button was pressed, false otherwise
 * @param key key received from the keyboard whose correspondence will be checked in certain cases
 * @return returns true if the received key was the same as the requested one and the mouse and button rectangles intersect at that moment, false otherwise
*/
bool (hit_button)(Rectangle * hitbox1, Rectangle * hitbox2, Button_Type * button_type, bool click, Keyboard_Key key);

/**
 * function that updates the color of the button (color attribute of the rectangle hitbox) based on its button_type
 * @param vm graphical mode in which the game is running
 * @param button address of the button whose rectangle color is to be updated
 * @param mt type of button based on which the color will be defined
*/
void (update_button_type)(Video_Mode vm, Button * button, Button_Type mt);

/**
 * function that activates a button making the "active" attribute of the corresponding rectangle "true"
 * @param button address of the button to be enabled
*/
void (enable_button)(Button * button);

/**
 * function that disables the button if hit_button returns true, making the "active" attribute of the corresponding rectangle "false" and the counting attribute to 0, and also updating the button_type to the next one in the list
 * @param button address of the button that you want to deactivate
 * @param hit true if the hit_button function returns true for the button in question, false otherwise
*/
void (disable_button)(Button * button, bool hit);

/**
 * function that informs a button upon a interrupt from timer or after a hit enabling or desabling it
 * @param button address of the button that is being tested if it has suffered a hit or if its counting attribute corresponds to its time_to_appear attribute
 * @param hitbox address of the rectangle of the button being checked
 * @param click true if the left mouse button was pressed, false otherwise
 * @param key key received from the keyboard
 * @return returns true if the button has been disabled, false if it has been enabled
*/
bool (alert_button)(Button * button, Rectangle * hitbox, bool click, Keyboard_Key key);

/**
 * function that assigns a short random time to the time_to_appear attribute of the button
 * @param button address of the button to create time_to_appear
*/
void (create_time_to_appear)(Button * button);

/**
 * function that fills the button's next_type_list attribute with random button types
 * @param button address of the button to fill list next_type_list
*/
void (create_type_to_appear)(Button * button);


#endif /* _ELEMENTS_H_ */
