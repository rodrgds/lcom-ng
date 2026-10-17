#pragma once
#include "video_gr.h"
#include "sprites/menu.xpm"
#include "sprites/highlight_button.xpm"
#include "sprites/menu_background.xpm"
#include "sprites/logotip.xpm"
#include "sprites/youLost.xpm"
#include "sprites/yourScoreWas.xpm"
#include "sprites/black_background.xpm"
#include "sprites/writename.xpm"
#include "sprites/scoreboard.xpm"
#include "game.h"  
#include "alphabet.h"   
#include "mouse.h"

xpm_image_t menu_img;
xpm_image_t menu_background_img;
xpm_image_t highlight_button_img;
xpm_image_t logotip_img;
xpm_image_t youLost_img;
xpm_image_t yourScoreWas_img;
xpm_image_t black_background_img;
xpm_image_t scoreboard_img;
xpm_image_t name;
xpm_image_t a;
xpm_image_t b;
xpm_image_t c;
xpm_image_t d;
xpm_image_t e;
xpm_image_t f;
xpm_image_t g;
xpm_image_t h;
xpm_image_t i;
xpm_image_t j;
xpm_image_t k;
xpm_image_t l;
xpm_image_t m;
xpm_image_t n;
xpm_image_t o;
xpm_image_t p;
xpm_image_t q;
xpm_image_t r;
xpm_image_t s;
xpm_image_t t;
xpm_image_t u;
xpm_image_t v;
xpm_image_t w;
xpm_image_t x;
xpm_image_t y;
xpm_image_t z;

typedef struct {
    int x, y;
    xpm_image_t img;
} Letter;

void (loadMainMenu)();

void (draw_main_menu_background)();

void MainMenuInterruptHandler(Device device);

void GameLostInterruptHandler(Device device);
void SaveInterruptHandler(Device device);
void ScoreBoardInterruptHandler(Device device);

void (loadLostMenu)();

void (savescore)(int score, char* name);
void (loadSaveMenu)();

void (letterhandle)(uint8_t lastletter);

void draw_scoreboard();

void draw_number(char *number,int liney);
void draw_name(char *name,int liney);





