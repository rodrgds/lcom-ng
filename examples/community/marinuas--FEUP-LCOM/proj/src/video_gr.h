#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

static uint8_t *video_mem;		/* Process (virtual) address to which VRAM is mapped */
static mmap_t map_mem;
//static void *temp_video_mem;
static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static unsigned bytes_per_pixel;
vbe_mode_info_t vmi_p;
int r;
struct minix_mem_range mr; /* physical memory range */
unsigned int vram_base; /* VRAM’s physical addresss */
unsigned int vram_size; /* VRAM’s size, but you can use the frame-buffer size, instead */
extern uint32_t counter;

/**
 * @brief Prepara o modo gráfico
 * @param mode - modo a ser inicializado
 */
int VBE_inteface(uint16_t mode);
/**
 * @brief Inicializa o modo gráfico escolhido
 * @param mode - modo a ser inicializado
 */
void* (vg_init)(uint16_t mode);
/**
 * @brief Mapeia a VRAM
 */
int mapping_vram();
/**
 * @brief Dá cor ao pixel
 * @param x - coordenada x do pixel 
 * @param y - coordenada y do pixel 
 * @param color - cor selecionada para colorir o pixel
 */
int pixel_color(uint16_t x, uint16_t y, uint32_t color);
/**
 * @brief Desenha o xpm no ecrã
 * @param image - imagem xpm a desenhar 
 * @param x - coordenada x do pixel 
 * @param y - coordenada y do pixel 
 * @param xpm_loaded - xpm carregado
 */
int vg_draw_xpm_proj(xpm_image_t image, uint16_t x, uint16_t y, uint8_t* xpm_loaded);
/**
 * @brief Faz buffer duplo.
 */
void change_buffer();
