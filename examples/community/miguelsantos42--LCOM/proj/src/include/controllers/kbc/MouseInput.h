#ifndef _LCOM_MOUSE_INPUT_H_
#define _LCOM_MOUSE_INPUT_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

/*!
 * @struct MouseInput
 * @brief Represents a formatted mouse input, providing access to button states and movement deltas.
 *
 * This structure encapsulates mouse input data and provides function pointers
 * for querying button states, movement, and for destroying the input object.
 */
struct MouseInput{
    /*!
     * @brief Checks if the left mouse button is pressed.
     * @param[in] input Pointer to the MouseInput object.
     * @return true if the left button is pressed, false otherwise.
     */
    bool (*leftPressed)(const struct MouseInput*);

    /*!
     * @brief Checks if the middle mouse button is pressed.
     * @param[in] input Pointer to the MouseInput object.
     * @return true if the middle button is pressed, false otherwise.
     */
    bool (*middlePressed)(const struct MouseInput*);

    /*!
     * @brief Checks if the right mouse button is pressed.
     * @param[in] input Pointer to the MouseInput object.
     * @return true if the right button is pressed, false otherwise.
     */
    bool (*rightPressed)(const struct MouseInput*);

    /*!
     * @brief Gets the X position of the mouse.
     * @param[in] input Pointer to the MouseInput object.
     * @return The X position.
     */
    int16_t (*getX)(const struct MouseInput*);

    /*!
     * @brief Gets the Y position of the mouse.
     * @param[in] input Pointer to the MouseInput object.
     * @return The Y position.
     */
    int16_t (*getY)(const struct MouseInput*);

    /*!
     * @brief Destroys the MouseInput object and frees its resources.
     * @param[in,out] input Pointer to the MouseInput object to destroy.
     */
    void (*destroy)(struct MouseInput*);

    void *data; /*!< Pointer to the internal data structure. */
};

/*!
 * @typedef MouseInput
 * @brief Typedef for struct MouseInput.
 */
typedef struct MouseInput MouseInput;

/*!
 * @brief Constructs a MouseInput object from a mouse packet.
 *
 * Allocates and initializes a MouseInput object using the provided mouse packet.
 *
 * @param[in] mousePacket Pointer to the mouse packet to be formatted.
 * @return Pointer to the newly created MouseInput object, or NULL on failure.
 */
MouseInput *(mouseConstructor)(const struct packet *mousePacket);

/*!
 * @brief Creates a copy of a MouseInput object.
 *
 * Allocates and initializes a new MouseInput object as a copy of the original input.
 *
 * @param[in] originalInput Pointer to the MouseInput object to copy.
 * @return Pointer to the newly created MouseInput copy, or NULL on failure.
 */
MouseInput *(mouseCopyConstructor)(const MouseInput *originalInput);

#endif
