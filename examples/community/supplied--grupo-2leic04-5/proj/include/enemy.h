#ifndef ENEMY_H
#define ENEMY_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>
#include <lcom/xpm.h>
#include "../include/player.h"
#include "../include/bullet.h"

#define MAX_ENEMIES 20

typedef enum {
    ENEMY_WALKER, // analog sprites - move verticalmente
    ENEMY_SHOTTER, // digital sprites - parado e dispara
    ENEMY_PARALYZER // alarm sprites - parado e paraliza
} EnemyType;

// Stats do inimigo mosrtam como ele se comporta, separado do estado de runtime (posiçao, animaçao, ativo ou nao)
typedef struct {
    int speed; // pixels por frame
    int health; // n de hits para morrer
    int direction; // 1 para baixo, -1 para cima, 0 para parado
    int startY; // posição Y inicial (referência para max_distance)
    int max_distance; // distância máxima de movimento vertical apenas para walkers
    int fire_cooldown;// contador decrescente entre disparos
    int fire_rate; // para shotters e paralyzers, taxa de disparo em frames
} EnemyStats;

typedef struct {
    int32_t x, y;
    EnemyType type;
    bool is_active;
    int wave_id; // id da wave do inimigo, usado para lógica de spawn e vitoria dos niveis

    EnemyStats stats; // estado comportamento 

    // animação
    uint8_t **frame_maps;
    xpm_image_t img_info;
    int total_time;
    int time_left;
    int current_frame;
    int max_frames;
    int rail_top; // para walkers limites top e bottom
    int rail_bot;
    int damage_ticks; // ticks restantes de dano (usado para piscar o inimigo quando é atingido)
} Enemy;


void init_enemies(Enemy enemies[], int count);
void update_enemies(Enemy enemies[], int count, Player *player);
void draw_enemies(Enemy enemies[], int count);
int enemy_load_sprites(void);
Enemy* enemy_get_all(void);
int enemy_get_count(void);

// Associa os ponteiros de sprites ao inimigo de acordo com o seu tipo
void enemy_assign_sprites(Enemy *enemy);

// ativa um inimigo com as coordenadas, tipo, wave_id e stats fornecidos
void enemy_spawn(Enemy *enemy, int x, int y, EnemyType type, int wave_id, const EnemyStats *stats);

// Devolve o indice do primeiro slot livre is_active false, ou -1 se não houver
int  enemy_find_free_slot(Enemy enemies[], int max);

// true se ainda houver inimigos ativos pertencentes a wave_id
bool enemy_wave_has_active(Enemy enemies[], int max, int wave_id);

// verifica se a bala do inimigo atingiu o player
bool check_bullet_hits_player(const Bullet *b, const Player *p);

#endif // ENEMY_H
