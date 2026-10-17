#include "graphics.h"
#include "game.h"
#include "deck.h"


int check_state = -1;
int check_p_cards = -1;
int check_d_cards = -1;
int check_money = -1;
int check_bet = -1;

uint32_t *video_mem = NULL;
vbe_mode_info_t vmi;

int yMax = 864;
int xMax = 1152;
int bytespp = 4;

// Optimized cursor buffer
#define CURSOR_BUFFER_SIZE (32 * 32)  // Assuming max cursor size
static uint32_t cursor_background[CURSOR_BUFFER_SIZE];
static int prev_mouse_x = 0;
static int prev_mouse_y = 0;
static bool first_draw = true;
static int mouse_prev_state = -1; // Mouse's own state tracker

xpm_image_t pixmap_list[18];
xpm_image_t cards_list[53];
xpm_image_t numbers[10];
xpm_image_t cursor;

void(loadPixmap)() {
    printf("\nLoading screens ...\n");
    xpm_load(menu1_xpm, XPM_8_8_8_8, &pixmap_list[0]);
    xpm_load(menu2_xpm, XPM_8_8_8_8, &pixmap_list[1]);
    xpm_load(bet1_xpm, XPM_8_8_8_8, &pixmap_list[2]);
    xpm_load(bet2_xpm, XPM_8_8_8_8, &pixmap_list[3]);
    xpm_load(bet3_xpm, XPM_8_8_8_8, &pixmap_list[4]);
    xpm_load(bet4_xpm, XPM_8_8_8_8, &pixmap_list[5]);
    xpm_load(bet5_xpm, XPM_8_8_8_8, &pixmap_list[6]);
    xpm_load(playing1_xpm, XPM_8_8_8_8, &pixmap_list[7]);
    xpm_load(playing2_xpm, XPM_8_8_8_8, &pixmap_list[8]);
    xpm_load(playing3_xpm, XPM_8_8_8_8, &pixmap_list[9]);
    xpm_load(dealerBet_xpm, XPM_8_8_8_8, &pixmap_list[10]);
    xpm_load(dealerExit_xpm, XPM_8_8_8_8, &pixmap_list[11]);
    xpm_load(winBet_xpm, XPM_8_8_8_8, &pixmap_list[12]);
    xpm_load(winExit_xpm, XPM_8_8_8_8, &pixmap_list[13]);
    xpm_load(pushBet_xpm, XPM_8_8_8_8, &pixmap_list[14]);
    xpm_load(pushExit_xpm, XPM_8_8_8_8, &pixmap_list[15]);
    xpm_load(bjBet_xpm, XPM_8_8_8_8, &pixmap_list[16]);
    xpm_load(bjExit_xpm, XPM_8_8_8_8, &pixmap_list[17]);

    //printf("\nLoaded screens\n");
}

