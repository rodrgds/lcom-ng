/**
	@file graphics.c
	@author Daniel Marques
	@author João Almeida

	Declaration of all the functions that handle the graphics part of the Project (which is quite a lot).
	Also contains some utility functions located at the end of this file.
*/

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "vbe.h"
#include "graphics.h"

#define BIT(n) 0x01<<(n)

static uint16_t *video_mem; /* Process address to which VRAM is mapped */
static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */



/* ------------------------------------------------------------------------------------------------------
	          -------------------------- FUNCTIONS THAT PAINT STUFF ------------------------
	 ------------------------------------------------------------------------------------------------------*/

int paintBMP(uint16_t *canvas ,unsigned int x , unsigned int y , uint16_t *img , unsigned int width , unsigned int height , int ignore_color){
  unsigned int i = 0, j =0 , data_i = 0;

	for (j = y ; j < (height+y) ; j++){
		for (i = x  ; i < (width+x) ;  i++ ,data_i++){
			if (img[data_i] != ignore_color && i>=0 && i<h_res && j>=0 && j<v_res){
				uint16_t *address = canvas;
				address += (i +  j*h_res);
				*(address) = img[data_i];

			}

    }
  }

	return 0;
}

void paintScreen(uint16_t *canvas ,unsigned int x, unsigned int y, int size_x, int size_y, unsigned short color) {
	unsigned int i = 0;
	unsigned int max_y = (size_y == 0) ? v_res : y + size_y; //if size_y is 0 then it paints to end of screen
	unsigned int max_x = (size_x == -1) ? h_res : x + size_x; //if size_x is 0 then it paints to right end of screen

	if (size_y >= 0){
	for (y ;  y < max_y  ;  y++)
		for (i = x; i < max_x; i++){
			if (i>0 && i<h_res && y>0 && y<v_res){
				uint16_t *address = canvas;
				address += (i +  y*h_res);
				*(address) = color;
			}
		}

	}
	else {
		for(y ; y > max_y ; y--){
			for(i = x ; i < max_x ; i++){
				if (i>0 && i<h_res && y>0 && y<v_res){
					uint16_t *address = canvas;
					address += (i +  y*h_res);
					*(address) = color;
				}
			}
		}
	}
}

void paintFinalScore(game_t *game , unsigned int score){
	unsigned int score_x = centerNumbers(score, game->h_numbers.width_of_1, game->h_numbers.width, HIGHSCORE_SEPARATION , h_res/4),
               highscore_x = centerNumbers(getHighscore(), game->h_numbers.width_of_1 , game->h_numbers.width, HIGHSCORE_SEPARATION , h_res*3/4);
	unsigned int y = 450;

  if (score <= getHighscore()){
    updateHighscore(game, game->buffer, score_x , y , score);
    updateHighscore(game, game->buffer, highscore_x, y, getHighscore() );
    paintBMP(game->buffer, h_res/4 - game->h_numbers.s_width/2 , y + 80 ,game->h_numbers.score, game->h_numbers.s_width, game->h_numbers.s_height, BLACK);
    paintBMP(game->buffer, h_res*3/4 - game->h_numbers.h_width/2 , y + 80 ,game->h_numbers.high_score, game->h_numbers.h_width, game->h_numbers.h_height, BLACK);
  }
	else {
    score_x = centerNumbers(score, game->h_numbers.width_of_1, game->h_numbers.width, HIGHSCORE_SEPARATION , h_res/2);
    updateHighscore(game, game->buffer, score_x , y , score);
    paintBMP(game->buffer, h_res - score_x - 35, y-80 , game->h_numbers.new_high,game->h_numbers.n_width,game->h_numbers.n_height,BLACK);
    paintBMP(game->buffer, h_res/2 - game->h_numbers.s_width/2 , y + 80 ,game->h_numbers.score, game->h_numbers.s_width, game->h_numbers.s_height, BLACK);
  	}
	memcpy(video_mem , game->buffer , h_res*v_res*sizeof(uint16_t));

}

