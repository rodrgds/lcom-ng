
#include <lcom/lcf.h>
#include "video_card.h"
#include "Fundos.h"
#include "cursor.h"
#include "words.h"
#include "Numeros.h"

Sprite* spriteCursor;
Sprite *spriteFundo;
Sprite *spriteGameOver;
Sprite* spritePausa;
Sprite* spriteMenu;
Sprite* spriteLeaderboard;
Sprite* spriteRecordDia;
Sprite* spriteInstructions;

Sprite *sprite_A;
Sprite *sprite_B;
Sprite *sprite_C;
Sprite *sprite_D;
Sprite *sprite_E;
Sprite *sprite_F;
Sprite *sprite_G;
Sprite *sprite_H;
Sprite *sprite_I;
Sprite *sprite_J;
Sprite *sprite_K;
Sprite *sprite_L;
Sprite *sprite_M;
Sprite *sprite_N;
Sprite *sprite_O;
Sprite *sprite_P;
Sprite *sprite_Q;
Sprite *sprite_R;
Sprite *sprite_S;
Sprite *sprite_T;
Sprite *sprite_U;
Sprite *sprite_V;
Sprite *sprite_W;
Sprite *sprite_X;
Sprite *sprite_Y;
Sprite *sprite_Z;

Sprite *sprite_0;
Sprite *sprite_1;
Sprite *sprite_2;
Sprite *sprite_3;
Sprite *sprite_4;
Sprite *sprite_5;
Sprite *sprite_6;
Sprite *sprite_7;
Sprite *sprite_8;
Sprite *sprite_9;
Sprite *sprite_dois_pontos;

void *video_mem;
uint32_t horizontalResolution;
uint32_t verticalResolution;
uint32_t bitsPerPixel;
uint32_t memoryModel;
extern int x0_word;
extern int num_seconds;
extern int level;
extern Time time_rtc;
extern int mouse_x,mouse_y;

void* initialAdress;
uint8_t* secondBuffer;

struct minix_mem_range mr;


void *(vg_init)(uint16_t mode)
{
    vbe_mode_info_t info;

    if (vbe_get_mode_info(mode, &info) != 0)
        return NULL;

    // Vai buscar as informações necessárias de info
    horizontalResolution = info.XResolution;
    verticalResolution = info.YResolution;
    bitsPerPixel = info.BitsPerPixel;
    memoryModel = info.MemoryModel;

    memset(&mr, 0, sizeof(mr));  // Mete tudo a zero na struct

    unsigned int vram_base = info.PhysBasePtr;
    unsigned int vram_size = verticalResolution * horizontalResolution * (bitsPerPixel/8);
    mr.mr_base = (phys_bytes)vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    int r;
    if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size); // Mapeamento de memória
    if (video_mem == MAP_FAILED)
        panic("couldn't map video memory");

    // Mudança do modo gráfico
    struct reg86 r1;
    memset(&r1, 0, sizeof(r1));  // Mete tudo a zero na struct
    r1.ax = (uint16_t)((0x4F << 8) | 0x02);
    r1.bx = 1 << 14 | mode;
    r1.intno = 0x10;
    if (sys_int86(&r1) != OK)
    {
        printf("set_vbe_mode: sys_int86() failed \n");
        return NULL;
    }
    secondBuffer = malloc(vram_size);
    if (secondBuffer == NULL) {
        printf("Not enough memory for double buffer!\n");
        exit(1);
    }
    return video_mem;
}


void init_sprite_backgroud(xpm_map_t fundo) {
    spriteFundo = create_sprite(fundo, 0, 0);
}

void init_sprite_game_over(xpm_map_t fundo) {
    spriteGameOver = create_sprite(fundo, 0, 0);
}

void init_sprite_pausa(xpm_map_t fundo_pausa) {
    spritePausa = create_sprite(fundo_pausa, 0, 0);
}

void init_menu_backgroud(xpm_map_t menu) {
    spriteMenu = create_sprite(menu, 0, 0);
}

void init_leaderboard_background(xpm_map_t xpm) {
    spriteLeaderboard = create_sprite(xpm, 0, 0);
}

void init_record_backgroud_dia(xpm_map_t xpm) {
    spriteRecordDia = create_sprite(xpm, 0, 0);
}

void init_sprite_mouse(xpm_map_t cursor) {
    spriteCursor = create_sprite(cursor, 0, 0);
}

void init_sprite_instructions(xpm_map_t xpm) {
    spriteInstructions = create_sprite(xpm, 0, 0);
}

