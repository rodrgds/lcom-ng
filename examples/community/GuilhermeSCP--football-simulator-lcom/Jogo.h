#ifndef __JOGO_H_
#define __JOGO_H_

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include <stdio.h>
#include <math.h>

#include "Base.h"

int  G1, G2;

void setResultado(Equipa e1, Equipa e2);
void AlteraDificuldade(double d);

#endif __JOGO_H_
