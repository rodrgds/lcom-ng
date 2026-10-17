#include "utils.h"

xpm_map_t (get_xpm_from_char)(char c) {
    int index = c - 97;
    if ('A' <= c && c <= 'Z') {index += 32;}
    return dark_orange_xpms[index];
}

xpm_map_t (get_bright_xpm_from_char)(char c) {
    int index = c - 97;
    if ('A' <= c && c <= 'Z') {index += 32;}
    return bright_orange_xpms[index];
}

xpm_map_t (get_error_xpm_from_char)(char c) {
    if (c == '_') return error_xpms[26];
    int index = c - 97;
    if ('A' <= c && c <= 'Z') {index += 32;}
    return error_xpms[index];
}
