#ifndef UTILS_H
#define UTILS_H

#define MAX_WORDS   5000
#define WORD_LEN    5
#define GUESSES     6

#define BG_GREEN    "\x1b[42m"
#define BG_YELLOW   "\x1b[43m"
#define BG_GRAY     "\x1b[100m"
#define COLOR_RESET "\x1b[0m"

void print_utf8_upper(const char *utf8);

#endif // UTILS_H
