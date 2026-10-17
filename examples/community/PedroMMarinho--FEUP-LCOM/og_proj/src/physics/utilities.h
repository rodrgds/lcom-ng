#pragma once
#include <lcom/lcf.h>
#include "../model/vector.h"
#include "../model/table.h"
#include "../model/ball/ball.h"
#include "../model/cushion.h"
#include "../model/pocket.h"
#include "../model/event.h"

/**
 * @brief Struct that represents the coefficients of a quartic equation.
*/
typedef struct {
  double a, b, c, d, e;
  bool valid;
} QuarticCoeff;
// DEBUGGING

/**
 * @brief Prints the details of an event.
 *
 * This function prints the type and time of the event.
 *
 * @param event Pointer to the Event structure.
 */

void printEvent(Event* event);

/**
 * @brief Prints a floating-point number.
 *
 * This function prints a floating-point number.
 *
 * @param num The floating-point number to print.
 */
void printFloat(double num);

/**
 * @brief Prints the coefficients of a quartic equation.
 *
 * This function prints the coefficients (a, b, c, d, e) of a quartic equation.
 *
 * @param q The QuarticCoeff structure containing the coefficients.
 */
void printCoef(QuarticCoeff q);

/**
 * @brief Prints the components of a vector.
 *
 * This function prints the x and y components of a vector.
 *
 * @param vec The vector to print.
 */
void printVector(vector_t vec);
// Maths

/**
 * @brief Calculates the unit vector from one point to another.
 *
 * This function calculates the unit vector pointing from one point (start) to another point (end).
 *
 * @param start The starting point.
 * @param end The ending point.
 * @return The unit vector from start to end.
 */
vector_t unitVector(vector_t a, vector_t b);

/**
 * @brief Calculates the angle of a vector.
 *
 * This function calculates the angle of a vector in radians.
 *
 * @param vec1 The vector for which to calculate the angle.
 * @return The angle of the vector in radians.
 */
double angle(vector_t vec1);

/**
 * @brief Calculates the angle between two vectors.
 *
 * This function calculates the angle between two vectors in radians.
 *
 * @param vec1 The first vector.
 * @param vec2 The second vector.
 * @return The angle between the vectors in radians.
 */
double angleBetween(vector_t vec1, vector_t vec2);

/**
 * @brief Finds the point on a line closest to a given point.
 *
 * This function calculates the point on a line defined by two points (p1 and p2) that is closest to a given point (p).
 *
 * @param p1 The first point defining the line.
 * @param p2 The second point defining the line.
 * @param p The point for which to find the closest point on the line.
 * @return The point on the line closest to the given point.
 */
vector_t linePointClosestTo(vector_t p1, vector_t p2, vector_t p);

/**
 * @brief Normalizes a vector.
 *
 * This function normalizes a vector, making its magnitude equal to 1.
 *
 * @param vector The vector to normalize.
 * @return The normalized vector.
 */

vector_t normalizeVector(vector_t vector);

/**
 * @brief Calculates the dot product of two vectors.
 *
 * This function calculates the dot product of two vectors.
 *
 * @param a The first vector.
 * @param b The second vector.
 * @return The dot product of the two vectors.
 */
double dotProduct(vector_t a, vector_t b);

/**
 * @brief Calculates the magnitude of a vector.
 *
 * This function calculates the magnitude (length) of a vector.
 *
 * @param vector The vector for which to calculate the magnitude.
 * @return The magnitude of the vector.
 */
double magnitudeOf(vector_t vector);

/**
 * @brief Rotates a 2D vector by a given angle.
 *
 * This function rotates a 2D vector by a given angle in radians.
 *
 * @param vec The vector to rotate.
 * @param ang The angle of rotation in radians.
 * @return The rotated vector.
 */
vector_t rotate2d(vector_t vec, double ang);

/**
 * @brief Solves a quartic equation.
 *
 * This function solves a quartic equation of the form x^4 + bx^3 + cx^2 + dx + e = 0.
 *
 * @param a The coefficient of x^3.
 * @param b The coefficient of x^2.
 * @param c The coefficient of x.
 * @param d The constant term.
 * @param e The constant term.
 * @param ans Pointer to an array to store the solutions.
 * @return The number of solutions.
 */
int quartic(double a, double b,double c,double d,double e,double* ans);

/**
 * @brief Solves a cubic equation.
 *
 * This function solves a cubic equation of the form x^3 + bx^2 + cx + d = 0.
 *
 * @param b The coefficient of x^2.
 * @param c The coefficient of x.
 * @param d The constant term.
 * @return The solution of the cubic equation.
 */
