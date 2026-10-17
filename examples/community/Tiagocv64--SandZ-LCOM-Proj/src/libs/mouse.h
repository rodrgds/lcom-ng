#pragma once

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

void (mouse_ih)();

int write_cmd(uint32_t cmd);

int write_arg(uint32_t arg);

int write_cmd_and_arg(uint32_t cmd, uint32_t arg);

int write_cmd_to_mouse(uint32_t mouse_cmd);

int write_cmd_mouse_and_check(uint32_t mouse_cmd);

bool interpreter_packet(struct packet* pak);
