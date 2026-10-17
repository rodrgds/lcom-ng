#include <lcom/lcf.h>
#include <stdint.h>
#include <minix/sysutil.h>

#include "game.h"
#include "video_gr.h"
#include "videocardVars.h"
#include "sprite.h"
#include "keyboard.h"
#include "mouse.h"
#include "state_machine.h"
#include "uart.h"
#include "i8042.h"
#include "i8254.h"
#include "rtcVars.h"
#include "gameVar.h"
#include "uartVars.h"

uint8_t kbc_data,/**< @brief byte that contain keyboard readed data */
 mouse_data; /**< @brief byte that contain mouse readed data */

/**
 * @brief saves the state of the screen
 * 
 */
screen_state_t screen_state = MENU;

/**
 * @brief seed of or rand function
 * 
 */
static uint32_t seed = 1;

//game variavels
/**
 * @brief score counter of player 1
 * 
 */
long long unsigned int score = 0; 
/**
 * @brief clock counter of player 1
 * 
 */
int clock_counter = CLOCKSTART;

//multiplayer variavels
/**
 * @brief score counter of player 2
 */
long long unsigned int score2 = 0;
 /**
 * @brief clock counter of player 2
 */
int clock_counter2 = CLOCKSTART;

 /**
  * @brief true if some machine is logged in the multiplayer
  */
bool playerIn = false;

/**
 *  @brief true if multiplayer in one machine have been chosen
 */
bool one_machine_multiplayer = false;

//leaderboard variavels
/**
 * @brief array that contain all the info for the leaderboard
 * 
 */
struct Point points[5];
/**
 * @brief if true in this singleplayer run score have already been added
 * 
 */
bool run_score_add = false; 

//rtc variavels
/**
 * @brief struct that contain date info to be printed
 * 
 */
Date_t date;

//uart variavels
/**
 * @brief if true Transmitter Holding Register is empty
 * 
 */
static bool valid_send_data = true;


void game_init(){
    vg_init(MODE5);
}

void game_run(){
    if(subs_interrupts() != 0){
        printf("Error while subscribing\n");
        return;
    }

    // LOAD ALL SPRITES
    if(load_sprites() != 0){
        printf("Error while loading sprites!\n");
        return;
    }

    //interrupt variavels
    int ipc_status;
    message msg;
    uint32_t kbc_irq_set = BIT(KBC_IRQ),
        timer_irq_set = BIT(TIMER0_IRQ),
        mouse_irq_set = BIT(MOUSE_IRQ),
        rtc_irq_set = BIT(RTC_IRQ),
        uart1_irq_set = BIT(UART_IRQ1);

    //kbc variavels
    unsigned size;
    uint8_t kbc_bytes[2];

    //timer variavels
    int time_elapsed = 0;

    //mouse variavels
    int counter = 0;
    uint8_t mouse_bytes[3]; 


    while(screen_state != EXIT || counter != 0 ) { // condition: ate encontrar esc __ counter != 0 make sure that any mouse inturreption stay behind
        int r;
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { // received notification 
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: // hardware interrupt notification 	
                    if (msg.m_notify.interrupts & mouse_irq_set) { // MOUSE INTERRUPT 
                        mouse_handler(mouse_bytes,&counter);
                    }	
                    if (msg.m_notify.interrupts & kbc_irq_set) { // KEYBOARD INTERRUPT 
                        keyboard_handler(kbc_bytes, &size);    
                    }
                    if (msg.m_notify.interrupts & timer_irq_set){ // MOUSE INTERRUPT 
                        if(timer_handler(&date,&time_elapsed) != 0){
                            printf("error in timer interrupt\n");
                            return;
                        }
                    }
                    if(msg.m_notify.interrupts & uart1_irq_set){
                        uart_handler();
                    }
                    if(msg.m_notify.interrupts & rtc_irq_set){
                        rtc_handler(&date);
                    }


                    break;
                default:
                    break; // no other notifications expected: do nothing 
            }
        } 
        else { // received a standard message, not a notification 
            // no standard messages expected: do nothing 
        }
    }

    if(unsubs_interrupts() != 0){
        printf("Error while unsubscribing\n");
        return;
    }
}

