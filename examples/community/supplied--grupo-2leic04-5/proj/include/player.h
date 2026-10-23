#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <lcom/xpm.h>

#define PLAYER_NUM_FRAMES 10
#define PLAYER_PLAY_FRAMES PLAYER_NUM_FRAMES
#define PLAYER_MAX_HP 4

typedef enum {
  FACING_UP = -1,
  FACING_DOWN = 1
} FacingVertical;

typedef struct {
  int x;
  int y;

  uint8_t **frame_maps;
  xpm_image_t img_info;
 
  int total_time;
  int time_left;
  int current_frame;
  FacingVertical facing_v;
  int freeze_ticks; // ticks restantes de congelamento
  int hp; // vida atual do player (max 4)
  int damage_ticks; // ticks restantes de dano (usado para piscar o player quando é atingido)
} Player;

// Inicializa o jogador no centro do ecrã
void player_init(uint8_t **maps, xpm_image_t img_info);

// Devolve um ponteiro para o estado atual do jogador
Player *player_get(void);

// Atualiza a posição do jogador com base nas teclas premidas
void player_update(void);

// Devolve true se ESC foi premido (sinaliza saída)
bool player_wants_exit(void);

// Reseta o estado das teclas
void player_reset_keys(void);

// Quando clicar em nova tentativa
void player_trigger_retry(void);

// Quando clicar em iniciar nível
void player_start_level(int level_time_ticks);

// Processa um byte de scancode do teclado
void scancode_process(uint8_t byte);

// Atualiza facing vertical com delta_y do rato
void player_update_facing_vertical(int16_t mouse_dy);

// Atualiza o estado do botão esquerdo do rato
void player_set_mouse_left_button(bool pressed);

// Congela o jogador por um certo número de ticks
void player_freeze(int ticks);

// Reduz 1 HP do player. Devolve true se morreu
bool player_take_damage(void);

// Devolve true se o player está morto (hp <= 0)
bool player_is_dead(void);

#endif /* PLAYER_H */
