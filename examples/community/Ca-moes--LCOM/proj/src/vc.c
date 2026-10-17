#include <lcom/lcf.h>
#include <vc.h>
#include <vc_macros.h>
#include <keyboard.h>
#include <i8042.h>
#include <math.h>

static vbe_mode_info_t info;/**< @brief structure to be initialized  with the VBE information on the input mode */

static void *video_mem;		   /**< @brief frame-buffer VM address */
static char* double_buf;     /**< @brief back buffer*/

static uint16_t hres;        /**< @brief Horizontal resolution in pixels */
static uint16_t vres;        /**< @brief Vertical resolution in pixels */
static unsigned bits_per_pixel; /**< @brief Number of VRAM bits per pixel */
static mmap_t mem_map;       /**< @brief mapping of physical memory to virtual memory*/

extern uint8_t SCANCODE[2];  /**<@brief Array to store the bytes from the scancode */
extern bool SCANCODE_2B;     /**<@brief true if the scancode has 2 bytes */ 
extern uint32_t TIMER_COUNTER; /**<@brief Counter to keep track of the elapsed time */ 

int vbe_get_info_mode(uint16_t mode, vbe_mode_info_t * vmi_p){
  reg86_t r;

	lm_alloc(sizeof(vbe_mode_info_t),&mem_map);

  memset(&r, 0, sizeof(r));
	r.ax = VBE | GET_VBE_MODE_INFO; /* VBE get mode info */
	/* translate the buffer linear address to a far pointer */
	r.es = PB2BASE(mem_map.phys); /* set a segment base */
	r.di = PB2OFF(mem_map.phys); /* set the offset accordingly */
	r.cx = mode;
	r.intno = VC_INT;
	if (sys_int86(&r) != OK) { /* call BIOS */
		return 1;
	}

  /*copy contents from mem_map virtual address to the vbe info struct*/
  char *src = (char *)mem_map.virt; 
  char *dest = (char *)vmi_p; 
  
  for (int i=0; i<(int)mem_map.size; i++) 
    dest[i] = src[i]; 

  lm_free(&mem_map);

  return 0;
}

int graphics_mode(uint16_t mode){
	/*Allocates a memory block in low memory area with 1MB*/
	int mb= 1024*1024;
	mem_map.phys=0;
	mem_map.size=mb;
	lm_alloc(mb,&mem_map);

	/*Initialize vram_base and vram_size*/
  memset(&info, 0, sizeof(info));/* zero the structure */

	/*for now use the function vbe_get_mode_info to obtain the physical memory address*/
	if(vbe_get_info_mode(mode,&info)!=0)
		return 1;

	unsigned int vram_base; /* VRAM’s physical addresss */
  unsigned int vram_size; /* VRAM’s size, but you can use the frame-buffer size, instead */

	vram_base  = info.PhysBasePtr;
  hres = info.XResolution;
  vres = info.YResolution;
  bits_per_pixel = info.BitsPerPixel;
  vram_size = ((bits_per_pixel+7)/8)*hres*vres; //using frame buffer size

	/* Allow memory mapping */
	struct minix_mem_range mr; /* physical memory range */
	int r;
  mr.mr_base = (phys_bytes)vram_base;
  mr.mr_limit = mr.mr_base + vram_size;
  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if(video_mem == MAP_FAILED)
    panic("couldn’t map video memory");

  double_buf = (char*) malloc(vram_size);//void *malloc(size_t size) - sizeof video mem = size of doub_buf(is a copy of video_mem)

  //Initialize Video card with Graphic Mode
  reg86_t r86;
  memset(&r86, 0, sizeof(r86));	/* zero the structure */

	r86.intno = VC_INT; /* BIOS video services */
  r86.ax = VBE | SET_VBE_MODE;
  r86.bx = SET_LINEAR_MODE | mode;
  
  if( sys_int86(&r86) != OK){ 
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }
	lm_free(&mem_map);
	return 0;
}

unsigned get_hres(){
  return hres;
}

unsigned get_vres(){
  return vres;
}

int draw_xpm_image(uint8_t *sprite, uint16_t x, uint16_t y, xpm_image_t img, void * buff) {
  int pixel_num = 0;
  xpm_image_t xd;
  xd.bytes = img.bytes;
  for(int j = 0; j < img.height; j++){
    for (int i=0; i < img.width; i++){
      if(((uint16_t *)(img.bytes))[pixel_num] != xpm_transparency_color(img.type))
        draw_pixel(x+i,y+j,&((uint16_t *)(xd.bytes))[pixel_num],buff);
      pixel_num++;
    }
  }
 
  return 0;
}

