#pragma once
#include <stdbool.h>
#include <stdint.h>
#include "bitmap.h"
#include "i8254.h"
#include "timer.h"
#include "kbd.h"
#include "mouse.h"
#include "vdh.h"
#include "rtc.h"

/**
 * @defgroup Main_Menu Main_Menu
 * @{
 *
 * Functions from the Main Menu
*/

// Possible States for the State Machine
typedef enum {MENU = 0,SINGLEP,MULTIP,SCORES,EXIT,SPLASHSCREEN} State;

// Possible Events for the State Machine
typedef enum {NOTH = 0, SPBTN, MPBTN, SCBTN,EXBTN} Event;

// Functions related to the current State of the State Machine
int stMENU();
int stSINGLEP();
int stMULTIP();
int stSCORES();
int stEXIT();

// State Machine
typedef struct {
  State state;
  Event event;
  int xi,yi; // Mouse Coordinates
  Bitmap * mouse; // Mouse Pointer
  int (*actions[5])();
}MainMenuStM;

// Backgroung Interface Images and Graphics
typedef struct {
    Bitmap * title;
    Bitmap * track1;
    Bitmap * track2;
    Bitmap * playBtn;
    Bitmap * playBtn_Hoover;
    Bitmap * multiBtn;
    Bitmap * multiBtn_Hoover;
    Bitmap * scoresBtn;
    Bitmap * scoresBtn_Hoover;
    Bitmap * exitBtn;
    Bitmap * exitBtn_Hoover;
    Bitmap * cloud1;
    Bitmap * cloud2;
    Bitmap * bigtree1;
    Bitmap * bigtree2;
    Bitmap * bigtree3;
    Bitmap * bigtree4;
    Bitmap * bigtree5;
    Bitmap * bigtree6;
    Bitmap * smalltree1;
    Bitmap * smalltree2;
    Bitmap * smalltree3;
    Bitmap * smalltree4;
    Bitmap * smalltree5;
    Bitmap * multitree1;
    Bitmap * multitree2;
    Bitmap * multitree3;
    Bitmap * multitree4;
    Bitmap * bird1;
    Bitmap * bird2;
    Bitmap * gameover;
    Bitmap * n0;
    Bitmap * n1;
    Bitmap * n2;
    Bitmap * n3;
    Bitmap * n4;
    Bitmap * n5;
    Bitmap * n6;
    Bitmap * n7;
    Bitmap * n8;
    Bitmap * n9;
    Bitmap * bar;
    Bitmap * time;
    Bitmap * sun;
    Bitmap * moon;
    int trackvelocity;
    int trackHeight;
    int cloudsvelocity;
    int btrack; // Track asset selector
    int bsmalltree; // Tree asset selector
    int bcloud; // Cloud asset selector
    int bcloudHeight; // Current Cloud height
    int sunX; // Sun X coordinate
    bool day; // True if day, False if Night
    int collisionCount; // Register of the Number of Frame collisions
    int obstacle; // Obstacle Selector
    int collisionLimit; // Frame collisions Limit
    uint32_t irq_timer;
    uint32_t irq_kbd;
    uint32_t irq_mouse;
    uint32_t irq_rtc;
} Interface;

// Trex Assets
typedef struct{
    Bitmap * trex1;
    Bitmap * trex2;
    Bitmap * trex3;
    Bitmap * trex4;
    Bitmap * trex5;
    int hy; // Current Trex height
    bool under; // Trex is lowering 
    bool jump;  // Trex is jumping 
    int corrector; // Jumping factor corrector
} Trex;

// Score Struct for one Play
typedef struct{
  int score;
  uint32_t hour;
  uint32_t min;
  uint32_t sec;
  uint32_t year;
  uint32_t month;
  uint32_t day;
} Score;

typedef struct{
  Score score1;
  Score score2;
  Score score3;
} HiScores;

// Splash Screen Animation
typedef struct{
  int x1;
  int xtrex;
  int x2;
  int xtitle;
  int xtrack;
  int mover;
} SplashScreen;

/**
 * @brief Game Main Loop
 * 
 * @return 0 if OK, 1 if NOT_OK
*/
int mainLoop();

/**
 * @brief Loads all the necessary images and initial data
*/
void LoadBitmaps();

/**
 * @brief Event Handler for Mouse on Main Menu
 *
 * @param p Mouse packet to be parsed
*/
void MenuMouseEventHandler(struct packet p);

/**
 * @brief Event Handler for Keyboard on Main Menu
*/
void MenuKbdEventHandler();

/**
 * @brief Main Menu Graphic Interface
*/
void MenuDrawInterface();

/**
 * @brief getHres Shortener
 *
 * @param n Fraction Numerator
 * 
 * @param d Fraction Denominator
*/
int H(int n, int d);

/**
 * @brief getVres Shortener
 * 
 * @param n Fraction Numerator
 * 
 * @param d Fraction Denominator
*/
int V(int n, int d);
