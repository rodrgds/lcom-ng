#include "vbe.h"

uint32_t mouse_array[12][12];

void *(vg_init)(uint16_t mode) {
  mr.mr_base = 0;
  mr.mr_limit = 1048576;

  vbe_mode_info_t info;
  vg_get_mode_info(mode, &info);
  phys_addr = (phys_bytes)info.PhysBasePtr;
  h_res = info.XResolution;
  v_res = info.YResolution;
  bitsPerPixel = info.BitsPerPixel;
  bytesPerPixel = bitsPerPixel / 8;

  if (bitsPerPixel % 8 != 0)
      bytesPerPixel++;

  red_s = info.RedMaskSize;
  red_p = info.RedFieldPosition;
  green_s = info.GreenMaskSize;
  green_p = info.GreenFieldPosition;
  blue_s = info.BlueMaskSize;
  blue_p = info.BlueFieldPosition;
  memoryModel = info.MemoryModel;
  vram_size = h_res * v_res * bytesPerPixel;
  vram_base = info.PhysBasePtr;

  //Double Buffer
  double_buffer = malloc(vram_size);

  /* Allow memory mapping */
  mr.mr_base = phys_addr;
  mr.mr_limit = phys_addr + (h_res * v_res * bytesPerPixel);
  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
      panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if (video_mem == MAP_FAILED)
      panic("couldn’t map video memory");

  /* Set VBE graphics mode */
  reg86_t reg86;
  memset(&reg86, 0, sizeof(reg86)); // Zero the structure
  reg86.ax = 0x4F02;                // VBE call, function 02: set VBE mode
  reg86.bx = 1 << 14 | mode;        // Set bit 14: linear framebuffer
  reg86.intno = 0x10;

  if (sys_int86(&reg86) != OK) {
      printf("set_vbe_mode: sys_int86() failed \n");
      return NULL;
  }

  return video_mem;
}



int (vg_get_mode_info)(uint16_t mode, vbe_mode_info_t * vmi_p){
    mmap_t map;

    lm_alloc(MODEINFO_SIZE, &map);

    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));
    reg86.intno = 0x10;
    reg86.ax = 0x4F01;
    reg86.cx = mode;
    reg86.es = PB2BASE(map.phys);
    reg86.di = PB2OFF(map.phys);
    if (sys_int86(&reg86) != OK) return 1;

    memcpy(vmi_p, map.virt, MODEINFO_SIZE);

    lm_free(&map);

    return 0;
}


uint32_t getmask(int k){
  uint32_t mask = 0xFF << 8*k;
  return mask; 
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color)
{
    if (x < h_res && y < v_res)
    {
      char* init_address = (char*) double_buffer + (y * h_res + x)* bytesPerPixel;
      
      for(int k = 0; k < bytesPerPixel; k++){ 
       init_address[k] = (color & getmask(k)) >> (8*k);
      }
        return 0;
    }
    return 1;
}

