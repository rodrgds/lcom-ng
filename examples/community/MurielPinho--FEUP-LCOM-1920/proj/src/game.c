#include <lcom/lcf.h>
#include "../assets/xpm.h"
#include "sprite.h"
#include "graphics.h"
#include "game.h"
#include "macros.h"
#include "menu.h"
#include "mouse.h"
#include "ps2.h"
#include "keyboard.h"
#include "mission.c"

Sprite *planets[maxGraphNodes];
Sprite *g_bg = NULL;
Sprite *g_selector = NULL;
Sprite *g_arwing = NULL;
Sprite *g_levelDone = NULL;
Sprite *g_Complete = NULL;
Graph *graph = NULL;
Mission *mission = NULL;
Game *game = NULL;
int nMission = 1;

Game *createGame()
{
    Game *game = (Game *)malloc(sizeof(Game));
    game->gState = Play;
    game->iterator = 0;
    game->selectedPlanet = 0;
    game->currentPlanet = 0;
    game->nVisitedEdges = 0;
    return game;
}

int gameLoop()
{

    int ipc_status, rec;
    message msg;
    initGame();
    scannedKey = 0;
    while (scannedKey != ESC)
    {
        if ((rec = driver_receive(ANY, &msg, &ipc_status)) != 0)
        {
            printf("driver_receive failed with: %d", rec);
            continue;
        }
        if (is_ipc_notify(ipc_status))
        {
            switch (_ENDPOINT_P(msg.m_source))
            {
            case HARDWARE:
                if (msg.m_notify.interrupts & kbd_bit)
                {
                    kbc_ih();
                    if (gameInput())
                    {
                        return 1;
                    }
                }
                if (msg.m_notify.interrupts & mouseBit)
                {
                    mouse_ih();
                }
                if (msg.m_notify.interrupts & timerBit)
                {
                    timer_int_handler();
                    checkGameState();
                    updateScreen();
                }
                break;
            default:
                break;
            }
        }
    }

    return 0;
}
int initGame()
{
    game = createGame();
    mission = loadMission(nMission);
    graph = createGraph(mission->edges, mission->nEdges);
    for (int i = 0; i < mission->nPlanets; i++)
    {
        planets[i] = create_sprite(mission->planets[i].sprite, mission->planets[i].x, mission->planets[i].y);
    }
    g_bg = create_sprite(background, 800, 300);
    g_selector = create_sprite(selector, planets[game->selectedPlanet]->x, planets[game->selectedPlanet]->y - selectorOffset);
    g_arwing = create_sprite(arwing, planets[game->currentPlanet]->x, planets[game->currentPlanet]->y);
    g_levelDone = create_sprite(levelDone, 400, 300);
    g_Complete = create_sprite(endingScreen, 400, 300);
    set_sprite_speed(g_bg, 800, 300, 0, 300, -1);
    return 0;
}
void updateScreen()
{
    if (interruptCounter % interruptPerFrame == 0)
    {
        clearScreen();
        if (g_bg->x != 0)
        {
            update_sprite(g_bg, 0, 300);
        }
        else
        {
            g_bg->x = 800;
        }
        if (game->gState == Complete)
        {
            drawComplete();
        }
        else if (game->gState == End)
        {
            drawEnd();
        }
        else
        {
            drawMission();
        }
        flipBuffer();
    }
}
void checkGameState()
{
    if (game->gState == Play)
    {
        if (game->nVisitedEdges == mission->nEdges)
        {
            if (nMission == lastMission)
            {
                game->gState = End;
            }
            else
            {
                game->gState = Complete;
            }
        }
    }
}
void moveSelector()
{
    kbd_game_input(&game->selectedPlanet, mission->nPlanets);
    g_selector->x = planets[game->selectedPlanet]->x;
    g_selector->y = planets[game->selectedPlanet]->y - selectorOffset;
}
void moveArwing()
{
    if (edgeVisited(graph, game->currentPlanet, game->selectedPlanet))
    {
        visitEdge(graph, game->currentPlanet, game->selectedPlanet);
        game->currentPlanet = game->selectedPlanet;
        g_arwing->x = planets[game->selectedPlanet]->x;
        g_arwing->y = planets[game->selectedPlanet]->y;
        game->nVisitedEdges++;
    }
}
int gameInput()
{
    if (scannedKey == ENTER)
    {
        if (game->gState == Complete)
        {
            missionComplete();
            return 0;
        }
        else if (game->gState == End)
        {
            nMission = 1;
            return 1;
        }
    }

    if (game->gState == Play)
    {
        if (scannedKey == M)
        {
            return 1;
        }
        else if (scannedKey == R)
        {
            resetMission();
        }
        else if (scannedKey == ENTER)
        {
            if (game->currentPlanet != game->selectedPlanet)
            {
                moveArwing();
            }
        }
        else if (scannedKey == ARR_LEFT || scannedKey == ARR_RIGHT)
        {
            moveSelector();
        }
    }
    return 0;
}
void drawMission()
{
    draw_sprite(g_bg);
    drawEdges();
    drawPlanets();
    draw_sprite(g_selector);
    draw_sprite(g_arwing);
}
void drawComplete()
{
    draw_sprite(g_bg);
    draw_sprite(g_levelDone);
}
void drawEnd()
{
    draw_sprite(g_bg);
    draw_sprite(g_Complete);
}
void drawEdges()
{
    uint32_t color;
    for (; game->iterator < mission->nEdges; game->iterator++)
    {
        int p1, p2;
        p1 = mission->edges[game->iterator].src;
        p2 = mission->edges[game->iterator].dest;
        if (!edgeVisited(graph, p1, p2))
        {
            color = visitedEdgeColor;
        }
        else
        {
            color = unvisitedEdgeColor;
        }
        drawLine(planets[p1]->x, planets[p1]->y, planets[p2]->x, planets[p2]->y, color);
    }
    game->iterator = 0;
}
void drawPlanets()
{
    for (; game->iterator < mission->nPlanets; game->iterator++)
    {
        draw_sprite(planets[game->iterator]);
    }
    game->iterator = 0;
}

void missionComplete()
{
    nMission++;
    game->gState = Play;
    mission = loadMission(nMission);
    initGame();
}
void resetMission()
{
    game->selectedPlanet = 0;
    game->currentPlanet = 0;
    game->nVisitedEdges = 0;
    g_selector->x = planets[game->selectedPlanet]->x;
    g_selector->y = planets[game->selectedPlanet]->y - selectorOffset;
    g_arwing->x = planets[game->selectedPlanet]->x;
    g_arwing->y = planets[game->selectedPlanet]->y;
    resetGraph(graph);
}
int subscribeInterrupts()
{
    mouse_enable_data_report();
    timer_subscribe_int(&timerBit);
    kbd_subscribe_int(&kbd_bit);
    mouse_subscribe_int(&mouseBit);

    return 0;
}
int unsubscribeInterrupts()
{
    timer_unsubscribe_int();
    kbd_unsubscribe_int();
    mouse_unsubscribe_int();
    mouse_reset();
    return 0;
}
