#include <lcom/lcf.h>
#include "graphics.h"
#include "game.h"
#include <alphabet.h>

uint8_t *double_buffer = NULL;
uint8_t *frame_buffer = NULL;
unsigned int frame_size = 0;

int (graphics_mode)(uint16_t sub) {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86)); 
    reg86.al = 0x02;  
    reg86.ah = 0x4F;       
    reg86.intno = 0x10;               
    reg86.bx = sub | BIT(14);     
    if (sys_int86(&reg86) != 0) {     
        printf("Set graphics mode failed\n");
        return 1;
    }
    return 0;
}

int set_frame_buffer(uint16_t mode) {
  // Get VBE mode info
  memset(&information, 0, sizeof(information));
  if (vbe_get_mode_info(mode, &information)) return 1;

  unsigned int PixelBytes = (information.BitsPerPixel + 7) / 8;
  frame_size = information.XResolution * information.YResolution * PixelBytes;

  // Map physical video memory
  struct minix_mem_range phy_addr;
  phy_addr.mr_base = information.PhysBasePtr; 
  phy_addr.mr_limit = phy_addr.mr_base + frame_size;

  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &phy_addr)) {
      printf("Physical memory allocation error\n");
      return 1;
  }

  frame_buffer = vm_map_phys(SELF, (void*) phy_addr.mr_base, frame_size);
  if (frame_buffer == NULL) {
      printf("Virtual memory allocation error\n");
      return 1;
  }

  // Allocate double buffer in RAM (system memory)
  double_buffer = malloc(frame_size);
  if (double_buffer == NULL) {
      printf("Double buffer allocation error\n");
      return 1;
  }
  memset(double_buffer, 0, frame_size);

  return 0;
}

  

int vg_draw_pixel(int x, int y, uint32_t color) {
  if (x < 0 || x >= information.XResolution || y < 0 || y >= information.YResolution)
      return 1;

  unsigned int PixelBytes = (information.BitsPerPixel + 7) / 8;
  unsigned int index = (information.XResolution * y + x) * PixelBytes;

  if (information.BitsPerPixel == 8) {
      uint8_t c = (uint8_t)(color & 0xFF);
      double_buffer[index] = c;
  } else {
      memcpy(&double_buffer[index], &color, PixelBytes);
  }
  return 0;
}


int vg_draw_line(uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint8_t* buffer) {
  for (unsigned i = 0; i < len; i++)
      if (vg_draw_pixel_buffer(buffer, x + i, y, color) != 0) return 1;
  return 0;
}


int my_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t* buffer) {
  for (unsigned i = 0; i < height; i++)
      if (vg_draw_line(x, y + i, width, color, buffer) != 0)
          return 1;
  return 0;
}

uint32_t (direct_mode)(uint32_t R, uint32_t G, uint32_t B) {
    return (R << information.RedFieldPosition) | (G << information.GreenFieldPosition) | (B << information.BlueFieldPosition);
  }
  
uint32_t (indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n) {
    return (first + (row * n + col) * step) % (1 << information.BitsPerPixel);
}

uint32_t (Red)(unsigned j, uint8_t step, uint32_t first) {
    return (R(first) + j * step) % (1 << information.RedMaskSize);
  }
  
uint32_t (Green)(unsigned i, uint8_t step, uint32_t first) {
    return (G(first) + i * step) % (1 << information.GreenMaskSize);
}
  
uint32_t (Blue)(unsigned j, unsigned i, uint8_t step, uint32_t first) {
    return (B(first) + (i + j) * step) % (1 << information.BlueMaskSize);
}
  
uint32_t (R)(uint32_t first){
    return ((1 << information.RedMaskSize) - 1) & (first >> information.RedFieldPosition);
}
  
uint32_t (G)(uint32_t first){
    return ((1 << information.GreenMaskSize) - 1) & (first >> information.GreenFieldPosition);
}
  
uint32_t (B)(uint32_t first){
    return ((1 << information.BlueMaskSize) - 1) & (first >> information.BlueFieldPosition);
}

