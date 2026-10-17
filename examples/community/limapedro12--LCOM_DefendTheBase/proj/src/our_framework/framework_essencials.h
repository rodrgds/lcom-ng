#ifndef FRAMEWORK_ESSENCIALS_H
#define FRAMEWORK_ESSENCIALS_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include "device_handlers/timer.h"
#include "device_handlers/kbd.h"
#include "device_handlers/mouse.h"
#include "device_handlers/gpu.h"
#include "device_handlers/rtc.h"

/**
 * @brief struct that represents a position
 * @param x x coordinate
 * @param y y coordinate
 */
typedef struct {
    int x;
    int y;
} position;

/**
 * @brief struct that represents a tower
 * @param x x coordinate
 * @param y y coordinate
 * @param new true if the tower is new, false otherwise
 * @param placed true if the tower is placed, false otherwise
 */
typedef struct {
    int level;
    int x;
    int y;
    bool new;
    bool placed;
} tower;

/**
 * @brief struct that represents an enemy
 * @param x x coordinate
 * @param y y coordinate
 * @param hp health points
 */
typedef struct {
    int x;
    int y;
    int hp;
} enemy;

/**
 * @brief Runs the game
 * 
 * This function runs the function given as a parameter 60 times per second and responds to I/O interrupts.
 * 
 * @param func 
 * @return int 
 */
int run(void (*func)());

/**
 * @brief Quits the game
 * 
 * This function sets the game_running variable to false, which will cause the run function to stop running.
 * 
 */
void quit();

/**
 * @brief Get the current time counter
 * 
 * This function returns the current time counter, which is incremented 60 times per second.
 * It is used to "initialize" a timer that can be used to check if a certain amount of time has passed,
 * using the is_time_interval_elapsed_seconds and is_time_interval_elapsed_milliseconds functions.
 * 
 * @return int 
 */
int get_time_counter();

/**
 * @brief Checks if a certain amount of seconds has passed
 * 
 * This function checks if a certain amount of seconds has passed since the start_time.
 * 
 * @param start_time 
 * @param interval 
 * @return true 
 * @return false 
 */
bool is_time_interval_elapsed_seconds(int start_time, int interval);

/**
 * @brief Checks if a certain amount of milliseconds has passed
 * 
 * This function checks if a certain amount of milliseconds has passed since the start_time.
 * 
 * @param start_time 
 * @param interval 
 * @return true 
 * @return false 
 */
bool is_time_interval_elapsed_milliseconds(int start_time, int interval);

/**
 * @brief Checks if a certain key is pressed or released
 * 
 * This function returns true if the key with the given scancode is pressed or released(depending onthe type of scancode)
 * 
 * @param scancode 
 * @return true 
 * @return false 
 */
bool is_key_pressed_code(uint8_t scancode);

/**
 * @brief Checks if a certain key is pressed or released
 * 
 * This function returns true if the key with the given character is pressed or released(depending on the isBreak parameter)
 * 
 * @param key 
 * @param isBreak 
 * @return true 
 * @return false 
 */
bool is_key_pressed(char key, bool isBreak);

/**
 * @brief Get the mouse position
 * 
 * @return position the mouse position
 */
position get_mouse_position();

/**
 * @brief Set the mouse position
 * 
 * This function sets the mouse position to the given coordinates.
 * 
 * @param x 
 * @param y 
 * @return true 
 * @return false 
 */
bool set_mouse_position(int x, int y);

/**
 * @brief Checks if the mouse is inside the screen
 * 
 * @param x_min 0
 * @param x_max 800
 * @param y_min 0
 * @param y_max 600
 * @return true if the mouse is inside the screen, false otherwise
 */
bool verify_mouse_limits(int x_min, int x_max, int y_min, int y_max);

/**
 * @brief Check if the middle button of the mouse is pressed
 * 
 * @return true if the middle button of the mouse is pressed, false otherwise
 */
bool is_mb_pressed();

/**
 * @brief Check if the left button of the mouse is pressed
 * 
 * @return true if the left button of the mouse is pressed, false otherwise
 */
bool is_lb_pressed();

/**
 * @brief Check if the right button of the mouse is pressed
 * 
 * @return true if the right button of the mouse is pressed, false otherwise
 */
bool is_rb_pressed();


/**
 * @brief Turns on graphics mode
 * 
 * This function turns on graphics mode, if it is not already on.
 * 
 */
void turn_on_graphics();

/**
 * @brief Draws a rectangle
 * 
 * This function draws a rectangle with the given parameters.
 * 
 * @param x 
 * @param y 
 * @param width 
 * @param height 
 * @param color 
 */
void draw_rectangle(int x, int y, int width, int height, uint32_t color);

/**
 * @brief paints the screen black
 * 
 */
void clear_screen();

/**
 * @brief Get the hours
 * 
 * @return uint hours
 */
uint get_hours();

/**
 * @brief Get the minutes
 * 
 * @return uint minutes
 */
uint get_minutes();

#endif
