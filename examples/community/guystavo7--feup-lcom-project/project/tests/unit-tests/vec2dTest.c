#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "../../include/vec2d.h" 

/* code for testing
  gcc -g -fsanitize=address vec2dTest.c ../../src/vec2d.c -o vec2dTest -lm
  ./vec2dTest
*/

/* ========================================================================== */
/* TESTING FRAMEWORK                                                          */
/* ========================================================================== */

#define EPSILON 0.0001f

// Helper to safely compare floating point numbers
bool float_equals(float a, float b) {
    return fabs(a - b) < EPSILON;
}

// Helper for formatted float test output
void print_result_float(const char* description, float expected, float obtained) {
    int passed = float_equals(expected, obtained);
    printf("%-55s | Expected: %-15.4f | Obtained: %-15.4f | [%s]\n", 
           description, expected, obtained, passed ? "TEST PASSED" : "TEST NOT PASSED");
}

/* ========================================================================== */
/* TEST CASES                                                                 */
/* ========================================================================== */

void test1_create_and_getters() {
    printf("\n--- Test 1: vec2dCreate & Getters ---\n");
    
    // Standard positive values
    Vec2d* v1 = vec2dCreate(3.5f, 2.1f);
    print_result_float("vec2dCreate sets X correctly (Positive)", 3.5f, vec2dGetX(v1));
    print_result_float("vec2dCreate sets Y correctly (Positive)", 2.1f, vec2dGetY(v1));
    
    // Negative values
    Vec2d* v2 = vec2dCreate(-10.99f, -0.05f);
    print_result_float("vec2dCreate sets X correctly (Negative)", -10.99f, vec2dGetX(v2));
    print_result_float("vec2dCreate sets Y correctly (Negative)", -0.05f, vec2dGetY(v2));

    vec2dFree(v1);
    vec2dFree(v2);
}

void test2_setters() {
    printf("\n--- Test 2: Setters ---\n");
    
    Vec2d* v = vec2dCreate(0.0f, 0.0f);
    
    vec2dSetX(v, 42.42f);
    print_result_float("vec2dSetX updates X correctly", 42.42f, vec2dGetX(v));
    
    vec2dSetY(v, -99.99f);
    print_result_float("vec2dSetY updates Y correctly", -99.99f, vec2dGetY(v));
    
    // Ensure one setter doesn't overwrite the other memory block
    print_result_float("vec2dSetY did not corrupt X", 42.42f, vec2dGetX(v));

    vec2dFree(v);
}

void test3_module_magnitude() {
    printf("\n--- Test 3: Module (Magnitude/Length) ---\n");
    
    // Standard 3-4-5 Triangle
    Vec2d* v1 = vec2dCreate(3.0f, 4.0f);
    print_result_float("vec2dModule calculates 3-4-5 magnitude", 5.0f, vec2dModule(v1));
    
    // Negative values (Magnitude should always be positive)
    Vec2d* v2 = vec2dCreate(-6.0f, 8.0f);
    print_result_float("vec2dModule handles negative coordinates", 10.0f, vec2dModule(v2));
    
    // Zero vector
    Vec2d* v_zero = vec2dCreate(0.0f, 0.0f);
    print_result_float("vec2dModule handles origin vector (0,0)", 0.0f, vec2dModule(v_zero));

    vec2dFree(v1);
    vec2dFree(v2);
    vec2dFree(v_zero);
}

void test4_direction_normalization() {
    printf("\n--- Test 4: Direction (Normalization) ---\n");
    
    Vec2d* v = vec2dCreate(3.0f, 4.0f);
    
    // vec2dNormalize modifies the vector IN PLACE
    vec2dNormalize(v);
    
    // Normalized vector of (3,4) is (3/5, 4/5) -> (0.6, 0.8)
    print_result_float("vec2dNormalize sets X correctly", 0.6f, vec2dGetX(v));
    print_result_float("vec2dNormalize sets Y correctly", 0.8f, vec2dGetY(v));
    
    // The magnitude of a normalized vector must ALWAYS be exactly 1.0
    print_result_float("vec2dNormalize magnitude is exactly 1.0", 1.0f, vec2dModule(v));

    vec2dFree(v); 
}

void test5_direction_edge_cases() {
    printf("\n--- Test 5: Direction Edge Cases (Divide by Zero) ---\n");
    
    Vec2d* v_zero = vec2dCreate(0.0f, 0.0f);
    vec2dNormalize(v_zero);
    
    bool handled_safely = false;
    float dx = vec2dGetX(v_zero);
    float dy = vec2dGetY(v_zero);
    
    // Check if the values are safely resting at 0.0f instead of NaN
    if (float_equals(dx, 0.0f) && float_equals(dy, 0.0f)) {
        handled_safely = true;
    }
    
    printf("%-55s | Expected: %-15s | Obtained: %-15s | [%s]\n", 
           "vec2dNormalize safely handles (0,0)", 
           "Handled", 
           handled_safely ? "Handled" : "Failed/NaN", 
           handled_safely ? "TEST PASSED" : "TEST NOT PASSED");

    vec2dFree(v_zero);
}

