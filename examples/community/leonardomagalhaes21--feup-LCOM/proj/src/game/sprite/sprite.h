/**
 * @file sprite.c
 * @brief Funções para manipulação de sprites e desenho de elementos gráficos na tela.
 */


#ifndef SPRITE_H_
#define SPRITE_H_

#include "xpm/Cuphead_running.xpm"
#include "xpm/cuphead/cuphead1.xpm"
#include "xpm/cuphead/cuphead2.xpm"
#include "xpm/cuphead/cuphead3.xpm"
#include "xpm/cuphead/cuphead4.xpm"
#include "xpm/cuphead/cuphead5.xpm"
#include "xpm/cuphead/cuphead6.xpm"
#include "xpm/cuphead/cuphead7.xpm"
#include "xpm/cuphead/cuphead8.xpm"
#include "xpm/cuphead/cuphead9.xpm"
#include "xpm/cuphead/cuphead10.xpm"
#include "xpm/cuphead/cuphead11.xpm"
#include "xpm/cuphead/cuphead12.xpm"
#include "xpm/cuphead/cuphead13.xpm"
#include "xpm/cuphead/cuphead14.xpm"
#include "xpm/cuphead/cuphead15.xpm"
#include "xpm/cuphead/cuphead16.xpm"
#include "xpm/cuphead/cupheadstand.xpm"

#include "xpm/font/PressStart2P-vaV7.xpm"
#include "xpm/font/PressStart2P-vaV7_115.xpm"
#include "xpm/font/PressStart2P-vaV7_110.xpm"
#include "xpm/font/PressStart2P-vaV7_11A.xpm"


#include "xpm/resolutionModes/14C/hearth2.xpm"
#include "xpm/resolutionModes/14C/mouse.xpm"
#include "xpm/resolutionModes/14C/square.xpm"
#include "xpm/resolutionModes/14C/backgrounds/full_Menu.xpm"
#include "xpm/resolutionModes/14C/backgrounds/background.xpm"
#include "xpm/resolutionModes/14C/backgrounds/ScoreBoardBackgrd.xpm"
#include "xpm/resolutionModes/14C/backgrounds/LeaderboardMenu.xpm"
#include "xpm/resolutionModes/14C/backgrounds/Instructions.xpm"
#include "xpm/resolutionModes/14C/monster1.xpm"
#include "xpm/resolutionModes/14C/monster2.xpm"
#include "xpm/resolutionModes/14C/bala.xpm"


#include "xpm/resolutionModes/11A/hearth2_11A.xpm"
#include "xpm/resolutionModes/11A/square11A.xpm"
#include "xpm/resolutionModes/11A/backgrounds/full_Menu11A.xpm"
#include "xpm/resolutionModes/11A/backgrounds/background11A.xpm"
#include "xpm/resolutionModes/11A/backgrounds/ScoreBoardBackgrd11A.xpm"
#include "xpm/resolutionModes/11A/backgrounds/LeaderboardMenu11A.xpm"
#include "xpm/resolutionModes/11A/backgrounds/Instructions-3.xpm"
#include "xpm/resolutionModes/11A/monster1_11A.xpm"
#include "xpm/resolutionModes/11A/monster2_11A.xpm"
#include "xpm/resolutionModes/11A/bala11A.xpm"

#include "xpm/resolutionModes/11A/cuphead/cuphead1_11A.xpm"
#include "xpm/resolutionModes/11A/cuphead/cuphead2_11A.xpm"
#include "xpm/resolutionModes/11A/cuphead/cuphead3_11A.xpm"
#include "xpm/resolutionModes/11A/cuphead/cuphead4_11A.xpm"
#include "xpm/resolutionModes/11A/cuphead/cuphead5_11A.xpm"
#include "xpm/resolutionModes/11A/cuphead/cuphead6_11A.xpm"
#include "xpm/resolutionModes/11A/cuphead/cuphead7_11A.xpm"
#include "xpm/resolutionModes/11A/cuphead/cuphead8_11A.xpm"
#include "xpm/resolutionModes/11A/cuphead/cuphead9_11A.xpm"
#include "xpm/resolutionModes/11A/cuphead/cuphead10_11A.xpm"
#include "xpm/resolutionModes/11A/cuphead/cuphead11_11A.xpm"
#include "xpm/resolutionModes/11A/cuphead/cuphead12_11A.xpm"
#include "xpm/resolutionModes/11A/cuphead/cuphead13_11A.xpm"
#include "xpm/resolutionModes/11A/cuphead/cuphead14_11A.xpm"
#include "xpm/resolutionModes/11A/cuphead/cuphead15_11A.xpm"
#include "xpm/resolutionModes/11A/cuphead/cuphead16_11A.xpm"
#include "xpm/resolutionModes/11A/cuphead/cupheadstand_11A.xpm"
#include "xpm/resolutionModes/11A/bala_inimigo_11A.xpm"

