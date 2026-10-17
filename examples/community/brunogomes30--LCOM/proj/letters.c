#include "letters.h"

xpm_image_t letters_image;
xpm_image_t numbers_image;

letter_xpm getLetterXpm(char c) {
  letter_xpm l;
  if ('0' <= c && c <= '9') {
    if (c == '0') {
      c = '9' + 1;
    }
    l.letters = &numbers_image;
    l.startX = NUMBER_PIXEL_WIDTH * (c - '1');
    l.width = NUMBER_PIXEL_WIDTH;
    l.height = NUMBER_PIXEL_HEIGHT;
  }
  else if(c == '-'){
      l.letters = &bar_image;
      l.startX = 0;
      l.width = NUMBER_PIXEL_WIDTH;
      l.height = 14;
  }
  else {
    if ('a' <= c && c <= 'z') {
      c -= 'a' - 'A';
      
    }
    l.letters = &letters_image;
    l.startX = LETTER_PIXEL_WIDTH * (c - 'A');
    l.width = LETTER_PIXEL_WIDTH;
    l.height = LETTER_PIXEL_HEIGHT;
  }

  return l;
}

/*
xpm_char * get_char_xpm(char c){
    xpm_char *ch = (xpm_char * );
    
    ch.xpm = l;
    ch.c = c;
    return ch
}

xpm_string get_string_xpm(char * str){
    letters = xpm_load()
    xpm_string newString;
    newString.first = NULL;
    xpm_char *current = newString.first;
    for(unsigned i = 0; i < str[i] != '\0', i++){
        if(i == 0){
            newString.first = get_char_xpm(str[i]);
            current = newString.first;
        } else {
            current->next = get_char_xpm(str[i]);
            current = current->next;
        }
    }
    return newString;
}
*/
