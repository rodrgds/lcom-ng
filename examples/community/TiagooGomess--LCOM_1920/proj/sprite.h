#ifndef LAB5_SPRITE_H
#define LAB5_SPRITE_H

#define H_RES 1024  // Resolução 1024x768
#define V_RES 768


#define WHITE 0xffffff
#define BLACK 0x000000
#define BLUE 0x0000ff
#define DARK_BLUE 0x00000f
#define YELLOW 0xffff00
#define GREEN 0x00365
#define PINK 0x749854


typedef struct {
    int x,y; // current position
    int width, height; // dimensions
    int xspeed, yspeed; // current speed
    uint8_t *map;  // the pixmap
} Sprite;

/**
 * @brief - Cria um objeto do tipo Sprite* correspondente ao xpm.
 * @param xpm - Xpm.
 * @param x - Coordenada x onde o sprite é inicializado.
 * @param y - Coordenada y onde o sprite é inicializado.
 * @return - Sprite* correspondente ao xpm; NULL em caso de erro.
 */
Sprite * create_sprite(xpm_map_t xpm, int x, int y);

/**
 * @brief - Desenha o sprite sp;
 * @param sp - Sprite a ser desenhado.
 * @param base - Zona da memória gráfica onde queremos desenhar.
 * @return - 0 em caso de sucesso; 1 em caso de erro.
 */
int draw_sprite(Sprite *sp, uint8_t *base);

/**
 * @brief - Desenha o sprite sp, com uma cor pretendida.
 * @param sp - Sprite a ser desenhado.
 * @param cor - Cor pretendida.
 * @param base - Zona da memória gráfica onde queremos desenhar.
 * @return - 0 em caso de sucesso; 1 em caso de erro.
 */
int draw_sprite_with_color(Sprite *sp, uint32_t cor, uint8_t *base);


#endif //LAB5_SPRITE_H