void game_end(){
    destroy_sprites();
    free_buffer_mem();
    vg_exit();
    printf("HOPE YOU LIKE THE GAME :)\n\n\nMADE BY:\nANDRE PEREIRA & MATILDE OLIVEIRA\n\n");
}

int subs_interrupts(){
    uint8_t kbc_bit_no = KBC_IRQ, timer_bit_no = TIMER0_IRQ, mouse_bit_no = MOUSE_IRQ ,rtc_bit_no = RTC_IRQ, uart1_bit_no = UART_IRQ1;
    if (mouse_enable_data_reporting_me() != 0){
        printf("Mouse enable data report error!");
        return 1;
    }
    if (mouse_subscribe_int(&mouse_bit_no) != 0){
        printf("Mouse Subscribe error!");
        return 1;
    }
    if(enable_periodic_interrupts() != 0){
        uint8_t regC;
        read_registerC(&regC);
        printf("Mouse enable periodic interrupts error!");
        return 1;
    }
    if (rtc_subscribe_int(&rtc_bit_no) != 0){
        printf("Rtc Subscribe error!");
        return 1;
    }
    if (config_uart1() != 0){
        printf("Uart1 config error!");
        return 1;
    }
    if (uart_subscribe_int(&uart1_bit_no) != 0){
        printf("Uart1 Subscribe error!");
        return 1;
    }
    if (kbc_subscribe_int(&kbc_bit_no) != 0){
        printf("Kbc subscribe error!");
        return 1;
    }
    if (timer_subscribe_int(&timer_bit_no) != 0){
        printf("Timer subscribe error!");
        return 1;
    }
    return 0;
}

int unsubs_interrupts(){
    if (mouse_unsubscribe_int() != 0){
        printf("Mouse Unscribe error!");
        //return 1;
    }
    if (mouse_disable_data_reporting() != 0){
        printf("Mouse disabel data report error!");
        //return 1;
    }
    disable_periodic_interrupts();
    if (rtc_unsubscribe_int() != 0){
        printf("Rtc disabel data report error!");
        //return 1;
    }
    if (uart_unsubscribe_int() != 0){
        printf("uart1 disabel data report error!");
        //return 1;
    }
    if (kbc_unsubscribe_int() != 0){
        printf("kbc unsubscribe error!");
        //return 1;
    }
    if (timer_unsubscribe_int() != 0){
        printf("timer unsubscribe error!");
        //return 1;
    }
    return 0;
}

int keyboard_handler(uint8_t bytes[], unsigned * size){
    *size = 0;
    bool make;
    kbc_ih();
    bytes[*size] = kbc_data;
    *size = 1;
    while (kbc_data == 0xE0){
        *size = 2;
        kbc_ih();
        bytes[(*size) - 1] = kbc_data;
    }
    if((kbc_data & BREAK_CODE) == 0){
        make = true;
    }
    else make = false;

    kbc_interrupt_validation(*size,bytes);

    return 0;
    
}

int mouse_handler(uint8_t mouse_bytes[], int * counter){
    struct packet pp;
    mouse_ih();
    mouse_bytes[(*counter)] = mouse_data;
    if((!(ALWAYS_1 & mouse_data) && (*counter) == 0))
        printf("Mouse_ih failed. Retring..\n");
    else (*counter) ++;
    
    if((*counter) == N_BYTES_TO_READ){
        get_packet(&pp, mouse_bytes);
        (*counter) = 0;
        mouse_event_type event;
        mouse_get_event_type(&pp,&event);
        mouse_event_handler(&pp,event);
    }
    return 0;
}

