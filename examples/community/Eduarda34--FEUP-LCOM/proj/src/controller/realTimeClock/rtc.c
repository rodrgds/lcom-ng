#include <lcom/lcf.h>
#include "rtc.h"
#include "game/game.h"

extern StateGame game_st;

int rtc_hook_id = 5;
rtc_info time_info;

void rtc_setup() {
    rtc_update_time();
    rtc_enable_periodic_interrupts();
}

int (rtc_subscribe_int)(uint8_t *bit_no) {
    if(bit_no == NULL) return 1;
    *bit_no = BIT(rtc_hook_id);

    if (sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE, &rtc_hook_id) != 0) {
        return 1;
    }
    return 0;
}

int (rtc_unsubscribe_int)(){
    if (sys_irqrmpolicy(&rtc_hook_id) != 0) {
        return 1;
    }
    return 0;
}

void (rtc_ih)(){
    uint8_t regC;
    rtc_output(REGISTER_C, &regC); // This also clears the interrupt internally

    if (regC & RTC_PF) {
        static int pf_counter = 0;
        pf_counter++;

        // Trigger action once every 2 PFs = ~1 second (at 2Hz)
        if (pf_counter % 2 == 0) {
            if (game_st == GAME_RUNNING) {
                score_increment();
            }
        }
    }
}

void rtc_enable_periodic_interrupts() {
    uint8_t prev_A, prev_B;

    sys_outb(REGISTER_INPUT, 0x80 | REGISTER_B);
    util_sys_inb(REGISTER_OUTPUT, &prev_B);

    sys_outb(REGISTER_INPUT, 0x80 | REGISTER_B);
    sys_outb(REGISTER_OUTPUT, prev_B | BIT(6));

    sys_outb(REGISTER_INPUT, 0x80 | REGISTER_A);
    util_sys_inb(REGISTER_OUTPUT, &prev_A);

    sys_outb(REGISTER_INPUT, 0x80 | REGISTER_A);
    sys_outb(REGISTER_OUTPUT, (prev_A & 0xF0) | 0x0F); // Set rate to 15 (2Hz)
}


int rtc_output(uint8_t command, uint8_t *output) {
    if (sys_outb(REGISTER_INPUT, command) != 0) return 1;
	if (util_sys_inb(REGISTER_OUTPUT, output) != 0) return 1;
    return 0;
}

int rtc_is_updating() {
    uint8_t result;
    if (rtc_output(REGISTER_UPDATING, &result)) return 1;
	return result & REGISTER_UPDATING;
}

int rtc_update_time() {
    if (rtc_is_updating() != 0) return 1;
    uint8_t output;

    if (rtc_output(0, &output) != 0) return 1;
    time_info.seconds = output;

    return 0;
}
