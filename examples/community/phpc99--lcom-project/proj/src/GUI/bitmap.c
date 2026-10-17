#include "src/GUI/bitmap.h"

/**
 * @brief Reads the BMP file header from the given file.
 *
 * Allocates and fills a `BitmapFileHeader` struct by reading the initial bytes from the file.
 * Validates the file type to ensure it's a proper BMP.
 *
 * @param file File pointer opened in binary mode.
 * @return Pointer to `BitmapFileHeader`, or NULL on failure.
 */

static BitmapFileHeader *read_file_header(FILE *file)
{
    BitmapFileHeader *fileHeader = malloc(sizeof(BitmapFileHeader));
    if (fileHeader == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }

    if (fread(&fileHeader->type, 2, 1, file) != 1)
    {
        printf("Error reading file type\n");
        free(fileHeader);
        return NULL;
    }

    if (fileHeader->type != 0x4D42)
    { // 'BM' in little-endian
        printf("Error: Not a valid BMP file\n");
        free(fileHeader);
        return NULL;
    }

    if (fread(&fileHeader->size, 4, 1, file) != 1 ||
        fread(&fileHeader->reserved, 4, 1, file) != 1 ||
        fread(&fileHeader->offset, 4, 1, file) != 1)
    {
        printf("Error reading file header fields\n");
        free(fileHeader);
        return NULL;
    }

    return fileHeader;
}

/**
 * @brief Reads the BMP info header from the file.
 *
 * Allocates and reads a `BitmapInfoHeader`, which contains the image's metadata
 * such as dimensions, bit depth, and compression.
 *
 * @param file File pointer opened in binary mode.
 * @return Pointer to `BitmapInfoHeader`, or NULL on failure.
 */

static BitmapInfoHeader *read_info_header(FILE *file)
{
    BitmapInfoHeader *infoHeader = malloc(sizeof(BitmapInfoHeader));
    if (infoHeader == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }

    if (fread(infoHeader, sizeof(BitmapInfoHeader), 1, file) != 1)
    {
        printf("Error reading info header\n");
        free(infoHeader);
        return NULL;
    }

    return infoHeader;
}

/**
 * @brief Reads the bitmap image data from the file.
 *
 * Allocates a buffer and reads the raw pixel data starting from the specified offset.
 *
 * @param file File pointer.
 * @param offset Byte offset in the file where image data begins.
 * @param size Number of bytes to read (size of the image data).
 * @return Pointer to the pixel data buffer, or NULL on failure.
 */

static uint8_t *read_bitmap_data(FILE *file, uint32_t offset, uint32_t size)
{
    fseek(file, offset, SEEK_SET);

    uint8_t *bitmapData = malloc(size);
    if (bitmapData == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }

    if (fread(bitmapData, size, 1, file) != 1)
    {
        printf("Error reading bitmap data\n");
        free(bitmapData);
        return NULL;
    }

    return bitmapData;
}

/**
 * @brief Loads a BMP file and converts it into a `sprite_t`.
 *
 * Handles reading the BMP headers and image data,
 * and returns a fully initialized sprite structure.
 *
 * @param path Path to the .bmp file.
 * @param mode Graphics mode used for initialization (currently unused).
 * @return Pointer to a `sprite_t` containing the image, or NULL on failure.
 */

sprite_t *bitmap_load(const char *path, uint16_t mode)
{
    if (path == NULL)
    {
        printf("Error: path is NULL\n");
        return NULL;
    }

    FILE *file = fopen(path, "rb");

    if (file == NULL)
    {
        printf("Error opening file %s\n", path);
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
        printf("Error allocating memory for sprite\n");
        free(fileHeader);
        free(infoHeader);
        free(bitmapData);
        fclose(file);
        return NULL;
    }

    sprite->video_map = bitmapData;
    sprite->position.x = 0;
    sprite->position.y = 0;
    sprite->width = infoHeader->width;
    sprite->height = infoHeader->height;
    sprite->bits_per_pixel = infoHeader->bits;

    free(fileHeader);
    free(infoHeader);
    fclose(file);

    return sprite;
}
