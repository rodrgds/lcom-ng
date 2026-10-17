#include <lcom/lcf.h>

#include "CodeToCharacter.h"


/* Contains all alphabet letters and corresponding break codes */
CodeToCharacter *list[26];

CodeToCharacter *new_pair(char c, uint8_t code) {
  CodeToCharacter *ctc = (CodeToCharacter *) malloc(sizeof(CodeToCharacter));

  ctc->character = c;
  ctc->break_code = code;

  return ctc;
}

void(fill_code_list)() {
  printf("\n");
  for (int i = 0; i < 26; i++) {
    CodeToCharacter *ctc = new_pair(i + 'A', get_code_from_character(i + 'A'));
    list[i] = ctc;
  }
}

unsigned char(get_code_from_character)(char c) {
  switch (c) {
    case 'A': return 0x9E;
    case 'B': return 0xB0;
    case 'C': return 0xAE;
    case 'D': return 0xA0;
    case 'E': return 0x92;
    case 'F': return 0xA1;
    case 'G': return 0xA2;
    case 'H': return 0xA3;
    case 'I': return 0x97;
    case 'J': return 0xA4;
    case 'K': return 0xA5;
    case 'L': return 0xA6;
    case 'M': return 0xB2;
    case 'N': return 0xB1;
    case 'O': return 0x98;
    case 'P': return 0x99;
    case 'Q': return 0x90;
    case 'R': return 0x93;
    case 'S': return 0x9F;
    case 'T': return 0x94;
    case 'U': return 0x96;
    case 'V': return 0xAF;
    case 'W': return 0x91;
    case 'X': return 0xAD;
    case 'Y': return 0x95;
    case 'Z': return 0xAC;
    default: break;
  }
  return 0x00;
}

char(get_character_from_code)(unsigned char code) {

  for (int i = 0; i < 26; i++) {
    if (list[i]->break_code == code)
      return list[i]->character;
  }

  return '\0';
}
