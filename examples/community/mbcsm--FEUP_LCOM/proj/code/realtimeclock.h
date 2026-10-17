#ifndef REALTIMECLOCK_H
#define REALTIMECLOCK_H

#define BIT(n) (0x01<<(n))

#define ADDRESS_REG       0x70
#define DATA_REG          0x71
#define RTC_IRQ           8

/*--- Register A ---*/
#define REG_A            10

#define REG_A_SECOND            0
#define REG_A_MINUTE            2
#define REG_A_HOUR              4
#define REG_A_DAY               7
#define REG_A_MONTH             8
#define REG_A_YEAR              9

#define REG_A_UIP        BIT(7) /* This bit is enabled when an update is in progress, data unsafe to read */

/*--- Register B ---*/
#define REG_B            11

#define REG_B_SET        BIT(7) /* If set, inhibits updates of time/date registers */
#define REG_B_PIE        BIT(6) /* For enabling periodic interrupts */
#define REG_B_AIE        BIT(5) /* For enabling alarm interrupts */
#define REG_B_UIE        BIT(4) /* For enabling update interrupts */
#define REG_B_DM         BIT(2) /* This bit is enabled when register data is binary, disabled when is BCD */

/*--- Register C ---*/
#define REG_C            12

#define REG_C_UE         BIT(4) /* Update interrupt pending */

/*--- Register D ---*/
#define REG_D            13

#endif
