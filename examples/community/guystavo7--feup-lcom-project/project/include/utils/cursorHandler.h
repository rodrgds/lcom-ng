#ifndef CURSOR_HANDLER
#define CURSOR_HANDLER

/** @file cursorHandler.h
 *  @brief Rato: posicao, botoes e modo de construcao/hover de torres.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../../../lab5/spriteXMP.h"
#include "../../../lab5/terminal.h"

typedef struct Cursor Cursor;

static Cursor* cursor;
static bool isCursorCreated = false;

/** @brief Liberta o cursor e sprites associados.
 *  @param[in] c Instancia do cursor.
 */
void cursorFree(Cursor* c);

/** @brief Obtem o cursor global (singleton).
 *  @return Ponteiro para Cursor.
 */
Cursor* getCursor();

/** @brief Repoe estado do cursor (posicao/botoes) ao valor inicial.
 *  @param[in,out] c Cursor.
 */
void cursorReset(Cursor* c);

/** @brief Actualiza estado bruto a partir do pacote do rato (deltas e botoes).
 *  @param[in,out] c Cursor.
 *  @param[in] dx Delta X.
 *  @param[in] dy Delta Y.
 *  @param[in] leftButton Estado botao esquerdo.
 *  @param[in] rightButton Estado botao direito.
 *  @param[in] middleButton Estado botao do meio.
 */
void cursorUpdateData(Cursor* c, int dx, int dy, bool leftButton, bool rightButton, bool middleButton);

/** @brief Actualiza desenho/animacao do cursor no terminal.
 *  @param[in,out] c Cursor.
 *  @param[in] terminal Terminal de video.
 *  @param[in] deltaTime Delta em segundos.
 */
void cursorUpdateCoordinates(Cursor* c);
void cursorUpdate(Cursor* c, Terminal* terminal, float deltaTime);

/** @brief Coordenada X logica do cursor.
 *  @param[in] c Cursor.
 *  @return Posicao X em pixels.
 */
int cursorGetX(Cursor* c);

/** @brief Coordenada Y logica do cursor.
 *  @param[in] c Cursor.
 *  @return Posicao Y em pixels.
 */
int cursorGetY(Cursor* c);

/** @brief Botao esquerdo mantido.
 *  @param[in] c Cursor.
 *  @return true se premido.
 */
bool cursorIsLeftButtonPressed(Cursor* c);

/** @brief Flanco de subida do botao esquerdo.
 *  @param[in] c Cursor.
 *  @return true no frame em que o botao passou a premido.
 */
bool cursorIsLeftButtonPressedDown(Cursor* c);

/** @brief Botao do meio mantido.
 *  @param[in] c Cursor.
 *  @return true se premido.
 */
bool cursorIsMiddleButtonPressed(Cursor* c);

/** @brief Flanco de subida do botao do meio.
 *  @param[in] c Cursor.
 *  @return true no instante da pressao.
 */
bool cursorIsMiddleButtonPressedDown(Cursor* c);

/** @brief Botao direito mantido.
 *  @param[in] c Cursor.
 *  @return true se premido.
 */
bool cursorIsRightButtonPressed(Cursor* c);

/** @brief Flanco de subida do botao direito.
 *  @param[in] c Cursor.
 *  @return true no instante da pressao.
 */
bool cursorIsRightButtonPressedDown(Cursor* c);

/** @brief Posicao em pixels (saida por referencia).
 *  @param[in] c Cursor.
 *  @param[out] px Coordenada X.
 *  @param[out] py Coordenada Y.
 */
void cursorGetPixelPosition(Cursor* c, int* px, int* py);

/** @brief Activa/desactiva modo de colocacao de torres (cursor de construcao).
 *  @param[in,out] c Cursor.
 *  @param[in] enabled true para modo construcao.
 */
void cursorSetConstructionMode(Cursor* c, bool enabled);

/** @brief Feedback visual se a celula sob o cursor e valida para construir.
 *  @param[in,out] c Cursor.
 *  @param[in] valid true se posicao valida.
 */
void cursorSetPlacementHoverValid(Cursor* c, bool valid);

#endif
