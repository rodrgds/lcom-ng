#include "graph.c"
#include <lcom/lcf.h>

#ifndef MISSION_C
#define MISSION_C

/** @defgroup  Mission Mission
 * @{
 * Functions to control the mission object
 */

/**
 * @brief Struct type for Planet object
 */
typedef struct
{
    int x;            /**< @brief int to represent the x coordinate of the Planet object */
    int y;            /**< @brief int to represent the y coordinate of the Planet object */
    xpm_map_t sprite; /**< @brief xpm_map_t to represent the sprite of the Planet object */
} Planet;

/**
 * @brief Struct type for Mission object
 */
typedef struct
{
    int nPlanets; /**< @brief int to represent the number of planets in the Mission object */
    int nEdges;   /**< @brief int to represent the number of edges in the Mission object */
    Edge *edges;  /**< @brief Edge* to the first element of the array containing the edges in the Mission object */
    Planet *planets; /**< @brief Planet* to the first element of the array containing the planets in the Mission object */
} Mission;

/** @brief Array containing the planet values for mission 1*/
Planet mission1[] = {{300, 400, snow}, {500, 400, sand}, {400, 200, rock}};
/** @brief Array containing the edge values for mission 1*/
Edge edges1[] = {{0, 1}, {0, 2}, {2, 1}};

/** @brief Array containing the planet values for mission 2*/
Planet mission2[] = {{200, 250, moon}, {300, 500, green}, {500, 500, mercury}, {600, 250, mars}, {400, 100, terran}};
/** @brief Array containing the edge values for mission 2*/
Edge edges2[] = {{0, 2}, {2, 4}, {1, 4}, {3, 1}, {0, 3}};

/** @brief Array containing the planet values for mission 3*/
Planet mission3[] = {{200, 300, ocean}, {600, 300, moon}, {400, 100, mars}, {200, 500, toxic}, {600, 500, green}};
/** @brief Array containing the edge values for mission 3*/
Edge edges3[] = {
    {0, 1},
    {0, 2},
    {0, 3},
    {2, 1},
    {1, 4},
    {3, 4},
};
/** @brief Array containing the planet values for mission 4*/
Planet mission4[] = {{500, 500, moon}, {300, 500, green}, {200, 250, mercury}, {600, 250, mars}, {400, 100, terran}};
/** @brief Array containing the edge values for mission 4*/
Edge edges4[] = {
    {0, 1},
    {0, 3},
    {0, 4},
    {1, 2},
    {1, 4},
    {3, 1},
    {3, 4},
    {4, 2},
};

/**
 * @brief Function to load a mission object
 * @param missionN int for the number of the mission to be loaded 
 * @return Returns the created mission object
 */
Mission *loadMission(int missionN)
{
    Mission *mission = (Mission *)malloc(sizeof(Mission));
    switch (missionN)
    {
    case 1:
        mission->planets = mission1;
        mission->edges = edges1;
        mission->nPlanets = sizeof(mission1) / sizeof(Planet);
        mission->nEdges = sizeof(edges1) / sizeof(Edge);
        break;
    case 2:
        mission->planets = mission2;
        mission->edges = edges2;
        mission->nPlanets = sizeof(mission2) / sizeof(Planet);
        mission->nEdges = sizeof(edges2) / sizeof(Edge);
        break;
    case 3:
        mission->planets = mission3;
        mission->edges = edges3;
        mission->nPlanets = sizeof(mission3) / sizeof(Planet);
        mission->nEdges = sizeof(edges3) / sizeof(Edge);
        break;
    case 4:
        mission->planets = mission4;
        mission->edges = edges4;
        mission->nPlanets = sizeof(mission4) / sizeof(Planet);
        mission->nEdges = sizeof(edges4) / sizeof(Edge);
        break;
    default:
        break;
    }
    return mission;
}

/**
 * @}
 */
#endif /*MISSION_C*/
