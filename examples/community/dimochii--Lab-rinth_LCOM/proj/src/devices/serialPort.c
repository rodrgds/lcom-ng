#include "serialPort.h"

/** @defgroup serialPort serialPort
 * @{
 *
 * Functions for serialPort
 */

int hook_id_sp = 6;
extern GameState gameState;
extern Sprite *player, *player2;
static Queue * receiveQueue, *sendQueue;

bool isPlayer1;
extern bool multi;
extern int num;
static bool valEmpty = true;

/**
 * @brief Subscribes to serial port interrupts.
 * 
 * @param irq_set Pointer to store the IRQ set.
 * @return 0 on success, 1 otherwise.
 */
int (sp_subscribe_int)(uint8_t* irq_set){
    *irq_set = BIT(hook_id_sp);
    if (sys_irqsetpolicy(IRQ_COM1 , IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_sp)) 
        return 1;
    return 0;
}

/**
 * @brief Unsubscribes from serial port interrupts.
 * 
 * @return 0 on success, 1 otherwise.
 */
int (sp_unsubscribe_int)(){
    if(sys_irqrmpolicy(&hook_id_sp)) return 1;
    return 0;
}

/**
 * @brief Enables serial port interrupts.
 * 
 * @return true on success, false otherwise.
 */
bool (sp_enable_int)(){
    uint8_t val;
    val = (IER_ERBFI | IER_ETBEI);
    return sys_outb(BASE_COM1 + IER, val);
}


/**
 * @brief Disables serial port interrupts.
 * 
 * @return true on success, false otherwise.
 */
bool (sp_disable_int)(){
    uint8_t val;
    util_sys_inb(IER + BASE_COM1, &val);
    val = val & ~(IER_ERBFI | IER_ETBEI| IER_ELSI | IER_EMSI);

    return sys_outb(BASE_COM1+ IER, val); 
}

/**
 * @brief Initializes the serial port.
 * 
 * @return 0 on success, 1 otherwise.
 */
int (sp_initialize)(){
    uint8_t val_ier;
    if (util_sys_inb(BASE_COM1+ IER, &val_ier)!= 0) 
        return 1;

    val_ier = (val_ier & 0xF0) | IER_ERBFI;
    if (sys_outb(BASE_COM1+ IER, val_ier)!= 0) 
        return 1;

    receiveQueue = newQueue(100);
    sendQueue = newQueue(100);
    
    /* Configuration */
    uint8_t val = (BIT(7));
    if (sys_outb(BASE_COM1+ LCR, val) != 0)
        return 1;
    if (sys_outb(BASE_COM1+ DLM, 0) != 0)
        return 1;
    if (sys_outb(BASE_COM1+ DLL, 1) != 0)
        return 1;
    
    val = sys_outb(BASE_COM1+ LCR, val);
    if (sys_outb(BASE_COM1+ LCR, val) != 0)
        return 1;

    return 0;
}


/**
 * @brief Receives a byte from the serial port.
 * 
 * @return 0 on success, 1 otherwise.
 */
int (receive_byte)(){
    uint8_t st;
    // Read status
    if (util_sys_inb(BASE_COM1 + LSR, &st)!= 0) 
        return 1;

    uint8_t val_rbr;
    if (st & LSR_DR){
      if (util_sys_inb(BASE_COM1 + RBR, &val_rbr)!= 0)
        return 1;
      if (!(st & (LSR_OE| LSR_PE| LSR_FE))){
          enqueue(receiveQueue,val_rbr);
          return 0;
      }
    }

    return 1;
}

/**
 * @brief Sends a byte to the serial port.
 * 
 * @param byte Byte to send.
 * @return 0 on success, 1 otherwise.
 */
int (send_byte)(uint8_t byte){
    int cond = enqueue(sendQueue,byte);
    if (valEmpty)
        return send_queue_bytes();
    else 
        return cond;
}

/**
 * @brief Sends bytes from the send queue.
 * 
 * @return 0 on success, 1 otherwise.
 */
int (send_queue_bytes)(){
    if (queueIsEmpty(sendQueue)){
        valEmpty = true;
        return 1;
    }

    uint8_t transmEmpty;
    while (!queueIsEmpty(sendQueue)){
        sys_outb(BASE_COM1 + THR, frontQueue(sendQueue));
        dequeue(sendQueue);

        util_sys_inb(BASE_COM1+LSR, &transmEmpty);
        transmEmpty = transmEmpty & LSR_THRE;
        valEmpty = transmEmpty;
        if (!transmEmpty) 
            return 1;
    }

    return 0;
}


/**
 * @brief Sends a scan code.
 * 
 * @param scancode Scan code to send.
 */
