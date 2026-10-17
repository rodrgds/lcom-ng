// -------------------------------------------------------------------------------------
// UART ports
#define COM1                0x3F8
#define COM2                0x2F8

#define COM1_IRQ            4
#define COM2_IRQ            3

// -------------------------------------------------------------------------------------
// UART Acessible Registers
#define SER_RBR                 0 // Receiver Buffer Register
#define SER_THR                 0 // Transmitter Holding Register
#define SER_DLL                 0 // Divisor Latch LSB
#define SER_DLM                 1 // Divisor Latch MSB
#define SER_IER                 1 // Interrupt Enable Register
#define SER_IIR                 2 // Interrupt Identification Register
#define SER_FCR                 2 // FIFO Control Register
#define SER_LCR                 3 // Line Control Register
#define SER_MCR                 4 // Modem Control Register
#define SER_LSR                 5 // Line Status Register
#define SER_MSR                 6 // Modem Status Register
#define SER_SR                  7 // Scratch Register

// -------------------------------------------------------------------------------------
// UART Line Control Register
#define BITS_PER_CHAR_MASK  (BIT(1) | BIT(0))
#define BITS_PER_CHAR(n)    (n - 5)                     // 0 -> 5 bits, 
                                                        // 1 -> 6 bits, 
                                                        // 2 -> 7 bits, 
                                                        // 3 -> 8 bits
#define STOP_BITS_MASK      BIT(2) 
#define STOP_BITS_2         BIT(2)                      // 0 -> 1 stop bit, 
                                                        // 1 -> 2 stop bits
#define PARITY_MASK         (BIT(5) | BIT(4) | BIT(3))
#define NO_PARITY           0
#define ODD_PARITY          BIT(3)
#define EVEN_PARITY         (BIT(4) | BIT(3))
#define PARITY_BIT_1        (BIT(5) | BIT(3))
#define PARITY_BIT_0        (BIT(5) | BIT(4) | BIT(3))
#define BREAK_CONTROL_MASK  BIT(6)
#define BREAK_CONTROL       BIT(6)                      // Sets the serial output to lo
#define DLAB_MASK           BIT(7)
#define DLAB                BIT(7)                      // 1 -> Divisor Latch Access  0 -> RBR, THR

#define BITRATE_115200      0x1
#define BITRATE_57600       0x2

// -------------------------------------------------------------------------------------
// UART Line Status Register
#define RECEIVER_DATA       BIT(0)                      // 1 -> There is data to be read
#define OVERRUN_ERR         BIT(1)                      // 1 -> Overrun error
#define PARITY_ERR          BIT(2)                      // 1 -> Parity error
#define FRAME_ERR           BIT(3)                      // 1 -> Frame error
#define BREAK_INTERRUPT     BIT(4)                      // 1 -> THR is empty
#define TRANSMITTER_EMPTY   BIT(5)                      // 1 -> Transmitter is empty
#define FIFO_ERROR          BIT(7)                      // 1 -> FIFO error

// -------------------------------------------------------------------------------------
// UART Interrupt Enable Register

#define ENABLE_RECEIVED_DATA_INT        BIT(0)          // 1 -> Enables received data interrupt
#define ENABLE_TRANSMITTER_EMPTY_INT    BIT(1)          // 1 -> Enables transmitter empty interrupt
#define ENABLE_RECEIVER_LINE_INT        BIT(2)          // 1 -> Enables receiver line status interrupt
#define ENABLE_MODEM_STATUS_INT         BIT(3)          // 1 -> Enables modem status interrupt

// -------------------------------------------------------------------------------------
// UART Interrupt Identification Register

#define INTERRUPT_PENDING       BIT(0)                      // 1 -> Interrupt pending
#define INTERRUPT_ORIGIN_MASK   (BIT(1) | BIT(2) | BIT(3))  // 0 -> Modem status interrupt
                                                            // 1 -> Transmitter empty interrupt
                                                            // 2 -> Received data available interrupt
                                                            // 3 -> Receiver line status interrupt
#define LINE_STATUS_INT         (BIT(2) | BIT(1))
#define RECEIVED_DATA_INT       BIT(2)
#define CHAR_TIMEOUT            (BIT(3) | BIT(2))
#define TRANSMITTER_EMPTY_INT   BIT(1)
#define MODEM_STATUS_INT        0
                                                            // the receiver FIFO 
#define FIFO_64BYTE             BIT(5)                      // 1 -> 64 byte FIFO enabled
// Bits 6 and 7 are set to 1 if FCR is set to 0

// -------------------------------------------------------------------------------------
// UART FIFO Control Register

#define ENABLE_FIFO             BIT(0)                      // 1 -> Enables both FIFO
#define CLEAR_RCVR_FIFO         BIT(1)                      // 1 -> Clears receiver FIFO
#define CLEAR_XMIT_FIFO         BIT(2)                      // 1 -> Clears transmitter FIFO
#define DMA_MODE_SELECT         BIT(3)                      // 1 -> DMA mode
#define RCVR_TRIGGER_1          0            
#define RCVR_TRIGGER_4          BIT(6)
#define RCVR_TRIGGER_8          BIT(7)
#define RCVR_TRIGGER_14         (BIT(6) | BIT(7))

// -------------------------------------------------------------------------------------
// UART Messages

#define START_GAME              0x20
#define END_GAME                0x21
#define UART_ACK                0x22
#define UART_NACK               0x23
#define END_OF_PACKET           0x24
#define PLAY_AGAIN              0x25

