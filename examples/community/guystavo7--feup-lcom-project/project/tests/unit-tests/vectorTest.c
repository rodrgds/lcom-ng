 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../../include/data_structures/vector.h" 

/* test command
  gcc -g -Wall -fsanitize=address vectorTest.c  ../../src/data_structures/vector.c -o pathTest -lm
  ./vectorTest
*/

/* ========================================================================== */
/* TESTING FRAMEWORK                              */
/* ========================================================================== */

// Helper to format and print the aligned test output
void print_result(const char* description, long long expected, long long obtained) {
    int passed = (expected == obtained);
    printf("%-60s | Expected: %-15lld | Obtained: %-15lld | [%s]\n", 
           description, 
           expected, 
           obtained, 
           passed ? "TEST PASSED" : "TEST NOT PASSED");
}


/* ========================================================================== */
/* TEST CASES                                 */
/* ========================================================================== */

void test1_create_and_size() {
    printf("\n--- Test 1: vec_create, vec_size (Multiple Lengths) ---\n");
    
    Vector* v1 = vectorCreate(sizeof(char), NULL);          
    Vector* v4 = vectorCreate(sizeof(int), NULL);           
    Vector* v8 = vectorCreate(sizeof(long long int), NULL); 
    
    print_result("vec_create size 0 (1-byte data)", 0, vectorGetSize(v1));
    print_result("vec_create size 0 (4-byte data)", 0, vectorGetSize(v4));
    print_result("vec_create size 0 (8-byte data)", 0, vectorGetSize(v8));
    
    vectorFree(v1); vectorFree(v4); vectorFree(v8);
}

void test2_push_back_and_get() {
    printf("\n--- Test 2: vec_push_back, vectorGetCopy (Different Byte Lengths) ---\n");
    
    Vector* v_char = vectorCreate(sizeof(char), NULL);
    char c = -50; 
    vectorPushBack(v_char, (char*)&c);
    char* res_c = (char*)vectorGetCopy(v_char, 0);
    print_result("1-Byte Push/Get Copy (char)", -50, res_c ? *res_c : 0);
    
    Vector* v_int = vectorCreate(sizeof(int), NULL);
    int i = 2147483647; 
    vectorPushBack(v_int, (char*)&i);
    int* res_i = (int*)vectorGetCopy(v_int, 0);
    print_result("4-Byte Push/Get Copy (int)", 2147483647, res_i ? *res_i : 0);

    Vector* v_ll = vectorCreate(sizeof(long long int), NULL);
    long long int ll = 0x123456789ABCDEF0LL; 
    vectorPushBack(v_ll, (char*)&ll);
    long long int* res_ll = (long long int*)vectorGetCopy(v_ll, 0);
    print_result("8-Byte Push/Get Copy (long long)", 0x123456789ABCDEF0LL, res_ll ? *res_ll : 0);

    vectorFree(v_char); vectorFree(v_int); vectorFree(v_ll);
    free(res_c); free(res_i); free(res_ll);
}

void test3_push_back_val() {
    printf("\n--- Test 3: vec_push_back_val, vec_get_val (Edge Cases) ---\n");
    Vector* v = vectorCreate(sizeof(long long int), NULL); 
    
    vectorPushBackVal(v, 666LL);                  
    vectorPushBackVal(v, -1LL);                   
    vectorPushBackVal(v, 0LL);                    
    vectorPushBackVal(v, 9223372036854775807LL);  
    
    print_result("vec_get_val (Standard Positive)", 666LL, vectorGetVal(v, 0));
    print_result("vec_get_val (Negative -1)", -1LL, vectorGetVal(v, 1));
    print_result("vec_get_val (Zero)", 0LL, vectorGetVal(v, 2));
    print_result("vec_get_val (Max 64-bit int)", 9223372036854775807LL, vectorGetVal(v, 3));
    
    vectorFree(v);
}

