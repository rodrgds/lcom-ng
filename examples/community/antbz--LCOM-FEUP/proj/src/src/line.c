#include "../headers/line.h"

line_t make_line(uint16_t x_i, uint16_t y_i, uint16_t x_f, uint16_t y_f) {
  uint16_t x = x_i;
  uint16_t y = y_i;
  line_t line;
  if (abs(x_i - x_f) < 15) {
    size_t n_points = abs(y_i - y_f) + 1;
    line.size = n_points * 2;
    line.points = (uint16_t*) malloc(sizeof(uint16_t) * line.size);
    size_t cnt = 0;
    while (cnt < line.size) {
      line.points[cnt] = x;
      cnt++;
      line.points[cnt] = y;
      cnt++;
      if (y_i < y_f) {
        y++;
      } else {
        y--;
      }
    }
  } else if (abs(y_i - y_f) < 15) {
    size_t n_points = abs(x_i - x_f) + 1;
    line.size = n_points * 2;
    line.points = (uint16_t*) malloc(sizeof(uint16_t) * line.size);
    size_t cnt = 0;
    while (cnt < line.size) {
      line.points[cnt] = x;
      cnt++;
      line.points[cnt] = y;
      cnt++;
      if(x_i < x_f) {
        x++;
      } else {
        x--;
      }
    }
  } else {
    size_t n_points = (double)abs(x_i - x_f) / 0.3;
    line.size = n_points * 2;
    double x_alt = (double)x;
    line.points = (uint16_t*) malloc(sizeof(uint16_t) * line.size);
    size_t cnt = 0;
    double slope = ((double)y_f - (double)y_i) / ((double)x_f - (double)x_i);
    double b = (double)y_i - (double)x_i * slope;
    while (cnt < line.size) {
      line.points[cnt] = (uint16_t)x_alt;
      cnt++;
      line.points[cnt] = y;
      cnt++;
      if(x_i < x_f) {
        x_alt += 0.3;
      } else {
        x_alt -= 0.3;
      }
      y = (int)(x_alt * slope + b);
    }
  }
  return line;
}

void destroy_line(line_t* line) {
  if (line->points) {
    free(line->points);
  }
}