void paintHour(game_t *game){
  unsigned int size = 0, x = HOUR_X, i = 0, j = 0;
  unsigned int * temp;
  for (i = 0 ; i < 3 ; i++ ){
    temp = separateNumbers( ( (i == 0) ? game->hour.hour : ( (i == 1) ? game->hour.minute : game->hour.second)),&size);

    if (size == 1){
      paintBMP(game->buffer,x,HOUR_Y,game->hour.zero ,game->hour.width, game->hour.height,BLACK);
      x+= game->hour.width;
    }

    paintBMP(game->buffer, x , HOUR_Y ,
            ( (temp[0] == 1) ? game->hour.one   :
            ( (temp[0] == 2) ? game->hour.two   :
            ( (temp[0] == 3) ? game->hour.three :
            ( (temp[0] == 4) ? game->hour.four  :
            ( (temp[0] == 5) ? game->hour.five  :
            ( (temp[0] == 6) ? game->hour.six   :
            ( (temp[0] == 7) ? game->hour.seven :
            ( (temp[0] == 8) ? game->hour.eight :
            ( (temp[0] == 9) ? game->hour.nine  : game->hour.zero))))))))),
            game->hour.width,game->hour.height,BLACK);
    x+= game->hour.width;

    if (size == 2){
      paintBMP(game->buffer, x , HOUR_Y ,
              ( (temp[1] == 1) ? game->hour.one   :
              ( (temp[1] == 2) ? game->hour.two   :
              ( (temp[1] == 3) ? game->hour.three :
              ( (temp[1] == 4) ? game->hour.four  :
              ( (temp[1] == 5) ? game->hour.five  :
              ( (temp[1] == 6) ? game->hour.six   :
              ( (temp[1] == 7) ? game->hour.seven :
              ( (temp[1] == 8) ? game->hour.eight :
              ( (temp[1] == 9) ? game->hour.nine  : game->hour.zero))))))))),
              game->hour.width,game->hour.height,BLACK);
      x += game->hour.width;

    }

    if (i != 2)
      paintBMP(game->buffer, x , HOUR_Y , game->hour.two_points , game->hour.width ,game->hour.height, BLACK);
    x+= game->hour.width;

    free(temp);
  }

}

void updateHighscore(game_t *game , uint16_t *canvas, unsigned int x , unsigned int y, unsigned int highscore){
	unsigned int size = 0, pos = 0, n_of_1 = 0;
	unsigned int *nums = separateNumbers(highscore, &size);
	for ( pos ; pos < size ; pos++ , x+= game->h_numbers.width+5){
		if ( n_of_1 )
			x-= 7;

		paintBMP(canvas, x ,y,
			 			(nums[pos] == 0 ) ? game->h_numbers.zero :
			 			(nums[pos] == 1 ) ? game->h_numbers.one :
			 			(nums[pos] == 2 ) ? game->h_numbers.two :
			 			(nums[pos] == 3 ) ? game->h_numbers.three :
			 			(nums[pos] == 5 ) ? game->h_numbers.five :
						(nums[pos] == 4 ) ? game->h_numbers.four :
			 			(nums[pos] == 6 ) ? game->h_numbers.six :
			 			(nums[pos] == 7 ) ? game->h_numbers.seven :
			 			(nums[pos] == 8 ) ? game->h_numbers.eight : game->h_numbers.nine ,
			 			(nums[pos] == 1 ) ? game->h_numbers.width_of_1 : game->h_numbers.width , game->h_numbers.height,BLACK);
	if( nums[pos] == 1)
		n_of_1=1;
	else
		n_of_1=0;

	}
	free(nums);
}

