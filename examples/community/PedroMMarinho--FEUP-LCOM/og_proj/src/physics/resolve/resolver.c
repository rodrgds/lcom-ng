#include "resolver.h"
#include "../events.h"
#include "../physicsMacros.h"
#include "../utilities.h"
#include <math.h>
// TODO: REMOVE ASSERT, FOR TESTING ONLY
#include <assert.h>


void resolveEvent(Table *table, Event event) {

  switch (event.type) {
    case INVALID:
      return;
      break;
      // Assert that velocity is almost 0 and that set them to 0
    case SPINNING_STATIONARY:
    case ROLLING_STATIONARY:

      assertStationary(event.ball1);
      updateBallNextTransition(table, event.ball1);
      break;
    case ROLLING_SPINNING:
      assertSpinning(event.ball1);
      updateBallNextTransition(table, event.ball1);
      break;
    case SLIDING_ROLLING:

      event.ball1->state = ROLLING;
      updateBallNextTransition(table, event.ball1);
      break;
    case BALL_BALL:
      if (table->firstBallHit == NULL) {

        table->firstBallHit = event.ball2;
      }
      resolveBallBall(event.ball1, event.ball2);
      updateBallNextTransition(table, event.ball1);
      updateBallNextTransition(table, event.ball2);
      break;
    case BALL_LINEAR_CUSHION:
      fixNormalDirection(event.linearCushion, event.ball1);
      makeBallLinearCushionKiss(event.ball1, event.linearCushion);
      resolveBallCushion(event.ball1, event.linearCushion->normal, table->cushionRestitution);
      updateBallNextTransition(table, event.ball1);
      break;
    case BALL_CIRCULAR_CUSHION:
      makeBallCircularCushionKiss(event.ball1, event.circularCushion);
      resolveBallCushion(event.ball1, CircularCushionNormal(event.circularCushion, event.ball1), table->cushionRestitution);
      updateBallNextTransition(table, event.ball1);
      break;
    case BALL_POCKET:
      printf("RESOLVE BALL POCKET\n");
      Ball *ball = event.ball1;
      if (table->player1->isPlaying) {
        if (table->player1->ballType == PLAYERBALLNONE) {
          if (ball->type != WHITE) {
            table->player1->ballType = ball->type == STRIPED ? PLAYERSTRIPED : PLAYERSOLID;
            table->player2->ballType = ball->type == STRIPED ? PLAYERSOLID : PLAYERSTRIPED;
          }
        }
      }
      else {
        if (table->player2->ballType == PLAYERBALLNONE) {
          if (ball->type != WHITE) {
            table->player2->ballType = ball->type == STRIPED ? PLAYERSTRIPED : PLAYERSOLID;
            table->player1->ballType = ball->type == STRIPED ? PLAYERSOLID : PLAYERSTRIPED;
          }
        }
      }
      if (((event.ball1->type == STRIPED) && (getPlayingPlayer(table)->ballType == PLAYERSTRIPED)) || ((event.ball1->type == SOLID) && (getPlayingPlayer(table)->ballType == PLAYERSOLID))) {
        table->pocketedOwnBall = true;
      }
      resolveBallPocket(event.ball1, table, table->pockets[event.pocket]);
      updateBallNextTransition(table, event.ball1);
      break;
    case STICK_BALL:
      // CHECK: WHITE IS ALWAYS BALL 0 ??
      resolveStickBall(table->cue, event.ball1, table->maxSpeedShot);
      updateBallNextTransition(table, event.ball1);
      break;
    default:
      break;
  }
}

