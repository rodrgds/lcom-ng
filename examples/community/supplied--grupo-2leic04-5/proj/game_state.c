#include "include/game_state.h"
#include <math.h>

// var global para armazenar o estado atual do jogo
static GameState current_state = STATE_MENU;
static GameState paused_level_state = STATE_LEVEL_1;
static MenuAction selected_action = MENU_ACTION_PRIMARY;
static bool pending_e0 = false;
static bool level_reset_flag = false; // sinaliza que os inimigos devem ser reiniciados
static void player_get_hitbox(const Player *player, float *x, float *y, float *w, float *h);
static void enemy_get_hitbox(const Enemy *enemy, float *cx, float *cy, float *r, float *rx, float *ry, float *rw, float *rh);
bool game_state_is_speech(void);

static GameState game_over_level_state = STATE_LEVEL_1;

#define SC_ESC 0x01
#define SC_W 0x11
#define SC_S 0x1F
#define SC_ENTER 0x1C
#define SC_ARROW_UP 0x48
#define SC_ARROW_DOWN 0x50

// Funçao para inicializar o estado do jogo, começa no menu
void game_state_init(void) {
    current_state = STATE_MENU;
    paused_level_state = STATE_LEVEL_1;
    selected_action = MENU_ACTION_PRIMARY;
    pending_e0 = false;
}

// getter de estado atual
GameState game_state_get(void) {
    return current_state;
}

// Funçao para mudar o estado atual do jogo
void game_state_set(GameState s) {
  current_state = s;
    if (s == STATE_MENU || s == STATE_VICTORY || s == STATE_PAUSED || s == STATE_GAME_OVER) {
        selected_action = MENU_ACTION_PRIMARY;
    }
}

// Inicia o nivel 1 (vindo do menu), reset total de HP
static void game_state_start_level_1(void) {
    // Reset HP ao iniciar o jogo
    Player *p = player_get();
    p->hp = PLAYER_MAX_HP;

    current_state = STATE_SPEECH_1;
    selected_action = MENU_ACTION_PRIMARY;
}

// Chamado quando o jogador completa uma wave/nível
void game_state_level_complete(void) {
    switch (current_state) {
        case STATE_LEVEL_1:
            current_state = STATE_SPEECH_2;
            selected_action = MENU_ACTION_PRIMARY;
            break;
        case STATE_LEVEL_2:
            current_state = STATE_SPEECH_3;
            selected_action = MENU_ACTION_PRIMARY;
            break;
        case STATE_LEVEL_3:
            current_state = STATE_SPEECH_4;
            selected_action = MENU_ACTION_PRIMARY;
            break;
        case STATE_LEVEL_4:
            current_state = STATE_VICTORY;
            selected_action = MENU_ACTION_PRIMARY;
            break;
        default:
            break;
    }
}

// Chamado quando o jogador morre definitivamente (HP == 0)
void game_state_trigger_game_over(void) {
    game_over_level_state = current_state; // guarda o nível atual
    current_state = STATE_GAME_OVER;
    selected_action = MENU_ACTION_PRIMARY;
}

