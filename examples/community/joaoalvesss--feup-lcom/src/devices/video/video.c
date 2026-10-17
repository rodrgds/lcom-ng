#include "video.h"

int video_init(uint16_t mode) {
     reg86_t reg;
     memset(&reg, 0, sizeof(reg));

     reg.bx = mode | LINEAR_BUFFER;
     reg.ax = VBE_MODE;
     reg.intno = VIDEO_VALUE;

     if (sys_int86(&reg) != 0) return 1;

     return 0;
}

int map_phys_vir_mem(uint16_t mode, uint8_t** buffer) {
     memset(&m_info, 0, sizeof(m_info));
     if (vbe_get_mode_info(mode, &m_info) != 0)return 1;

     struct minix_mem_range mmr;
     mmr.mr_base = m_info.PhysBasePtr;
     mmr.mr_limit = mmr.mr_base + (m_info.XResolution * m_info.YResolution * m_info.BitsPerPixel) / 8;

     if ((sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mmr)) != 0) return 1;

     *buffer = vm_map_phys(SELF, (void *)mmr.mr_base, ((m_info.XResolution * m_info.YResolution * m_info.BitsPerPixel) / 8));
     if (*buffer == MAP_FAILED) return 1;

     return 0;
}

int draw_pixel(uint16_t x, uint16_t y, uint32_t color, uint8_t* buffer) {
     if (x >= m_info.XResolution || y >= m_info.YResolution) return 0;
     if (x < 0 || y < 0) return 0;

     unsigned bytes_per_pixel = (m_info.BitsPerPixel + 7) / 8;
     unsigned pos = (x + (m_info.XResolution * y)) * bytes_per_pixel;

     if (memcpy(&buffer[pos], &color, bytes_per_pixel) == NULL) return 1;

    return 0;
}