void (loadCards)() {
    printf("\nLoading cards ...\n");

    xpm_load(C2, XPM_8_8_8_8, &cards_list[0]);
    xpm_load(D2, XPM_8_8_8_8, &cards_list[1]);
    xpm_load(H2, XPM_8_8_8_8, &cards_list[2]);
    xpm_load(S2, XPM_8_8_8_8, &cards_list[3]);

    xpm_load(C3, XPM_8_8_8_8, &cards_list[4]);
    xpm_load(D3, XPM_8_8_8_8, &cards_list[5]);
    xpm_load(H3, XPM_8_8_8_8, &cards_list[6]);
    xpm_load(S3, XPM_8_8_8_8, &cards_list[7]);

    xpm_load(C4, XPM_8_8_8_8, &cards_list[8]);
    xpm_load(D4, XPM_8_8_8_8, &cards_list[9]);
    xpm_load(H4, XPM_8_8_8_8, &cards_list[10]);
    xpm_load(S4, XPM_8_8_8_8, &cards_list[11]);

    xpm_load(C5, XPM_8_8_8_8, &cards_list[12]);
    xpm_load(D5, XPM_8_8_8_8, &cards_list[13]);
    xpm_load(H5, XPM_8_8_8_8, &cards_list[14]);
    xpm_load(S5, XPM_8_8_8_8, &cards_list[15]);

    xpm_load(C6, XPM_8_8_8_8, &cards_list[16]);
    xpm_load(D6, XPM_8_8_8_8, &cards_list[17]);
    xpm_load(H6, XPM_8_8_8_8, &cards_list[18]);
    xpm_load(S6, XPM_8_8_8_8, &cards_list[19]);

    xpm_load(C7, XPM_8_8_8_8, &cards_list[20]);
    xpm_load(D7, XPM_8_8_8_8, &cards_list[21]);
    xpm_load(H7, XPM_8_8_8_8, &cards_list[22]);
    xpm_load(S7, XPM_8_8_8_8, &cards_list[23]);

    xpm_load(C8, XPM_8_8_8_8, &cards_list[24]);
    xpm_load(D8, XPM_8_8_8_8, &cards_list[25]);
    xpm_load(H8, XPM_8_8_8_8, &cards_list[26]);
    xpm_load(S8, XPM_8_8_8_8, &cards_list[27]);

    xpm_load(C9, XPM_8_8_8_8, &cards_list[28]);
    xpm_load(D9, XPM_8_8_8_8, &cards_list[29]);
    xpm_load(H9, XPM_8_8_8_8, &cards_list[30]);
    xpm_load(S9, XPM_8_8_8_8, &cards_list[31]);

    xpm_load(C10, XPM_8_8_8_8, &cards_list[32]);
    xpm_load(D10, XPM_8_8_8_8, &cards_list[33]);
    xpm_load(H10, XPM_8_8_8_8, &cards_list[34]);
    xpm_load(S10, XPM_8_8_8_8, &cards_list[35]);

    xpm_load(CA, XPM_8_8_8_8, &cards_list[36]);
    xpm_load(DA, XPM_8_8_8_8, &cards_list[37]);
    xpm_load(HA, XPM_8_8_8_8, &cards_list[38]);
    xpm_load(SA, XPM_8_8_8_8, &cards_list[39]);

    xpm_load(CJ, XPM_8_8_8_8, &cards_list[40]);
    xpm_load(DJ, XPM_8_8_8_8, &cards_list[41]);
    xpm_load(HJ, XPM_8_8_8_8, &cards_list[42]);
    xpm_load(SJ, XPM_8_8_8_8, &cards_list[43]);

    xpm_load(CK, XPM_8_8_8_8, &cards_list[44]);
    xpm_load(DK, XPM_8_8_8_8, &cards_list[45]);
    xpm_load(HK, XPM_8_8_8_8, &cards_list[46]);
    xpm_load(SK, XPM_8_8_8_8, &cards_list[47]);

    xpm_load(CQ, XPM_8_8_8_8, &cards_list[48]);
    xpm_load(DQ, XPM_8_8_8_8, &cards_list[49]);
    xpm_load(HQ, XPM_8_8_8_8, &cards_list[50]);
    xpm_load(SQ, XPM_8_8_8_8, &cards_list[51]);

    xpm_load(RB, XPM_8_8_8_8, &cards_list[52]);
    
    //printf("\nLoaded cards\n");
}

void (loadNumbers)() {
    printf("\nLoading numbers ...\n");
    xpm_load(_0_xpm, XPM_8_8_8_8, &numbers[0]);
    xpm_load(_1_xpm, XPM_8_8_8_8, &numbers[1]);
    xpm_load(_2_xpm, XPM_8_8_8_8, &numbers[2]);
    xpm_load(_3_xpm, XPM_8_8_8_8, &numbers[3]);
    xpm_load(_4_xpm, XPM_8_8_8_8, &numbers[4]);
    xpm_load(_5_xpm, XPM_8_8_8_8, &numbers[5]);
    xpm_load(_6_xpm, XPM_8_8_8_8, &numbers[6]);
    xpm_load(_7_xpm, XPM_8_8_8_8, &numbers[7]);
    xpm_load(_8_xpm, XPM_8_8_8_8, &numbers[8]);
    xpm_load(_9_xpm, XPM_8_8_8_8, &numbers[9]);
    //printf("\nLoaded numbers ...\n");
}

