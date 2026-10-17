#include <lcom/lcf.h>
#include "../images/0.xpm"
#include "../images/1.xpm"
#include "../images/2.xpm"
#include "../images/3.xpm"
#include "../images/4.xpm"
#include "../images/5.xpm"
#include "../images/6.xpm"
#include "../images/7.xpm"
#include "../images/8.xpm"
#include "../images/9.xpm"
#include "../images/dois_pontos100.xpm"

#define MINUTE 60 * 60

#define SEC 0x00
#define SEC_ALARM 0X01
#define MNT 0x02
#define MIN_ALARM 0X03
#define HOUR 0x04
#define HOUR_ALARM 0X05
#define DAY_OF_WEEK 0x06
#define DAY_OF_MONTH 0x07
#define MONTH 0x08
#define YEAR 0x09
#define REG_A 0x0A
#define REG_B 0x0B // set data to bcd format
#define REG_C 0x0C
#define REG_D 0x0D
#define RTC_ADDR_REG 0x70 // write control reg to this address
#define RTC_DATA_REG 0x71 // read data from this register
#define RTC_IRQ 0x08

// Reg_A
#define UIP BIT(7)

// Reg_B
#define HOURS_24_MODE BIT(1)
#define BCD_MODE BIT(2)
#define SQWE BIT(3)
#define INHIBIT_UPDATES BIT(7)

typedef struct {
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
  uint8_t day;
  uint8_t day_of_week;
  uint8_t month;
  uint8_t year;
} Date;

// Gets
uint8_t get_second();
uint8_t get_minute();
uint8_t get_hour();
uint8_t get_day();
uint8_t get_day_of_week();
uint8_t get_month();
uint8_t get_year();
uint8_t get_mode(uint8_t rtc_register);
Date get_date();

// Utils
int init_mode();
int disable_updates();
int enable_updates();
bool update_in_process();

