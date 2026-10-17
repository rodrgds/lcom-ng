#include <lcom/lcf.h>
#include <mouse.h>

// All the following functions were created during the labs
// except the parse, which was adapted to the goal of the project

// Global Variables for Mouse
uint32_t p_byte;
uint8_t lb;
uint16_t x_mouse, y_mouse;
bool ih_return;
extern uint16_t hres;
extern uint16_t vres;
int hook_id_mouse = 10;

void(mouse_ih)() {

  uint32_t status_reg;

  sys_inb(STATUS_REG, &status_reg); // Reads the status register and puts it in 'status_reg'

  // Checks for errors on timeout/parity and checks if aux coms from the correct device (mouse)
  // If there are no errors, move the ob contents to the packet byte 'p_byte'
  if (((TMT_ERR | PAR_ERR) & status_reg) || !(status_reg & AUX)) {

    uint32_t erro;

    sys_inb(OUT_BUFF, &erro);
    ih_return = 1;
  }
  else {

    sys_inb(OUT_BUFF, &p_byte);
    ih_return = 0;
  }
}

void(mouse_subscribe)() {

  sys_irqsetpolicy(MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id_mouse);
}

void(mouse_unsubscribe)() {

  sys_irqrmpolicy(&hook_id_mouse);
}

void disable_mouse_ih() {

  sys_irqdisable(&hook_id_mouse);
}

void enable_mouse_ih() {

  sys_irqenable(&hook_id_mouse);
}

void mouse_command_stream(uint8_t arg) {

  uint32_t byte, status;

  sys_outb(KBC_CMD_REG, KBC_WRITE_MOUSE);
  sys_inb(STATUS_REG, &status);
  sys_outb(IN_BUFF, arg);
  sys_inb(OUT_BUFF, &byte);
}

// The following function was adapted for the goal of the project

void parse(uint8_t packets[], uint8_t *lb, uint16_t *x_mouse, uint16_t *y_mouse) {

  *lb = (packets[0] & BIT(0)); // Variable to be set with the state of left button of the mouse

  // Treatment of the x coordinate of the mouse
  if (packets[0] & BIT(4)) {
    *x_mouse += (packets[1] | SE_8_TO_16);
    if (*x_mouse <= 0) { *x_mouse = 1; }
    if (*x_mouse + 25 >= hres) { *x_mouse = hres - 26; }
  }
  else {
    *x_mouse += packets[1];
    if (*x_mouse <= 0) { *x_mouse = 1; }
    if (*x_mouse + 25 >= hres) { *x_mouse = hres - 26; }
  }

  // Treatment of the y coordinate of the mouse
  if (packets[0] & BIT(5)) {
    *y_mouse -= (packets[2] | SE_8_TO_16);
    if (*y_mouse <= 0) { *y_mouse = 1; }
    if (*y_mouse + 33 >= vres) { *y_mouse = vres - 34; }
  }
  else {
    *y_mouse -= packets[2];
    if (*y_mouse <= 0) { *y_mouse = 1; }
    if (*y_mouse + 33 >= vres) { *y_mouse = vres - 34; }
  }
}

