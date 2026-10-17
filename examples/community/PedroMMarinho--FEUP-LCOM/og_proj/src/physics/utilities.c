
#include "utilities.h"
#include "evolve.h"
#include "math.h"
#include "physicsMacros.h"
#include "quartic.h"




void printEvent(Event* event){

  printf("Type: %d",event->type);
  printf("Time: ");
  printFloat(event->time);
  printf("\n");
}

void printFloat(double num){
  char a[90];
  sprintf(a, "%f", num);
  printf("%s\n", a);
}

void printCoef(QuarticCoeff q){

  char a[30];
  sprintf(a, "%f", q.a);
  printf("a - %s\n", a);
  sprintf(a, "%f", q.b);
  printf("b - %s\n", a);
  sprintf(a, "%f", q.c);
  printf("c - %s\n", a);
  sprintf(a, "%f", q.d);
  printf("d - %s\n", a);
  sprintf(a, "%f", q.e);
  printf("e - %s\n", a);

}

void printVector(vector_t vec){

  char a[30];

  sprintf(a, "%f", vec.x);
  printf("x: %s\n", a);
  sprintf(a, "%f", vec.y);
  printf("y: %s\n", a);
}

double angle(vector_t vec1) {

  double ang = atan2(vec1.y, vec1.x);

  return ang < 0 ? 2 * M_PI + ang : ang;
}

double angleBetween(vector_t vec1, vector_t vec2){
  double ang = atan2(vec1.y, vec1.x) - atan2(vec2.y, vec2.x);
  return ang < 0 ? 2 * M_PI + ang : ang;
}

vector_t linePointClosestTo(vector_t p1, vector_t p2, vector_t p){
  vector_t diff = {p2.x-p1.x, p2.y-p1.y};
  vector_t connection = {p1.x - p.x, p1.y-p.y};

  double t = -dotProduct(connection, diff) / dotProduct(diff, diff);

  vector_t result = {p1.x + diff.x * t, p1.y + diff.y * t};
  return result;
}

vector_t rotate2d(vector_t vec, double ang) {

  double cosseno = cos(ang);
  double seno = sin(ang);

  vector_t rotated = {cosseno * vec.x - seno * vec.y, seno * vec.x + cosseno * vec.y};
  return rotated;
}

vector_t unitVector(vector_t start, vector_t end) {
  vector_t vector;
  vector.x = end.x - start.x;
  vector.y = end.y - start.y;
  return normalizeVector(vector);
}

vector_t normalizeVector(vector_t vector) {
  double magnitude = magnitudeOf(vector);
  vector.x /= magnitude;
  vector.y /= magnitude;
  return vector;
}

double dotProduct(vector_t a, vector_t b) {
  return a.x * b.x + a.y * b.y;
}

double magnitudeOf(vector_t vector) {
  return sqrt(vector.x * vector.x + vector.y * vector.y);
}

// Function to solve a cubic equation using the Cardano's formula
/**
 * @brief Solves a quadratic equation.
 *
 * This function solves a quadratic equation of the form ax^2 + bx + c = 0.
 *
 * @param a The coefficient of x^2.
 * @param b The coefficient of x.
 * @param c The constant term.
 * @param ans Pointer to an array to store the solutions.
 * @return The number of solutions (0, 1, or 2).
 */

int solveQuadratic(double a, double b, double c, double *ans) {

  if (a == 0) {
    double u = -c / b;
    *ans = u;
    return 1;
  }

  double d = b * b - (4 * a * c);
  if (d < 0)
    return 0;
  if (d == 0) {
    *ans = -b / (2 * a);
    return 1;
  }

  double sqrtD = sqrt(d);
  ans[0] = (-b + sqrtD) / (2 * a);
  ans[1] = (-b - sqrtD) / (2 * a);
  return 2;
}

double cubic(double b, double c, double d) {
  double p = c - b * b / 3.0;
  double q = 2.0 * b * b * b / 27.0 - b * c / 3.0 + d;

  if (p == 0.0)
    return pow(q, 1.0 / 3.0);
  if (q == 0.0)
    return 0.0;

  double t = sqrt(fabs(p) / 3.0);
  double g = 1.5 * q / (p * t);
  if (p > 0.0)
    return -2.0 * t * sinh(asinh(g) / 3.0) - b / 3.0;

  if (4.0 * p * p * p + 27.0 * q * q < 0.0)
    return 2.0 * t * cos(acos(g) / 3.0) - b / 3.0;

  if (q > 0.0)
    return -2.0 * t * cosh(acosh(-g) / 3.0) - b / 3.0;

  return 2.0 * t * cosh(acosh(g) / 3.0) - b / 3.0;
}

