#include <minix/syslib.h>
#include <minix/drivers.h>
#include "Base.h"


int getNDef(Tactica tac){
	int def;
	def=tac.def;
	return def;
}

int getNMed(Tactica tac){
	int med;
	med=tac.med;
	return med;
}

int getNAta(Tactica tac){
	int ata;
	ata=tac.ata;
	return ata;
}

int getDefOv(Equipa e){

	int i=0, j=0, ove_def;
	if (e.jog1.pos==2 && e.jog1.tit==0)
	{
		i+=e.jog1.ove;
		j++;
	}
	if (e.jog2.pos==2 && e.jog2.tit==0)
	{
		i+=e.jog2.ove;
		j++;
	}
	if (e.jog3.pos==2 && e.jog3.tit==0)
	{
		i+=e.jog3.ove;
		j++;
	}
	if (e.jog4.pos==2 && e.jog4.tit==0)
	{
		i+=e.jog4.ove;
		j++;
	}
	if (e.jog5.pos==2 && e.jog5.tit==0)
	{
		i+=e.jog5.ove;
		j++;
	}
	if (e.jog6.pos==2 && e.jog6.tit==0)
	{
		i+=e.jog6.ove;
		j++;
	}
	if (e.jog7.pos==2 && e.jog7.tit==0)
	{
		i+=e.jog7.ove;
		j++;
	}
	if (e.jog8.pos==2 && e.jog8.tit==0)
	{
		i+=e.jog8.ove;
		j++;
	}
	if (e.jog9.pos==2 && e.jog9.tit==0)
	{
		i+=e.jog9.ove;
		j++;
	}
	if (e.jog10.pos==2 && e.jog10.tit==0)
	{
		i+=e.jog10.ove;
		j++;
	}
	if (e.jog11.pos==2 && e.jog11.tit==0)
	{
		i+=e.jog11.ove;
		j++;
	}
	if (e.jog12.pos==2 && e.jog12.tit==0)
	{
		i+=e.jog12.ove;
		j++;
	}
	if (e.jog13.pos==2 && e.jog13.tit==0)
	{
		i+=e.jog13.ove;
		j++;
	}
	if (e.jog14.pos==2 && e.jog14.tit==0)
	{
		i+=e.jog14.ove;
		j++;
	}
	if (e.jog15.pos==2 && e.jog15.tit==0)
	{
		i+=e.jog15.ove;
		j++;
	}
	if (e.jog16.pos==2 && e.jog16.tit==0)
	{
		i+=e.jog16.ove;
		j++;
	}
	if (e.jog17.pos==2 && e.jog17.tit==0)
	{
		i+=e.jog17.ove;
		j++;
	}
	if (e.jog18.pos==2 && e.jog18.tit==0)
	{
		i+=e.jog18.ove;
		j++;
	}
	ove_def = i/j;
	return ove_def;
}

int getMedOv(Equipa e){

	int i=0, j=0, ove_med;
	if (e.jog1.pos==3 && e.jog1.tit==0)
	{
		i+=e.jog1.ove;
		j++;
	}
	if (e.jog2.pos==3 && e.jog2.tit==0)
	{
		i+=e.jog2.ove;
		j++;
	}
	if (e.jog3.pos==3 && e.jog3.tit==0)
	{
		i+=e.jog3.ove;
		j++;
	}
	if (e.jog4.pos==3 && e.jog4.tit==0)
	{
		i+=e.jog4.ove;
		j++;
	}
	if (e.jog5.pos==3 && e.jog5.tit==0)
	{
		i+=e.jog5.ove;
		j++;
	}
	if (e.jog6.pos==3 && e.jog6.tit==0)
	{
		i+=e.jog6.ove;
		j++;
	}
	if (e.jog7.pos==3 && e.jog7.tit==0)
	{
		i+=e.jog7.ove;
		j++;
	}
	if (e.jog8.pos==3 && e.jog8.tit==0)
	{
		i+=e.jog8.ove;
		j++;
	}
	if (e.jog9.pos==3 && e.jog9.tit==0)
	{
		i+=e.jog9.ove;
		j++;
	}
	if (e.jog10.pos==3 && e.jog10.tit==0)
	{
		i+=e.jog10.ove;
		j++;
	}
	if (e.jog11.pos==3 && e.jog11.tit==0)
	{
		i+=e.jog11.ove;
		j++;
	}
	if (e.jog12.pos==3 && e.jog12.tit==0)
	{
		i+=e.jog12.ove;
		j++;
	}
	if (e.jog13.pos==3 && e.jog13.tit==0)
	{
		i+=e.jog13.ove;
		j++;
	}
	if (e.jog14.pos==3 && e.jog14.tit==0)
	{
		i+=e.jog14.ove;
		j++;
	}
	if (e.jog15.pos==3 && e.jog15.tit==0)
	{
		i+=e.jog15.ove;
		j++;
	}
	if (e.jog16.pos==3 && e.jog16.tit==0)
	{
		i+=e.jog16.ove;
		j++;
	}
	if (e.jog17.pos==3 && e.jog17.tit==0)
	{
		i+=e.jog17.ove;
		j++;
	}
	if (e.jog18.pos==3 && e.jog18.tit==0)
	{
		i+=e.jog18.ove;
		j++;
	}
	ove_med = i/j;
	return ove_med;
}

