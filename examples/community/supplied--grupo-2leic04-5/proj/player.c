#include "include/player.h"
#include "include/config.h"
#include "include/bullet.h"
#include "include/renderer.h"
#include <stdbool.h>
#include <stdint.h>

// Estado das teclas privado a este módulo
static bool key_up = false;
static bool key_down = false;
static bool key_left = false;
static bool key_right = false;
static bool key_esc = false;

// Estados do rato
static bool mouse_left_button_pressed = false;

// cooldown entre tiros (em ticks)
static int fire_cooldown = 0;

//instancia unica do jogador, ou seja, singleton
static Player player;

// ticks desde a ultima troca de frame do jogador, usado para controlar a velocidade da animaçao
static int animation_ticks = 0;


//inizializa o jogador
void player_init(uint8_t **maps, xpm_image_t img_info) {

  // guarda os mapas e a info da imagem do jogador
  player.frame_maps = maps;
  player.img_info = img_info;

  // direçao inicial do jogador, para baixo
  player.facing_v = FACING_DOWN;

  // posiciona o jogador no centro do ecrã
  player.x = (SCREEN_W - player.img_info.width) / 2;
  player.y = (SCREEN_H - player.img_info.height) / 2;

  player.current_frame = 0;
  animation_ticks = 0;

  player.total_time = 3600; // 60 segundos a 60 ticks por segundo
  player.time_left = 3600;
  player.freeze_ticks = 0;
  player.damage_ticks = 0;
  player.hp = PLAYER_MAX_HP;
}

void player_reset_keys(void) {
    key_up = false;
    key_down = false;
    key_left = false;
    key_right = false;
    key_esc = false;
}


void player_start_level(int level_time_ticks) {
  // tempo total do nivel e tempo restante
  player.total_time = level_time_ticks;
  player.time_left = level_time_ticks;

  // reposiciona o jogador no centro do ecra
  player.x = (SCREEN_W - player.img_info.width) / 2;
  player.y = (SCREEN_H - player.img_info.height) / 2;

  // direçao inicial do jogador, para baixo
  player.facing_v = FACING_DOWN;

  // reseta a animaçao para o frame inicial
  player.current_frame = 0;

  // reset o cooldown dos tiros
  animation_ticks = 0;

  // reseta o estado do rato
  mouse_left_button_pressed = false;

  // reseta o cooldown dos tiros
  fire_cooldown = 0;

  // reseta o estado de congelamento
  player.freeze_ticks = 0;

  // recupera a vida maxima do player
  player.hp = PLAYER_MAX_HP;

  player_reset_keys();
}

void player_trigger_retry(void) {
  player.time_left = player.total_time; // reseta o tempo
  player.current_frame = 0;
  animation_ticks = 0;
}

Player *player_get(void) {
  return &player;
}

// para sair ou menu 
bool player_wants_exit(void) {
  return key_esc;
}

void player_update(void) {
  if (player.freeze_ticks > 0) player.freeze_ticks--;
  
  // movimento basico (bloqueado se congelado)
  if (player.freeze_ticks <= 0) {
    if (key_up) player.y -= PLAYER_SPEED;
    if (key_down) player.y += PLAYER_SPEED;
    if (key_left) player.x -= PLAYER_SPEED;
    if (key_right) player.x += PLAYER_SPEED;
  }

    // manter dentro de limites
  if (player.x < PLAY_AREA_X_MIN) player.x = PLAY_AREA_X_MIN;
  if (player.y < PLAY_AREA_Y_MIN) player.y = PLAY_AREA_Y_MIN;

  if (player.x > PLAY_AREA_X_MAX - player.img_info.width)
      player.x = PLAY_AREA_X_MAX - player.img_info.width;

  if (player.y > PLAY_AREA_Y_MAX - player.img_info.height)
      player.y = PLAY_AREA_Y_MAX - player.img_info.height;

  if (player.time_left > 0) player.time_left--;
  else player_trigger_retry();

  // animaçao do jogador, troca de frame a cada 16 ticks (trocar o valor no if para mudar a velocidade da animaçao)
  animation_ticks++;
  if (animation_ticks >= 8) {
    animation_ticks = 0;
    player.current_frame = (player.current_frame + 1) % PLAYER_PLAY_FRAMES;
  }

  if (fire_cooldown > 0) fire_cooldown--;
  if (player.damage_ticks > 0) player.damage_ticks--;

  // disparar se o botao esquerdo do rato estiver pressionado e o cooldown tiver a 0
  if (mouse_left_button_pressed && fire_cooldown == 0) {
    
    // calcula a posicao do tiro para sair do centro do jogador
    int bullet_x = player.x + (player.img_info.width  - renderer_get_bullet_width())  / 2;
    int bullet_y = player.y + (player.img_info.height - renderer_get_bullet_height()) / 2;

    // cria o tiro na direçao vertical, se for criado com sucesso, reseta o cooldown
    if (bullet_spawn(bullet_x, bullet_y, (int)player.facing_v, BULLET_SPEED, BULLET_PLAYER) != -1) {
      fire_cooldown = BULLET_COOLDOWN;
    }
  }
}

//atualiza a direçao vertical segundo o movimento do rato
void player_update_facing_vertical(int16_t mouse_dy) {
  if (mouse_dy > 0) player.facing_v = FACING_UP;
  else if (mouse_dy < 0) player.facing_v = FACING_DOWN;
}

// atualiza estado do botao esquerdo do rato
void player_set_mouse_left_button(bool pressed) {
  mouse_left_button_pressed = pressed;
}

static bool pending_e0 = false; // último byte foi 0xE0?

void scancode_process(uint8_t byte) {
  if (byte == 0xE0) {
    pending_e0 = true;
    return;
  }

  bool is_break = (byte & 0x80) != 0;
  uint8_t code = byte & ~0x80; // remove bit de break


  // setas 
  if (pending_e0) {
    pending_e0 = false;
    switch (code) {
      case 0x48: key_up = !is_break; break;
      case 0x50: key_down = !is_break; break;
      case 0x4B: key_left = !is_break; break;
      case 0x4D: key_right = !is_break; break;
      default: break;
    }
    } 
  else {
    // WASD e ESC
    switch (code) {
      case 0x11: key_up    = !is_break; break;
      case 0x1F: key_down  = !is_break; break;
      case 0x1E: key_left  = !is_break; break;
      case 0x20: key_right = !is_break; break;
      case 0x01: key_esc   = !is_break; break;
      default: break;
    }
  }
}

void player_freeze(int ticks) {
  player.freeze_ticks = ticks;
}

// Reduz 1 HP. Devolve true se morreu. E agora evita overflow de dano
bool player_take_damage(void) {
  if (player.hp > 0) {
    player.hp--;
  }

  if (player.hp < 0) {
    player.hp = 0;
  }

  player.damage_ticks = 20; // pisca o jogador por 20 frames

  return player.hp == 0;
}

bool player_is_dead(void) {
  return player.hp <= 0;
}
