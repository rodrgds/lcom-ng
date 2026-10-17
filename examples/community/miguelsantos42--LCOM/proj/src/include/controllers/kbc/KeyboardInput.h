#ifndef _LCOM_KEYBOARD_INPUT_H_
#define _LCOM_KEYBOARD_INPUT_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

/*!
 * @struct KeyboardInput
 * @brief Represents a formatted keyboard input, providing access to key state and value.
 *
 * This structure encapsulates keyboard input data and provides function pointers
 * for querying key state, retrieving the key value, and for destroying the input object.
 */
struct KeyboardInput {
    /*!
     * @brief Checks if the key was released.
     * @param[in] input Pointer to the KeyboardInput object.
     * @return true if the key was released, false otherwise.
     */
    bool (*wasReleased)(const struct KeyboardInput*);

    /*!
     * @brief Gets the string representation of the key.
     * @param[in] input Pointer to the KeyboardInput object.
     * @return Pointer to a string representing the key.
     */
    const char *(*getKey)(const struct KeyboardInput*);

    /*!
     * @brief Destroys the KeyboardInput object and frees its resources.
     * @param[in,out] input Pointer to the KeyboardInput object to destroy.
     */
    void (*destroy)(struct KeyboardInput*);

    void *data; /*!< Pointer to the internal data structure. */
};

/*!
 * @typedef KeyboardInput
 * @brief Typedef for struct KeyboardInput.
 */
typedef struct KeyboardInput KeyboardInput;

/*!
 * @brief Constructs a KeyboardInput object from a scan code sequence.
 *
 * Allocates and initializes a KeyboardInput object using the provided scan code and its size.
 *
 * @param[in] scanCode Pointer to the scan code sequence.
 * @param[in] scanCodeSize Size of the scan code sequence.
 * @return Pointer to the newly created KeyboardInput object, or NULL on failure.
 */
KeyboardInput *(keyboardConstructor)(const uint8_t *scanCode, const uint8_t scanCodeSize);

/*!
 * @brief Creates a copy of a KeyboardInput object.
 *
 * Allocates and initializes a new KeyboardInput object as a copy of the original input.
 *
 * @param[in] originalInput Pointer to the KeyboardInput object to copy.
 * @return Pointer to the newly created KeyboardInput copy, or NULL on failure.
 */
KeyboardInput *(keyboardCopyConstructor)(const KeyboardInput *originalInput);

#endif
