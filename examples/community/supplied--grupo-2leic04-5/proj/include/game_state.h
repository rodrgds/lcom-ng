#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "player.h"
#include "enemy.h"
#include "bullet.h"

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    STATE_MENU,
    STATE_SPEECH_1,       // painel de speech antes do nível 1
    STATE_SPEECH_2,       // painel de speech antes do nível 2
    STATE_SPEECH_3,       // painel de speech antes do nível 3
    STATE_SPEECH_4,       // painel de speech antes do nível 4
    STATE_LEVEL_1,
    STATE_LEVEL_2,
    STATE_LEVEL_3,
    STATE_LEVEL_4,
    STATE_PAUSED,
    STATE_VICTORY,         // ecrã de créditos/win
    STATE_GAME_OVER,       // ecrã de derrota
    STATE_EXIT
} GameState;

// ações possíveis no menu de pausa e na tela de vitória
typedef enum {
    MENU_ACTION_PRIMARY,
    MENU_ACTION_RESTART,
    MENU_ACTION_QUIT
} MenuAction;

void game_state_init(void);// inicializa o estado do jogo, começando no menu
GameState game_state_get(void);// devolve o estado atual
void game_state_set(GameState new_state);// muda o estado atual
void game_state_handle_scancode(uint8_t byte);// lida com o scancode recebido
bool game_state_is_level(void);// check se o estado atual é um dos estados de nível (para atualizar inimigos, player, etc)
bool game_state_is_speech(void);// check se o estado atual é um dos estados de speech
int game_state_current_level(void);// devolve o número do nível atual (1-4) ou 0 se não estiver em um nível
MenuAction game_state_selected_action(void);// devolve a ação selecionada no menu de pausa ou vitoria
bool game_state_take_level_reset(void);// para ativar a animação de reset

// Chamado pelo loop principal quando o nível atual foi completado
void game_state_level_complete(void);

// Chamado pelo loop principal quando o jogador morre
void game_state_trigger_game_over(void);


/* HITBOXES - offsets e dimensões */
// player
#define PLAYER_HITBOX_OFFSET_X 42
#define PLAYER_HITBOX_OFFSET_Y 35
#define PLAYER_HITBOX_W 44
#define PLAYER_HITBOX_H 55

//walker analog
#define WALKER_HITBOX_CX_OFFSET_X 64
#define WALKER_HITBOX_CY_OFFSET_Y 64
#define WALKER_HITBOX_RADIUS 21

//shotter digital
#define SHOTTER_HITBOX_OFFSET_X 15
#define SHOTTER_HITBOX_OFFSET_Y 42
#define SHOTTER_HITBOX_W 74
#define SHOTTER_HITBOX_H 34

//paralyser alarm
#define PARALYZER_HITBOX_OFFSET_X 37
#define PARALYZER_HITBOX_OFFSET_Y 35
#define PARALYZER_HITBOX_W 54
#define PARALYZER_HITBOX_H 56

//bala player
#define PLAYER_BULLET_CX_OFFSET 50
#define PLAYER_BULLET_CY_OFFSET 44
#define PLAYER_BULLET_RADIUS 8

//bala inimiga retangular
#define ENEMY_BULLET_HITBOX_OFFSET_X 6
#define ENEMY_BULLET_HITBOX_OFFSET_Y 3
#define ENEMY_BULLET_HITBOX_W 8
#define ENEMY_BULLET_HITBOX_H 19

// detetores de colisão básicos
bool check_rect_collision(float x1, float y1, float w1, float h1,
                         float x2, float y2, float w2, float h2);
bool check_circle_collision(float x1, float y1, float r1,
                            float x2, float y2, float r2);
bool check_circle_rect_collision(float cx, float cy, float r,
                            float rx, float ry, float rw, float rh);
                            
// detetores de colisão de alto nível
bool check_player_hits_enemy(const Player *player, const Enemy *enemy);
bool check_bullet_hits_enemy(Bullet *bullet, const Enemy *enemy);
bool check_enemy_bullet_hits_player(Bullet *bullet, const Player *player);

#endif /* GAME_STATE_H */
