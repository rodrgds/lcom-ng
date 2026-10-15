#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "../../include/monster/path.h"
#include "../../include/data_structures/vector.h"
#include "../../include/utils/vec2d.h"

/* test command
  gcc -g -Wall -fsanitize=address pathTest.c ../../src/monster/path.c ../../src/vec2d.c ../../src/data_structures/vector.c -o pathTest -lm
  ./pathTest
*/

/* ========================================================================== */
/* TESTING FRAMEWORK                                                          */
/* ========================================================================== */

#define EPSILON 0.0001f

bool float_equals(float a, float b) {
    return fabs(a - b) < EPSILON;
}

void print_result_float(const char* description, float expected, float obtained) {
    int passed = float_equals(expected, obtained);
    printf("%-60s | Expected: %-10.4f | Obtained: %-10.4f | [%s]\n", 
           description, expected, obtained, passed ? "TEST PASSED" : "TEST NOT PASSED");
}

void print_result_bool(const char* description, bool expected, bool obtained) {
    int passed = (expected == obtained);
    printf("%-60s | Expected: %-10s | Obtained: %-10s | [%s]\n", 
           description, expected ? "True" : "False", obtained ? "True" : "False", passed ? "TEST PASSED" : "TEST NOT PASSED");
}

/* ========================================================================== */
/* TEST CASES                                                                 */
/* ========================================================================== */

void test1_path_create_and_subscribe() {
    printf("\n--- Test 1: Creation and Initial Subscription ---\n");
    
    // UPDATED: Added NULL as the freeFunction callback for the new Vector interface
    Vector* pts = vectorCreateWithFloatingData(sizeof(float), NULL, 4, 10.0f, 15.0f, 20.0f, 25.0f);
    Path* p = pathCreate(5.0f, pts);
    
    int dummy_entity = 1;
    void* ptr = &dummy_entity;
    
    pathSubscribe(p, ptr);
    
    Vec2d* curTarget = vec2dCreate(0.0f, 0.0f);
    pathGetCurPoint(p, ptr, curTarget);
    
    print_result_float("pathGetCurPoint initializes to Point 1 X", 10.0f, vec2dGetX(curTarget));
    print_result_float("pathGetCurPoint initializes to Point 1 Y", 15.0f, vec2dGetY(curTarget));

    vec2dFree(curTarget);
    pathFree(p);
    vectorFree(pts); 
}

void test2_multiple_subscribers() {
    printf("\n--- Test 2: Multiple Independent Subscribers ---\n");
    
    // UPDATED: Added NULL 
    Vector* pts = vectorCreateWithFloatingData(sizeof(float), NULL, 6, 0.0f, 0.0f, 100.0f, 0.0f, 100.0f, 100.0f);
    Path* p = pathCreate(2.0f, pts);
    
    int entity1 = 1;
    int entity2 = 2;
    
    pathSubscribe(p, &entity1);
    pathSubscribe(p, &entity2);
    
    // Advance ONLY entity 2 to the next point
    pathGoToNextPoint(p, &entity2);
    
    Vec2d* target1 = vec2dCreate(0.0f, 0.0f);
    Vec2d* target2 = vec2dCreate(0.0f, 0.0f);
    
    pathGetCurPoint(p, &entity1, target1);
    pathGetCurPoint(p, &entity2, target2);
    
    print_result_float("Subscriber 1 remains at Point 1 X", 0.0f, vec2dGetX(target1));
    print_result_float("Subscriber 2 advanced to Point 2 X", 100.0f, vec2dGetX(target2));

    vec2dFree(target1);
    vec2dFree(target2);
    pathFree(p);
    vectorFree(pts);
}