void test6_copy_constructors_and_setters() {
    printf("\n--- Test 6: Create/Set with Vec2d (Copying) ---\n");
    
    Vec2d* original = vec2dCreate(15.5f, -7.2f);
    
    // 1. Test vec2dCreateWithVec2d
    Vec2d* copy = vec2dCreateWithVec2d(original);
    print_result_float("vec2dCreateWithVec2d copies X correctly", 15.5f, vec2dGetX(copy));
    print_result_float("vec2dCreateWithVec2d copies Y correctly", -7.2f, vec2dGetY(copy));
    
    // Ensure deep copy (modifying copy shouldn't affect original)
    vec2dSetX(copy, 100.0f);
    print_result_float("Deep copy verified (Original X unchanged)", 15.5f, vec2dGetX(original));

    // 2. Test vec2dSetWithVec2d
    Vec2d* target = vec2dCreate(0.0f, 0.0f);
    vec2dSetWithVec2d(target, original);
    print_result_float("vec2dSetWithVec2d overwrites X correctly", 15.5f, vec2dGetX(target));
    print_result_float("vec2dSetWithVec2d overwrites Y correctly", -7.2f, vec2dGetY(target));

    vec2dFree(original);
    vec2dFree(copy);
    vec2dFree(target);
}

void test7_module2() {
    printf("\n--- Test 7: Module Squared (vec2dModule2) ---\n");
    
    Vec2d* v1 = vec2dCreate(3.0f, 4.0f);
    // Magnitude is 5, so Magnitude Squared should be 25
    print_result_float("vec2dModule2 calculates squared magnitude", 25.0f, vec2dModule2(v1));
    
    Vec2d* v2 = vec2dCreate(-6.0f, 8.0f);
    // Magnitude is 10, squared is 100
    print_result_float("vec2dModule2 with negative coordinates", 100.0f, vec2dModule2(v2));

    vec2dFree(v1);
    vec2dFree(v2);
}

void test8_scalar_multiplication() {
    printf("\n--- Test 8: Scalar Multiplication (vec2dMultiply) ---\n");
    
    Vec2d* v1 = vec2dCreate(2.0f, -3.5f);
    
    // Multiply by 2.5
    vec2dMultiply(v1, 2.5f);
    print_result_float("vec2dMultiply scales X correctly", 5.0f, vec2dGetX(v1));
    print_result_float("vec2dMultiply scales Y correctly", -8.75f, vec2dGetY(v1));
    
    // Multiply by 0 (should zero out the vector)
    vec2dMultiply(v1, 0.0f);
    print_result_float("vec2dMultiply by zero results in X=0", 0.0f, vec2dGetX(v1));
    print_result_float("vec2dMultiply by zero results in Y=0", 0.0f, vec2dGetY(v1));

    vec2dFree(v1);
}

void test9_add_sub_vectors() {
    printf("\n--- Test 9: Vector Addition & Subtraction ---\n");
    
    Vec2d* v_base = vec2dCreate(10.0f, 20.0f);
    Vec2d* v_operand = vec2dCreate(5.5f, -4.0f);
    
    // Add v_operand to v_base
    vec2dAddVec(v_base, v_operand);
    print_result_float("vec2dAddVec updates X correctly (10 + 5.5)", 15.5f, vec2dGetX(v_base));
    print_result_float("vec2dAddVec updates Y correctly (20 - 4.0)", 16.0f, vec2dGetY(v_base));
    
    // Ensure v_operand was not altered
    print_result_float("vec2dAddVec leaves operand X unchanged", 5.5f, vec2dGetX(v_operand));
    
    // Subtract v_operand from v_base
    vec2dSubVec(v_base, v_operand);
    // Should return exactly back to 10.0 and 20.0
    print_result_float("vec2dSubVec updates X correctly (15.5 - 5.5)", 10.0f, vec2dGetX(v_base));
    print_result_float("vec2dSubVec updates Y correctly (16.0 - (-4.0))", 20.0f, vec2dGetY(v_base));

    vec2dFree(v_base);
    vec2dFree(v_operand);
}

/* ========================================================================== */
/* MAIN TEST SUIT                                                             */
/* ========================================================================== */

void testSuitVec2d() {
    printf("==================================================================================\n");
    printf("                             VEC2D INTERFACE TEST SUITE                           \n");
    printf("==================================================================================\n");
    
    test1_create_and_getters();
    test2_setters();
    test3_module_magnitude();
    test4_direction_normalization();
    test5_direction_edge_cases();
    test6_copy_constructors_and_setters();
    test7_module2();
    test8_scalar_multiplication();
    test9_add_sub_vectors();

    printf("\n==================================================================================\n");
    printf("                                  END OF TEST SUITE                               \n");
    printf("==================================================================================\n");
}

int main() {
    testSuitVec2d();
    return 0;
}