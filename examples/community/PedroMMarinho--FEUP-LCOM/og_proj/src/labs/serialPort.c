#include "serialPort.h"

static int sp_hookId = 7;

static Queue* sendQueue;
static Queue* receiveQueue;

static int transmitter_empty = 1;
bool sendTrySync = false;

/**
 * @brief Reads a byte from a serial port.
 * 
 * This function reads a byte from the specified serial port and stores it in the provided value pointer.
 * 
 * @param port The port to read from.
 * @param value Pointer to a variable to store the read value.
 * @return int 0 on success, 1 on failure.
 */
static int sp_read(int port, uint8_t* value){
    if(util_sys_inb(port + COM1, value)) return 1;
    return 0;
}

/**
 * @brief Writes a byte to a serial port.
 * 
 * This function writes the specified byte to the given serial port.
 * 
 * @param port The port to write to.
 * @param value The byte value to write.
 * @return int 0 on success, 1 on failure.
 */
static int sp_write(int port, uint8_t value){
    if(sys_outb(COM1 + port,value)) return 1;
    return 0;
}

/**
 * @brief Sets the bitrate of the serial port.
 * 
 * This function sets the bitrate of the serial port by configuring the divisor latch registers.
 * 
 * @param bitrate The desired bitrate.
 * @return int 0 on success, 1 on failure.
 */
static int sp_bitrate(int bitrate) {
    uint16_t rate = 115200 / bitrate;

    uint8_t ratemsb = 0, ratelsb = 0;
    util_get_LSB(rate, &ratelsb);
    util_get_MSB(rate, &ratemsb);

    //SET DLAB
    uint8_t lcr;
    
    if(sp_read(LCR,&lcr)) return 1;

    lcr = lcr | LCR_DLAB;

    if(sp_write(LCR,lcr)) return 1;

    //SET BITRATE
    if(sp_write(DIVISOR_LATCH_LSB,ratelsb))return 1;

    if(sp_write(DIVISOR_LATCH_MSB,ratemsb))return 1;

    //RESET DLAB
    if(sp_read(LCR,&lcr)) return 1;

    lcr = lcr & ~LCR_DLAB;

    if(sp_write(LCR,lcr)) return 1;

  return 0;
}
/**
 * @brief Initializes the serial port.
 * 
 * This function initializes the serial port by enabling interrupts, setting the FIFO, and configuring the bitrate.
 * 
 * @return int 0 on success, 1 on failure.
 */

static int sp_init() {

    //ENABLE INTERRUPTIONS
    uint8_t ier;
    if(sp_read(IER,&ier)) return 1;
    ier = ier | (IER_RDAI | IER_THREI | IER_RLSI);
    if(sp_write(IER, ier)) return 1;


    //ENABLE FIFO AND CLEAR TRANSMITTER AND RECEIVER FIFO
    uint8_t fcr;
    if(sp_read(FCR,&fcr)) return 1;
    fcr = fcr | (FCR_ENABLE_FIFO | FCR_CLEAR_RCVR_FIFO | FCR_CLEAR_XMIT_FIFO);
    if(sp_write(FCR, fcr)) return 1;

    sendQueue = new_queue();
    receiveQueue = new_queue();

    //SET BITRATE
    if (sp_bitrate(115200)) return 1; 

    //SET PARITY AND BITS PER CHAR
    uint8_t lcr;

    if(sp_read(LCR,&lcr)) return 1;
    
    lcr = (lcr & 0xC4) | (LCR_8_BITS) | (LCR_ODD_PARITY);

    if(sp_write(LCR,lcr)) return 1;


  return 0;
}

/**
 * @brief Ends the serial port operation.
 * 
 * This function disables the serial port interrupts and clears the FIFO, also freeing the queues.
 * 
 * @return int 0 on success, 1 on failure.
 */

static int sp_end() {

    //DISABLE INTERRUPTIONS
    uint8_t ier;
    if(sp_read(IER,&ier)) return 1;
    ier = ier & (~IER_RDAI | ~IER_THREI | ~IER_RLSI);
    if(sp_write(IER, ier)) return 1;

    //DISABLE FIFO AND CLEAR TRANSMITTER AND RECEIVER FIFO
    uint8_t fcr;
    if(sp_read(FCR,&fcr)) return 1;
    fcr = fcr & (~FCR_ENABLE_FIFO | ~FCR_CLEAR_RCVR_FIFO | ~FCR_CLEAR_XMIT_FIFO);
    if(sp_write(FCR, fcr)) return 1;

    free(sendQueue->values);
    free(receiveQueue->values);
    free(sendQueue);
    free(receiveQueue);

  return 0;
}


