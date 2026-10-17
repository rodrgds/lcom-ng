#ifndef _LCOM_INSTRUCTIONS_STATE_H_
#define _LCOM_INSTRUCTIONS_STATE_H_

#include <lcom/lcf.h>

/*!
 * @struct InstructionsState
 * @brief Represents the state and behavior of the instructions screen.
 *
 * This structure encapsulates the instructions screen state and provides function pointers
 * for drawing the instructions and destroying the state object.
 */
struct InstructionsState{
    /*!
     * @brief Draws the instructions screen.
     * @param[in] state Pointer to the InstructionsState object.
     */
    void (*draw)(struct InstructionsState*);

    /*!
     * @brief Destroys the InstructionsState object and frees its resources.
     * @param[in,out] state Pointer to the InstructionsState object to destroy.
     */
    void (*destroy)(struct InstructionsState*);

    void *data; /*!< Pointer to the internal data structure. */
};

/*!
 * @typedef InstructionsState
 * @brief Typedef for struct InstructionsState.
 */
typedef struct InstructionsState InstructionsState;

/*!
 * @var InstructionsState* instructionState
 * @brief Global pointer to the current instructions state object.
 */
extern InstructionsState* instructionState;

/*!
 * @brief Constructs an InstructionsState object.
 *
 * Allocates and initializes an InstructionsState object.
 *
 * @return Pointer to the newly created InstructionsState object, or NULL on failure.
 */
InstructionsState *(instructionsStateConstructor)();

#endif