void updateBuffer(game_t *game, unsigned int status , unsigned int option){
	//PAINTING BACKGROUND
	memcpy(game->buffer , game->background_img , h_res*v_res*sizeof(uint16_t));

	if (status == 2 || status == 4){ //ONLY PAINT THE MAIN MENU
		if (option == 0){ // 0 = PLAY
			paintBMP(game->buffer, game->menus.playx , game->menus.playy , game->menus.play_s,game->menus.play_w,game->menus.play_h,BLACK);
			paintBMP(game->buffer, game->menus.exitx , game->menus.exity , game->menus.exit_un ,game->menus.exit_w,game->menus.exit_h,BLACK);
		}
		else if (option == 1){ // 1 = EXIT
			paintBMP(game->buffer, game->menus.playx , game->menus.playy , game->menus.play_un,game->menus.play_w,game->menus.play_h,BLACK);
			paintBMP(game->buffer, game->menus.exitx , game->menus.exity , game->menus.exit_s ,game->menus.exit_w,game->menus.exit_h,BLACK);
		}

		paintBMP(game->buffer, 200 , 0 , game->logo , game->logo_w , game->logo_h , BLACK);
		paintScreen(game->buffer,game->mountain1.x,game->mountain1.y,game->mountain1.size_x,game->mountain1.size_y,game->mountain1.color);
		paintBMP(game->buffer, game->ninja.x , game->ninja.y , game->ninja.stand_img , game->ninja.size_x , game->ninja.size_y , BLACK);

		if (status == 4)
			paintFinalScore(game,game->score);
	}
	else if (status == 3) { //NINJA IS FALLING
		//PAINTING MOUNTAIN1
		paintScreen(game->buffer , game->mountain1.x , game->mountain1.y, game->mountain1.size_x	,game->mountain1.size_y,game->mountain1.color);
		//PAINTING MOUNTAIN2
		paintScreen(game->buffer , game->mountain2.x, game->mountain2.y, game->mountain2.size_x, game->mountain2.size_y, game->mountain2.color);
		//PAINTING HIGHSCORE
		updateHighscore(game,game->buffer,HIGHSCORE_X , HIGHSCORE_Y ,game->score);
		//PAINTING NINJA
		paintBMP(game->buffer , game->ninja.x, game->ninja.y, ((game->ninja.status) ? game->ninja.walk_img : game->ninja.stand_img) , game->ninja.size_x , game->ninja.size_y, BLACK);
	}
	else{
		//PAINTING MOUNTAIN1
		paintScreen(game->buffer , game->mountain1.x , game->mountain1.y, game->mountain1.size_x	,game->mountain1.size_y,game->mountain1.color);
		//PAINTING MOUNTAIN2
		paintScreen(game->buffer , game->mountain2.x, game->mountain2.y, game->mountain2.size_x, game->mountain2.size_y, game->mountain2.color);
		//PAINTING HIGHSCORE
		updateHighscore(game , game->buffer, HIGHSCORE_X , HIGHSCORE_Y ,game->score);
		//PAINTING NINJA
		paintBMP(game->buffer , game->ninja.x, game->ninja.y, ((game->ninja.status) ? game->ninja.walk_img : game->ninja.stand_img) , game->ninja.size_x , game->ninja.size_y, BLACK);

    //PAINTING STICK
		if(game->stick.size > 0){
			if (game->stick.status)
				paintScreen(game->buffer , game->stick.x, game->stick.y, game->stick.width, ((game->stick.size >= game->stick.y) ? -game->stick.y : -game->stick.size), game->stick.color);
			else if (status == 0)
				paintScreen(game->buffer , ( (game->mountain1.x + game->mountain1.size_x) > 0) ? game->mountain1.x + game->mountain1.size_x : 0,
										game->stick.y-4, (game->stick.size > h_res) ? -1 : game->stick.size, game->stick.width , game->stick.color);
		}
	}

  //PAINTING HOUR
  paintHour(game);
  //PAINTING MOUSE
  paintBMP(game->buffer, game->mouse.x , game->mouse.y , game->mouse.cursor, game->mouse.width,game->mouse.height,BLACK);
}

void moveMouse(game_t *game, unsigned int status, unsigned int option){
	updateBuffer(game,status,option);
	memcpy(video_mem , game->buffer , h_res*v_res*sizeof(uint16_t));
}

