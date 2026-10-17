#include "draw_functions.h"



bool capslock = false;

const xpm_row_t* get_letter_sprite(char letter) {
    switch (letter) {
        case 'a': return a;
        case 'A': return A;
        case 'b': return b;
        case 'B': return B;
        case 'c': return c;
        case 'C': return C;
        case 'd': return d;
        case 'D': return D;
        case 'e': return e;
        case 'E': return E;
        case 'f': return f;
        case 'F': return F;
        case 'g': return g;
        case 'G': return G;
        case 'h': return h;
        case 'H': return H;
        case 'i': return i;
        case 'I': return I;
        case 'j': return j;
        case 'J': return J;
        case 'k': return k;
        case 'K': return K;
        case 'l': return l;
        case 'L': return L;
        case 'm': return m;
        case 'M': return M;
        case 'n': return n;
        case 'N': return N;
        case 'o': return o;
        case 'O': return O;
        case 'p': return p;
        case 'P': return P;
        case 'q': return q;
        case 'Q': return Q;
        case 'r': return r;
        case 'R': return R;
        case 's': return s;
        case 'S': return S;
        case 't': return t;
        case 'T': return T;
        case 'u': return u;
        case 'U': return U;
        case 'v': return v;
        case 'V': return V;
        case 'w': return w;
        case 'W': return W;
        case 'x': return x;
        case 'X': return X;
        case 'y': return y;
        case 'Y': return Y;
        case 'z': return z;
        case 'Z': return Z;
        case ',': return comma;
        case '.': return dot;
        case '?': return question_mark;
        case '!': return exclamation_mark;
        case ' ': return space;
        case '>': return arrow;
        case '0': return zero;
        case '1': return one;
        case '2': return two;
        case '3': return three;
        case '4': return four;
        case '5': return five;
        case '6': return six;
        case '7': return seven;
        case '8': return eight;
        case '9': return nine;
        case ':': return double_dots;
        case '%': return percentage;
        default: return NULL;
    }
}

const xpm_row_t* get_letter_sprite_green(char letter) {
    switch (letter) {
        case 'a': return a_green;
        case 'b': return b_green;
        case 'c': return c_green;
        case 'd': return d_green;
        case 'e': return e_green;
        case 'f': return f_green;
        case 'g': return g_green;
        case 'h': return h_green;
        case 'i': return i_green;
        case 'j': return j_green;
        case 'k': return k_green;
        case 'l': return l_green;
        case 'm': return m_green;
        case 'n': return n_green;
        case 'o': return o_green;
        case 'p': return p_green;
        case 'q': return q_green;
        case 'r': return r_green;
        case 's': return s_green;
        case 't': return t_green;
        case 'u': return u_green;
        case 'v': return v_green;
        case 'w': return w_green;
        case 'x': return x_green;
        case 'y': return y_green;
        case 'z': return z_green;
        case 'A': return A_green;
        case 'B': return B_green;
        case 'C': return C_green;
        case 'D': return D_green;
        case 'E': return E_green;
        case 'F': return F_green;
        case 'G': return G_green;
        case 'H': return H_green;
        case 'I': return I_green;
        case 'J': return J_green;
        case 'K': return K_green;
        case 'L': return L_green;
        case 'M': return M_green;
        case 'N': return N_green;
        case 'O': return O_green;
        case 'P': return P_green;
        case 'Q': return Q_green;
        case 'R': return R_green;
        case 'S': return S_green;
        case 'T': return T_green;
        case 'U': return U_green;
        case 'V': return V_green;
        case 'W': return W_green;
        case 'X': return X_green;
        case 'Y': return Y_green;
        case 'Z': return Z_green;
        case ',': return comma_green;
        case '.': return dot_green;
        case '?': return question_mark_green;
        case '!': return exclamation_mark_green;
        case ' ': return space;
        default: return NULL;
    }
}

const xpm_row_t* get_letter_sprite_black(char letter) {
    switch (letter) {
        case 'a': return a_black;
        case 'b': return b_black;
        case 'c': return c_black;
        case 'd': return d_black;
        case 'e': return e_black;
        case 'f': return f_black;
        case 'g': return g_black;
        case 'h': return h_black;
        case 'i': return i_black;
        case 'j': return j_black;
        case 'k': return k_black;
        case 'l': return l_black;
        case 'm': return m_black;
        case 'n': return n_black;
        case 'o': return o_black;
        case 'p': return p_black;
        case 'q': return q_black;
        case 'r': return r_black;
        case 's': return s_black;
        case 't': return t_black;
        case 'u': return u_black;
        case 'v': return v_black;
        case 'w': return w_black;
        case 'x': return x_black;
        case 'y': return y_black;
        case 'z': return z_black;
        case 'A': return A_black;
        case 'B': return B_black;
        case 'C': return C_black;
        case 'D': return D_black;
        case 'E': return E_black;
        case 'F': return F_black;
        case 'G': return G_black;
        case 'H': return H_black;
        case 'I': return I_black;
        case 'J': return J_black;
        case 'K': return K_black;
        case 'L': return L_black;
        case 'M': return M_black;
        case 'N': return N_black;
        case 'O': return O_black;
        case 'P': return P_black;
        case 'Q': return Q_black;
        case 'R': return R_black;
        case 'S': return S_black;
        case 'T': return T_black;
        case 'U': return U_black;
        case 'V': return V_black;
        case 'W': return W_black;
        case 'X': return X_black;
        case 'Y': return Y_black;
        case 'Z': return Z_black;
        case ',': return comma_black;
        case '.': return dot_black;
        case '?': return question_mark_black;
        case '!': return exclamation_mark_black;
        case ' ': return space;
        default: return NULL;
    }
}

