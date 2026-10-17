#include "graphics.h"
#include "stdlib.h"


//// SPRITES /////

// Todos os sprites sao guardados em arrays globais

rgbImage PlayerSprite[3];
rgbImage BulletSprite[3];
rgbImage EBulletSprite[10];
rgbImage ExplosionSprite[6];
rgbImage PowerUpSprite[6];
rgbImage ShieldSprite[2];
rgbImage RedBeamSprite[1];
rgbImage EnemySprite[3];
rgbImage EnemyCruiserSprite[2];
rgbImage StarSprite[8];
rgbImage EnemyMotherShipSprite[2];
rgbImage MotherLightBeamSprite[1];
rgbImage MotherShieldSprite[1];
rgbImage ButtonSprite[3];
rgbImage GameOverSprite[2];
rgbImage LogoSprite[1];
rgbImage LevelSignSprite[4];


/*!
 * inicializa os sprites a partir de uma spritesheet
 * \param path caminho para a root do jogo
 */
int init_sprites(char* path) {
	char* filepath = malloc(strlen(path) + strlen("/res/sprites.bmp") + 8);
	strcpy(filepath,path);
	strcat(filepath,"/res/sprites.bmp");

	rgbImage spriteSheet1;
	if (loadBMP(filepath,&spriteSheet1) != 0) return 1;

	const int w = 64;
	const int w2 = w/2;
	int i;


	for(i = 0; i < 6; i++)
		PowerUpSprite[i] = cropImageCentered(spriteSheet1, i*w2, 4*w, w2, w2); //(spritesheet, posicao em x, posicao em y, largura, altura)

	for(i = 0; i < 2; i++)
		ShieldSprite[i] = cropImageCentered(spriteSheet1, i*w, w,w,w);

	RedBeamSprite[0] = cropImageCentered(spriteSheet1, 2*w, 1*w,w,w);

	MotherShieldSprite[0] = cropImageCentered(spriteSheet1, 3*w, 2*w,w,w);

	MotherLightBeamSprite[0]= cropImageCentered(spriteSheet1, 5*w,w,w,w);


	for (i = 0; i < 3; i++)
		PlayerSprite[i] = cropImageCentered(spriteSheet1, i*w,0,w,w);

	//balas do jogador
	for (i = 0; i < 3; i++)
		BulletSprite[i] = cropImageCentered(spriteSheet1, i*w,2*w,w,w);

	//balas dos inimigos
	for (i = 0; i < 4; i++)
			EBulletSprite[i] = cropImageCentered(spriteSheet1, i*w2 + 4*w,2*w,w2,w2);
		for (i = 0; i < 2; i++)
			EBulletSprite[i+4] = cropImageCentered(spriteSheet1, (3+i)*w,w,w,w);
		for(i = 0; i < 4; i++)
			EBulletSprite[i+6] = cropImageCentered(spriteSheet1, 4*w + i*w2,2*w + w2,w2,w2);

	for (i = 0; i < 6; i++)
		ExplosionSprite[i] = cropImageCentered(spriteSheet1, i*w,3*w,w,w);

	for (i = 0; i < 3; i++)
		EnemySprite[i] = cropImageCentered(spriteSheet1, (i+4)*w,0,w,w);

	for (i = 0; i < 2; i++)
		EnemyCruiserSprite[i] = cropImageCentered(spriteSheet1, (i*2 + 7)*w,0,2*w,3*w);

	for(i = 0; i < 2; i++)
		EnemyMotherShipSprite[i] = cropImageCentered(spriteSheet1, (i*8)*w, 8*w, 8*w-1, 5*w);

	for (i = 0; i < 8; i+=2) {
		StarSprite[i] = cropImageCentered(spriteSheet1, (i/2)*w2, 4*w + w2, w2,w2);
		StarSprite[i+1] = cropImageCentered(spriteSheet1, (i/2)*w2, 5*w, w2,w2);
	}

	for (i = 0; i < 2; i++)
		ButtonSprite[i] = cropImageCentered(spriteSheet1, 0,(14+i)*w, 3*w,w-1);
	ButtonSprite[i] = cropImage(spriteSheet1, 0,13*w,w2,w2);

	for (i = 0; i < 4; i++)
		LevelSignSprite[i] = cropImageCentered(spriteSheet1, 6*w,(4+i)*w,2*w,w);

	GameOverSprite[0] = cropImageCentered(spriteSheet1,3*w,14*w,6*w,2*w);
	GameOverSprite[1] = cropImageCentered(spriteSheet1,9*w,13*w,7*w-1,3*w-1);

	LogoSprite[0] = cropImageCentered(spriteSheet1,8*w,5*w,8*w,3*w);

	free(spriteSheet1.pixels);
	free(filepath);
	return 0;
};

