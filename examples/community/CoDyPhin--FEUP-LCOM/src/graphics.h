#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <lcom/vbe.h>

/** @defgroup graphics Graphics
 * @{
 * Graphics functions
 */

/**
 * @brief Sets up memory for graphics mode
 * @param mode Graphics mode
 * @return Video Memory Address
 */
void* (map_adress)(uint16_t mode);

/**
 * @brief Get Method for the Horizontal Resolution
 * @return Horizontal Resolution
 */
uint16_t getHres();
/**
 * @brief Get Method for the Veritcal Resolution
 * @return Vertical Resolution
 */
uint16_t getVres();


/**
 * @brief Initializes the Graphics Mode
 * @param mode
 * @return Pointer for the Graphics Memory Address
 */
void* (vg_init)(uint16_t mode);

/**
 * @brief Sets up the VBE
 * @param mode Graphics mode
 * @return 1 if error, 0 if OK
 */
int (gpu_mode)(uint16_t mode);
/** @} end of Graphics */

