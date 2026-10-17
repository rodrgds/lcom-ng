/**
 * @file list.h
 *
 */

#ifndef _LCOM_LIST_H_
#define _LCOM_LIST_H_

#include "bullet.h"
#include "object.h"

/**
 * @brief struct that represents an element of a list
 * @param value the actual element
 * @param prev the previous element of the list
 * @param next the next element of the list
 */
typedef struct ListElement {
    void* value;
    struct ListElement* prev;
    struct ListElement* next;
} ListElement;

/**
 * @brief struct that represents a list
 * @param head the current head (first element) of the list
 * @param tail the current tail (last element) of the list
 * @param size the current size of the list
 */
typedef struct {
    ListElement* head;
    ListElement* tail;
    int size;
} List;

/**
 * @brief Create a list object
 * 
 * @return List* 
 */
List* create_list();

/* GETTERS */
/**
 * @brief returns the element at the front of the list
 * @param list list whose front element will be returned
 * @return void* the element at the front of the list
 */
void* get_front(List* list);

/**
 * @brief returns the element at the back of the list
 * @param list list whose back element will be returned
 * @return void* the element at the back of the list
 */
void* get_back(List* list);

/**
 * @brief inserts a new element at the back of the list
 * @param list list wherein the element will be inserted
 * @param value element to be inserted
 */
void push_back(List* list, void* value);

/**
 * @brief inserts a new element at the front of the list
 * @param list list wherein the element will be inserted
 * @param value element to be inserted
 */
void push_front(List* list, void* value);

/**
 * @brief removes the element at the back of the list
 * @param list list whose back element will be removed
 * @param type if 0, the element is an Object, otherwise it is a Bullet
 */
void pop_back(List* list, uint8_t type);

/**
 * @brief removes the element at the front of the list
 * @param list list whose front element will be removed
 * @param type if 0, the element is an Object, otherwise it is a Bullet
 */
void pop_front(List* list, uint8_t type);

/**
 * @brief deallocates the memory of a list element
 * @param list list where the element to be removed belongs
 * @param element element whose memory will be freed
 * @param type if 0, the element is an Object, otherwise it is a Bullet
 */
void pop_element(List* list, ListElement* element, uint8_t type);

/**
 * @brief deallocates the memory of a list element
 * @param element element whose memory will be freed
 * @param type if 0, the element is an Object, otherwise it is a Bullet
 */
void freeElement(ListElement *element, uint8_t type);

/**
 * @brief deallocates the memory of a list
 * @param list list whose memory will be freed
 * @param type if 0, the list contained Objects, otherwise it contained Bullets
 */
void freeList(List *list, uint8_t type);

#endif // _LCOM_LIST_H_
