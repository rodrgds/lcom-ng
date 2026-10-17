#include "image.h"

Image *image_create_sprite(xpm_map_t xpm){
    Image *image = (Image*) malloc(sizeof *image);
    xpm_image_t pixmap;

    image->type = SPRITE;
    image->colors = (uint16_t*) xpm_load(xpm, XPM_5_6_5, &pixmap);

    if (image->colors == NULL){
        printf("Could not initialize sprite!\n");
        free(image);

        image = NULL;
    }

    image->width = pixmap.width;
    image->height = pixmap.height;

    return image;
}

Image *image_create_shape(uint16_t width, uint16_t height, uint16_t color){
    Image *image = (Image*) malloc(sizeof *image);
    image->colors = malloc(sizeof *image->colors);

    image->type = SHAPE;
    image->width = width; 
    image->height = height;
    *image->colors = color;

    return image;
}

void image_destroy(Image* image){
    if (image == NULL) return;

    free(image->colors);
    free(image);
}
