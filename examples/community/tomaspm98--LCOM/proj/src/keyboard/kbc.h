#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <minix/sysutil.h>
#include "i8042.h"
#include <lcom/lcf.h>



/// @brief Read the KBC status
/// @param status KBC status
/// @return 0 if success, 1 if no success
int (read_KBC_status)(uint8_t* status);

#endif /* _LCOM_KBC_H_ */
