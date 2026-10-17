#include <lcom/lcf.h>

#include "number.h"

/* Contains all the numbers xpms*/
static xpm_image_t numbers[10];

/* Contains the separator xpm */
xpm_image_t separator;

/* From pen_pal.c : Indicates whether the game is being played in dark or light mode */
extern bool dark;


void(load_numbers)() {
  xpm_load(number0_xpm, XPM_8_8_8_8, &numbers[0]);
  xpm_load(number1_xpm, XPM_8_8_8_8, &numbers[1]);
  xpm_load(number2_xpm, XPM_8_8_8_8, &numbers[2]);
  xpm_load(number3_xpm, XPM_8_8_8_8, &numbers[3]);
  xpm_load(number4_xpm, XPM_8_8_8_8, &numbers[4]);
  xpm_load(number5_xpm, XPM_8_8_8_8, &numbers[5]);
  xpm_load(number6_xpm, XPM_8_8_8_8, &numbers[6]);
  xpm_load(number7_xpm, XPM_8_8_8_8, &numbers[7]);
  xpm_load(number8_xpm, XPM_8_8_8_8, &numbers[8]);
  xpm_load(number9_xpm, XPM_8_8_8_8, &numbers[9]);

  xpm_load(separator_xpm, XPM_8_8_8_8, &separator);
}

void(display_time_left)(int time_left) {
  int first_digit_xpos = 1160, second_digit_xpos = 1198, ypos = 47;
  uint32_t color = (dark) ? DARK_MODE_GREY : WHITE;

  /* Clear the position where we want to draw the time left */
  my_vg_draw_rectangle(first_digit_xpos, ypos, 74, 52, color, true);

  /* If between 5 and 10, draw it with a yellow color */
  if (time_left < 10 && time_left > 5) {
    draw_pixmap(first_digit_xpos, ypos, &numbers[0], true);
    draw_pixmap(1198, 47, &numbers[time_left], true);
  }

  /* If lower than 5, draw it red */
  else if (time_left <= 5) {
    draw_pixmap_fixed_color(1160, ypos, &numbers[0], RED, true);
    draw_pixmap_fixed_color(1198, ypos, &numbers[time_left], RED, true);
  }

  /* Else, draw it green */
  else {
    int digit_1 = time_left/10, digit_2 = time_left % 10;
    draw_pixmap(first_digit_xpos, ypos, &numbers[digit_1], true);
    draw_pixmap(second_digit_xpos, ypos, &numbers[digit_2], true);
  }
}

void(draw_number)(int number, int xpos, int ypos, uint32_t color){
  int digit_1, digit_2;

  if (number >= 10){
    digit_1 = number / 10;
    digit_2 = number % 10;
    draw_pixmap_fixed_color(xpos, ypos, &numbers[digit_1], color, true);
    draw_pixmap_fixed_color(xpos + 37, ypos, &numbers[digit_2], color, true);
  }

  else
    draw_pixmap_fixed_color(xpos, ypos, &numbers[number], color, true);
}

int (print_time)(Time time){
  /* If dark mode is toggle, background color is grey, otherwise, white */
  uint32_t bg_color = (dark) ? DARK_MODE_GREY : WHITE;

  /* If dark mode is toggle, elements have to be drawn in white, otherwise, black */
  uint32_t color = (dark) ? WHITE : BLACK;

  int digit_1, digit_2;

  /* Hours */
  my_vg_draw_rectangle(1000, 950, 280, 52, bg_color, true);

  digit_1 = time.hours / 10;
  digit_2 = time.hours % 10;

  draw_pixmap_fixed_color(1000, 950, &numbers[digit_1], color, true);
  draw_pixmap_fixed_color(1038, 950, &numbers[digit_2], color, true);

  draw_pixmap_fixed_color(1075, 960, &separator, color, true);

  /* Minutes */
  digit_1 = time.minutes / 10;
  digit_2 = time.minutes % 10;

  draw_pixmap_fixed_color(1100, 950, &numbers[digit_1], color, true);
  draw_pixmap_fixed_color(1138, 950, &numbers[digit_2], color, true);

  draw_pixmap_fixed_color(1175, 960, &separator, color, true);

  /* Seconds */
  digit_1 = time.seconds / 10;
  digit_2 = time.seconds % 10;

  draw_pixmap_fixed_color(1200, 950, &numbers[digit_1], color, true);
  draw_pixmap_fixed_color(1238, 950, &numbers[digit_2], color, true);

  return 0;
}