void test3_distance_checking() {
    printf("\n--- Test 3: Point Change Distance Logic ---\n");
    
    // UPDATED: Added NULL
    Vector* pts = vectorCreateWithFloatingData(sizeof(float), NULL, 2, 10.0f, 10.0f);
    Path* p = pathCreate(3.0f, pts);
    
    int entity = 1;
    pathSubscribe(p, &entity);
    
    // Entity is far away (distance = 5.0)
    Vec2d* pos_far = vec2dCreate(5.0f, 10.0f); 
    bool check1 = pathCheckInPointChangeDistance(p, &entity, pos_far);
    print_result_bool("Distance 5.0 > 3.0 (Should be False)", false, check1);
    
    // Entity is just inside radius (distance = 2.0)
    Vec2d* pos_close = vec2dCreate(8.0f, 10.0f); 
    bool check2 = pathCheckInPointChangeDistance(p, &entity, pos_close);
    print_result_bool("Distance 2.0 <= 3.0 (Should be True)", true, check2);

    vec2dFree(pos_far);
    vec2dFree(pos_close);
    pathFree(p);
    vectorFree(pts);
}

void test4_direction_calculation() {
    printf("\n--- Test 4: Direction to Current Point ---\n");
    
    // UPDATED: Added NULL
    Vector* pts = vectorCreateWithFloatingData(sizeof(float), NULL, 2, 10.0f, 5.0f);
    Path* p = pathCreate(1.0f, pts);
    
    int entity = 1;
    pathSubscribe(p, &entity);
    
    // Entity is at (10, 0). Needs to go UP to reach (10, 5)
    Vec2d* pos = vec2dCreate(10.0f, 0.0f);
    Vec2d* dir = vec2dCreate(0.0f, 0.0f);
    
    pathGetDirectionToCurPoint(p, &entity, pos, dir);
    
    print_result_float("Direction X should be 0.0", 0.0f, vec2dGetX(dir));
    print_result_float("Direction Y should be 1.0 (UP)", 1.0f, vec2dGetY(dir));

    vec2dFree(pos);
    vec2dFree(dir);
    pathFree(p);
    vectorFree(pts);
}

void test5_full_following_logic() {
    printf("\n--- Test 5: Full pathFollowingDir Logic ---\n");
    
    // UPDATED: Added NULL
    Vector* pts = vectorCreateWithFloatingData(sizeof(float), NULL, 4, 50.0f, 0.0f, 50.0f, 50.0f);
    Path* p = pathCreate(2.0f, pts);
    
    int entity = 1;
    pathSubscribe(p, &entity);
    
    Vec2d* dir = vec2dCreate(0.0f, 0.0f);
    
    Vec2d* pos_far = vec2dCreate(10.0f, 0.0f);
    pathFollowingDir(p, &entity, pos_far, dir);
    print_result_float("Following Dir far from P1 (X=1.0)", 1.0f, vec2dGetX(dir));
    print_result_float("Following Dir far from P1 (Y=0.0)", 0.0f, vec2dGetY(dir));
    
    // Scenario B: Hit Point 1 threshold! Pos = (49, 0).
    Vec2d* pos_close = vec2dCreate(49.0f, 0.0f);
    pathFollowingDir(p, &entity, pos_close, dir);
    
    Vec2d* currentTarget = vec2dCreate(0.0f, 0.0f);
    pathGetCurPoint(p, &entity, currentTarget);
    
    print_result_float("pathFollowingDir automatically advanced target X", 50.0f, vec2dGetX(currentTarget));
    print_result_float("pathFollowingDir automatically advanced target Y", 50.0f, vec2dGetY(currentTarget));

    vec2dFree(dir);
    vec2dFree(pos_far);
    vec2dFree(pos_close);
    vec2dFree(currentTarget);
    pathFree(p);
    vectorFree(pts);
}

/* ========================================================================== */
/* MAIN TEST SUIT                                                             */
/* ========================================================================== */

void testSuitPath() {
    printf("==================================================================================\n");
    printf("                              PATH INTERFACE TEST SUITE                           \n");
    printf("==================================================================================\n");
    
    test1_path_create_and_subscribe();
    test2_multiple_subscribers();
    test3_distance_checking();
    test4_direction_calculation();
    test5_full_following_logic();

    printf("\n==================================================================================\n");
    printf("                                  END OF TEST SUITE                               \n");
    printf("==================================================================================\n");
}

int main() {
    testSuitPath();
    return 0;
}