// Funçao para lidar com os scancodes do teclado, usada para navegar no menu e avançar level
void game_state_handle_scancode(uint8_t byte) {
    if (byte == 0xE0) {
        pending_e0 = true;
        return;
    }

    bool is_break = (byte & 0x80) != 0;
    uint8_t code = byte & ~0x80;
    bool was_e0 = pending_e0;
    pending_e0 = false;

    if (is_break) return;

    // ESC sempre pausa o jogo se estiver num nível
    if (code == SC_ESC) {
        if (game_state_is_level()) {
            paused_level_state = current_state;
            current_state = STATE_PAUSED;
            selected_action = MENU_ACTION_PRIMARY;
            player_reset_keys();
        }
        // compor porque ao pressionar esc mesmo estando em pausa por alguma motivo sai do jogo, se calhar ignoramos so o esc nesse caso
        else if (current_state == STATE_PAUSED) {
            current_state = paused_level_state;
            selected_action = MENU_ACTION_PRIMARY;
            player_reset_keys();
        }
        else {
            current_state = STATE_EXIT;
        }
        return;
    }

    // Navegaçao no menu principal
    if (current_state == STATE_MENU) {
        if ((!was_e0 && code == SC_S) || (was_e0 && code == SC_ARROW_DOWN)) {
            selected_action = MENU_ACTION_QUIT;
            return;
        }
        if ((!was_e0 && code == SC_W) || (was_e0 && code == SC_ARROW_UP)) {
            selected_action = MENU_ACTION_PRIMARY;
            return;
        }
        if (!was_e0 && code == SC_ENTER) {
            if (selected_action == MENU_ACTION_QUIT) current_state = STATE_EXIT;
            else game_state_start_level_1();
        }
        return;
    }

    // Ecrã de speech - ENTER avança para o nível
    if (game_state_is_speech()) {
        if (!was_e0 && code == SC_ENTER) {
            switch (current_state) {
                case STATE_SPEECH_1: current_state = STATE_LEVEL_1; break;
                case STATE_SPEECH_2: current_state = STATE_LEVEL_2; break;
                case STATE_SPEECH_3: current_state = STATE_LEVEL_3; break;
                case STATE_SPEECH_4: current_state = STATE_LEVEL_4; break;
                default: return;
            }
            player_start_level(3600);
            bullet_init();
            level_reset_flag = true;
        }
        return;
    }

    // Ecrã de vitória
    if (current_state == STATE_VICTORY) {
        if (!was_e0 && code == SC_ENTER)
          current_state = STATE_MENU;
        return;
    }

    // Ecrã de game over - ENTER reinicia ou sai
    if (current_state == STATE_GAME_OVER) {
        if (!was_e0 && code == SC_ENTER) {
            Player *p = player_get();
            p->hp = PLAYER_MAX_HP;
            switch (game_over_level_state) {
                case STATE_LEVEL_1: current_state = STATE_SPEECH_1; break;
                case STATE_LEVEL_2: current_state = STATE_SPEECH_2; break;
                case STATE_LEVEL_3: current_state = STATE_SPEECH_3; break;
                case STATE_LEVEL_4: current_state = STATE_SPEECH_4; break;
                default: current_state = STATE_SPEECH_1; break;
            }
            selected_action = MENU_ACTION_PRIMARY;
        }
        return;
    }
    
    // Navegaçao no menu de pausa
    if (current_state == STATE_PAUSED) {
        if ((!was_e0 && code == SC_S) || (was_e0 && code == SC_ARROW_DOWN)) {
            if (selected_action == MENU_ACTION_PRIMARY) selected_action = MENU_ACTION_RESTART;
            else selected_action = MENU_ACTION_QUIT;
            return;
        }

        if ((!was_e0 && code == SC_W) || (was_e0 && code == SC_ARROW_UP)) {
            if (selected_action == MENU_ACTION_QUIT) selected_action = MENU_ACTION_RESTART;
            else selected_action = MENU_ACTION_PRIMARY;
            return;
        }

        if (!was_e0 && code == SC_ENTER) {
            if (selected_action == MENU_ACTION_QUIT) current_state = STATE_EXIT;
            else if (selected_action == MENU_ACTION_RESTART) {
                // reinicia o nível pausado voltando ao seu speech
                Player *p = player_get();
                p->hp = PLAYER_MAX_HP;
                switch (paused_level_state) {
                    case STATE_LEVEL_1: current_state = STATE_SPEECH_1; break;
                    case STATE_LEVEL_2: current_state = STATE_SPEECH_2; break;
                    case STATE_LEVEL_3: current_state = STATE_SPEECH_3; break;
                    case STATE_LEVEL_4: current_state = STATE_SPEECH_4; break;
                    default: current_state = STATE_SPEECH_1; break;
                }
                selected_action = MENU_ACTION_PRIMARY;
            }
            else current_state = paused_level_state;
        }
        return;
    }

     /* 
     DEBUG: avançar level com ENTER
        if (game_state_is_level() && !was_e0 && code == SC_ENTER) {
            game_state_level_complete();
        }
    */
}

// Funçao para verificar se o estado atual é um dos níveis
bool game_state_is_level(void) {
    return current_state == STATE_LEVEL_1 ||
           current_state == STATE_LEVEL_2 ||
           current_state == STATE_LEVEL_3 ||
           current_state == STATE_LEVEL_4;
}

// Verifica se o estado atual é um ecrã de speech
bool game_state_is_speech(void) {
    return current_state == STATE_SPEECH_1 ||
           current_state == STATE_SPEECH_2 ||
           current_state == STATE_SPEECH_3 ||
           current_state == STATE_SPEECH_4;
}

// getter para o número do nível atual, devolve 0 se não estiver num nível
int game_state_current_level(void) {
    GameState level_state = (current_state == STATE_PAUSED) ? paused_level_state : current_state;

    switch (level_state) {
        case STATE_LEVEL_1: return 1;
        case STATE_LEVEL_2: return 2;
        case STATE_LEVEL_3: return 3;
        case STATE_LEVEL_4: return 4;
        default: return 0;
    }
}

// getter para a ação selecionada no menu, usada para destacar o botão selecionado e decidir a ação ao carregar enter
MenuAction game_state_selected_action(void) {
    return selected_action;
}

bool check_rect_collision(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
    return (x1 < x2 + w2 && x1 + w1 > x2 &&
           y1 < y2 + h2 && y1 + h1 > y2);
}

bool check_circle_collision(float x1, float y1, float r1, float x2, float y2, float r2) {
    float dx = x1 - x2, dy = y1 - y2;
    float sum = r1 + r2;
    return (dx * dx + dy * dy) < (sum * sum);
}

