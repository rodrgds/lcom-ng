#include <lcom/lcf.h>
#include "ser.h"
#include "uart.h"
#include "../queue.h"

static int hook_ser = 3;
static int ih_err = 0;
static bool ready_to_send = true;
static struct Queue *queue_send = NULL, *queue_receive = NULL;

int ser_init() {
    if (ser_conf(8, 1, 1)) {
        printf("Error setting up serial port\n");
        return 1;
    }

    if (bit_rate_conf(9600)) {
        printf("Error setting up serial port's bit rate\n");
        return 1;
    }
    
    if (ser_enable_interrupts()) {
        printf("Error enabling serial port's interrupts\n");
        return 1;
    }

    if (init_fifos()) {
        printf("Error enabling serial port's fifos\n");
        return 1;
    }

    init_queues();

    return 0;
}

void init_queues() {
    queue_send = create_queue();
    queue_receive = create_queue();
}

void clear_queues() {
    clear_queue(queue_send);
    clear_queue(queue_receive);
}

void destroy_queues() {
    destroy_queue(queue_send);
    destroy_queue(queue_receive);
}

uint8_t receive_queue_pop() {
    uint8_t byte = front(queue_receive);
    pop_queue(queue_receive);
    return byte;
}

void send_queue_push(uint8_t byte) {
    push_queue(queue_send, byte);
} 

bool receive_queue_is_empty() {
    return is_empty(queue_receive);
}

bool send_queue_is_empty() {
    return is_empty(queue_send);
}

uint8_t receive_queue_front() {
    return front(queue_receive);
}

uint8_t receive_queue_size() {
    return get_size(queue_receive);
}

bool transmitter_ready() {
    return ready_to_send;
}

int check_ih_err() {
    int temp = ih_err;
    ih_err = 0;
    return temp;
}

int ser_subscribe_int(uint8_t* bit_no) {
    *bit_no = BIT(hook_ser);

    if (sys_irqsetpolicy(SER_COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_ser)) {
        printf("Error while subscribing to interrupts from serial port\n");
        return 1;
    }

    return 0;
}

int ser_unsubscribe_int() {
    if (sys_irqrmpolicy(&hook_ser)) {
        printf("Error while unsubscribing to interrupts from serial port\n");
        return 1;
    }

    return 0;
}

int ser_enable_interrupts() {
    uint8_t interrupts = SER_DATA_RECEIVE_INT | SER_RECEIVER_ERR_INT | SER_EMPTY_TRANSMIT_INT;

    if (sys_outb(SER_IER, interrupts)) {
        printf("Error while enabling serial port's interrupts\n");
        return 1;
    }

    return 0;
}

int ser_conf(uint8_t bits, uint8_t stop, uint8_t par_mode) {
    uint8_t conf = BIT(3);

    if (bits < 5 || bits > 8) {
        printf("Invalid number of bits (5-8)\n");
        return 1;
    }

    if (stop < 1 || stop > 2) {
        printf("Invalid number of stop bits (1-2)\n");
        return 1;
    }

    if (par_mode > 4 || par_mode < 1) {
        printf("Invalid parity mode (1-4)\n");
        return 1;
    }

    conf |= bits - 5;

    conf |= ((stop - 1) << 2);

    if (par_mode % 2 == 0)
        conf |= BIT(4);
    if (par_mode > 2)
        conf |= BIT(5);

    if (write_lcr(conf))
        return 1;

    return 0;
}

int bit_rate_conf(uint32_t bit_rate) {
    uint8_t ser_conf, lsb, msb;
    uint16_t val = 115200/bit_rate;

    if (read_lcr(&ser_conf))
        return 1;
    
    ser_conf |= LCR_DLAB;

    if (write_lcr(ser_conf))
        return 1;
    
    if (util_get_LSB(val, &lsb))
        return 1;
    
    if (util_get_MSB(val, &msb))
        return 1;
    
    if (sys_outb(SER_DLL, lsb)){
        printf("Error while writing to DLL\n");
        return 1;
    }
    
    if (sys_outb(SER_DLM, msb)){
        printf("Error while writing to DLM\n");
        return 1;
    }

    ser_conf &= ~LCR_DLAB;

    if (write_lcr(ser_conf))
        return 1;
    
    return 0;
}

int init_fifos() {
    uint8_t config = FCR_ENABLE_FIFOS | FCR_CLEAR_FIFOS;

    if (sys_outb(SER_FCR, config)) {
        printf("Error while writing FIFOS configuration in fifo control register\n");
        return 1;
    }

    return 0;
}

