#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>

#include "mouse.h"

/** @defgroup mouse mouse
 * @{
 *
 * Functions for using the mouse
 */

int hook_id_mouse = 3;

int m_index = 0;
uint8_t m_byte;
uint8_t m_bytes[3];
struct packet m_packet;

/**
 * @brief Writes a command to the mouse.
 * 
 * @param command The command to be sent to the mouse.
 * @return int Returns 0 on success, 1 on failure.
 */
int (write_mouse) (uint8_t command) {
    int attemps_count = 10;
    uint8_t received_byte = NACK;

    while (attemps_count && (received_byte != ACK)) {
        if (write_kbc(KBC_CMD, WRITE_BYTE_MOUSE) != 0)
            return 1;
        if (write_kbc(IN_BUF, command) != 0)
            return 1;

        tickdelay(micros_to_ticks(20000));

        if (util_sys_inb(OUT_BUF, &received_byte) != 0)
            return 1;
            
        attemps_count--;
    }

    if (received_byte == ACK)
        return 0;

    return 1;
}


/**
 * @brief Subscribes and enables mouse interrupts.
 * 
 * @param irq_set Pointer to the memory where the IRQ bit mask will be stored.
 * @return int Returns 0 on success, 1 on failure.
 */
int (mouse_subscribe_int)(uint8_t* irq_set) {
    if (irq_set == NULL)
        return 1;

    *irq_set = BIT(hook_id_mouse);

    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse) != 0)
        return 1;

    return 0;
}

/**
 * @brief Unsubscribes mouse interrupts.
 * 
 * @return int Returns 0 on success, 1 on failure.
 */
int (mouse_unsubscribe_int)() {
    if (sys_irqrmpolicy(&hook_id_mouse) != 0)
        return 1;
    return 0;
}

/**
 * @brief Stores the byte received from the mouse.
 */
void (store_byte_mouse)() {
    if ((m_index == 0 && (m_byte & M_BYTE_1))     
                || (m_index > 0 && m_index < 3)) {        // Bit 3 of byte 1 is always 1
        m_bytes[m_index] = m_byte;
        m_index++;
    }
}

/**
 * @brief Mouse interrupt handler.
 * 
 * Reads the byte from the mouse and stores it.
 */
void (mouse_ih)() {
    if (read_kbc(OUT_BUF, &m_byte, 1) != 0)
        printf("Error reading a byte from mouse\n");
    else
        store_byte_mouse();
}

/**
 * @brief Stores the bytes received from the mouse in a packet structure.
 */
void (store_bytes_packet)() {
    for (int i = 0; i < 3; i++)
        m_packet.bytes[i] = m_bytes[i];
    
    m_packet.lb = m_bytes[0] & M_LB;
    m_packet.rb = m_bytes[0] & M_RB;
    m_packet.mb = m_bytes[0] & M_MB;

    m_packet.x_ov = m_bytes[0] & M_X_OVFL;
    m_packet.y_ov = m_bytes[0] & M_Y_OVFL;

    if (m_bytes[0] & M_MSB_X_DELTA) m_packet.delta_x = m_bytes[1] | 0xFF00;
    else m_packet.delta_x = m_bytes[1];

    if (m_bytes[0] & M_MSB_Y_DELTA) m_packet.delta_y = m_bytes[2] | 0xFF00;
    else m_packet.delta_y = m_bytes[2];
}


