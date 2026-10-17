#ifndef _LCOM_SP_H_
#define _LCOM_SP_H_


#include <lcom/lcf.h>
#include <minix/sysutil.h>

#define SP_CLEAR        255
#define SP_FIRST_INT    254
#define SER_IRQ         4  

/// @brief send data to the opposite serial port
/// @param data data sent 
/// @return 1 in case of success, and 0 if there´s any problem with the system call
int (sp_send_data)(uint8_t data);

/// @brief receives data from the opposite serial port
/// @return 1 in case of success, and 0 if there´s any problem with the system call
int (sp_process_data)();

/// @brief subscribes the serial port interrupts
/// @return 1 in case of success, and 0 if there´s any problem with the system call
int (sp_subscribe_interrupts)();

/// @brief subscribes the serial port interrupts
/// @return 1 in case of success, and 0 if there´s any problem with the system call
int (sp_unsubscribe_interrupts)();

#endif

