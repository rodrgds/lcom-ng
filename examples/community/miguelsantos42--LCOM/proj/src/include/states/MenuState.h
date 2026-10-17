#ifndef _LCOM_MENU_STATE_H_
#define _LCOM_MENU_STATE_H_

#include <lcom/lcf.h>

/*!
 * @enum MenuOption
 * @brief Represents the possible menu options.
 *
 * This enumeration defines the various options available in the menu, such as starting the game,
 * viewing instructions, accessing settings, or exiting the game.
 */
typedef enum {
    MenuOptionStart,        /*!< Start the game. */
    MenuOptionInstructions, /*!< View instructions. */
    MenuOptionSettings,     /*!< Access settings. */
    MenuOptionExit          /*!< Exit the game. */
} MenuOption;

/*!
 * @struct MenuState
 * @brief Represents the state and behavior of the menu screen.
 *
 * This structure encapsulates the menu screen state and provides function pointers
 * for drawing the menu, navigating options, selecting an option, and destroying the state object.
 */
struct MenuState {
    /*!
     * @brief Draws the menu screen.
     * @param[in,out] state Pointer to the MenuState object.
     * @return 0 on success, non-zero otherwise.
     */
    int (*draw)(struct MenuState*);

    /*!
     * @brief Gets the currently selected menu option.
     * @param[in] state Pointer to the MenuState object.
     * @return The currently selected menu option.
     */
    MenuOption (*getSelectedOption)(struct MenuState*);

    /*!
     * @brief Moves the selection up in the menu.
     * @param[in,out] state Pointer to the MenuState object.
     */
    void (*setUp)(struct MenuState*);

    /*!
     * @brief Moves the selection down in the menu.
     * @param[in,out] state Pointer to the MenuState object.
     */
    void (*setDown)(struct MenuState*);

    /*!
     * @brief Updates the menu selection based on mouse position.
     * @param[in,out] state Pointer to the MenuState object.
     * @param[in] x X coordinate of the mouse.
     * @param[in] y Y coordinate of the mouse.
     */
    void (*setMouse)(struct MenuState*, uint16_t x, uint16_t y);

    /*!
     * @brief Selects the currently highlighted menu option.
     * @param[in,out] state Pointer to the MenuState object.
     */
    void (*select)(struct MenuState*);

    /*!
     * @brief Destroys the MenuState object and frees its resources.
     * @param[in,out] state Pointer to the MenuState object to destroy.
     */
    void (*destroy)(struct MenuState*);

    void *data; /*!< Pointer to the internal data structure. */
};

/*!
 * @typedef MenuState
 * @brief Typedef for struct MenuState.
 */
typedef struct MenuState MenuState;

/*!
 * @var MenuState* menuState
 * @brief Global pointer to the current menu state object.
 */
extern MenuState* menuState;

/*!
 * @brief Constructs a MenuState object.
 *
 * Allocates and initializes a MenuState object.
 *
 * @return Pointer to the newly created MenuState object, or NULL on failure.
 */
MenuState* menuStateConstructor();

#endif
