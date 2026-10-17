#pragma once

/** @defgroup Graphics Graphics
 * @{
 *
 * Funções para programar a placa gráfica.
 */

//////////////////////////////////////////////////////////////////////////////////
static char *video_mem;		/* Process (virtual) address to which VRAM is mapped */
static int bits_per_pixel; /* Number of VRAM bits per pixel */
static int num_bytes_mode;


//////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Devolve informação sober o modo do VBE incluindo tamanho do ecrã cores e endereço fisico da VRAM
 * @param mode - Modo sobre o qual será devolvida a informação
 * @param vmi_p - endereço da estrutura vbe_mode_info_t a ser inicializada
 * @return 0 em caso de sucesso e outro valor caso contrário
 */
int vbe_info(uint16_t mode,  vbe_mode_info_t *vmi_p);

/**
 * @brief Faz o mapeamento da memória virtual
 * @param vmi_p - endereço da estrutura vbe_mode_info_t a ser inicializada
 * @return 0 em caso de sucesso e outro valor caso contrário
 */
int map_vram(vbe_mode_info_t *vmi_p);

/**
 * @brief Desenha
 * @param pic - Endereco base do array de caracteres da imagem xpm
 * @param xpm - imagem xpm a ser desenhada
 * @param x - coordenada x da imagem a ser desenhada
 * @param y - coordenada y da imagem a ser desenhada
 * @return
 */
int vg_draw_xpm(unsigned char* pic,  xpm_image_t* xpm, uint16_t x, uint16_t y);

/**
 * @brief Copia o conteudo do array de caracteres doubleBuff para video_mem
 */
void double_buffering();

/**
 * @brief Verifica se ocorre colisao entre objetos na area definida pelos parametros
 * @param pic  - Endereco base do array de characteres da imagem xpm
 * @param x	- coordenada x da area retangular onde e procurada colisao
 * @param y - coordenada y da area retangular onde e procurada colisao
 * @param width -largura da area retangular onde e procurada colisao
 * @param height -altura da area retangular onde e procurada colisao
 * @return Verdadeiro se ocorre colisao (se for encontrada a cor 0xF800 na area), senao devolve falso
 */
bool check_colision(unsigned char* pic, int x, int y,int width, int height);

/**@}*/
