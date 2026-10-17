#include <lcom/lcf.h>

#include <stdint.h>
#include "utils.h"
#include "graphics.h"

#ifdef LAB3
extern int sys_inb_count;
#endif

// @brief gets 16 bits and grabs the least significant byte and puts it into lsb
// @param val original 2 byte value that we want to shorted to only the LSB
// @param *lsb memory space where the 1 byte we achieved from the val will be stored
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {

  val = val & 0x00ff;  // mask zero's msb leaving only the 8 lsbits
  *lsb = (uint8_t) val; // tranforms 16 to 8 grabbing only the 8 lsb bits
  return 0;

}

// @brief gets 16 bits and grabs the most significant byte and puts it into msb
// @param val original 2 byte value that we want to shorted to only the MSB
// @param *msb memory space where the 1 byte we achieved from the val will be stored
int(util_get_MSB)(uint16_t val, uint8_t *msb) {

    val = (val >> 8) & 0x00ff; // shifts msb to lsb  and masks makes sure that everything behind is 0
    *msb = (uint8_t) val;      // tranforms 16 to 8 grabbing only the 8 lsb bits
    return 0;

}

//function that takes the 32 bit output from sys_inb() and casts it to 8 bits
// sys_inb read information from the port
// @port is where we real the data from
// @value, outside variable that gets updated with the 8 bit output of sys_inb
int (util_sys_inb)(int port, uint8_t *value) {
  #ifdef LAB3
      sys_inb_count++;
  #endif
  uint32_t v;
  int response = sys_inb(port, &v);
  *value = (uint8_t) v;
  return response;

}


Object newObject(int x, int y, xpm_image_t sprite, enum object_tag object_tag)
{
    Object object;
    object.x = x;
    object.y = y;
    object.width = sprite.width;
    object.height = sprite.height;
    object.sprite = sprite;
    object.object_tag = object_tag;
    object.active = true;
    return object;
}

bool (colides)(Object o1, Object o2)
{
  if(!o1.active || !o2.active) return false;
  
  if(o2.x < o1.x && o1.x < o2.x + o2.width && o1.y + o1.height > o2.y && o1.y + o1.height < o2.y + o2.height) 
    return true; 
  if(o1.x + o1.width > o2.x && o1.x + o1.width < o2.x + o2.width && o1.y + o1.height > o2.y && o1.y + o1.height < o2.y + o2.height)
    return true;
  return false;
}

xpm_map_t getNumberXPM(int n)
{
  if(n == 0) return ZERO;
  if(n == 1) return ONE;
  if(n == 2) return TWO;
  if(n == 3) return THREE;
  if(n == 4) return FOUR;
  if(n == 5) return FIVE;
  if(n == 6) return SIX;
  if(n == 7) return SEVEN;
  if(n == 8) return EIGHT;
  if(n == 9) return NINE;
  return NULL; 
}

xpm_map_t getLetterXPM(char c)
{
  if(c == 'a' || c == 'A') return LETTER_A;
  if(c == 'b' || c == 'B') return LETTER_B;
  if(c == 'c' || c == 'C') return LETTER_C;
  if(c == 'd' || c == 'D') return LETTER_D;
  if(c == 'e' || c == 'E') return LETTER_E;
  if(c == 'f' || c == 'F') return LETTER_F;
  if(c == 'g' || c == 'G') return LETTER_G;
  if(c == 'h' || c == 'H') return LETTER_H;
  if(c == 'i' || c == 'I') return LETTER_I;
  if(c == 'j' || c == 'J') return LETTER_J;
  if(c == 'k' || c == 'K') return LETTER_K;
  if(c == 'l' || c == 'L') return LETTER_L;
  if(c == 'm' || c == 'M') return LETTER_M;
  if(c == 'n' || c == 'N') return LETTER_N;
  if(c == 'o' || c == 'O') return LETTER_O;
  if(c == 'p' || c == 'P') return LETTER_P;
  if(c == 'q' || c == 'Q') return LETTER_Q;
  if(c == 'r' || c == 'R') return LETTER_R;
  if(c == 's' || c == 'S') return LETTER_S;
  if(c == 't' || c == 'T') return LETTER_T;
  if(c == 'u' || c == 'U') return LETTER_U;
  if(c == 'v' || c == 'V') return LETTER_V;
  if(c == 'w' || c == 'W') return LETTER_W;
  if(c == 'x' || c == 'X') return LETTER_X;
  if(c == 'y' || c == 'Y') return LETTER_Y;
  if(c == 'z' || c == 'Z') return LETTER_Z;
  if(c == '0') return ZERO;
  if(c == '1') return ONE;
  if(c == '2') return TWO;
  if(c == '3') return THREE;
  if(c == '4') return FOUR;
  if(c == '5') return FIVE;
  if(c == '6') return SIX;
  if(c == '7') return SEVEN;
  if(c == '8') return EIGHT;
  if(c == '9') return NINE;
  if(c == '-') return LETTER_TRACE;
  if(c == ':') return LETTER_COLUMN;
  if(c == '/') return LETTER_DASH;
  if(c == '!') return LETTER_EXCLAMATION;
  return NULL; 
}

