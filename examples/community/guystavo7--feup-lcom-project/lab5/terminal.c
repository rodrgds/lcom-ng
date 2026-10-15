#include "terminal.h"
#include "terminalUtils.h"

struct Terminal {
    char* page1;
    char* page2;
    char* terminalMem;
    int current;

    char* empytyBuffer;

    vbe_mode_info_t vmi;
    struct minix_mem_range mr;

    unsigned int bytesPerPixel;
    unsigned long vram_size;
};

Terminal* newTerminal(uint16_t mode) {
    char* terminalMem;
    vbe_mode_info_t vmi;

    if (vbe_get_mode_info(mode, &vmi)) {
        printf("Error on getting mode\n");
        return NULL;
    }

    unsigned bytesPerPixel = vmi.BitsPerPixel / 8;
    if (vmi.BitsPerPixel % 8)
        bytesPerPixel++;

    unsigned long vram_size = vmi.XResolution * vmi.YResolution * bytesPerPixel;
    unsigned long total_size = vram_size * 2;

    struct minix_mem_range mr;
    mr.mr_base = (phys_bytes)vmi.PhysBasePtr;
    mr.mr_limit = mr.mr_base + total_size;

    int r;
    if ((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
        printf("Error on sys_privctl\n");
        return NULL;
    }

    if ((terminalMem = vm_map_phys(SELF, (void*)mr.mr_base, total_size)) == MAP_FAILED) {
        printf("Error on page1 vm_map_phys\n");
        return NULL;
    }

    if (enterMode(mode)) {
        printf("Error on entering mode\n");
        return NULL;
    }

    Terminal* res = (Terminal*)malloc(sizeof(Terminal));
    res->empytyBuffer = malloc(sizeof(char) * vram_size);
    memset(res->empytyBuffer, 0, vram_size);

    res->vmi = vmi;
    res->mr = mr;
    res->terminalMem = terminalMem;
    res->page1 = terminalMem;
    res->page2 = terminalMem + vram_size;
    res->current = 0;
    res->bytesPerPixel = bytesPerPixel;
    res->vram_size = vram_size;

    return res;
}

void drawRectangle(Terminal* terminal, int x, int y, uint16_t width, uint16_t height, uint32_t color) {
    for (uint16_t i = y; i < y + height; i++) {
        drawLine(terminal, x, i, width, color);
    }
}

void drawPixel(Terminal* terminal, int x, int y, uint32_t color) {
    if (y < 0 || y >= terminal->vmi.YResolution || x < 0 || x >= terminal->vmi.XResolution) return;
    long currentX = x * terminal->bytesPerPixel;
    long offset = currentX + y * terminal->vmi.XResolution * terminal->bytesPerPixel;

    memcpy(&terminal->terminalMem[offset], &color, terminal->bytesPerPixel);
}

void drawPixelInEmptyBuffer(Terminal* terminal, int x, int y, uint32_t color) {
    if (y < 0 || y >= terminal->vmi.YResolution || x < 0 || x >= terminal->vmi.XResolution) return;
    long currentX = x * terminal->bytesPerPixel;
    long offset = currentX + y * terminal->vmi.XResolution * terminal->bytesPerPixel;

    memcpy(&terminal->empytyBuffer[offset], &color, terminal->bytesPerPixel);
}

void drawLine(Terminal* terminal, int x, int y, uint16_t width, uint32_t color) {
    int end = x + width;

    for (int i = x; i < end; i++) {
        drawPixel(terminal, i, y, color);
    }
}

void freeTerminal(Terminal* terminal) {
    free(terminal->empytyBuffer);
    free(terminal);
    vg_exit();
}

unsigned int getBytesPerPixel(Terminal* terminal) {
    return terminal->bytesPerPixel;
}

void clearEmptyBuffer(Terminal* terminal) {
    memset(terminal->empytyBuffer, 0, terminal->vram_size);
}

void refreshTerminal(Terminal* terminal) {
    if (terminal->current == 0) {
        pageFlip(0);
        terminal->current = 1;
        terminal->terminalMem = terminal->page2;
    } else {
        pageFlip(terminal->vmi.YResolution);
        terminal->current = 0;
        terminal->terminalMem = terminal->page1;
    }

    memcpy(terminal->terminalMem, terminal->empytyBuffer, terminal->vmi.XResolution * terminal->vmi.YResolution * terminal->bytesPerPixel);
}