int getAtaOv(Equipa e){

	int i=0, j=0, ove_ata;
	if (e.jog1.pos==4 && e.jog1.tit==0)
	{
		i+=e.jog1.ove;
		j++;
	}
	if (e.jog2.pos==4 && e.jog2.tit==0)
	{
		i+=e.jog2.ove;
		j++;
	}
	if (e.jog3.pos==4 && e.jog3.tit==0)
	{
		i+=e.jog3.ove;
		j++;
	}
	if (e.jog4.pos==4 && e.jog4.tit==0)
	{
		i+=e.jog4.ove;
		j++;
	}
	if (e.jog5.pos==4 && e.jog5.tit==0)
	{
		i+=e.jog5.ove;
		j++;
	}
	if (e.jog6.pos==4 && e.jog6.tit==0)
	{
		i+=e.jog6.ove;
		j++;
	}
	if (e.jog7.pos==4 && e.jog7.tit==0)
	{
		i+=e.jog7.ove;
		j++;
	}
	if (e.jog8.pos==4 && e.jog8.tit==0)
	{
		i+=e.jog8.ove;
		j++;
	}
	if (e.jog9.pos==4 && e.jog9.tit==0)
	{
		i+=e.jog9.ove;
		j++;
	}
	if (e.jog10.pos==4 && e.jog10.tit==0)
	{
		i+=e.jog10.ove;
		j++;
	}
	if (e.jog11.pos==4 && e.jog11.tit==0)
	{
		i+=e.jog11.ove;
		j++;
	}
	if (e.jog12.pos==4 && e.jog12.tit==0)
	{
		i+=e.jog12.ove;
		j++;
	}
	if (e.jog13.pos==4 && e.jog13.tit==0)
	{
		i+=e.jog13.ove;
		j++;
	}
	if (e.jog14.pos==4 && e.jog14.tit==0)
	{
		i+=e.jog14.ove;
		j++;
	}
	if (e.jog15.pos==4 && e.jog15.tit==0)
	{
		i+=e.jog15.ove;
		j++;
	}
	if (e.jog16.pos==4 && e.jog16.tit==0)
	{
		i+=e.jog16.ove;
		j++;
	}
	if (e.jog17.pos==4 && e.jog17.tit==0)
	{
		i+=e.jog17.ove;
		j++;
	}
	if (e.jog18.pos==4 && e.jog18.tit==0)
	{
		i+=e.jog18.ove;
		j++;
	}
	ove_ata = i/j;
	return ove_ata;
}

int getGROv(Equipa e){

	int ove_GR;
	ove_GR = e.jog1.ove;
	return ove_GR;

}



