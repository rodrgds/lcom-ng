#ifndef _LCOM_KBC_INPUT_H_
#define _LCOM_KBC_INPUT_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "KeyboardInput.h"
#include "MouseInput.h"

/*!
 * @struct KBCInput
 * @brief Represents a formatted KBC (Keyboard/Mouse) input, providing access to the underlying device input.
 *
 * This structure encapsulates either a keyboard or mouse input and provides function pointers
 * for querying the type of input, accessing the underlying input object, and for destroying the input object.
 */
struct KBCInput{
    /*!
     * @brief Checks if the input is from an auxiliary device (mouse).
     * @param[in] input Pointer to the KBCInput object.
     * @return true if the input is from a mouse, false if from a keyboard.
     */
    bool (*isAux)(const struct KBCInput*);

    /*!
     * @brief Gets the underlying KeyboardInput object.
     * @param[in] input Pointer to the KBCInput object.
     * @return Pointer to the KeyboardInput object if the input is from a keyboard, NULL otherwise.
     */
    const KeyboardInput *(*getKeyboardInput)(const struct KBCInput*);

    /*!
     * @brief Gets the underlying MouseInput object.
     * @param[in] input Pointer to the KBCInput object.
     * @return Pointer to the MouseInput object if the input is from a mouse, NULL otherwise.
     */
    const MouseInput *(*getMouseInput)(const struct KBCInput*);

    /*!
     * @brief Destroys the KBCInput object and frees its resources.
     * @param[in,out] input Pointer to the KBCInput object to destroy.
     */
    void (*destroy)(struct KBCInput*);

    void *data; /*!< Pointer to the internal data structure. */
};

/*!
 * @typedef KBCInput
 * @brief Typedef for struct KBCInput.
 */
typedef struct KBCInput KBCInput;

/*!
 * @brief Constructs a KBCInput object from a KeyboardInput object.
 *
 * Allocates and initializes a KBCInput object using the provided KeyboardInput.
 *
 * @param[in] input Pointer to the KeyboardInput object to be wrapped.
 * @return Pointer to the newly created KBCInput object, or NULL on failure.
 */
KBCInput *(kBCKeyboardConstructor)(const KeyboardInput *input);

/*!
 * @brief Constructs a KBCInput object from a MouseInput object.
 *
 * Allocates and initializes a KBCInput object using the provided MouseInput.
 *
 * @param[in] input Pointer to the MouseInput object to be wrapped.
 * @return Pointer to the newly created KBCInput object, or NULL on failure.
 */
KBCInput *(kBCMouseConstructor)(const MouseInput *input);

/*!
 * @brief Creates a copy of a KBCInput object.
 *
 * Allocates and initializes a new KBCInput object as a copy of the original input.
 *
 * @param[in] originalInput Pointer to the KBCInput object to copy.
 * @return Pointer to the newly created KBCInput copy, or NULL on failure.
 */
KBCInput *(kBCCopyConstructor)(const KBCInput *originalInput);

#endif
