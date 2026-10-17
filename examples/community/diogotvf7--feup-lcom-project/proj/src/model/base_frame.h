#pragma once
#include <lcom/lcf.h>
#include "model/queue.h"
#include "colors.h"

/**
 * @brief Structure representing a frame buffer.
 */
typedef struct {
    uint16_t width; /**< The width of the frame buffer. */
    uint16_t height; /**< The height of the frame buffer. */
    uint16_t bytes_per_pixel; /**< The number of bytes per pixel. */
    unsigned size; /**< The size of the frame buffer. */
    uint8_t *base_addr; /**< The base address of the frame buffer. */
} frame_buffer_t;

/**
 * @brief Creates a frame buffer with the specified width, height, and bytes per pixel.
 *
 * @param width The width of the frame buffer.
 * @param height The height of the frame buffer.
 * @param bytes_per_pixel The number of bytes per pixel.
 */
void create_frame_buffer(uint16_t width, uint16_t height, uint16_t bytes_per_pixel);

/**
 * @brief Draws a pixel with the specified color at the given coordinates in the frame buffer.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color of the pixel.
 */
void draw_frame_pixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a circle with the specified position, thickness, and color in the frame buffer.
 *
 * @param p The position of the center of the circle.
 * @param thickness The thickness of the circle.
 * @param color The color of the circle.
 */
void draw_frame_circle(Position p, uint16_t thickness, uint32_t color);

/**
 * @brief Draws a line using Bresenham's algorithm with the specified positions, color, and thickness in the frame buffer.
 *
 * @param p1 The starting position of the line.
 * @param p2 The ending position of the line.
 * @param color The color of the line.
 * @param thickness The thickness of the line.
 */
void draw_bresenham_line(Position p1, Position p2, uint32_t color, uint16_t thickness);

/**
 * @brief Processes a packet with the specified color and radius.
 *
 * @param color The color of the packet.
 * @param radius The radius of the packet.
 * @return The result of the packet processing.
 */
int process_packet(uint32_t color, int radius);

/**
 * @brief Resets the frame buffer.
 */
void reset_frame();