void criaEquipasTacticas()
{
	Tactica t433t;
	Tactica t451t;
	Tactica t442t;
	Tactica t541t;
	Tactica t532t;
	Tactica t352t;
	Tactica t343t;

	Equipa RealMadrid;
	Equipa Barcelona;

	/*------------------*/
	t433t.nome = "4-3-3";
	t433t.def = 4;
	t433t.med = 3;
	t433t.ata = 3;
	/*------------------*/
	t442t.nome = "4-4-2";
	t442t.def = 4;
	t442t.med = 4;
	t442t.ata = 2;
	/*------------------*/
	t451t.nome = "4-5-1";
	t451t.def = 4;
	t451t.med = 5;
	t451t.ata = 1;
	/*------------------*/
	t541t.nome = "5-4-1";
	t541t.def = 5;
	t541t.med = 4;
	t541t.ata = 1;
	/*------------------*/
	t532t.nome = "5-3-2";
	t532t.def = 5;
	t532t.med = 3;
	t532t.ata = 2;
	/*------------------*/
	t352t.nome = "3-5-2";
	t352t.def = 3;
	t352t.med = 5;
	t352t.ata = 2;
	/*------------------*/
	t343t.nome = "3-4-3";
	t343t.def = 3;
	t343t.med = 4;
	t343t.ata = 3;

	tacticas.tac1=t433t;
	tacticas.tac2=t451t;
	tacticas.tac3=t442t;
	tacticas.tac4=t541t;
	tacticas.tac5=t532t;
	tacticas.tac6=t352t;
	tacticas.tac7=t343t;

	/*-----------REAL MADRID-----------*/
	RealMadrid.num=1;
	RealMadrid.jog1.nome = "Casillas";
	RealMadrid.jog1.ove = 90;
	RealMadrid.jog1.pos = 1;
	RealMadrid.jog1.tit = 0;
	RealMadrid.jog1.num = 1;
	RealMadrid.jog2.nome = "Ricardo Carvalho";
	RealMadrid.jog2.ove = 84;
	RealMadrid.jog2.pos = 2;
	RealMadrid.jog2.tit = 0;
	RealMadrid.jog2.num = 4;
	RealMadrid.jog3.nome = "Pepe";
	RealMadrid.jog3.ove = 85;
	RealMadrid.jog3.pos = 2;
	RealMadrid.jog3.tit = 0;
	RealMadrid.jog3.num = 3;
	RealMadrid.jog4.nome = "Sergio Ramos";
	RealMadrid.jog4.ove = 86;
	RealMadrid.jog4.pos = 2;
	RealMadrid.jog4.tit = 0;
	RealMadrid.jog4.num = 2;
	RealMadrid.jog5.nome = "Fabio Coentrao";
	RealMadrid.jog5.ove = 85;
	RealMadrid.jog5.pos = 2;
	RealMadrid.jog5.tit = 0;
	RealMadrid.jog5.num = 8;
	RealMadrid.jog6.nome = "Raul Albiol";
	RealMadrid.jog6.ove = 82;
	RealMadrid.jog6.pos = 2;
	RealMadrid.jog6.tit = 1;
	RealMadrid.jog6.num = 12;
	RealMadrid.jog7.nome = "Kaka";
	RealMadrid.jog7.ove = 86;
	RealMadrid.jog7.pos = 3;
	RealMadrid.jog7.tit = 1;
	RealMadrid.jog7.num = 13;
	RealMadrid.jog8.nome = "Xabi Alonso";
	RealMadrid.jog8.ove = 85;
	RealMadrid.jog8.pos = 3;
	RealMadrid.jog8.tit = 0;
	RealMadrid.jog8.num = 6;
	RealMadrid.jog9.nome = "Khedira";
	RealMadrid.jog9.ove = 83;
	RealMadrid.jog9.pos = 3;
	RealMadrid.jog9.tit = 1;
	RealMadrid.jog9.num = 14;
	RealMadrid.jog10.nome = "Lass Diarra";
	RealMadrid.jog10.ove = 83;
	RealMadrid.jog10.pos = 3;
	RealMadrid.jog10.tit = 1;
	RealMadrid.jog10.num = 15;
	RealMadrid.jog11.nome = "Cristiano Ronaldo";
	RealMadrid.jog11.ove = 93;
	RealMadrid.jog11.pos = 4;
	RealMadrid.jog11.tit = 0;
	RealMadrid.jog11.num = 7;
	RealMadrid.jog12.nome = "Benzema";
	RealMadrid.jog12.ove = 87;
	RealMadrid.jog12.pos = 4;
	RealMadrid.jog12.tit = 0;
	RealMadrid.jog12.num = 9;
	RealMadrid.jog13.nome = "Higuain";
	RealMadrid.jog13.ove = 86;
	RealMadrid.jog13.pos = 4;
	RealMadrid.jog13.tit = 1;
	RealMadrid.jog13.num = 16;
	RealMadrid.jog14.nome = "Di Maria";
	RealMadrid.jog14.ove = 89;
	RealMadrid.jog14.pos = 4;
	RealMadrid.jog14.tit = 0;
	RealMadrid.jog14.num = 11;
	RealMadrid.jog15.nome = "Mesut Ozil";
	RealMadrid.jog15.ove = 90;
	RealMadrid.jog15.pos = 3;
	RealMadrid.jog15.tit = 0;
	RealMadrid.jog15.num = 10;
	RealMadrid.jog16.nome = "Hamit Altintop";
	RealMadrid.jog16.ove = 80;
	RealMadrid.jog16.pos = 3;
	RealMadrid.jog16.tit = 1;
	RealMadrid.jog16.num = 18;
	RealMadrid.jog17.nome = "Arbeloa";
	RealMadrid.jog17.ove = 79;
	RealMadrid.jog17.pos = 2;
	RealMadrid.jog17.tit = 1;
	RealMadrid.jog17.num = 17;
	RealMadrid.jog18.nome = "Marcelo";
	RealMadrid.jog18.ove = 85;
	RealMadrid.jog18.pos = 2;
	RealMadrid.jog18.tit = 0;
	RealMadrid.jog18.num = 5;

	/*-----------BARCELONA-----------*/
	Barcelona.jog1.nome = "Valdés";
	Barcelona.jog1.ove = 81;
	Barcelona.jog1.pos = 1;
	Barcelona.jog1.tit = 0;
	Barcelona.jog1.num = 1;
	Barcelona.jog2.nome = "Dani Alves";
	Barcelona.jog2.ove = 85;
	Barcelona.jog2.pos = 2;
	Barcelona.jog2.tit = 0;
	Barcelona.jog2.num = 2;
	Barcelona.jog3.nome = "Piqué";
	Barcelona.jog3.ove = 86;
	Barcelona.jog3.pos = 2;
	Barcelona.jog3.tit = 0;
	Barcelona.jog3.num = 3;
	Barcelona.jog4.nome = "Puyol";
	Barcelona.jog4.ove = 85;
	Barcelona.jog4.pos = 2;
	Barcelona.jog4.tit = 0;
	Barcelona.jog4.num = 4;
	Barcelona.jog5.nome = "Eric Abidal";
	Barcelona.jog5.ove = 81;
	Barcelona.jog5.pos = 2;
	Barcelona.jog5.tit = 0;
	Barcelona.jog5.num = 5;
	Barcelona.jog6.nome = "Fabregas";
	Barcelona.jog6.ove = 87;
	Barcelona.jog6.pos = 3;
	Barcelona.jog6.tit = 0;
	Barcelona.jog6.num = 7;
	Barcelona.jog7.nome = "Xavi";
	Barcelona.jog7.ove = 90;
	Barcelona.jog7.pos = 3;
	Barcelona.jog7.tit = 0;
	Barcelona.jog7.num = 10;
	Barcelona.jog8.nome = "Iniesta";
	Barcelona.jog8.ove = 89;
	Barcelona.jog8.pos = 4;
	Barcelona.jog8.tit = 0;
	Barcelona.jog8.num = 8;
	Barcelona.jog9.nome = "Mascherano";
	Barcelona.jog9.ove = 80;
	Barcelona.jog9.pos = 3;
	Barcelona.jog9.tit = 1;
	Barcelona.jog9.num = 12;
	Barcelona.jog10.nome = "Seydou Keita";
	Barcelona.jog10.ove = 81;
	Barcelona.jog10.pos = 3;
	Barcelona.jog10.tit = 1;
	Barcelona.jog10.num = 13;
	Barcelona.jog11.nome = "Sergio Busquets";
	Barcelona.jog11.ove = 81;
	Barcelona.jog11.pos = 3;
	Barcelona.jog11.tit = 0;
	Barcelona.jog11.num = 6;
	Barcelona.jog12.nome = "David Villa";
	Barcelona.jog12.ove = 88;
	Barcelona.jog12.pos = 4;
	Barcelona.jog12.tit = 0;
	Barcelona.jog12.num = 9;
	Barcelona.jog13.nome = "Alexis Sanchez";
	Barcelona.jog13.ove = 86;
	Barcelona.jog13.pos = 4;
	Barcelona.jog13.tit = 1;
	Barcelona.jog13.num = 14;
	Barcelona.jog14.nome = "Lionel Messi";
	Barcelona.jog14.ove = 93;
	Barcelona.jog14.pos = 4;
	Barcelona.jog14.tit = 0;
	Barcelona.jog14.num = 11;
	Barcelona.jog15.nome = "Pedro";
	Barcelona.jog15.ove = 82;
	Barcelona.jog15.pos = 4;
	Barcelona.jog15.tit = 1;
	Barcelona.jog15.num = 15;
	Barcelona.jog16.nome = "Thiago Alcantara";
	Barcelona.jog16.ove = 81;
	Barcelona.jog16.pos = 3;
	Barcelona.jog16.tit = 1;
	Barcelona.jog16.num = 16;
	Barcelona.jog17.nome = "Maxwell";
	Barcelona.jog17.ove = 79;
	Barcelona.jog17.pos = 2;
	Barcelona.jog17.tit = 1;
	Barcelona.jog17.num = 17;
	Barcelona.jog18.nome = "Adriano";
	Barcelona.jog18.ove = 78;
	Barcelona.jog18.pos = 2;
	Barcelona.jog18.tit = 1;
	Barcelona.jog18.num = 18;
	Barcelona.num=2;

	RealMadrid.tac=t433t;
	Barcelona.tac=t433t;
	equipa3=RealMadrid;
	equipa4=Barcelona;
}

