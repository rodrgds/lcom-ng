#include <lcom/lcf.h>     
#include <stdint.h>
#include "graphics.h"
#include "game.h"

static uint32_t ticks = 0;
static unsigned int seconds = 0;
uint32_t tempo = 0;

void (timer_ih)() {
    //clear_screen();
    ticks++;

    //gameUpdate();
    draw_screen();
    if (ticks == sys_hz()) {
        seconds++;
        ticks = 0;
    }
}

void (timer_reset)() {
    tempo = 0;                    // Define o tempo como 0
}

uint32_t (timer_get_elapsed)() {
    return tempo;
}
