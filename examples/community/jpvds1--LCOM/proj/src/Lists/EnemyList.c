#include "EnemyList.h"

struct EnemyNode *EnemyFirst = NULL;
struct EnemyNode *EnemyAdded = NULL;
int EnemySize = 0;

void el_printNode(const struct EnemyNode *node)
{
    if(node == NULL)
    {
        printf("This node is NULL");
    }
    else
    {
        printf("Current position is %u : %u : %d", node->enemy->x, node->enemy->x, node->enemy->direction);
    }
}

struct EnemyNode *el_getNewNode(uint16_t x, uint16_t y, uint16_t min_x, uint16_t max_x, uint16_t min_y, uint16_t max_y)
{
    struct EnemyNode *newNode = NULL;
    newNode = malloc(sizeof(struct EnemyNode));
    if(newNode != NULL)
    {
        newNode->enemy = malloc(sizeof(struct Enemy));
        if(newNode->enemy != NULL)
        {
            newNode->enemy->x = x;
            newNode->enemy->y = y;
            newNode->enemy->min_x = min_x;
            newNode->enemy->max_x = max_x;
            newNode->enemy->min_y = min_y;
            newNode->enemy->max_y = max_y;
            newNode->enemy->direction = 0;
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

void el_printList(const struct EnemyNode *list)
{
    printf("Printing List:");
    const struct EnemyNode *t;
    t = list;
    if(t == NULL)
    {
        printf("list is empty\n");
    }
    else
    {
        while(t != NULL)
        {
            el_printNode(t);
            t = t->next;
        }
        printf("\n");
    }
}

void el_CleanUp(struct EnemyNode *list)
{
    struct EnemyNode *nextInList;
    while(list != NULL)
    {
        nextInList = list->next;
        printf("Cleaning %u : %u\n", list->enemy->x, list->enemy->y);
        free(list->enemy);
        free(list);
        list = nextInList;
    }
    EnemySize = 0;
}

void el_CleanNode(struct EnemyNode *cnode)
{
    free(cnode->enemy);
    free(cnode);
}

void el_removeFirst()
{
    if(EnemyFirst != NULL)
    {
        if(EnemyAdded == EnemyFirst)
        {
            EnemyAdded = EnemyFirst->next;
        }
        struct EnemyNode *tmp = EnemyFirst;
        EnemyFirst = EnemyFirst->next;
        el_CleanNode(tmp);
    }
    EnemySize--;
}

void el_removeNode(struct EnemyNode *rnode)
{
    struct EnemyNode *nextIn = EnemyFirst;
    struct EnemyNode *toDelete = NULL;

    if(rnode == EnemyFirst)
    {
        EnemyFirst = EnemyFirst->next;
        if (EnemyFirst == NULL) {
            EnemyAdded = NULL;
        }
        el_CleanNode(rnode);
        EnemySize--;
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

            if (EnemyAdded == toDelete) {
                EnemyAdded = nextIn;
            }

            el_CleanNode(toDelete);
            EnemySize--;
        }
        else
        {
            printf("Node not found in list.\n");
        }
    }
}

void el_addNode(const struct EnemyNode *newNode)
{
    if(EnemyFirst == NULL)
    {
        EnemyFirst = el_getNewNode(newNode->enemy->x, newNode->enemy->y, newNode->enemy->min_x, newNode->enemy->max_x, newNode->enemy->min_y, newNode->enemy->max_y);
        if(EnemyFirst != NULL)
        {
            EnemyAdded = EnemyFirst;
        }
    }
    else
    {
        struct EnemyNode *last = EnemyFirst;
        while (last->next != NULL)
        {
            last = last->next;
        }
        last->next = el_getNewNode(newNode->enemy->x, newNode->enemy->y, newNode->enemy->min_x, newNode->enemy->max_x, newNode->enemy->min_y, newNode->enemy->max_y);
        if (last->next != NULL)
        {
            EnemyAdded = last->next;
        }
    }
    EnemySize++;
}