char getNameJogador(Equipa e, int num)
{
	if (e.jog1.num==num)
	{
		return *e.jog1.nome;
	}
	if (e.jog2.num==num)
	{
		return *e.jog2.nome;
	}
	if (e.jog3.num==num)
	{
		return *e.jog3.nome;
	}
	if (e.jog4.num==num)
	{
		return *e.jog4.nome;
	}
	if (e.jog5.num==num)
	{
		return *e.jog5.nome;
	}
	if (e.jog6.num==num)
	{
		return *e.jog6.nome;
	}
	if (e.jog7.num==num)
	{
		return *e.jog7.nome;
	}
	if (e.jog8.num==num)
	{
		return *e.jog8.nome;
	}
	if (e.jog9.num==num)
	{
		return *e.jog9.nome;
	}
	if (e.jog10.num==num)
	{
		return *e.jog10.nome;
	}
	if (e.jog11.num==num)
	{
		return *e.jog11.nome;
	}
	if (e.jog12.num==num)
	{
		return *e.jog12.nome;
	}
	if (e.jog13.num==num)
	{
		return *e.jog13.nome;
	}
	if (e.jog14.num==num)
	{
		return *e.jog14.nome;
	}
	if (e.jog15.num==num)
	{
		return *e.jog15.nome;
	}
	if (e.jog16.num==num)
	{
		return *e.jog16.nome;
	}
	if (e.jog17.num==num)
	{
		return *e.jog17.nome;
	}
	if (e.jog18.num==num)
	{
		return *e.jog18.nome;
	}
}

