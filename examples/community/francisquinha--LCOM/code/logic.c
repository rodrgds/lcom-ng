/** @file logic.c
 * @brief file containing definitions of the functions that deal with the game logic
 */
#include "logic.h"
#include "logic_static.h"

static void get_specs(unsigned short index, unsigned short specs[4]) {
	specs[0] = (index / 27) % 3;
	specs[1] = (index / 9) % 3;
	specs[2] = (index / 3) % 3;
	specs[3] = index % 3;
}

static unsigned short check_specs(unsigned short specs0[4], unsigned short specs1[4], unsigned short specs2[4]) {
	unsigned short i;
	for (i = 0; i < 4; i++)
		if ((specs0[i] + specs1[i] + specs2[i]) % 3 != 0) return 1;
	return 0;
}

static void get_all_specs(Game *game, unsigned short specs[21][4]) {
	if (game->gamestate->display == 0) return;
	unsigned short i;
	for (i = 0; i < game->gamestate->display; i++)
		get_specs(game->gamestate->display_cards[i], specs[i]);
}

unsigned short check_set(Game *game) {
	unsigned short specs0[4];
	unsigned short specs1[4];
	unsigned short specs2[4];
	get_specs(game->gamestate->display_cards[game->gamestate->remove_cards[0]], specs0);
	get_specs(game->gamestate->display_cards[game->gamestate->remove_cards[1]], specs1);
	get_specs(game->gamestate->display_cards[game->gamestate->remove_cards[2]], specs2);
	return check_specs(specs0, specs1, specs2);
}

unsigned short check_no_sets(Game *game) {
	unsigned short specs[21][4];
	get_all_specs(game, specs);
	unsigned short i, j, k;
	for (i = 0; i < game->gamestate->display; i++) {
		for (j = i + 1; j < game->gamestate->display; j++) {
			for (k = j + 1; k < game->gamestate->display; k++) {
				if (!check_specs(specs[i], specs[j], specs[k])) return 1;
			}
		}
	}
	return 0;
}

unsigned short get_set(Game *game) {
	unsigned short specs[21][4];
	get_all_specs(game, specs);
	unsigned short i, j, k, l;
	if (game->gamestate->remove_card == 0) {
		for (i = 0; i < game->gamestate->display; i++) {
			for (j = i + 1; j < game->gamestate->display; j++) {
				for (k = j + 1; k < game->gamestate->display; k++) {
					if (!check_specs(specs[i], specs[j], specs[k])) {
						game->gamestate->remove_cards[game->gamestate->remove_card] = i;
						game->gamestate->remove_card++;
						return 0;
					}
				}
			}
		}
		return 1;
	}
	else if (game->gamestate->remove_card == 1) {
		i = game->gamestate->remove_cards[0];
		for (j = 0; j < game->gamestate->display; j++) {
			if (j == i) continue;
			for (k = j + 1; k < game->gamestate->display; k++) {
				if (k == i) continue;
				if (!check_specs(specs[i], specs[j], specs[k])) {
					game->gamestate->remove_cards[game->gamestate->remove_card] = j;
					game->gamestate->remove_card++;
					return 0;
				}
			}
		}
		return 1;
	}
	else if (game->gamestate->remove_card == 2) {
		i = game->gamestate->remove_cards[0];
		j = game->gamestate->remove_cards[1];
		for (k = 0; k < game->gamestate->display; k++) {
			if (k == i || k == j) continue;
			if (!check_specs(specs[i], specs[j], specs[k])) {
				game->gamestate->remove_cards[game->gamestate->remove_card] = k;
				game->gamestate->remove_card++;
				return 0;
			}
		}
		return 1;
	}
	return 1;
}
