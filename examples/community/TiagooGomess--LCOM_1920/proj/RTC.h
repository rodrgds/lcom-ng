#pragma once

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71
#define RTC_IRQ 8
#define HOUR_REGISTER 4
#define SECOND_REGISTER 0
#define A_REGISTER 10
#define B_REGISTER 11
#define BCD 0x04 // se o bit 2 for 1, então está em BCD
#define UPDATE_IN_PROGRESS 0x80 // se o bit 7 for 1, então entá a fazer update
#define UPDATE_TIME 244 // in micro seconds


/**
 * @brief - Subscribes and enables RTC interrupts.
 * @param bit_no - address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt.
 * @return - Return 0 upon success and non-zero otherwise.
 */
int rtc_subscribe_int(uint8_t* bit_no);

/**
 * @brief - Unsubscribes RTC interrupts.
 * @return - Return 0 upon success and non-zero otherwise.
 */
int rtc_unsubscribe_int();

/**
 * Lê a hora e o segundo atuais.
 */
void read_time();

/**
 * @return - true se passou 1 segundo, 0 caso contrário.
 */
bool a_second_passed();

/**
 * @brief - Hora e segundo atuais.
 */
typedef struct {
  uint8_t hour;      /*!< Hora atual. */
  uint8_t second;    /*!< Segundo atual. */
} Time;