int draw_sentence(letter *sentence, uint16_t x, uint16_t y, void * buff,uint8_t errors, int correct) {
  uint16_t x1=x; /*x1 -> posição onde desenhar a letras*/
  bool finish = false;
  char color='b';
  for(int i = 0; !finish; i++){
    if(i <= correct){
      color = 'g';
    }
    else if(i > correct && i <= (correct + (int)errors)){
      color = 'r';
    }
    else{
      color = 'b';
    }
    if(sentence[i].letter == '.'){
      finish = true;
    }

    uint16_t temp=x1;
    if(sentence[i].letter == ' '){
      //procura o proximo espaço da frase delimitando a palavra
      for(int j = i+1; true; j++){
        if(sentence[j].letter == ' ' || sentence[j].letter == '.'){// encontrando um espaço
          //calcula se a proxima palavra cabe no ecra 
          for(int k = i+1; k <= j; k++){ 
            temp += sentence[k].img.width + 1;
          }
          if(temp > x + 680){
            y += sentence[i].img.height - 2;
            x1=x;
          }
          break;
          if(sentence[j].letter == '.') break;
        }
    }
    }
  
    if((x1-x)>700){
      y += sentence[i].img.height + 1;
      x1=x;
    }
    if(sentence[i].letter != ' '){
      if(color == 'g')
        draw_letter(sentence[i].sprite,x1,y,sentence[i].img,buff,greenChar);
      else if( color == 'r')
        draw_letter(sentence[i].sprite,x1,y,sentence[i].img,buff,redChar);
      else
        draw_letter(sentence[i].sprite,x1,y,sentence[i].img,buff,blackChar);
      
      x1+=sentence[i].img.width + 1;
    }
    else{
      if(x1 != x){
        if(color == 'g')
          draw_letter(sentence[i].sprite,x1,y,sentence[i].img,buff,greenChar);
        else if( color == 'r')
          draw_letter(sentence[i].sprite,x1,y,sentence[i].img,buff,redChar);
        else
          draw_letter(sentence[i].sprite,x1,y,sentence[i].img,buff,blackChar);
      
        x1+=sentence[i].img.width + 1;
      }
    }
  }
  return 0;
}

int draw_input_sentence(letter *input,letter *sentence, uint16_t x, uint16_t y, void * buff,uint8_t errors, int correct) {

  uint16_t x1=x; /*x1 -> posição onde desenhar a letras*/
  bool finish = false;
  char color='b';

  for(int i = 0; i <= correct + (int)errors; i++){ 
    
    if(i <= correct){
      color = 'g';
    }
    else if(i > correct && i <= (correct + (int)errors)){
      color = 'r';
    }
    else{
      color = 'b';
    }
    if(input[i].letter == '.'){
      finish = true;
    }
    if(input[i].letter == '.'){
      finish = true;
    }
    
    uint16_t temp=x1;
    if(sentence[i].letter == ' '){
      //procura o proximo espaço da frase delimitando a palavra
      for(int j = i+1; true; j++){
      if(sentence[j].letter == ' ' || sentence[j].letter == '.'){// encontrando um espaço
        //calcula se a proxima palavra cabe no ecra 
        for(int k = i+1; k <= j; k++){ 
          temp += sentence[k].img.width + 1;
        }
        if(temp > x + 700){
          y +=sentence[i].img.height - 2;
          x1=x;
        }
        break;
        if(sentence[j].letter == '.') break;
      }
    }
    }
  
    if(input[i].letter != ' '){
      if(color == 'g')
        draw_letter(input[i].sprite,x1,y,input[i].img,buff,greenChar);
      else if( color == 'r')
        draw_letter(input[i].sprite,x1,y,input[i].img,buff,redChar);
      else
        draw_letter(input[i].sprite,x1,y,input[i].img,buff,blackChar);
      
      x1+=input[i].img.width + 1;
    }
    else{
      if(x1 != x){
        if(color == 'g')
          draw_letter(input[i].sprite,x1,y,input[i].img,buff,greenChar);
        else if( color == 'r')
          draw_letter(input[i].sprite,x1,y,input[i].img,buff,redChar);
        else
          draw_letter(input[i].sprite,x1,y,input[i].img,buff,blackChar);
      
        x1+=input[i].img.width + 1;
      }
    }
  }
  return 0;
}

int draw_name(letter *sentence, uint16_t x, uint16_t y, void * buff,unsigned int n) {
  uint16_t x1=x; /*x1 -> posição onde desenhar a letras*/
  
  for(unsigned int i = 0; i<n;i++){
    draw_letter(sentence[i].sprite,x1,y,sentence[i].img,buff,blackChar);
    x1+=sentence[i].img.width + 1;
  }
  
  return 0;
}

int draw_letter(uint8_t *sprite, uint16_t x, uint16_t y, xpm_image_t img, void * buff, void * color) {
  int pixel_num = 0;
  for(int j = 0; j < img.height; j++){
    for (int i=0; i < img.width; i++){
      if(((uint16_t *)(img.bytes))[pixel_num] != xpm_transparency_color(img.type)){
        if(((uint16_t *)(img.bytes))[pixel_num] == *blackChar && (((char*)color)[1] != blackChar[1] || ((char*)color)[2] != blackChar[2]))
          draw_pixel(x+i,y+j,color,buff);
        else
          draw_pixel(x+i,y+j,&((uint16_t *)(img.bytes))[pixel_num],buff);
      }
      pixel_num++;
    }
  }
 
  return 0;
}

void draw_pixel (uint16_t x, uint16_t y, const void  *color, void * buff){
  if(x >= hres || y>= vres) return ; //deal with illegal dimensions - for xpm
  uint32_t byte = bits_per_pixel/8;
  uint32_t pos = (y* hres +x)*byte;
  uint8_t * pixel = (uint8_t *) buff + pos;

  char *clr = (char*) color;

  for(size_t i=0;i< byte; i++)
    pixel[i]=clr[i];
}

void double_buffering(){
	memcpy(video_mem, double_buf, (hres * vres * ceil(bits_per_pixel / 8.0)));
}

char* get_double_buffer() {
	return double_buf;
}

void free_double_buffer(){
  free(double_buf);
}
