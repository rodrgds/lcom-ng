#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_


#include <minix/sysutil.h>
#include <lcom/lcf.h>

/**
     * @brief Subscribes and enables Timer interrupts. 
     * 
     * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int (timer_subscribe_int)(uint8_t *bit_no);

/**
     * @brief Unsubscribes Timer interrupts. 
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int (timer_unsubscribe_int)();

/**
     * @brief  Timer interrupt handler.
     * Increments counter
     * 
**/
void (timer_int_handler)();

/**
     * @brief  Increments elapsed time count and calls multiple functions.
     * 
     * Every second, call the functions:
     * hero_actions();
     * check_bomb();
     * move_monsters_green();
     * rtc_ih();
     * 
     * Every 1/6 of a second, it calls the functions:
     * move_monsters_red();
     * check_unlimited_bombs();
     * check_game();
     * 
     * Every interrupt:
     * Calls the interrupt handler
     * Copy the data from the double_buffer to the frame_buffer
     * 
     * @return Return 0 upon success and non-zero otherwise
**/
int (timer_update)();

#endif /* __TIMER_H */
