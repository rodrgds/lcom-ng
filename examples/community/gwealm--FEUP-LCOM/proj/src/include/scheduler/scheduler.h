#pragma once

#include <lcom/lcf.h>
#include "errors.h"

/**
 * @brief Keeps scheduler data
 * 
 */
struct scheduler_task {
  size_t id;

  unsigned long initial_ticks;
  unsigned long remaining_ticks;

  void (*action)();

  bool repeat;
};

/**
 * @brief Initializes scheduler
 * 
 */
int (scheduler_init)();

/**
 * @brief Reset scheduler
 * 
 */
int (scheduler_cleanup)();

/**
 * @brief Sets timeout
 * 
 * @param seconds Timeout seconds
 * @param action  Function to be executed after the timeout
 */
size_t (scheduler_set_timeout)(unsigned long seconds, void (*action)());

/**
 * @brief Sets scheduler interval
 * 
 * @param seconds Interval seconds
 * @param action Function to be executed after the interval
 */
size_t (scheduler_set_interval)(unsigned long seconds, void (*action)());

/**
 * @brief Removes task from scheduler
 * 
 * @param id Task id
 */
bool (scheduler_remove_task)(size_t id);

/**
 * @brief Reduces the time in each task
 * 
 */
void (scheduler_tick)();
