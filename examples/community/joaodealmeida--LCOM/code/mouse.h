#ifndef __Mouse_H
#define __Mouse_H

/*
 * @brief Subscribes and enables PS/2 (Mouse) interrupts
 * @return Returns bit order in interrupt mask; negative value on failure
 * */
int Mouse_subscribe_int(void);

/**
 * @brief Unsubscribes Ps/2 (Mouse) interrupts
 * @return Return 0 upon success and non-zero otherwise
 */
int Mouse_unsubscribe_int();

/**
 * @brief send command to Mouse
 * @param command to to be sent
 * @return Return 0 upon sucess and non-zero otherwise
 */
int Mouse_sendCommand(unsigned long commandArgument);

/**
 * @brief Enables stream mode N packages
 * @return Return 0 upon sucess and non-zero otherwise
 */
int Mouse_enable_stream();

/**
 * @brief Reads one byte and saves it on the packet[]
 * @return Return 0 upon sucess and non-zero otherwise
 */
int Mouse_readPacketByte();

/**
 * @brief Prints packet in a user friendly way
 */
void Mouse_displayPacket();

/**
 * @brief Gets Mouse configuration bytes
 */
int Mouse_getConfig();

/**
 * @brief Prints Mouse config
 */
void Mouse_displayConfig();

/**
 * @brief (deprecated) handler used by Mouse before having a generic one
 * @param cnt number of packets to count
 */
void Mouse_int_handler_temp(unsigned short cnt);

/**
 * @brief Mouse handler to be use in generic handler
 * @param bitmask the irq mask to check if the interruption was from the Mouse
 */
void Mouse_int_handler(unsigned int bitmask);

/**
 * @brief sets counter to 0 to start counting packages
 */
void Mouse_resetPacketsCounter() ;

/**
 * @brief gets number of packages counted by the package counter
 */
int Mouse_getPacketsCounter() ;

/**
 * @brief will enable the friendly display of received packages
 */
void Mouse_activatePacketsDisplay();

/**
 * @brief get value of last package x dislocation
 * @return last read package x dislocation
 */
int Mouse_getXdelta();
/**
 * @brief get value of last package y dislocation
 * @return last read package y dislocation
 */
int Mouse_getYdelta();

/**
 * @brief get the current state of the left Mouse button
 * @return 1 if pressed, 0 if not
 */
int Mouse_getLbuttonState();
/**
 * @brief get the current state of the right Mouse button
 * @return 1 if pressed, 0 if not
 */
int Mouse_getRbuttonState();
/**
 * @brief get the current state of the middle Mouse button
 * @return 1 if pressed, 0 if not
 */
int Mouse_getMbuttonState();
/**
 * @brief check if a complete package was read (packetIndice=0)
 * @return 0 if finished reading a complete package, 1 if is in the middle of reading one
 */
short Mouse_packageRead();

#endif
