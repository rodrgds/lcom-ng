#include "scancode.h"

typedef struct { uint8_t sc; char ch; } ScanMapping;

static const ScanMapping scancode_map[] = {
    {0x0E, '\b'},
    {0x10, 'q'}, {0x11, 'w'}, {0x12, 'e'}, {0x13, 'r'}, {0x14, 't'},
    {0x15, 'y'}, {0x16, 'u'}, {0x17, 'i'}, {0x18, 'o'}, {0x19, 'p'},
    {0x1E, 'a'}, {0x1F, 's'}, {0x20, 'd'}, {0x21, 'f'}, {0x22, 'g'},
    {0x23, 'h'}, {0x24, 'j'}, {0x25, 'k'}, {0x26, 'l'}, {0x2C, 'z'},
    {0x2D, 'x'}, {0x2E, 'c'}, {0x2F, 'v'}, {0x30, 'b'}, {0x31, 'n'},
    {0x32, 'm'}, {0x39, ' '},
};

char scancode_to_char(uint8_t sc) {
    
    for (int i = 0; i < (int)(sizeof(scancode_map) / sizeof(scancode_map[0])); i++)
        if (scancode_map[i].sc == sc) {
            return scancode_map[i].ch;
        }

    return 0;
}
