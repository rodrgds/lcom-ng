#if defined(EXPLICIT_BEFORE)
#include <stdbool.h>
#endif
#if defined(MODERN)
#include <lcom/lcf.h>
#elif defined(MODERN_TIMER)
#include <lcom/timer.h>
#else
#include <minix/drivers.h>
#endif
#if defined(EXPLICIT_AFTER)
#include <stdbool.h>
#endif
#include <assert.h>
#if defined(__cplusplus)
#include <type_traits>

static_assert(std::is_same<decltype(&lcf_set_language),bool(*)(const char*)>::value,"C++ boolean function ABI");
#ifndef LCOM_COMPAT_LEGACY
static_assert(std::is_same<decltype(((struct packet*)0)->lb),bool>::value,"C++ packet boolean type");
static_assert(sizeof(((struct packet*)0)->lb)==1,"C++ packet boolean width");
#endif
#elif !defined(MODERN) && !defined(MODERN_TIMER) && !defined(EXPLICIT_BEFORE) && !defined(EXPLICIT_AFTER)
#ifdef bool
#error legacy_minix_drivers_does_not_own_stdbool
#endif
/* The old BMP library owns an integer typedef; local identifiers may shadow it. */
typedef int bool;
_Static_assert(sizeof(bool)==sizeof(int),"student boolean typedef remains student-owned");
static char artist_local(void){char bool=7;return bool;}
#else
#ifndef bool
#error explicit_modern_or_stdbool_header_owns_bool_macro
#endif
_Static_assert(sizeof(bool)==1,"C standard bool ABI");
#endif
#if !defined(__cplusplus) && !defined(LCOM_COMPAT_LEGACY)
_Static_assert(sizeof(((struct packet*)0)->lb)==1,"packet boolean ABI stays one byte");
_Static_assert(sizeof(((struct packet*)0)->x_ov)==1,"overflow boolean ABI stays one byte");

_Static_assert(__builtin_types_compatible_p(__typeof__(&kbd_print_scancode),int(*)(_Bool,uint8_t,uint8_t*)),"boolean argument ABI");
#endif
#ifndef __cplusplus
_Static_assert(__builtin_types_compatible_p(__typeof__(&lcf_set_language),_Bool(*)(const char*)),"boolean return ABI");
#endif
int main(void){
#ifndef LCOM_COMPAT_LEGACY
struct packet p={0};p.lb=1;p.rb=0;p.x_ov=1;assert(p.lb&&!p.rb&&p.x_ov);
int marker; mmap_t mapping={0};
#ifdef __cplusplus
mapping.virt=&marker;
#else
mapping.virtual=&marker;
#endif
assert(mapping.virt==&marker);
#endif
#if !defined(__cplusplus) && !defined(MODERN) && !defined(MODERN_TIMER) && !defined(EXPLICIT_BEFORE) && !defined(EXPLICIT_AFTER)
assert(artist_local()==7);
#else
bool value=true;assert(value);
#endif
return 0;}
