/**
 * @file profiler.h
 * @brief Lightweight manual section profiler for performance debugging.
 * @ingroup group_profiler
 *
 * When compiled with -DNINJIX_PROFILE, profiling macros record nested
 * section timing (usec resolution) and output a folded FlameGraph file
 * on shutdown. When profiling is disabled, all macros expand to no-ops.
 *
 * The output folded file can be visualised with Brendan Gregg's FlameGraph
 * tool.  An example flame graph is shown below:
 *
 * ![Ninjix flame graph](ninjix.svg)
 */

#ifndef PROJ_PROFILER_H
#define PROJ_PROFILER_H

#include <stdint.h>

#ifdef NINJIX_PROFILE

void profiler_reset(void);
void profiler_begin(const char *name);
void profiler_end(void);
void profiler_report(void);

#define PROFILE_RESET() profiler_reset()
#define PROFILE_BEGIN(name) profiler_begin(name)
#define PROFILE_END() profiler_end()
#define PROFILE_REPORT() profiler_report()

#else

#define PROFILE_RESET() ((void)0)
#define PROFILE_BEGIN(name) ((void)0)
#define PROFILE_END() ((void)0)
#define PROFILE_REPORT() ((void)0)

#endif

#endif
