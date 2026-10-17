#include <lcom/lcf.h>
#include "mouse.h"
#include "keyboard.h"

struct packet pp;
uint8_t count = 0;
int mouse_x = 176; 
int mouse_y = 132;

void select_start() { state = bet5; }
void select_exit() { exit_request = true; }
void select_a_bet() { change_bet(); }
void select_BET() { game_start(); }
void select_stand() { game_player_stand(); }
void select_hit() { game_player_hit(); }
void select_double() { game_player_double(); }
void select_reset() { state = MENU_START; }

Button menu_buttons[] = {
    {485, 443, 657, 488, MENU_START, select_start},
    {504, 530, 644, 574, MENU_EXIT, select_exit}
};

Button bet_buttons[] = {
    {420, 566, 482, 633, bet5, select_a_bet},
    {503, 566, 567, 633, bet10, select_a_bet},
    {582, 566, 646, 633, bet25, select_a_bet},
    {664, 566, 729, 633, bet50, select_a_bet},
    {518, 652, 630, 687, BET, select_BET}
};

Button play_buttons[] = {
    {383, 651, 498, 687, STAND, select_stand},
    {523, 651, 628, 687, HIT, select_hit},
    {651, 651, 770, 687, DOUBLE, select_double},    
};

Button dealerWin_buttons[] = {
    {410, 662, 556, 700, DEALER_BET, select_start},
    {590, 662, 724, 700, DEALER_EXIT, select_reset}
};

Button playerWin_buttons[] = {
    {410, 662, 556, 700, PLAYER_BET, select_start},
    {590, 662, 724, 700, PLAYER_EXIT, select_reset}
};

Button pushWin_buttons[] = {
    {410, 662, 556, 700, PUSH_BET, select_start},
    {590, 662, 724, 700, PUSH_EXIT, select_reset}
};

Button bjWin_buttons[] = {
    {410, 662, 556, 700, BJ_BET, select_start},
    {590, 662, 724, 700, BJ_EXIT, select_reset}
};


void checkPosition(int x, int y, bool leftclick);

void (mouse_ih)() {
    uint8_t status, out_buf;

    util_sys_inb(0x64, &status);
    if ((status & BIT(0)) && (status & BIT(5))) {

        util_sys_inb(0x60, &out_buf);
        if (count == 0 && !(out_buf & BIT(3))) return;
   
        pp.bytes[count] = out_buf;
        count++;
    
        if (count == 3) {
            count = 0;
            pp.lb = pp.bytes[0] & BIT(0);
            pp.rb = pp.bytes[0] & BIT(1);
            pp.mb = pp.bytes[0] & BIT(2);
            pp.x_ov = pp.bytes[0] & BIT(6);
            pp.y_ov = pp.bytes[0] & BIT(7);

            if  (pp.bytes[0] & BIT(4)) { // Movimento X negativo
                pp.delta_x = pp.bytes[1] | 0xFF00;
            }
            else {
                pp.delta_x = pp.bytes[1];
            }
            if (pp.bytes[0] & BIT(5)) { //Movimento Y negativo
                pp.delta_y = pp.bytes[2] | 0xFF00;
            }
            else {
                pp.delta_y = pp.bytes[2];
            }

            mouse_x += pp.delta_x;
            mouse_y -= pp.delta_y;

            mouse_x = (mouse_x < 176) ? 176: (mouse_x >= 953 ) ? 953 : mouse_x;
            mouse_y = (mouse_y < 132) ? 132 : (mouse_y >= 700) ? 700 : mouse_y;

            checkPosition(mouse_x, mouse_y, pp.lb);

        }
    }
}


void checkPosition(int x, int y, bool leftclick) {

    ////printf("\n--- CHECKING POSITION ---\n");
    //if (pp.lb == 1) //printf("X: %d, Y: %d, LEFT CLICK: LB=%d\n", x, y ,pp.lb);

    Button *buttons = NULL;
    int count = 0;
    
    switch (state) {
        case MENU_START:
        case MENU_EXIT:
            buttons = menu_buttons;
            count = sizeof(menu_buttons) / sizeof(Button);
            break;
        case bet5:
        case bet10:
        case bet25:
        case bet50:
        case BET:
            buttons = bet_buttons;
            count = sizeof(bet_buttons) / sizeof(Button);
            break;
        case STAND:
        case HIT:
        case DOUBLE:
            buttons = play_buttons;
            count = sizeof(play_buttons) / sizeof(Button);
            break;
        case DEALER_BET:
        case DEALER_EXIT:
            buttons = dealerWin_buttons;
            count = sizeof(dealerWin_buttons) / sizeof(Button);
            break;
        case PLAYER_BET:
        case PLAYER_EXIT:
            buttons = playerWin_buttons;
            count = sizeof(playerWin_buttons) / sizeof(Button);
            break; 
        case PUSH_BET:
        case PUSH_EXIT:
            buttons = pushWin_buttons;
            count = sizeof(pushWin_buttons) / sizeof(Button);
            break; 
        case BJ_BET:
        case BJ_EXIT:
            buttons = bjWin_buttons;
            count = sizeof(bjWin_buttons) / sizeof(Button);
            break;
        default:
            break;
    }

    for (int i = 0; i < count; i++) {
        //printf("State was : %d, count = %d, i = %d\n", state, count, i);

        if ((x >= buttons[i].x1 && x <= buttons[i].x2) && (y >= buttons[i].y1 && y <= buttons[i].y2)) {
            state = buttons[i].state;
            if (leftclick) buttons[i].onClick();
        }
    }

}






int mouse_init() {
    uint8_t response;
    
    if (sys_outb(0x64, 0xA8) != 0) {
        //printf("Error enabling mouse device\n");
        return 1;
    }
    
    if (sys_outb(0x64, 0xD4) != 0) return 1;
    if (sys_outb(0x60, 0xEA) != 0) return 1;  //Stream Mode
    if (util_sys_inb(0x60, &response) != 0 || response != 0xFA) {
        //printf("Mouse did not acknowledge Set Stream Mode\n");
        return 1;
    }
    
    if (sys_outb(0x64, 0xD4) != 0) return 1;
    if (sys_outb(0x60, 0xF4) != 0) return 1;  // Enable Data Reporting
    if (util_sys_inb(0x60, &response) != 0 || response != 0xFA) {
        //printf("Mouse did not acknowledge Enable Data Reporting\n");
        return 1;
    }
    
    count = 0;
    return 0;
}
