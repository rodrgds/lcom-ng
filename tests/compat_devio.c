#include <minix/devio.h>
#include <stddef.h>
_Static_assert(sizeof(port_t)==2,"16-bit I/O port");
_Static_assert(sizeof(((pvb_pair_t*)0)->value)==1,"8-bit I/O payload");
_Static_assert(sizeof(((pvw_pair_t*)0)->value)==2,"16-bit I/O payload");
_Static_assert(sizeof(((pvl_pair_t*)0)->value)==4,"32-bit I/O payload");
_Static_assert(offsetof(pvb_pair_t,port)==0 && offsetof(pvw_pair_t,port)==0 && offsetof(pvl_pair_t,port)==0,"port leads each pair");
#include <assert.h>
#include <setjmp.h>
static jmp_buf failed;
void panic(const char *format, ...) { (void)format; longjmp(failed, 1); }
int main(void) {
    pvb_pair_t byte;
    pvw_pair_t word;
    pvl_pair_t full;
    pv_set(byte, 0x60, 0xf4);
    pv_set(word, 0x3f8, 65535);
    pv_set(full, 0x3f8, 0xffffffffu);
    assert(byte.port == 0x60 && byte.value == 0xf4);
    assert(word.value == 65535 && full.value == 0xffffffffu);
    if (!setjmp(failed)) { pv_set(byte, 0x60, 256); return 1; }
    if (!setjmp(failed)) { pv_set(byte, 65536, 1); return 2; }
    return 0;
}
