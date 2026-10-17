#ifndef _LCOM_KBC_STATE_H_
#define _LCOM_KBC_STATE_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "src/include/controllers/kbc/KBCInput.h"
#include "CharacterState.h"

/*!
 * @struct KBCState
 * @brief Represents the state and behavior of the keyboard and mouse controller (KBC).
 *
 * This structure encapsulates the state of the keyboard and mouse input handling,
 * providing function pointers for updating the state, advancing to the next frame,
 * and destroying the state object.
 */
struct KBCState{
    /*!
     * @brief Updates the KBC state based on the provided KBCInput.
     *
     * Processes the input from the keyboard or mouse and updates the internal state accordingly.
     *
     * @param[in,out] state Pointer to the KBCState object.
     * @param[in] input Pointer to the KBCInput object containing the input event.
     * @return true if the state was updated successfully, false otherwise.
     */
    bool (*updateState)(struct KBCState*, const KBCInput*);

    /*!
     * @brief Advances the KBC state to the next frame.
     *
     * Performs per-frame updates for the keyboard and mouse state, such as handling continuous input.
     *
     * @param[in,out] state Pointer to the KBCState object.
     */
    void (*nextFrame)(struct KBCState*);

    /*!
     * @brief Destroys the KBCState object and frees its resources.
     *
     * @param[in,out] state Pointer to the KBCState object to destroy.
     */
    void (*destroy)(struct KBCState*);

    void *data; /*!< Pointer to the internal data structure. */
};

/*!
 * @typedef KBCState
 * @brief Typedef for struct KBCState.
 */
typedef struct KBCState KBCState;

/*!
 * @brief Constructs a KBCState object.
 *
 * Allocates and initializes a KBCState object, setting up the internal keyboard and mouse state handlers.
 *
 * @return Pointer to the newly created KBCState object, or NULL on failure.
 */
KBCState *(kBCStateConstructor)();

#endif