int quartic(double a, double b, double c, double d, double e, double *ans) {



  b = b / a;
  c = c / a;
  d = d / a;
  e = e / a;

  double p = c - 0.375 * b * b;
  double q = 0.125 * b * b * b - 0.5 * b * c + d;
  double m = cubic(p, 0.25 * p * p + 0.01171875 * b * b * b * b - e + 0.25 * b * d - 0.0625 * b * b * c, -0.125 * q * q);
  if (q == 0.0) {
    if (m < 0.0){
      printf("deu errado\n");
      return 0;
    }
    int nroots = 0;
    double sqrt_2m = sqrt(2.0 * m);
    if (-m - p > 0.0) {
      double delta = sqrt(2.0 * (-m - p));
      ans[nroots++] = -0.25 * b + 0.5 * (sqrt_2m - delta);
      ans[nroots++] = -0.25 * b - 0.5 * (sqrt_2m - delta);
      ans[nroots++] = -0.25 * b + 0.5 * (sqrt_2m + delta);
      ans[nroots++] = -0.25 * b - 0.5 * (sqrt_2m + delta);
    }

    if (-m - p == 0.0) {
      ans[nroots++] = -0.25 * b - 0.5 * sqrt_2m;
      ans[nroots++] = -0.25 * b + 0.5 * sqrt_2m;
    }

    return nroots;
  }

  if (m < 0.0)
    return 0;
  double sqrt_2m = sqrt(2.0 * m);
  int nroots = 0;
  if (-m - p + q / sqrt_2m >= 0.0) {
    double delta = sqrt(2.0 * (-m - p + q / sqrt_2m));
    ans[nroots++] = 0.5 * (-sqrt_2m + delta) - 0.25 * b;
    ans[nroots++] = 0.5 * (-sqrt_2m - delta) - 0.25 * b;
  }

  if (-m - p - q / sqrt_2m >= 0.0) {
    double delta = sqrt(2.0 * (-m - p - q / sqrt_2m));
    ans[nroots++] = 0.5 * (sqrt_2m + delta) - 0.25 * b;
    ans[nroots++] = 0.5 * (sqrt_2m - delta) - 0.25 * b;
  }

  return nroots;
}

// Actually physics calculations

vector_t relativeVelocity(Ball *ball) {

  vector_t relVelocity = {-ball->ang_velocity.y * ball->radius + ball->velocity.x, ball->ang_velocity.x * ball->radius + ball->velocity.y};

  return relVelocity;
}

double getSpinTime(Ball *ball, double u, double g) {

  if (u == 0)
    return INFINITY;

  return abs(ball->ang_velocity.z) * 2 * ball->radius / (5 * u * g);
}

double getRollTime(Ball *ball, double u, double g) {

  if (u == 0)
    return INFINITY;
  return magnitudeOf(ball->velocity) / (u * g);
}

double getSlideTime(Ball *ball, double u, double g) {
  if (u == 0)
    return INFINITY;

  return 2 * magnitudeOf(relativeVelocity(ball)) / (7 * u * g);
}

