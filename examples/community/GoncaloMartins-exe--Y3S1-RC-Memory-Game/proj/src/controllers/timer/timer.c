#include "timer.h"

uint32_t counter = 0; // Global interrupt counter
int hook_id = 0;      // Hook ID for interrupt subscrip
uint32_t game_seconds = 0;
bool timer_running = false;
/**
 * @brief Sets the frequency of the specified timer.
 * 
 * @param timer Timer number (0, 1, or 2).
 * @param freq Desired frequency in Hz.
 * 
 * @return 0 on success, 1 on error.
 * 
 * Validates input parameters, reads current timer configuration,
 * modifies it to set LSB followed by MSB access mode,
 * calculates the divider for the desired frequency,
 * and writes the new configuration and divider to the timer ports.
 */
int (my_timer_set_frequency)(uint8_t timer, uint32_t freq) {
  // Validate parameters
  if (timer > 2 || freq < 19 || freq > TIMER_FREQ) {
    printf("Erro: parâmetros inválidos (timer=%u, freq=%u)\n", timer, freq);
    return 1;
  }

  uint8_t current_conf;
  if (my_timer_get_conf(timer, &current_conf) != 0) {
    printf("Erro ao obter configuração atual do timer %u\n", timer);
    return 1;
  }

  // Keep lower 4 bits (mode) and add LSB followed by MSB access mode
  uint8_t new_ctrl = (current_conf & 0x0F) | TIMER_LSB_MSB;
   
  // Set timer selection bits
  switch (timer) {
    case 0: new_ctrl |= TIMER_SEL0; break;
    case 1: new_ctrl |= TIMER_SEL1; break;
    case 2: new_ctrl |= TIMER_SEL2; break;
    default: return 1;
  }
  
  // Calculate divider from desired frequency
  uint16_t divider = TIMER_FREQ / freq;
  uint8_t lsb = 0, msb = 0;
  
  // Extract LSB and MSB from divider
  if (util_get_LSB(divider, &lsb) != 0 || util_get_MSB(divider, &msb) != 0) {
    printf("Erro ao calcular LSB/MSB para freq %u\n", freq);
    return 1;
  }

  // Send LSB then MSB to the corresponding timer port
  if (sys_outb(TIMER_CTRL, new_ctrl) != 0) return 1;

  // Escrever LSB e MSB na porta correta
  uint8_t timer_port = TIMER_0 + timer;
  if (sys_outb(timer_port, lsb) != 0) return 1;
  if (sys_outb(timer_port, msb) != 0) return 1;

  return 0;
}


/*______________________________________________________________________________________*/
/**
 * @brief Subscribes to timer interrupts.
 * 
 * @param[out] bit_no Pointer to store the bit mask of the IRQ.
 * 
 * @return 0 on success, 1 on failure.
 * 
 * Uses sys_irqsetpolicy with IRQ_REENABLE to subscribe to TIMER0_IRQ.
 */
int (my_timer_subscribe_int)(uint8_t *bit_no) {
  if (!bit_no) return 1;    // Null pointer check
  *bit_no = BIT(hook_id);   // Return the bit mask for IRQ
  // Subscribe to timer interrupts
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
}

/*______________________________________________________________________________________*/

/**
 * @brief Unsubscribes from timer interrupts.
 * 
 * @return 0 on success, 1 on failure.
 */
int (my_timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

/*______________________________________________________________________________________*/
/**
 * @brief Timer interrupt handler.
 * 
 * Increments global interrupt counter and game seconds counter.
 */
void (timer_int_handler)() {
  counter++; // Increment global counter on each interrupt
  printf("Timer interrupt: counter = %u\n", counter); // Increment global counter on each interrupt
  game_seconds++;
}

/*______________________________________________________________________________________*/
/**
 * @brief Reads the configuration/status of the specified timer.
 * 
 * @param timer Timer number (0, 1, or 2).
 * @param[out] st Pointer to store the status byte.
 * 
 * @return 0 on success, 1 on error.
 * 
 * Sends a read-back command to the timer control register and reads the status byte.
 */
int (my_timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || timer > 2) {
    printf("Erro: parâmetros inválidos em timer_get_conf.\n");
    return 1;
  }

  // Prepare read-back command for the selected timer
  uint8_t read_back_cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  
  // Send read-back command
  if (sys_outb(TIMER_CTRL, read_back_cmd) != 0) {
    printf("Erro ao enviar comando de leitura para o timer %u\n", timer);
    return 1;
  }
  
  // Read status byte from timer
  if (util_sys_inb(TIMER_0 + timer, st) != 0) {
    printf("Erro ao ler configuração do timer %u\n", timer);
    return 1;
  }

  return 0;
}

/*______________________________________________________________________________________*/
/**
 * @brief Displays the configuration of the specified timer.
 * 
 * @param timer Timer number (0, 1, or 2).
 * @param st Status byte of the timer.
 * @param field Field of the status to display (tsf_all, tsf_initial, tsf_mode, tsf_base).
 * 
 * @return 0 on success, 1 on error.
 * 
 * Interprets the requested field of the timer status and calls timer_print_config to print it.
 */
int (my_timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val val; // Union to hold interpreted config

  switch (field) {
    case tsf_all:
      val.byte = st;    // Return entire status byte
      break;

    case tsf_initial: {  // Interpret initialization mode
      uint8_t init_mode = (st & (BIT(4) | BIT(5))) >> 4;

      switch (init_mode) {
        case 1: val.in_mode = LSB_only; break;
        case 2: val.in_mode = MSB_only; break;
        case 3: val.in_mode = MSB_after_LSB; break;
        default: val.in_mode = INVAL_val;
      }
      break;
    }

    case tsf_mode: {     // Interpret counting mode
      uint8_t mode = (st >> 1) & 0x07;

      // Adjust values for modes 6 and 7
      if (mode == 6) val.count_mode = 2;
      else if (mode == 7) val.count_mode = 3;
      else val.count_mode = mode;
      break;
    }

    case tsf_base:   // BCD/binary mode
      val.bcd = (st & TIMER_BCD) ? true : false;
      break;

    default:
      printf("Erro: campo desconhecido no display_conf.\n");
      return 1;
  }

  return timer_print_config(timer, field, val);  // Display configuration
}
