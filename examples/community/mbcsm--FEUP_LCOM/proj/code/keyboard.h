#pragma once

int kbd_subscribe_int(uint8_t *bit_no);

int kbd_unsubscribe_int();

void kbd_ih();

void kbd_asm_ih();

int kbd_write(uint8_t cmd);

int kbd_read();

int kbd_cmd_reset(uint32_t cmd, uint32_t arg);
