#include "scheduler/scheduler.h"
#include "vector/vector.h"
#include "events/events.h"
#include "devices/timer.h"

static size_t id = 0;
static vector *tasks;

int (scheduler_init)() {
  tasks = vec_create(sizeof(struct scheduler_task));

  event_on(TIMER_TICK, scheduler_tick);
  return OKAY;
}

int (scheduler_cleanup)() {
  vec_destroy(tasks);

  event_unregister(TIMER_TICK, scheduler_tick);
  return OKAY;
}

size_t (scheduler_set_timeout)(unsigned long seconds, void (*action)()) {
  struct scheduler_task task;

  task.id = id++;
  task.initial_ticks = task.remaining_ticks = seconds * timer_get_frequency();
  task.action = action;
  task.repeat = false;

  vec_push(tasks, &task);
  return task.id;
}

size_t (scheduler_set_interval)(unsigned long seconds, void (*action)()) {
  struct scheduler_task task;

  task.id = id++;
  task.initial_ticks = task.remaining_ticks = seconds * timer_get_frequency();
  task.action = action;
  task.repeat = true;

  vec_push(tasks, &task);
  return task.id;
}

bool (scheduler_remove_task)(size_t id) {
  bool found = false;
  size_t l = 0;
  size_t r = tasks->len;

  size_t mid;
  while (l <= r) {
    mid = (l + r) / 2;
    struct scheduler_task *task = vec_at(tasks, mid);

    if (task->id == id) {
      found = true;
      break;
    } else if (task->id > id) {
      r = mid - 1;
    } else if (task->id < id) {
      l = mid + 1;
    }
  }

  if (!found) {
    return false;
  }

  vec_erase(tasks, mid);
  return true;
}

void (scheduler_tick)() {
  for (size_t i = 0; i < tasks->len;) {
    struct scheduler_task *task = vec_at(tasks, i);

    if (task->remaining_ticks == 0) {
      task->action();

      if (task->repeat) {
        task->remaining_ticks = task->initial_ticks;
        i++;
      } else {
        vec_erase(tasks, i);
      }

      continue;
    }

    task->remaining_ticks--;
    i++;
  }
}
