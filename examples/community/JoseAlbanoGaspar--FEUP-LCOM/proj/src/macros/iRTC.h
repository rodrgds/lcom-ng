//
// Created by barba on 10/06/2022.
//

#ifndef SRC_IRTC_H
#define SRC_IRTC_H
#include <lcom/lcf.h>

/** @defgroup RTC RTC
 * Basic functions to operate via polling mode on the Real-Time Clock (RTC).
 */

/* Access to RTC */

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71


/* RTC’s Internal Address Space */

/* REGISTER */
#define REGA 10
#define REGB 11

/* INPUTS */
#define DAY   7
#define MONTH 8
#define YEAR  9

#define HOURS   4
#define MINUTES 2
#define SECONDS 0

/* FLAGS */
#define REGA_UIP BIT(7)
#define REGB_BIN BIT(2)


/* CURENT MILLENIUM  */
#define CURRENT_MILLENIUM 2000


/**@}*/

#endif // SRC_IRTC_H
