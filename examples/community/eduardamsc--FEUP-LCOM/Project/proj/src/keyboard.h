/**
 * @brief Subscribes and enables keyboard
 * @return Bit order in interrupt mask, 1 upon failure to subscribe
 */
int keyboard_subscribe_int();

/**
 * @brief Unsubscribes keyboard
 * @return 1 upon failure to unsubscribe
 */
int keyboard_unsubscribe_int();

/**
 * @brief Gets scan code from the pressed on the keyboard
 * @return Scan code read
 */
unsigned long keyboard_read();