void (loadCursor)() {
    printf("\nLoading cursor ...\n");
    xpm_load(cursor_xpm, XPM_8_8_8_8, &cursor);
    //printf("\nLoading cursor ...\n");
}

void(clear_screen)()
{
    for (int dy = 0; dy < yMax; dy++)
    {
        for (int dx = 0; dx < xMax; dx++)
        {
            uint32_t *pixel = video_mem + ((dy) * (xMax) + dx) * bytespp;
            *pixel = 0x00;
        }
    }
}

int(graphics_exit)()
{
    reg86_t r86;
    memset(&r86, 0, sizeof(r86));

    r86.intno = 0x10;
    r86.ah = 0x00;
    r86.al = 0x03;

    if (sys_int86(&r86) != OK)
        return 1;
    return 0;
}

int(graphics_init)()
{
    reg86_t r86;
    memset(&r86, 0, sizeof(r86));

    r86.intno = 0x10;
    r86.ah = 0x4F;
    r86.al = 0x02;
    r86.bx = 0x14C | BIT(14);

    if (sys_int86(&r86) != OK)
        return 1;

    if (vbe_get_mode_info(0x14C, &vmi) != OK)
        return 1;

    struct minix_mem_range mr;
    mr.mr_base = (phys_bytes)vmi.PhysBasePtr;
    mr.mr_limit = mr.mr_base + (vmi.XResolution * vmi.YResolution * (vmi.BitsPerPixel / 8));

    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != OK)
        return 1;
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, mr.mr_limit - mr.mr_base);

    return (video_mem == MAP_FAILED) ? 1 : 0;
}

void (draw_mouse)() {
    if (!video_mem) return;

    int current_x = mouse_x;
    int current_y = mouse_y;

    if (mouse_prev_state != state) {
        first_draw = true;
    }

    if (!first_draw && current_x == prev_mouse_x && current_y == prev_mouse_y && state == mouse_prev_state) {
        return;
    }

    uint32_t *prev_pos = video_mem + (prev_mouse_y * vmi.XResolution + prev_mouse_x);
    uint32_t *curr_pos = video_mem + (current_y * vmi.XResolution + current_x);

    if (!first_draw) {
        for (int y = 0; y < cursor.height; y++) {
            memcpy(prev_pos + (y * vmi.XResolution), &cursor_background[y * cursor.width], cursor.width * sizeof(uint32_t));
        }
    }

    for (int y = 0; y < cursor.height; y++) {
        uint32_t *pixel = curr_pos + (y * vmi.XResolution);
        uint32_t *src_pixel = (uint32_t*) cursor.bytes + (y * cursor.width);
        for (int x = 0; x < cursor.width; x++) {
            cursor_background[y * cursor.width + x] = pixel[x];
            if (src_pixel[x] != 0x00fa07dd) { 
                pixel[x] = src_pixel[x];
            }
        }
    }

    prev_mouse_x = current_x;
    prev_mouse_y = current_y;
    mouse_prev_state = state;
    first_draw = false;
}

int(draw_screen)()
{
    if (!video_mem)
        return 0;

    
    if (check_state != state) {
        int x_offset = (vmi.XResolution - pixmap_list[state].width) / 2;
        int y_offset = (vmi.YResolution - pixmap_list[state].height) / 2;

        for (int y = 0; y < pixmap_list[state].height; y++)
        {
            for (int x = 0; x < pixmap_list[state].width; x++)
            {
                uint32_t *pixel = video_mem + ((y + y_offset) * vmi.XResolution + (x + x_offset));
                uint32_t *src_pixel = (uint32_t*) pixmap_list[state].bytes + (y * pixmap_list[state].width + x);
                
                *pixel = *src_pixel;
                
                
            }
        }
        if (state >= 2 && state <= 6) drawBet();
        if (state >= 7 && state <= 9) draw_playing();
        if (state >= 2) drawBalance();
        if (state >= 10) draw_result();
    }
    
    if (state >= 7 && state <= 9 && check_p_cards != player_c_count) draw_playing();
    if (state >= 2 && check_money != money) drawBalance();
    if (state >= 2 && state <= 6 && check_bet != bet) drawBet();
    
    draw_mouse();
    check_state = state;

    return 1;
}