int getOvJogador(Equipa e, int num)
{
	if (e.jog1.num==num)
	{
		return e.jog1.ove;
	}
	if (e.jog2.num==num)
	{
		return e.jog2.ove;
	}
	if (e.jog3.num==num)
	{
		return e.jog3.ove;
	}
	if (e.jog4.num==num)
	{
		return e.jog4.ove;
	}
	if (e.jog5.num==num)
	{
		return e.jog5.ove;
	}
	if (e.jog6.num==num)
	{
		return e.jog6.ove;
	}
	if (e.jog7.num==num)
	{
		return e.jog7.ove;
	}
	if (e.jog8.num==num)
	{
		return e.jog8.ove;
	}
	if (e.jog9.num==num)
	{
		return e.jog9.ove;
	}
	if (e.jog10.num==num)
	{
		return e.jog10.ove;
	}
	if (e.jog11.num==num)
	{
		return e.jog11.ove;
	}
	if (e.jog12.num==num)
	{
		return e.jog12.ove;
	}
	if (e.jog13.num==num)
	{
		return e.jog13.ove;
	}
	if (e.jog14.num==num)
	{
		return e.jog14.ove;
	}
	if (e.jog15.num==num)
	{
		return e.jog15.ove;
	}
	if (e.jog16.num==num)
	{
		return e.jog16.ove;
	}
	if (e.jog17.num==num)
	{
		return e.jog17.ove;
	}
	if (e.jog18.num==num)
	{
		return e.jog18.ove;
	}
}

