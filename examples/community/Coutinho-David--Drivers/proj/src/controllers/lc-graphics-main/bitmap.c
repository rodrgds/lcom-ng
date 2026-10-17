#include "includes/bitmap.h"

static BitmapFileHeader *read_file_header(FILE *file)
{
    BitmapFileHeader *fileHeader = malloc(sizeof(BitmapFileHeader));
    if (fileHeader == NULL)
    {
        //printf("Memory allocation failed\n");
        return NULL;
    }

    if (fread(&fileHeader->type, 2, 1, file) != 1)
    {
        //printf("Error reading file type\n");
        free(fileHeader);
        return NULL;
    }

    if (fileHeader->type != 0x4D42)
    { // 'BM' in little-endian
        //printf("Error: Not a valid BMP file\n");
        free(fileHeader);
        return NULL;
    }

    if (fread(&fileHeader->size, 4, 1, file) != 1 ||
        fread(&fileHeader->reserved, 4, 1, file) != 1 ||
        fread(&fileHeader->offset, 4, 1, file) != 1)
    {
        //printf("Error reading file header fields\n");
        free(fileHeader);
        return NULL;
    }

    return fileHeader;
}

static BitmapInfoHeader *read_info_header(FILE *file)
{
    BitmapInfoHeader *infoHeader = malloc(sizeof(BitmapInfoHeader));
    if (infoHeader == NULL)
    {
        //printf("Memory allocation failed\n");
        return NULL;
    }

    if (fread(infoHeader, sizeof(BitmapInfoHeader), 1, file) != 1)
    {
        //printf("Error reading info header\n");
        free(infoHeader);
        return NULL;
    }

    return infoHeader;
}

static uint8_t *read_bitmap_data(FILE *file, uint32_t offset, uint32_t size)
{
    fseek(file, offset, SEEK_SET);

    uint8_t *bitmapData = malloc(size);
    if (bitmapData == NULL)
    {
        //printf("Memory allocation failed\n");
        return NULL;
    }

    if (fread(bitmapData, size, 1, file) != 1)
    {
        //printf("Error reading bitmap data\n");
        free(bitmapData);
        return NULL;
    }

    return bitmapData;
}

sprite_t *bitmap_load(const char *path, uint16_t mode)
{
    if (path == NULL)
    {
        //printf("Error: path is NULL\n");
        return NULL;
    }

    FILE *file = fopen(path, "rb");

    if (file == NULL)
    {
        //printf("Error opening file %s\n", path);
        return NULL;
    }

    BitmapFileHeader *fileHeader = read_file_header(file);
    
    if (fileHeader == NULL)
    {
        fclose(file);
        return NULL;
    }

    BitmapInfoHeader *infoHeader = read_info_header(file);
    
    if (infoHeader == NULL)
    {
        free(fileHeader);
        fclose(file);
        return NULL;
    }

    uint8_t *bitmapData = read_bitmap_data(file, fileHeader->offset, infoHeader->image_size);
    
    if (bitmapData == NULL)
    {
        free(fileHeader);
        free(infoHeader);
        fclose(file);
        return NULL;
    }

    sprite_t *sprite = malloc(sizeof(sprite_t));
    
    if (sprite == NULL)
    {
        //printf("Error allocating memory for sprite\n");
        free(fileHeader);
        free(infoHeader);
        free(bitmapData);
        fclose(file);
        return NULL;
    }

    sprite->video_map = bitmapData;
    sprite->x = 0;
    sprite->y = 0;
    sprite->width = infoHeader->width;
    sprite->height = infoHeader->height;
    sprite->bits_per_pixel = infoHeader->bits;

    free(fileHeader);
    free(infoHeader);
    fclose(file);

    return sprite;
}
