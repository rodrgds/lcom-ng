#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

static int DEBUG = 0;


#define TRANSPARENT_R 0xFF
#define TRANSPARENT_G 0x00
#define TRANSPARENT_B 0xFF

/*!
 * Carrega um ficheiro bitmap BMP com cor de 24 bits.
 * Foi testado com BMPs criados em Windows e a partir da especificacao do formato BMP na pagina da wikipedia
 * Lê o header do ficheiro para obter informacoes sobre a imagem.
 * Aloca dinamicamente espaco para o array de pixeis
 * Recebe o array de pixeis invertendo da ordem do formato bmp para a ordem standart de arrays de pixeis utilizada
 *
 *\param filename string de c com o path para a root do programa
 *\param res pointer para estrutura onde guardar um pointer para um array de pixeis alocado dinamicamente e informacoes sobre a imagem
 *\return devolve 0 se for bem sucedido
 */
 int loadBMP(char* filename,rgbImage* res) {

	 	FILE* fp;

	 	if ((fp = fopen(filename,"rb")) == NULL) {printf("Erro a abrir o ficheiro\n"); return 1;}

	 	// BMP Header
	 	unsigned char fileHeader[14];
	 	int i; int in, dc = 0;
	 	for(i = 0; i < 14; i++) {
	 		if ( (in = fgetc(fp)) == EOF) {printf("Ficheiro Corrompido\n"); return 1;}
	 		fileHeader[i] = (in & 0xFF);
	 		if (DEBUG) printf("%.2x ",fileHeader[i]);
	 		if ((++dc)%16 == 0) printf("\n");
	 	}

	 	int headerSize = *(int*)(fileHeader + 10);

	 	unsigned char* Header = malloc(headerSize-14);

	 	for(i = 0; i < headerSize - 14; i++) {
	 		if ( (in = fgetc(fp)) == EOF) {printf("Ficheiro Corrompido\n"); return 1;}
	 		Header[i] = (in & 0xFF);
	 		if (DEBUG) {
	 			printf("%.2x ",Header[i]);
	 			if ((++dc)%16 == 0) printf("\n");
	 		}
	 	}


		int width = *(int*)(Header + 4);
		int height = *(int*)(Header + 8);
		int bpp = (*(int*)(Header + 14)) >> 3;

		free(Header);


		// Pixel Array

		unsigned char* pix = malloc(width*height*bpp);

		int j,c,p, bytes_per_row = ((24*width + 31)/32)*4;
		for(i = height-1; i >= 0; i--) {
			c = bytes_per_row;
			for(j = 0; j < width; j++) {
				p = (i*width + j)*bpp;
				if ( (in = fgetc(fp)) == EOF) break; pix[p] = in;
				if ( (in = fgetc(fp)) == EOF) break; pix[p + 1] = in;
				if ( (in = fgetc(fp)) == EOF) break; pix[p + 2] = in;
				c-=bpp;
			}
			for(;c > 0; c--) if(fgetc(fp) == EOF) break;
		}
		if (i >= 0 || j < width || c > 0) {
			if (DEBUG) printf("Ficheiro Corrompido: Erro na matriz de pixeis. %d %d %d\n", i,j,c);
			return 1;
		}
		if (DEBUG) {
			for(i = 0; i < width*height; i++)
				printf("(%d,%d): %d:%d:%d\n",
						i%width, i/width,
						pix[i*3],pix[i*3 + 1],pix[i*3 + 2]);

			printf("W: %d; H: %d;\n", width, height);
		}

		fclose(fp);


		rgbImage img;
		img.pixels = pix;
		img.width = width;
		img.height = height;
		img.offsetX = 0;
		img.offsetY = 0;
		*res = img;

		return 0;
};

 /*!
  * Desenha uma imagem numa posicao no ecra.
  * Uma vez que e uma funcao muito utilizada tentou-se optimiza-la ao maximo pelo que o codigo ficou menos legivel.
  *
  *\param img imagem a desenhar
  *\param x coordenada x da posicao no ecra onde desenhar a imagem
  *\param y coordenada y da posicao no ecra onde desenhar a imagem
  *
  *\return devolve sempre 0
  */
int drawImage(rgbImage img, int x, int y) {
	 screen_info info = vg_getScreenInfo();
	 int WIDTH = info.WIDTH, HEIGHT = info.HEIGHT;
	 x -= img.offsetX; y -= img.offsetY;

	 if (img.width + x <= 0 || img.height + y <= 0 || x >= WIDTH || y >= HEIGHT) return 0;
	 unsigned char* s = info.screen; unsigned char* g;

	 int i, j;
	 for(i = 0; i<img.height; i++) {
		 if (i + y < 0) continue;
		 if (i + y >= HEIGHT) break;
		 s = info.screen + ((i+y)*WIDTH + max(0,x)) * 3;
		 g = img.pixels + ((i*img.width) + (x < 0 ? -x : 0))*3;
		 j = max(img.width + (x < 0 ? x : 0) + (x + img.width >= WIDTH ? WIDTH - x - img.width : 0) ,0);

		for(; j > 0; j--) {
			if (!(g[0] == 255 && g[1] == 0 && g[2] == 255))
				 memcpy(s,g,3);
			 s+=3; g+=3;
		 }
	}
	return 0;
	// May the lord have mercy on my imortal soul, as I have created an abomination.
}