void init_letters_and_numbers_sprites() {
    sprite_A = create_sprite(getXPM('A'), 0, 0);
    sprite_B = create_sprite(getXPM('B'), 0, 0);
    sprite_C = create_sprite(getXPM('C'), 0, 0);
    sprite_D = create_sprite(getXPM('D'), 0, 0);
    sprite_E = create_sprite(getXPM('E'), 0, 0);
    sprite_F = create_sprite(getXPM('F'), 0, 0);
    sprite_G = create_sprite(getXPM('G'), 0, 0);
    sprite_H = create_sprite(getXPM('H'), 0, 0);
    sprite_I = create_sprite(getXPM('I'), 0, 0);
    sprite_J = create_sprite(getXPM('J'), 0, 0);
    sprite_K = create_sprite(getXPM('K'), 0, 0);
    sprite_L = create_sprite(getXPM('L'), 0, 0);
    sprite_M = create_sprite(getXPM('M'), 0, 0);
    sprite_N = create_sprite(getXPM('N'), 0, 0);
    sprite_O = create_sprite(getXPM('O'), 0, 0);
    sprite_P = create_sprite(getXPM('P'), 0, 0);
    sprite_Q = create_sprite(getXPM('Q'), 0, 0);
    sprite_R = create_sprite(getXPM('R'), 0, 0);
    sprite_S = create_sprite(getXPM('S'), 0, 0);
    sprite_T = create_sprite(getXPM('T'), 0, 0);
    sprite_U = create_sprite(getXPM('U'), 0, 0);
    sprite_V = create_sprite(getXPM('V'), 0, 0);
    sprite_W = create_sprite(getXPM('W'), 0, 0);
    sprite_X = create_sprite(getXPM('X'), 0, 0);
    sprite_Y = create_sprite(getXPM('Y'), 0, 0);
    sprite_Z = create_sprite(getXPM('Z'), 0, 0);

    sprite_0 = create_sprite(getXPM('0'), 0, 0);
    sprite_1 = create_sprite(getXPM('1'), 0, 0);
    sprite_2 = create_sprite(getXPM('2'), 0, 0);
    sprite_3 = create_sprite(getXPM('3'), 0, 0);
    sprite_4 = create_sprite(getXPM('4'), 0, 0);
    sprite_5 = create_sprite(getXPM('5'), 0, 0);
    sprite_6 = create_sprite(getXPM('6'), 0, 0);
    sprite_7 = create_sprite(getXPM('7'), 0, 0);
    sprite_8 = create_sprite(getXPM('8'), 0, 0);
    sprite_9 = create_sprite(getXPM('9'), 0, 0);
    sprite_dois_pontos = create_sprite(dois_pontos , 1125, 127);
}

bool is_day() { // return false se for noite e true se for dia
    if (time_rtc.hour > 18 || time_rtc.hour < 7)
        return false;
    return true;
}

void init_all_sprites_day() {
    init_letters_and_numbers_sprites();
    init_sprite_pausa(fundo_pausa_dia);
    init_sprite_backgroud(fundo_dia);
    init_sprite_game_over(Game_Over_Dia_xpm);
    init_menu_backgroud(Menu_dia_xpm);
    init_leaderboard_background(Leaderboard_dia_xpm);
    init_record_backgroud_dia(Recorde_Dia_xpm);
    init_sprite_mouse(rato_xpm);
    init_sprite_instructions(instructions_day_xpm);
}

void init_all_sprites_night() {
    init_letters_and_numbers_sprites();
    init_sprite_pausa(fundo_pausa_noite);
    init_sprite_backgroud(fundo_noite);
    init_sprite_game_over(Game_Over_Noite_xpm);
    init_menu_backgroud(Menu_noite_xpm);
    init_leaderboard_background(Leaderboard_noite_xpm);
    init_record_backgroud_dia(Recorde_Noite_xpm);
    init_sprite_mouse(rato_xpm);
    init_sprite_instructions(instructions_night_xpm);
}

void init_all_sprites() {
    read_time();
    if (is_day()) {
        init_all_sprites_day();
        return;
    }
    else {
        init_all_sprites_night();
        return;
    }
}


void draw_backgroud(Sprite* sprite, uint8_t* initial_address) {
    draw_sprite(sprite, initial_address);
}

void draw_cursor(Sprite* sprite, uint8_t* initial_address) {
    spriteCursor->x = mouse_x;
    spriteCursor->y = mouse_y;
    draw_sprite(sprite,initial_address);
}

void draw_timer(int num_seconds, uint8_t* initial_address) {
    int y = 127;
    if (num_seconds >= 60) {
        int minutes = num_seconds / 60;
        num_seconds = num_seconds % 60;
        displayNumber(1090, y, minutes, initial_address);
        draw_sprite(sprite_dois_pontos, initial_address);
        if (num_seconds < 10) {
            displayNumber(1160, y, 0, initial_address);
            displayNumber(1205, y, num_seconds, initial_address);
        }
        else {
            displayNumber(1160, y, num_seconds, initial_address);
        }
    }
    else {
        displayNumber(1090, y, 0, initial_address);
        draw_sprite(sprite_dois_pontos, initial_address);
        if (num_seconds < 10) {
            displayNumber(1160, y, 0, initial_address);
            displayNumber(1205, y, num_seconds, initial_address);
        }
        else {
            displayNumber(1160, y, num_seconds, initial_address);
        }
    }
}

void display_level(uint8_t* initial_address) {
    displayNumber(1150, 217, level, initial_address);
}


void draw_everything_in_game(char* word, int score, uint8_t* initial_address, int y0_word) {

    draw_backgroud(spriteFundo, initial_address);
    displayWord(x0_word, y0_word, word, initial_address);
    displayNumber(1150, 30, score, initial_address); // score
    draw_timer(num_seconds, initial_address);
    display_level(initial_address);
}

void free_letters_and_numbers() {
    free(sprite_0);
    free(sprite_1);
    free(sprite_2);
    free(sprite_3);
    free(sprite_4);
    free(sprite_5);
    free(sprite_6);
    free(sprite_7);
    free(sprite_8);
    free(sprite_9);
    free(sprite_A);
    free(sprite_B);
    free(sprite_C);
    free(sprite_D);
    free(sprite_E);
    free(sprite_F);
    free(sprite_G);
    free(sprite_H);
    free(sprite_I);
    free(sprite_J);
    free(sprite_K);
    free(sprite_L);
    free(sprite_M);
    free(sprite_N);
    free(sprite_O);
    free(sprite_P);
    free(sprite_Q);
    free(sprite_R);
    free(sprite_S);
    free(sprite_T);
    free(sprite_U);
    free(sprite_V);
    free(sprite_W);
    free(sprite_X);
    free(sprite_Y);
    free(sprite_Z);
}

void free_everything() {
    free_letters_and_numbers();
    free(spritePausa);
    free(spriteFundo);
    free(secondBuffer);
    free(spriteCursor);
}
