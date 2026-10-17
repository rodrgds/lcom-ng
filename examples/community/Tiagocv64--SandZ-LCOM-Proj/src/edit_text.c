#include <lcom/lcf.h>
#include "libs/video.h"
#include "libs/i8042.h"
#include "Bitmap.h"
#include "enum.h"
#include "edit_text.h"
#include "button.h"
#include "text.h"

Bitmap* edit_up;
Bitmap* edit_down;

extern int x_mouse;
extern int y_mouse;
extern bool left_press_mouse;

bool convert_to_char(char* c, uint8_t d){
  switch(d){
    case A:
      *c = 'A';
      break;
    case B:
      *c = 'B';
      break;
    case C:
      *c = 'C';
      break;
    case D:
      *c = 'D';
      break;
    case E:
      *c = 'E';
      break;
    case F:
      *c = 'F';
      break;
    case G:
      *c = 'G';
      break;
    case H:
      *c = 'H';
      break;
    case I:
      *c = 'I';
      break;
    case J:
      *c = 'J';
      break;
    case K:
      *c = 'K';
      break;
    case L:
      *c = 'L';
      break;
    case M:
      *c = 'M';
      break;
    case N:
      *c = 'N';
      break;
    case O:
      *c = 'O';
      break;
    case P:
      *c = 'P';
      break;
    case Q:
      *c = 'Q';
      break;
    case R:
      *c = 'R';
      break;
    case S:
      *c = 'S';
      break;
    case T:
      *c = 'T';
      break;
    case U:
      *c = 'U';
      break;
    case V:
      *c = 'V';
      break;
    case W:
      *c = 'W';
      break;
    case X:
      *c = 'X';
      break;
    case Y:
      *c = 'Y';
      break;
    case Z:
      *c = 'Z';
      break;
    default:
      return false;
  }
  return true;
}

void init_edittext(){
  edit_up = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/button_up.bmp");
  edit_down = loadBitmap("/home/lcom/labs/lcom1819-t4g13/proj/assets/button_down.bmp");
}

void destroy_edittext(){
  deleteBitmap(edit_up);
  deleteBitmap(edit_down);
}

struct EditText create_edittext(int x, int y, bool focused){
  struct EditText edt;
  edt.x = x;
  edt.y = y;
  edt.t_size = 0;
  edt.mouseOver = false;
  edt.focused = focused;
  edt.under_draw = false;
  edt.under_count = 0;
  return edt;
}

void receive_input(struct EditText* edt, uint8_t d){
  if (edt->focused){
    if (d == BACKSPACE){
      edt->t_size = (edt->t_size > 0)? edt->t_size-1 : 0;
      return;
    }
    if (edt->t_size >= 25)
      return;
    char input;
    if (convert_to_char(&input, d))
      edt->text[edt->t_size++] = input;
  }
}

void update_edittext(struct EditText* edt){
  if (edt->x - 150 < x_mouse && edt->x + 150 > x_mouse && edt->y - 50 < y_mouse && edt->y + 50 > y_mouse)
    edt->mouseOver = true;
  else
    edt->mouseOver = false;

  if (edt->under_count++ > 30){
    edt->under_count = 0;
    edt->under_draw = !edt->under_draw;
  }

  if (left_press_mouse){
    if (edt->mouseOver)
      edt->focused = true;
    else
      edt->focused = false;
  }
}

void draw_edittext(struct EditText edt){
  if (edt.focused){
    drawBitmap(edit_down, edt.x - 150, edt.y - 50);
  } else {
    drawBitmap(edit_up, edt.x - 150, edt.y - 50);
  }
  char tmp[26];
  if (edt.under_draw && edt.focused){
    sprintf(tmp, "%s", edt.text);
    tmp[edt.t_size] = '_';
    if (edt.t_size > 10){
      print_string(&tmp[edt.t_size-10], 11, edt.y - 25, edt.x - 130, WHITE, MEDIUM, ALIGN_LEFT);
    } else {
      print_string(tmp, edt.t_size+1, edt.y - 25, edt.x - 130, WHITE, MEDIUM, ALIGN_LEFT);
    }
  } else {
    sprintf(tmp, "%s", edt.text);
    if (edt.t_size > 10){
      print_string(&tmp[edt.t_size-10], 10, edt.y - 25, edt.x - 130, WHITE, MEDIUM, ALIGN_LEFT);
    } else {
      print_string(tmp, edt.t_size, edt.y - 25, edt.x - 130, WHITE, MEDIUM, ALIGN_LEFT);
    }
  }
}
