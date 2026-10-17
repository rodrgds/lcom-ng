/*
After some reading and experementing I think it's better implemented this way
allows the use of macros similar to alloc and realloc previously defined
To use void one would need to use void pointers and always
use type casting after a generic func return
I'm quite happy with this solution XD , the macro madness

note:to use pointer types must use < typedef type* tyepeAlias > due to func name
*/

/** @defgroup smart_vector smart_vector
 * @{
 * "Generic" smart vectors(sv) using mainly macros.
 * \n Use the macro defineSmartVector(type) to define a struct <type>_SV.
 *
 *  Properties:
 *  - Allocates a given capacity and tracks the number of elements present in the vector;
 *  - Reallocates memory when capacity is exceeded by an increment given by the programmer;
 *  - Can remove elements or free them in case of pointers. Sends the element or pointer to las position of array
 *  .
 *  Notes:
 *  - Must use typedef to use pointers
 *  - Must free memory after using
 *  - Must include stdlib.h
 *  - Can use <type>_find to map or count by using check with return value always equal to 0
 *  .
 *
 *	 <b>Funcs defined with defineSmartVector(type) </b>
 *   - <b>(func)  <type>_SV *<type>_SV_init (int initialCapacity,unsigned int inc)</b>
 *   \n Allocates memory for new smart vector
 *   \n \b initialCapacity initial capacity of the array, will allocate space for <initialCapacity> number of elements
 *   \n \b inc defines how many elements are created when the current number of elements exceeds capacity
 *   \n \b returns pointer to created sv
 *   - <b>(func)  long type##_SV_find(type##_SV sv, char (*check) (type*) )</b>
 *   \n Search the first element that satisfies a certain condition
 *   \n \b sv smart vector to iterate
 *   \n \b check condition, receives element by reference and if it does not fit the criteria should return 0 or something else otherwise
 *   \n \b returns index of first element found or negative if not found
 *
 */

#ifndef __SMARTVECTOR_H
#define __SMARTVECTOR_H

#define SV_REALLOC(type,number,sv) do {	\
		type* tmp= (type *) realloc(sv->elems,(number)*sizeof(type)); \
		if(tmp!=NULL) sv->elems=tmp; \
		} while(0)
//else printf("\nSV_REALLOC (type:%s,number:%s) FAILED",#type,#number);\

/**
 * @brief Add element to sv
 * @param sv smart vector of given type
 * @param elem element to be added to the smart vector
 * @param type and element type
 */
#define SV_ADD(sv,elem,type) do {\
if(sv->size>=sv->currentMaxCapacity) {\
SV_REALLOC(type,(sv->currentMaxCapacity+sv->increment),sv);\
sv->currentMaxCapacity=sv->currentMaxCapacity+sv->increment;\
}\
sv->elems[sv->size]=elem;\
sv->size++;\
} while(0)

/**
 * @brief Removes element at given index and places it on given destination
 * @param sv smart vector
 * @param index index of the value to be removed
 * @param dest destination
 */
#define SV_REMOVE_TO(sv,index,dest) do {\
if (sv->size>0){\
dest=sv->elems[index];\
if (sv->size>index&&sv->size>1) sv->elems[index]=sv->elems[sv->size-1];\
sv->size--;\
}\
} while(0)

/**
 * @brief Removes element at given index
 * @param sv smart vector
 * @param index index of the value to be removed
 */
#define SV_REMOVE(sv,index) do {\
if (sv->size>0){\
if (sv->size>index&&sv->size>1) sv->elems[index]=sv->elems[sv->size-1];\
sv->size--;\
}\
} while(0)

/**
 * @brief Removes all elements
 * @param sv smart vector
 */
#define SV_REMOVE_ALL(sv) do {\
sv->size=0;\
}\
} while(0)

/**
 * @brief Removes element at given index and frees memory used by it (use with pointers only)
 * @param sv smart vector
 * @param index index of the value to be deleted
 * \n note: does not free stuff pointed by pointed object. use with caution.
 */
#define SV_DELETE(sv,index) do {\
if (sv->size>0){\
free(sv->elems[index]);\
if (sv->size>index&&sv->size>1) sv->elems[index]=sv->elems[sv->size-1];\
sv->size--;\
}\
} while(0)

/**
 * @brief Removes all elements and frees memory used by them (use with pointers only)
 * @param sv smart vector
 * \n note: does not free stuff pointed by pointed objects. use with caution.
 */
#define SV_DELETE_ALL(sv) do {\
long i=sv->size-1;\
while (i>=0){\
free(sv->elems[i]);\
i--;}\
sv->size=0;\
}\
} while(0)

/**
 * @brief Defines a sv struct for given type and init and find funcs
 * @param type type
 * \n notes:pointer types must use typedef
 */
#define defineSmartVector(type) \
typedef struct type##_SV_struct {\
    type *elems; \
    unsigned int size; \
	unsigned int currentMaxCapacity; \
	unsigned int increment; \
}type##_SV;\
static type##_SV *type##_SV_init(int initialCapacity,unsigned int inc)\
{\
    type##_SV *vector = (type##_SV*) malloc(sizeof(type##_SV));\
    if (!vector) return NULL;\
    vector->elems = (type*) malloc(sizeof(type) * initialCapacity);\
    if (!vector->elems) { free(vector); return NULL; }\
    vector->size=0;\
	vector->increment=inc;\
	if(!vector->increment)vector->increment++;\
	vector->currentMaxCapacity = initialCapacity;\
    return vector;\
}\
static long type##_SV_find(type##_SV *sv, char (*check) (type*) )\
{\
long i=0;\
for (;i<sv->size;i++) if (check(& sv->elems[i])!=0) return i;\
return -1;\
}
//not expecting to call the same sort many times
//maybe better to use a func otherwise
//sorter char (*comp) (type,type)
/**
 * @brief Sorts given sv
 * @param sv smart vector to sort
 * @param comp func used to compare, should return different than 0 if element has priority over others
 * @param type type of size elements
 * \n notes:pointer types must use typedef
 */
#define SV_SORT(sv,comp,type){\
unsigned int p;\
for (p = 1; p < sv->size; p++){\
type tmp = sv->elems[p];\
unsigned int j; \
  for (j = p; j > 0 && ( (*comp) (tmp,sv->elems[j-1]) ); j--) \
     sv->elems[j] = sv->elems[j-1]; \
  sv->elems[j] = tmp; \
}\
\
}

/**
 * @brief frees allocated mem used by sv (struct and array only!)
 * @param sv smart vector
 * \n note:does not free stuff pointed in array when using pointer types.
 */
#define SV_FREE(sv){\
free(sv->elems);\
free(sv);\
}

/** @} end of smart_vector */
#endif //__SMARTVECTOR_H