char getPosJogador(Equipa e, int num)
{
	if(e.jog1.num==num)
	{
		return e.jog1.pos;
	}
	if(e.jog2.num==num)
	{
		return e.jog2.pos;
	}
	if(e.jog3.num==num)
	{
		return e.jog3.pos;
	}
	if(e.jog4.num==num)
	{
		return e.jog4.pos;
	}
	if(e.jog5.num==num)
	{
		return e.jog5.pos;
	}
	if(e.jog6.num==num)
	{
		return e.jog6.pos;
	}
	if(e.jog7.num==num)
	{
		return e.jog7.pos;
	}
	if(e.jog8.num==num)
	{
		return e.jog8.pos;
	}
	if(e.jog9.num==num)
	{
		return e.jog9.pos;
	}
	if(e.jog10.num==num)
	{
		return e.jog10.pos;
	}
	if(e.jog11.num==num)
	{
		return e.jog11.pos;
	}
	if(e.jog12.num==num)
	{
		return e.jog12.pos;
	}
	if(e.jog13.num==num)
	{
		return e.jog13.pos;
	}
	if(e.jog14.num==num)
	{
		return e.jog14.pos;
	}
	if(e.jog15.num==num)
	{
		return e.jog15.pos;
	}
	if(e.jog16.num==num)
	{
		return e.jog16.pos;
	}
	if(e.jog17.num==num)
	{
		return e.jog17.pos;
	}
	if(e.jog18.num==num)
	{
		return e.jog18.pos;
	}
}

void setEquipa (int e, int nro)
{
	if (nro==3)
	{
		if (e==1)
		{
			equipa1=equipa3;
		}
		if (e==2)
		{
			equipa2=equipa3;
		}
	}
	if (nro==4)
	{
		if (e==1)
		{
			equipa1=equipa4;
		}
		if (e==2)
		{
			equipa2=equipa4;
		}
	}
}

int getNumEquipa(Equipa e)
{
	return e.num;
}

