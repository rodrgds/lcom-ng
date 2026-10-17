#include "serial_port.h"
#include "queue.h"
#include "game.h"

int serial_port_hook_id = UART_COM1_IRQ;

Queue* serial_port_send_queue;
Queue* serial_port_receive_queue;

bool receivingMouse = false;
bool mouseComplete = false;
bool receivingKbd = false;
bool kbdComplete= false;
bool twoBytes = false;
uint8_t kbdBytes[2];

extern bool WPressed;
extern bool SPressed;
extern bool APressed;
extern bool DPressed;
extern bool LBPressed;
extern bool esc_pressed;

extern int16_t delta_x;
extern int16_t delta_y;

bool can_send = true;

extern unsigned int current_player;



int serial_port_subscribe_int(int *bit_no)
{
        if(bit_no == NULL)
                return 1;
        
        if(sys_irqsetpolicy(UART_COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &serial_port_hook_id))
                return 1;

        uint8_t enable_fifo = FCR_ENABLE_FIFO | FCR_CLEAR_REC | FCR_CLEAR_TRANS;
        if(sys_outb(UART_COM1+UART_FCR, enable_fifo))
                return 1;

        uint8_t aux;
        if(util_sys_inb(UART_COM1 + UART_IER, &aux))
                return 1;
        aux &= 0xF0;

        sys_outb(UART_COM1 + UART_IER, aux);

        util_sys_inb(UART_COM1 + UART_IIR, &aux);

        if (!(aux & 0xC0))
                return 1;

        sys_irqdisable(&serial_port_hook_id);

        if(serial_port_enable())
                return 1;

        return 0;
}


int serial_port_enable()
{
        uint32_t rate = 1;
        uint8_t aux;

        if(util_sys_inb(UART_COM1+UART_LCR, &aux))
                return 1;

        aux &= LCR_BREAK_CONTROL;
        aux |= (LCR_NUM_STOP_BITS| LCR_DLAB) ;

        if(sys_outb(UART_COM1+UART_LCR, aux))
                return 1;

        if (sys_outb(UART_COM1, (rate & 0x00ff)) != 0) 
                return 1;
        if (sys_outb(UART_COM1 + UART_IER, ((rate >> 16) & 0x00ff))!=0)
                return 1;

        aux &= ~LCR_DLAB;
        if(sys_outb(UART_COM1+UART_LCR, aux))
                return 1;

        clear_rec();
        clear_trans();
        
        serial_port_send_queue = createQueue();
	serial_port_receive_queue = createQueue();

        return 0;
}

void clear_rec()
{
        uint8_t aux;
        util_sys_inb(UART_COM1 + UART_FCR, &aux);
        aux |= FCR_CLEAR_REC;
        sys_outb(UART_COM1 + UART_FCR, aux);
}

void clear_trans()
{
        uint8_t aux;
        util_sys_inb(UART_COM1 + UART_FCR, &aux);
        aux |= FCR_CLEAR_TRANS;
        sys_outb(UART_COM1 + UART_FCR, aux);     
}

int serial_port_unsubscribe_int()
{
        
        clear_rec();
        clear_trans();

        free(serial_port_send_queue);
        free(serial_port_receive_queue);

        if(sys_irqrmpolicy(&serial_port_hook_id))
                return 1;
        return 0;
}

void serial_port_ih()
{
        uint8_t aux;
        util_sys_inb(UART_COM1+UART_IIR, &aux);
        if(!(aux & UART_IIR_INT_PEND))
        {
                switch(aux & UART_IIR_INT_TYPE)
                {
                case UART_IIR_RECEIVE_AVAIL:
                        serial_port_rec();
                        break;
                case UART_IIR_TRANS_HOLD:
                        can_send = true;
                        break;
                default:
                        break;
                }
        }
}

int serial_port_send()
{
        uint8_t aux;
        uint8_t data;
        while(!queue_empty(serial_port_send_queue))
        {
               
                if(util_sys_inb(UART_COM1 + UART_LSR, &aux))
                        return 1;

                if(aux & LSR_TRANS_HOLD_EMPTY)
                {
                        data = deQueue(serial_port_send_queue);
                        if(sys_outb(UART_COM1 + UART_THR, data))
                        {
                                enQueue(serial_port_send_queue, data);
                                return 1;
                        }
                }
        }
        return 0;
}

