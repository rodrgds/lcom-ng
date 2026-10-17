#ifndef KBC_H
#define KBC_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include "i8042.h"
//#include "bitmanip.h"

int kbc_read_status_buf(unsigned char *status);

int kbc_write_in_buf(unsigned char data);

int kbc_read_out_buf(unsigned char *data);

int kbc_send_command(unsigned char command);

int kbc_send_command_with_arg(volatile unsigned char command, volatile unsigned char arg);

int mouse_send_command(unsigned char command);

#endif
