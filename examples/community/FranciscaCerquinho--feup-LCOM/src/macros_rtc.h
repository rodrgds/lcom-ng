#define BIT(n) (0x01<<(n))

#define IRQ_LINE_RTC 8

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71


#define SECONDS_REG_RTC 0
#define MINUTES_REG_RTC 2
#define HOUR_REG_RTC 4
#define DAY_REG_RTC 7
#define MONTH_REG_RTC 8
#define YEAR_REG_RTC 9
#define REGISTER_A 10
#define REGISTER_B 11
#define UIP_SET BIT(7)
