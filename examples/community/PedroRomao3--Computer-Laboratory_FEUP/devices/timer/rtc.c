#include "rtc.h"

int rtc_timer_hook_id = 4;

time_info data;

int read_rtc(uint8_t command, uint8_t * value) {
  if (sys_outb(0x70, command)) return 1;
  if (util_sys_inb(0x71, value)) return 1;
  return 0;
}

uint8_t result_time(uint8_t value, uint8_t is_bcm) {
  //printf("input value: %u ; ", value);
  if(!is_bcm){
    //printf("passed value: %u.\n", (((value / 16) * 10) + (value % 16)));
    return ((value / 16) * 10) + (value % 16); 
  }else 
    return value;
}

uint8_t get_value(uint8_t addr, uint8_t is_bcm) {
  uint8_t value;
  if(read_rtc(addr, &value)) return 100;
  return result_time(value, is_bcm);
}

int set_time() {
  // verificar se podemos ler data
  uint8_t r;
  if(read_rtc(10, &r)) return 1;
  if(r & BIT(7)) return 1; // o rtc esta dar a update a data

  // verificar o mode da contagem
  if(read_rtc(11, &r)) return 1;
  uint8_t is_bcm = r & BIT(2);

  if((data.segunda = get_value(0, is_bcm)) == 100) return 1;
  if((data.minuta = get_value(2, is_bcm)) == 100) return 1;
  if((data.hora = get_value(4, is_bcm)) == 100) return 1;
  if((data.dia = get_value(7, is_bcm)) == 100) return 1;
  if((data.mes = get_value(8, is_bcm)) == 100) return 1;
  if((data.ano = get_value(9, is_bcm)) == 100) return 1;

  //printf("time: %u:%u:%u:%u:%u:%u\n", data.ano, data.mes, data.dia, data.hora, data.minuta, data.segunda);

  return 0;
}

uint8_t is_leap_year(uint8_t year) {
  return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

uint8_t days_in_month(uint8_t month, uint8_t year) {
  uint8_t days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (month == 2 && is_leap_year(year)) return 29;
  return days[month - 1];
}

void add_second() {
    data.segunda++;
    if (data.segunda >= 60) {
        data.segunda = 0;
        data.minuta++;
        if (data.minuta >= 60) {
            data.minuta = 0;
            data.hora++;
            if (data.hora >= 24) {
                data.hora = 0;
                data.dia++;
                if (data.dia > days_in_month(data.mes, data.ano)) {
                    data.dia = 1;
                    data.mes++;
                    if (data.mes > 12) {
                        data.mes = 1;
                        data.ano++;
                    }
                }
            }
        }
    }
}

bool is_past(time_info current_time, time_info to_check) {
  if (to_check.ano < current_time.ano) return true;
  if (to_check.ano > current_time.ano) return false;
  
  if (to_check.mes < current_time.mes) return true;
  if (to_check.mes > current_time.mes) return false;
  
  if (to_check.dia < current_time.dia) return true;
  if (to_check.dia > current_time.dia) return false;

  if (to_check.hora < current_time.hora) return true;
  if (to_check.hora > current_time.hora) return false;
  
  return to_check.minuta < current_time.minuta;
}

time_difference calculate_time_difference(time_info current_time, time_info to_check) {
  time_difference diff = {-1, -1, -1, -1};
  if (is_past(current_time, to_check)) return diff;

  diff.months = (to_check.ano - current_time.ano) * 12 + to_check.mes - current_time.mes;
  diff.days = to_check.dia - current_time.dia;
  if (diff.days < 0) {
    diff.months--;
    diff.days += days_in_month(current_time.mes, current_time.ano);
  }

  diff.hours = to_check.hora - current_time.hora;
  if (diff.hours < 0) {
    diff.days--;
    diff.hours += 24;
  }

  diff.minutes = to_check.minuta - current_time.minuta;
  if (diff.minutes < 0) {
    diff.hours--;
    diff.minutes += 60;
  }

  return diff;
}
