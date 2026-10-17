#include "collisions.h"


unsigned int collision_spaceship_asteroid(space_ship nv1, asteroid ast1, asteroid ast2)
{
	// colisão com o asteroid 1

	if ((ast1.y + 115) >= nv1.y)
	{
		if ((nv1.x >= ast1.x) && (nv1.x <= (ast1.x + 126)))
				return 1; // há colisão à esquerda

		if (((nv1.x + 71 ) >= ast1.x) && ((nv1.x + 71 ) <= (ast1.x + 126)))
				return 1; // há colisão à direita

	}

	// colisão com o asteroid 2

		if ((ast2.y + 115) >= nv1.y)
		{
			if ((nv1.x >= ast2.x) && (nv1.x <= (ast2.x + 126)))
					return 1; // há colisão à esquerda

			if (((nv1.x + 71 ) >= ast2.x) && ((nv1.x + 71 ) <= (ast2.x + 126)))
					return 1; // há colisão à direita

		}


		return 0; // não há colisão
}
