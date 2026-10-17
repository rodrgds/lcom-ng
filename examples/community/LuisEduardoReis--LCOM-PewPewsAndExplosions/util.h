#ifndef __UTIL_H_
#define __UTIL_H_

#include <math.h>

#define PI 3.14159265

/*!
 * devolve o minimo entre a e b
 */
#define min(a,b) ( (a<b) ? a : b )
/*!
 * devolve o maximo entre a e b
 */
#define max(a,b) ( (a>b) ? a : b )

/*!
 * devolve a posicao de um passo no percurso de a ate b
 * \param a ponto de origem
 * \param b ponto final
 * \param step incremento
 */
#define stepTo(a,b,step) ( (a>b) ? max(a-step, b) : min (a+step, b) )

/*!
 * cria uma cor em rgb
 * \param r componente de vermelho
 * \param g componente de verde
 * \param b componente de azul
 */
#define makeColor(r,g,b) ( ((r & 0xFF) << 16) + ((g & 0xFF) << 8) + (b & 0xFF))

/*!
 * devolve um numero aleatorio entre a e b
 */
#define randomRange(a,b) ((b-a)<=0 ? a : ((rand() % (b-a)) + a))


/*!
 * devolve a direcao entre dois pontos (angulo do vetor formado pelos dois pontos com a horizontal)
 * \param x1 x do ponto 1
 * \param y1 y do ponto 1
 * \param x2 x do ponto 2
 * \param y2 y do ponto 2
 */
#define point_direction(x1,y1, x2,y2) (atan2(x2-x1,y2-y1) * (180.0/PI) - 90)

/*!
 * devolve a distancia
 * \param x1 x do ponto 1
 * \param y1 y do ponto 1
 * \param x2 x do ponto 2
 * \param y2 y do ponto 2
 */
#define point_distance(x1,y1, x2,y2) sqrt(pow(x2-x1,2) + pow(y2-y1,2))

/*!
 * devolve a componente x de um vetor
 * \param len tamanho do vetor
 * \param dir direcao do vetor (angulo com a horizontal)
 */
#define lengthdir_x(len,dir) (cos(dir * (PI/180.0))*len)

/*!
 * devolve a componente y de um vetor
 * \param len tamanho do vetor
 * \param dir direcao do vetor (angulo com a horizontal)
 */
#define lengthdir_y(len,dir) (-sin(dir * (PI/180.0))*len)


/*!
 * escolhe aleatoriamente entre dois numeros
 */
#define randomBetween(a, b) (a + rand()%2 * (b-a))


#endif