#include "xpm/resolutionModes/110/hearth2_110.xpm"
#include "xpm/resolutionModes/110/square_110.xpm"
#include "xpm/resolutionModes/110/backgrounds/full_Menu_110.xpm"
#include "xpm/resolutionModes/110/backgrounds/background_110.xpm"
#include "xpm/resolutionModes/110/backgrounds/ScoreBoardBackgrd_110.xpm"
#include "xpm/resolutionModes/110/backgrounds/LeaderboardMenu_110.xpm"
#include "xpm/resolutionModes/110/backgrounds/Instructions-1.xpm"
#include "xpm/resolutionModes/110/monster1_110.xpm"
#include "xpm/resolutionModes/110/monster2_110.xpm"
#include "xpm/resolutionModes/110/bala_110.xpm"
#include "xpm/resolutionModes/110/bala_inimigo_110.xpm"

#include "xpm/resolutionModes/110/cuphead/cuphead1_110.xpm"
#include "xpm/resolutionModes/110/cuphead/cuphead2_110.xpm"
#include "xpm/resolutionModes/110/cuphead/cuphead3_110.xpm"
#include "xpm/resolutionModes/110/cuphead/cuphead4_110.xpm"
#include "xpm/resolutionModes/110/cuphead/cuphead5_110.xpm"
#include "xpm/resolutionModes/110/cuphead/cuphead6_110.xpm"
#include "xpm/resolutionModes/110/cuphead/cuphead7_110.xpm"
#include "xpm/resolutionModes/110/cuphead/cuphead8_110.xpm"
#include "xpm/resolutionModes/110/cuphead/cuphead9_110.xpm"
#include "xpm/resolutionModes/110/cuphead/cuphead10_110.xpm"
#include "xpm/resolutionModes/110/cuphead/cuphead11_110.xpm"
#include "xpm/resolutionModes/110/cuphead/cuphead12_110.xpm"
#include "xpm/resolutionModes/110/cuphead/cuphead13_110.xpm"
#include "xpm/resolutionModes/110/cuphead/cuphead14_110.xpm"
#include "xpm/resolutionModes/110/cuphead/cuphead15_110.xpm"
#include "xpm/resolutionModes/110/cuphead/cuphead16_110.xpm"
#include "xpm/resolutionModes/110/cuphead/cupheadstand_110.xpm"


#include "xpm/resolutionModes/115/hearth2_115.xpm"
#include "xpm/resolutionModes/115/square_115.xpm"
#include "xpm/resolutionModes/115/backgrounds/full_Menu_115.xpm"
#include "xpm/resolutionModes/115/backgrounds/background_115.xpm"
#include "xpm/resolutionModes/115/backgrounds/ScoreBoardBackgrd_115.xpm"
#include "xpm/resolutionModes/115/backgrounds/LeaderboardMenu_115.xpm"
#include "xpm/resolutionModes/115/backgrounds/Instructions-2.xpm"
#include "xpm/resolutionModes/115/monster1_115.xpm"
#include "xpm/resolutionModes/115/monster2_115.xpm"
#include "xpm/resolutionModes/115/bala_115.xpm"
#include "xpm/resolutionModes/115/bala_inimigo_115.xpm"

#include "xpm/resolutionModes/115/cuphead/cuphead1_115.xpm"
#include "xpm/resolutionModes/115/cuphead/cuphead2_115.xpm"
#include "xpm/resolutionModes/115/cuphead/cuphead3_115.xpm"
#include "xpm/resolutionModes/115/cuphead/cuphead4_115.xpm"
#include "xpm/resolutionModes/115/cuphead/cuphead5_115.xpm"
#include "xpm/resolutionModes/115/cuphead/cuphead6_115.xpm"
#include "xpm/resolutionModes/115/cuphead/cuphead7_115.xpm"
#include "xpm/resolutionModes/115/cuphead/cuphead8_115.xpm"
#include "xpm/resolutionModes/115/cuphead/cuphead9_115.xpm"
#include "xpm/resolutionModes/115/cuphead/cuphead10_115.xpm"
#include "xpm/resolutionModes/115/cuphead/cuphead11_115.xpm"
#include "xpm/resolutionModes/115/cuphead/cuphead12_115.xpm"
#include "xpm/resolutionModes/115/cuphead/cuphead13_115.xpm"
#include "xpm/resolutionModes/115/cuphead/cuphead14_115.xpm"
#include "xpm/resolutionModes/115/cuphead/cuphead15_115.xpm"
#include "xpm/resolutionModes/115/cuphead/cuphead16_115.xpm"
#include "xpm/resolutionModes/115/cuphead/cupheadstand_115.xpm"

