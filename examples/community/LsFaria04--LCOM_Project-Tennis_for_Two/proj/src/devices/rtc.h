#ifndef RTC_H
#define RTC_H

#include <lcom/lcf.h>

#define  RTC_ADDR_REG 0x70
#define  RTC_DATA_REG 0x71 

#define DAY 7
#define MONTH 8
#define YEAR 9

#define REG_A 10
#define REG_A_UIP BIT(7)

#define REG_B 11
#define REG_B_DM BIT(2)

/**
 * @brief Obtém a data atual.
 * @param day dia atual
 * @param month mês atual
 * @param year ano atual
 * @returns 1 se ocorrer um erro e 0 caso tenha corrido tudo bem
*/
int (get_date)(uint8_t *day, uint8_t *month, uint8_t *year);

/** 
 * @brief Programa o RTC para enviar valores em binário
 * @returns 1 se ocorrer um erro e 0 caso tenha corrido tudo bem
*/
int (set_binary)();

/**
 * @brief Lê os valores que estão no rtc
 * @param command Comando enviado com informação do valor que queremos ler (dia, mês, ...)
 * @param output Valor lido pelo rtc
 * @returns 1 se ocorrer um erro e 0 caso tenha corrido tudo bem
*/
int (read_RTC)(uint8_t command, uint8_t *output);

#endif
