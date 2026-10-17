#include "base_frame.h"

frame_buffer_t frame_buffer;
uint16_t _width;
uint16_t _height;
uint8_t _bytes_per_pixel;
unsigned _size;
uint8_t *_base_addr;

extern Queue *pos_queue;

void create_frame_buffer(uint16_t width, uint16_t height, uint16_t bytes_per_pixel) {
  frame_buffer.width = width;
  frame_buffer.height = height;
  frame_buffer.bytes_per_pixel = bytes_per_pixel;
  frame_buffer.size = width * height * bytes_per_pixel;
  frame_buffer.base_addr = (uint8_t *)malloc(frame_buffer.size);
  memset(frame_buffer.base_addr, WHITE, frame_buffer.size);
}

void draw_frame_pixel(uint16_t x, uint16_t y, uint32_t color) {
  if (frame_buffer.base_addr == NULL) return;

  if (x >= frame_buffer.width || x < 0 || y >= frame_buffer.height || y < 0) return;

  int pixel_pos = frame_buffer.width * y + x;
  int byte_offset = pixel_pos * frame_buffer.bytes_per_pixel;

  if (memcpy(
    &frame_buffer.base_addr[byte_offset], 
    &color, 
    (unsigned) frame_buffer.bytes_per_pixel) == NULL)
      return;
}

void draw_frame_circle(Position p, uint16_t thickness, uint32_t color) {
  for (int i = -thickness/2; i <= thickness/2; i++) {
    for (int j = -thickness/2; j <= thickness/2; j++) {
      draw_frame_pixel(p.x + i, p.y + j, color);
    }
  }
}

void draw_bresenham_line(Position p1, Position p2, uint32_t color, uint16_t thickness) {
  int dx = abs(p2.x - p1.x);
  int dy = abs(p2.y - p1.y);
  int sx = (p1.x < p2.x) ? 1 : -1;
  int sy = (p1.y < p2.y) ? 1 : -1;
  int error = dx - dy;

  while (1) {
    for (int i = -thickness/2; i <= thickness/2; i++) {
      for (int j = -thickness/2; j <= thickness/2; j++) {
        draw_frame_pixel(p1.x + i, p1.y + j, color);
      }
    }

    if (p1.x == p2.x && p1.y == p2.y) {
      break;
    }

    int error2 = 2 * error;

    if (error2 > -dy) {
      error -= dy;
      p1.x += sx;
    }

    if (error2 < dx) {
      error += dx;
      p1.y += sy;
    }
  }
}

int process_packet(uint32_t color, int radius) {
  int size = queue_size(&pos_queue);
  if (size == 0) return 1;
  struct Position position1 = *((Position *)queue_front(&pos_queue));
  if (size == 1) {
    draw_frame_circle(position1, radius, color);
    return 1;
  } else {
    queue_pop(&pos_queue);
    struct Position position2 = *((Position *) queue_front(&pos_queue));
    draw_bresenham_line(position1, position2, color, radius);
  }
  return 0;
}

void reset_frame() {
  memset(frame_buffer.base_addr, WHITE, frame_buffer.size);
}