void test4_set_and_set_val() {
    printf("\n--- Test 4: vec_set, vec_set_val (Using 2-byte short) ---\n");
    Vector* v = vectorCreate(sizeof(short), NULL); 
    
    vectorPushBackVal(v, 10LL);
    vectorPushBackVal(v, 20LL);

    short new_val = 32000;
    vectorSet(v, 0, (char*)&new_val);
    short* res1 = (short*)vectorGetCopy(v, 0);
    print_result("vec_set modifies 1st element via pointer", 32000, res1 ? *res1 : -1);

    vectorSetVal(v, 1, -15000LL); 
    short* res2 = (short*)vectorGetCopy(v, 1);
    print_result("vec_set_val modifies 2nd element directly", -15000, res2 ? *res2 : -1);

    vectorFree(v);
    free(res1); free(res2);
}

void test5_insert() {
    printf("\n--- Test 5: vec_insert (Block Shifting) ---\n");
    Vector* v = vectorCreate(sizeof(char), NULL); 
    
    vectorPushBackVal(v, 10LL);
    vectorPushBackVal(v, 30LL);

    char mid_val = 20;
    vectorInsert(v, 1, (char*)&mid_val); 
    
    print_result("vec_insert increases size", 3, vectorGetSize(v));
    
    char* res = (char*)vectorGetCopy(v, 1);
    print_result("vec_insert places element at index 1", 20, res ? *res : -1);

    char* res2 = (char*)vectorGetCopy(v, 2);
    print_result("vec_insert shifted subsequent element", 30, res2 ? *res2 : -1);

    vectorFree(v);
    free(res); free(res2);
}

void test6_pop_back() {
    printf("\n--- Test 6: vec_pop_back ---\n");
    Vector* v = vectorCreate(sizeof(int), NULL);
    vectorPushBackVal(v, 100000LL);
    vectorPushBackVal(v, 200000LL);
    
    vectorPopBack(v);
    print_result("vec_pop_back reduces size by 1", 1, vectorGetSize(v));

    int* res = (int*)vectorGetCopy(v, 0);
    print_result("Remaining element is untouched", 100000, res ? *res : -1);

    vectorFree(v);
    free(res);
}

void test7_remove() {
    printf("\n--- Test 7: vec_remove ---\n");
    Vector* v = vectorCreate(sizeof(long long int), NULL);
    vectorPushBackVal(v, 100LL);
    vectorPushBackVal(v, 200LL);
    vectorPushBackVal(v, 300LL);
    
    vectorRemove(v, 1); 
    
    print_result("vec_remove reduces size", 2, vectorGetSize(v));
    
    long long int* res = (long long int*)vectorGetCopy(v, 1);
    print_result("Elements are shifted left after remove", 300, res ? *res : -1);

    vectorFree(v);
    free(res);
}

void test8_resize() {
    printf("\n--- Test 8: vec_resize ---\n");
    Vector* v = vectorCreate(sizeof(int), NULL); 
    vectorPushBackVal(v, 1LL);
    vectorPushBackVal(v, 2LL);
    
    vectorResize(v, 10);
    print_result("vec_resize grows the vector capacity", 10, vectorGetSize(v));
    
    vectorResize(v, 1);
    print_result("vec_resize shrinks the vector capacity", 1, vectorGetSize(v));
    
    vectorFree(v);
}

void test9_find() {
    printf("\n--- Test 9: vec_find ---\n");
    Vector* v = vectorCreate(sizeof(int), NULL); 
    
    vectorPushBackVal(v, 44444LL);
    vectorPushBackVal(v, 55555LL);
    vectorPushBackVal(v, 66666LL);
    
    int target = 55555;
    print_result("vec_find locates existing element", 1, vectorFind(v, (char*)&target));
    
    int missing = 99999;
    print_result("vec_find returns -1 for missing elements", -1, vectorFind(v, (char*)&missing));

    vectorFree(v);
}

