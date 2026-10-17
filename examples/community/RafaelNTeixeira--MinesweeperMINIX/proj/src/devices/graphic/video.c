#include "video.h"

static vbe_mode_info_t vmi;
static void *base_address;
unsigned int x_res;
unsigned int y_res;
static uint16_t screen_center_x;
static uint16_t screen_center_y;

static mmap_t mem_map;

#pragma pack(1)
typedef struct 
{
  char VbeSignature[4];
  BCD VbeVersion[2];
  phys_bytes OemStringPtr;
  uint32_t Capabilities;
  phys_bytes VideoModePtr;
  uint16_t TotalMemory;

  BCD OemSoftwareRev[2];
  phys_bytes OemVendorNamePtr;
  phys_bytes OemProductNamePtr;
  phys_bytes OemProductRevPtr;
  char Reserved[222];
  char OemData[256];

} VbeInfoBlock;
#pragma options align=reset

phys_bytes (far_pointer)(phys_bytes pp) {
  phys_bytes msb = (pp & 0xFFFF0000) >> 12;
  phys_bytes lsb = pp & 0xFFFF;
  return (phys_bytes) mem_map.virt + (msb | lsb) - (phys_bytes) mem_map.phys;
}

void *(vg_init)(uint16_t mode){

  if(map_virtual_machine(mode) != 0) return NULL;

  if ((change_mode(mode)) != 0) return NULL;

  return base_address;
}

int(change_mode)(uint16_t mode){
    reg86_t reg;
    memset(&reg, 0, sizeof(reg));
    reg.intno = 0x10;
    reg.ax = AH_DEFAULT << 8 | MODE_SET; 
    reg.bx = 1 << 14 | mode;

    if (sys_int86(&reg) != CALL_SUCCESS) {
        printf("ERROR! Change_mode: sys_int86() failed \n");
        return 1;
    }

    return 0;
}

int(map_virtual_machine)(uint16_t mode){
    int r;
    struct minix_mem_range mem_range;

    if(vbe_get_mode_info(mode, &vmi) != 0) return 1;

    phys_bytes vram_base = vmi.PhysBasePtr;

  screen_center_x = vmi.XResolution/2;
  screen_center_y = vmi.YResolution/2;

    x_res = vmi.XResolution;
    y_res = vmi.YResolution;

    size_t vram_size = (vmi.XResolution * vmi.YResolution) * (vmi.BytesPerScanLine / vmi.XResolution); 
    
    mem_range.mr_base = vram_base;
    mem_range.mr_limit = mem_range.mr_base + vram_size;
    
    if(F_OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mem_range))) 
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    
    base_address = vm_map_phys(SELF, (void *) mem_range.mr_base, vram_size);
    
    if (base_address == MAP_FAILED) 
        panic("couldn't map video memory");

    return 0;
}

void draw_board_cell(xpm_image_t img, uint8_t x, uint8_t y) {
  draw_xpm(img, (img.width * x) + (screen_center_x - 5 * img.width), ((vmi.YResolution-55) - (y * img.height)));
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  if (y + height > vmi.YResolution)
    height = vmi.YResolution - y;
  if (x + width > vmi.XResolution)
    width = vmi.XResolution - x;

  for (int i = 0; i < height; i++) {
    vg_draw_hline(x, y, width, color);
    y++;
  }
  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t width, uint32_t color){
  
  for (uint16_t i = 0; i < width; i++){
    draw_pixel(x, y, color);
    x++;
  }
  return 0;
}

void (draw_pixel)(uint16_t x, uint16_t y, uint32_t color){  
  uint64_t offset = ((y * vmi.XResolution) + x) * (vmi.BytesPerScanLine / vmi.XResolution);
  void *addr = (void *)((char *) base_address + offset);

  memcpy(addr, &color, vmi.BytesPerScanLine / vmi.XResolution);
}

void vg_clear() {
  int vram_size = (vmi.XResolution * vmi.YResolution) 
    * (vmi.BytesPerScanLine / vmi.XResolution); 
  char *vmem = (char*) base_address;
  for (int i = 0; i < vram_size; i++)
    vmem[i] = 0;
}

int vg_draw_matrix(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step){
    uint width = x_res / no_rectangles;
    uint height = y_res / no_rectangles;
    uint x, y;
    bool is_indexed = (mode == IDX_1024x768_8);
    
    vg_clear();
    for (y = 0; y < no_rectangles; ++y) {
        for (x = 0; x < no_rectangles; ++x) {
            uint32_t color = is_indexed ? 
            get_indexed_color(y, x,first,no_rectangles,step) : 
            get_direct_color(y, x,first,no_rectangles,step);
            if (vg_draw_rectangle(x * width, y * height, width, height, color) != 0) return 1;
        }
    }

    return 0;
}