int(vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color)
{
    for (unsigned int i = y; i < (y + len); i++)
    {
        vg_draw_pixel(x, i, color);
    }
    return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color)
{
    for (unsigned int i = x; i < (x + len); i++)
    {
        vg_draw_pixel(i, y, color);
    }
    return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
{
    for (unsigned int i = y; i < y + height; i++)
    {
        vg_draw_hline(x, i, width, color);
    }
    return 0;
}

int(vg_draw_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step)
{
    uint32_t color_temp/*,red,green,blue;
    uint32_t initial_blue = (first & getmask(1)) >> 8;
    uint32_t initial_green = (first & getmask(2)) >> 16;
    uint32_t initial_red = (first & getmask(3)) >> 24*/;
    for (unsigned int col = 0; col < no_rectangles; col++) // Col
    {
        for (unsigned int row = 0; row < no_rectangles; row++) // Row
        {
            if (memoryModel != 0x06) // Indexed Color Model
            {
                color_temp = (first + (row * no_rectangles + col) * step) % (1 << bitsPerPixel);
            }
            else // Direct Color Model
            {
                color_temp = ((uint8_t) first + (col + row) * step) % (1 << blue_s);
                color_temp += (((uint8_t)(first >> blue_s) + row * step) % (1 << (green_s)) << blue_s);
                color_temp += (((uint8_t)(first >> (blue_s + green_s)) + col * step) % (1 << (red_s)) << (blue_s + green_s));
            }
            printf("COLOR : %x\n",color_temp);
            vg_draw_rectangle(col * (h_res / no_rectangles), row * (v_res / no_rectangles), (h_res / no_rectangles), (v_res / no_rectangles), color_temp);
        }
    }
    return 0;
}

int(vg_draw_pixmap)(xpm_map_t xpm, uint16_t x, uint16_t y)
{
    // Can be XPM_INDEXED, XPM_1_5_5_5, XPM_5_6_5, XPM_8_8_8 or XPM_8_8_8_8
    enum xpm_image_type type = XPM_8_8_8;
    xpm_image_t img;
    uint8_t *map = xpm_load(xpm, type, &img);
    if(map == NULL) {
        printf("ERROR::xpm_load failed!\n");
        return 1;
    }
    /* Print Sprite */
    for (unsigned int i = 0; i < img.height; i++)
    {
        for (unsigned int j = 0; j < img.width; j++)
        {
            vg_draw_pixel(x + j, y + i, *(map + img.width * i + j));
        }
    }
    /* */
    return 0;
}
int (vg_ultimate_pixmap_handler)(uint16_t x, uint16_t y,uint16_t mode, enum pixmap pixtype){
    uint32_t title_color = 0, score_color = 0, cursor_color = 0, enemy_color = 0,
     enemy_eye_color = 0, death_color = 0, apple_color = 0, number_color = 0,background_color = 0;
    
    switch (mode)
    {
    case 0x110:
        title_color = SCORE_BAR_COLOR_110;
        score_color = SCORE_COLOR_110;
        cursor_color = MOUSE_CURSOR_COLOR_110;
        enemy_color = ENEMY_COLOR_110;
        enemy_eye_color = ENEMY_EYE_COLOR_110;
        death_color = DEATH_COLOR_110;
        apple_color = APPLE_COLOR_110;
        number_color = SCORE_COLOR_110;
        background_color = MENU_BACKGORUND_110;
        break;
    case 0x11A:
        title_color = SCORE_BAR_COLOR_11A;
        score_color = SCORE_COLOR_11A;
        cursor_color = MOUSE_CURSOR_COLOR_11A;
        enemy_color = ENEMY_COLOR_11A;
        enemy_eye_color = ENEMY_EYE_COLOR_11A;
        death_color = DEATH_COLOR_11A;
        apple_color = APPLE_COLOR_11A;
        number_color = SCORE_COLOR_11A;
        background_color = MENU_BACKGORUND_11A;
        break;
    case 0x115:
    case 0x14C:
        title_color = SCORE_BAR_COLOR;
        score_color = SCORE_COLOR;
        cursor_color = MOUSE_CURSOR_COLOR;
        enemy_color = ENEMY_COLOR;
        enemy_eye_color = ENEMY_EYE_COLOR;
        death_color = DEATH_COLOR;
        apple_color = APPLE_COLOR;
        number_color = SCORE_COLOR;
        background_color = MENU_BACKGORUND;
        break;
    case 0x105:
        title_color = SCORE_BAR_COLOR_105;
        score_color = SCORE_COLOR_105;
        cursor_color = MOUSE_CURSOR_COLOR_105;
        enemy_color = ENEMY_COLOR_105;
        enemy_eye_color = ENEMY_EYE_COLOR_105;
        death_color = DEATH_COLOR_105;
        apple_color = APPLE_COLOR_105;
        number_color = SCORE_COLOR_105;
        background_color = MENU_BACKGORUND_105;
        break;
    default:
        break;
    }
    unsigned int height = 0, width = 0;
    if (pixtype == SCORE) {
        height = 14;
        width = 154;
    }
    if (pixtype == CROSSHAIR){
        height = 12;
        width = 12;
    }
    if (pixtype == ENEMY || pixtype == DEAD_ENEMY || pixtype == APPLE){
        height = 20;
        width = 20;
    }
    if (pixtype == NUMBER0 || pixtype == NUMBER1 || pixtype == NUMBER2 || pixtype == NUMBER3 || 
        pixtype == NUMBER4 || pixtype == NUMBER5 || pixtype == NUMBER6 || pixtype == NUMBER7 ||
        pixtype == NUMBER8 || pixtype == NUMBER9 || pixtype == TWO_POINTS || pixtype == SLASH){
            height = 15;
            width = 26;
    }
    if (pixtype == GAMEOVER || pixtype == EXIT || pixtype == PLAY || pixtype == TITLE) height = 37;
    if (pixtype == EXIT) width = 207;
    if (pixtype == GAMEOVER) width = 234;
    if (pixtype == PLAY) width = 271;
    if (pixtype == TITLE) width = 409;

    if(pixtype == GO_BACK_1){
        width = 246;
        height = 16;
    }
    else if(pixtype == GO_BACK_2){
        width = 360;
        height = 16;
    }
    
    for (unsigned int i = 0; i < height; i++)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            if (pixtype == SCORE) if (score[i][j] == '.') vg_draw_pixel(x + j, y + i, score_color);

            if (pixtype == TITLE) if (title[i][j] == '.') vg_draw_pixel(x + j, y + i, title_color);

            if (pixtype == GAMEOVER) if (gameOver[i][j] == '.') vg_draw_pixel(x + j, y + i, title_color);

            if (pixtype == PLAY){
                if (play[i][j] == '.') vg_draw_pixel(x + j, y + i, title_color);
                else if(play[i][j] == '0') vg_draw_pixel(x + j, y + i, background_color);
            }

            if (pixtype == EXIT){
                if (exitPix[i][j] == '.') vg_draw_pixel(x + j, y + i, title_color);
                else if(exitPix[i][j] == '0') vg_draw_pixel(x + j, y + i, background_color);
            }

            if (pixtype == CROSSHAIR) if (crosshair[i][j] == '.') {
                memcpy(&mouse_array[i][j], (char*) double_buffer + ((y+i) * h_res + (x+j))*bytesPerPixel, bytesPerPixel);
                
                vg_draw_pixel(x + j, y + i, cursor_color);
            }

            if (pixtype == ENEMY){
                if (enemyPix[i][j] == '.') vg_draw_pixel(x + j, y + i, enemy_color);
                else if (enemyPix[i][j] == '0') vg_draw_pixel(x + j, y + i, enemy_eye_color);  
            }
            if (pixtype == DEAD_ENEMY) if (enemyPix[i][j] == '.' || enemyPix[i][j] == '0')  vg_draw_pixel(x + j, y + i, death_color);

            if (pixtype == APPLE){
                if (applePix[i][j] == '.') vg_draw_pixel(x + j, y + i, apple_color);
                else if (applePix[i][j] == '0') vg_draw_pixel(x + j, y + i, enemy_eye_color);
            }
            
            if (pixtype == NUMBER0) if (number0[i][j] == '.') vg_draw_pixel(x + j, y + i, number_color);
            if (pixtype == NUMBER1) if (number1[i][j] == '.') vg_draw_pixel(x + j, y + i, number_color);
            if (pixtype == NUMBER2) if (number2[i][j] == '.') vg_draw_pixel(x + j, y + i, number_color);
            if (pixtype == NUMBER3) if (number3[i][j] == '.') vg_draw_pixel(x + j, y + i, number_color);
            if (pixtype == NUMBER4) if (number4[i][j] == '.') vg_draw_pixel(x + j, y + i, number_color);
            if (pixtype == NUMBER5) if (number5[i][j] == '.') vg_draw_pixel(x + j, y + i, number_color);
            if (pixtype == NUMBER6) if (number6[i][j] == '.') vg_draw_pixel(x + j, y + i, number_color);
            if (pixtype == NUMBER7) if (number7[i][j] == '.') vg_draw_pixel(x + j, y + i, number_color);
            if (pixtype == NUMBER8) if (number8[i][j] == '.') vg_draw_pixel(x + j, y + i, number_color);
            if (pixtype == NUMBER9) if (number9[i][j] == '.') vg_draw_pixel(x + j, y + i, number_color);
            if (pixtype == TWO_POINTS) if (doublePoint[i][j] == '.') vg_draw_pixel(x + j, y + i, number_color);
            if (pixtype == SLASH) if (slash[i][j] == '.') vg_draw_pixel(x + j, y + i, number_color);

            if (pixtype == GO_BACK_1){
                if (gameOverGoBack[i][j] == '.') vg_draw_pixel(x + j, y + i, apple_color);
            }
            if (pixtype == GO_BACK_2){
                if (gameOverGoBack2[i][j] == '.') vg_draw_pixel(x + j, y + i, apple_color);
            }
        }
    }
    return 0;
}

