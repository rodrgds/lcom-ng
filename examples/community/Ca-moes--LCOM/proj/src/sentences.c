#include <lcom/lcf.h>
#include "sentences.h"
#include <limits.h>

void loadLetters(){

  abc = (letter*)malloc(nLetters*sizeof(letter));
  numbers = (number*)malloc(12*sizeof(number));

  enum xpm_image_type type= XPM_5_6_5;

  numbers[0].number = '0';
  numbers[0].sprite = xpm_load(xpm_0,type,&numbers[0].img);

  numbers[1].number = '1';
  numbers[1].sprite = xpm_load(xpm_1,type,&numbers[1].img);

  numbers[2].number = '2';
  numbers[2].sprite = xpm_load(xpm_2,type,&numbers[2].img);

  numbers[3].number = '3';
  numbers[3].sprite = xpm_load(xpm_3,type,&numbers[3].img);

  numbers[4].number = '4';
  numbers[4].sprite = xpm_load(xpm_4,type,&numbers[4].img);

  numbers[5].number = '5';
  numbers[5].sprite = xpm_load(xpm_5,type,&numbers[5].img);

  numbers[6].number = '6';
  numbers[6].sprite = xpm_load(xpm_6,type,&numbers[6].img);

  numbers[7].number = '7';
  numbers[7].sprite = xpm_load(xpm_7,type,&numbers[7].img);

  numbers[8].number = '8';
  numbers[8].sprite = xpm_load(xpm_8,type,&numbers[8].img);

  numbers[9].number = '9';
  numbers[9].sprite = xpm_load(xpm_9,type,&numbers[9].img);

  numbers[10].number = ':';
  numbers[10].sprite = xpm_load(TwoDots,type,&numbers[10].img);

  numbers[11].number = '/';
  numbers[11].sprite = xpm_load(bar,type,&numbers[11].img);

  abc[0].letter = 'a';
  abc[0].capital = false;
  abc[0].sprite = xpm_load(a,type,&abc[0].img);
  abc[0].makeCode = 0x1E;
  abc[0].breakCode = 0x9E;

  abc[1].letter = 'b';
  abc[1].capital = false;
  abc[1].sprite = xpm_load(b,type,&abc[1].img);
  abc[1].makeCode = 0x30;
  abc[1].breakCode = 0xB0;
  
  abc[2].letter = 'c';
  abc[2].capital = false;
  abc[2].sprite = xpm_load(c,type,&abc[2].img);
  abc[2].makeCode = 0x2E;
  abc[2].breakCode = 0xAE;
  
  abc[3].letter = 'd';
  abc[3].capital = false;
  abc[3].sprite = xpm_load(d,type,&abc[3].img);
  abc[3].makeCode = 0x20;
  abc[3].breakCode = 0xA0;

  abc[4].letter = 'e';
  abc[4].capital = false;
  abc[4].sprite = xpm_load(e,type,&abc[4].img);
  abc[4].makeCode = 0x12;
  abc[4].breakCode = 0x92;

  abc[5].letter = 'f';
  abc[5].capital = false;
  abc[5].sprite = xpm_load(f,type,&abc[5].img);
  abc[5].makeCode = 0x21;
  abc[5].breakCode = 0xA1;

  abc[6].letter = 'g';
  abc[6].capital = false;
  abc[6].sprite = xpm_load(g,type,&abc[6].img);
  abc[6].makeCode = 0x22;
  abc[6].breakCode = 0xA2;

  abc[7].letter = 'h';
  abc[7].capital = false;
  abc[7].sprite = xpm_load(h,type,&abc[7].img);
  abc[7].makeCode = 0x23;
  abc[7].breakCode = 0xA3;
  
  abc[8].letter = 'i';
  abc[8].capital = false;
  abc[8].sprite = xpm_load(i,type,&abc[8].img);
  abc[8].makeCode = 0x17;
  abc[8].breakCode = 0x97;

  abc[9].letter = 'j';
  abc[9].capital = false;
  abc[9].sprite = xpm_load(j,type,&abc[9].img);
  abc[9].makeCode = 0x24;
  abc[9].breakCode = 0xA4;
  
  abc[10].letter = 'k';
  abc[10].capital = false;
  abc[10].sprite = xpm_load(k,type,&abc[10].img);
  abc[10].makeCode = 0x25;
  abc[10].breakCode = 0xA5;

  abc[11].letter = 'l';
  abc[11].capital = false;
  abc[11].sprite = xpm_load(l,type,&abc[11].img);
  abc[11].makeCode = 0x26;
  abc[11].breakCode = 0xA6;

  abc[12].letter = 'm';
  abc[12].capital = false;
  abc[12].sprite = xpm_load(m,type,&abc[12].img);
  abc[12].makeCode = 0x32;
  abc[12].breakCode = 0xB2;

  abc[13].letter = 'n';
  abc[13].capital = false;
  abc[13].sprite = xpm_load(n,type,&abc[13].img);
  abc[13].makeCode = 0x31;
  abc[13].breakCode = 0xB1;

  abc[14].letter = 'o';
  abc[14].capital = false;
  abc[14].sprite = xpm_load(o,type,&abc[14].img);
  abc[14].makeCode = 0x18;
  abc[14].breakCode = 0x98;

  abc[15].letter = 'p';
  abc[15].capital = false;
  abc[15].sprite = xpm_load(p,type,&abc[15].img);
  abc[15].makeCode = 0x19;
  abc[15].breakCode = 0x99;

  abc[16].letter = 'q';
  abc[16].capital = false;
  abc[16].sprite = xpm_load(q,type,&abc[16].img);
  abc[16].makeCode = 0x10;
  abc[16].breakCode = 0x90;

  abc[17].letter = 'r';
  abc[17].capital = false;
  abc[17].sprite = xpm_load(r,type,&abc[17].img);
  abc[17].makeCode = 0x13;
  abc[17].breakCode = 0x93;

  abc[18].letter = 's';
  abc[18].capital = false;
  abc[18].sprite = xpm_load(s,type,&abc[18].img);
  abc[18].makeCode = 0x1F;
  abc[18].breakCode = 0x9F;

  abc[19].letter = 't';
  abc[19].capital = false;
  abc[19].sprite = xpm_load(t,type,&abc[19].img);
  abc[19].makeCode = 0x14;
  abc[19].breakCode = 0x94;

  abc[20].letter = 'u';
  abc[20].capital = false;
  abc[20].sprite = xpm_load(u,type,&abc[20].img);
  abc[20].makeCode = 0x16;
  abc[20].breakCode = 0x96;

  abc[21].letter = 'v';
  abc[21].capital = false;
  abc[21].sprite = xpm_load(v,type,&abc[21].img);
  abc[21].makeCode = 0x2F;
  abc[21].breakCode = 0xAF;

  abc[22].letter = 'w';
  abc[22].capital = false;
  abc[22].sprite = xpm_load(w,type,&abc[22].img);
  abc[22].makeCode = 0x11;
  abc[22].breakCode = 0x91;

  abc[23].letter = 'x';
  abc[23].capital = false;
  abc[23].sprite = xpm_load(x,type,&abc[23].img);
  abc[23].makeCode = 0x2D;
  abc[23].breakCode = 0xAD;

  abc[24].letter = 'y';
  abc[24].capital = false;
  abc[24].sprite = xpm_load(y,type,&abc[24].img);
  abc[24].makeCode = 0x15;
  abc[24].breakCode = 0x95;

  abc[25].letter = 'z';
  abc[25].capital = false;
  abc[25].sprite = xpm_load(z,type,&abc[25].img);
  abc[25].makeCode = 0x2C;
  abc[25].breakCode = 0xAC;
  
  // Capital letters

  abc[26].letter = 'A';
  abc[26].capital = true;
  abc[26].sprite = xpm_load(A,type,&abc[26].img);
  abc[26].makeCode = 0x1E;
  abc[26].breakCode = 0x9E;

  abc[27].letter = 'B';
  abc[27].capital = true;
  abc[27].sprite = xpm_load(B,type,&abc[27].img);
  abc[27].makeCode = 0x30;
  abc[27].breakCode = 0xB0;
  
  abc[28].letter = 'C';
  abc[28].capital = true;
  abc[28].sprite = xpm_load(C,type,&abc[28].img);
  abc[28].makeCode = 0x2E;
  abc[28].breakCode = 0xAE;
  
  abc[29].letter = 'D';
  abc[29].capital = true;
  abc[29].sprite = xpm_load(D,type,&abc[29].img);
  abc[29].makeCode = 0x20;
  abc[29].breakCode = 0xA0;

  abc[30].letter = 'E';
  abc[30].capital = true;
  abc[30].sprite = xpm_load(E,type,&abc[30].img);
  abc[30].makeCode = 0x12;
  abc[30].breakCode = 0x92;

  abc[31].letter = 'F';
  abc[31].capital = true;
  abc[31].sprite = xpm_load(F,type,&abc[31].img);
  abc[31].makeCode = 0x21;
  abc[31].breakCode = 0xA1;

  abc[32].letter = 'G';
  abc[32].capital = true;
  abc[32].sprite = xpm_load(G,type,&abc[32].img);
  abc[32].makeCode = 0x22;
  abc[32].breakCode = 0xA2;

  abc[33].letter = 'H';
  abc[33].capital = true;
  abc[33].sprite = xpm_load(H,type,&abc[33].img);
  abc[33].makeCode = 0x23;
  abc[33].breakCode = 0xA3;
  
  abc[34].letter = 'I';
  abc[34].capital = true;
  abc[34].sprite = xpm_load(I,type,&abc[34].img);
  abc[34].makeCode = 0x17;
  abc[34].breakCode = 0x97;

  abc[35].letter = 'J';
  abc[35].capital = true;
  abc[35].sprite = xpm_load(J,type,&abc[35].img);
  abc[35].makeCode = 0x24;
  abc[35].breakCode = 0xA4;
  
  abc[36].letter = 'K';
  abc[36].capital = true;
  abc[36].sprite = xpm_load(K,type,&abc[36].img);
  abc[36].makeCode = 0x25;
  abc[36].breakCode = 0xA5;

  abc[37].letter = 'L';
  abc[37].capital = true;
  abc[37].sprite = xpm_load(L,type,&abc[37].img);
  abc[37].makeCode = 0x26;
  abc[37].breakCode = 0xA6;

  abc[38].letter = 'M';
  abc[38].capital = true;
  abc[38].sprite = xpm_load(M,type,&abc[38].img);
  abc[38].makeCode = 0x32;
  abc[38].breakCode = 0xB2;

  abc[39].letter = 'N';
  abc[39].capital = true;
  abc[39].sprite = xpm_load(N,type,&abc[39].img);
  abc[39].makeCode = 0x31;
  abc[39].breakCode = 0xB1;

  abc[40].letter = 'O';
  abc[40].capital = true;
  abc[40].sprite = xpm_load(O,type,&abc[40].img);
  abc[40].makeCode = 0x18;
  abc[40].breakCode = 0x98;

  abc[41].letter = 'P';
  abc[41].capital = true;
  abc[41].sprite = xpm_load(P,type,&abc[41].img);
  abc[41].makeCode = 0x19;
  abc[41].breakCode = 0x99;

  abc[42].letter = 'Q';
  abc[42].capital = true;
  abc[42].sprite = xpm_load(Q,type,&abc[42].img);
  abc[42].makeCode = 0x10;
  abc[42].breakCode = 0x90;

  abc[43].letter = 'R';
  abc[43].capital = true;
  abc[43].sprite = xpm_load(R,type,&abc[43].img);
  abc[43].makeCode = 0x13;
  abc[43].breakCode = 0x93;

  abc[44].letter = 'S';
  abc[44].capital = true;
  abc[44].sprite = xpm_load(S,type,&abc[44].img);
  abc[44].makeCode = 0x1F;
  abc[44].breakCode = 0x9F;

  abc[45].letter = 'T';
  abc[45].capital = true;
  abc[45].sprite = xpm_load(T,type,&abc[45].img);
  abc[45].makeCode = 0x14;
  abc[45].breakCode = 0x94;

  abc[46].letter = 'U';
  abc[46].capital = true;
  abc[46].sprite = xpm_load(U,type,&abc[46].img);
  abc[46].makeCode = 0x16;
  abc[46].breakCode = 0x96;

  abc[47].letter = 'V';
  abc[47].capital = true;
  abc[47].sprite = xpm_load(V,type,&abc[47].img);
  abc[47].makeCode = 0x2F;
  abc[47].breakCode = 0xAF;

  abc[48].letter = 'W';
  abc[48].capital = true;
  abc[48].sprite = xpm_load(W,type,&abc[48].img);
  abc[48].makeCode = 0x11;
  abc[48].breakCode = 0x91;

  abc[49].letter = 'X';
  abc[49].capital = true;
  abc[49].sprite = xpm_load(X,type,&abc[49].img);
  abc[49].makeCode = 0x2D;
  abc[49].breakCode = 0xAD;

  abc[50].letter = 'Y';
  abc[50].capital = true;
  abc[50].sprite = xpm_load(Y,type,&abc[50].img);
  abc[50].makeCode = 0x15;
  abc[50].breakCode = 0x95;

  abc[51].letter = 'Z';
  abc[51].capital = true;
  abc[51].sprite = xpm_load(Z,type,&abc[51].img);
  abc[51].makeCode = 0x2C;
  abc[51].breakCode = 0xAC;

  // SYMBOLS

  abc[52].letter = '.';
  abc[52].capital = false;
  abc[52].sprite = xpm_load(pontofinal,type,&abc[52].img);
  abc[52].makeCode = 0x34;
  abc[52].breakCode = 0xB4;

  abc[53].letter = ',';
  abc[53].capital = false;
  abc[53].sprite = xpm_load(virgula,type,&abc[53].img);
  abc[53].makeCode = 0x33;
  abc[53].breakCode = 0xB3;

  abc[54].letter = ':';
  abc[54].capital = true;
  abc[54].sprite = xpm_load(doispontos,type,&abc[54].img);
  abc[54].makeCode = 0x34;
  abc[54].breakCode = 0xB4;

  abc[55].letter = ';';
  abc[55].capital = true;
  abc[55].sprite = xpm_load(pontovirg,type,&abc[55].img);
  abc[55].makeCode = 0x33;
  abc[55].breakCode = 0xB3;

  abc[56].letter = '\'';
  abc[56].capital = false;
  abc[56].sprite = xpm_load(apost,type,&abc[56].img);
  abc[56].makeCode = 0x0C;
  abc[56].breakCode = 0x8C;

  abc[57].letter = ' ';
  abc[57].capital = false;
  abc[57].sprite = xpm_load(space,type,&abc[57].img);
  abc[57].makeCode = 0x39;
  abc[57].breakCode = 0xB9;

  abc[58].letter = '0';
  abc[58].capital = false;
  abc[58].sprite = xpm_load(empty,type,&abc[58].img);
  abc[58].makeCode = 0x0E;
  abc[58].breakCode = 0x8E;
}

