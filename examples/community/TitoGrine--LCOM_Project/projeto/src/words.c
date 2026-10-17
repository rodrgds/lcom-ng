#include "words.h"
#include "alphabet.h"
#include "sprite.h"

/* Size of the word_bank array */
const unsigned int BANK_SIZE = 5;
/* Array that contains all the words used in the game */
static char* word_bank[] = {
  "apple",
  "book",
  "pencil",
  "table",
  "pen"
};


char* get_random_word() {

  int r = rand() % BANK_SIZE;

  return word_bank[r];
}


/* Array of xpms, used to associate the ASCII of a char to its corresponding xpm */
static const char** xpm[] = {
/*   0 */ NULL, // 
/*   1 */ NULL, //  
/*   2 */ NULL, //  
/*   3 */ NULL, //  
/*   4 */ NULL, //  
/*   5 */ NULL, //  
/*   6 */ NULL, //  
/*   7 */ NULL, //  
/*   8 */ NULL, //  
/*   9 */ NULL, // 	 
/*  10 */ NULL, // 
/*  11 */ NULL, //  
/*  12 */ NULL, //  
/*  13 */ NULL, // 
/*  14 */ NULL, //  
/*  15 */ NULL, //  
/*  16 */ NULL, //  
/*  17 */ NULL, //  
/*  18 */ NULL, //  
/*  19 */ NULL, //  
/*  20 */ NULL, //  
/*  21 */ NULL, // 
/*  22 */ NULL, // 
/*  23 */ NULL, // 
/*  24 */ NULL, // 
/*  25 */ NULL, // 
/*  26 */ NULL, // 
/*  27 */ NULL, // 
/*  28 */ NULL, // 
/*  29 */ NULL, // 
/*  30 */ NULL, // 
/*  31 */ NULL, // 
/*  32 */ space,            // (space)
/*  33 */ exclamation_mark, // ! 
/*  34 */ quotation_mark_1, // " 
/*  35 */ NULL, // # 
/*  36 */ NULL, // $ 
/*  37 */ NULL, // % 
/*  38 */ NULL, // & 
/*  39 */ apostrophe,       // ' 
/*  40 */ NULL, // ( 
/*  41 */ NULL, // ) 
/*  42 */ NULL, // * 
/*  43 */ plus,             // + 
/*  44 */ comma,            // , 
/*  45 */ hyphen,           // - 
/*  46 */ full_stop,        // . 
/*  47 */ slash,            // /

/*  48 */ zero_number,      // 0 
/*  49 */ one_number,       // 1 
/*  50 */ two_number,       // 2 
/*  51 */ three_number,     // 3 
/*  52 */ four_number,      // 4 
/*  53 */ five_number,      // 5 
/*  54 */ six_nummber,      // 6 
/*  55 */ seven_number,     // 7 
/*  56 */ eight_number,     // 8 
/*  57 */ nine_number,      // 9 

/*  58 */ colon,            // : 
/*  59 */ semicolon,        // ; 
/*  60 */ NULL, // < 
/*  61 */ equal,            // = 
/*  62 */ NULL, // > 
/*  63 */ question_mark,    // ? 
/*  64 */ NULL, // @ 

/*  65 */ A_letter, // A 
/*  66 */ B_letter, // B 
/*  67 */ C_letter, // C 
/*  68 */ D_letter, // D 
/*  69 */ E_letter, // E 
/*  70 */ F_letter, // F 
/*  71 */ G_letter, // G 
/*  72 */ H_letter, // H 
/*  73 */ I_letter, // I 
/*  74 */ J_letter, // J 
/*  75 */ K_letter, // K 
/*  76 */ L_letter, // L 
/*  77 */ M_letter, // M 
/*  78 */ N_letter, // N 
/*  79 */ O_letter, // O 
/*  80 */ P_letter, // P
/*  81 */ Q_letter, // Q 
/*  82 */ R_letter, // R 
/*  83 */ S_letter, // S 
/*  84 */ T_letter, // T 
/*  85 */ U_letter, // U 
/*  86 */ V_letter, // V 
/*  87 */ W_letter, // W 
/*  88 */ X_letter, // X 
/*  89 */ Y_letter, // Y 
/*  90 */ Z_letter, // Z 

/*  91 */ NULL, // [ 
/*  92 */ NULL, // (backslach) 
/*  93 */ NULL, // ] 
/*  94 */ NULL, // ^ 
/*  95 */ underscore, // _ 
/*  96 */ NULL, // ` 

/*  97 */ a_letter, // a 
/*  98 */ b_letter, // b 
/*  99 */ c_letter, // c 
/* 100 */ d_letter, // d 
/* 101 */ e_letter, // e 
/* 102 */ f_letter, // f 
/* 103 */ g_letter, // g
/* 104 */ h_letter, // h 
/* 105 */ i_letter, // i 
/* 106 */ j_letter, // j 
/* 107 */ k_letter, // k 
/* 108 */ l_letter, // l 
/* 109 */ m_letter, // m 
/* 110 */ n_letter, // n 
/* 111 */ o_letter, // o 
/* 112 */ p_letter, // p 
/* 113 */ q_letter, // q 
/* 114 */ r_letter, // r 
/* 115 */ s_letter, // s 
/* 116 */ t_letter, // t 
/* 117 */ u_letter, // u 
/* 118 */ v_letter, // v 
/* 119 */ w_letter, // w 
/* 120 */ x_letter, // x 
/* 121 */ y_letter, // y 
/* 122 */ z_letter, // z 

/* 123 */ NULL, // { 
/* 124 */ NULL, // | 
/* 125 */ NULL, // } 
/* 126 */ NULL, // ~ 
/* 127 */ NULL  // 
};



