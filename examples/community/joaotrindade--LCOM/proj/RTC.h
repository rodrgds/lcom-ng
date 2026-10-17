#ifndef __RTC_H
#define __RTC_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include <minix/com.h>

//int test_conf();
int test_date();
int test_int();
/** @brief Devolve a data do sistema para as variavel de argumento */
void get_data(long int* dia, long int* mes, long int* ano, long int* hora, long int* minuto);
//void get_data(long int &dia, long int &mes, long int &ano, long int &hora, long int &minuto);

#endif /* __RTC_H */
