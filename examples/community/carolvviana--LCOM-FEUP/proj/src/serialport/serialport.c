#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#include "serialport.h"
#include <lcom/utils.h>

uint8_t data = 0x00;
uint8_t data_x_ball = 0x00;
uint8_t data_y_ball = 0x00;
int hook_id_serialp = 4;

int(init_serialp)() {
  int int_enable = write_address_with_offset(SERIALP_IER_OFFSET, SERIALP_RECEIVER);
  if (int_enable != 0) {
    return 1;
  } // enables the "received data available" interrupt -> ou seja, há interrupção quando há dados para ler

  bool no_data = true;
  do {
    no_data = read_data_serialp() == 1; // se não houver dados para ler, então está tudo bem
  } while (!no_data);

  if (send_data_serialp(SERIALP_STARTGAME) != 0)
    return 1; // avisar que estamos prontos para conectar

  return 0;
}

void(serialp_ih)() {
    uint8_t interrupt_status = 0;

    interrupt_status = read_data_serialp();
    if (interrupt_status != 0){
        printf("Error in serialp_ih()");
    }
}

int(read_data_serialp)() {
  uint8_t status;

  if (read_status_serialp(&status) != 0) {
    printf("Error in read_data_serialp() - Could not read status");
    return 1;
  };                                 // lemos o status a ver se está tudo okay

  if (status & SERIALP_RECEIVING_DATA) {      // BIT(0) do status estar a 1 significa que h+á dados para ler -> "Set to 1 when there is data for receiving"
    printf("read_data_serialp() - Status: %d\n", status);
    while (status & SERIALP_RECEIVING_DATA) { // enquanto houver dados para ler
      // tickdelay(2000);
      int aux = util_sys_inb(SERIALP_ADDRESS, &data);
      // tickdelay(2000);
      if (aux != 0)
        return 1;
      // if (util_sys_inb(SERIALP_ADDRESS, &data) != 0) {
      //     printf("Error in read_data_serialp() - Could not read data\n");
      //     return 1;    //lemos os dados
      // }

      return serialp_check_error(&status); // se houver erro, retorna 1

      printf("read_data_serialp() - Read data: %d\n", data);
    }
  }
  else { // não há dados
    data = 0x00;
    printf("Error in read_data_serialp() - read data not ready\n");
    return 1;
  }

  return 0;
}

int serialp_get_status(uint8_t *status) {
  return util_sys_inb(SERIALP_ADDRESS + SERIALP_LSR_OFFSET, status);
  // gets status from the lsr (line status register)
}

int serialp_check_error(uint8_t *status) {
  if (*status & SERIALP_OVERRUN_ERR) {
    printf("Error in SerialPort - Overrun error");
    return 1;
  }
  if (*status & SERIALP_FRAME_ERR) {
    printf("Error in SerialPort - Frame error");
    return 1;
  }
  if (*status & SERIALP_PARITY_ERR) {
    printf("Error in SerialPort - Parity error");
    return 1;
  }
  return 0;
}

int(read_status_serialp)(uint8_t *status) {
  if (serialp_get_status(status) != 0) {
    printf("Error in read_status_serialp() - Could not read LSR");
    return 1;
  }
  return serialp_check_error(status);
}

int(send_data_serialp)(uint8_t data) {
  uint8_t status;
  uint8_t attempts = 3;
  while (attempts) {

    if (read_status_serialp(&status) != 0)
      return 1; // lê status

    if ((status & (SERIALP_TRANSMITTERS)) != 0) {
      // tickdelay(100);
      return sys_outb(SERIALP_ADDRESS, data);
      /* porquê o bit(5) ou bit(6) ?
      BIT(5)-> "When set, means that the UART is ready to accept a new character for transmitting"
      BIT(6)-> "When set, means that both the THR and the Transmitter Shift Register are both empty"
      */
    }

    attempts--;
  }
  return 1;
}

int(serialp_connection)(int *ready) {
  if (data == SERIALP_STARTGAME) {   // se ele estiver pronto para começar a jogar
    send_data_serialp(SERIALP_PLAY); // envia mensagem a dizer que vamos começar
    *ready = SERIALP_STARTGAME;
    return 0;
  }
  else if (data == SERIALP_PLAY) {
    *ready = SERIALP_PLAY;
    return 0; // se não já estamos a jogar
  }
  return 1;
}

int(serialp_subscribe_int)(uint8_t *bit_no) {
  *bit_no = BIT(hook_id_serialp);

  if (sys_irqsetpolicy(SERIALP_IRQLINE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_serialp) != 0)
    return 1;
  return 0;
}

int(serialp_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_serialp) != 0)
    return 1;
  return 0;
}

int (write_address_with_offset)(int offset, uint8_t data) {
  if (sys_outb(SERIALP_ADDRESS + offset, data) != 0) {
    printf("Error in write_address_with_offset() - Could not write to address %d\n");
    return 1;
  }
  return 0;
}
