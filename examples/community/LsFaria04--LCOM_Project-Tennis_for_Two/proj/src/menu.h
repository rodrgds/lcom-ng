#ifndef MENU_H
#define MENU_H

#include <lcom/lcf.h>
#include "states.h"
#include "sprite.h"
#include "xpms.h"
#include "player.h"
#include "game.h"
#include "devices/i8042.h"
#include "devices/i8254.h"
#include "devices/kbd.h"
#include "devices/mouse.h"
#include "devices/rtc.h"
#include "devices/VBE.h"
#include "devices/video.h"

/**
* @struct Menu
* @brief Estrutura que contém sprites do menu principal e do menu de pausa
*/
typedef struct {
    Sprite title;               /**< Sprite do título do menu inicial. */
    Sprite play_button;         /**< Sprite do botão play. */
    Sprite quit;                /**< Sprite do botão quit. */
    Sprite play_button_hover;   /**< Sprite do botão play quando está em hover. */
    Sprite quit_hover;          /**< Sprite do botão quit quando está em hover. */
    Sprite pause_menu;          /**< Sprite do menu de pausa. */
    Sprite resume;              /**< Sprite do botão resume. */
    Sprite restart;             /**< Sprite do botão restart. */
    Sprite quit_pause;          /**< Sprite do botão quit do menu de pausa. */
    Sprite resume_hover;        /**< Sprite do botão resume quando está em hover. */
    Sprite restart_hover;       /**< Sprite do botão restart quando está em hover. */
    Sprite quit_pause_hover;    /**< Sprite do botão quit do menu de pausa quando está em hover. */
    uint8_t selected;           /**< Índice do item atualmente selecionado no menu. */
} Menu;

/**
 * @struct Mouse
 * @brief Estrutura que contém as coordenadas e o sprite do mouse
 */
typedef struct {
    Sprite* sprite; /**< Apontador para o sprite do mouse. */
    int x;          /**< Coordenada x do mouse. */
    int y;          /**< Coordenada y do mouse. */
} Mouse;

/**
 * @brief inicializa sprites do menu
 * @param isStartMenu permite escolher o menu a inicializar
 * @return Retorna um apontador para o menu ou null caso algo falhe
*/
Menu* (initialize_menu)(bool isStartMenu);

/**
 * @brief chama funções relevantes quando há uma interrupção vinda do teclado e o jogo está no menu
 * @param code code da tecla premida
 * @param state pointer para variável Game_state que pode ou não ser atualizada
 * @param menu pointer para menu onde selected pode ou não ser atualizada
 * @param isStartMenu escolhe entre menu inicial e menu de pausa
 * @return 1 se algum erro ocorrer, 0 caso contrário
*/
int (kbd_handler_menu)(Game_state* state, Menu* menu, bool isStartMenu);

/**
 * @brief atualiza parâmetros state e menu caso necessário
 * @param code code da tecla premida
 * @param state pointer para variável Game_state que pode ou não ser atualizada
 * @param menu pointer para menu onde selected pode ou não ser atualizada
 * @return 1 se algum erro ocorrer, 0 caso contrário
*/
int (update_selected)(unsigned char code, Game_state* state, Menu* menu);

/**
 * @brief chama funções relevantes para lidar com interrupções vindas do timer quando o jogo está no menu
 * @param menu menu a ser atualizado
 * @param mouse mouse a ser atualizado
 * @param isStartMenu escolhe entre menu inicial ou menu de pausa
 * @return 1 se algum erro ocorrer, 0 caso contrário
*/
int (timer_handler_menu)(Menu* menu, Mouse* mouse, bool isStartMenu);

/**
 * @brief desenha um sprite na tela
 * @param x_offset coordenada x inicial
 * @param y_offset coordenada y inicial
 * @param sprite sprite a ser desenhado
 * @return 1 se algum erro ocorrer, 0 caso contrário
*/
int (draw_field)(int x_offset, int y_offset, Sprite sprite);

/**
 * @brief desenha o menu
 * @param menu menu a ser desenhado
 * @return 1 se algum erro ocorrer, 0 caso contrário
*/
int (draw_menu)(Menu* menu);


/**
 * @brief escolhe um sprite que corresponde a um numero de 0 a 9
 * @param num número que indica o sprite a ser escolhido
 * @param sprite pointer que recebe o sprite do número indicado no parâmetro num
 * @return 1 se algum erro ocorrer, 0 caso contrário
*/
int (choose_number_sprite)(uint8_t num, Sprite* sprite);

/**
 * @brief atualiza variáveis game_state e menu quando nessessário
 * @param code code da tecla premida
 * @param game_state pointer para state do jogo
 * @param menu pointer para menu
 * @return 1 se algum erro ocorrer, 0 caso contrário
*/
int update_selected_pause(unsigned char code, Game_state* game_state, Menu* menu);

/**
 * @brief desenha o menu de pausa
 * @param menu pointer para menu com os sprites nessessários
 * @return 1 se algum erro ocorrer, 0 caso contrário
*/
int drawPause(Menu *menu);

/**
 * @brief usa o rato para atualizar select do menu
 * @param menu pointer para menu
 * @param mouse pointer para o mouse
 * @param game_state estado do jogo
 * @param isStartMenu booleano que indica se o menu é o inicial ou não
 * @return 1 se algum erro ocorrer, 0 caso contrário
*/
void update_selected_mouse(Menu* menu, Mouse* mouse, Game_state* game_state, bool isStartMenu);

/**
 * @brief Cria um objeto Mouse.
 * @return Retorna um apontador para o Mouse.
 */
Mouse* createMouse();

/**
 * @brief Desenha o mouse na tela.
 * @param mouse Apontador para o objeto Mouse a ser desenhado.
 * @return Retorna 1 se ocorreu algum erro e 0 se correu tudo bem
 */
int drawMouse(Mouse* mouse);

/**
 * @brief Destrói um objeto Mouse.
 * @param mouse Apontador para o objeto Mouse a ser destruído.
 */
void destroyMouse(Mouse* mouse);

/**
 * @brief Atualiza a posição do mouse.
 * @param mouse Apontador para o objeto Mouse a ser atualizado.
 * @param delta_x Mudança na coordenada x.
 * @param delta_y Mudança na coordenada y.
 * @param state Estado atual do jogo
 * @return Retorna 1 se ocorreu algum erro e 0 se correu tudo bem
 */
int updateMousePosition(Mouse* mouse, int delta_x, int delta_y, Game_state* state);

/**
 * @brief  chama funções relevantes quando há uma interrupção vinda do mouse e o jogo está no menu
 * @param start Booleano que indica se o menu atual é o inicial ou não
 * @param menu Ponteiro para o menu
 * @param mouse Ponteiro para o mouse 
 * @param game_state Ponteiro para o Game_state representando o estado atual do jogo.
 */
void menuMouseHandler(bool start, Menu* menu, Mouse* mouse, Game_state* game_state);

#endif
