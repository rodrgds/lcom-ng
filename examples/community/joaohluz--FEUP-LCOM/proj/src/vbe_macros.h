#ifndef _VBE_MACROS_H_
#define _VBE_MACROS_H_
/** @defgroup VBE_MACROS VBE_MACROS
 * @{
 *
 * Macros importantes para funções da placa gráfica
 */

#define BIT(n) (0x01<<(n))

#define VBE_MODE  0x4f02
#define MODE_INFO 0x4f01
#define VBE_INFO  0x4f00
#define LINEAR    BIT(14)
#define INIT      0x10

#define INDEXED            0x105
#define DIRECT_COLOR_15    0x110
#define DIRECT_COLOR_24    0x115
#define DIRECT_COLOR_16    0x11a
#define DIRECT_COLOR_32    0x14c

/**@}*/

#endif