void test10_stress_test() {
    printf("\n--- Test 10: Stress Test (10,000 Items Dynamic Reallocation) ---\n");
    Vector* v = vectorCreate(sizeof(int), NULL);
    
    int iterations = 10000;
    for(int i = 0; i < iterations; i++) {
        vectorPushBackVal(v, (long long int)i);
    }
    
    print_result("Handled massive dynamic reallocations (size)", iterations, vectorGetSize(v));
    
    int* first = (int*)vectorGetCopy(v, 0);
    int* middle = (int*)vectorGetCopy(v, 5000);
    int* last = (int*)vectorGetCopy(v, 9999);
    
    print_result("Stress Test: 1st element intact", 0, first ? *first : -1);
    print_result("Stress Test: 5001st element intact", 5000, middle ? *middle : -1);
    print_result("Stress Test: Last element intact", 9999, last ? *last : -1);
    
    vectorFree(v);
    free(first); free(middle); free(last);
}

void test11_custom_structs() {
    printf("\n--- Test 11: Complex Custom Structs (Mixed Types/Padding) ---\n");
    
    typedef struct {
        int id;
        double health;
    } Player;
    
    Vector* v = vectorCreate(sizeof(Player), NULL);
    
    Player p1 = {1, 100.5};
    Player p2 = {2, 75.0};
    
    vectorPushBack(v, (char*)&p1);
    vectorPushBack(v, (char*)&p2);
    
    Player* res = (Player*)vectorGetCopy(v, 1);
    
    int struct_id_match = (res && res->id == 2) ? 1 : 0;
    int struct_hp_match = (res && res->health == 75.0) ? 1 : 0;
    
    print_result("Struct Copy: Integer field intact", 1, struct_id_match);
    print_result("Struct Copy: Double field intact", 1, struct_hp_match);
    
    vectorFree(v);
    if(res) free(res);
}

void test12_boundary_manipulations() {
    printf("\n--- Test 12: Extreme Boundary Insertions & Removals ---\n");
    Vector* v = vectorCreate(sizeof(int), NULL);
    
    vectorPushBackVal(v, 10LL);
    vectorPushBackVal(v, 20LL);
    
    int front = 999;
    vectorInsert(v, 0, (char*)&front);
    int* res_front = (int*)vectorGetCopy(v, 0);
    print_result("Insert at Head (Index 0)", 999, res_front ? *res_front : -1);
    
    vectorRemove(v, 0);
    int* res_after_rm = (int*)vectorGetCopy(v, 0);
    print_result("Remove Head (Shifted correctly)", 10, res_after_rm ? *res_after_rm : -1);
    
    vectorFree(v);
    free(res_front); free(res_after_rm);
}

void test13_strings() {
    printf("\n--- Test 13: Character Arrays (Strings) ---\n");
    Vector* v = vectorCreate(16, NULL); 
    
    char word1[16] = "Hello";
    char word2[16] = "Vector";
    
    vectorPushBack(v, word1);
    vectorPushBack(v, word2);
    
    char* res = (char*)vectorGetCopy(v, 1);
    int string_match = (res && strcmp(res, "Vector") == 0) ? 1 : 0;
    
    print_result("String pushed and copied correctly", 1, string_match);
    
    vectorFree(v);
    if(res) free(res);
}

void test14_floats() {
    printf("\n--- Test 14: Floating Point Numbers (float & double) ---\n");
    
    Vector* v_float = vectorCreate(sizeof(float), NULL); 
    float f1 = 3.14159f;
    float f2 = -987.654f;

    vectorPushBack(v_float, (char*)&f1);
    vectorPushBack(v_float, (char*)&f2);

    float* res_f1 = (float*)vectorGetCopy(v_float, 0);
    float* res_f2 = (float*)vectorGetCopy(v_float, 1);

    int f_match1 = (res_f1 && *res_f1 == 3.14159f) ? 1 : 0;
    int f_match2 = (res_f2 && *res_f2 == -987.654f) ? 1 : 0;

    print_result("Float Copy: 1st element exact (3.14159)", 1, f_match1);
    print_result("Float Copy: 2nd element exact (-987.654)", 1, f_match2);

    Vector* v_double = vectorCreate(sizeof(double), NULL);
    double d1 = 2.718281828459045; 
    
    vectorPushBack(v_double, (char*)&d1);
    double* res_d1 = (double*)vectorGetCopy(v_double, 0);
    
    int d_match = (res_d1 && *res_d1 == 2.718281828459045) ? 1 : 0;
    print_result("Double Copy: High precision exact match", 1, d_match);

    vectorFree(v_float);
    vectorFree(v_double);
    if(res_f1) free(res_f1);
    if(res_f2) free(res_f2);
    if(res_d1) free(res_d1);
}

