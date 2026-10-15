#include <lcom/lcf.h>

#include "controller/graphics/vbe.h"

/* static global variable to store vbe mode information.
 * this structure holds details about the selected video mode */
static vbe_mode_info_t mode_info;
// double buffer index
static uint8_t buff_index = 0;
/* static global pointer to the mapped video memory in the process's address space.
 * in minix 3, vram is not directly accessible and needs to be mapped */
static uint8_t *video_mem[3] = {NULL, NULL, NULL};

// auxiliary global variables
static uint16_t h_res = 0;
static uint16_t v_res = 0;
static uint8_t bytes_per_pixel = 0;
static uint32_t vram_size = 0;

vbe_mode_info_t(vbe_get_mode)(void) {
  return mode_info;
}

uint8_t *(vbe_get_video_mem) (void) {
  return video_mem[buff_index];
}

uint16_t(vbe_get_h_res)(void) {
  return h_res;
}

uint16_t(vbe_get_v_res)(void) {
  return v_res;
}

uint8_t(vbe_get_bytes_per_pixel)(void) {
  return bytes_per_pixel;
}

uint32_t(vbe_get_vram_size)(void) {
  return vram_size;
}

int(vbe_get_mode_information)(uint16_t mode, vbe_mode_info_t *vmi) {
  if (vmi == NULL) {
    fprintf(stderr, "vbe_get_mode_information: vmi pointer cannot be null.");
    return 1;
  }

  mmap_t map;
  if (memset(&map, 0, sizeof(map)) == NULL) {
    perror("vbe_get_mode_information: failed to clear mmap_t.");
    return 1;
  }
  if (lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL) {
    perror("vbe_get_mode_information: failed to allocate memory.");
    return 1;
  }

  struct reg86 args;
  if (memset(&args, 0, sizeof(args)) == NULL) {
    perror("vbe_get_mode_information: failed to clear reg86.");
    lm_free(&map);
    return 1;
  }

  args.ah = VBE_FUNCTION;      // indicate a vbe function call
  args.al = VBE_RET_MODE;      // function 01h - return VBE mode information
  args.cx = mode;              // mode number
  args.es = PB2BASE(map.phys); // segment base address
  args.di = PB2OFF(map.phys);  // offset within segment
  args.intno = VBE_INT;        // video BIOS interrupt

  // call sys_int86 to invoke the bios interrupt with the specified register values
  if (sys_int86(&args) != 0) {
    perror("vbe_get_mode_information: failed to call sys_int86.");
    lm_free(&map);
    return 1;
  }

  if (args.ax != VBE_FUNCTION) { // AH should be 0x4F if successful
    fprintf(stderr, "vbe_get_mode_information: BIOS call failed");
    lm_free(&map);
    return 1;
  }

  *vmi = *(vbe_mode_info_t *) map.virt;

  if (lm_free(&map) != true) {
    perror("vbe_get_mode_information: failed to free map mem.");
    return 1;
  }
  return 0;
}

int(vbe_exit)(void) {
  // holds the arguments for the bios interrupt call
  struct reg86 args;
  // clear the reg86 structure to avoid unexpected behavior
  if (memset(&args, 0, sizeof(args)) == NULL) {
    perror("vbe_exit: failed to clear reg86.");
    return 1;
  }

  // set video mode function
  args.ah = VBE_SET_VIDEO_MODE;
  // 80x25 text mode
  args.al = VBE_MODE_80x25_TEXT;
  // set intno to VBE_INT (0x10) to specify the bios video services interrupt
  args.intno = VBE_INT;

  // call sys_int86 to invoke the bios interrupt with the specified register values
  if (sys_int86(&args) != 0) {
    perror("vbe_exit: failed to call sys_int86.");
    return 1;
  }

  return 0;
}