xpm_image_t img;


void (draw_playing)() {
    int y_player = 490;
    int y_dealer = 145;
    for (int i = 0; i < player_c_count; i++) {
        draw_card(player_cards[i], i, y_player);
    }

    check_p_cards = player_c_count;
    draw_card(dealer_cards[0], 0, y_dealer);
    draw_card(52, 1, y_dealer);
    drawScores(0);

}

void (draw_result)() {
    int y_player = 490;
    int y_dealer = 145;
    for (int i = 0; i < player_c_count; i++) {
        draw_card(player_cards[i], i, y_player);
    }
    for (int i = 0; i < dealer_c_count; i++) {
        draw_card(dealer_cards[i], i, y_dealer);
    }
    
    drawScores(1);


}

void (draw_card)(int index, int pos, int y_offset) {
    int x_offset = 481;
    int move = 47;
    for (int y = 0; y < cards_list[index].height; y++) {
        for (int x = 0; x < cards_list[index].width; x++) {
            uint32_t *pixel = video_mem + ((y + y_offset) * vmi.XResolution + (x + x_offset + move * pos));
            uint32_t *src_pixel = (uint32_t*) cards_list[index].bytes + (y * cards_list[index].width + x);

            *pixel = *src_pixel;
        }
    }
   
}

void (drawNumbers)(int number, int pos, int x_offset, int y_offset) {
    int move = 25;
    for (int y = 0; y < numbers[number].height; y++)
    {
        for (int x = 0; x < numbers[number].width ; x++)
        {
            uint32_t *pixel = video_mem + ((y + y_offset) * vmi.XResolution + (x + x_offset - move * pos));
            uint32_t *src_pixel = (uint32_t*)  numbers[number].bytes + (y *  numbers[number].width + x);
            *pixel = *src_pixel;
        }
    }
}

void (drawBalance)() {
    int tmp = money;
    int check = 1;
    check_money = money;
    int cur;
    int x_offset = 300;
    int y_offset = 690;
    int pos = 0;
    if (money == 0) {
        drawNumbers(0,pos,x_offset,y_offset);
    }
    while (check != 0) {
        cur = tmp%10;
        check = tmp;
        tmp /= 10;
        drawNumbers(cur,pos,x_offset,y_offset);
        pos++;
    }
}


void (drawBet)() {
    int tmp = bet;
    check_bet = bet;
    int cur;
    int x_offset = 580;
    int y_offset = 525;
    int pos = 0;
    if (bet == 0) {
        drawNumbers(0,pos,x_offset,y_offset);
    }
    while (tmp != 0) {
        cur = tmp%10;
        tmp /= 10;
        drawNumbers(cur,pos,x_offset,y_offset);
        pos++;
    }
}

void (drawScores)(int over) {
    int tmp = score[0];
    check_bet = bet;
    int cur;
    int x_offset = 450;
    int y_offset = 480;
    int pos = 0;
    while (tmp != 0) {
        cur = tmp%10;
        tmp /= 10;
        drawNumbers(cur,pos,x_offset,y_offset);
        pos++;
    }
    tmp = over == 0 ? fakeScore : score[1];
    y_offset = 145;
    pos = 0;
    while (tmp != 0) {
        cur = tmp%10;
        tmp /= 10;
        drawNumbers(cur,pos,x_offset,y_offset);
        pos++;
    }
}
