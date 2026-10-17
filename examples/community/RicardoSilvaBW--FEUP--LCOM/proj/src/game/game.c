#include <lcom/lcf.h>
#include <lcom/utils.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "game.h"
#include "assets_loader.h"

#include "../config.h"
#include "../controller/keyboard-mouse/kbc.h"
#include "../controller/keyboard-mouse/mouse.h"
#include "../controller/keyboard-mouse/i8042.h"
#include "../controller/keyboard-mouse/input.h"
#include "../controller/timer/i8254.h"
#include "../controller/video/graphics.h"
#include "../model/note.h"
#include "../model/sprite.h"
#include "../view/renderer.h"

extern uint32_t counter;
extern uint8_t scancode;

game_mode_t current_mode = MODE_HARDCORE;
double current_note_speed = BASE_SPEED;

sprite laneSprites[NUM_LANES];
note notes[NUM_LANES];

sprite *target_sprites[NUM_LANES];
sprite *lit_arrow_sprites[NUM_LANES];
sprite *clock_sprite = NULL;
sprite *score_sprite = NULL;
sprite *background_sprite = NULL;
sprite *idle_cheerleader_sprite = NULL;
sprite *cheerleader_poses[NUM_LANES];
sprite *cheerleader_hc_pose_sprite = NULL;
sprite *cursor_sprite = NULL;
sprite *heart_sprite = NULL;
sprite *heart_hit_sprite = NULL;

int remaining_secs = GAME_DURATION_SECS;

int score = 0;

int current_cheer_pose = -1;
int cheer_pose_timer = 0;

int notes_since_heart = 0;

int lane_x[NUM_LANES];

static void (update_notes)() {
    for (int i = 0; i < NUM_LANES; i++) {
        note_update(&notes[i]);
        if (notes[i].active && notes[i].sp->y + notes[i].sp->height < 0) {
            note_reset_random(&notes[i], get_v_res(), NOTE_SPAWN_MAX_DELAY);
            assign_note_type(&notes[i]);
        }

        if (notes[i].missed){
            if (notes[i].type == HEART_NOTE) score -= 50;
            if (score < 0) score = 0;
            note_reset_random(&notes[i], get_v_res(), NOTE_SPAWN_MAX_DELAY);
            assign_note_type(&notes[i]);
        }

        if (!notes[i].active && notes[i].spawn_timer == 0 && !notes[i].missed) notes[i].active = true;
    }
}

static void (render_frame)() {
    clear_buffer();
    provide_static_background();

    if (current_cheer_pose == -1 && idle_cheerleader_sprite != NULL) draw_sprite(idle_cheerleader_sprite);
    else if (current_cheer_pose == HEART_CHEER_POSE && cheerleader_hc_pose_sprite != NULL) draw_sprite(cheerleader_hc_pose_sprite);
    else if (cheerleader_poses[current_cheer_pose] != NULL) draw_sprite(cheerleader_poses[current_cheer_pose]);
    
    draw_acceptance_zone(lane_x, target_sprites);
    for (int i = 0; i < NUM_LANES; i++) {
        if (notes[i].type == HEART_NOTE) {
            sprite* heart = (notes[i].hit) ? heart_hit_sprite : heart_sprite;
            if (heart != NULL) {
                heart->x = notes[i].sp->x;
                heart->y = notes[i].sp->y;
                
                draw_sprite(heart);
            }
        }
        else if (notes[i].hit && lit_arrow_sprites[i] != NULL) {
            lit_arrow_sprites[i]->x = notes[i].sp->x;
            lit_arrow_sprites[i]->y = notes[i].sp->y;
            
            draw_sprite(lit_arrow_sprites[i]);
        } 
        else draw_note(&notes[i]);
    }

    draw_time(clock_sprite, remaining_secs, 10, 10);
    
    if (cursor_sprite) {
        int mx = 0, my = 0;
        mouse_get_pos(&mx, &my);
        cursor_sprite->x = mx;
        cursor_sprite->y = my;
        draw_sprite(cursor_sprite);
    }

    draw_score(score_sprite, score, 10, 60);
    swap_buffer();
}

