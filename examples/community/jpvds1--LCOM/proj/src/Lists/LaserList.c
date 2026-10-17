#include "LaserList.h"

struct LaserNode *LaserFirst = NULL; //The first node of the list
struct LaserNode *LaserAdded = NULL; //The last node of the list

void ll_printNode(const struct LaserNode *node) //prints the relevant info of a node
{
    if(node == NULL)
    {
        printf("This node is NULL");
    }
    else
    {
        printf("Current position is %u : %u", node->laser->x, node->laser->y);
    }
}

struct LaserNode *ll_getNewNode(uint16_t x, uint16_t y, int user) //Creates a new node but doesnt add it to the list
{
    struct LaserNode *newNode = NULL;
    newNode = malloc(sizeof(struct LaserNode)); //assign the necessary memory
    if(newNode != NULL) //check if malloc worked
    {
        newNode->laser = malloc(sizeof(struct Laser)); //assign the necessary memory
        if(newNode->laser != NULL) //check if malloc worked
        {
            //set the variables
            newNode->laser->x = x + 39;
            newNode->laser->y = y - 5;
            newNode->laser->user = user;
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

void ll_printList(const struct LaserNode *list) //prints the relevant info of all the nodes starting from node list
{
    printf("Printing List:");
    const struct LaserNode *t;
    t = list;
    if(t == NULL)
    {
        printf("list is empty\n");
    }
    else
    {
        while(t != NULL)
        {
            ll_printNode(t);
            t = t->next;
        }
        printf("\n");
    }
}

void ll_CleanUp(struct LaserNode *list) //cleans the memory of all the nodes starting in node list
{
    struct LaserNode *nextInList;
    while(list != NULL)
    {
        nextInList = list->next;
        printf("Cleaning %u : %u\n", list->laser->x, list->laser->y);
        free(list->laser);
        free(list);
        list = nextInList;
    }
}

void ll_CleanNode(struct LaserNode *cnode) //cleans the memory of node cnode
{
    free(cnode->laser);
    free(cnode);
}

void ll_removeFirst() //removes the first element of the list
{
    if(LaserFirst != NULL)
    {
        if(LaserAdded == LaserFirst) //set added in case they are the same
        {
            LaserAdded = LaserFirst->next;
        }
        struct LaserNode *tmp = LaserFirst;
        LaserFirst = LaserFirst->next;
        ll_CleanNode(tmp);
    }
}

void ll_removeNode(struct LaserNode *rnode) //removes node rnode from the list
{
    struct LaserNode *nextIn = LaserFirst;
    struct LaserNode *toDelete = NULL;

    if(rnode == LaserFirst) //checks if rnode is the first node
    {
        LaserFirst = LaserFirst->next;
        if (LaserFirst == NULL) {
            LaserAdded = NULL;
        }
        ll_CleanNode(rnode);
    }
    else
    {
        while (nextIn != NULL && nextIn->next != rnode) //get to the node before rnode
        {
            nextIn = nextIn->next;
        }

        if (nextIn != NULL)
        {
            toDelete = nextIn->next;
            nextIn->next = rnode->next;

            if (LaserAdded == toDelete) {
                LaserAdded = nextIn;
            }

            ll_CleanNode(toDelete);
        }
        else
        {
            printf("Node not found in list.\n");
        }
    }
}

void ll_addNode(const struct LaserNode *newNode) //adds a node to the end of the list
{
    if(LaserFirst == NULL) //adds a node in case the list is empty
    {
        LaserFirst = ll_getNewNode(newNode->laser->x, newNode->laser->y, newNode->laser->user);
        if(LaserFirst != NULL)
        {
            LaserAdded = LaserFirst;
        }
    }
    else
    {
        struct LaserNode *last = LaserFirst;
        while (last->next != NULL) //get to the last node
        {
            last = last->next;
        }
        last->next = ll_getNewNode(newNode->laser->x, newNode->laser->y, newNode->laser->user);
        if (last->next != NULL)
        {
            LaserAdded = last->next;
        }
    }
}
