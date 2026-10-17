#ifndef _MENU_H_
#define _MENU_H_

/** @defgroup Menu Menu
 * @{
 *
 * Funções para programar o Menu.
 */

/**
 * @brief Struct usada para guardar a informação do rato
 * @param x - coordenada x do rato
 * @param y - coordenada y do rato
 * @param click - indica se o botão esquerdo do rato foi premido
 * @param play - indica se o rato está sobre o botão Play do menu
 * @param score - indica se o rato está sobre o botão High Scores do menu
 * @param exit - indica se o rato está sobre o botão Exit do menu
 */
typedef struct {
     int x, y;
     bool click, play, score, exit;
} Mouse;

/**
 * @brief Inicia a struct Mouse alocando a memória necessária e atribuindo valores a todos os parametros
 * @return Devolve o apontador para a struct Mouse
 */
Mouse* mouseInit();

/**
 * @brief Atualiza a posição do rato no ecrã. Pode ainda alterar o estado do jogo caso haja um click do mouse sobre um dos botões do menu
 * @return delta_x - deslocamento do rato na direção do eixo horizontal desde a última interrupção do mouse
 * @return delta_y - deslocamento do rato na direção do eixo vertical desde a última interrupção do mouse
 */
void updatePosition(Mouse* mouse, int delta_x, int delta_y);

/**
 * @brief Liberta o espaço de memória préviamnete ocupado pela struct mouse
 * @param mouse - apontador para a struct
 */
void deleteMouse(Mouse* mouse);

/**
 * @brief Dá aos parametros da struct mouse os seus valores iniciais
 * @param mouse - apontador para a struct
 */
void reset_mouse(Mouse* mouse);

/**
 * @brief Struct com os vários estados do jogo
 * @param MAIN_MENU - estado do jogo quando está no menu principal
 * @param SCORE - estado do jogo quando está no menu dos high scores
 * @param NAME - estado do jogo quando esta no menu de introdução do username
 * @param GAME - estado do jogo quando o jogador está a meio de uma partida
 * @param PAUSE - estado do jogo quando o jogador faz uma pauda a meio de uma partida
 * @param GAME_OVER - estado do jogo quando o jogador perde e é direcionado para o menu de Game Over
 * @param WON - estado do jogo quando o jogador vence e é direcionado para o menu de You Won
 * @param INSTRUCTIONS - estado do jogo quando está no menu de instruções
 * @param COMP - estado do jogo quando o jogador sai do mesmo
 */
typedef enum {
	MAIN_MENU, SCORE, NAME, GAME, PAUSE, GAME_OVER, WON, INSTRUCTIONS, COMP
} game_st;

/**@}*/

#endif