void Substituicao(Equipa e, int num1, int num2)
{
	if(e.jog1.num==num1 && e.jog1.tit==0)
	{
		e.jog1.num=num2;
	}
	if(e.jog2.num==num1 && e.jog2.tit==0)
	{
		e.jog2.num=num2;
	}
	if(e.jog3.num==num1 && e.jog3.tit==0)
	{
		e.jog3.num=num2;
	}
	if(e.jog4.num==num1 && e.jog4.tit==0)
	{
		e.jog4.num=num2;
	}
	if(e.jog5.num==num1 && e.jog5.tit==0)
	{
		e.jog5.num=num2;
	}
	if(e.jog6.num==num1 && e.jog6.tit==0)
	{
		e.jog6.num=num2;
	}
	if(e.jog7.num==num1 && e.jog7.tit==0)
	{
		e.jog7.num=num2;
	}
	if(e.jog8.num==num1 && e.jog8.tit==0)
	{
		e.jog8.num=num2;
	}
	if(e.jog9.num==num1 && e.jog9.tit==0)
	{
		e.jog9.num=num2;
	}
	if(e.jog10.num==num1 && e.jog10.tit==0)
	{
		e.jog10.num=num2;
	}
	if(e.jog11.num==num1 && e.jog11.tit==0)
	{
		e.jog11.num=num2;
	}
	if(e.jog12.num==num1 && e.jog12.tit==0)
	{
		e.jog12.num=num2;
	}
	if(e.jog13.num==num1 && e.jog13.tit==0)
	{
		e.jog13.num=num2;
	}
	if(e.jog14.num==num1 && e.jog14.tit==0)
	{
		e.jog14.num=num2;
	}
	if(e.jog15.num==num1 && e.jog15.tit==0)
	{
		e.jog15.num=num2;
	}
	if(e.jog16.num==num1 && e.jog16.tit==0)
	{
		e.jog16.num=num2;
	}
	if(e.jog17.num==num1 && e.jog17.tit==0)
	{
		e.jog17.num=num2;
	}
	if(e.jog18.num==num1 && e.jog18.tit==0)
	{
		e.jog18.num=num2;
	}
	/*===============================*/
	if(e.jog1.num==num2 && e.jog1.tit==1)
	{
		e.jog1.num=num1;
		e.jog1.tit=0;
	}
	if(e.jog2.num==num2 && e.jog2.tit==1)
	{
		e.jog2.num=num1;
		e.jog2.tit=0;
	}
	if(e.jog2.num==num2 && e.jog2.tit==1)
	{
		e.jog2.num=num1;
		e.jog2.tit=0;
	}
	if(e.jog3.num==num2 && e.jog3.tit==1)
	{
		e.jog3.num=num1;
		e.jog3.tit=0;
	}
	if(e.jog4.num==num2 && e.jog4.tit==1)
	{
		e.jog4.num=num1;
		e.jog4.tit=0;
	}
	if(e.jog5.num==num2 && e.jog5.tit==1)
	{
		e.jog5.num=num1;
		e.jog5.tit=0;
	}
	if(e.jog6.num==num2 && e.jog6.tit==1)
	{
		e.jog6.num=num1;
		e.jog6.tit=0;
	}
	if(e.jog7.num==num2 && e.jog7.tit==1)
	{
		e.jog7.num=num1;
		e.jog7.tit=0;
	}
	if(e.jog8.num==num2 && e.jog8.tit==1)
	{
		e.jog8.num=num1;
		e.jog8.tit=0;
	}
	if(e.jog9.num==num2 && e.jog9.tit==1)
	{
		e.jog9.num=num1;
		e.jog9.tit=0;
	}
	if(e.jog10.num==num2 && e.jog10.tit==1)
	{
		e.jog10.num=num1;
		e.jog10.tit=0;
	}
	if(e.jog11.num==num2 && e.jog11.tit==1)
	{
		e.jog11.num=num1;
		e.jog11.tit=0;
	}
	if(e.jog12.num==num2 && e.jog12.tit==1)
	{
		e.jog12.num=num1;
		e.jog12.tit=0;
	}
	if(e.jog13.num==num2 && e.jog13.tit==1)
	{
		e.jog13.num=num1;
		e.jog13.tit=0;
	}
	if(e.jog14.num==num2 && e.jog14.tit==1)
	{
		e.jog14.num=num1;
		e.jog14.tit=0;
	}
	if(e.jog15.num==num2 && e.jog15.tit==1)
	{
		e.jog15.num=num1;
		e.jog15.tit=0;
	}
	if(e.jog16.num==num2 && e.jog16.tit==1)
	{
		e.jog16.num=num1;
		e.jog16.tit=0;
	}
	if(e.jog17.num==num2 && e.jog17.tit==1)
	{
		e.jog17.num=num1;
		e.jog17.tit=0;
	}
	if(e.jog18.num==num2 && e.jog18.tit==1)
	{
		e.jog18.num=num1;
		e.jog18.tit=0;
	}
	/*===============================*/
	if(e.jog1.num==num2 && e.jog1.tit==0)
	{
		e.jog1.tit=1;
	}
	if(e.jog2.num==num2 && e.jog2.tit==0)
	{
		e.jog2.tit=1;
	}
	if(e.jog3.num==num2 && e.jog3.tit==0)
	{
		e.jog3.tit=1;
	}
	if(e.jog4.num==num2 && e.jog4.tit==0)
	{
		e.jog4.tit=1;
	}
	if(e.jog5.num==num2 && e.jog5.tit==0)
	{
		e.jog5.tit=1;
	}
	if(e.jog6.num==num2 && e.jog6.tit==0)
	{
		e.jog6.tit=1;
	}
	if(e.jog7.num==num2 && e.jog7.tit==0)
	{
		e.jog7.tit=1;
	}
	if(e.jog8.num==num2 && e.jog8.tit==0)
	{
		e.jog8.tit=1;
	}
	if(e.jog9.num==num2 && e.jog9.tit==0)
	{
		e.jog9.tit=1;
	}
	if(e.jog10.num==num2 && e.jog10.tit==0)
	{
		e.jog10.tit=1;
	}
	if(e.jog11.num==num2 && e.jog11.tit==0)
	{
		e.jog11.tit=1;
	}
	if(e.jog12.num==num2 && e.jog12.tit==0)
	{
		e.jog12.tit=1;
	}
	if(e.jog13.num==num2 && e.jog13.tit==0)
	{
		e.jog13.tit=1;
	}
	if(e.jog14.num==num2 && e.jog14.tit==0)
	{
		e.jog14.tit=1;
	}
	if(e.jog15.num==num2 && e.jog15.tit==0)
	{
		e.jog15.tit=1;
	}
	if(e.jog16.num==num2 && e.jog16.tit==0)
	{
		e.jog16.tit=1;
	}
	if(e.jog17.num==num2 && e.jog17.tit==0)
	{
		e.jog17.tit=1;
	}
	if(e.jog18.num==num2 && e.jog18.tit==0)
	{
		e.jog18.tit=1;
	}
}
