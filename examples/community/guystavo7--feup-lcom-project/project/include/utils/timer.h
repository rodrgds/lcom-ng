#ifndef TIMER
#define TIMER

#pragma once

/** @file timer.h
 *  @brief Temporizador simples com contagem decrescente e estado parado/corrido.
 */

typedef struct Timer Timer;

#include <stdlib.h>
#include <stdbool.h>

/** @brief Cria um temporizador com tempo inicial e contador iguais a time, parado.
 *  @param[in] time Duracao inicial em segundos (valor do contador ao reiniciar).
 *  @return Ponteiro para Timer alocado em heap.
 */
Timer* timerNew(float time);

/** @brief Liberta a memoria do temporizador.
 *  @param[in] timer Instancia a libertar.
 */
void timerFree(Timer* timer);

/** @brief Devolve o tempo inicial configurado (valor de reset).
 *  @param[in] timer Temporizador valido.
 *  @return Tempo inicial em segundos.
 */
float timerGetInitialTime(Timer* timer);

/** @brief Define o tempo inicial usado em timerRestart.
 *  @param[in,out] timer Temporizador a configurar.
 *  @param[in] initialTime Novo tempo inicial em segundos.
 */
void timerSetInitialTime(Timer* timer, float initialTime);

/** @brief Retoma a contagem se estava parado (nao altera o contador).
 *  @param[in,out] timer Temporizador.
 */
void timerStart(Timer* timer);

/** @brief Repoe o contador para o tempo inicial e marca o timer como a correr.
 *  @param[in,out] timer Temporizador.
 */
void timerRestart(Timer* timer);

/** @brief Para o temporizador (a contagem deixa de diminuir).
 *  @param[in,out] timer Temporizador.
 */
void timerStop(Timer* timer);

/** @brief Indica se o timer esta activo e ainda nao chegou a zero.
 *  @param[in] timer Temporizador.
 *  @return true se nao esta parado e counter > 0; caso contrario false.
 */
bool timerIsRunning(Timer* timer);

/** @brief Decrementa o contador em deltaTime enquanto o timer corre.
 *  @param[in,out] timer Temporizador.
 *  @param[in] deltaTime Passo de tempo em segundos (tipicamente frame delta).
 */
void timerUpdate(Timer* timer, float deltaTime);

#endif
