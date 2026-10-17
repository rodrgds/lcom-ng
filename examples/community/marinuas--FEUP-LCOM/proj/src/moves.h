/**
 * @brief Cria os fogos.
 */
int fogos_pos_arr();
/**
 * @brief Cria os ets e posiciona-os aleatoriamente no eixo dos y.
 */
int ets_pos_arr();
/**
 * @brief Atualiza a posição do cão a subir.
 * @param image - imagem xpm a desenhar 
 * @param xpm_loaded - xpm carregado
 */
int up( xpm_image_t image, uint8_t* xpm_loaded);
/**
 * @brief Atualiza a posição do cão a descer.
 * @param image - imagem xpm a desenhar 
 * @param xpm_loaded - xpm carregado
 */
int down(xpm_image_t image, uint8_t* xpm_loaded);
/**
 * @brief Cria os asteroides e posiciona-os aleatoriamente no eixo dos y.
 */
int asteroides_pos_arr();
/**
 * @brief Cria as coins e posiciona-as aleatoriamente no eixo dos y.
 */
int coins_pos_arr();