int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y, uint8_t *buffer) {

    xpm_image_t img;
    
  
    uint8_t *pixmap = xpm_load(xpm, XPM_8_8_8, &img);

    for (int h = 0 ; h < img.height ; h++) {
      for (int w = 0 ; w < img.width ; w++) {
            int idx = (h*img.width+w)*3;
            uint8_t b = pixmap[idx];
            uint8_t g = pixmap[idx+1];
            uint8_t r = pixmap[idx+2];
            uint32_t color = direct_mode(r,g,b);
            if(r==0xAD && g==0xD8 && b==0xE6) {
                continue;
            }
            if (color != 0x000000) {
                vg_draw_pixel_buffer(buffer, x + w, y + h, color);
            }
      }
    }
    return 0;
  }

  const xpm_row_t* get_alphabet_xpm(char c) {
    return get_alphabet_xpm_highlight(c, 0);
}

  const xpm_row_t* get_alphabet_xpm_highlight(char c, int highlight) {
    switch (c) {
        case 'A': 
        if (highlight==1) return A_w;
        else if (highlight==2) return A_b;
        else return A;
        case 'B': 
        if (highlight==1) return BB_w;
        else if (highlight==2) return BB_b;
        else return BB;
        case 'C': 
        if (highlight==1) return C_w;
        else if (highlight==2) return C_b;
        else return C;
        case 'D': 
        if (highlight==1) return D_w;
        else if (highlight==2) return D_b;
        else return D;
        case 'E': 
        if (highlight==1) return E_w;
        else if (highlight==2) return E_b;
        else return E;
        case 'F': 
        if (highlight==1) return F_w;
        else if (highlight==2) return F_b;
        else return F;
        case 'G': 
        if (highlight==1) return GG_w;
        else if (highlight==2) return GG_b;
        else return GG;
        case 'H': 
        if (highlight==1) return H_w;
        else if (highlight==2) return H_b;
        else return H;
        case 'I': 
        if (highlight==1) return I_w;
        else if (highlight==2) return I_b;
        else return I;
        case 'J': 
        if (highlight==1) return J_w;
        else if (highlight==2) return J_b;
        else return J;
        case 'K': 
        if (highlight==1) return K_w;
        else if (highlight==2) return K_b;
        else return K;
        case 'L': 
        if (highlight==1) return L_w;
        else if (highlight==2) return L_b;
        else return L;
        case 'M': 
        if (highlight==1) return M_w;
        else if (highlight==2) return M_b;
        else return M;
        case 'N': 
        if (highlight==1) return N_w;
        else if (highlight==2) return N_b;
        else return N;
        case 'O': 
        if (highlight==1) return O_w;
        else if (highlight==2) return O_b;
        else return O;
        case 'P': 
        if (highlight==1) return P_w;
        else if (highlight==2) return P_b;
        else return P;
        case 'Q': 
        if (highlight==1) return Q_w;
        else if (highlight==2) return Q_b;
        else return Q;
        case 'R': 
        if (highlight==1) return RR_w;
        else if (highlight==2) return RR_b;
        else return RR;
        case 'S': 
        if (highlight==1) return S_w;
        else if (highlight==2) return S_b;
        else return S;
        case 'T': 
        if (highlight==1) return T_w;
        else if (highlight==2) return T_b;
        else return T;
        case 'U': 
        if (highlight==1) return U_w;
        else if (highlight==2) return U_b;
        else return U;
        case 'V': 
        if (highlight==1) return V_w;
        else if (highlight==2) return V_b;
        else return V;
        case 'W': 
        if (highlight==1) return W_w;
        else if (highlight==2) return W_b;
        else return W;
        case 'X': 
        if (highlight==1) return X_w;
        else if (highlight==2) return X_b;
        else return X;
        case 'Y': 
        if (highlight==1) return Y_w;
        else if (highlight==2) return Y_b;
        else return Y;
        case 'Z': 
        if (highlight==1) return Z_w;
        else if (highlight==2) return Z_b;
        else return Z;        
        case '0':
        if (highlight==1) return zero_w;
        else if (highlight==2) return zero_b;
        return zero;
        case '1':
        if (highlight==1) return one_w;
        else if (highlight==2) return one_b;
         return one;
        case '2':
        if (highlight==1) return two_w;
        else if (highlight==2) return two_b; 
        return two;
        case '3':
        if (highlight==1) return three_w;
        else if (highlight==2) return three_b;
         return three;
        case '4':
        if (highlight==1) return four_w;
        else if (highlight==2) return four_b;
         return four;
        case '5':
        if (highlight==1) return five_w;
        else if (highlight==2) return five_b; 
        return five;
        case '6':
        if (highlight==1) return six_w;
        else if (highlight==2) return six_b; 
        return six;
        case '7': 
        if (highlight==1) return seven_w;
        else if (highlight==2) return seven_b;
        return seven;
        case '8': 
        if (highlight==1) return eight_w;
        else if (highlight==2) return eight_b;
        return eight;
        case '9': 
        if (highlight==1) return nine_w;
        else if (highlight==2) return nine_b;
        return nine;
        case ' ': return space;

        default: return NULL;
    }
}