#include "xpm/resolutionModes/14C/bala_inimigo.xpm"

#include "xpm/resolutionModes/110/backgrounds/ResChoose110.xpm"

#include "devices/graphics/graphics.h"


#define DEFAULT_X_RESOLUTION_14C 1152
#define DEFAULT_Y_RESOLUTION_14C 864


/**
 * @brief Struct that represents a sprite.
 * 
 * A sprite is a 2D image that can be drawn on the screen.
 */
typedef struct {
    int width, height;
    uint32_t *map;
} Sprite;

Sprite *cuphead1;
Sprite *cuphead2;
Sprite *cuphead3;
Sprite *cuphead4;
Sprite *cuphead5;
Sprite *cuphead6;
Sprite *cuphead7;
Sprite *cuphead8;
Sprite *cuphead9;
Sprite *cuphead10;
Sprite *cuphead11;
Sprite *cuphead12;
Sprite *cuphead13;
Sprite *cuphead14;
Sprite *cuphead15;
Sprite *cuphead16;
Sprite *cupheadstand;
Sprite *mouse_cursor;
Sprite *menu_full;
Sprite *background;
Sprite *monster1;
Sprite *monster2;
Sprite *hearthLife;
Sprite *bala;
Sprite *font;
Sprite *ScoreBackGrnd; 
Sprite *LeaderBackGrnd; 
Sprite *bala_inimigo;
Sprite *ResChooseBckgrd;
Sprite *instructions;


/**
 * @brief Cria um sprite a partir de um arquivo xpm.
 * 
 * @param pic O mapa de pixels do sprite em formato xpm.
 * @return Sprite* Um ponteiro para o sprite criado.
 */
Sprite *create_sprite(xpm_map_t pic);


/**
 * @brief Libera a memória alocada para um sprite.
 * 
 * @param sp O sprite a ser destruído.
 */
void destroy_sprite(Sprite *sp);





/**
 * @brief Desenha um sprite na tela.
 * 
 * @param sp O sprite a ser desenhado.
 * @param x A posição x na tela onde o sprite será desenhado.
 * @param y A posição y na tela onde o sprite será desenhado.
 * @return int Retorna 0 em caso de sucesso e 1 em caso de falha.
 */
int draw_sprite(Sprite *sp, int x, int y);


/**
 * @brief Desenha um sprite invertido na tela (horizontalmente).
 * 
 * @param sp O sprite a ser desenhado.
 * @param x A posição x na tela onde o sprite será desenhado.
 * @param y A posição y na tela onde o sprite será desenhado.
 * @return int Retorna 0 em caso de sucesso e 1 em caso de falha.
 */
int draw_reverse_sprite(Sprite *sp, int x, int y);


/**
 * @brief Carrega todos os sprites necessários de acordo com o modo de vídeo especificado.
 * 
 * @param mode O modo de vídeo atual.
 */
void loadAllSprites(uint16_t mode);


/**
 * @brief Limpa uma área da tela preenchendo com a cor preta.
 * 
 * @param x A posição x na tela da área a ser limpa.
 * @param y A posição y na tela da área a ser limpa.
 * @param width A largura da área a ser limpa.
 * @return int Retorna 0 em caso de sucesso e 1 em caso de falha.
 */
int clean_img(uint16_t x, uint16_t y, uint16_t width);


/**
 * @brief Desenha um caractere na tela.
 * 
 * @param c O caractere a ser desenhado.
 * @param x A posição x na tela onde o caractere será desenhado.
 * @param y A posição y na tela onde o caractere será desenhado.
 * @return int Retorna 0 em caso de sucesso e 1 em caso de falha.
 */
int drawChar(char c, uint16_t x, uint16_t y);


/**
 * @brief Desenha um número na tela.
 * 
 * @param c O número a ser desenhado.
 * @param x A posição x na tela onde o número será desenhado.
 * @param y A posição y na tela onde o número será desenhado.
 * @return int Retorna 0 em caso de sucesso e 1 em caso de falha.
 */
int drawNum(int c, uint16_t x, uint16_t y);


/**
 * @brief Desenha um texto na tela.
 * 
 * @param txt O texto a ser desenhado.
 * @param x A posição x na tela onde o texto será desenhado.
 * @param y A posição y na tela onde o texto será desenhado.
 * @return int Retorna 0 em caso de sucesso e 1 em caso de falha.
 */
int drawTxt(char txt[], uint16_t x, uint16_t y);


/**
 * @brief Carrega os sprites iniciais necessários para o jogo.
 */
void loadInitialSprites();


/**
 * @brief Libera a memória dos sprites iniciais.
 */
void freeInitialSprites();

bool check_collision(Sprite *sp1, int x1, int y1, Sprite *sp2, int x2, int y2);

void freeAllSprites();

#endif