char getCodeChar(uint8_t code)
{
  if(code == 0x9e) return 'a';
  if(code == 0xb0) return 'b';
  if(code == 0xae) return 'c';
  if(code == 0xa0) return 'd';
  if(code == 0x92) return 'e';
  if(code == 0xa1) return 'f';
  if(code == 0xa2) return 'g';
  if(code == 0xa3) return 'h';
  if(code == 0x97) return 'i';
  if(code == 0xa4) return 'j';
  if(code == 0xa5) return 'k';
  if(code == 0xa6) return 'l';
  if(code == 0xb2) return 'm';
  if(code == 0xb1) return 'n';
  if(code == 0x98) return 'o';
  if(code == 0x99) return 'p';
  if(code == 0x90) return 'q';
  if(code == 0x93) return 'r';
  if(code == 0x9f) return 's';
  if(code == 0x94) return 't';
  if(code == 0x96) return 'u';
  if(code == 0xaf) return 'v';
  if(code == 0x91) return 'w';
  if(code == 0xad) return 'x';
  if(code == 0x95) return 'y';
  if(code == 0xac) return 'z';
  if(code == 0x8e) return '<';
  if(code == 0xb9) return ' ';
  return '-';
}

void draw_text_from_keyboard(char* str, uint8_t data, int* size, int x, int y)
{
  char c = getCodeChar(data);
  if(c == '<' && *size > 0)
  {
    str[*size] = '\0';
    (*size)--;
    vg_draw_rectangle(x+(LETTERS_WIDTH+5)*(*size),y,LETTERS_WIDTH,10,0);
  } 
  else if(((c >= 'a' && c <= 'z') || c == ' ') && *size <= 99)
  {
    str[*size] = c;
    str[++(*size)] = '\0';
    draw_text(str, x, y);
  }
  draw_frame();
}

void change_game_state(enum game_state* ost, enum game_state nst)
{
    fill_bckgnd();
    draw_frame();
    *ost = nst;
}

///Top scores array
int highscores[SCORE_BOARD_SIZE];
///Top score player name array
char names[SCORE_BOARD_SIZE][255];

void swap_i(int *xp, int *yp)  
{  
    int temp = *xp;  
    *xp = *yp;  
    *yp = temp;  
}

void swap_c(char *xp, char *yp) 
{  
  char tmp[255];
  strcpy(tmp,xp);
  strcpy(xp,yp);
  strcpy(yp,tmp);
}  

void sort_scores(char arr2[SCORE_BOARD_SIZE][255],int arr[SCORE_BOARD_SIZE])  
{  
    int i, j;  
    for (i = 0; i < SCORE_BOARD_SIZE-1; i++)      
    for (j = 0; j < SCORE_BOARD_SIZE-i-1; j++)  
        if (arr[j] < arr[j+1])  
        {
            swap_i(&arr[j], &arr[j+1]);
            swap_c(arr2[j], arr2[j+1]);
        }
}

void load_scores()
{
    if( access("/home/lcom/labs/proj/scores", F_OK ) == -1 )
    {
      FILE *fp;
        fp = fopen("/home/lcom/labs/proj/scores", "w");
        fclose(fp);
    }


    FILE *fp;

      fp = fopen("/home/lcom/labs/proj/scores", "r");
      if(fp != NULL)
      {
        int i = 0;
        while (fgets(names[i], 1024, fp)) 
        {
            if(strlen(names[i]) == 1) continue;
            int val;
            fscanf(fp,"%d",&val);
          highscores[i] = val;
            i++;
        }
      sort_scores(names,highscores);
    }
      fclose(fp);
}
