#include "scancodes.h"


char scancode_to_char(uint8_t scancode) {
    if (scancode == A_BREAK) {
        return 'a';
    }
    else if (scancode == B_BREAK) {
        return 'b';
    }
    else if (scancode == C_BREAK) {
        return 'c';
    }
    else if (scancode == D_BREAK) {
        return 'd';
    }
    else if (scancode == E_BREAK) {
        return 'e';
    }
    else if (scancode == F_BREAK) {
        return 'f';
    }
    else if (scancode == G_BREAK) {
        return 'g';
    }
    else if (scancode == H_BREAK) {
        return 'h';
    }
    else if (scancode == I_BREAK) {
        return 'i';
    }
    else if (scancode == J_BREAK) {
        return 'j';
    }
    else if (scancode == K_BREAK) {
        return 'k';
    }
    else if (scancode == L_BREAK) {
        return 'l';
    }
    else if (scancode == M_BREAK) {
        return 'm';
    }
    else if (scancode == N_BREAK) {
        return 'n';
    }
    else if (scancode == O_BREAK) {
        return 'o';
    }
    else if (scancode == P_BREAK) {
        return 'p';
    }
    else if (scancode == Q_BREAK) {
        return 'q';
    }
    else if (scancode == R_BREAK) {
        return 'r';
    }
    else if (scancode == S_BREAK) {
        return 's';
    }
    else if (scancode == T_BREAK) {
        return 't';
    }
    else if (scancode == U_BREAK) {
        return 'u';
    }
    else if (scancode == V_BREAK) {
        return 'v';
    }
    else if (scancode == X_BREAK) {
        return 'x';
    }
    else if (scancode == Y_BREAK) {
        return 'y';
    }
    else if (scancode == W_BREAK) {
        return 'w';
    }
    else if (scancode == Z_BREAK) {
        return 'z';
    }
    return '?';
}

char* scancode_to_char_pointer(uint8_t scancode) {
        if (scancode == A_BREAK) {
        return "a";
    }
    else if (scancode == B_BREAK) {
        return "b";
    }
    else if (scancode == C_BREAK) {
        return "c";
    }
    else if (scancode == D_BREAK) {
        return "d";
    }
    else if (scancode == E_BREAK) {
        return "e";
    }
    else if (scancode == F_BREAK) {
        return "f";
    }
    else if (scancode == G_BREAK) {
        return "g";
    }
    else if (scancode == H_BREAK) {
        return "h";
    }
    else if (scancode == I_BREAK) {
        return "i";
    }
    else if (scancode == J_BREAK) {
        return "j";
    }
    else if (scancode == K_BREAK) {
        return "k";
    }
    else if (scancode == L_BREAK) {
        return "l";
    }
    else if (scancode == M_BREAK) {
        return "m";
    }
    else if (scancode == N_BREAK) {
        return "n";
    }
    else if (scancode == O_BREAK) {
        return "o";
    }
    else if (scancode == P_BREAK) {
        return "p";
    }
    else if (scancode == Q_BREAK) {
        return "q";
    }
    else if (scancode == R_BREAK) {
        return "r";
    }
    else if (scancode == S_BREAK) {
        return "s";
    }
    else if (scancode == T_BREAK) {
        return "t";
    }
    else if (scancode == U_BREAK) {
        return "u";
    }
    else if (scancode == V_BREAK) {
        return "v";
    }
    else if (scancode == X_BREAK) {
        return "x";
    }
    else if (scancode == Y_BREAK) {
        return "y";
    }
    else if (scancode == W_BREAK) {
        return "w";
    }
    else if (scancode == Z_BREAK) {
        return "z";
    }
    return "";
}

bool is_valid_scancode(uint8_t scancode) {
    if (scancode == A_BREAK) {
        return true;
    }
    else if (scancode == B_BREAK) {
        return true;
    }
    else if (scancode == C_BREAK) {
        return true;
    }
    else if (scancode == D_BREAK) {
        return true;
    }
    else if (scancode == E_BREAK) {
        return true;
    }
    else if (scancode == F_BREAK) {
        return true;
    }
    else if (scancode == G_BREAK) {
        return true;
    }
    else if (scancode == H_BREAK) {
        return true;
    }
    else if (scancode == I_BREAK) {
        return true;
    }
    else if (scancode == J_BREAK) {
        return true;
    }
    else if (scancode == K_BREAK) {
        return true;
    }
    else if (scancode == L_BREAK) {
        return true;
    }
    else if (scancode == M_BREAK) {
        return true;
    }
    else if (scancode == N_BREAK) {
        return true;
    }
    else if (scancode == O_BREAK) {
        return true;
    }
    else if (scancode == P_BREAK) {
        return true;
    }
    else if (scancode == Q_BREAK) {
        return true;
    }
    else if (scancode == R_BREAK) {
        return true;
    }
    else if (scancode == S_BREAK) {
        return true;
    }
    else if (scancode == T_BREAK) {
        return true;
    }
    else if (scancode == U_BREAK) {
        return true;
    }
    else if (scancode == V_BREAK) {
        return true;
    }
    else if (scancode == X_BREAK) {
        return true;
    }
    else if (scancode == Y_BREAK) {
        return true;
    }
    else if (scancode == W_BREAK) {
        return true;
    }
    else if (scancode == Z_BREAK) {
        return true;
    }
    return false;
}
