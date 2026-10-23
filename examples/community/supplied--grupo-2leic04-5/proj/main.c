#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#include "include/game_state.h"
#include "include/player.h"
#include "include/renderer.h"
#include "include/bullet.h"
#include "include/enemy.h"
#include "include/level_config.h"
#include "../lab1/rtc.h"
#include "../lab2/timer.h"
#include "../lab3/kbc.h"
#include "../lab4/mouse.h"

// frequência alvo do loop
#define FPS 60u

void renderer_set_victory_date(const rtc_date *date);

int(proj_main_loop)(int argc, char *argv[]) {
    (void)argc; (void)argv;

    if (renderer_init() != 0) return 1;

    game_state_init();
    renderer_draw();

    Enemy enemies[MAX_ENEMIES];
    LevelRuntime level_runtime;
    level_start(&level_runtime, enemies, MAX_ENEMIES, 0);

    // subscreve timer IRQ0
    uint8_t  timer_bit;
    if (timer_subscribe_int(&timer_bit) != 0) {
        renderer_cleanup();
        return 1;
    }
    uint32_t timer_irq = BIT(timer_bit);

    // subscreve teclado IRQ1 
    uint8_t  kbd_bit;
    if (kbd_subscribe_int(&kbd_bit) != 0) {
        timer_unsubscribe_int();
        renderer_cleanup();
        return 1;
    }
    uint32_t kbd_irq = BIT(kbd_bit);

    // subscreve rato IRQ12
    if (my_mouse_enable_data_reporting() != 0) {
        kbd_unsubscribe_int(); timer_unsubscribe_int(); renderer_cleanup(); return 1;
    }
    uint8_t mouse_bit;
    if (mouse_subscribe_int(&mouse_bit) != 0) {
        my_mouse_disable_data_reporting();
        kbd_unsubscribe_int(); timer_unsubscribe_int(); renderer_cleanup(); return 1;
    }
    uint32_t mouse_irq = BIT(mouse_bit);

    uint32_t hz = (uint32_t) sys_hz();

    // quantos ticks de timer por frame de jogo, garante pelo menos 1 para evitar divisão por zero
    uint32_t ticks_per_frame = hz / FPS;
    if (ticks_per_frame == 0) ticks_per_frame = 1;

    uint32_t tick_count = 0; // contador que reinicia em cada frame
    bool needs_redraw = true; // desenha o primeiro frame sempre

    int ipc_status = 0;
    message msg;

    while (game_state_get() != STATE_EXIT) {

        if (driver_receive(ANY, &msg, &ipc_status) != 0) {
            printf("driver_receive failed\n");
            continue;
        }

        if (!is_ipc_notify(ipc_status)) continue;
        if (_ENDPOINT_P(msg.m_source) != HARDWARE) continue;

        uint32_t interrupts = msg.m_notify.interrupts;

        // teclado
        if (interrupts & kbd_irq) {
            kbc_ih();
            if (!kbc_has_error()) {
                uint8_t scancode = kbc_get_scancode();
                game_state_handle_scancode(scancode);
                if (game_state_is_level()) {
                    scancode_process(scancode);
                }
                needs_redraw = true; // qualquer tecla pode mudar o estado
            }
        }

        // Se entrámos num nível diferente ou foi feito restart, reinicializa os inimigos
        if (game_state_take_level_reset()) {
            int cur_level = game_state_current_level();
            if (cur_level != 0) {
                level_start(&level_runtime, enemies, MAX_ENEMIES, cur_level);
            }
        }

        // timer
        if (interrupts & timer_irq) {
            timer_int_handler();
            tick_count++;

            if (tick_count >= ticks_per_frame) {
                tick_count = 0;

                if (game_state_is_level()) {
                    Player *p = player_get();
                    bullet_update();
                    level_update(&level_runtime, enemies, MAX_ENEMIES);
                    update_enemies(enemies, MAX_ENEMIES, p);
                    player_update();

                    // verifica se o nivel foi completado (so se ainda estivermos num nivel,
                    // update_enemies pode ter chamado game_state_trigger_game_over)
                    if (game_state_is_level() && level_is_complete(&level_runtime, enemies, MAX_ENEMIES)) {
                        rtc_date victory_date;
                        if (rtc_read_date(&victory_date) == 0) {
                            renderer_set_victory_date(&victory_date);
                        }
                        game_state_level_complete();
                    }

                    needs_redraw = true; // niveis animam sempre
                }
            }
        }

        // desenha sempre que necessário, independente do timer
        if (needs_redraw) {
            renderer_draw();
            needs_redraw = false;
        }
        
        // rato
        if (interrupts & mouse_irq) {
            mouse_ih();
            if (mouse_packet_ready()) {
                struct packet pp;
                mouse_parse_packet(&pp);

                // atualiza direçao vertical do jogador e estado do botao esquerdo do rato
                if (game_state_is_level()) {
                    player_update_facing_vertical(pp.delta_y);
                    player_set_mouse_left_button(pp.lb);
                }
                
                mouse_clear_packet_flag();
            }
        }
    }

    kbd_unsubscribe_int();
    timer_unsubscribe_int();
    mouse_unsubscribe_int();
    my_mouse_disable_data_reporting();
    renderer_cleanup();
    return 0;
}

int main(int argc, char *argv[]) {
    lcf_set_language("EN-US");
    lcf_trace_calls("/home/lcom/proj/trace.txt");
    lcf_log_output("/home/lcom/proj/output.txt");

    if (lcf_start(argc, argv)) return 1;
    lcf_cleanup();
    return 0;
}
