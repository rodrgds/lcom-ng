#ifndef _LCOM_CHARACTER_STATE_H_
#define _LCOM_CHARACTER_STATE_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

/*!
 * @var CharacterState* playableCharacterState
 * @brief Pointer to the main playable character's state.
 */
extern struct CharacterState* playableCharacterState;

/*!
 * @var MouseState* mouseState
 * @brief Pointer to the mouse state object.
 */
extern struct MouseState* mouseState;

/*!
 * @var CharacterState* characters[10000]
 * @brief Array of pointers to all character states in the game.
 */
extern struct CharacterState* characters[10000];

/*!
 * @var uint16_t characterNum
 * @brief Number of characters currently in the game.
 */
extern uint16_t characterNum;

/*!
 * @enum ScreenDirection
 * @brief Possible movement directions for characters.
 */
enum ScreenDirection{
    ScreenUp,    /*!< Move up */
    ScreenDown,  /*!< Move down */
    ScreenLeft,  /*!< Move left */
    ScreenRight, /*!< Move right */
    ScreenNo     /*!< No movement */
};

/*!
 * @enum XPMType
 * @brief Types of XPM sprites used for characters and objects.
 */
enum XPMType{
    XPMWall,      /*!< Wall object */
    XPMHero,      /*!< Hero character */
    XPMLife,      /*!< Life collectible */
    XPMReload,    /*!< Reload collectible */
    XPMChest,     /*!< Chest object */
    XPMMonsterI,  /*!< Monster character */
    XPMBullet     /*!< Bullet object */
};

/*!
 * @struct MouseState
 * @brief Represents the state of the mouse cursor in the game.
 */
struct MouseState{
    /*!
     * @brief Sets the mouse position.
     * @param[in,out] state Pointer to the MouseState object.
     * @param[in] x X coordinate.
     * @param[in] y Y coordinate.
     */
    void (*setPosition)(struct MouseState*, uint16_t, uint16_t);

    /*!
     * @brief Draws the mouse cursor.
     * @param[in] state Pointer to the MouseState object.
     */
    void (*draw)(struct MouseState*);

    /*!
     * @brief Deletes the MouseState object and frees its resources.
     * @param[in,out] state Pointer to the MouseState object to delete.
     */
    void (*delete)(struct MouseState*);

    /*!
     * @brief Gets the X coordinate of the mouse.
     * @param[in] state Pointer to the MouseState object.
     * @return X coordinate.
     */
    uint16_t (*getMouseX)(struct MouseState*);

    /*!
     * @brief Gets the Y coordinate of the mouse.
     * @param[in] state Pointer to the MouseState object.
     * @return Y coordinate.
     */
    uint16_t (*getMouseY)(struct MouseState*);

    void *data; /*!< Pointer to the internal data structure. */
};

/*!
 * @struct CharacterState
 * @brief Represents the state and behavior of a character or object in the game.
 */
struct CharacterState{
    /*!
     * @brief Gets the X coordinate of the character.
     * @param[in] state Pointer to the CharacterState object.
     * @return X coordinate.
     */
    int16_t (*getX)(const struct CharacterState*);

    /*!
     * @brief Gets the Y coordinate of the character.
     * @param[in] state Pointer to the CharacterState object.
     * @return Y coordinate.
     */
    int16_t (*getY)(const struct CharacterState*);

    /*!
     * @brief Gets the current movement direction of the character.
     * @param[in] state Pointer to the CharacterState object.
     * @return Current direction.
     */
    enum ScreenDirection (*getDirection)(const struct CharacterState*);

    /*!
     * @brief Gets the XPM type of the character.
     * @param[in] state Pointer to the CharacterState object.
     * @return XPM type.
     */
    enum XPMType (*getXPMType)(const struct CharacterState*);

    /*!
     * @brief Moves the character in the specified direction.
     * @param[in,out] state Pointer to the CharacterState object.
     * @param[in] direction Direction to move.
     */
    void (*move)(struct CharacterState*, const enum ScreenDirection);

    /*!
     * @brief Draws the character on the screen.
     * @param[in] state Pointer to the CharacterState object.
     */
    void (*draw)(const struct CharacterState*);

    /*!
     * @brief Destroys the CharacterState object and frees its resources.
     * @param[in,out] state Pointer to the CharacterState object to destroy.
     */
    void (*destroy)(struct CharacterState*);

    void *data; /*!< Pointer to the internal data structure. */
};

/*!
 * @typedef CharacterState
 * @brief Typedef for struct CharacterState.
 */
typedef struct CharacterState CharacterState;

/*!
 * @typedef MouseState
 * @brief Typedef for struct MouseState.
 */
typedef struct MouseState MouseState;

/*!
 * @brief Constructs a CharacterState object.
 *
 * Allocates and initializes a CharacterState object at the specified position and with the given XPM type.
 *
 * @param[in] x X coordinate.
 * @param[in] y Y coordinate.
 * @param[in] xpm XPM type.
 * @return Pointer to the newly created CharacterState object, or NULL on failure.
 */
CharacterState *(characterStateConstructor)(int16_t x, int16_t y, enum XPMType xpm);

/*!
 * @brief Constructs a MouseState object.
 *
 * Allocates and initializes a MouseState object.
 *
 * @return Pointer to the newly created MouseState object, or NULL on failure.
 */
MouseState *(mouseStateConstructor)();

/*!
 * @brief Draws the map from a file.
 *
 * Reads the map file and creates the corresponding character objects.
 *
 * @param[in] map Path to the map file.
 * @return 0 on success, non-zero otherwise.
 */
int (draw_map)(char* map);

/*!
 * @brief Draws the game information (bullets, life, score) on the screen.
 */
void (draw_information)();

/*!
 * @brief Destroys all character and map objects, freeing their resources.
 */
void (destroyMap)();

/*!
 * @brief Reloads the map from the specified file, resetting game state.
 *
 * @param[in] path Path to the map file.
 * @return 0 on success, non-zero otherwise.
 */
int (reloadMap)(char* path);

/*!
 * @brief Moves all monster characters in the game.
 */
void (moveMonsters)();

/*!
 * @brief Checks for collisions between the hero and reload collectibles.
 */
void (checkReloadCollision)();

/*!
 * @brief Resolves a collision between a bullet and another character.
 *
 * @param[in] state Pointer to the bullet CharacterState.
 * @param[in] i Index of the character to check collision with.
 * @return Index of the character after collision resolution.
 */
uint16_t (resolveBulletCollision)(CharacterState* state, uint16_t i);

/*!
 * @brief Moves all bullet characters in the game.
 */
void (moveBullets)();

/*!
 * @brief Shoots a bullet in the specified direction from the hero's position.
 *
 * @param[in] direction Direction to shoot the bullet.
 */
void (shootBullet)(enum ScreenDirection direction);

/*!
 * @brief Allows the specified character to move in the next frame.
 *
 * @param[in,out] character Pointer to the CharacterState object.
 */
void (allowMovement)(CharacterState* character);

#endif
