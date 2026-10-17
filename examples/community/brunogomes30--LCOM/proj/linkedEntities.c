#include "linkedEntities.h"

void addEntity(DoubleLinkedList *list, void *sprite, EntityType type){
    if(list->firstElement == NULL){
        //There is no Linked Element
        list->firstElement = (EntitiesLinkedElement *) malloc(sizeof(EntitiesLinkedElement));
        list->firstElement->data = sprite;
        list->firstElement->type = type;
        list->firstElement->remove = 0;
        list->firstElement->next = NULL;
        list->firstElement->previous = NULL;
        list->lastElement = list->firstElement;
    } else if(list->lastElement == NULL){
        //There is only one linked Element
        list->lastElement = (EntitiesLinkedElement *) malloc(sizeof(EntitiesLinkedElement));
        list->lastElement->data = sprite;
        list->lastElement->type = type;
        list->lastElement->remove = 0;
        list->lastElement->previous = list->firstElement;
        list->lastElement->next = NULL;
        list->firstElement->next = list->lastElement;
    } else {
        //add Linked Element at last
        EntitiesLinkedElement *element = (EntitiesLinkedElement *) malloc(sizeof(EntitiesLinkedElement));
        element->previous = list->lastElement;
        element->data = sprite;
        element->type = type;
        element->next = NULL;
        element->remove = 0;
        list->lastElement->next = element;
        list->lastElement = element;
    }
}

void removeLinkedElement(DoubleLinkedList *list, EntitiesLinkedElement *linkedElement){
    if(linkedElement->previous != NULL){
        linkedElement->previous->next = linkedElement->next;
    }
    if(linkedElement->next != NULL){
        linkedElement->next->previous = linkedElement->previous;
    } else {
        list->lastElement = linkedElement->previous;
    }
    free(linkedElement);
}
