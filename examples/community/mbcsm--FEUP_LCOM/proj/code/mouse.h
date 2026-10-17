#pragma once
/** @defgroup mouse mouse
 * @{
 *
 * utils for mouse
 */
/**
 * @brief Subscribes Mouse interrupts
 * @return bit_no
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribes Mouse interrupts
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_unsubscribe_int();

/**
 * @brief Read from the kbc buffer
 * @return Return buffer information
 */
uint32_t read_kbc();

/**
 * @brief makes a kbd kernel call
 * @return Return 0 upon success and non-zero otherwise
 */
int write_kbc_command(uint8_t command);

int write_kbc_argument(uint8_t argument);

int write_kbc_cmd_byte(uint32_t cmd_byte);

/**
 * @brief makes a mouse kernel call
 * @return Return 0 upon success and non-zero otherwise
 */
int write_mouse_command(uint32_t command);

/**
 * @brief Disables mouse
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_disable_int();

/**
 * @brief Enables mouse
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_enable_int();

/**
 * @brief Enables Data report
 * @return Return 0 upon success and non-zero otherwise
 */
int enable_data_report();

/**
 * @brief Disables Data report
 * @return Return 0 upon success and non-zero otherwise
 */
int disable_data_report();

/**
 * @brief Set stream mode of the mouse
 * @return Return 0 upon success and non-zero otherwise
 */
int set_stream_mode();

/**
 * @brief Set remote mode of the mouse
 * @return Return 0 upon success and non-zero otherwise
 */
int set_remote_mode();

/**
 * @brief reads data from mouse buffer
 * @param packet pointer to the mouse packet
 * @return Return 0 upon success and non-zero otherwise
 */
int read_mouse_data(uint32_t *packet);

enum state {
    INIT,
	IDRAWU,
    DRAWINGUP,
	VERTEX,
	IDRAWD,
	DRAWINGDOWN,
    FINAL
};

enum event_type {
	LBDOWN,
	LBUP,
	MOVEUP,
	RESIDUAL,
	RBDOWN,
	RBUP,
	MOVEDOWN,
	OTHER
};

/**
 * @brief updates the state of the mouse
 * @param evt enum of the mouse events
 */
void updateState(enum event_type evt);

/**
 * @brief handles an event of moving and clicking to make an M on the screen with the mouse
 * @param packet pointer to the mouse packet
 * @param x minimum lenght of the arm
 * @param tolerance permited to the user
 * @return Return true upon success and false otherwise
 */
bool event(struct packet *pp, uint8_t x, uint8_t tolerance);


/**@}*/