void loadSentences(){     
  sentences = allocateTwoDimenArrayOnHeapUsingMalloc(nSentences,nChars);
  for(unsigned i = 0; i<nSentences; i++){ //percorre as linhas 
    for(unsigned int j=0; j<strlen(sentencesChar[i]); j++){ // percorre as colunas
      for(unsigned int k =0; k < nLetters; k++){ // percorre as letras
        if(abc[k].letter == sentencesChar[i][j]){ // se encontrar a letra
          sentences[i][j] = abc[k]; // coloca na posição já alocada da memoria
          break;
        }
      }
    }
  }
  errorMessage = (letter*)malloc(50*sizeof(letter));
  for(unsigned int j=0;j<50; j++){
    for(unsigned int k =0; k < nLetters; k++){ // percorre as letras
      if(abc[k].letter == errorMessageChar[j]){ // se encontrar a letra
        errorMessage[j] = abc[k]; // coloca na posição já alocada da memoria
        break;
      }
    }
  }                                                                            
}

letter ** allocateTwoDimenArrayOnHeapUsingMalloc(int row, int col){
	letter ** ptr = (letter**) malloc(sizeof(letter *)*row);
	for(int i = 0; i < row; i++)
	{
		ptr[i] = (letter *) malloc(sizeof(letter)*col);
	}
	return ptr;
}

