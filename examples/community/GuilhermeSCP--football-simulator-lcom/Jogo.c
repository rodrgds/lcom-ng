#include <minix/syslib.h>
#include <minix/drivers.h>
#include <time.h>
#include <math.h>

#include "Jogo.h"

double dif= 2.5;

void setResultado(Equipa e1, Equipa e2)
{
	int sorte1, sorte2, div1, div2;
	srand ( time(NULL) );
	sorte1 = rand() % (6+1);
	sorte2 = rand() % (6+1);
	div1 = 2 + rand() % (3-2+1);
	div2 = 2 + rand() % (3-2+1);


	G1=((((0.3*getNMed(e1.tac)*getMedOv(e1)+0.7*getNAta(e1.tac)*getAtaOv(e1))*dif)-(getNDef(e2.tac)*getDefOv(e2)+getGROv(e2)))/100)*(sorte1/div1);
	G2=((((0.3*getNMed(e2.tac)*getMedOv(e2)+0.7*getNAta(e2.tac)*getAtaOv(e2))*2.5)-(getNDef(e1.tac)*getDefOv(e1)+getGROv(e1)))/100)*(sorte2/div2);

}

void AlteraDificuldade(double d)
{
	dif = d;
}