/*!
 * Mistura duas compoentes de uma cor atravez de uma multiplicacao.
 *\param a valor da primeira componente
 *\param b valor da segunda componente
 *\return valor do blend resultante
 */
unsigned char blendColors(unsigned short a, unsigned short b) {return (a*b)/0xFF;}

/*!
 * Converte uma cor no formato HSV para o formato RGB
 *\param h componente Hue da cor
 *\param s componente Saturation da cor
 *\param v componete Value da cor
 *\return cor resultante em rgb
 */
unsigned int hsvTorgb(unsigned int h, float s, float v) {
	h = h % 360;
	s = max(min(s,1.0),0.0);
	v = max(min(v,1.0),0.0);
	s = 1.0; v = 1.0;
	float C = v*s;
	float X = C*(1.0 - fabs(fmod((double)h/60.0,2.0) -1.0) );
	float m = v - C;
	unsigned char r = m*255,g = m*255,b = m*255;

	if (h < 60)       {r += C*255; g+=X*255;}
	else if (h < 120) {r += X*255; g+=C*255;}
	else if (h < 180) {g += C*255; b+=X*255;}
	else if (h < 240) {g += X*255; b+=C*255;}
	else if (h < 300) {r += X*255; b+=C*255;}
	else	          {r += C*255; b+=X*255;}


	printf("%.2x %.2x %.2x\n",r,g,b);
	return ((r << 16) + (g << 8) + b) & 0xFFFFFF;
}

/*!
 * Desenha uma imagem misturada com uma cor.
 *\param img imagem a desenhar
 *\param x coordenada x da posicao no ecra onde desenhar a imagem
 *\param y coordenada y da posicao no ecra onde desenhar a imagem
 *\param color cor a misturar com a imagem
 *\return devolve sempre 0
 */
int drawImageBlend(rgbImage img, int x, int y, unsigned int color) {
	 screen_info info = vg_getScreenInfo();
	 int WIDTH = info.WIDTH, HEIGHT = info.HEIGHT;
	 x -= img.offsetX; y -= img.offsetY;

	 if (img.width + x <= 0 || img.height + y <= 0 || x >= WIDTH || y >= HEIGHT) return 0;
	 unsigned char* s = info.screen; unsigned char* g; unsigned char blend[3];
	 blend[0] = color & 0xFF;
	 blend[1] = (color & 0xFF00) >> 8;
	 blend[2] = (color & 0xFF0000) >> 16;

	 int i, j;
	 for(i = 0; i<img.height; i++) {
		 if (i + y < 0) continue;
		 if (i + y >= HEIGHT) break;
		 s = info.screen + ((i+y)*WIDTH + max(0,x)) * 3;
		 g = img.pixels + ((i*img.width) + (x < 0 ? -x : 0))*3;
		 j = max(img.width + (x < 0 ? x : 0) + (x + img.width >= WIDTH ? WIDTH - x - img.width : 0) ,0);

		for(; j > 0; j--) {
			if (!(g[0] == TRANSPARENT_B && g[1] == TRANSPARENT_G && g[2] == TRANSPARENT_R)) {
				s[0] = blendColors(g[0], blend[0]);
				s[1] = blendColors(g[1], blend[1]);
				s[2] = blendColors(g[2], blend[2]);
			}
			 s+=3; g+=3;
		 }
	}
	return 0;
}

/*!
 * Cria uma copia de uma parte de uma imagem. e alocada memoria para a copia.
 *\param img imagem original de onde e copiada a imagem final
 *\param x coordenada x do canto superior esquerdo da imagem final na imagem original
 *\param y coordenada y do canto superior esquerdo da imagem final na imagem original
 *\param w largura da imagem final
 *\param h altura da imagem final
 *\return devolve a imagem resultante. Caso as coordenadas sejam invalidas ou a imagem saia fora da original e devolvida uma imagem vazia.
 */
rgbImage cropImage(rgbImage img, int x, int y, int w, int h) {
	rgbImage res;
	res.width = 0; res.height = 0; res.pixels = NULL;
	if (x + w > img.width || y + h > img.height || y < 0 || x < 0) return res;

	res.pixels = malloc(w*h*3);
	res.width = w;
	res.height = h;

	int i, j;
	for(i = 0; i<h; i++) {
		 memcpy(res.pixels + i*w*3,
				img.pixels + ((i+y)*img.width + x)*3,
				w*3);
	}

	return res;
};


/*!
 * Cria uma copia de uma parte de uma imagem. e alocada memoria para a copia. O resultado e centrado.
 *\param img imagem original de onde e copiada a imagem final
 *\param x coordenada x do canto superior esquerdo da imagem final na imagem original
 *\param y coordenada y do canto superior esquerdo da imagem final na imagem original
 *\param w largura da imagem final
 *\param h altura da imagem final
 *\return devolve a imagem resultante centrada. Caso as coordenadas sejam invalidas ou a imagem saia fora da original e devolvida uma imagem vazia.
 */
rgbImage cropImageCentered(rgbImage img, int x, int y, int w, int h) {
	rgbImage res = cropImage(img,x,y,w,h);
	res.offsetX = w/2; res.offsetY = h/2;
	return res;
};
