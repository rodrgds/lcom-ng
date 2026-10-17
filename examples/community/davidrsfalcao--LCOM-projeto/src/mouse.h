#ifndef __MOUSE_H
#define __MOUSE_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <stdio.h>
#include <stdlib.h>
#include <minix/sysutil.h>

static int hook_id_mouse = 12;
unsigned long packet[3];
static int byte_number = 0;

/**
 * @brief Subscribes and enables Mouse interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int mouse_subscribe_int();
/**
 * @brief Unsubscribes Mouse interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_unsubscribe();



#endif