void destroyTwoDimenArrayOnHeapUsingFree(letter ** ptr, int row, int col){
  for(int i = 0; i < row; i++){
    free(ptr[i]);
  }
  free(ptr);
}

void destroySentencesAndLetters(){
  destroyTwoDimenArrayOnHeapUsingFree(sentences,nSentences,nChars);
  free(abc);
  free(numbers);
}

#include <stdio.h>
#include <stdlib.h>

inline void swap(char *x, char *y) {
	char t = *x; *x = *y; *y = t;
}

char* reverse(char *buffer, int i, int j)
{
	while (i < j)
		swap(&buffer[i++], &buffer[j--]);

	return buffer;
}

char* itoa(int value, int base)
{
  char * buffer = (char*)malloc(sizeof(unsigned int) * CHAR_BIT + 2);
  if (buffer == NULL) return NULL;
  buffer[0] = '\0';

	// invalid input
	if (base < 2 || base > 32)
		return buffer;

	// consider absolute value of number
	unsigned int n = value < 0 ? 0u - (unsigned int)value : (unsigned int)value;

	int i = 0;
	while (n)
	{
		int r = n % base;

		if (r >= 10) 
			buffer[i++] = 65 + (r - 10);
		else
			buffer[i++] = 48 + r;

		n = n / base;
	}

	// if number is 0
	if (i == 0)
		buffer[i++] = '0';

	// If base is 10 and value is negative, the resulting string 
	// is preceded with a minus sign (-)
	// With any other base, value is always considered unsigned
	if (value < 0 && base == 10)
		buffer[i++] = '-';

	buffer[i] = '\0'; // null terminate string

	// reverse the string and return it
	return reverse(buffer, 0, i - 1);
}
