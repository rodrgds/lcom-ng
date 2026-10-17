#include "events.h"
#include "math.h"
#include "utilities.h"

Event getNextBallBallCollision(Table *table) {

  
  Event event = newInvalidEvent();

  size_t collisionNumber = 0;
  // TODO: Only save space for balls moving
  int size = (table->ballNumber + 1) * (table->ballNumber / 2.0) + 1;

  size_t *balls1 = (size_t *) malloc(sizeof(size_t) * size);
  size_t *balls2 = (size_t *) malloc(sizeof(size_t) * size);
  QuarticCoeff *coeficients = (QuarticCoeff *) malloc(sizeof(QuarticCoeff) * size);

  for (size_t i = 0; i < table->ballNumber - 1; i++) {

    Ball *ball1 = table->balls[i];

    for (size_t j = i + 1; j < table->ballNumber; j++) {

      Ball *ball2 = table->balls[j];

      if (ball1->state == POCKETED || ball2->state == POCKETED)
        continue;
      if (ballNotMoving(ball1) && ballNotMoving(ball2))
        continue;

      vector_t positionDiff = {ball1->position.x - ball2->position.x, ball1->position.y - ball2->position.y};
      if (magnitudeOf(positionDiff) < (ball1->radius + ball2->radius)) {
      
       
        continue;
      }

      balls2[collisionNumber] = j;
      balls1[collisionNumber] = i;

      coeficients[collisionNumber] = getBallBallCollisionCoeff(ball1, ball2, table->rollingFriction, table->slidingFriction, table->gravityAcceleration);

 
   
      // printCoef(coeficients[collisionNumber]);

      collisionNumber++;
    }
  }
  if (collisionNumber == 0) {
    return event;
  }
  double collisionTime;
  int i = findSmallerCoeficient(collisionNumber, coeficients, &collisionTime);
  if (i < 0) {
    printEvent(&event);
    return event;
  }

  event.type = BALL_BALL;
  event.ball1 = table->balls[balls1[i]];
  event.ball2 = table->balls[balls2[i]];
  event.time = collisionTime;

  free(coeficients);
  free(balls1);
  free(balls2);


  return event;
}

Event getNextBallLinearCushionCollision(Table *table) {
  
  Event event = newInvalidEvent();

  for (size_t i = 0; i < table->ballNumber; i++) {

    Ball *ball = table->balls[i];

    if (ballNotMoving(ball))
      continue;

   
    // TODO: is the hard code ok in this case?
    for (size_t j = 0; j < 6; j++) {

      LinearCushion* cushion = table->linearCushions[j];

      double collisionTime = getBallLinearCushionCollisionTime(table, ball, cushion);
      
      if (collisionTime < event.time) {
  
        event.time = collisionTime;
        event.type = BALL_LINEAR_CUSHION;
        event.ball1 = ball;
        event.linearCushion = cushion;
      }
    }
  }
  
  printEvent(&event);
  return event;
}


Event getNextBallCircularCushionCollision(Table* table){

  
  Event event = newInvalidEvent();
  size_t collisionNumber = 0;

  size_t size = table->ballNumber * 12;
  size_t* balls = (size_t*)malloc(sizeof(size_t) * size);
  size_t* cushions = (size_t*)malloc(sizeof(size_t) * size);
  QuarticCoeff* coeficients = (QuarticCoeff*)malloc(sizeof(QuarticCoeff) * size);

  for (size_t i = 0;  i < table->ballNumber; i++){

    Ball* ball = table->balls[i];

    if (ballNotMoving(ball)) continue;

    for (size_t j = 0; j<12; j++){

      CircularCushion* cushion = table->circularCushions[j];

      balls[collisionNumber] = i;
      cushions[collisionNumber] = j;
      coeficients[collisionNumber] = getBallCircularCushionCollisionCoeff(ball, cushion, table->rollingFriction, table->slidingFriction, table->gravityAcceleration);

      collisionNumber++;
    }
  }
  if (collisionNumber == 0){
    return event;
  }
  double collisionTime;
  int i = findSmallerCoeficient(collisionNumber, coeficients, &collisionTime);
  if (i == -1){
    return event;
  }

  event.type = BALL_CIRCULAR_CUSHION;
  event.ball1 = table->balls[balls[i]];
  event.circularCushion = table->circularCushions[cushions[i]];
  event.time = collisionTime;

  free(coeficients);
  free(balls);
  free(cushions);
  return event;
}



