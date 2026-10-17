#include <lcom/lcf.h>
#include "frog.h"
#include "log.h"
#include "turtle.h"
#include "vehicle.h"
#include "draws.h"


/**
 * @brief Enum to define the types of entities
 * 
 */
typedef enum {FROG = 1, LOG = 2, TURTLE = 3, VEHICLE = 4} Type;

/**
 * @brief Enum to define the four 2d directions
 * 
 */
typedef enum {UP = 1, LEFT = 2, RIGHT = 3, DOWN = 4} Direction;

/**
 * @brief Struct entity that has Structs of entities it will have as well as the number of those entities and the type
 * 
 */
struct Entity
{
  Frog frog;
  Log logs[6];
  Turtle turtles[2];
  Vehicle vehicles[6];
  int logNr;
  int turtleNr;
  int vehicleNr;
  bool UnderWaterTurtles;
  Type type;
  
};

/**
 * @brief Initiates the entity given the type
 * 
 * @param ent entity
 * @param type type of entity
 */
void initEntity(struct Entity * ent, Type type);

/**
 * @brief Receives an entity and draws it in its position
 * 
 */
int (drawEntity(struct Entity ent));

/**
 * @brief Moves the frog in the given direction, will return false if the move was not possible to be made
 * 
 */
bool (moveFrog(struct Entity * ent,Direction dir));

/**
 * @brief Creates the array of struct Log by generating random positions for them
 * 
 */
void (createLogs(struct Entity * ent));

/**
 * @brief Updates the logs position based on their speed
 * 
 */
void (moveLogs(struct Entity * ent));

/**
 * @brief Creates the array of struct Turtle by generating random positions for them
 * 
 */
void (createTurtle(struct Entity * ent));

/**
 * @brief Draws frog in its position and in the correct direction
 * 
 * @param ent Entity frog
 * @param direction Direction in which to draw the frog (up,down,left,right)
 */
void drawFrog(struct Entity ent, Direction direction);

/**
 * @brief Creates the array of struct Vehicle by generating random positions for them
 * 
 * @param ent 
 */
void createVehicle(struct Entity * ent);

/**
 * @brief Switches the turtles bool variable day to true or false depending on whether if its day or night
 * 
 */
void (switchTurtlesDay(struct Entity * ent,bool day));

/**
 * @brief Switches the turtles bool variable underwater to true or false depending on the current value of said bool (if true makes it false and if false makes it true)
 * 
 */
void (switchTurtlesUnderwater(struct Entity * ent));

/**
 * @brief Updates the turtles position based on their speed
 * 
 */
void (moveTurtles(struct Entity * ent));

/**
 * @brief Updates the vehicles position based on their speed
 * 
 */
void (moveVehicles(struct Entity * ent));

/**
 * @brief Colision function to determine whether or not the frog is on top of a log, returns true if it is and changes the int of the log index so that later we know in which log the frog is so that we can make the frog move automatically with the log based on its speed since different logs may have different speeds
 * 
 * @param entFrog Entity Frog
 * @param entLogs Entity Log
 * @param logIndex index of the log in the log array
 * @return true 
 * @return false 
 */
bool frogOnLog(struct Entity entFrog, struct Entity entLogs,int * logIndex);

/**
 * @brief Colision function to determine whether or not the frog is on top of a turtle, returns true if it is and changes the int of the turtle index 
 * @param entFrog Entity Frog
 * @param entTurtles Entity vehicle
 * @param turtleIndex index of the turtle in the turtle array
 * @return true 
 * @return false 
 */
bool frogOnTurtle(struct Entity entFrog, struct Entity entTurtles,int * turtleIndex);

/**
 * @brief Colision function to know if a vehicle (car or truck) has hit the frog, returns true if it has
 * 
 * @param entFrog Entity frog
 * @param entVehicle Entity vehicle
 * @return true 
 * @return false 
 */
bool frogOnVehicle(struct Entity entFrog, struct Entity entVehicle);
