/**
@file rtc.h
@brief Constants and functions for programming the RTC (Real-Time Clock) hardware */

#ifndef RTC_H
#define RTC_H

#include <minix/sysutil.h>
#include <lcom/lcf.h>

/**

@struct clock_fields_struct
@brief Struct that represents the fields of a clock (hours, minutes, seconds, day, month, year) */
typedef struct{ 
    uint8_t sec; /**< seconds */ 
    uint8_t min; /**< minutes */ 
    uint8_t h; /**< hours */ 
    uint8_t d; /**< day */ 
    uint8_t m; /**< month */ 
    uint8_t y; /**< year */ 
    } clock_fields_struct;

#define rtc_irq 8 /**<@brief RTC IRQ line number */ 
#define REG_A 10 /**<@brief Register A address */ 
#define REG_B 11 /**<@brief Register B address */

/**
@brief Subscribes the RTC interruptions
@param bit_no pointer to the variable that will store the IRQ bit number
@return Return 0 upon success, 1 otherwise */
int rtc_subscribe_int(uint8_t *bit_no);

/**
@brief Unsubscribes the RTC interruptions
@return Return 0 upon success, 1 otherwise */
int rtc_unsubscribe_int();

/**
@brief Gets the current date and time from the RTC hardware */
void get_rtc_info();

/**
@brief Gets the mode of operation of the RTC
@return Return 0 if the RTC is in binary mode, 1 if it is in BCD mode */
int get_rtc_mode();

/**
@brief Extracts the data from a specific register in the RTC hardware
@param reg_to_access The address of the register to access
@param output_data The pointer to the variable that will store the data extracted from the register
@return Return 0 upon success, 1 otherwise */
int extract_rtc_data(uint8_t reg_to_access, uint8_t *output_data);

/**
@brief Checks if the RTC is doing an update
@return Return 0 if the RTC is not doing an update, 1 otherwise */
int rtc_doing_update();

/**
@brief Converts a value in binary-coded decimal (BCD) to binary
@param bcd_value The BCD value to convert
@return The binary value */
uint8_t convert_in_binary(uint8_t bcd_value);

/**
@brief Updates the struct that stores the current date and time based on the values returned by the RTC
@return Return 0 upon success, 1 otherwise */
int clock_fields_update();

#endif


/*
#ifndef RTC_H
#define RTC_H

#include <minix/sysutil.h>
#include <lcom/lcf.h>

typedef struct {
    uint8_t sec; //segundos
    uint8_t min; //minutos
    uint8_t h; //horas
    uint8_t d; //dia
    uint8_t m; //mes
    uint8_t y; //ano
} clock_fields_struct;

#define rtc_irq    8
#define REG_A      10
#define REG_B      11

// Functions
int rtc_subscribe_int(uint8_t *bit_no);
int rtc_unsubscribe_int();
void get_rtc_info();
int get_rtc_mode();
int extract_rtc_data(uint8_t reg_to_access, uint8_t *output_data);
int rtc_doing_update();
uint8_t convert_in_binary(uint8_t bcd_value);
int clock_fields_update();

#endif
*/
