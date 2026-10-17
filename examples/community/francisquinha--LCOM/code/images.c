/** @file images.c
 * @brief file containing definitions of the functions that deal with xpm images
 */
#include "images.h"
#include "images/cards/card0000.h"
#include "images/cards/card0001.h"
#include "images/cards/card0002.h"
#include "images/cards/card0010.h"
#include "images/cards/card0011.h"
#include "images/cards/card0012.h"
#include "images/cards/card0020.h"
#include "images/cards/card0021.h"
#include "images/cards/card0022.h"
#include "images/cards/card0100.h"
#include "images/cards/card0101.h"
#include "images/cards/card0102.h"
#include "images/cards/card0110.h"
#include "images/cards/card0111.h"
#include "images/cards/card0112.h"
#include "images/cards/card0120.h"
#include "images/cards/card0121.h"
#include "images/cards/card0122.h"
#include "images/cards/card0200.h"
#include "images/cards/card0201.h"
#include "images/cards/card0202.h"
#include "images/cards/card0210.h"
#include "images/cards/card0211.h"
#include "images/cards/card0212.h"
#include "images/cards/card0220.h"
#include "images/cards/card0221.h"
#include "images/cards/card0222.h"
#include "images/cards/card1000.h"
#include "images/cards/card1001.h"
#include "images/cards/card1002.h"
#include "images/cards/card1010.h"
#include "images/cards/card1011.h"
#include "images/cards/card1012.h"
#include "images/cards/card1020.h"
#include "images/cards/card1021.h"
#include "images/cards/card1022.h"
#include "images/cards/card1100.h"
#include "images/cards/card1101.h"
#include "images/cards/card1102.h"
#include "images/cards/card1110.h"
#include "images/cards/card1111.h"
#include "images/cards/card1112.h"
#include "images/cards/card1120.h"
#include "images/cards/card1121.h"
#include "images/cards/card1122.h"
#include "images/cards/card1200.h"
#include "images/cards/card1201.h"
#include "images/cards/card1202.h"
#include "images/cards/card1210.h"
#include "images/cards/card1211.h"
#include "images/cards/card1212.h"
#include "images/cards/card1220.h"
#include "images/cards/card1221.h"
#include "images/cards/card1222.h"
#include "images/cards/card2000.h"
#include "images/cards/card2001.h"
#include "images/cards/card2002.h"
#include "images/cards/card2010.h"
#include "images/cards/card2011.h"
#include "images/cards/card2012.h"
#include "images/cards/card2020.h"
#include "images/cards/card2021.h"
#include "images/cards/card2022.h"
#include "images/cards/card2100.h"
#include "images/cards/card2101.h"
#include "images/cards/card2102.h"
#include "images/cards/card2110.h"
#include "images/cards/card2111.h"
#include "images/cards/card2112.h"
#include "images/cards/card2120.h"
#include "images/cards/card2121.h"
#include "images/cards/card2122.h"
#include "images/cards/card2200.h"
#include "images/cards/card2201.h"
#include "images/cards/card2202.h"
#include "images/cards/card2210.h"
#include "images/cards/card2211.h"
#include "images/cards/card2212.h"
#include "images/cards/card2220.h"
#include "images/cards/card2221.h"
#include "images/cards/card2222.h"
#include "images/buttons/set_button.h"
#include "images/buttons/nosets_button.h"
#include "images/buttons/hint_button.h"
#include "images/buttons/pause_button.h"
#include "images/buttons/play_button.h"
#include "images/buttons/save_button.h"
#include "images/buttons/exit_button.h"
#include "images/letters.h"
#include "images/numbers.h"
#include "images/background.h"
#include "images/set.h"

#define DEBUG_MALLOC 0

