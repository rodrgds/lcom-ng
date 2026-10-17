#ifndef __PIXMAP_H
#define __PIXMAP_H

typedef struct {
int width, height;
char *pixmap;
}Pixmap;

/** @brief add pixmap to a list containing all loaded pixmaps
 */
int addPixmap(Pixmap *pm);

/** @brief erases all allocated memory with loaded pixmaps
 */
void pixmap_cleanMemory();

/** @brief add pixmap to a list containing all loaded pixmaps
 * 	@param index pixmap index in loaded pixmaps list
 *  @return pointer to pixmap at given index
 */
Pixmap* getPixmap(unsigned int index);

/** @brief read xmp
 * copied from Joao Cardoso's read_xpm.c (jcard@fe.up.pt)
 *  @return generated pixmap adress
 */
char *read_xpm(char *map[], int *wd, int *ht);

#endif //__PIXMAP_H