static void (process_lane_hit)(note_lane lane) {
    for (int i = 0; i < NUM_LANES; i++) {
        if (!notes[i].active) continue;
        if (notes[i].lane != lane) continue;
        if (notes[i].type == HEART_NOTE) return;

        if (note_is_hittable(&notes[i])) {
            current_cheer_pose = i;
            cheer_pose_timer = CHEERLEADER_POSE_DURATION_TICKS;
            score += 100;
            notes[i].hit = true;
        }
        
        break;
    }
}

static void (process_mouse_click)() {
    if (!(mouse_get_buttons() & 1)) return;
    int mouse_x, mouse_y;

    mouse_get_pos(&mouse_x, &mouse_y);

    for (int i = 0; i < NUM_LANES; i++) {
        if (!notes[i].active || notes[i].type != HEART_NOTE || notes[i].hit) continue;
        if (note_is_clickable(&notes[i], heart_sprite, mouse_x, mouse_y)) {
            score += 150;
            current_cheer_pose = HEART_CHEER_POSE;
            cheer_pose_timer = CHEERLEADER_POSE_DURATION_TICKS;
            notes[i].hit = true;
        }
    }
}

int (game_run)(uint8_t kb_irq_set, uint8_t mouse_irq_set, uint8_t timer_irq_set) {
    int frequency = sys_hz();

    int ipc_status, r;
    message msg;

    remaining_secs = GAME_DURATION_SECS;
    score = 0;
    notes_since_heart = 0;

    initialize_notes();
    initialize_hud_assets();

    uint8_t* game_background = malloc(get_vram_size());
    draw_xpm(background_sprite->img, background_sprite->colors, 0, 0);
    memcpy(game_background, get_back_buffer(), get_vram_size());
    set_static_background(game_background);

    FILE *f = fopen("settings.cfg", "r");
    char key[64], val[64];
    
    if (f) {
        t_input_scheme ins = INPUT_ARROWS;
        game_mode_t gm = MODE_HARDCORE;
        while (fscanf(f, "%63s = %63s\n", key, val) == 2) {
            if (strcmp(key, "input") == 0) {
                if (strcmp(val, "ARROWS") == 0) ins = INPUT_ARROWS;
                else if (strcmp(val, "DFJK") == 0) ins = INPUT_DFJK;
                else ins = INPUT_WASD;
            } else if (strcmp(key, "mode") == 0) {
                if (strcmp(val, "BASIC") == 0) gm = MODE_BASIC;
                else if (strcmp(val, "CHALLENGE") == 0) gm = MODE_CHALLENGE;
                else gm = MODE_HARDCORE;
            }
        }
        fclose(f);

        set_input_scheme(ins);

        current_mode = gm;
    } 
    else {
        set_input_scheme(INPUT_ARROWS);
        current_mode = MODE_BASIC;
    }

    current_note_speed = BASE_SPEED;

    bool running = true;
    bool game_completed = false;

    while (running) {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:		
                if (msg.m_notify.interrupts & kb_irq_set) {
                    kbc_ih();
                    if (scancode == 0x01) {
                        running = false;
                    }
                    note_lane lane = get_pressed_lane(scancode);
                    
                    if (lane != LANE_NONE) process_lane_hit(lane);
                }
                if (msg.m_notify.interrupts & mouse_irq_set) { 
                    process_mouse_interrupt();
                    if (current_mode == MODE_HARDCORE) process_mouse_click();
                }
                if (msg.m_notify.interrupts & timer_irq_set) { // I didn't separate this one because I don't think we can change the timer.h file to create a new function that does this (and also retrieves the elapsed seconds, which would have to move there as well)
                    timer_int_handler();
                    if (counter != 0 && counter % frequency == 0) {
                        if (remaining_secs > 0) remaining_secs--;
                        if (current_mode == MODE_CHALLENGE || current_mode == MODE_HARDCORE) {
                            if (remaining_secs % NOTE_SPEED_INCREASE_INTERVAL_S == 0) {
                                current_note_speed += SPEED_INCREMENT;
                                for (int i = 0; i < NUM_LANES; i++) notes[i].speed = current_note_speed;
                            }
                        }
                    }
                    if (current_cheer_pose != -1) {
                        cheer_pose_timer--;
                        if (cheer_pose_timer == 0) current_cheer_pose = -1;
                    }
                    update_notes();
                    render_frame();
                }

                break;
                default:
                break;
            }

            if (remaining_secs == 0) {
                game_completed = true;
                running = false;
            }
        }
    }  

    return game_completed;
}
