// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// Any header files included below this line should have been created by you
#include "keyboard.h"
#include "timer.h"
#include "mouse.h"
#include "drawings.h"
#include "game.h"
//#include "sprite.h"
#include "rtc.h"
extern uint8_t byte;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}
/*
static int print_usage() {
  printf("Usage: <mode - hex>\n");
  return 1;
}*/

int(proj_main_loop)(int argc, char *argv[]) {
    void* graph_address = vg_init(0x14c);
    if(graph_address == NULL) return 1;
    uint8_t* adrs = graph_address;
    uint8_t* double_buffer = (uint8_t*) malloc(1152*864*4);
    xpm_map_t leverArray[5] = {
            lever1,
            lever2,
            lever3,
            lever4,
            lever5
    };
    xpm_map_t heartArray[] = {
            redHrt,
            blueHrt,
            greenHrt,
            yellowHrt
    };
    xpm_map_t timeArray[11] = {
            zero,
            um,
            two,
            tres,
            four,
            five,
            six,
            sete,
            oito,
            nove,
            del
    };
    xpm_map_t cursor = cursorXPM;
    Cursor* mouseSprt = create_sprite(cursor, 400, 400, 0, 0);
    xpm_image_t bkgrdSprite_info;
    xpm_image_t menuBkgrdInfo;
    uint8_t* bkgrdSprite = xpm_load(bkgrd, XPM_DIRECT, &bkgrdSprite_info);
    uint8_t * menuBkgrdSpr = xpm_load(menuBkgrd, XPM_DIRECT, &menuBkgrdInfo);
    if(draw_background(menuBkgrd, adrs)){
        vg_exit();
        return 1;
    }
    if(draw_menu_options(startSel, helpNorm, exitNorm, adrs)){
        vg_exit();
        return 1;
    }
    if(drawRTC(menuBkgrdSpr, menuBkgrdInfo, timeArray, adrs)){
        vg_exit();
        return 1;
    }
    message msg;
    uint8_t bit_no, r;
    uint32_t  irq_set, irq_set_timer, irq_set_mouse;
    if (keyboard_subscribe_int(&bit_no)) {
        vg_exit();
        return 1;
    }
    irq_set = BIT(bit_no);
    if(timer_subscribe_int(&bit_no)){
        vg_exit();
        return 1;
    }
    irq_set_timer = BIT(bit_no);
    if (write_mouse_cmd(MOUSE_ENABLE)) return 1;
    if(mouse_subscribe_int(&bit_no)) {
        vg_exit();
        return 1;
    }
    irq_set_mouse = BIT(bit_no);

    uint32_t intCounter = 0, auxCounter = 0;
    uint16_t chosenTime = 10*60;
    int aux, ipc_status;
    uint8_t size = 0, bytes[2];
    bool condition = true, twoBSC = false, make;
    uint8_t arrowCount = 0;
    uint16_t elapsedTime = 0, frameCounter = 0;
    unsigned byteCounter = 0; // number of bytes read
    uint8_t packetBytes[3];
    uint8_t d1 = 0, d2 = 0;
    struct packet pp;
    uint64_t turns = 0;
    uint8_t finalpos = 0, deadPlrs = 0;
    uint32_t playercolour;
    bool diceThr = false, moveplayer = false, condition2 = true, inHelp, inGame, pause = false, youWon = false;
    while(condition) { /* You may want to use a different condition
                 Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */
                    if (msg.m_notify.interrupts & irq_set_timer) {
                        auxCounter++;
                        if(auxCounter % sys_hz() == 0){
                            if(drawRTC(menuBkgrdSpr, menuBkgrdInfo, timeArray, adrs)){
                                vg_exit();
                                return 1;
                            }
                        }
                    }
                    if (msg.m_notify.interrupts & irq_set_mouse) {
                        mouse_ih();
                        if (byteCounter == 0 && (byte & BYTE1_CHECK)) {
                            packetBytes[0] = byte;
                            byteCounter++;
                        }
                        else if (byteCounter == 1) {
                            packetBytes[1] = byte;
                            byteCounter++;
                        }
                        else if (byteCounter == 2) {
                            packetBytes[2] = byte;
                            byteCounter++;
                        }
                        if (byteCounter == 3){
                            byteCounter = 0;
                            continue;
                        }
                    }
                    if (msg.m_notify.interrupts & irq_set) { /* keyboard interrupt */
                        kbc_ih();
                        /*aux = scanCodeReader(&make, &size, bytes);
                        if (aux == -1) {
                            condition = false;
                        }*/
                        if(getScanCode() == TWO_BYTE_LSB) {
                            twoBSC = true;
                            continue;
                        }
                        if(((getScanCode() == 0x50) || (getScanCode() == 0x48))  && twoBSC) {
                            if(getScanCode() == 0x50){ //seta para baixo
                                if(arrowCount == 2) arrowCount = 0;
                                else {
                                    arrowCount +=1;
                                }
                            }
                            if(getScanCode() == 0x48) {
                                if(arrowCount == 0) arrowCount = 2;
                                else {
                                    arrowCount -=1;
                                }
                            }
                            switch (arrowCount){
                                case 0:
                                    draw_menu_options(startSel, helpNorm, exitNorm, adrs);
                                    break;
                                case 1:
                                    draw_menu_options(startNorm, helpSel, exitNorm, adrs);
                                    break;
                                case 2:
                                    draw_menu_options(startNorm, helpNorm, exitSel, adrs);
                                    break;
                            }
                        }
                        if(getScanCode() == 0x1c){
                            switch (arrowCount) {
                                case 0:
                                    if (draw_background(bkgrd, double_buffer)) {
                                        vg_exit();
                                        return 1;
                                    }
                                    if (draw_tabuleiro(whiteSquare, double_buffer, 704, 128)) {
                                        vg_exit();
                                        return 1;
                                    }
                                    deadPlrs = 0;
                                    elapsedTime = 0;
                                    arrowCount = 0;
                                    uint8_t colorinput = 0;
                                    youWon = false;
                                    switch(colorinput) {
                                        case 0:
                                            playercolour = RED;
                                            break;
                                        case 1:
                                            playercolour = BLUE;
                                            break;
                                        case 2:
                                            playercolour = GREEN;
                                            break;
                                        case 3:
                                            playercolour = YELLOW;
                                            break;
                                    }
                                    if (draw_start_players(redPlr, bluePlr, greenPlr, yellowPlr, double_buffer, colorinput)) {
                                        vg_exit();
                                        return 1;
                                    }
                                    if(drawHearts(heartArray, double_buffer)){
                                        vg_exit();
                                        return 1;
                                    }
                                    if(drawHP(bkgrdSprite, timeArray, double_buffer)){
                                        vg_exit();
                                        return 1;
                                    }
                                    if(drawTime(bkgrdSprite, bkgrdSprite_info, timeArray, chosenTime, elapsedTime, double_buffer)){
                                        vg_exit();
                                        return 1;
                                    }
                                    if(drawSlots(slotsbg, leverArray[0], double_buffer)){
                                        vg_exit();
                                        return 1;
                                    }
                                    inGame = true;
                                    while(inGame) { /* You may want to use a different condition Get a request message. */
                                        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
                                            printf("driver_receive failed with: %d", r);
                                            continue;
                                        }
                                        if (is_ipc_notify(ipc_status)) { /* received notification */
                                            switch (_ENDPOINT_P(msg.m_source)) {
                                                case HARDWARE: /* hardware interrupt notification */
                                                    if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
                                                        kbc_ih();
                                                        aux = scanCodeReader(&make, &size, bytes);
                                                        if (aux == -1) {
                                                            pause = true;
                                                            clear_image(4*400,4*250, 250, 200, adrs, bkgrdSprite);
                                                            draw_option(pauseM, adrs, 4*500, 4*250);
                                                            draw_option(resumeBtn,adrs, 4*500, 4*330);
                                                            draw_option(menuBtnOr, adrs, 4*500, 4*410);
                                                            twoBSC = false;
                                                            arrowCount = 0;
                                                            while(pause){
                                                                if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
                                                                    printf("driver_receive failed with: %d", r);
                                                                    continue;
                                                                }
                                                                if (is_ipc_notify(ipc_status)) { /* received notification */
                                                                    switch (_ENDPOINT_P(msg.m_source)) {
                                                                        case HARDWARE: /* hardware interrupt notification */
                                                                            if (msg.m_notify.interrupts & irq_set){
                                                                                kbc_ih();
                                                                                if(getScanCode() == TWO_BYTE_LSB) {
                                                                                    twoBSC = true;
                                                                                    continue;
                                                                                }
                                                                                if(((getScanCode() == 0x50) || (getScanCode() == 0x48))  && twoBSC) {
                                                                                    if(getScanCode() == 0x50){ //seta para baixo
                                                                                        if(arrowCount == 1) arrowCount = 0;
                                                                                        else {
                                                                                            arrowCount +=1;
                                                                                        }
                                                                                    }
                                                                                    if(getScanCode() == 0x48) {
                                                                                        if(arrowCount == 0) arrowCount = 1;
                                                                                        else {
                                                                                            arrowCount -=1;
                                                                                        }
                                                                                    }
                                                                                    switch (arrowCount){
                                                                                        case 0:
                                                                                            draw_option(resumeBtn,adrs, 4*500, 4*330);
                                                                                            draw_option(menuBtnOr, adrs, 4*500, 4*410);
                                                                                            break;
                                                                                        case 1:
                                                                                            draw_option(resumeBtnOr,adrs, 4*500, 4*330);
                                                                                            draw_option(menuBtn, adrs, 4*500, 4*410);
                                                                                            break;
                                                                                    }
                                                                                }
                                                                                if(getScanCode() == 0x1c){
                                                                                    switch (arrowCount) {
                                                                                        case 0:
                                                                                            pause = false;
                                                                                            arrowCount = 0;
                                                                                            //memcpy(adrs, double_buffer, 1152*864*4);
                                                                                            break;
                                                                                        case 1:
                                                                                            pause = false;
                                                                                            inGame = false;
                                                                                            arrowCount = 0;
                                                                                            break;
                                                                                    }
                                                                                }
                                                                            }

                                                                    }
                                                                }
                                                                if (msg.m_notify.interrupts & irq_set_mouse) {
                                                                    mouse_ih();
                                                                    if (byteCounter == 0 && (byte & BYTE1_CHECK)) {
                                                                        packetBytes[0] = byte;
                                                                        byteCounter++;
                                                                    }
                                                                    else if (byteCounter == 1) {
                                                                        packetBytes[1] = byte;
                                                                        byteCounter++;
                                                                    }
                                                                    else if (byteCounter == 2) {
                                                                        packetBytes[2] = byte;
                                                                        byteCounter++;
                                                                    }
                                                                    if (byteCounter == 3){
                                                                        byteCounter = 0;
                                                                        continue;
                                                                    }
                                                                }
                                                            }
                                                        }
                                                        if(inGame == false){
                                                            break;
                                                        }
                                                        if(getScanCode() == 0x20){
                                                            diceThr = true;
                                                        }
                                                    }
                                                    if (msg.m_notify.interrupts & irq_set_timer) {
                                                        intCounter++;
                                                        if ((intCounter % sys_hz()) == 0) {
                                                            elapsedTime++;
                                                            if(drawTime(bkgrdSprite, bkgrdSprite_info, timeArray, chosenTime, elapsedTime, double_buffer)){
                                                                inGame = false;
                                                            };
                                                        }
                                                        if (((intCounter % 7)== 0) && moveplayer) {
                                                            draw_player(&players[turns%4],double_buffer,players[turns%4].position+1);
                                                            if (players[turns%4].position == finalpos) {
                                                                moveplayer = false;
                                                                if (turns%4 == 0) {
                                                                    condition2 = board[(players[turns %4].position)].color != playercolour;
                                                                    if (condition2){
                                                                        if (board[(players[turns %4].position)].color == WHITE){
                                                                            if ((int)players[turns%4].hp - 10 <= 0){
                                                                                condition2 = false;
                                                                            }
                                                                        }
                                                                        else {
                                                                            if((int)players[turns%4].hp - 5 <= 0) {
                                                                                condition2 = false;
                                                                                players[turns%4].hp = 0;
                                                                                inGame = false;
                                                                                break;
                                                                            }
                                                                            else{
                                                                                players[turns%4].hp-=5;
                                                                            }
                                                                            if((int)players[turns%4].hp - 15 <= 0){
                                                                                condition2 = false;
                                                                            }
                                                                        }
                                                                    }
                                                                    else {
                                                                        if((int)players[turns%4].hp + 2 <= 100){
                                                                            players[turns%4].hp += 2;
                                                                        }
                                                                    }
                                                                    drawHP(bkgrdSprite, timeArray, double_buffer);
                                                                    if(condition2){
                                                                        draw_option(MenuBuy,double_buffer, 4*440, 4*250);
                                                                        draw_option(MenuYes, double_buffer, 4*350, 4*350);
                                                                        draw_option(MenuNo, double_buffer, 4*750, 4*350);
                                                                    }
                                                                    while (condition2) {
                                                                        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
                                                                            printf("driver_receive failed with: %d", r);
                                                                            continue;
                                                                        }
                                                                        if (is_ipc_notify(ipc_status)) { /* received notification */
                                                                            switch (_ENDPOINT_P(msg.m_source)) {
                                                                                case HARDWARE: /* hardware interrupt notification */
                                                                                    if (msg.m_notify.interrupts & irq_set) {
                                                                                        kbc_ih();
                                                                                        aux = scanCodeReader(&make, &size, bytes);
                                                                                        if (aux == -1) {
                                                                                            pause = true;
                                                                                            clear_image(4*300,4*250, 250, 550, adrs, bkgrdSprite);
                                                                                            draw_option(pauseM, adrs, 4*500, 4*250);
                                                                                            draw_option(resumeBtn,adrs, 4*500, 4*330);
                                                                                            draw_option(menuBtnOr, adrs, 4*500, 4*410);
                                                                                            twoBSC = false;
                                                                                            arrowCount = 0;
                                                                                            while(pause){
                                                                                                if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
                                                                                                    printf("driver_receive failed with: %d", r);
                                                                                                    continue;
                                                                                                }
                                                                                                if (is_ipc_notify(ipc_status)) { /* received notification */
                                                                                                    switch (_ENDPOINT_P(msg.m_source)) {
                                                                                                        case HARDWARE: /* hardware interrupt notification */
                                                                                                            if (msg.m_notify.interrupts & irq_set){
                                                                                                                kbc_ih();
                                                                                                                if(getScanCode() == TWO_BYTE_LSB) {
                                                                                                                    twoBSC = true;
                                                                                                                    continue;
                                                                                                                }
                                                                                                                if(((getScanCode() == 0x50) || (getScanCode() == 0x48))  && twoBSC) {
                                                                                                                    if(getScanCode() == 0x50){ //seta para baixo
                                                                                                                        if(arrowCount == 1) arrowCount = 0;
                                                                                                                        else {
                                                                                                                            arrowCount +=1;
                                                                                                                        }
                                                                                                                    }
                                                                                                                    if(getScanCode() == 0x48) {
                                                                                                                        if(arrowCount == 0) arrowCount = 1;
                                                                                                                        else {
                                                                                                                            arrowCount -=1;
                                                                                                                        }
                                                                                                                    }
                                                                                                                    switch (arrowCount){
                                                                                                                        case 0:
                                                                                                                            draw_option(resumeBtn,adrs, 4*500, 4*330);
                                                                                                                            draw_option(menuBtnOr, adrs, 4*500, 4*410);
                                                                                                                            break;
                                                                                                                        case 1:
                                                                                                                            draw_option(resumeBtnOr,adrs, 4*500, 4*330);
                                                                                                                            draw_option(menuBtn, adrs, 4*500, 4*410);
                                                                                                                            break;
                                                                                                                    }
                                                                                                                }
                                                                                                                if(getScanCode() == 0x1c){
                                                                                                                    switch (arrowCount) {
                                                                                                                        case 0:
                                                                                                                            pause = false;
                                                                                                                            arrowCount = 0;
                                                                                                                            break;
                                                                                                                        case 1:
                                                                                                                            pause = false;
                                                                                                                            condition2 = false;
                                                                                                                            inGame = false;
                                                                                                                            arrowCount = 0;
                                                                                                                            continue;
                                                                                                                            break;
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                    }
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                        if (getScanCode() == 0xB1) {
                                                                                            condition2 = false;
                                                                                        }
                                                                                        if (getScanCode() == 0x95) {
                                                                                            if(board[(players[turns % 4].position)].color == WHITE){
                                                                                                players[turns % 4].hp -= 10;
                                                                                            }
                                                                                            else{
                                                                                                players[turns % 4].hp -= 15;
                                                                                            }
                                                                                            switch (colorinput){
                                                                                                case 0:
                                                                                                    paint_square(redSquare, double_buffer, players[turns%4].position, RED);
                                                                                                    break;
                                                                                                case 1:
                                                                                                    paint_square(blueSquare, double_buffer, players[turns%4].position, BLUE);
                                                                                                    break;
                                                                                                case 2:
                                                                                                    paint_square(greenSquare, double_buffer, players[turns%4].position, GREEN);
                                                                                                    break;
                                                                                                case 3:
                                                                                                    paint_square(yellowSquare, double_buffer, players[turns%4].position, YELLOW);
                                                                                                    break;
                                                                                            }
                                                                                            condition2 = false;
                                                                                        }
                                                                                    }
                                                                                    if (msg.m_notify.interrupts & irq_set_mouse) {
                                                                                        mouse_ih();
                                                                                        if (byteCounter == 0 && (byte & BYTE1_CHECK)) {
                                                                                            packetBytes[0] = byte;
                                                                                            byteCounter++;
                                                                                        }
                                                                                        else if (byteCounter == 1) {
                                                                                            packetBytes[1] = byte;
                                                                                            byteCounter++;
                                                                                        }
                                                                                        else if (byteCounter == 2) {
                                                                                            packetBytes[2] = byte;
                                                                                            byteCounter++;
                                                                                        }
                                                                                        if (byteCounter == 3){
                                                                                            byteCounter = 0;
                                                                                            continue;
                                                                                        }
                                                                                    }
                                                                            }
                                                                            memcpy(adrs, double_buffer, 1152*864*4);
                                                                        }
                                                                    }
                                                                    clear_image(4*440, 4*250, 60, 315, double_buffer, bkgrdSprite);
                                                                    clear_image(4*350, 4*350, 60, 100, double_buffer, bkgrdSprite);
                                                                    clear_image(4*750, 4*350, 60, 100, double_buffer, bkgrdSprite);
                                                                    for(uint8_t s = 0; s < 4; s++){
                                                                        if(players[s].hp > 0){
                                                                            draw_player(&players[s],double_buffer,players[s].position);
                                                                        }
                                                                    }
                                                                }
                                                                else{
                                                                    condition2 = false;
                                                                    if(board[(players[turns %4].position)].color != players[turns%4].color){
                                                                        if(board[(players[turns %4].position)].color == WHITE){
                                                                            if(players[turns%4].hp > 20){
                                                                                condition2 = true;
                                                                                players[turns%4].hp-=10;
                                                                            }
                                                                            else{
                                                                                condition2 = false;
                                                                            }
                                                                        }
                                                                        else{
                                                                            if((int)players[turns%4].hp - 5 <= 0){
                                                                                players[turns%4].hp = 0;
                                                                                condition2 = false;
                                                                                killPlayer(players[turns%4], whiteSquare, double_buffer);
                                                                                deadPlrs++;
                                                                                if(deadPlrs == 3){
                                                                                    youWon = true;
                                                                                    inGame = false;
                                                                                    break;
                                                                                }
                                                                            }
                                                                            else{
                                                                                players[turns%4].hp-=5;
                                                                                if(players[turns%4].hp > 20){
                                                                                    condition2 = true;
                                                                                    players[turns%4].hp-=15;
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                    else{
                                                                        if((int)players[turns%4].hp + 2 <= 100){
                                                                            players[turns%4].hp += 2;
                                                                        }
                                                                    }
                                                                    if(condition2){
                                                                        switch (players[turns%4].color){
                                                                            case RED:
                                                                                paint_square(redSquare, double_buffer, players[turns%4].position, RED);
                                                                                break;
                                                                            case BLUE:
                                                                                paint_square(blueSquare, double_buffer, players[turns%4].position, BLUE);
                                                                                break;
                                                                            case GREEN:
                                                                                paint_square(greenSquare, double_buffer, players[turns%4].position, GREEN);
                                                                                break;
                                                                            case YELLOW:
                                                                                paint_square(yellowSquare, double_buffer, players[turns%4].position, YELLOW);
                                                                                break;
                                                                        }
                                                                        for(uint8_t s = 0; s < 4; s++){
                                                                            if(players[s].hp > 0){
                                                                                draw_player(&players[s],double_buffer,players[s].position);
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                                if(drawHP(bkgrdSprite, timeArray, double_buffer)){
                                                                    vg_exit();
                                                                    return 1;
                                                                }
                                                                turns++;
                                                                while(players[turns%4].hp == 0){
                                                                    turns++;
                                                                }
                                                            }
                                                        }
                                                        if(((turns%4) != 0) && !moveplayer && inGame){
                                                            diceThr = true;
                                                        }
                                                        if(((intCounter % 7) == 0) && diceThr && inGame) {
                                                            frameCounter++;
                                                            if(frameCounter == 9){
                                                                d1 = RNG(1,6);
                                                                d2 = RNG(1,6);
                                                                finalpos = (players[turns%4].position+d1+d2)%28;
                                                                if(drawNum(timeArray[d1], bkgrdSprite, 4*510, 4*530+4*35, double_buffer)){
                                                                    return 1;
                                                                }
                                                                if(drawNum(timeArray[d2], bkgrdSprite, 4*510+4*55, 4*530+4*35, double_buffer)){
                                                                    return 1;
                                                                }
                                                                moveplayer = true;
                                                                diceThr = false;
                                                                frameCounter = 0;
                                                            }
                                                            else{
                                                                switch (frameCounter){
                                                                    case 0:
                                                                        if(throwDice(leverArray[0], bkgrdSprite, bkgrdSprite_info, double_buffer)) inGame = false;
                                                                        break;
                                                                    case 1:
                                                                        if(throwDice(leverArray[1], bkgrdSprite, bkgrdSprite_info, double_buffer)) inGame = false;
                                                                        break;
                                                                    case 2:
                                                                        if(throwDice(leverArray[2], bkgrdSprite, bkgrdSprite_info, double_buffer)) inGame = false;
                                                                        break;
                                                                    case 3:
                                                                        if(throwDice(leverArray[3], bkgrdSprite, bkgrdSprite_info, double_buffer)) inGame = false;
                                                                        break;
                                                                    case 4:
                                                                        if(throwDice(leverArray[4], bkgrdSprite, bkgrdSprite_info, double_buffer)) inGame = false;
                                                                        break;
                                                                    case 5:
                                                                        if(throwDice(leverArray[3], bkgrdSprite, bkgrdSprite_info, double_buffer)) inGame = false;
                                                                        break;
                                                                    case 6:
                                                                        if(throwDice(leverArray[2], bkgrdSprite, bkgrdSprite_info, double_buffer)) inGame = false;
                                                                        break;
                                                                    case 7:
                                                                        if(throwDice(leverArray[1], bkgrdSprite, bkgrdSprite_info, double_buffer)) inGame = false;
                                                                        break;
                                                                    case 8:
                                                                        if(throwDice(leverArray[0], bkgrdSprite, bkgrdSprite_info, double_buffer)) inGame = false;
                                                                        break;
                                                                }
                                                            }
                                                        }
                                                        memcpy(adrs, double_buffer, 1152*864*4);
                                                        if(elapsedTime == chosenTime) inGame = false;
                                                    }
                                                    if (msg.m_notify.interrupts & irq_set_mouse) {
                                                        mouse_ih();
                                                        if (byteCounter == 0 && (byte & BYTE1_CHECK)) {
                                                            packetBytes[0] = byte;
                                                            byteCounter++;
                                                        }
                                                        else if (byteCounter == 1) {
                                                            packetBytes[1] = byte;
                                                            byteCounter++;
                                                        }
                                                        else if (byteCounter == 2) {
                                                            packetBytes[2] = byte;
                                                            byteCounter++;
                                                        }
                                                        if (byteCounter == 3) {
                                                            packet_parser(&pp, packetBytes);
                                                            //mouse_print_packet(&pp);
                                                            mouseSprt->xspeed = pp.delta_x;
                                                            mouseSprt->yspeed = -pp.delta_y;
                                                            /*drawSlots(slotsbg, leverArray[0], double_buffer);
                                                            if(drawNum(timeArray[d1], bkgrdSprite, 4*510, 4*530+4*35, double_buffer)){
                                                                return 1;
                                                            }
                                                            if(drawNum(timeArray[d2], bkgrdSprite, 4*510+4*55, 4*530+4*35, double_buffer)){
                                                                return 1;
                                                            }*/
                                                            move_sprite(mouseSprt, double_buffer, bkgrdSprite);
                                                            drawTime(bkgrdSprite, bkgrdSprite_info, timeArray, chosenTime, elapsedTime, double_buffer);
                                                            byteCounter = 0;
                                                            memcpy(adrs, double_buffer, 1152*864*4);
                                                        }
                                                    }
                                            }
                                        }
                                    }
                                    if(draw_background(bkgrd, adrs)){
                                        vg_exit();
                                        return 1;
                                    }
                                    if(drawHearts(heartArray, adrs)){
                                        vg_exit();
                                        return 1;
                                    }
                                    if(drawHP(bkgrdSprite, timeArray, adrs)){
                                        vg_exit();
                                        return 1;
                                    }
                                    if(youWon){
                                        draw_option(Victory, adrs, 4*400, 4*400);
                                    }
                                    else {
                                        draw_option(GmOver, adrs, 4 * 400, 4 * 400);
                                    }
                                    sleep(3);
                                    turns = 0;
                                    if(draw_background(menuBkgrd, adrs)){
                                        vg_exit();
                                        return 1;
                                    }
                                    if(draw_menu_options(startSel, helpNorm, exitNorm, adrs)){
                                        vg_exit();
                                        return 1;
                                    }
                                    if(drawRTC(menuBkgrdSpr, menuBkgrdInfo, timeArray, adrs)){
                                        vg_exit();
                                        return 1;
                                    }
                                    break;
                                case 1:
                                    inHelp = true;
                                    arrowCount = 0;
                                    if(draw_background(menuHelp, adrs)){
                                        vg_exit();
                                        return 1;
                                    }
                                    while(inHelp) {
                                        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
                                            printf("driver_receive failed with: %d", r);
                                            continue;
                                        }
                                        if (is_ipc_notify(ipc_status)) { /* received notification */
                                            switch (_ENDPOINT_P(msg.m_source)) {
                                                case HARDWARE:
                                                    if (msg.m_notify.interrupts & irq_set) { /* keyboard interrupt */
                                                        kbc_ih();
                                                        aux = scanCodeReader(&make, &size, bytes);
                                                        if (aux == -1) {
                                                            inHelp = false;
                                                        }
                                                    }
                                                    if (msg.m_notify.interrupts & irq_set_mouse) {
                                                        mouse_ih();
                                                        if (byteCounter == 0 && (byte & BYTE1_CHECK)) {
                                                            packetBytes[0] = byte;
                                                            byteCounter++;
                                                        }
                                                        else if (byteCounter == 1) {
                                                            packetBytes[1] = byte;
                                                            byteCounter++;
                                                        }
                                                        else if (byteCounter == 2) {
                                                            packetBytes[2] = byte;
                                                            byteCounter++;
                                                        }
                                                        if (byteCounter == 3){
                                                            byteCounter = 0;
                                                            continue;
                                                        }
                                                    }
                                            }
                                        }
                                    }
                                    if(draw_background(menuBkgrd, adrs)){
                                        vg_exit();
                                        return 1;
                                    }
                                    if(draw_menu_options(startSel, helpNorm, exitNorm, adrs)){
                                        vg_exit();
                                        return 1;
                                    }
                                    break;
                                case 2:
                                    condition = false;
                                    break;
                            }
                            break;
                        }
                        if(getScanCode() != TWO_BYTE_LSB){
                            twoBSC = false;
                        }
                    }
                default:
                    break; /* no other notifications expected: do nothing */
            }
        }
        else { /* received a standard message, not a notification
               no standard messages expected: do nothing */
        }
    }
    if(write_mouse_cmd(MOUSE_DISABLE)) return 1;
    if(keyboard_unsubscribe_int()) return 1;
    if(mouse_unsubscribe_int()) return 1;
    if(timer_unsubscribe_int())return 1;
    if (vg_exit()) return 1;
  return 0;
}