double cubic(double b,double c,double d);
/**
*
*@brief Finds the smallest positive root of a quartic equation.
*This function finds the smallest positive root of a quartic equation.
*@param n The number of coefficients.
*@param coeficients An array of QuarticCoeff structures.
*@param result Pointer to store the smallest positive root.
*@return The index of the smallest positive root in the coefficients array.
*/
int findSmallerCoeficient(size_t n, QuarticCoeff* coeficients, double* result);

/**
 * @brief Calculates the unit vector from the ball's
 * @param ball Pointer to the Ball structure.
*@param ang The angle of the ball's velocity vector.
*@return The unit vector from the ball's position to the point of contact.
*/

vector_t get_uVec(Ball* ball, double ang);

// Physics calculations utilities
/**
 * @brief Calculates the relative velocity of a ball.
 *
 * This function calculates the relative velocity of a ball at a given state.
 *
 * @param ball Pointer to the Ball structure.
 * @return The relative velocity of the ball.
 */
vector_t relativeVelocity(Ball* ball);

/**
 * @brief Calculates the time for sliding motion to stop for a ball.
 *
 * This function calculates the time for sliding motion to stop for a ball.
 *
 * @param ball Pointer to the Ball structure.
 * @param u The coefficient of friction.
 * @param g The gravitational acceleration.
 * @return The time for sliding motion to stop.
 */
double getSlideTime(Ball* ball, double u, double g);

/**
 * @brief Calculates the time for rolling motion to stop for a ball.
 *
 * This function calculates the time for rolling motion to stop for a ball.
 *
 * @param ball Pointer to the Ball structure.
 * @param u The coefficient of friction.
 * @param g The gravitational acceleration.
 * @return The time for rolling motion to stop.
 */
double getRollTime(Ball* ball, double u, double g);

/**
 * @brief Calculates the time for spinning motion to stop for a ball.
 *
 * This function calculates the time for spinning motion to stop for a ball.
 *
 * @param ball Pointer to the Ball structure.
 * @param u The coefficient of friction.
 * @param g The gravitational acceleration.
 * @return The time for spinning motion to stop.
 */
double getSpinTime(Ball* ball, double u, double g);

/**
 * @brief Calculates the time for a linear cushion collision with a ball.
 *
 * This function calculates the time for a linear cushion collision with a ball.
 *
 * @param table Pointer to the Table structure.
 * @param ball Pointer to the Ball structure.
 * @param cushion Pointer to the LinearCushion structure.
 * @return The time for the collision.
 */

double getBallLinearCushionCollisionTime(Table *table, Ball *ball, LinearCushion* cushion);

/**
*@brief Calculates the coefficients of a ball-ball collision.
*This function calculates the coefficients of a quartic equation representing a *ball-ball collision.
*@param ball1 Pointer to the first Ball structure.
*@param ball2 Pointer to the second Ball structure.
*@param uRolling The coefficient of rolling friction.
*@param uSliding The coefficient of sliding friction.
*@param g The gravitational acceleration.
*@return The coefficients of the collision equation.
*/

QuarticCoeff getBallBallCollisionCoeff(Ball* ball1, Ball* ball2, double uRolling, double uSlidding, double g);

/**
*
*@brief Calculates the coefficients of a ball-pocket collision.
*This function calculates the coefficients of a quartic equation representing a *ball-pocket collision.
*@param ball Pointer to the Ball structure.
*@param pocket Pointer to the Pocket structure.
*@param uRolling The coefficient of rolling friction.
*@param uSliding The coefficient of sliding friction.
*@param g The gravitational acceleration.
*@return The coefficients of the collision equation.
*/

QuarticCoeff getBallPocketCollisionCoeff(Ball *ball, Pocket* pocket, double uRolling, double uSlidding, double g);

/**
*
*@brief Calculates the coefficients of a ball-circular cushion collision.
*This function calculates the coefficients of a quartic equation representing a *ball-circular cushion collision.
*@param ball Pointer to the Ball structure.
*@param cushion Pointer to the CircularCushion structure.
*@param uRolling The coefficient of rolling friction.
*@param uSliding The coefficient of sliding friction.
*@param g The gravitational acceleration.
*@return The coefficients of the collision equation.
*/

QuarticCoeff getBallCircularCushionCollisionCoeff(Ball *ball, CircularCushion* cushion, double uRolling, double uSlidding, double g);