char ***get_cards() {
	char ***cards = (char ***)malloc(sizeof(char **) * 81);
	if (DEBUG_MALLOC) printf("Allocated %lu bytes for cards\n", sizeof(char **) * 81);
	cards[0] = card0000;
	cards[1] = card0001;
	cards[2] = card0002;
	cards[3] = card0010;
	cards[4] = card0011;
	cards[5] = card0012;
	cards[6] = card0020;
	cards[7] = card0021;
	cards[8] = card0022;
	cards[9] = card0100;
	cards[10] = card0101;
	cards[11] = card0102;
	cards[12] = card0110;
	cards[13] = card0111;
	cards[14] = card0112;
	cards[15] = card0120;
	cards[16] = card0121;
	cards[17] = card0122;
	cards[18] = card0200;
	cards[19] = card0201;
	cards[20] = card0202;
	cards[21] = card0210;
	cards[22] = card0211;
	cards[23] = card0212;
	cards[24] = card0220;
	cards[25] = card0221;
	cards[26] = card0222;
	cards[27] = card1000;
	cards[28] = card1001;
	cards[29] = card1002;
	cards[30] = card1010;
	cards[31] = card1011;
	cards[32] = card1012;
	cards[33] = card1020;
	cards[34] = card1021;
	cards[35] = card1022;
	cards[36] = card1100;
	cards[37] = card1101;
	cards[38] = card1102;
	cards[39] = card1110;
	cards[40] = card1111;
	cards[41] = card1112;
	cards[42] = card1120;
	cards[43] = card1121;
	cards[44] = card1122;
	cards[45] = card1200;
	cards[46] = card1201;
	cards[47] = card1202;
	cards[48] = card1210;
	cards[49] = card1211;
	cards[50] = card1212;
	cards[51] = card1220;
	cards[52] = card1221;
	cards[53] = card1222;
	cards[54] = card2000;
	cards[55] = card2001;
	cards[56] = card2002;
	cards[57] = card2010;
	cards[58] = card2011;
	cards[59] = card2012;
	cards[60] = card2020;
	cards[61] = card2021;
	cards[62] = card2022;
	cards[63] = card2100;
	cards[64] = card2101;
	cards[65] = card2102;
	cards[66] = card2110;
	cards[67] = card2111;
	cards[68] = card2112;
	cards[69] = card2120;
	cards[70] = card2121;
	cards[71] = card2122;
	cards[72] = card2200;
	cards[73] = card2201;
	cards[74] = card2202;
	cards[75] = card2210;
	cards[76] = card2211;
	cards[77] = card2212;
	cards[78] = card2220;
	cards[79] = card2221;
	cards[80] = card2222;
	return cards;
}

char **get_set_button() {
	return set_button;
}

char **get_nosets_button() {
	return nosets_button;
}

char **get_hint_button() {
	return hint_button;
}

char **get_pause_button() {
	return pause_button;
}

char **get_play_button() {
	return play_button;
}

char **get_save_button() {
	return save_button;
}

char **get_exit_button() {
	return exit_button;
}

char ***get_letters() {
	char ***letters = (char ***)malloc(sizeof(char **) * 27);
	if (DEBUG_MALLOC) printf("Allocated %lu bytes for letters\n", sizeof(char **) * 27);
	letters[0] = SPACE_xpm;
	letters[1] = A_xpm;
	letters[2] = B_xpm;
	letters[3] = C_xpm;
	letters[4] = D_xpm;
	letters[5] = E_xpm;
	letters[6] = F_xpm;
	letters[7] = G_xpm;
	letters[8] = H_xpm;
	letters[9] = I_xpm;
	letters[10] = J_xpm;
	letters[11] = K_xpm;
	letters[12] = L_xpm;
	letters[13] = M_xpm;
	letters[14] = N_xpm;
	letters[15] = O_xpm;
	letters[16] = P_xpm;
	letters[17] = Q_xpm;
	letters[18] = R_xpm;
	letters[19] = S_xpm;
	letters[20] = T_xpm;
	letters[21] = U_xpm;
	letters[22] = V_xpm;
	letters[23] = W_xpm;
	letters[24] = X_xpm;
	letters[25] = Y_xpm;
	letters[26] = Z_xpm;
	return letters;
}

char ***get_numbers() {
	char ***numbers = (char ***)malloc(sizeof(char **) * 13);
	if (DEBUG_MALLOC) printf("Allocated %lu bytes for numbers\n", sizeof(char **) * 13);
	numbers[0] = n0_xpm;
	numbers[1] = n1_xpm;
	numbers[2] = n2_xpm;
	numbers[3] = n3_xpm;
	numbers[4] = n4_xpm;
	numbers[5] = n5_xpm;
	numbers[6] = n6_xpm;
	numbers[7] = n7_xpm;
	numbers[8] = n8_xpm;
	numbers[9] = n9_xpm;
	numbers[10] = dots_xpm;
	numbers[11] = bar_xpm;
	numbers[12] = space_xpm;
	return numbers;
}

char **get_background() {
	return rainbow;
}

char **get_set_logo() {
	return set;
}