bool check_circle_rect_collision(float cx, float cy, float r, float rx, float ry, float rw, float rh) {

    float nx = cx < rx ? rx : (cx > rx + rw ? rx + rw : cx);
    float ny = cy < ry ? ry : (cy > ry + rh ? ry + rh : cy);

    float dx = cx - nx;
    float dy = cy - ny;
    float distance_squared = dx * dx + dy * dy;

    return distance_squared < (r * r);
}

static void player_get_hitbox(const Player *player, float *x, float *y, float *w, float *h) {
    *x = (float)(player->x + PLAYER_HITBOX_OFFSET_X);
    *y = (float)(player->y + PLAYER_HITBOX_OFFSET_Y);
    *w = PLAYER_HITBOX_W;
    *h = PLAYER_HITBOX_H;
}

static void enemy_get_hitbox(const Enemy *enemy, float *cx, float *cy, float *r, float *rx, float *ry, float *rw, float *rh) {
    switch (enemy->type) {
        case ENEMY_WALKER:
            *cx = (float)(enemy->x + WALKER_HITBOX_CX_OFFSET_X);
            *cy = (float)(enemy->y + WALKER_HITBOX_CY_OFFSET_Y);
            *r = WALKER_HITBOX_RADIUS;
            break;

        case ENEMY_SHOTTER:
            *rx = (float)(enemy->x + SHOTTER_HITBOX_OFFSET_X);
            *ry = (float)(enemy->y + SHOTTER_HITBOX_OFFSET_Y);
            *rw = SHOTTER_HITBOX_W;
            *rh = SHOTTER_HITBOX_H;
            break;

        case ENEMY_PARALYZER:
            *rx = (float)(enemy->x + PARALYZER_HITBOX_OFFSET_X);
            *ry = (float)(enemy->y + PARALYZER_HITBOX_OFFSET_Y);
            *rw = PARALYZER_HITBOX_W;
            *rh = PARALYZER_HITBOX_H;
            break;
    }
}

bool check_bullet_hits_enemy(Bullet *bullet, const Enemy *enemy) {
    if (!bullet || !enemy || !enemy->is_active) return false;

    float bcx = bullet->x + PLAYER_BULLET_CX_OFFSET;
    float bcy = bullet->y + PLAYER_BULLET_CY_OFFSET;
    float br = PLAYER_BULLET_RADIUS;

    bool hit = false;
    float ecx, ecy, er, erx, ery, erw, erh;
    erx = ery = erw = erh = ecx = ecy = 0;
    enemy_get_hitbox(enemy, &ecx, &ecy, &er, &erx, &ery, &erw, &erh);

    if (enemy->type == ENEMY_WALKER) {
        hit = check_circle_collision(bcx, bcy, br, ecx, ecy, er);
    } else {
        hit = check_circle_rect_collision(bcx, bcy, br,
                                          erx, ery, erw, erh);
    }

    if (hit) {
        bullet->is_active = false; // desativa a bala
    }
    return hit;
   
}

bool check_enemy_bullet_hits_player(Bullet *bullet, const Player *player) {
    if (!bullet || !player || !bullet->is_active || bullet->type != BULLET_ENEMY) return false;

    float brx = bullet->x + ENEMY_BULLET_HITBOX_OFFSET_X;
    float bry = bullet->y + ENEMY_BULLET_HITBOX_OFFSET_Y;
    float brw = ENEMY_BULLET_HITBOX_W;
    float brh = ENEMY_BULLET_HITBOX_H;

    float px, py, pw, ph;
    player_get_hitbox(player, &px, &py, &pw, &ph);

    bool hit = check_rect_collision(px, py, pw, ph, brx, bry, brw, brh);
    if (hit) {
        bullet->is_active = false; // desativa a bala
    }
    return hit;
}

bool check_player_hits_enemy(const Player *player, const Enemy *enemy) {
    if (!enemy->is_active) return false;

    float px, py, pw, ph;
    player_get_hitbox(player, &px, &py, &pw, &ph);

    float ecx, ecy, er, erx, ery, erw, erh;
    erx = ery = erw = erh = ecx = ecy = 0;
    enemy_get_hitbox(enemy, &ecx, &ecy, &er, &erx, &ery, &erw, &erh);

    if (enemy->type == ENEMY_WALKER) {
        return check_circle_rect_collision(ecx, ecy, er,
                                          px, py, pw, ph);
    } else {
        return check_rect_collision(px, py, pw, ph,
                                   erx, ery, erw, erh);
    }
}

// Devolve true e limpa a flag se um novo nível foi iniciado ou reiniciado, usado pelo main para saber quando reinicializar inimigos.
bool game_state_take_level_reset(void) {
    if (level_reset_flag) {
        level_reset_flag = false;
        return true;
    }
    return false;
}