int serial_port_rec()
{
        
        uint8_t data;
        uint8_t aux;

        if(util_sys_inb(UART_COM1 + UART_LSR, &aux))
                return 1;
        while(aux & LSR_RECEIVER_DATA)
        { 
                
                //updateMousePosition(40,-40);

                if(aux & (LSR_OVERRUN_ERROR |LSR_PARITY_ERROR|LSR_FRAMING_ERROR))
                {
                        clear_rec();
                        clear_trans();
                        return 1;
                }

                if(util_sys_inb(UART_COM1 + UART_RBR, &data))
                        return 1;
                        
                serial_port_interp(data);
        
                util_sys_inb(UART_COM1 + UART_LSR, &aux);
        }
        return 0;
}

void serial_port_interp(uint8_t data)
{
        static int mouseCount = 0;
        static bool sign_x = false;
        static bool sign_y = false;
        static bool make = false;
        static struct packet pp;


        if(current_player == 1)
        {
                if(data == 'M' && !receivingMouse)
                {
                        receivingMouse = true;
                }
                else if(receivingMouse)
                {
                        if(mouseCount == 0)
                        {
                                if ((data & BIT(3)) != 0)
                                {
                                        pp.bytes[0] = data;
                                        if (data & MOUSE_X_DELTA_SIGN)
                                                sign_x = true;
                                        else
                                                sign_x = false;

                                        if (data & MOUSE_Y_DELTA_SIGN)
                                                sign_y = true;
                                        else
                                                sign_y = false;
                                        mouseCount++;
                                }
                        }
                        else if (mouseCount == 1) {
                                pp.bytes[1] = data;
                                mouseCount++;
                                        
                        }
                        else if (mouseCount == 2) {
                                pp.bytes[2] = data;
                                pp.lb = pp.bytes[0] & MOUSE_LB_PRESSED;
                                pp.rb = pp.bytes[0] & MOUSE_RB_PRESSED;
                                pp.mb = pp.bytes[0] & MOUSE_MB_PRESSED;
                                pp.delta_x = ninebit16bit(sign_x, pp.bytes[1]);
                                pp.delta_y = ninebit16bit(sign_y, pp.bytes[2]);
                                pp.x_ov = pp.bytes[0] & MOUSE_X_OVF;
                                pp.y_ov = pp.bytes[0] & MOUSE_Y_OVF;
                                
                                updateMousePosition( pp.delta_x, pp.delta_y);
                                if(pp.lb)
                                        LBPressed = true;
                                else 
                                        LBPressed = false;
                                
                                receivingMouse = false;
                                mouseComplete = true;
                                mouseCount = 0;

                        }
                }               
                
        }
        else if(current_player == 2)
        {
                if(data == 'K' && !receivingKbd)
                {
                        receivingKbd = true;
                }
                else if(twoBytes && receivingKbd)
                {
                        if (data & BIT(7))
                                make = false;
                        else
                                make = true;
                        kbdBytes[0] = 0xE0;
                        kbdBytes[1] = data;

                        twoBytes = false;
                }
                else if(!twoBytes && receivingKbd)
                {
                        if (data == 0xE0) {
                                twoBytes = true;
                        } 
                        else
                        {
                                twoBytes = false;

                                if (data & BIT(7))
                                        make = false;
                                else
                                        make = true;
                                kbdBytes[0] = data;
                                
                                if (data == 0x11)
                                        WPressed = true;
                                else if (data == 0x91)
                                        WPressed = false;
                                else if (data == 0x1F)
                                        SPressed = true;
                                else if (data == 0x9F)
                                        SPressed = false;
                                else if(data == 0x1E)
                                        APressed = true;
                                else if(data == 0x9E)
                                        APressed = false;
                                else if(data == 0x20)
                                        DPressed = true;
                                else if(data == 0xA0)
                                        DPressed = false;
                                else if(data == 0x01)
                                        esc_pressed = true;
                                
                                receivingKbd = false;
                        }
                }             
        }   
}

void serial_port_write(uint8_t data)
{
        enQueue(serial_port_send_queue, data);
}