int(vbe_set_video_mode)(uint16_t mode) {
  // holds the arguments for the bios interrupt call
  struct reg86 args;
  // clear the reg86 structure to avoid unexpected behavior
  if (memset(&args, 0, sizeof(args)) == NULL) {
    perror("vbe_set_video_mode: failed to clear reg86.");
    return 1;
  }

  // set ah register to VBE_FUNCTION (0x4f) to indicate a vbe function call
  args.ah = VBE_FUNCTION;
  // set al register to VBE_SET_MODE (0x02) to select the "set vbe mode" function
  args.al = VBE_SET_MODE;
  /* set bx register to the desired video mode number ored with vbe_linear_mode.
   * setting bit 14 of bx enables the linear frame buffer model for easier vram access */
  args.bx = mode | VBE_LINEAR_MODE;
  // set intno to VBE_INT (0x10) to specify the bios video services interrupt
  args.intno = VBE_INT;

  // call sys_int86 to invoke the bios interrupt with the specified register values
  if (sys_int86(&args) != 0) {
    perror("vbe_set_video_mode: failed to call sys_int86.");
    return 1;
  }

  /* check the return status in ah and al registers after the vbe call.
   * al should be 0x4f if the vbe function is supported, and ah should be 0x00 for success */
  if (args.ah != VBE_CALL_SUCCESS || args.al != VBE_FUNCTION) {
    // check if ah indicates a failed function call (0x01).
    if (args.ah == VBE_CALL_FAIL) {
      fprintf(stderr, "vbe_set_video_mode: vbe call fail.");
    }
    // check if ah indicates the function is not supported in the current hardware configuration (0x02)
    else if (args.ah == VBE_CALL_NOT_SUPPORTED) {
      fprintf(stderr, "vbe_set_video_mode: vbe call not supported.");
    }
    // check if ah indicates the function is invalid in the current video mode (0x03)
    else if (args.ah == VBE_CALL_INVALID) {
      fprintf(stderr, "vbe_set_video_mode: vbe call invalid.");
    }
    // handle other non-success return codes in ah
    else {
      fprintf(stderr, "vbe_set_video_mode: function not supported.");
    }
    return 1;
  }

  return 0;
}

int(vbe_map_vram)(uint16_t mode) {
  /* check if setting the memory of the mode_info structure to zero fails.
   * mode_info will store the video mode information */
  if (memset(&mode_info, 0, sizeof(mode_info)) == NULL) {
    perror("vbe_map_vram: failed to clear mode_info.");
    return 1;
  }
  /* call the vbe_get_mode_info function to retrieve information about the specified video mode.
   * this function is provided by the lcf */
  if (vbe_get_mode_information(mode, &mode_info) != 0) {
    fprintf(stderr, "vbe_map_vram: failed to get mode info.");
    return 1;
  }

  // initialize the auxiliary global variables
  h_res = mode_info.XResolution;
  v_res = mode_info.YResolution;
  bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
  vram_size = h_res * v_res * bytes_per_pixel;

  // declare a struct minix_mem_range to define the physical memory range
  struct minix_mem_range mr;
  // check if setting the memory of the minix_mem_range structure to zero fails
  if (memset(&mr, 0, sizeof(mr)) == NULL) {
    perror("vbe_map_vram: failed to clear minix_mem_range.");
    return 1;
  }

  // set the base physical address of the memory range to the physical base pointer obtained from mode_info.
  mr.mr_base = (phys_bytes) mode_info.PhysBasePtr;
  // set the limit of the memory range to the base address plus the total vram size.
  mr.mr_limit = mr.mr_base + 3 * vram_size;

  // request permission to map the specified physical memory range into the process's address space using the sys_privctl kernel call
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != 0) {
    perror("vbe_map_vram: failed to request permission.");
    return 1;
  }

  // map the physical memory region into the process's virtual address space using the vm_map_phys kernel call.
  for (uint8_t i = 0; i < 3; i++) {
    video_mem[i] = (uint8_t *) vm_map_phys(SELF, (void *) (mr.mr_base + i * vram_size), vram_size);
  }

  // check if the memory mapping failed.
  if ((void *) video_mem[0] == MAP_FAILED || (void *) video_mem[1] == MAP_FAILED || (void *) video_mem[2] == MAP_FAILED) {
    fprintf(stderr, "vbe_map_vram: map failed.");
    return 1;
  }

  return 0;
}

int(vbe_flip_page)(void) {
  // holds the arguments for the bios interrupt call
  struct reg86 args;
  // clear the reg86 structure to avoid unexpected behavior
  if (memset(&args, 0, sizeof(args)) == NULL) {
    perror("vbe_flip_page: failed to clear reg86.");
    return 1;
  }

  args.ah = VBE_FUNCTION;
  args.al = VBE_SET_DISPLAY_START_CTRL;
  // set display start during vertical retrace (prevents tearing)
  args.bl = VBE_SET_DISPLAY_START_VERTICAL;
  // first displayed pixel in scan line (no horizontal panning)
  args.cx = 0;
  // first displayed scan line, calculated to show the appropriate vertical section of VRAM
  args.dx = buff_index * v_res;
  // video BIOS interrupt
  args.intno = VBE_INT;

  // call sys_int86 to invoke the bios interrupt with the specified register values
  if (sys_int86(&args) != 0) {
    perror("vbe_flip_page: failed to call sys_int86.");
    return 1;
  }

  if (args.ah != VBE_CALL_SUCCESS || args.al != VBE_FUNCTION) {
    fprintf(stderr, "vbe_flip_page: function call error.");
    return 1;
  }

  // cycle to the next buffer
  buff_index = (buff_index + 1) % 3;
  return 0;
}
