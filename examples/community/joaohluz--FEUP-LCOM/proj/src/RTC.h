#ifndef _RTC_H_
#define _RTC_H_

/** @defgroup RTC RTC
 * @{
 *
 * Funções para programar o RTC.
 */

#define RTC_ADDR_REG	0X70
#define RTC_DATA_REG	0X71
#define RTC_REG_A		0x0A
#define RTC_REG_B		0x0B
#define RTC_REG_C		12
#define RTC_REG_D		13

#define RTC_SECONDS  	0x00
#define RTC_MINUTES  	0x02
#define RTC_HOURS	  	0x04
#define RTC_DAYS	  	0x07
#define RTC_MONTH	  	0x08
#define RTC_YEAR  		0x09

#define RTC_UIP			(0x01 << 7)
#define BCD_MODE		(0x01 << 2)


/**
 * @brief Espera que RTC_UIP tenha o valor 0
 */
void wait_valid_rtc();

/**
 * @brief Devolve o valor do registo fornecido por argumento
 * @param reg - registo a ser verificado
 * @return Valor contido em reg
 */
uint32_t read_reg(int reg);

/**
 * @brief Devolve o valor contido em RTC_SECONDS
 * @return Segundos
 */
uint32_t get_Seconds();

/**
 * @brief Devolve o valor contido em RTC_MINUTES
 * @return Minutos
 */
uint32_t get_Minute();

/**
 * @brief Devolve o valor contido em RTC_HOURS
 * @return Horas
 */
uint32_t get_Hour();

/**
 * @brief Devolve o valor contido em RTC_DAYS
 * @return Dia do mes
 */
uint32_t get_Day();

/**
 * @brief Devolve o valor contido em RTC_MONTH
 * @return Mes
 */
uint32_t get_Month();

/**
 * @brief Devolve o valor contido em RTC_YEAR
 * @return Ano
 */
uint32_t get_Year();

/**@}*/

#endif