unsigned int moveMountains(game_t *game , unsigned int cont){
	unsigned int stick_max_x     = (((game->mountain1.size_x > 0) ? (game->mountain1.x+game->mountain1.size_x) : 0) + game->stick.size),
							 mountain2_max_x = game->mountain2.x + game->mountain2.size_x,
							 mountain2_min_x = game->mountain2.x,
							 ninja_min_x     = game->ninja.x + 13,
							 ninja_max_x     = game->ninja.x + game->ninja.size_x;

		//	( (ninja on correct position on mountain) AND (stick within limits of mountain) )
		//	OR
		//  ( (ninja on top of stick) AND (stick bigger than mountain) )
		//  OR
		//  ( (ninja out of stick) AND (stick smaller than mountain) )
		//
	if ( ( (ninja_max_x < (mountain2_max_x - 10)) && (stick_max_x > mountain2_min_x) && (stick_max_x < mountain2_max_x))
		||
		 ( (ninja_min_x < stick_max_x) && (stick_max_x > mountain2_max_x))
		||
		 ( (ninja_max_x < stick_max_x) && (stick_max_x < mountain2_min_x))
	 )
	{
				if (game->mountain1.x > 0)
					game->mountain1.x -= (game->mountain1.x >= MOUNTAIN_MOVE) ? MOUNTAIN_MOVE : game->mountain1.x;
				else if (game->mountain1.size_x > 0)
					game->mountain1.size_x -= (game->mountain1.size_x >= MOUNTAIN_MOVE) ? MOUNTAIN_MOVE : game->mountain1.size_x;
				else if (game->mountain1.size_x == 0)
					game->stick.size -= (game->stick.size >= MOUNTAIN_MOVE) ? MOUNTAIN_MOVE : game->stick.size ;

				if (game->mountain2.x > 0)
					game->mountain2.x -= (game->mountain2.x >= MOUNTAIN_MOVE) ? MOUNTAIN_MOVE : game->mountain2.x;
				else if (game->mountain2.size_x > 0)
					game->mountain2.size_x -= (game->mountain2.size_x >= MOUNTAIN_MOVE) ? MOUNTAIN_MOVE : game->mountain2.size_x;

				game->ninja.y = (stick_max_x > 48) ? 454 : 459 ;

				if (cont >=5){
 					game->ninja.size_x = 44;
					game->ninja.status = 1;
				}
				else{
					 game->ninja.size_x = 40;
					 game->ninja.status = 0;
				 }

				return 3; //RETURN 3 WHEN NINJA IS STILL MOVING
		}

		else {
			//ERASE ALL ON SCREEN TO PUT NINJA ON STAND
			game->ninja.size_x = 40;
			game->ninja.status = 0;
			game->ninja.y = 459;

			updateBuffer(game,0,0);
			memcpy(video_mem , game->buffer , h_res*v_res*sizeof(uint16_t));
			paintScreen( video_mem, ( (game->mountain1.x + game->mountain1.size_x) > 0) ? game->mountain1.x + game->mountain1.size_x : 0,
														game->stick.y-5, game->stick.size, game->stick.width , game->stick.color);


			if (stick_max_x > mountain2_max_x) //STICK TOO BIG
				return 2;
			else if (stick_max_x < mountain2_min_x) //STCK TOO SMALL
				return 1;
			else{ // STICK ON TOP OF MOUNTAIN
        game->stick.x = mountain2_max_x-5;
        return 0;
      }
		}
}

void startGame(game_t *game){
	updateBuffer(game,1,1);
	memcpy(video_mem , game->buffer , h_res*v_res*sizeof(uint16_t));
}

unsigned int growStick(game_t *game){
	game->stick.size += 5;
	game->stick.status = 1;

	updateBuffer(game, 1, 0);
	if (game->stick.size > 800){ //WHEN MAXIMUM SIZE REACHED STOP GROWING
		game->stick.status = 0;
		return 0;
	}
	memcpy(video_mem , game->buffer , h_res*v_res*sizeof(uint16_t));
	return 1;
}

void stickFallAnimation(game_t *game){
	game->stick.status = 0;
	updateBuffer(game, 0, 1);
	memcpy(video_mem , game->buffer , h_res*v_res*sizeof(uint16_t));
}

unsigned int ninjaFallAnimation(game_t *game){
	game->stick.size = 0;
	if(game->ninja.y < v_res){
		game->ninja.y += 3;
		updateBuffer(game,3,1);
		memcpy(video_mem , game->buffer , h_res*v_res*sizeof(uint16_t));
		return 0;
	}
	return 1;
}

/* ------------------------------------------------------------------------------------------------------
	            -------------------------- USEFUL FUNCTIONS ------------------------
	 ------------------------------------------------------------------------------------------------------*/

//BASED ON http://stackoverflow.com/a/14279511
uint16_t *LoadBitmapFile(char *filename, bit_map_info_header_t *bit_map_info_header) {
	 	FILE *filePtr; //our file pointer
	 	bit_map_file_header_t bit_map_file_header; //our bitmap file header
	 	uint16_t *bitmapImage;  //store image data

	 	//open filename in read binary mode
	 	filePtr = fopen(filename, "rb");
	 	if (filePtr == NULL) {
	 		printf("ERROR READING FILE! \n");
	 		return NULL;
	 	}
	 	//read the bitmap file header
	 	fread(&bit_map_file_header, sizeof(bit_map_file_header_t), 1, filePtr);

	 	//verify that this is a bmp file by check bitmap id
	 	if (bit_map_file_header.type != 0x4D42) {
	 		fclose(filePtr);
	 		return NULL;
	 	}

	 	//read the bitmap info header
	 	fread(bit_map_info_header, sizeof(bit_map_info_header_t), 1, filePtr);

	 	//move file point to the begging of bitmap data
	 	fseek(filePtr, bit_map_file_header.off_bits, SEEK_SET);

	 	//allocate enough memory for the bitmap image data
	 	bitmapImage = (uint16_t*) malloc(bit_map_info_header->size_image);

	 	//verify memory allocation
	 	if (!bitmapImage) {
	 		free(bitmapImage);
	 			fclose(filePtr);
	 		return NULL;
	 	}

	 	//read in the bitmap image data
	 	fread(bitmapImage, bit_map_info_header->size_image, 1, filePtr);

	   //make sure bitmap image data was read
	   if (bitmapImage == NULL)
	   {
	       fclose(filePtr);
	       return NULL;
	   }

	   //close file and return bitmap iamge data
	   fclose(filePtr);
	   return correctImage(bitmapImage, bit_map_info_header->width, bit_map_info_header->height);
	 }

