#include "AnimationList.h"

struct AnimationNode *AnimationFirst = NULL;
struct AnimationNode *AnimationAdded = NULL;
int AnimationSize = 0;

void al_printNode(const struct AnimationNode *node)
{
    if(node == NULL)
    {
        printf("This node is NULL");
    }
    else
    {
        printf("Current position is %u : %u : %u", node->animation->x, node->animation->x, node->animation->curr_xpm);
    }
}

struct AnimationNode *al_getNewNode(uint16_t x, uint16_t y)
{
    struct AnimationNode *newNode = NULL;
    newNode = malloc(sizeof(struct AnimationNode));
    if(newNode != NULL)
    {
        newNode->animation = malloc(sizeof(struct Animation));
        if(newNode->animation != NULL)
        {
            newNode->animation->x = x;
            newNode->animation->y = y;
            newNode->animation->curr_xpm = 0;
            newNode->animation->time_to_change = 5;
            newNode->next = NULL;
            //printf("New Node at %p\n", newNode);
        }
        else
        {
            free(newNode);
            newNode = NULL;
            printf("Allocation failure\n");
        }
    }
    else
    {
        printf("Allocation Failiure");
    }
    return newNode;
}

void al_printList(const struct AnimationNode *list)
{
    printf("Printing List:");
    const struct AnimationNode *t;
    t = list;
    if(t == NULL)
    {
        printf("list is empty\n");
    }
    else
    {
        while(t != NULL)
        {
            al_printNode(t);
            t = t->next;
        }
        printf("\n");
    }
}

void al_CleanUp(struct AnimationNode *list)
{
    struct AnimationNode *nextInList;
    while(list != NULL)
    {
        nextInList = list->next;
        printf("Cleaning %u : %u\n", list->animation->x, list->animation->y);
        free(list->animation);
        free(list);
        list = nextInList;
    }
    AnimationSize = 0;
}

void al_CleanNode(struct AnimationNode *cnode)
{
    free(cnode->animation);
    free(cnode);
}

void al_removeFirst()
{
    if(AnimationFirst != NULL)
    {
        if(AnimationAdded == AnimationFirst)
        {
            AnimationAdded = AnimationFirst->next;
        }
        struct AnimationNode *tmp = AnimationFirst;
        AnimationFirst = AnimationFirst->next;
        al_CleanNode(tmp);
    }
    AnimationSize--;
}

void al_removeNode(struct AnimationNode *rnode)
{
    struct AnimationNode *nextIn = AnimationFirst;
    struct AnimationNode *toDelete = NULL;

    if(rnode == AnimationFirst)
    {
        AnimationFirst = AnimationFirst->next;
        if (AnimationFirst == NULL) {
            AnimationAdded = NULL;
        }
        al_CleanNode(rnode);
        AnimationSize--;
    }
    else
    {
        while (nextIn != NULL && nextIn->next != rnode)
        {
            nextIn = nextIn->next;
        }

        if (nextIn != NULL)
        {
            toDelete = nextIn->next;
            nextIn->next = rnode->next;

            if (AnimationAdded == toDelete) {
                AnimationAdded = nextIn;
            }

            al_CleanNode(toDelete);
            AnimationSize--;
        }
        else
        {
            printf("Node not found in list.\n");
        }
    }
}

void al_addNode(const struct AnimationNode *newNode)
{
    if(AnimationFirst == NULL)
    {
        AnimationFirst = al_getNewNode(newNode->animation->x, newNode->animation->y);
        if(AnimationFirst != NULL)
        {
            AnimationAdded = AnimationFirst;
        }
    }
    else
    {
        struct AnimationNode *last = AnimationFirst;
        while (last->next != NULL)
        {
            last = last->next;
        }
        last->next = al_getNewNode(newNode->animation->x, newNode->animation->y);
        if (last->next != NULL)
        {
            AnimationAdded = last->next;
        }
    }
    AnimationSize++;
}