void test15_get_back() {
    printf("\n--- Test 15: vectorGetBack (Retrieval of Last Element) ---\n");
    Vector* v = vectorCreate(sizeof(int), NULL); 
    
    vectorPushBackVal(v, 77LL);
    vectorPushBackVal(v, 88LL);
    vectorPushBackVal(v, 99LL);
    
    int* res = (int*)vectorGetBack(v);
    print_result("vectorGetBack fetches last element", 99, res ? *res : -1);

    vectorPopBack(v);
    int* res_after_pop = (int*)vectorGetBack(v);
    print_result("vectorGetBack accurate after pop", 88, res_after_pop ? *res_after_pop : -1);

    vectorFree(v);
}

void test16_create_with_data() {
    printf("\n--- Test 16: vectorCreateWithData (Variadic with NULL free) ---\n");
    
    Vector* v_int = vectorCreateWithData(sizeof(int), NULL, 3, 111, 222, 333);
    print_result("Variadic Create (int): Size initialized to 3", 3, vectorGetSize(v_int));
    
    int* res_i2 = (int*)vectorGetCopy(v_int, 2);
    print_result("Variadic Create (int): 3rd element exact", 333, res_i2 ? *res_i2 : -1);

    Vector* v_bool = vectorCreateWithData(sizeof(bool), NULL, 2, true, false);
    print_result("Variadic Create (bool): Size initialized to 2", 2, vectorGetSize(v_bool));
    
    bool* res_b0 = (bool*)vectorGetCopy(v_bool, 0);
    print_result("Variadic Create (bool): 1st element exact", 1, res_b0 ? *res_b0 : -1);

    vectorFree(v_int);
    vectorFree(v_bool);
    if (res_i2) free(res_i2);
    if (res_b0) free(res_b0);
}

void test17_create_with_floating_data() {
    printf("\n--- Test 17: vectorCreateWithFloatingData (Variadic with NULL free) ---\n");

    Vector* v_float = vectorCreateWithFloatingData(sizeof(float), NULL, 3, 3.14f, -2.5f, 0.0f);
    print_result("Variadic Floating (float): Size initialized to 3", 3, vectorGetSize(v_float));

    float* res_f1 = (float*)vectorGetCopy(v_float, 1);
    int f_match1 = (res_f1 && *res_f1 == -2.5f) ? 1 : 0;
    print_result("Variadic Floating (float): 2nd element exact", 1, f_match1);

    vectorFree(v_float);
    if (res_f1) free(res_f1);
}

// Global counter for the custom free function test
static int custom_free_call_count = 0;

void my_mock_free_function(void* data) {
    // Assuming the vector stores pointers to dynamically allocated ints
    int** ptr_to_dynamic_int = (int**)data;
    if (ptr_to_dynamic_int && *ptr_to_dynamic_int) {
        free(*ptr_to_dynamic_int);
        custom_free_call_count++;
    }
}