/*!
 * liberta a memoria onde foram alocados os pixeis de cada sprite
 */
int freeSprites() {
	int i;
	for(i = 0; i <3; i++)
		free(PlayerSprite[i].pixels);

	for(i = 0; i <3; i++)
		free(BulletSprite[i].pixels);

	for(i = 0; i <10; i++)
		free(EBulletSprite[i].pixels);

	for(i = 0; i <6; i++)
		free(ExplosionSprite[i].pixels);

	for(i = 0; i < 2; i++)
		free(ShieldSprite[i].pixels);

	free(RedBeamSprite[0].pixels);
	free(MotherShieldSprite[0].pixels);

	for(i=0; i < 6; i++)
		free(PowerUpSprite[i].pixels);

	for(i=0; i < 3; i++)
		free(EnemySprite[i].pixels);

	for(i=0; i < 2; i++)
		free(EnemyCruiserSprite[i].pixels);

	for(i = 0; i < 2; i++)
		free (EnemyMotherShipSprite[i].pixels);

	for(i = 0; i < 8; i++)
		free(StarSprite[i].pixels);

	for(i = 0; i < 3; i++)
		free(ButtonSprite[i].pixels);

	for(i = 0; i < 2; i++)
		free(GameOverSprite[i].pixels);

	free(LogoSprite[0].pixels);

	return 0;
}


//// FONT ////////////

rgbImage font[256];

/*!
 * inicializa os caracteres para a escrita de texto em modo grafico
 * \param path caminho para a root do jogo
 */
int init_font(char* path) {
	char* filepath = malloc(strlen(path) + strlen("/res/chars.bmp") + 8);
	strcpy(filepath,path);
	strcat(filepath,"/res/chars.bmp");

	rgbImage fontSheet;
	if (loadBMP(filepath, &fontSheet) != 0) return 1;

	printf("%d x %d\n", fontSheet.width, fontSheet.height);
	int i ,x ,y;
	for(i = 0; i < 256; i++) {
		x = (i%16) * 16; y = (i/16) * 12;
		font[i] = cropImage(fontSheet, x,y, 8, 11);
	}

	free(fontSheet.pixels);
	free(filepath);
	return 0;
};

/*!
 * liberta a memoria onde foram alocados os caracteres para escrita
 */
int freeFont() {
	int i;
	for(i = 0; i<256; i++)
		free(font[i].pixels);
	return 0;
};

/*!
 * escreve texto numa posicao do ecra
 * \param text string de c com o texto a ser escrito
 * \param x posicao x no ecra onde escrever o texto
 * \param y posicao y no ecra onde escrever o texto
 */
int drawText(char* text, int x, int y) {
	return drawTextBlend(text,x,y,0xFFFFFF);
};

/*!
 * escreve texto numa posicao do ecra com uma dada cor
 * \param text string de c com o texto a ser escrito
 * \param x posicao x no ecra onde escrever o texto
 * \param y posicao y no ecra onde escrever o texto
 * \param blend cor com a qual o texto e desenhado (formato rgb de 24 bits)
 */
int drawTextBlend(char* text, int x, int y, int blend) {

	unsigned char c; int i = 0;
	while( (c = text[i++]) != 0)
		drawImageBlend(font[c], x + i*8, y, blend);

	return 0;
};