int (vg_ultimate_pixmap_eraser)(uint16_t x, uint16_t y,uint16_t mode, enum pixmap pixtype, bool mouse){
    uint32_t arena_color = 0, board_color = 0;
    
    switch (mode)
    {
    case 0x110:
        arena_color = ARENA_BACKGROUND_COLOR_110;
        board_color = SCORE_BAR_COLOR_110;
        break;
    case 0x11A:
        arena_color = ARENA_BACKGROUND_COLOR_11A;
        board_color = SCORE_BAR_COLOR_11A;
        break;
    case 0x115:
    case 0x14C:
        arena_color = ARENA_BACKGROUND_COLOR;
        board_color = SCORE_BAR_COLOR;
        break;
    case 0x105:
        arena_color = ARENA_BACKGROUND_COLOR_105;
        board_color = SCORE_BAR_COLOR_105;
        break;
    default:
        break;
    }
    unsigned int height = 0, width = 0;
    if (pixtype == CROSSHAIR){
        height = 12;
        width = 12;
    }
    else if (pixtype == ENEMY || pixtype == APPLE){
        height = 20;
        width = 20;
    }
    else if (pixtype == NUMBER0 || pixtype == NUMBER1 || pixtype == NUMBER2 || pixtype == NUMBER3 || 
        pixtype == NUMBER4 || pixtype == NUMBER5 || pixtype == NUMBER6 || pixtype == NUMBER7 || pixtype == NUMBER8 || pixtype == NUMBER9){
            height = 15;
            width = 26;
    }
    for (unsigned int i = 0; i < height; i++)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            if (pixtype == CROSSHAIR) if (crosshair[i][j] == '.') {
                if (!mouse) vg_draw_pixel(x + j, y + i, mouse_array[i][j]);
                else vg_draw_pixel(x + j, y + i, arena_color);
            } 

            if (pixtype == ENEMY) if (enemyPix[i][j] == '.' || enemyPix[i][j] == '0') vg_draw_pixel(x + j, y + i, arena_color);

            if (pixtype == APPLE) if (applePix[i][j] == '.' || enemyPix[i][j] == '0')  vg_draw_pixel(x + j, y + i, arena_color);

            if (pixtype == NUMBER0) if (number0[i][j] == '.') vg_draw_pixel(x + j, y + i, board_color);
            if (pixtype == NUMBER1) if (number1[i][j] == '.') vg_draw_pixel(x + j, y + i, board_color);
            if (pixtype == NUMBER2) if (number2[i][j] == '.') vg_draw_pixel(x + j, y + i, board_color);
            if (pixtype == NUMBER3) if (number3[i][j] == '.') vg_draw_pixel(x + j, y + i, board_color);
            if (pixtype == NUMBER4) if (number4[i][j] == '.') vg_draw_pixel(x + j, y + i, board_color);
            if (pixtype == NUMBER5) if (number5[i][j] == '.') vg_draw_pixel(x + j, y + i, board_color);
            if (pixtype == NUMBER6) if (number6[i][j] == '.') vg_draw_pixel(x + j, y + i, board_color);
            if (pixtype == NUMBER7) if (number7[i][j] == '.') vg_draw_pixel(x + j, y + i, board_color);
            if (pixtype == NUMBER8) if (number8[i][j] == '.') vg_draw_pixel(x + j, y + i, board_color);
            if (pixtype == NUMBER9) if (number9[i][j] == '.') vg_draw_pixel(x + j, y + i, board_color);
        }
    }
    return 0;
}

