#include <lcom/lcf.h>

// Macros
#define hook_idrtc            5
#define rtc_irql              8
#define secrtc        0
#define minrtc        2
#define hoursrtc      4   
#define dayrtc        7
#define monthrtc      8
#define yearrtc       9

#define inp_reg      0x70
#define out_reg     0x71

#define upd_reg     10
#define count_reg   11

#define bin            BIT(2)
#define updt           BIT(7)

// Estrutura que reune todos os dados importantes
typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} real_time_info;

// Functions

int (rtc_subscribe_int)(uint8_t *bit_no) ;
int (rtc_unsubscribe_int)();
int (rtc_read)(uint8_t inp);
int (rtcupdating)();
uint8_t convert_tobin (uint8_t bcd);
void (LoadRtc)();
