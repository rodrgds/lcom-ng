#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

/* Project XPM decoder used by the graphics and menu renderers. */
char *read_xpm(char *map[], int *width, int *height);

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void * vg_init(unsigned long mode);

/**
 * @brief Fills the screen with the input color
 * 
 * @param color color to fill the screen with
 * @return 0 on success, non-zero upon failure
 */
int vg_fill(unsigned long color);

/**
 * @brief Sets input pixel with input color
 * 
 * Sets the color of the pixel at the specified position to the input value, 
 *  by writing to the corresponding location in VRAM
 * 
 * @param x horizontal coordinate, starts at 0 (leftmost pixel)
 * @param y vertical coordinate, starts at 0 (top pixel)
 * @param color color to set the pixel
 * @return 0 on success, non-zero otherwise
 */
int vg_set_pixel(unsigned long x, unsigned long y, unsigned long color);

/**
 * @brief Returns the color of the input pixel
 * 
 * Returns the color of the pixel at the specified position, 
 *  by reading to the corresponding location in VRAM
 * 
 * @param x horizontal coordinate, starts at 0 (leftmost pixel)
 * @param y vertical coordinate, starts at 0 (top pixel)
 * @return color of the pixel at coordinates (x,y), or -1 if some input argument is not valid
 */
long vg_get_pixel(unsigned long x, unsigned long y);


/**
 * @brief Draw line segment with specifed end points and color
 * 
 * Draws a line segment with the specified end points and the input color, 
 *  by writing to VRAM
 * 
 * @param xi horizontal coordinate of "first" endpoint, starts at 0 (leftmost pixel)  
 * @param yi vertical coordinate of "first" endpoint, starts at 0 (top pixel)
 * @param xf horizontal coordinate of "last" endpoint, starts at 0 (leftmost pixel)  
 * @param yf vertical coordinate of "last" endpoint, starts at 0 (top pixel)  
 * @param color color of the line segment to draw
 * @return 0 upon success, non-zero upon failure
 */
int vg_draw_line(unsigned long xi, unsigned long yi, 
		           unsigned long xf, unsigned long yf, unsigned long color);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */

/** @brief Desenha a pontuacao  @param Valor da Pontuacao @param Coordenada X @param Coordenada Y @param Tamanho 1 - Pequeno, 0 - Grande */
void drawPontuacao(int valor,int x_pos, int y_pos, int small);
/** @brief Desenha um numero no ecra @param Algarismo a Imprimir @param Coordenada X @param Coordenada Y @Apagar(1) ou Escrever(0) @param Tamanho 1 - Pequeno, 0 - Grande */
void drawNumber(int numero, int x_pos, int y_pos, int erase, int small);
/** @brief Desenha uma letra no ecra @param Letra a Imprimir @param Coordenada X @param Coordenada Y @Apagar(1) ou Escrever(0)  */
void drawLetter(char letra, int x_pos, int y_pos, int erase);
/** @brief Desenha o user Interface @param ID da seccao. Inserir Nome(0) @param Coordenada X @param Coordenada Y @Apagar(1) ou Escrever(0)   */
void drawInterface(int numero_id, int x_pos, int y_pos, int erase);

int vg_exit(void);

 /** @} end of video_gr */
 
#endif /* __VIDEO_GR_H */