Event getNextBallPocketCollision(Table *table) {



  Event event = newInvalidEvent();
  int size = table->ballNumber * 6;
  size_t *balls = (size_t *) malloc(sizeof(size_t) * size);

  // TODO: CHECK IF POCKET NEEDS TO BE STORED
  size_t *pockets = (size_t *) malloc(sizeof(size_t) * size);
  QuarticCoeff *coeficients = (QuarticCoeff *) malloc(sizeof(QuarticCoeff) * size);

  size_t collisionNumber = 0;

  for (size_t i = 0; i < table->ballNumber; i++) {

    Ball *ball = table->balls[i];

    if (ballNotMoving(ball))
      continue;

    for (size_t j = 0; j < 6; j++) {
      Pocket *pocket = table->pockets[j];

      balls[collisionNumber] = i;
      pockets[collisionNumber] = j;

      coeficients[collisionNumber] = getBallPocketCollisionCoeff(ball, pocket, table->rollingFriction, table->slidingFriction, table->gravityAcceleration);
      collisionNumber++;
    }
  }
  if (collisionNumber == 0) {
    return event;
  }

  double collisionTime;
  int i = findSmallerCoeficient(collisionNumber, coeficients, &collisionTime);
  if (i == -1)
    return event;

  event.type = BALL_POCKET;
  event.ball1 = table->balls[balls[i]];
  event.pocket = pockets[i];
  event.time = collisionTime;

  free(coeficients);
  free(balls);
  free(pockets);
  return event;
}

Event getNextTransition(Table *table) {

  Event nextTransition = newInvalidEvent();

  for (size_t i = 0; i < table->ballNumber; i++) {
    Ball *ball = table->balls[i];
    if (ball->transition->type == INVALID)
      continue;
    if (ball->transition->time < nextTransition.time) {
      nextTransition = *ball->transition;
    }
  }

  printEvent(&nextTransition);
  return nextTransition;
}

Event getNextEvent(Table *table) {

  Event event = newInvalidEvent();
  Event testEvent;

  testEvent = getNextTransition(table);
  event = testEvent.time < event.time ? testEvent : event;

  testEvent = getNextBallBallCollision(table);
  event = testEvent.time < event.time ? testEvent : event;

  testEvent = getNextBallLinearCushionCollision(table);
  event = testEvent.time < event.time ? testEvent : event;

  testEvent = getNextBallCircularCushionCollision(table);
  event = testEvent.time < event.time ? testEvent : event;

  testEvent = getNextBallPocketCollision(table);
  event = testEvent.time < event.time ? testEvent : event;


  return event;
}

void updateBallNextTransition(Table *table, Ball *ball) {

  if (ballNotMoving(ball)) {
    Event transition = newInvalidEvent();
    *ball->transition = transition;
    return;
  }

  switch (ball->state) {

    case SPINNING: {
      double time = getSpinTime(ball, table->spinningFriction, table->gravityAcceleration);
      Event transition = {time, SPINNING_STATIONARY, ball, NULL, NULL,NULL, -1};
      *ball->transition = transition;
      break;
    }
    case ROLLING: {

      double rollTime = getRollTime(ball, table->rollingFriction, table->gravityAcceleration);
      double spinTime = getSpinTime(ball, table->spinningFriction, table->gravityAcceleration);
      Event transition = {rollTime, ROLLING_STATIONARY, ball, NULL, NULL, NULL, -1};
      if (spinTime > rollTime) {
        transition.type = ROLLING_SPINNING;
      }
      *ball->transition = transition;
      break;
    }

    case SLIDING: {

      double slideTime = getSlideTime(ball, table->slidingFriction, table->gravityAcceleration);
      Event transition = {slideTime, SLIDING_ROLLING, ball, NULL, NULL,NULL, -1};
      *ball->transition = transition;
      break;
    }
    default:
      break;
  }
}