int read_lcr(uint8_t *conf) {
    if (util_sys_inb(SER_LCR, conf)) {
        printf("Error while reading from LCR\n");
        return 1;
    }

    return 0;
}

int write_lcr(uint8_t conf) {
    if (sys_outb(SER_LCR, conf)) {
        printf("Error while writing to LCR\n");
        return 1;
    }

    return 0;
}


int iir_status_read(uint8_t *irr_status) {
    if (util_sys_inb(SER_IIR, irr_status)) {
        printf("Error while reading interrupt identification register\n");
        return 1;
    }

    return 0;
}

int read_lsr(uint8_t *status) {
    if (util_sys_inb(SER_LSR, status)) {
        printf("Error while reading from line status register\n");
        return 1;
    }

    return 0;
}

int get_data_errors(int errors[]) {
    uint8_t status;
    uint8_t mask = BIT(1);

    if (read_lsr(&status))
        return 1;
    
    for (int i = 0; i < 4; mask <<= 1, i++) {
        if (status & mask)
            errors[i] = 1;
    }

    return 0;
}

int handle_errors(int errors[]) {
    // TODO: Actually handle the errors when the simple communication is done.

    for (int i = 0; i < 4; i++) {
        printf("Error %d\n", i);
    }

    return 0;
}

int read_byte(uint8_t *data) {
    uint8_t conf;

    if (read_lcr(&conf))
        return 1;
    
    if (conf & LCR_DLAB) {
        conf = conf & ~LCR_DLAB;
        if (write_lcr(conf))
            return 1;
    }

    if (util_sys_inb(SER_RHR, data)) {
        printf("Error getting data from uart fifo\n");
        return 1;
    }

    printf("Read data: %x\n", *data);

    micro_delay(500);

    return 0;
}

int send_byte(uint8_t data) {
    uint8_t conf;

    if (read_lcr(&conf))
        return 1;
    
    if (conf & LCR_DLAB)
        conf = conf & ~LCR_DLAB;

    if (write_lcr(conf))
        return 1;

    if (sys_outb(SER_THR, data)) {
        printf("Error writing to uart fifo\n");
        return 1;
    }

    printf("Sent %x\n", data);

    micro_delay(500);

    return 0;
}

int send_bytes() {
    for (int count = 0; count < SEND_FIFO_SIZE && !is_empty(queue_send); count++) {
        uint8_t data = front(queue_send);

        if (send_byte(data)) {
            printf("Error while sending byte throught uart fifo\n");
            return 1;
        }

        pop_queue(queue_send);
    }

    ready_to_send = false;
    return 0;
}

void ser_ih() {
    uint8_t iir_status, int_orig = 0;

    if (iir_status_read(&iir_status)) {
        ih_err = 1;
    }

    int_orig = (iir_status & INT_ORIGIN_MASK) >> 1;

    if (iir_status & IIR_INT_STATUS)
        return;

    switch(int_orig) {
        case IIR_DATA_ERR: {
            int errors[4] = {0, 0, 0, 0};
            if (get_data_errors(errors)) {
                ih_err = 1;
                break;
            }
            handle_errors(errors);
            break;
        }
        case IIR_DATA_RECEIVED:
        case IIR_CHAR_TIMEOUT: {
            uint8_t lsr, data;

            do {
                if (read_lsr(&lsr)){
                    ih_err = 1;
                    return;
                }
                // TODO: Need to check if there is an error or wait for ACK even?
                if (read_byte(&data)) {
                    ih_err = 1;
                    return;
                }

                if (data != 0) push_queue(queue_receive, data);
            } while(lsr & LSR_RECEIVED_DATA);

            break;
        }
        case IIR_TRANSMIT_EMPTY: {
            if (is_empty(queue_send)) {
                ready_to_send = true;
                return;
            }

            if (send_bytes()) 
                ih_err = 1;

            break;
        }
        default:
            printf("Unknown interrupt code\n");
            ih_err = 1;
            break;
    }
}

int clear_fifos() {
    uint8_t config = FCR_CLEAR_FIFOS;

    if (sys_outb(SER_FCR, config)) {
        printf("Error while writing FIFOS configuration in fifo control register\n");
        return 1;
    }

    return 0;
}

void receive_queue_push(uint8_t data) {
    push_queue(queue_receive, data);
}

int check_transmitter() {
    uint8_t lsr;

    if (read_lsr(&lsr)) {
        printf("Error while reading lsr\n");
        return 1;
    }

    if (lsr & LSR_TRANSMITTER_EMPTY)
        ready_to_send = true;

    return 0;
}