double getBallLinearCushionCollisionTime(Table *table, Ball *ball, LinearCushion* cushion) {

 
  if (ballNotMoving(ball))
    return INFINITY;

  double ang = angle(ball->velocity);
  double v = magnitudeOf(ball->velocity);

  double lx = cushion->lx;
  double ly = cushion->ly;
  double l0 = cushion->l0;

  vector_t u = get_uVec(ball, ang);

  double K = -0.5 * table->gravityAcceleration * (ball->state == SLIDING ? table->slidingFriction : table->rollingFriction);
  double cosseno = cos(ang);
  double seno = sin(ang);

  vector_t a = {K * (u.x * cosseno - u.y * seno), K * (u.x * seno + u.y * cosseno)};
  vector_t b = {v * cosseno, v * seno};
  vector_t c = {ball->position.x, ball->position.y};

  double A = lx * a.x + ly * a.y;
  double B = lx * b.x + ly * b.y;

  // TODO: UNDERSTAND WHICH SIDE COLIDE
  double solutions[4];
  int nSolutions;

  double C = l0 + lx * c.x + ly * c.y + ball->radius * sqrt(lx * lx + ly * ly);
  nSolutions = solveQuadratic(A, B, C, solutions);
  C = l0 + lx * c.x + ly * c.y - ball->radius * sqrt(lx * lx + ly * ly);
  nSolutions += solveQuadratic(A, B, C, solutions + nSolutions);

  double minTime = INFINITY;
  for (int i = 0; i < nSolutions; i++) {

    double root = solutions[i];
    if (root < EPS)
      continue;

    // TODO: IMPROVE THIS PART COPY LIKE THIS IS LAME. CREATE FUNCTION FOR THAT ??
    Ball ballCpy;
    ballCpy.position = ball->position;
    ballCpy.velocity = ball->velocity;
    ballCpy.ang_velocity = ball->ang_velocity;
    ballCpy.state = ball->state;
    ballCpy.transition = NULL;
    
   
    evolveBallMotion(table, &ballCpy, root);
    
    // Check if colision happens in the actuall cushion not in the line that defines it
    vector_t cushionLineVec = {cushion->p2.x - cushion->p1.x, cushion->p2.y - cushion->p1.y};
    vector_t ballCornerVec = {cushion->p1.x - ballCpy.position.x, cushion->p1.y - ballCpy.position.y};

    double score = -dotProduct(ballCornerVec, cushionLineVec) / dotProduct(cushionLineVec, cushionLineVec);
    if (score < 0 || score > 1)
      continue;
    if (root < minTime)
      minTime = root;
  }


  return minTime;
}

vector_t get_uVec(Ball *ball, double ang) {
  vector_t defaultReturn = {1, 0};
  if (ball->state == ROLLING)
    return defaultReturn;

  vector_t relVelocity = relativeVelocity(ball);
  if (relVelocity.x == 0 && relVelocity.y == 0)
    return defaultReturn;

  return rotate2d(normalizeVector(relVelocity), -ang);
}

QuarticCoeff getBallBallCollisionCoeff(Ball *ball1, Ball *ball2, double uRolling, double uSlidding, double g) {

  QuarticCoeff coefficient;
  if (ballNotMoving(ball1) && ballNotMoving(ball2)){
    printf("Balls not moving, error\n");
    coefficient.valid = false;
    return coefficient;
  }

  vector_t a1 = {0, 0};
  vector_t b1 = {0, 0};
  vector_t a2 = {0, 0};
  vector_t b2 = {0, 0};

  if (!ballNotMoving(ball1)) {
    double ang = angle(ball1->velocity);

    double v1 = magnitudeOf(ball1->velocity);

    vector_t u1 = get_uVec(ball1, ang);

    double K1 = -0.5 * g * (ball1->state == SLIDING ? uSlidding : uRolling);
    double cosseno = cos(ang);
    double seno = sin(ang);

    a1.x = K1 * (u1.x * cosseno - u1.y * seno);
    a1.y = K1 * (u1.x * seno + u1.y * cosseno);
    b1.x = v1 * cosseno;
    b1.y = v1 * seno;
  }

  if (!ballNotMoving(ball2)) {
    double ang = angle(ball2->velocity);
    double v2 = magnitudeOf(ball2->velocity);

    vector_t u2 = get_uVec(ball2, ang);

    double K2 = -0.5 * g * (ball2->state == SLIDING ? uSlidding : uRolling);
    double cosseno = cos(ang);
    double seno = sin(ang);

    a2.x = K2 * (u2.x * cosseno - u2.y * seno);
    a2.y = K2 * (u2.x * seno + u2.y * cosseno);
    b2.x = v2 * cosseno;
    b2.y = v2 * seno;
  }


  vector_t A = {a2.x - a1.x, a2.y - a1.y};
  vector_t B = {b2.x - b1.x, b2.y - b1.y};
  vector_t C = {ball2->position.x - ball1->position.x, ball2->position.y - ball1->position.y};



  coefficient.a = A.x * A.x + A.y * A.y;
  coefficient.b = 2 * A.x * B.x + 2 * A.y * B.y;
  coefficient.c = B.x * B.x + 2 * A.x * C.x + 2 * A.y * C.y + B.y * B.y;
  coefficient.d = 2 * B.x * C.x + 2 * B.y * C.y;
  coefficient.e = C.x * C.x + C.y * C.y - 4.0 * ball1->radius * ball1->radius;
  coefficient.valid = true;

  return coefficient;
}