void test18_custom_free_function_intensive() {
    printf("\n--- Test 18: vectorCreate Custom freeFunction Callback (INTENSIVE) ---\n");
    custom_free_call_count = 0;
    
    // Create vector that stores POINTERS to integers, passing our custom callback
    Vector* v = vectorCreate(sizeof(int*), my_mock_free_function);
    
    // ---------------------------------------------------------
    // PHASE 1: Massive Allocation
    // ---------------------------------------------------------
    int TOTAL_INITIAL_ITEMS = 1000;
    for (int i = 0; i < TOTAL_INITIAL_ITEMS; i++) {
        int* dyn_int = malloc(sizeof(int)); 
        *dyn_int = i;
        vectorPushBack(v, (char*)&dyn_int);
    }
    print_result("Massive Allocation (Nothing freed yet)", 0, custom_free_call_count);

    // ---------------------------------------------------------
    // PHASE 2: Pop Back
    // ---------------------------------------------------------
    vectorPopBack(v);
    vectorPopBack(v);
    vectorPopBack(v);
    print_result("Callback triggered on vectorPopBack x3", 3, custom_free_call_count);

    // ---------------------------------------------------------
    // PHASE 3: Targeted Removal (Edges and Middle)
    // ---------------------------------------------------------
    vectorRemove(v, 0); // Remove Head
    vectorRemove(v, 500); // Remove Middle
    vectorRemove(v, vectorGetSize(v) - 1); // Remove Tail
    print_result("Callback triggered on targeted vectorRemove x3", 6, custom_free_call_count);

    // ---------------------------------------------------------
    // PHASE 4: Shrinking Resize
    // ---------------------------------------------------------
    // Current size is 1000 - 3 (pops) - 3 (removes) = 994.
    // Shrinking it to 500 should permanently discard and free 494 elements.
    int current_size = vectorGetSize(v);
    int target_size = 500;
    int expected_frees_from_resize = current_size - target_size;
    
    vectorResize(v, target_size);
    print_result("Callback triggered on vectorResize (Shrinking)", 6 + expected_frees_from_resize, custom_free_call_count);

    // ---------------------------------------------------------
    // PHASE 5: Overwriting Elements (Strict Memory Leak Check)
    // ---------------------------------------------------------
    // If you overwrite index 0, the pointer that USED to be there is lost.
    // A robust vector implementation MUST call the freeFunction on the old data before overwriting.
    int expected_frees_before_set = custom_free_call_count;
    
    int* new_dyn_int = malloc(sizeof(int)); 
    *new_dyn_int = 9999;
    vectorSet(v, 0, (char*)&new_dyn_int); // Overwrite index 0
    
    print_result("Callback triggered on vectorSet (Freeing old overwritten data)", expected_frees_before_set + 1, custom_free_call_count);

    // ---------------------------------------------------------
    // PHASE 6: Final Cleanup
    // ---------------------------------------------------------
    // Destroy the entire vector. 
    // Total allocations = 1000 (initial) + 1 (the new item from vectorSet) = 1001.
    // By the time the vector is freed, EVERY SINGLE ONE should have triggered the callback.
    vectorFree(v);
    print_result("Callback triggered on vectorFree for all remaining items", TOTAL_INITIAL_ITEMS + 1, custom_free_call_count);
}
/* ========================================================================== */
/* MAIN TEST SUIT                              */
/* ========================================================================== */

void testSuit() {
    printf("==================================================================================\n");
    printf("                            VECTOR INTERFACE TEST SUITE                           \n");
    printf("==================================================================================\n");
    
    test1_create_and_size();
    test2_push_back_and_get();
    test3_push_back_val();
    test4_set_and_set_val();
    test5_insert();
    test6_pop_back();
    test7_remove();
    test8_resize();
    test9_find();
    test10_stress_test();
    test11_custom_structs();
    test12_boundary_manipulations();
    test13_strings();
    test14_floats();
    
    // New Interface Tests
    test15_get_back();
    test16_create_with_data();
    test17_create_with_floating_data();
    test18_custom_free_function_intensive();

    printf("\n==================================================================================\n");
    printf("                                END OF TEST SUITE                                 \n");
    printf("==================================================================================\n");
}

int main() {
    // Run the full test suite
    //test
    /*
    gcc -g -fsanitize=address vectorTest.c vector.c -o vectorTest
    ./vectorTest
    */
    testSuit();
    return 0;
}