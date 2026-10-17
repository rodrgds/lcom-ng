#include "scoreboard.h"
#include "font.h"
#include <lcom/lcf.h>
#include <string.h>
#include <stdio.h>

// store best score per level 
static score_entry_t scores[MAX_SCORES];

void scoreboard_init() {
    memset(scores, 0, sizeof(scores));
}

void scoreboard_add(const char *name, int level, int scr, int suns_spent,
                    rtc_date *date) {
    int idx = level - 1;
    if (idx < 0 || idx >= MAX_SCORES) return;

    // save if first time or spent fewer suns
    if (!scores[idx].valid || suns_spent < scores[idx].suns_spent) {
        strncpy(scores[idx].name, name, 12);
        scores[idx].name[12] = '\0';
        scores[idx].score = scr;
        scores[idx].suns_spent = suns_spent;
        scores[idx].level = level;
        scores[idx].date = *date;
        scores[idx].valid = true;
    }
}

score_entry_t* scoreboard_get(int level) {
    int idx = level - 1;
    if (idx < 0 || idx >= MAX_SCORES) return NULL;
    if (!scores[idx].valid) return NULL;
    return &scores[idx];
}

void scoreboard_render() {
    // dark background
    vg_draw_rectangle(0, 0, 800, 600, 0x1A1A2E);

    // title
    font_draw_string("SCOREBOARD", 310, 30, 0x00FF00);

    // column headers 
    font_draw_string("LVL", 30, 70, 0xAAAAFF);
    font_draw_string("NAME", 100, 70, 0xAAAAFF);
    font_draw_string("SCORE", 280, 70, 0xAAAAFF);
    font_draw_string("SUNS", 400, 70, 0xAAAAFF);
    font_draw_string("DATE", 520, 70, 0xAAAAFF);

    // separator 
    vg_draw_rectangle(20, 90, 760, 2, 0x444466);

    // draw each level entry 
    for (int i = 0; i < MAX_SCORES; i++) {
        int y = 105 + i * 45;

        font_draw_number(i + 1, 40, y, 0xFFFFFF);

        if (scores[i].valid) {
            font_draw_string(scores[i].name, 100, y, 0xFFFFFF);
            font_draw_number(scores[i].score, 280, y, 0xFFFF00);
            font_draw_number(scores[i].suns_spent, 400, y, 0xFF8800);

            // date: DD/MM/YY HH:MM /
            char date_str[20];
            sprintf(date_str, "%02d/%02d/%02d %02d:%02d",
                    scores[i].date.day, scores[i].date.month,
                    scores[i].date.year, scores[i].date.hours,
                    scores[i].date.minutes);
            font_draw_string(date_str, 520, y, 0xCCCCCC);
        } else {
            font_draw_string("---", 100, y, 0x555555);
        }
    }

    // instructions 
    font_draw_string("PRESS ESC TO RETURN", 260, 560, 0xFF0000);
}