const xpm_row_t* get_letter_sprite_red(char letter) {
    switch (letter) {
        case 'a': return a_red;
        case 'b': return b_red;
        case 'c': return c_red;
        case 'd': return d_red;
        case 'e': return e_red;
        case 'f': return f_red;
        case 'g': return g_red;
        case 'h': return h_red;
        case 'i': return i_red;
        case 'j': return j_red;
        case 'k': return k_red;
        case 'l': return l_red;
        case 'm': return m_red;
        case 'n': return n_red;
        case 'o': return o_red;
        case 'p': return p_red;
        case 'q': return q_red;
        case 'r': return r_red;
        case 's': return s_red;
        case 't': return t_red;
        case 'u': return u_red;
        case 'v': return v_red;
        case 'w': return w_red;
        case 'x': return x_red;
        case 'y': return y_red;
        case 'z': return z_red;
        case 'A': return A_red;
        case 'B': return B_red;
        case 'C': return C_red;
        case 'D': return D_red;
        case 'E': return E_red;
        case 'F': return F_red;
        case 'G': return G_red;
        case 'H': return H_red;
        case 'I': return I_red;
        case 'J': return J_red;
        case 'K': return K_red;
        case 'L': return L_red;
        case 'M': return M_red;
        case 'N': return N_red;
        case 'O': return O_red;
        case 'P': return P_red;
        case 'Q': return Q_red;
        case 'R': return R_red;
        case 'S': return S_red;
        case 'T': return T_red;
        case 'U': return U_red;
        case 'V': return V_red;
        case 'W': return W_red;
        case 'X': return X_red;
        case 'Y': return Y_red;
        case 'Z': return Z_red;
        case ',': return comma_red;
        case '.': return dot_red;
        case '?': return question_mark_red;
        case '!': return exclamation_mark_red;
        case ' ': return space;
        default: return NULL;
    }
}

char breakcode_to_ascii(uint8_t breakcode) {
    switch (breakcode) {
        case 0x9E: return capslock ? 'A' : 'a';
        case 0xB0: return capslock ? 'B' : 'b';
        case 0xAE: return capslock ? 'C' : 'c';
        case 0xA0: return capslock ? 'D' : 'd';
        case 0x92: return capslock ? 'E' : 'e';
        case 0xA1: return capslock ? 'F' : 'f';
        case 0xA2: return capslock ? 'G' : 'g';
        case 0xA3: return capslock ? 'H' : 'h';
        case 0x97: return capslock ? 'I' : 'i';
        case 0xA4: return capslock ? 'J' : 'j';
        case 0xA5: return capslock ? 'K' : 'k';
        case 0xA6: return capslock ? 'L' : 'l';
        case 0xB2: return capslock ? 'M' : 'm';
        case 0xB1: return capslock ? 'N' : 'n';
        case 0x98: return capslock ? 'O' : 'o';
        case 0x99: return capslock ? 'P' : 'p';
        case 0x90: return capslock ? 'Q' : 'q';
        case 0x93: return capslock ? 'R' : 'r';
        case 0x9F: return capslock ? 'S' : 's';
        case 0x94: return capslock ? 'T' : 't';
        case 0x96: return capslock ? 'U' : 'u';
        case 0xAF: return capslock ? 'V' : 'v';
        case 0x91: return capslock ? 'W' : 'w';
        case 0xAD: return capslock ? 'X' : 'x';
        case 0x95: return capslock ? 'Y' : 'y';
        case 0xAC: return capslock ? 'Z' : 'z';
        case 0xB4: return '.';
        case 0xB3: return ',';
        case 0x8C: return '?';
        case 0x82: return '!';
        case 0xBA:
            capslock = !capslock;
            return 0;
        case 0xB9: return ' ';
        default: return 0;
    }
}

void cache_line_heights(const char* word, int* height_cache) {
    for (int i = 0; word[i] != '\0'; ++i) {
        unsigned char ch = (unsigned char)word[i];
        if (height_cache[ch] == -1) {
            const xpm_row_t* sprite = get_letter_sprite(ch);
            if (sprite != NULL)
                height_cache[ch] = get_xpm_height(sprite);
            else
                height_cache[ch] = 0;
        }
    }
}

