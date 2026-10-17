#ifndef _LCOM_SETTINGS_STATE_H_
#define _LCOM_SETTINGS_STATE_H_

#include <lcom/lcf.h>

/*!
 * @enum Difficulty
 * @brief Represents the difficulty levels of the game.
 *
 * This enumeration defines the possible difficulty levels: Easy, Medium, and Hard.
 */
typedef enum {
    Easy,   /*!< Easy difficulty level. */
    Medium, /*!< Medium difficulty level. */
    Hard    /*!< Hard difficulty level. */
} Difficulty;

/*!
 * @struct SettingsState
 * @brief Represents the state and behavior of the settings screen.
 *
 * This structure encapsulates the settings screen state and provides function pointers
 * for drawing the settings, navigating options, selecting an option, and destroying the state object.
 */
struct SettingsState {
    /*!
     * @brief Gets the currently selected difficulty level.
     * @param[in] state Pointer to the SettingsState object.
     * @return The currently selected difficulty level.
     */
    Difficulty (*getSelectedDifficulty)(struct SettingsState*);

    /*!
     * @brief Moves the selection up in the settings menu.
     * @param[in,out] state Pointer to the SettingsState object.
     */
    void (*setUp)(struct SettingsState*);

    /*!
     * @brief Moves the selection down in the settings menu.
     * @param[in,out] state Pointer to the SettingsState object.
     */
    void (*setDown)(struct SettingsState*);

    /*!
     * @brief Updates the settings selection based on mouse position.
     * @param[in,out] state Pointer to the SettingsState object.
     * @param[in] x X coordinate of the mouse.
     * @param[in] y Y coordinate of the mouse.
     */
    void (*setMouse)(struct SettingsState*, uint16_t x, uint16_t y);

    /*!
     * @brief Selects the currently highlighted difficulty option.
     * @param[in,out] state Pointer to the SettingsState object.
     */
    void (*select)(struct SettingsState*);

    /*!
     * @brief Draws the settings screen.
     * @param[in,out] state Pointer to the SettingsState object.
     * @return 0 on success, non-zero otherwise.
     */
    int (*draw)(struct SettingsState*);

    /*!
     * @brief Destroys the SettingsState object and frees its resources.
     * @param[in,out] state Pointer to the SettingsState object to destroy.
     */
    void (*destroy)(struct SettingsState*);

    void *data; /*!< Pointer to the internal data structure. */
};

/*!
 * @typedef SettingsState
 * @brief Typedef for struct SettingsState.
 */
typedef struct SettingsState SettingsState;

/*!
 * @var SettingsState* settingsState
 * @brief Global pointer to the current settings state object.
 */
extern SettingsState* settingsState;

/*!
 * @var Difficulty selectedDifficulty
 * @brief Global variable holding the currently selected difficulty level.
 */
extern Difficulty selectedDifficulty;

/*!
 * @brief Constructs a SettingsState object.
 *
 * Allocates and initializes a SettingsState object.
 *
 * @return Pointer to the newly created SettingsState object, or NULL on failure.
 */
SettingsState *(settingsStateConstructor)();

/*!
 * @brief Gets the map file path corresponding to the specified difficulty level.
 *
 * Returns the file path of the map associated with the given difficulty level.
 *
 * @param[in] difficulty Difficulty level.
 * @return Pointer to the string containing the map file path.
 */
char* getMapByDifficulty(Difficulty difficulty);

#endif
