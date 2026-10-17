#ifndef _LCOM_MACROS_H_
#define _LCOM_MACROS_H_

#include <lcom/lcf.h>

/*!
 * @def CHECK_NULL(returnValue)
 * @brief Checks if the return value is non-zero and returns NULL if true.
 *
 * This macro is used to validate a return value and return NULL in case of an error.
 *
 * @param returnValue The value to check.
 */
#define CHECK_NULL(returnValue) do{ if((returnValue) != 0) return NULL; } while(0)

/*!
 * @def CHECK_RETURN(returnValue)
 * @brief Checks if the return value is non-zero and returns 1 if true.
 *
 * This macro is used to validate a return value and return 1 in case of an error.
 *
 * @param returnValue The value to check.
 */
#define CHECK_RETURN(returnValue) do{ if((returnValue) != 0) return 1; } while(0)

/*!
 * @def CHECK_VOID(returnValue)
 * @brief Checks if the return value is non-zero and returns void if true.
 *
 * This macro is used to validate a return value and return void in case of an error.
 *
 * @param returnValue The value to check.
 */
#define CHECK_VOID(returnValue) do{ if((returnValue) != 0) return; } while(0)

/*!
 * @def CHECK_ERROR(returnValue, error, ...)
 * @brief Checks if the return value is non-zero, prints an error message, and returns void if true.
 *
 * This macro is used to validate a return value, print an error message, and return void in case of an error.
 *
 * @param returnValue The value to check.
 * @param error The error message to print.
 * @param ... Additional arguments for the error message.
 */
#define CHECK_ERROR(returnValue, error, ...) do{ if((returnValue) != 0) {printf(error, __VA_ARGS__); return;} } while(0)

/*!
 * @def CHECK_NULL_ERROR(returnValue, error, ...)
 * @brief Checks if the return value is non-zero, prints an error message, and returns NULL if true.
 *
 * This macro is used to validate a return value, print an error message, and return NULL in case of an error.
 *
 * @param returnValue The value to check.
 * @param error The error message to print.
 * @param ... Additional arguments for the error message.
 */
#define CHECK_NULL_ERROR(returnValue, error, ...) do{ if((returnValue) != 0){ printf(error, __VA_ARGS__); return NULL; } } while(0)

/*!
 * @def CHECK_RETURN_ERROR(returnValue, error, ...)
 * @brief Checks if the return value is non-zero, prints an error message, and returns 1 if true.
 *
 * This macro is used to validate a return value, print an error message, and return 1 in case of an error.
 *
 * @param returnValue The value to check.
 * @param error The error message to print.
 * @param ... Additional arguments for the error message.
 */
#define CHECK_RETURN_ERROR(returnValue, error, ...) do{ if((returnValue) != 0){ printf(error, __VA_ARGS__); return 1; } } while(0)

/*!
 * @def getData(i, d, v)
 * @brief Retrieves the data pointer from a struct and validates it.
 *
 * This macro is used to retrieve the `data` field from a struct and validate its existence.
 * If the struct or its `data` field is NULL, the macro returns the specified value.
 *
 * @param i Pointer to the struct.
 * @param d Variable to store the `data` pointer.
 * @param v Value to return in case of an error.
 */
#define getData(i, d, v) do{if(!(i)) return (v); d = (i)->data; if(!(d)) return (v); }while(0)

/*!
 * @def getVoidData(i, d)
 * @brief Retrieves the data pointer from a struct and validates it, returning void on error.
 *
 * This macro is used to retrieve the `data` field from a struct and validate its existence.
 * If the struct or its `data` field is NULL, the macro returns void.
 *
 * @param i Pointer to the struct.
 * @param d Variable to store the `data` pointer.
 */
#define getVoidData(i, d) do{if(!(i)) return; d = (i)->data; if(!(d)) return; }while(0)

#endif
