/**
 * @file profiler.c
 * @brief Manual section profiler -- nested timing, folded FlameGraph output.
 * @ingroup group_profiler
 *
 * Instruments hot paths with PROFILE_BEGIN/PROFILE_END macros. Each
 * section accumulates self-time in microseconds. On shutdown,
 * profiler_report() writes a folded stack file suitable for FlameGraph
 * rendering and prints a summary table to stdout.
 */

#include "profiler.h"

#ifdef NINJIX_PROFILE

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define PROFILER_MAX_STACK 64
#define PROFILER_MAX_STATS 1024
#define PROFILER_MAX_PATH 512
#define PROFILER_MAX_NAME 96
#define PROFILER_OUTPUT_PATH "/home/lcom/labs/proj/ninjix.folded"

typedef struct {
  const char *name;
  uint64_t start_us;
  uint64_t child_us;
} profiler_stack_entry_t;

typedef struct {
  char path[PROFILER_MAX_PATH];
  char name[PROFILER_MAX_NAME];
  uint64_t self_us;
  uint64_t total_us;
  uint32_t calls;
} profiler_stat_t;

static profiler_stack_entry_t profiler_stack[PROFILER_MAX_STACK];
static profiler_stat_t profiler_stats[PROFILER_MAX_STATS];
static uint16_t profiler_stack_depth;
static uint16_t profiler_dropped_depth;
static uint16_t profiler_stat_count;
static bool profiler_overflowed;

static uint64_t profiler_now_us(void)
{
  struct timespec ts;

#ifdef CLOCK_MONOTONIC
  if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
#endif
    if (clock_gettime(CLOCK_REALTIME, &ts) != 0)
      return 0;

  return (uint64_t)ts.tv_sec * 1000000ULL + (uint64_t)ts.tv_nsec / 1000ULL;
}

static void profiler_copy_truncated(char *dst, size_t dst_size, const char *src)
{
  if (dst == NULL || dst_size == 0)
    return;

  if (src == NULL)
    src = "(null)";

  strncpy(dst, src, dst_size - 1);
  dst[dst_size - 1] = '\0';
}

static void profiler_current_path(char *path, size_t path_size)
{
  size_t used = 0;

  if (path == NULL || path_size == 0)
    return;

  path[0] = '\0';
  for (uint16_t i = 0; i < profiler_stack_depth; i++)
  {
    const char *name = profiler_stack[i].name != NULL ? profiler_stack[i].name : "(null)";
    int written = snprintf(path + used, path_size - used, "%s%s",
                           i == 0 ? "" : ";", name);
    if (written < 0)
      break;

    if ((size_t)written >= path_size - used)
    {
      path[path_size - 1] = '\0';
      profiler_overflowed = true;
      break;
    }

    used += (size_t)written;
  }
}

static profiler_stat_t *profiler_find_or_add_stat(const char *path, const char *name)
{
  for (uint16_t i = 0; i < profiler_stat_count; i++)
  {
    if (strcmp(profiler_stats[i].path, path) == 0)
      return &profiler_stats[i];
  }

  if (profiler_stat_count >= PROFILER_MAX_STATS)
  {
    profiler_overflowed = true;
    return NULL;
  }

  profiler_stat_t *stat = &profiler_stats[profiler_stat_count++];
  memset(stat, 0, sizeof(*stat));
  profiler_copy_truncated(stat->path, sizeof(stat->path), path);
  profiler_copy_truncated(stat->name, sizeof(stat->name), name);
  return stat;
}

void profiler_reset(void)
{
  memset(profiler_stack, 0, sizeof(profiler_stack));
  memset(profiler_stats, 0, sizeof(profiler_stats));
  profiler_stack_depth = 0;
  profiler_dropped_depth = 0;
  profiler_stat_count = 0;
  profiler_overflowed = false;
}

void profiler_begin(const char *name)
{
  if (profiler_stack_depth >= PROFILER_MAX_STACK)
  {
    if (profiler_dropped_depth < UINT16_MAX)
      profiler_dropped_depth++;
    profiler_overflowed = true;
    return;
  }

  profiler_stack_entry_t *entry = &profiler_stack[profiler_stack_depth++];
  entry->name = name;
  entry->start_us = profiler_now_us();
  entry->child_us = 0;
}

void profiler_end(void)
{
  uint64_t now_us = profiler_now_us();
  char path[PROFILER_MAX_PATH];

  if (profiler_dropped_depth > 0)
  {
    profiler_dropped_depth--;
    return;
  }

  if (profiler_stack_depth == 0)
  {
    profiler_overflowed = true;
    return;
  }

  profiler_stack_entry_t entry = profiler_stack[profiler_stack_depth - 1];
  uint64_t elapsed_us = now_us >= entry.start_us ? now_us - entry.start_us : 0;
  uint64_t self_us = elapsed_us >= entry.child_us ? elapsed_us - entry.child_us : 0;

  profiler_current_path(path, sizeof(path));
  profiler_stat_t *stat = profiler_find_or_add_stat(path, entry.name);
  if (stat != NULL)
  {
    stat->self_us += self_us;
    stat->total_us += elapsed_us;
    stat->calls++;
  }

  profiler_stack_depth--;
  if (profiler_stack_depth > 0)
    profiler_stack[profiler_stack_depth - 1].child_us += elapsed_us;
}

static void profiler_write_folded(FILE *stream)
{
  for (uint16_t i = 0; i < profiler_stat_count; i++)
  {
    if (profiler_stats[i].self_us == 0)
      continue;

    fprintf(stream, "%s %llu\n",
            profiler_stats[i].path,
            (unsigned long long)profiler_stats[i].self_us);
  }
}

static void profiler_print_summary(void)
{
  printf("\nNinjix profiler summary (microseconds):\n");
  printf("%-34s %10s %10s %8s\n", "section", "self", "total", "calls");

  for (uint16_t i = 0; i < profiler_stat_count; i++)
  {
    printf("%-34s %10llu %10llu %8u\n",
           profiler_stats[i].name,
           (unsigned long long)profiler_stats[i].self_us,
           (unsigned long long)profiler_stats[i].total_us,
           (unsigned)profiler_stats[i].calls);
  }

  if (profiler_overflowed)
    printf("profiler: stack/stat/path capacity was exceeded; output may be incomplete\n");
}

void profiler_report(void)
{
  FILE *folded = fopen(PROFILER_OUTPUT_PATH, "w");
  if (folded != NULL)
  {
    profiler_write_folded(folded);
    fclose(folded);
    printf("Ninjix profiler folded output: %s\n", PROFILER_OUTPUT_PATH);
  }
  else
  {
    printf("Ninjix profiler folded output follows:\n");
    profiler_write_folded(stdout);
  }

  profiler_print_summary();
}

#endif