int (sp_subscribe)(uint8_t *bit_no) {

    if (sp_init()) return 1;

    if (bit_no == NULL) return 1;    

    *bit_no = BIT(sp_hookId);

    if (sys_irqsetpolicy(COM1_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &sp_hookId) != 0){
        printf("Error: could not subscribe SP interruption\n");
        return 1;
    } 
    sp_ih();

    return 0;
}


int (sp_unsubscribe)() {
    if (sp_end()) return 1;

    if (sys_irqrmpolicy(&sp_hookId) != 0) {return 3;}

    return 0;
}


int sp_clear(){
    uint8_t fcr;
    if(sp_read(FCR, &fcr)) return 1;
    fcr |= (FCR_CLEAR_RCVR_FIFO | FCR_CLEAR_XMIT_FIFO | FCR_ENABLE_FIFO);
    if(sp_write(FCR, fcr)) return 1;
    while(queue_pop(receiveQueue) != 0);
    return 0;
}

/**
 * @brief Sends data stored in the queue.
 * 
 * This function sends data stored in the send queue to the serial port.
 * 
 * @return int 0 on success, 1 on failure.
 */
int send_queue_data(){ //send stored info in queue
        
    if(queue_isEmpty(sendQueue)){
        printf("queue is empty\n");
        transmitter_empty = 1;
        return 1;
    }

    uint8_t lsr;

    while(!queue_isEmpty(sendQueue)){
        sp_write(THR,queue_front(sendQueue));
        queue_pop(sendQueue);
        sp_read(LSR, &lsr);
        transmitter_empty = lsr & LSR_TRANSMITTER_HOLD_EMPTY;
        if(!transmitter_empty) return 1;
    }
    return 0;
}

int send_SP_data(uint8_t data){ 
    printf("send_byte %x\n",data);
    queue_push(sendQueue,data);

    if(transmitter_empty){
        return send_queue_data();
    }
    else return 1;
}


int establish_connection(){ 
    bool connection = false;
    uint8_t frontByte = queue_front(receiveQueue);
    if(frontByte == TRYSYNC){
        send_SP_data(FIRSTSYNC);
    }else if(frontByte == FIRSTSYNC){
        connection = true;
        send_SP_data(SECONDSYNC); 

    }else if(frontByte == SECONDSYNC){ //1 more?? dizer que tbm entrou
        connection = true;
    }else if(!sendTrySync){
        send_SP_data(TRYSYNC);
        sendTrySync = true;
    }
    
    queue_pop(receiveQueue);
    return connection;
}

/**
 * @brief Receives data from the serial port.
 * 
 * This function receives data from the serial port and pushes it to the receive queue.
 * 
 * @return int 0 on success, 1 on failure.
 */

int receive_SP_data(){ 
    uint8_t lsr, rbr;
    sp_read(LSR, &lsr);
    
    if(lsr & LSR_RECEIVER_DATA){
        sp_read(RBR, &rbr);

        int lsrError = lsr & (LSR_OVERRUN_ERROR | LSR_PARITY_ERROR | LSR_FRAMING_ERROR);
        
        if(!(lsrError)){
            queue_push(receiveQueue, rbr);
            printf("read_byte %x\n",rbr);
            return 0;
        }
    }
    return 1;
}


void sp_ih(){ 
    uint8_t interrupt;
    sp_read(IIR, &interrupt);
    while(!(interrupt & IIR_PENDING)) { //Interrupt Pending
        if(interrupt & IIR_DATA_AVAILABLE){
            printf("data available\n");
            while(!receive_SP_data());
            sp_read(IIR, &interrupt);
        }
        if(interrupt & IIR_TRANSMIT_HOLD_EMPTY){
            printf("transmit hold empty\n");
            send_queue_data();
            sp_read(IIR, &interrupt);
        }
    }
}

void resetMultiplayer(){
    sendTrySync = false;
    transmitter_empty = 1;
    while(queue_pop(sendQueue) != 0);
    while(queue_pop(receiveQueue) != 0);
}