void print_xpm_string_centered(const char *str, int y, uint8_t *buffer, int screen_width){
        print_xpm_string_centered_highlight(str, y, buffer, screen_width, 0);
    }


  void print_xpm_string_centered_highlight(const char *str, int y, uint8_t *buffer, int screen_width, int highlight) {

    int letter_width = 37;  // Replace with your actual letter width
    int spacing = -2;        // Adjust as desired

    // Calculate how many actual letters (not spaces) in str
    int count = 0, space_count = 0;
    for (const char *p = str; *p; p++) {
        if (*p == ' ') space_count++;
        else count++;
    }
    if (count == 0) return;

    // Total pixel width
    int total_width = count * (letter_width + spacing) - spacing + space_count * (letter_width / 2);
    int x = (screen_width - total_width) / 2;

    // Draw each character
    for (const char *p = str; *p; ++p) {
        char c = *p;
        if (c == ' ') {
            x += letter_width / 2; // smaller space for spaces
            continue;
        }
        if (c >= 'a' && c <= 'z') c -= ('a' - 'A'); // convert to uppercase
        xpm_map_t letter_xpm = get_alphabet_xpm_highlight(c, highlight);
        if (letter_xpm) {
            print_xpm(letter_xpm, x, y, buffer);
        }
        x += letter_width + spacing;
    }
}



  void draw_text_centered(const char* text, int y, uint32_t color, uint8_t* buffer) {
    int len = strlen(text);
    int char_width = 16;
    int x_start = (information.XResolution - len * char_width) / 2;

    for (int i = 0; i < len; i++) {
        vg_draw_char(text[i], x_start + i * char_width, y, color, buffer);
    }
}

int vg_draw_char(char c, uint16_t x, uint16_t y, uint32_t color, uint8_t* buffer) {
  if (c < 0 || c > 127) return 1;
  for (int row = 0; row < 8; row++) {
      uint8_t row_data = font8x8_basic[(int)c][row];
      for (int col = 0; col < 8; col++) {
          if (row_data & (1 << (7 - col))) {
              vg_draw_pixel_buffer(buffer, x + col * 2,     y + row * 2,  color);
              vg_draw_pixel_buffer(buffer, x + col * 2 + 1, y + row * 2,     color);
              vg_draw_pixel_buffer(buffer, x + col * 2,     y + row * 2 + 1, color);
              vg_draw_pixel_buffer(buffer, x + col * 2 + 1, y + row * 2 + 1, color);
          }
      }
  }
  return 0;
}

int vg_draw_pixel_buffer(uint8_t *buffer, int x, int y, uint32_t color) {
  if (x < 0 || x >= information.XResolution || y < 0 || y >= information.YResolution)
      return 1;

  unsigned int PixelBytes = (information.BitsPerPixel + 7) / 8;
  unsigned int index = (information.XResolution * y + x) * PixelBytes;

  if (information.BitsPerPixel == 8) {
      buffer[index] = (uint8_t)(color & 0xFF);
  } else {
      memcpy(&buffer[index], &color, PixelBytes);
  }
  return 0;
}


