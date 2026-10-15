#ifndef INPUT_H
#define INPUT_H

#include "../game/gamestate.h"

/**
 * @brief Handles keyboard input for single player classic mode.
 * 
 * @param gs Pointer to the current game state.
 * @return int Status code (0 for success, non-zero for error).
 */
int handleKeyboardInputSinglePlayerClassic(GameState *gs);

/**
 * @brief Handles mouse input for single player classic mode.
 * 
 * @param gs Pointer to the current game state.
 * @return int Status code (0 for success, non-zero for error).
 */
int handleMouseInputSinglePlayerClassic(GameState *gs);

/**
 * @brief Handles timer input for single player classic mode.
 * 
 * @param gs Pointer to the current game state.
 * @return int Status code (0 for success, non-zero for error).
 */
int handleTimerInputSinglePlayerClassic(GameState *gs);

/**
 * @brief Handles keyboard input for single player IO mode.
 * 
 * @param gs Pointer to the current game state.
 * @return int Status code (0 for success, non-zero for error).
 */
int handleKeyboardInputSinglePlayerIO(GameState *gs);

/**
 * @brief Handles mouse input for single player IO mode.
 * 
 * @param gs Pointer to the current game state.
 * @return int Status code (0 for success, non-zero for error).
 */
int handleMouseInputSinglePlayerIO(GameState *gs);

/**
 * @brief Handles timer input for single player IO mode.
 * 
 * @param gs Pointer to the current game state.
 * @return int Status code (0 for success, non-zero for error).
 */
int handleTimerInputSinglePlayerIO(GameState *gs);

/**
 * @brief Handles keyboard input for local multiplayer mode.
 * 
 * @param gs Pointer to the current game state.
 * @return int Status code (0 for success, non-zero for error).
 */
int handleKeyboardInputMultiPlayerLocal(GameState *gs);

/**
 * @brief Handles mouse input for local multiplayer mode.
 * 
 * @param gs Pointer to the current game state.
 * @return int Status code (0 for success, non-zero for error).
 */
int handleMouseInputMultiPlayerLocal(GameState *gs);

/**
 * @brief Handles timer input for local multiplayer mode.
 * 
 * @param gs Pointer to the current game state.
 * @return int Status code (0 for success, non-zero for error).
 */
int handleTimerInputMultiPlayerLocal(GameState *gs);

/**
 * @brief Handles keyboard input for multiplayer mode.
 * 
 * @param gs Pointer to the current game state.
 * @return int Status code (0 for success, non-zero for error).
 */
int handleKeyboardInputMultiPlayer(GameState *gs);

/**
 * @brief Handles mouse input for multiplayer mode.
 * 
 * @param gs Pointer to the current game state.
 * @return int Status code (0 for success, non-zero for error).
 */
int handleMouseInputMultiPlayer(GameState *gs);

/**
 * @brief Handles timer input for multiplayer mode.
 * 
 * @param gs Pointer to the current game state.
 * @return int Status code (0 for success, non-zero for error).
 */
int handleTimerInputMultiPlayer(GameState *gs);

/**
 * @brief Handles input from the serial port.
 * 
 * @return int Status code (0 for success, non-zero for error).
 */
int handleSerialPortInput();

#endif // INPUT_H