QuarticCoeff getBallPocketCollisionCoeff(Ball *ball, Pocket* pocket, double uRolling, double uSlidding, double g) {

  QuarticCoeff coefficient;
  if (ballNotMoving(ball)){
    coefficient.valid = false;
    return coefficient;
  }

  double ang = angle(ball->velocity);

  double v = magnitudeOf(ball->velocity);

  vector_t u = get_uVec(ball, ang);


  double K = -0.5 * g * (ball->state == SLIDING ? uSlidding : uRolling);
  double cosseno = cos(ang);
  double seno = sin(ang);



  vector_t a, b;
  a.x = K * (u.x * cosseno - u.y * seno);
  a.y = K * (u.x * seno + u.y * cosseno);
  b.x = v * cosseno;
  b.y = v * seno;
  vector_t c = {ball->position.x, ball->position.y};


  coefficient.a = 0.5 * (a.x*a.x + a.y*a.y);
  coefficient.b = a.x * b.x + a.y * b.y;
  coefficient.c = a.x * (c.x - pocket->position.x) + a.y * (c.y - pocket->position.y) + 0.5 * (b.x*b.x + b.y*b.y);
  coefficient.d = b.x * (c.x - pocket->position.x) + b.y * (c.y - pocket->position.y);
  coefficient.e = 0.5 * (pocket->position.x * pocket->position.x + pocket->position.y * pocket->position.y + c.x*c.x + c.y*c.y - pocket->radius*pocket->radius)-(c.x * pocket->position.x+ c.y * pocket->position.y);
  coefficient.valid = true;

  return coefficient;
}


QuarticCoeff getBallCircularCushionCollisionCoeff(Ball *ball, CircularCushion* cushion, double uRolling, double uSlidding, double g){

  QuarticCoeff coefficient;
  if (ballNotMoving(ball)){
    coefficient.valid = false;
    return coefficient;
  }

  double phi = angle(ball->velocity);
  double v = magnitudeOf(ball->velocity);

  vector_t u = get_uVec(ball, phi);

  double K = -0.5 * g * (ball->state == SLIDING ? uSlidding:uRolling);
  double cosseno = cos(phi);
  double seno = sin(phi);

  vector_t a, b;
  a.x = K * (u.x * cosseno - u.y * seno);
  a.y = K * (u.x * seno + u.y * cosseno);
  b.x = v * cosseno;
  b.y = v * seno;
  vector_t c = {ball->position.x, ball->position.y};

  coefficient.a = 0.5 * (a.x * a.x + a.y * a.y);
  coefficient.b = a.x * b.x + a.y * b.y;
  coefficient.c = a.x * (c.x - cushion->position.x) + a.y * (c.y - cushion->position.y) + 0.5 * (b.x * b.x + b.y*b.y);
  coefficient.d = b.x * (c.x - cushion->position.x) + b.y * (c.y - cushion->position.y);
  coefficient.e = 0.5 * (cushion->position.x * cushion->position.x + cushion->position.y * cushion->position.y + c.x * c.x  + c.y*c.y - (cushion->radius + ball->radius) * (cushion->radius + ball->radius))- (c.x * cushion->position.x + c.y * cushion->position.y);
  coefficient.valid = true;


  return coefficient;
}





int findSmallerCoeficient(size_t n, QuarticCoeff *coeficients, double *result) {
  int index = -1;
  double smallerResult = INFINITY;

  for (size_t i = 0; i < n; i++) {

    QuarticCoeff coef = coeficients[i];
    if (!coef.valid) continue;

    double root = smallerPositiveQuarticRoot(coeficients[i].a, coeficients[i].b, coeficients[i].c, coeficients[i].d, coeficients[i].e);
    if (root < smallerResult) {
      index = i;
      smallerResult = root;
    }
  }

  *result = smallerResult;

  return index;
}
