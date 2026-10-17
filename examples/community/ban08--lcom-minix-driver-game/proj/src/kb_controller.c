#include "kb_controller.h"


/**
 * @brief Reads the status register of the keyboard controller.
 *
 * This function uses util_sys_inb to read the status register of the 8042 keyboard controller.
 *
 * @param status Pointer to a variable where the status byte will be stored.
 * @return 0 on success, non-zero on failure.
 */
int (readKBControllerstat)(uint8_t* status) 
{

    return util_sys_inb(KBC_STATUS_REGISTER, status);

}

/**
 * @brief Reads the output from the keyboard or mouse controller.
 *
 * This function checks the output buffer of the keyboard controller and reads the data if available.
 * It also performs error checking for parity, timeout, and data source (keyboard vs mouse).
 *
 * @param port The I/O port to read from.
 * @param output Pointer to a variable where the output data will be stored.
 * @param mouse Boolean indicating if the data is from the mouse (1) or keyboard (0).
 * @return 0 on success, non-zero on error (e.g., timeout, parity error, or invalid source).
 */
int readKBControlleroutput(uint8_t port, uint8_t *output, uint8_t mouse) 
{

    uint8_t status;

    uint8_t attempts = 10;
    
    for (int i = 0; i < attempts; i++){

        if (readKBControllerstat(&status) != 0) {
            printf("Error: Status!\n");
            return 1;
        }

        if ((status & FULL_OUTPUT_BUF) != 0) {
            if(util_sys_inb(port, output) != 0){
                printf("Error: FULL_OUTPUT_BUF!\n");
                return 1;
            }
            if((status & PARITY_ERR) != 0){
                printf("Error: PARITY_ERR!\n");
                return 1;
            }
            if((status & TIMEOUT_ERR) != 0){
                printf("Error: TIMEOUT_ERR!\n");
                return 1;
            }
            if (mouse && !(status & OUT_NOT_FOUND)) {
                printf("Error: OUT_NOT_FOUND_MOUSE\n");
                return 1;
            } 
            if (!mouse && (status & OUT_NOT_FOUND)) {
                printf("Error: OUT_NOT_FOUND\n");
                return 1;
            } 
            return 0; //se não existirem erros, return 0, success
        }


        tickdelay(micros_to_ticks(WAIT_KBCONTROLLER)); //20000 microsegundos, ou seja, 20ms, tal como especificado em "IBM's specification of the i8042 requires the keyboard to respond to a command in 20 ms"

    }
    return 1;
}

/**
 * @brief Writes a command byte to the keyboard controller.
 *
 * This function waits for the input buffer to be clear before sending a command byte
 * to the specified port.
 *
 * @param port The I/O port to write to.
 * @param cmd_byte The command byte to send.
 * @return 0 on success, non-zero on failure.
 */
int (writeKBControllercommand)(uint8_t port, uint8_t cmd_byte) 
{

    uint8_t status;

    uint8_t attempts = 10;

    for (int i = 0; i < attempts; i++){

        if (readKBControllerstat(&status) != 0){
            printf("Error: Status!\n");
            return 1;
        }

        if ((status & FULL_INPUT_BUF) == 0){

            if(sys_outb(port, cmd_byte) != 0){

                printf("Error: cmd_byte!\n");

                return 1;

            }

            return 0;
        }
        tickdelay(micros_to_ticks(WAIT_KBCONTROLLER));
        
    }
    
    return 1;

}
