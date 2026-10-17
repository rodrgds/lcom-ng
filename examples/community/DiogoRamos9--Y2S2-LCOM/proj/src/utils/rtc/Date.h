#include <lcom/lcf.h>

typedef struct {
  uint8_t  min, hour, day, month, year;

} Date;

Date * Loaddate();

void get_date_time();

void updtdate();

