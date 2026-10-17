#include <lcom/lcf.h>

#include "list.h"

/*  NOTE FOR TYPES:
0 = Object
1 = Bullet  */

void freeElement(ListElement *element, uint8_t type) {
    if (type == 1) {
        Bullet* obj = element->value;
        freeObject(obj->object);
        free(element->value);
        return;
    }
    free(element->value);
    free(element);
}

void freeList(List *list, uint8_t type) {
    ListElement* ptr = list->head;
    ListElement* prev = NULL;

    while (ptr != NULL) {
        prev = ptr;
        ptr = prev->next;
        freeElement(prev, type);
    }
    
}

List* create_list(){
    List* list = malloc(sizeof(* list));
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void* get_front(List* list){
    return (list->size) ? list->head->value : NULL;
}

void* get_back(List* list){
    return (list->size) ? list->tail->value : NULL;
}

void push_back(List* list, void* value){
    // create the new element
    ListElement* new_element = (ListElement*) malloc(sizeof *new_element);
    new_element->value = value;
    new_element->next = NULL;
    new_element->prev = list->tail;

    // update the list
    if (list->size) list->tail->next = new_element;
    if (!list->size) list->head = new_element;
    list->tail = new_element;

    ++list->size;
}

void push_front(List* list, void* value){
    // create the new element
    ListElement* new_element = (ListElement*) malloc(sizeof *new_element);
    new_element->value = value;
    new_element->next = list->head;
    new_element->prev = NULL;

    // update the list
    if (list->size) list->head->prev = new_element;
    list->head = new_element;
    if (!list->size) list->tail = new_element;

    ++list->size;
}

void pop_back(List* list, uint8_t type){
    if (!list->size) return;

    // update the list
    --list->size;

    ListElement* old_tail = list->tail;
    list->tail = old_tail->prev;

    if (list->size)
        list->tail->next = NULL;
    else
        list->head = NULL;

    freeElement(old_tail, type);
}

void pop_front(List* list, uint8_t type){
    if (!list->size) return;

    // update the list
    --list->size;

    ListElement* old_head = list->head;
    list->head = old_head->next;

    if (list->size)
        list->head->prev = NULL;
    else
        list->tail = NULL;

    freeElement(old_head, type);
}

void pop_element(List* list, ListElement* element, uint8_t type) {
    // update the size
    --list->size;

    // remove the element
    if(list->size == 0){
        list->head = NULL;
        list->tail = NULL;
    }else if (element == list->head) {
        element->next->prev = NULL;
        list->head = element->next;
    } else if (element == list->tail) {
        element->prev->next = NULL;
        list->tail = element->prev;
    } else {
        element->next->prev = element->prev;
        element->prev->next = element->next;
    }

    freeElement(element, type);
}
