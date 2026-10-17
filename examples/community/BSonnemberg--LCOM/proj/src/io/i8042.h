#pragma once

#define KBC_ST_REG 0x64
#define KBC_CMD_REG 0x64

#define KBC_INBUF 0x60
#define KBC_OUTBUF 0x60

#define KBC_ST_IBF BIT(1)
#define KBC_ST_OBF BIT(0)

// timeout and parity errors
#define KBC_ST_ERR (BIT(6) | BIT(7))

#define KBC_READ_CMDB 0x20
#define KBC_WRITE_CMDB 0x60

// delay between retrying
#define DELAY_TICKS micros_to_ticks(20000)