void (send_scan)(uint8_t scancode){
    uint8_t send_code = 0;
    if (scancode == A_KEY_MK) {
        send_code |= BIT(0);
    }
    else if (scancode == A_KEY_BRK) {
        send_code |= BIT(1);
    }
    else if (scancode == D_KEY_MK) {
        send_code |= BIT(2);
    }
    else if (scancode == D_KEY_BRK) {
        send_code |= BIT(3);
    }
    else if (scancode == W_KEY_MK) {
        send_code |= BIT(4);
    }
    else if (scancode == W_KEY_BRK) {
        send_code |= BIT(5);
    }
    else if (scancode == S_KEY_MK) {
        send_code |= BIT(6);
    }
    else if (scancode == S_KEY_BRK) {
        send_code |= BIT(7);
    }

    send_byte(send_code);
}


/**
 * @brief Manages the button press.
 * 
 * @param scancode Scan code of the button press.
 * @param isPlay1 Boolean indicating if it is player 1.
 */
void (manage_button)(uint8_t scancode, bool isPlay1) {
  if (isPlay1) handle_ingame_scancode(scancode, player);
  else handle_ingame_scancode_multi(scancode, player2); 
  send_scan(scancode);
}

/**
 * @brief Cleans the serial port interrupts.
 * 
 * @return 0 on success, 1 otherwise.
 */
int (sp_clean_int)(){
    if (sys_outb(BASE_COM1 + FCR, FCR_CLEAR)!= 0) 
        return 1;
    while (!queueIsEmpty(receiveQueue))
        dequeue(receiveQueue);

    return 0;
}

/**
 * @brief Interrupt handler for the serial port.
 */
void (sp_ih)(){
    uint8_t val;
    util_sys_inb(BASE_COM1 + IIR, &val);

    while (!(val & IIR_NIP)) {
        if(val & IIR_RDA){
            while (receive_byte() == 0);
            util_sys_inb(BASE_COM1 + IIR, &val);
        }
        if(val & IIR_THRE){
            send_queue_bytes();
            util_sys_inb(BASE_COM1 + IIR, &val);
        }
    }
}


/**
 * @brief Handles the start of multiplayer.
 */
void (sp_handle_start_multi)() {
    if(frontQueue(receiveQueue) == 0x53) {
        send_byte(0x54);
    } 
    else if (frontQueue(receiveQueue) == 0x54) {
        send_byte(0x55);
    } 
    else if(frontQueue(receiveQueue) == 0x55) {
        uint8_t srandByte = 0;
        while(srandByte == ACK || srandByte == NACK || srandByte == END || srandByte == 0){
            srandByte = rand();
        }
        send_byte(0x56);
        send_byte(srandByte);
        srandom(srandByte);
    } 
    else if(frontQueue(receiveQueue) == 0x56) {
        dequeue(receiveQueue);
        uint8_t srandByte = dequeue(receiveQueue);
        while(srandByte == 0){
            receive_byte();
            srandByte = dequeue(receiveQueue);
        }
        srandom(srandByte);
        send_byte(0x57);
    } 
    else if(frontQueue(receiveQueue) == 0x57) {
        gameState = GAME;
        multi = true;
    }

    gameState = GAME; 
    multi =true;

    while(!queueIsEmpty(receiveQueue))
        dequeue(receiveQueue);
}


/**
 * @brief Main handler for the serial port.
 */
void (sp_handler)(){
  sp_ih();
  num++;
  update_energy_sprite(num); 

  if (gameState == MULTI)
    sp_handle_start_multi();
  else if (gameState == GAME && multi) 
    sp_handle_receive_info();
  else
    sp_clean_int();
}

/**
 * @brief Handles receiving information from the serial port.
 */
void (sp_handle_receive_info)(){
    while (!queueIsEmpty(receiveQueue)) {
        uint8_t receivedByte = dequeue(receiveQueue);
        uint8_t scancode = 0x00;

        if(receivedByte & BIT(0)) 
            scancode = A_KEY_MK;
        else if(receivedByte & BIT(1)) 
            scancode = A_KEY_BRK;
        else if(receivedByte & BIT(2)) 
            scancode = D_KEY_MK;
        else if(receivedByte & BIT(3)) 
            scancode = D_KEY_BRK;
        else if(receivedByte & BIT(4)) 
            scancode = W_KEY_MK;
        else if(receivedByte & BIT(5))
            scancode = W_KEY_BRK;
        else if(receivedByte & BIT(6)) 
            scancode = S_KEY_MK;
        else if(receivedByte & BIT(7)) 
            scancode = S_KEY_BRK;
        else
            return;
   
        if (!isPlayer1) handle_ingame_scancode(scancode, player);
        else handle_ingame_scancode_multi(scancode, player2);   
    }
}