uint16_t *correctImage(uint16_t *arr, unsigned int w , unsigned int h){
	 	unsigned int i = 0, j = 0;
	 	uint16_t temp;
	 	//FLIP IMAGE VERTICALLY
	 	for(j = 0 ; j < (h/2) ; j++){
	 		for(i = 0 ; i < w ; i++){
	 			temp 							= arr[(h-j-1)*w +i];
	 			arr[(h-j-1)*w +i] = arr[j*w + i];
	 			arr[j*w + i] 			= temp;
	 		}
	 	}

	 	return arr;
	 }

unsigned int *separateNumbers(unsigned int number, unsigned int *size){
	 	unsigned int n = number, n_size =0, first = 1;
	 	while( n > 0 ){
	 		n_size++;
	 		n = n/10;
	 	}
	 	if (number == 0)
	 		n_size++;
	 	*size = n_size;
	 	unsigned int *nums = malloc(n_size*sizeof(unsigned int) );
	 	n_size--;
	 	while( (first || number > 0) && n_size >= 0){
	 		nums[n_size] = (number % 10);
	 		number = number/10;
	 		n_size--;
	 		first = 0;
	 	}
	 	return nums;
	 }

unsigned int centerNumbers(unsigned int number , unsigned int w_of_1 , unsigned int w_of_others , unsigned int separation, unsigned int x){
	 	unsigned int size=0, width = 0, i = 0;
	 	unsigned int *nums = separateNumbers(number, &size);
	 	for (i = 0 ; i < size ; i++){
	 		width +=  ( (nums[i] == 1) ? w_of_1+separation : w_of_others+separation );
	 	}
	 	return (x - width/2);
	 }


/* ------------------------------------------------------------------------------------------------------
	   -------------------------- PURE UTILITY FUNCTIONS, NO MAJOR WORK BELOW ------------------------
	 ------------------------------------------------------------------------------------------------------*/
void *vgInit(unsigned short mode) {
	struct reg86u r;
	vbe_mode_info_t vbe;
	r.u.w.ax = VBE_CALL_SET_MODE;
	r.u.w.bx = 1 << 14 | mode;
	r.u.b.intno = VBE_BIOS_INTERRUPT;

	if (sys_int86(&r) != OK)
		printf("set_vbe_mode: sys_int86() failed \n");
	vbe_get_mode_info(GRAPHICS_MODE, &vbe);

	video_mem = vramMap(&vbe);

	return video_mem;
}

int vgExit() {
	struct reg86u reg86;
	reg86.u.b.intno = 0x10; /* BIOS video services */
	reg86.u.b.ah = 0x00; /* Set Video Mode function */
	reg86.u.b.al = 0x03; /* 80x25 text mode*/

	if (sys_int86(&reg86) != OK) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return 0;
}

void *vramMap(vbe_mode_info_t *vbe) {
	int r;
	struct mem_range mr;
	void *video_mem;

	h_res = vbe->XResolution;
	v_res = vbe->YResolution;
	bits_per_pixel = vbe->BitsPerPixel;

	//ALLOW MEMORY MAPPING
	mr.mr_base = vbe->PhysBasePtr;
	mr.mr_limit = mr.mr_base + (h_res * v_res*2);

	if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	video_mem = vm_map_phys(SELF, (void *) mr.mr_base,h_res*v_res*2);
	if (video_mem == MAP_FAILED) {
		panic("Could not map video memory\n");
		return NULL;
	}

	return video_mem;
}

unsigned int getHRes() {
	return h_res;
}

unsigned int getVRes(){
	return v_res;
}

uint16_t *getVideoMem(){
	return (uint16_t*)video_mem;
}
