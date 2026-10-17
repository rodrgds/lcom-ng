#ifndef LAB3_KEYBOARD_H
#define LAB3_KEYBOARD_H

int (keyboard_subscribe_int)(int *irq_set) ;

int (keyboard_unsubscribe_int)();

void (keyboard_IH)();

int (print_scancode)();

int (keyboard_enable_interupts)();

int (keyboard_write_command)(uint8_t reg, uint32_t cmd);

int (keyboard_read_out_buff)(uint8_t *output);

#endif //LAB3_KEYBOARD_H