void resolveBallBall(Ball *ball1, Ball *ball2) {
  makeBallsKiss(ball1, ball2);

  printf("RESOLVE BALL BALL\n");

  vector_t posVec = {ball2->position.x - ball1->position.x, ball2->position.y - ball1->position.y};
  vector_t n = normalizeVector(posVec);
  vector_t t = rotate2d(n, M_PI / 2);

  vector_t velVec = {ball1->velocity.x - ball2->velocity.x, ball1->velocity.y - ball2->velocity.y};
  double velMagni = magnitudeOf(velVec);

  double ang = angleBetween(velVec, posVec);
  double seno = sin(ang);
  double cosseno = cos(ang);

  // TODO: CHECK IF ADDING V2 TO BOTH VELOCITIES ISNT A MISTAKE
  ball1->velocity.x = t.x * velMagni * seno + ball2->velocity.x;
  ball1->velocity.y = t.y * velMagni * seno + ball2->velocity.y;

  ball2->velocity.x = n.x * velMagni * cosseno + ball2->velocity.x;
  ball2->velocity.y = n.y * velMagni * cosseno + ball2->velocity.y;

  ball1->state = SLIDING;
  ball2->state = SLIDING;
}

void makeBallsKiss(Ball *ball1, Ball *ball2) {

  vector_t ballBallVec = {ball2->position.x - ball1->position.x, ball2->position.y - ball1->position.y};

  vector_t n = normalizeVector(ballBallVec);

  // TODO - Fix the use of EPS to make it regular
  double distance = magnitudeOf(ballBallVec);
  double error = 2.0 * ball1->radius - distance;
  if (error > 0) {
    double correction = (error + 1e-5) / 2;
    ball1->position.x -= correction * n.x;
    ball1->position.y -= correction * n.y;

    ball2->position.x += correction * n.x;
    ball2->position.y += correction * n.y;
  }
}

void resolveBallCushionRealistic(Ball *ball, vector_t cushionNormal, double restitution, double friction) {

  double psi = angle(cushionNormal);

  vector_t velocityR = rotate2d(ball->velocity, -psi);
  vector_t xyAngVelocity = {ball->ang_velocity.x, ball->ang_velocity.y};
  vector_t xyAngVelocityR = rotate2d(xyAngVelocity, -psi);

  double phi = fmod(angle(velocityR), (2 * M_PI));

  double h = 14;
  double theta_a = asin(h / ball->radius - 1);
  double cosseno = cos(theta_a);
  double seno = sin(theta_a);

  vector_t s;
  // sx = rvw_R[1, 0] * np.sin(theta_a) - rvw_R[1, 2] * np.cos(theta_a) + R * rvw_R[2, 1]
  s.x = velocityR.x * seno + ball->radius * xyAngVelocityR.y;
  s.y = -velocityR.y - ball->radius * ball->ang_velocity.z * cosseno + ball->radius * xyAngVelocityR.x * seno;
  double c = velocityR.x * cosseno;

  // CHECK MASS NECESSARY
  // II = 2 / 5 * m * R**2
  // A = 7 / 2 / m
  // B = 1 / m

  double II = 2 / 5 * (ball->radius * ball->radius);
  double A = 7 / 2;
  double B = 1;

  double PzE = (1 + restitution) * c / B;
  double PzS = sqrt(s.x * s.x + s.y * s.y) / A;

  double PX, PY, PZ;

  double helper = (1 + restitution) * c / B;
  if (PzS <= PzE) {
    PX = -s.x / A * seno - helper * cosseno;
    PY = s.y / A;
    PZ = s.x / A * cosseno - helper * seno;
  }
  else {
    PX = -friction * helper * cos(phi) * seno - helper * cosseno;
    PY = friction * helper * sin(phi);
    PZ = friction * helper * cos(phi) * cosseno - helper * seno;
  }

  velocityR.x += PX;
  velocityR.y += PY;

  xyAngVelocityR.x += -ball->radius / II * PY * seno;
  xyAngVelocityR.y += ball->radius / II * (PX * seno - PZ * cosseno);
  ball->ang_velocity.z += ball->radius / II * PY * cosseno;

  ball->velocity = rotate2d(velocityR, psi);
  xyAngVelocity = rotate2d(velocityR, psi);
  ball->ang_velocity.x = xyAngVelocity.x;
  ball->ang_velocity.y = xyAngVelocity.y;

  ball->state = SLIDING;
}

void resolveBallCushion(Ball *ball, vector_t cushionNormal, double restitution) {

  double ang = angle(cushionNormal);
  vector_t velocityC = rotate2d(ball->velocity, -ang);

  velocityC.x *= -restitution;

  ball->velocity = rotate2d(velocityC, ang);
  ball->state = SLIDING;
}