int timer_handler(Date_t * date,int *time_elapsed){
    (*time_elapsed)++;

    if(screen_state == GAME){
        handle_game_changes(&score,&clock_counter);
        draw_game_screen(clock_counter,score);
        if(*time_elapsed % 60 == 0){
            clock_counter--;
            if(clock_counter <= 0){
                screen_state = SCORE;
                add_score();
            }
        }
    }
    else if(screen_state == MENU){
        handle_game_changes(&score,&clock_counter);
        handle_menu_screen(date);
    }
    else if(screen_state == SCORE){
        draw_score_screen(score);
    }
    else if(screen_state == LEADERBOARD){
        draw_leaderboard_screen(points);
    }
    else if(screen_state == MULTIPLAYER){
        handle_multiplayer_changes(&score,&clock_counter,&score2,&clock_counter2);
        draw_multiplayer_game_screen(clock_counter,score,clock_counter2,score2);
        if(*time_elapsed % 60 == 0){
            clock_counter--;
            clock_counter2--;
            if(clock_counter <= 0 && clock_counter2 <= 0){
                screen_state = MULTI_SCORE;
                playerIn = false;
            }
        }
    }
    else if(screen_state == MULTI_SCORE){
        draw_multi_score_screen(score,score2);
    }
    else if(screen_state == LOADING){
        draw_loading_screen();
    }

    return 0;
}

int rtc_handler(Date_t * date){
    uint8_t regC;
    read_registerC(&regC);
    if(screen_state == MENU && valid_read_date() && (regC & RTC_P_INT_PEND_C)){
        read_date(date);
    }
    return 0;
}

int uart_handler(){
    uint8_t data = 0;
    key_event event;
    uint8_t reg_IIR;

    if (util_sys_inb(COM1_REG + IIR_REG, &reg_IIR) != 0){
        printf("Error while reading identification reg\n");
        return 1;
    }

    if(!(reg_IIR & IIR_NO_INTERRUPT)){
        if(((reg_IIR & (IIR_P_INT_RLS)) == (IIR_P_INT_RLS))){
            printf("parity error!\n");
            read_RBR_byte(&data);
        }
        else if(((reg_IIR & (IIR_P_R_DATA)) == (IIR_P_R_DATA))){
            if(check_LSR_byte_received()){
                read_RBR_byte(&data);
            }
            if(data == UART_LOAD){
                if(playerIn){
                    reset_game(true);
                    screen_state = MULTIPLAYER;
                }
                else{
                    playerIn = true;
                }
            }
            else if(data == UART_EXIT){
                if(screen_state == MULTIPLAYER && !one_machine_multiplayer){
                    screen_state = MULTI_SCORE;
                }
                playerIn = false;
            }
            else if(data == UART_SEED){
                bool received = false;
                while(!received){
                    if(check_LSR_byte_received()){
                        read_RBR_byte(&data);
                        received = true;
                    }
                }
                set_seed(data);
            }
            else{
                key_byte_get_event(data,&event);
                kbc_event_handler(event);
            }

        }
        else if(((reg_IIR & (IIR_P_INT_TIME)) == (IIR_P_INT_TIME))){
            return 1;
        }
        else if(((reg_IIR & (IIR_P_INT_THR)) == (IIR_P_INT_THR))){
            valid_send_data = true;
        }
    }
    return 0;
}

int reset_game(bool useSeed){
    clock_counter = CLOCKSTART;
    clock_counter2 = CLOCKSTART;
    score = 0;
    score2 = 0;
    run_score_add = false;
    if(useSeed){
        reset_sprites(true);
    }
    else{
        reset_sprites(false);
    }
    return 0;
}

void add_score(){
    if(!run_score_add){
        struct Point curr_point;
        curr_point.date = date;
        curr_point.score = score;
        for (int i = 0; i < 5; i++){
            if(points[i].score < curr_point.score){
               struct Point temp_point = points[i];
               points[i] = curr_point;
               curr_point = temp_point;
            }            
        }
        run_score_add = true;
    }
}

void send_keys_byte(uint8_t val){
    if(valid_send_data || check_LSR_THR_empty()){ // send
        send_THR_byte(val);
        valid_send_data = false;
    }
}

//imported code from stack overflow
uint32_t myRand (void) {
    const static uint32_t a = 214013U;
    const static uint32_t c = 2531011U;
    // m is, of course, 2^32 since the values will wrap.

    seed = seed * a + c;
    return (seed >> 16) & 0x7FFF;
}

void set_seed(uint8_t seed_val){
    seed = seed_val;
}

uint32_t get_seed(){
    return seed;
}
