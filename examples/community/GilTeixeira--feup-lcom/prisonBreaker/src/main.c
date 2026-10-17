#include <stdio.h>
#include "defs.h"
#include "i8042.h"
#include "i8254.h"
#include "video_gr.h"
#include "prisonBreaker.h"

int main(int argc, char **argv) {

	srand(time(NULL));
	vg_init(DEFAULT_MODE);

	PrisonBreaker* prisonBreaker = (PrisonBreaker*) initPrisonBreaker();
	while (!prisonBreaker->gameState->done) {
		updatePrisonBreaker(prisonBreaker);

		if (!prisonBreaker->gameState->done) {
			if (prisonBreaker->draw)
				drawPrisonBreaker(prisonBreaker);

			flipDisplay();
		}

	}
	stopPrisonBreaker(prisonBreaker);

	vg_exit();

	return Ok;
}