void(vg_erase_pixmap)(xpm_map_t xpm, uint16_t x, uint16_t y)
{
    if (x < h_res && y < v_res)
    {
        // Can be XPM_INDEXED, XPM_1_5_5_5, XPM_5_6_5, XPM_8_8_8 or XPM_8_8_8_8
        enum xpm_image_type type = XPM_INDEXED;
        xpm_image_t img;
        uint8_t *sprite = xpm_load(xpm, type, &img);

        /*Print Sprite */
        for (unsigned int i = 0; i < img.height; i++)
        {
            for (unsigned int j = 0; j < img.width; j++)
            {
                vg_draw_pixel(j + x, i + y, 0 * (*sprite + 1));
            }
        }
    }
}

uint8_t *(vg_create_sprite)(xpm_map_t xpm, xpm_image_t *img)
{
    // Can be XPM_INDEXED, XPM_1_5_5_5, XPM_5_6_5, XPM_8_8_8 or XPM_8_8_8_8
    enum xpm_image_type type = XPM_INDEXED;
    uint8_t *sprite = xpm_load(xpm, type, img);
    return sprite;
}

int(vg_draw_sprite)(uint8_t *sprite, xpm_image_t img, uint16_t x, uint16_t y)
{
    for (unsigned int i = 0; i < img.height; i++)
    {
        for (unsigned int j = 0; j < img.width; j++)
        {
            vg_draw_pixel(j + x, i + y, *(sprite + i * img.width + j));
        }
    }
    return 0;
}

int(vg_erase_sprite)(uint8_t *sprite, xpm_image_t img, uint16_t x, uint16_t y)
{
    for (unsigned int i = 0; i < img.height; i++)
    {
        for (unsigned int j = 0; j < img.width; j++)
        {
            vg_draw_pixel(j + x, i + y, 0);
        }
    }
    return 0;
}

void (swapBuffer)(){
    memcpy(video_mem, double_buffer, h_res*v_res*bytesPerPixel);
}