void draw_string(const char string[], uint16_t x_pos, uint16_t y_pos) {
  
  unsigned int i = 0;
  int index;            // char ASCII code
  Sprite* letter;

  // while the end of the string is not reached
  while ( (index = string[i]) != 0 ) {
    // creates a sprite for the char in the position i of the string
    letter = create_sprite(xpm[index], x_pos, y_pos, 0, 0);
    draw_Sprite(letter);

    // increments the x coordinate
    x_pos += 2 + letter->width;

    // destroys the sprite to avoid excessive memory usage
    destroy_sprite(letter);
    
    i++;
  }
}

// resulting string of the write_string funtion
char resulting_string[20] = { '\0' };
// flag that assinalates when the string is complete
bool complete_string = false;
int str_index = 0;

void write_string(char new_char, uint16_t x_pos, uint16_t y_pos, int background_color) {

  static uint16_t x, y;
  static int color;

  // if it is the beggining of the string ( first char that will be written )
  if ( str_index == 0 ) {
    x = x_pos;
    y = y_pos;
    color = background_color;
  }

  // if the string is complete
  if ( complete_string ) {
    complete_string = false;      
    resulting_string[0] = '\0';   // set the first element to char 0
    str_index = 0;
    return;
  }

  // maximum length reached
  if (str_index > 18) {
    if (new_char == '\b') {
        str_index--;
        int index = resulting_string[str_index];                        // get the ASCII code of the previous char
        Sprite *letter = create_sprite(xpm[index], x, y, 0, 0);         // creates the sprite to see the dimensions of it
        x -= (2 + letter->width);
        vg_draw_rectangle(x, y, letter->width, letter->height, color);  // draw a rectangle on top of the displayed char sprite
        resulting_string[str_index] = '\0';
        destroy_sprite(letter);
    }
    else if ( new_char == '\n' ) {
      complete_string = true;
    }
    return;
  }

  if ( new_char == '\n' ) {
    complete_string = true;
  }
  else if ( new_char == '\b' ) {
    if ( str_index > 0 ) {
      str_index--;
      int index = resulting_string[str_index];
      Sprite* letter = create_sprite(xpm[index], x, y, 0, 0);
      x -= (2 + letter->width);
      vg_draw_rectangle(x, y, letter->width, letter->height, color);
      resulting_string[str_index] = '\0';
      destroy_sprite(letter);
    }
  }
  else if ( new_char == '\0' ) {
    // do nothing
    // this is a case because some of the keys that are not mapped will return char 0
  }
  else {
    resulting_string[str_index] = new_char;
    resulting_string[str_index + 1] = '\0';   // in case ther was trash from previous usage

    int index = new_char;
    Sprite* letter = create_sprite(xpm[index], x, y, 0, 0); // creates the sprite for the char
    x += 2 + letter->width;
    draw_Sprite(letter);
    destroy_sprite(letter);
    str_index++;
  }

  double_buffer();
}

void clear_string() {
  complete_string = false;
  resulting_string[0] = '\0';
  str_index = 0;
}


/* Three random words that are display to the player so he can choose */
char* words[3];
/* One of the words from the array above, chosen by the player */
char* chosen_word;


void get_words() {
  words[0] = get_random_word();

  unsigned int index = 1;

  while ( index < 3 ) {
    
    char* tmp_word = get_random_word(); 
    bool dif_flag = true;

    for ( unsigned int i = 0; i < index; i++) {

      if ( strcmp( tmp_word , words[i] ) == 0 ) {
        dif_flag = false;
        break;
      }

    }

    if ( dif_flag ) {
      words[index] = tmp_word;
      index++;
    }
  }

  
}

void set_word(char* string) {
  chosen_word = string;
}