uint8_t (R)(uint32_t color) {
  return color >> vmi.RedFieldPosition;
}

uint8_t (G)(uint32_t color) {
  return color >> vmi.GreenFieldPosition;
}

uint8_t (B)(uint32_t color) {
  return color >> vmi.BlueFieldPosition;
}

uint32_t (build_RGB)(uint8_t r, uint8_t g, uint8_t b) {
  return ((r << vmi.RedFieldPosition) | (g << vmi.GreenFieldPosition) | b << vmi.BlueFieldPosition);
}

uint8_t (get_indexed_color)(uint row, uint col, uint32_t first, uint8_t no_rectangles, uint8_t step) {
  return (first + (row * no_rectangles + col) * step) % (1 << vmi.BitsPerPixel);
}

uint32_t (get_direct_color)(uint row, uint col, uint32_t first, uint8_t no_rectangles, uint8_t step) {
  uint8_t r, g, b;
  r = (R(first) + col * step) % (1 << vmi.RedMaskSize);
  g = (G(first) + row * step) % (1 << vmi.GreenMaskSize);
  b = (B(first) + (col + row) * step) % (1 << vmi.BlueMaskSize);

  return build_RGB(r, g, b);
}

void(draw_xpm)(xpm_image_t img, uint16_t x, uint16_t y) {
  if (y + img.height > vmi.YResolution)
    y = vmi.YResolution - img.height;
  if (x + img.width > vmi.XResolution)
    x = vmi.XResolution - img.width;

  uint64_t cnt = 0;
  uint8_t bpp = img.size / (img.height * img.width);

  for (int i = 0; i < img.height; i++) {
    for (int j = 0; j < img.width; j++) {
      uint64_t offset = (((y + i) * vmi.XResolution) + (x + j)) * bpp;

      void *addr = (void *) ((char *) base_address + offset);

      uint32_t color = 0;
      for (size_t off = 0; off < 3; off++)
        color |= *(img.bytes + (cnt * bpp) + off) << (off * 8);

      if (color == 0x00b140) {
        cnt++;
        continue;
      }
      
      memcpy(addr, &color, bpp);
      cnt++;
    }
  }
}


int vg_draw_board(xpm_image_t board) {
  uint8_t tetromino_side = (vmi.YResolution - 30)/16;
  uint8_t x = screen_center_x - (tetromino_side * 5) - 10;

  draw_xpm(board, x, 10);

  return EXIT_SUCCESS;
}

int (controller_info)(vg_vbe_contr_info_t * info) {
  VbeInfoBlock v;
  VbeInfoBlock *vbeBlock = &v;
  lm_alloc(sizeof(VbeInfoBlock), &mem_map);
  ((char*) mem_map.virt)[0] = 'V';
  ((char*) mem_map.virt)[1] = 'B';
  ((char*) mem_map.virt)[2] = 'E';
  ((char*) mem_map.virt)[3] = '2';

  reg86_t reg;
  memset(&reg,0,sizeof(reg));
  reg.ax = 0x4F00;
  reg.es = PB2BASE(mem_map.phys); /* set a segment base */
  reg.di = PB2OFF(mem_map.phys); /* set the offset accordingly */
  reg.intno = VIDEO_CARD;

  if (sys_int86(&reg) != 0) {return 1;}

  memcpy(vbeBlock, mem_map.virt, mem_map.size);

  info->VBESignature[0] = vbeBlock->VbeSignature[0];
  info->VBESignature[1] = vbeBlock->VbeSignature[1];
  info->VBESignature[2] = vbeBlock->VbeSignature[2];
  info->VBESignature[3] = vbeBlock->VbeSignature[3];

  info->VBEVersion[0] = vbeBlock->VbeVersion[0];
  info->VBEVersion[1] = vbeBlock->VbeVersion[1];

  info->TotalMemory = 64*vbeBlock->TotalMemory;
  info->OEMString = (char *)(far_pointer(vbeBlock->OemStringPtr));
  info->VideoModeList = (uint16_t *)(far_pointer(vbeBlock->VideoModePtr));
  info->OEMVendorNamePtr = (char *)(far_pointer(vbeBlock->OemVendorNamePtr));
  info->OEMProductNamePtr = (char *)(far_pointer(vbeBlock->OemProductNamePtr));
  info->OEMProductRevPtr = (char *)(far_pointer(vbeBlock->OemProductRevPtr));

  lm_free(&mem_map);

  return 0;
}