void draw_word_green(const char* word, int x, int y) {
    int x_res = x;
    int height_cache[256];
    memset(height_cache, -1, sizeof(height_cache));
    cache_line_heights(word, height_cache);

    for (int idx = 0; word[idx] != '\0'; idx++) {
        unsigned char ch = (unsigned char)word[idx];
        const xpm_row_t* sprite = get_letter_sprite_green(ch); 
        if (sprite != NULL) {
            uint16_t width = 0, height = 0;
            read_pixmap((xpm_row_t *)sprite, &width, &height);
            draw_element((xpm_row_t *)sprite, x_res, y - height_cache[ch]);
            x_res += width;
        }
    }
}




int draw_sentence(char* sentence, int x, int y, char* color) {
    int x_res = x;
    int y_res = y;
    int fixed_spacing = 0;
    int line_spacing = 8;
    int screen_width = get_horizontal_resolution();

    char** words = split_phrase(sentence);
    if (words == NULL) return 0;

    int word_count = 0;

    for (int w = 0; words[w] != NULL; w++) {
        char* word = words[w];
        int word_width = 0;
        int max_word_height = 0;

        
        for (int idx = 0; word[idx] != '\0'; idx++) {
            const xpm_row_t* sprite = get_letter_sprite(word[idx]);
            if (sprite != NULL) {
                int width = get_xpm_width(sprite);
                int height = get_xpm_height(sprite);
                word_width += width + fixed_spacing;
                if (height > max_word_height) max_word_height = height;
            }
        }

        
        if (x_res + word_width > screen_width) {
            x_res = x;
            y_res += line_spacing + get_xpm_height(get_letter_sprite('A'));
        }

        
        int word_x = x_res;
        for (int idx = 0; word[idx] != '\0'; idx++) {
            unsigned char ch = (unsigned char)word[idx];
            const xpm_row_t* sprite = NULL;
            if (strcmp(color, "white") == 0) {
                sprite = get_letter_sprite(ch);
            }
            else if (strcmp(color, "green") == 0) {
                sprite = get_letter_sprite_green(ch);
            }
            else if (strcmp(color, "red") == 0) {
                sprite = get_letter_sprite_red(ch);
            }
            else if (strcmp(color, "black") == 0) {
                sprite = get_letter_sprite_black(ch);
            }
            else {
                sprite = get_letter_sprite(ch);
            }

            if (sprite != NULL) {
                uint16_t width = 0, height = 0;
                read_pixmap((xpm_row_t *)sprite, &width, &height);
                int draw_y = y_res - get_xpm_height(sprite);
                draw_element((xpm_row_t *)sprite, word_x, draw_y);
                word_x += width + fixed_spacing;
            }
        }

        x_res += word_width;

        
        if (words[w + 1] != NULL) {
            const xpm_row_t* space_sprite = get_letter_sprite(' ');
            if (space_sprite != NULL) {
                x_res += get_xpm_width(space_sprite) + fixed_spacing;
            }
        }

        word_count++;
    }

    for (int i = 0; words[i] != NULL; i++) free(words[i]);
    free(words);

    return word_count;
}

char** split_phrase(const char* phrase) {
    char** words = malloc((MAX_WORDS + 1) * sizeof(char*)); 
    if (words == NULL) return NULL;

    int count = 0;
    const char* start = phrase;
    const char* ptr = phrase;

    while (*ptr != '\0') {
        if (*ptr == ' ') {
            size_t len = ptr - start + 1; 
            words[count] = malloc(len + 1);
            if (words[count] == NULL) return NULL;
            strncpy(words[count], start, len);
            words[count][len] = '\0';
            count++;
            ptr++;
            start = ptr;
        } else {
            ptr++;
        }
    }

    
    if (ptr != start) {
        size_t len = ptr - start;
        words[count] = malloc(len + 1);
        if (words[count] == NULL) return NULL;
        strncpy(words[count], start, len);
        words[count][len] = '\0';
        count++;
    }

    words[count] = NULL; 
    return words;
}


int get_xpm_width(xpm_row_t const xpm[]) {
    int width;
    sscanf(xpm[0], "%d", &width);
    return width;
}

int get_xpm_height(xpm_row_t const xpm[]) {
    int width, height;
    sscanf(xpm[0], "%d %d", &width, &height);
    return height;
}


void refresh_frame_buffer() {
    vg_draw_content();
}


void update_frame(int x1 , int x2 , int y1 , int y2, char* element , int newX , int newY){


    int width = x2 - x1;
    int height = y2 - y1;
    vg_draw_rectangle(x1, y1, width, height, 0x000000);
    draw_sentence(element, newX , newY, "white");
}

void draw_start_game(){
    clearScreen();
    refresh_frame_buffer();


  
}