void makeBallLinearCushionKiss(Ball *ball, LinearCushion *cushion) {

  vector_t c = linePointClosestTo(cushion->p1, cushion->p2, ball->position);

  // TODO: CREATE FUNCIONS FOR THIS??
  // TODO: FIX USE OF EPS
  vector_t connection = {ball->position.x - c.x, ball->position.y - c.y};

  double distance = magnitudeOf(connection);
  double error = ball->radius - distance;
  if (error > 0) {
    double correction = (error + 1e-6) / 2;
    ball->position.x -= correction * cushion->normal.x;
    ball->position.y -= correction * cushion->normal.y;
  }
}

void makeBallCircularCushionKiss(Ball *ball, CircularCushion *cushion) {
  vector_t normal = CircularCushionNormal(cushion, ball);

  vector_t connection = {ball->position.x - cushion->position.x, ball->position.y - cushion->position.y};
  double distance = magnitudeOf(connection);
  double error = ball->radius + cushion->radius - distance;
  if (error > 0) {
    double correction = (error + EPS_SPACE);
    ball->position.x -= correction * normal.x;
    ball->position.y -= correction * normal.y;
  }
}

void resolveBallPocket(Ball *ball, Table *table, Pocket *pocket) {

  printf("Player 1 ball type: %d\n", table->player1->ballType);
  printf("Player 2 ball type: %d\n", table->player2->ballType);
  ball->state = POCKETED;
  ball->position = pocket->position;
}

void assertStationary(Ball *ball) {
  // TODO: USE BETTER EPS

  ball->velocity.x = 0;
  ball->velocity.y = 0;

  ball->ang_velocity.x = 0;
  ball->ang_velocity.y = 0;
  ball->ang_velocity.z = 0;

  ball->state = STATIONARY;
}

void assertSpinning(Ball *ball) {
  // TODO: USE BETTER EPS

  ball->velocity.x = 0;
  ball->velocity.y = 0;

  ball->ang_velocity.x = 0;
  ball->ang_velocity.y = 0;

  ball->state = SPINNING;
}

void resolveStickBall(Cue *cue, Ball *ball, double maxSpeed) {

  // TODO: CHECK IF SPIN IS NOT TOO MUCH
  double sideSpin = cue->sideEnglish * ball->radius * 0.15;
  double verticalSpin = cue->verticalEnglish * ball->radius * 0.15;

  double phi = cue->angle;
  double theta = cue->elevationAngle;

  // CHECK: II = 2 / 5 * m * R**2 if mass can be removed
  double II = 2.0 / 5.0 * ball->radius * ball->radius;
  double c = sqrt(ball->radius * ball->radius - sideSpin * sideSpin - verticalSpin * verticalSpin);

  // CHECK: numerator = 2 * M * V0 If mass can be removed
  double numerator = 2 * cue->charge * maxSpeed;
  double cosseno = cos(theta);
  double seno = sin(theta);

  double temp = sideSpin * sideSpin + (verticalSpin * cosseno) * (verticalSpin * cosseno) + (c * cosseno) * (c * cosseno) - 2 * verticalSpin * c * cosseno * seno;

  // CHECK denominator = 1 + m / M + 5 / 2 / R**2 * temp
  double denominator = 1 + 5.0 / (2.0 * (ball->radius * ball->radius)) * temp;

  double F = numerator / denominator;

  vector_t velocityB = {0, -F * cosseno};

  vector3_t vec = {-c * seno + verticalSpin * cosseno, sideSpin * seno, -sideSpin * cosseno};
  vector3_t angVelocityB = {F / II * vec.x, F / II * vec.y, F / II * vec.z};

  double ang = phi + M_PI / 2;

  ball->velocity = rotate2d(velocityB, ang);
  vector_t xyAngVelocity = {angVelocityB.x, angVelocityB.y};
  xyAngVelocity = rotate2d(xyAngVelocity, ang);
  ball->ang_velocity.x = xyAngVelocity.x;
  ball->ang_velocity.y = xyAngVelocity.y;
  ball->ang_velocity.z = angVelocityB.z;

  ball->state = SLIDING;
}
