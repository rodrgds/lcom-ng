#include <stdbool.h>
#include <stdio.h>

#include "../include/utils/timer.h"

/* ========================================================================== */
/* TESTING FRAMEWORK                                                          */
/* ========================================================================== */

// Helper for numeric results
void print_result(const char* description, long long expected, long long obtained) {
    int passed = (expected == obtained);
    printf("%-50s | Expected: %-15lld | Obtained: %-15lld | [%s]\n",
           description, expected, obtained, passed ? "TEST PASSED" : "TEST NOT PASSED");
}

// Helper for boolean results (True/False look much better than 1/0)
void print_result_bool(const char* description, bool expected, bool obtained) {
    int passed = (expected == obtained);
    printf("%-50s | Expected: %-15s | Obtained: %-15s | [%s]\n",
           description,
           expected ? "True" : "False",
           obtained ? "True" : "False",
           passed ? "TEST PASSED" : "TEST NOT PASSED");
}

/* ========================================================================== */
/* TEST CASES                                                                 */
/* ========================================================================== */

void test1_create_and_initial_state() {
    printf("\n--- Test 1: Creation and Initial State ---\n");
    long long int targetTime = 5000LL;
    Timer* t = newTimer(targetTime);

    print_result("newTimer stores initial time correctly", targetTime, timerGetInitialTime(t));
    print_result_bool("Timer is NOT running immediately after creation", false, timerIsRunning(t));

    freeTimer(t);
}

void test2_set_initial_time() {
    printf("\n--- Test 2: Set Initial Time ---\n");
    Timer* t = newTimer(1000LL);

    timerSetInitialTime(t, 9999LL);
    print_result("timerSetInitialTime overwrites old value", 9999LL, timerGetInitialTime(t));

    // Testing boundary conditions (0 and negatives)
    timerSetInitialTime(t, 0LL);
    print_result("timerSetInitialTime accepts 0", 0LL, timerGetInitialTime(t));

    timerSetInitialTime(t, -500LL);
    print_result("timerSetInitialTime accepts negative bounds", -500LL, timerGetInitialTime(t));

    freeTimer(t);
}

void test3_start_and_stop() {
    printf("\n--- Test 3: Start and Stop State Machine ---\n");
    Timer* t = newTimer(1000LL);

    timerStart(t);
    print_result_bool("timerStart sets isRunning to True", true, timerIsRunning(t));

    // Starting an already running timer shouldn't break it
    timerStart(t);
    print_result_bool("Calling timerStart twice safely keeps it True", true, timerIsRunning(t));

    timerStop(t);
    print_result_bool("timerStop sets isRunning to False", false, timerIsRunning(t));

    // Stopping an already stopped timer shouldn't break it
    timerStop(t);
    print_result_bool("Calling timerStop twice safely keeps it False", false, timerIsRunning(t));

    freeTimer(t);
}

void test4_restart() {
    printf("\n--- Test 4: Restart Logic ---\n");
    Timer* t = newTimer(2000LL);

    // Scenario A: Restarting a stopped timer
    timerRestart(t);
    print_result_bool("timerRestart starts a previously stopped timer", true, timerIsRunning(t));

    // Scenario B: Restarting an already running timer
    timerRestart(t);
    print_result_bool("timerRestart keeps an active timer running", true, timerIsRunning(t));

    freeTimer(t);
}

void test5_update_logic() {
    printf("\n--- Test 5: Update Logic & Expiration ---\n");
    /* NOTE: This test assumes that when deltaTime accumulates past the initial time,
             the timer automatically stops (isRunning becomes false). */

    Timer* t = newTimer(1000LL);
    timerStart(t);

    timerUpdate(t, 400LL);
    print_result_bool("Timer still running after partial update (400/1000)", true, timerIsRunning(t));

    timerUpdate(t, 599LL);
    print_result_bool("Timer still running right before expiration (999/1000)", true, timerIsRunning(t));

    timerUpdate(t, 1LL);
    print_result_bool("Timer automatically stops upon exact expiration (1000/1000)", false, timerIsRunning(t));

    // Test overshoot
    timerRestart(t);
    timerUpdate(t, 5000LL); // Massive delta time
    print_result_bool("Timer automatically stops upon massive overshoot", false, timerIsRunning(t));

    freeTimer(t);
}

void test6_update_while_stopped() {
    printf("\n--- Test 6: Update Guard Clauses ---\n");
    Timer* t = newTimer(1000LL);

    // Timer is NOT started yet
    timerUpdate(t, 2000LL); // Try to force it to expire while stopped

    timerStart(t);
    timerUpdate(t, 100LL);
    // If the 2000LL update leaked through while stopped, this will unexpectedly be False.
    print_result_bool("timerUpdate is ignored if timer is stopped", true, timerIsRunning(t));

    freeTimer(t);
}

void test7_multiple_instances() {
    printf("\n--- Test 7: Independent Instances (No Global State) ---\n");
    Timer* t1 = newTimer(1000LL);
    Timer* t2 = newTimer(5000LL);

    timerStart(t1);

    print_result_bool("T1 is running independently", true, timerIsRunning(t1));
    print_result_bool("T2 remains stopped independently", false, timerIsRunning(t2));

    timerUpdate(t1, 1000LL); // T1 expires

    print_result_bool("T1 expired independently", false, timerIsRunning(t1));
    print_result_bool("T2 is still unaffected", false, timerIsRunning(t2));

    freeTimer(t1);
    freeTimer(t2);
}

/* ========================================================================== */
/* MAIN TEST SUIT                                                             */
/* ========================================================================== */

void testTimer() {
    printf("==================================================================================\n");
    printf("                             TIMER INTERFACE TEST SUITE                           \n");
    printf("==================================================================================\n");

    test1_create_and_initial_state();
    test2_set_initial_time();
    test3_start_and_stop();
    test4_restart();
    test5_update_logic();
    test6_update_while_stopped();
    test7_multiple_instances();

    printf("\n==================================================================================\n");
    printf("                                  END OF TEST SUITE                               \n");
    printf("==================================================================================\n");
}


