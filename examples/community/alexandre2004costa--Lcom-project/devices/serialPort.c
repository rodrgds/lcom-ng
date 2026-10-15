
#include "serialPort.h"

/** @defgroup serialPort serialPort
  * @{
 */

Queue *receive;
Queue *transmit;
int hook_id = 4;
bool ableToSend = true;


/**
 * @brief Reads the content in the address and puts it in the command.
 * 
 * @param address Address of the register.
 * @param command Pointer to the variable where the content read from the register will be stored.
 * @return 0 in case of success, 1 otherwise.
 */
int (serialRead)(int address, uint8_t *command){
    if (address < 0 || address > 7) return 1;
    if (util_sys_inb(COM1_ADDRS + address, command) != 0){
        printf("Error in sysInB\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Writes the command to the specified address.
 * 
 * @param address Address of the register.
 * @param command Content to be written to the register.
 * @return 0 in case of success, 1 otherwise.
 */
int (serialWrite)(int address, uint8_t command){
    if (address < 0 || address > 7) return 1;
    if (sys_outb(COM1_ADDRS + address, command) != 0) return 1;
    return 0;
}

/**
 * @brief Configures the UART bitrate.
 * 
 * @param bitRate Desired bitrate.
 * @return 0 in case of success, 1 otherwise.
 */
int (configureBitrate)(int bitRate) {
    uint8_t command;
    // To get the default command in the line
    if (serialRead(LCR, &command) != 0) {
        printf("Error in serial read\n");
        return 1;
    }
    command |= LCR_DLAB; // To activate Dlab
    if (serialWrite(LCR, command) != 0) {
        printf("Error in serial write\n");
        return 1;
    }
    uint16_t divisor = 115200 / bitRate;
    uint8_t msb = 0;
    uint8_t lsb = 0;
    util_get_LSB(divisor, &lsb);
    util_get_MSB(divisor, &msb);
    if (serialWrite(DLL, lsb) != 0) {
        printf("Error in serial write\n");
        return 1;
    }
    if (serialWrite(DLM, msb) != 0) {
        printf("Error in serial write\n");
        return 1;
    }

    // Deactivate DLAB
    if (serialRead(LCR, &command) != 0) {
        printf("Error in serial read\n");
        return 1;
    }
    command &= ~LCR_DLAB; // To deactivate Dlab
    if (serialWrite(LCR, command) != 0) {
        printf("Error in serial write\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Sets the parameters for the UART such as number of bits per character and parity control.
 * 
 * @param numberBitsPerChar Number of bits per character (must be between 5 and 8).
 * @param parityControl Parity control type (0: no parity, 1: odd parity, 2: even parity, 3: all 1, 4: all 0).
 * @return 0 in case of success, 1 otherwise.
 */
int setParameters(int numberBitsPerChar, int parityControl) {
    uint8_t command = 0;
    if (serialRead(LCR, &command) != 0) return 1;
    command = command & 0xC4; // Clear the bits that will be changed
    if (numberBitsPerChar < 5 || numberBitsPerChar > 8) return 1;
    switch (numberBitsPerChar) {
        case 5:
            command |= LCR_BITS_PER_CHAR_5;
            break;
        case 6:
            command |= LCR_BITS_PER_CHAR_6;
            break;     
        case 7:
            command |= LCR_BITS_PER_CHAR_7;
            break;  
        case 8:
            command |= LCR_BITS_PER_CHAR_8;
            break;        
    }
    if (parityControl < 0 || parityControl > 4) return 1;
    switch (parityControl) {
        case 0:
            command |= LCR_NO_PARITY;
            break;
        case 1:
            command |= LCR_ODD_PARITY;
            break;
        case 2:
            command |= LCR_EVEN_PARITY;
            break;
        case 3:
            command |= LCR_BIT1_PARITY;
            break;     
        case 4:
            command |= LCR_BIT0_PARITY;
            break;               
    }
    if (serialWrite(LCR, command) != 0) {
        printf("Error in serialWrite\n");
        return 1;
    }
    return 0;
}


/**
 * @brief Initializes the serial port by setting up parameters, configuring the bitrate, and enabling FIFOs.
 * 
 * @return 0 in case of success, 1 otherwise.
 */
int (initialSetup)() {
    receive = createQueue();
    transmit = createQueue();
    if (setParameters(8, 1) != 0) {
        printf("Error in setting the parameters\n");
        return 1;
    }
    if (configureBitrate(115200) != 0) {
        return 1;
    }
    return enableFifos();
}

/**
 * @brief Enables serial port interrupts for data available, transmitter holding register empty, and receiver line status.
 * 
 * @return 0 in case of success, 1 otherwise.
 */
int (enableSerialInterrupts)() {
    return serialWrite(IER, IER_ENABLE_DATA_AVALBL_INT | IER_ENABLE_THR_EMPTY_INT | IER_ENABLE_RECEIVER_LS_INT);
}

/**
 * @brief Disables all serial port interrupts and clears the FIFO control register.
 * 
 * @return 0 in case of success, 1 otherwise.
 */
int (disableSerialInterrupts)() {
    serialWrite(IER, 0);
    serialWrite(FCR, 0);
    return 0;
}

/**
 * @brief Subscribes to serial port interrupts and performs the initial setup.
 * 
 * @param bit_no Pointer to store the bit number for the interrupt.
 * @return 0 in case of success, 1 otherwise.
 */
int (serial_subscribe)(uint8_t *bit_no) {
    *bit_no = BIT(hook_id);
    if (sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK) {
        printf("Failed in setPolicy\n");
        return 1;
    }

    if (initialSetup() != 0) {
        printf("Fail in initial Setup\n");
        return 1;
    }
    if (enableSerialInterrupts() != 0) {
        printf("Fail in enabling interrupts\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Unsubscribes from serial port interrupts and disables them.
 * 
 * @return 0 in case of success, 1 otherwise.
 */
int (serial_unsubscribe)() {
    if (disableSerialInterrupts() != 0) {
        printf("Failed in disabling serial interrupts\n");
        return 1;
    }
    if (sys_irqrmpolicy(&hook_id) != 0) {
        printf("Failed in removePolicy\n");
        return 1;
    }
    return 0;
}

/**
 * @brief Interrupt handler for serial port.
 * 
 * @return 0 in case of success, 1 otherwise.
 */
int (serial_ih)() {
    uint8_t command;
    if (serialRead(IIR, &command) != 0) {
        return 1;
    }
    printf("command %d\n ", command);
    command &= 0x0f;
    if (!(command & IIR_NO_INTERRUPT)) {
        switch (command) {
            case IIR_RECEIVED_DATA_AVLBL:
                readByte();
                break;
            case IIR_THR_EMPTY:
                ableToSend = true;
                sendByte();
                break;
            case IIR_CHARACT_TIMEOUT:
                ableToSend = true;
                readByte();
                break;
            default:
                break;
        }
    }
    return 0;
}

/**
 * @brief Checks if the serial port is ready to write.
 * 
 * @return 0 if ready to write, 1 otherwise.
 */
int ableToWrite() {
    uint8_t status = 0;
    if (serialRead(LSR, &status) != 0) {
        return 1;
    }
    return (status & LSR_THR_EMPTY) ? 0 : 1;
}

/**
 * @brief Checks if there is data available to read from the serial port.
 * 
 * @return 0 if data is available, 1 otherwise.
 */
int ableToRead() {
    uint8_t status = 0;
    if (serialRead(LSR, &status) != 0) {
        return 1;
    }
    return (status & LSR_DATA_FOR_RECEIVING) ? 0 : 1;
}

/**
 * @brief Sends a byte from the transmit queue if it is not empty and the serial port is ready to write.
 * 
 * @return 0 in case of success, 1 otherwise.
 */
int (sendByte)() {
    if (isEmpty(transmit) || ableToWrite() != 0) {
        return 1;
    }
    uint8_t byte = dequeue(transmit);
    if (serialWrite(THR, byte) != 0) {
        printf("Error in serial writing\n");
        return 1;
    }
    ableToSend = false;
    return 0;
}

/**
 * @brief Reads a byte from the serial port if data is available and adds it to the receive queue.
 * 
 * @return 0 in case of success, 1 otherwise.
 */
int (readByte)() {
    if (ableToRead() != 0) {
        printf("Cannot read byte at the moment\n");
        return 1;
    }
    uint8_t byte;
    if (serialRead(RBR, &byte) != 0) {
        return 1;
    }
    enqueue(receive, byte);
    return 0;
}

/**
 * @brief Retrieves the top byte from the receive queue.
 * 
 * @return The byte from the top of the receive queue.
 */
uint8_t (receiveTop)() {
    uint8_t byte = 0;
    if (!isEmpty(receive)) {
        byte = dequeue(receive);
    }
    return byte;
}

/**
 * @brief Adds a byte to the transmit queue if it is not full.
 * 
 * @param byte The byte to be added to the transmit queue.
 */
void (addToSend)(uint8_t byte) {
    if (!isFull(transmit)) {
        enqueue(transmit, byte);
    } else {
        printf("Transmit queue is full\n");
    }
}

/**
 * @brief Enables FIFOs for both receiver and transmitter and clears them.
 * 
 * @return 0 in case of success, 1 otherwise.
 */
int (enableFifos)() {
    uint8_t command;
    if (serialRead(FCR, &command) != 0) {
        return 1;
    }
    command |= ENABLE_BOTH_FIFOS;
    if (serialWrite(FCR, command) != 0) {
        printf("Error in serial write\n");
        return 1;
    }
    if (serialRead(FCR, &command) != 0) {
        return 1;
    }
    command |= CLEAN_BYTES_RCVR;
    if (serialWrite(FCR, command) != 0) {
        printf("Error in serial write\n");
        return 1;
    }
    if (serialRead(FCR, &command) != 0) {
        return 1;
    }
    command |= CLEAN_BYTES_XMIT;
    if (serialWrite(FCR, command) != 0) {
        printf("Error in serial write\n");
        return 1;
    }
    return 0;
}

/**@}*/


 
