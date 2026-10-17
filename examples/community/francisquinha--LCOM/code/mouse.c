/** @file mouse.c
 * @brief file containing definitions of the functions that deal with the game mouse
 */

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>

#include "mouse.h"
#include "mouse_static.h"
#include "i8042.h"

#define DEBUG_MALLOC 0

Mouse *create_mouse() {
	//allocate space for the "object"
	Mouse *mouse = (Mouse *)malloc(sizeof(Mouse));
	if (DEBUG_MALLOC) printf("Allocated %lu bytes for mouse\n", sizeof(Mouse));
	if (mouse == NULL)
		return NULL;
	mouse->bytes[0] = 0;
	mouse->bytes[1] = 0;
	mouse->bytes[2] = 0;
	mouse->byte_counter = 0;
	mouse->first_packet = 1;
	mouse->x = 0;
	mouse->y = 0;
	mouse->left = 0;
	mouse->middle = 0;
	mouse->right = 0;
	mouse->info = 0;
	return mouse;
}

void fix_mouse_position(Mouse* mouse, unsigned h_res, unsigned v_res) {
	if (mouse->x < 0) mouse->x = 0;
	if (mouse->y < 0) mouse->y = 0;
	if (mouse->x >= h_res) mouse->x = h_res - 1;
	if (mouse->y >= v_res) mouse->y = v_res - 1;
}

static void get_mse_info(Mouse *mouse) {
	mouse->info = 1;
	mouse->left = (mouse->bytes[0] & MSE_LB) ? 1 : 0;
	mouse->middle = (mouse->bytes[0] & MSE_MB) ? 1 : 0;
	mouse->right = (mouse->bytes[0] & MSE_RB) ? 1 : 0;
	int shift;
	if (mouse->bytes[0] & MSE_X_SIGN) shift = mouse->bytes[1] | 0xFFFFFF00;
	else shift = mouse->bytes[1];
	mouse->x += shift;
	if (mouse->bytes[0] & MSE_Y_SIGN) shift = mouse->bytes[2] | 0xFFFFFF00;
	else shift = mouse->bytes[2];
	mouse->y -= shift;
}

static long mse_read() {
	unsigned long data;
	unsigned long stat;
	unsigned short attempts = 0;
	/* loop while 8042 output buffer is empty and number attempts within limits*/
	while (attempts < MSE_ATTEMPTS) {
		sys_inb(KBC_STAT_REG, &stat);
		if ((stat & KBC_OBF) && (stat & KBC_AUX)) {
			sys_inb(KBC_OUT_BUF, &data);
			if ((stat & (KBC_PAR_ERR | KBC_TO_ERR)) == 0)
				return (long) data;
			else
				return -1;
		}
		tickdelay(micros_to_ticks(DELAY_KBC));
		attempts++;										/* increase number of attempts */
	}
}

static int mse_write(unsigned long port, unsigned long cmd) {
	unsigned long stat;
	unsigned short attempts = 0;
	/* loop while 8042 output buffer is empty and number attempts within limits*/
	while (attempts < MSE_ATTEMPTS) {
		sys_inb(KBC_STAT_REG, &stat);
		if ((stat & KBC_IBF) == 0) {
			sys_outb(port, cmd);
			return 0;
		}
		tickdelay(micros_to_ticks(DELAY_KBC));
		attempts++;										/* increase number of attempts */
	}
}

int mse_write_read(unsigned long cmd) {
	long mse_reply;
	unsigned short attempts = 0;
	while (attempts < MSE_ATTEMPTS) {
		mse_write(KBC_STAT_REG, KBC_WRT_MSE);					/* send the first command to forward argument to mouse */
		while (attempts < MSE_ATTEMPTS) {
			mse_write(KBC_CMD_REG, cmd);						/* send argument command to mouse */
			mse_reply = mse_read();								/* read mouse reply */
			if (mse_reply == KBC_MSE_ACK) return 0;				/* if there is positive acknowledgement return ok */
			else if (mse_reply == KBC_MSE_ERROR) {				/* if this is the second consecutive error */
				attempts++;										/* increase number of attempts */
				break;											/* and repeat the whole thing */
			}
			else {												/* else there is a negative acknowledgement, first error */
				attempts++;										/* increase number of attempts */
				continue;										/* and repeat the argument */
			}
		}
	}
	return 1;													/* all attempts have been used and the command write failed */
}

char **get_mouse_xpm() {
	return mouse_xpm;
}

void mouse_int_handler(Mouse *mouse) {
	int byte;
	byte = (int) mse_read();									/* get next byte */
	if (byte == -1) return;										/* there was some error, discard this info */
	if (mouse->first_packet && mouse->byte_counter == 0) {									/* this is the first packet, must check synchronization issues */
		if (byte & BIT(3)) {									/* if BIT(3) of this byte is on */
			mouse->byte_counter = 1;							/* this is probably the first byte, set byte counter to 1 */
			mouse->bytes[0] = byte;								/* and store this as first byte */
			return;
		}
		else if (mouse->byte_counter == 0) return;				/* BIT(3) is off and we have not received the first byte yet, ignore this byte */
	}
	/* in all other cases, we are either in the second or third byte of first packet with BIT(3) off, or we are in another packet, both are ok to proceed */
	mouse->bytes[mouse->byte_counter] = byte;					/* store the byte */
	mouse->byte_counter++;										/* increment the byte counter */
	if (mouse->byte_counter == 3 && mouse->info == 0) {			/* if this was the third byte */
		get_mse_info(mouse);
		mouse->byte_counter = 0;								/* reset byte counter */
		mouse->first_packet = 0;								/* reset first packet indicator */
	}
}
