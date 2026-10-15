#include <stdio.h>
#include <stdlib.h>
#include "macros.h"

#ifndef GRAPH_C
#define GRAPH_C

/** @defgroup Graph Graph
 * @{
 * Functions to control the graph object
 */

/**
 * @brief Struct type for Graph object
 */
typedef struct
{
    struct NodeTag *head[maxGraphNodes]; /**< @brief Node array containing the graph nodes*/
} Graph;

/**
 * @brief Struct type for Node object
 */
typedef struct NodeTag
{
    int dest;        /**< @brief int to represent the destination of the node */
    int visited;     /**< @brief int to represent if the node was visited */
    struct NodeTag *next; /**< @brief Node* for the next Node in the graph*/
} Node;

/**
 * @brief Struct type for Edge object
 */
typedef struct
{
    int src; /**< @brief int to represent the source of the edge */
    int dest; /**< @brief int to represent the destination of the edge */
} Edge;

/**
 * @brief Function to create a graph object
 * @param edges[] Edge array containing the edges in the graph
 * @param n unsigned int for the number of nodes in the graph 
 * @return Returns the created graph object
 */
Graph *createGraph(Edge edges[], unsigned int n)
{
    unsigned i;

    Graph *graph = (Graph *)malloc(sizeof(Graph));

    for (i = 0; i < maxGraphNodes; i++)
    {
        graph->head[i] = NULL;
    }
    for (i = 0; i < n; i++)
    {
        int src = edges[i].src;
        int dest = edges[i].dest;


        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->dest = dest;
        newNode->next = graph->head[src];
        newNode->visited = 0;
        graph->head[src] = newNode;


        newNode = (Node *)malloc(sizeof(Node));
        newNode->dest = src;
        newNode->next = graph->head[dest];
        newNode->visited = 0;
        graph->head[dest] = newNode;
    }
    return graph;
}
/**
 * @brief Function to set a graph edge as visited
 * @param a int for the source node of the edge 
 * @param b int for the destination node of the edge 
 * @param graph Graph* for the graph object 
 */
void visitEdge(Graph *graph, int a, int b)
{
    Node *ptr = graph->head[a];
    while (ptr != NULL)
    {
        if (ptr->dest == b)
        {
            ptr->visited = 1;
            break;
        }
        ptr = ptr->next;
    }
    ptr = graph->head[b];
    while (ptr != NULL)
    {
        if (ptr->dest == a)
        {
            ptr->visited = 1;
            break;
        }
        ptr = ptr->next;
    }
}
/**
 * @brief Function to reset a graph's edges
 * @param graph Graph* for the graph object 
 */
void resetGraph(Graph *graph)
{
    int i;
    for (i = 0; i < maxGraphNodes; i++)
    {
        Node *ptr = graph->head[i];
        while (ptr != NULL)
        {
            ptr->visited = 0;
            ptr = ptr->next;
        }
    }
}
/**
 * @brief Function to check if a graph edge was visited
 * @param a int for the source node of the edge 
 * @param b int for the destination node of the edge 
 * @param graph Graph* for the graph object 
 * @return Returns 1 if the edge was visited
 */
int edgeVisited(Graph *graph, int a, int b)
{
    Node *ptr = graph->head[a];
    while (ptr != NULL)
    {
        if (ptr->dest == b)
        {
            if (!ptr->visited)
            {
                return 1;
            }
        }
        ptr = ptr->next;
    }
    return 0;
}

/**
 * @}
 */
#endif /*GRAPH_C*/
