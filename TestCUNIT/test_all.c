#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "../keyboard.h"
#include "../common_structs.h"
#include "../keyboard.c"
#include "../engine.c"
#include "../display.h"
#include "../display.c"

/*TESTS UNITAIRES */

void test_valid_file(void) {
    const char* filename = "TestCUNIT/test_valid.txt";
    int setCount = 0;
    
    word_set* sets = loadWordSetsFromFile(filename, &setCount);
    
    CU_ASSERT_EQUAL(setCount, 2);
    CU_ASSERT_STRING_EQUAL(sets[0].words, "mot1 mot2 mot3");
    CU_ASSERT_STRING_EQUAL(sets[0].communLetters, "abc");
    CU_ASSERT_EQUAL(sets[0].lettersCount, 3);
    
    for (int i = 0; i < setCount; i++) {
        free(sets[i].words);
        free(sets[i].communLetters);
    }
    free(sets);
}

void test_empty_file(void) {
    const char* filename = "TestCUNIT/test_empty.txt";
    int setCount = -1; 
    
    word_set* sets = loadWordSetsFromFile(filename, &setCount);
    
    CU_ASSERT_EQUAL(setCount, 0);
    CU_ASSERT_PTR_NOT_NULL(sets);
    free(sets);
}

void test_incomplete_set(void) {
    const char* filename = "TestCUNIT/test_incomplete.txt"; 
    int setCount = 0;
    
    word_set* sets = loadWordSetsFromFile(filename, &setCount);
    
    CU_ASSERT_EQUAL(setCount, 0);
    free(sets);
}

void test_normal_input(void) {
    FILE* input = fopen("TestCUNIT/test_input.txt", "w+");
    fprintf(input, "TestPlayer\n");
    rewind(input);
    freopen("TestCUNIT/test_input.txt", "r", stdin);
    
    getPlayerName();
    
    CU_ASSERT_PTR_NOT_NULL(playerName);
    if (playerName != NULL) {
        CU_ASSERT_STRING_EQUAL(playerName, "TestPlayer");
    }
    
    fclose(input);
    freopen("/dev/tty", "r", stdin);
}

void test_empty_input(void) {
    FILE* input = fopen("TestCUNIT/test_input.txt", "w+");
    fprintf(input, "\n");
    rewind(input);
    freopen("TestCUNIT/test_input.txt", "r", stdin);
    
    getPlayerName();
    
    CU_ASSERT_PTR_NOT_NULL(playerName);
    if (playerName != NULL) {
        CU_ASSERT_STRING_EQUAL(playerName, "");
    }
    
    fclose(input);
    freopen("/dev/tty", "r", stdin);
}

void test_long_input(void) {
    FILE* input = fopen("TestCUNIT/test_input.txt", "w+");
    for (int i = 0; i < MAX_NAME_LENGTH+10; i++) {
        fprintf(input, "a");
    }
    fprintf(input, "\n");
    rewind(input);
    freopen("TestCUNIT/test_input.txt", "r", stdin);
    
    getPlayerName();
    
    CU_ASSERT_PTR_NOT_NULL(playerName);
    if (playerName != NULL) {
        CU_ASSERT(strlen(playerName) <= MAX_NAME_LENGTH-1);
    }
    
    fclose(input);
    freopen("/dev/tty", "r", stdin);
}

void test_correct_guess(void) {
    GameContext ctx = {0};
    ctx.currentWords = strdup("test");
    ctx.hiddenLetters = strdup("e");
    ctx.lettersCount = 1;
    static short int score = 0;
    ctx.fraiseTagada = &score;

    FILE* input_file = fopen("test_input.txt", "w+");
    fprintf(input_file, "e\n");  
    rewind(input_file);
    
    FILE* real_stdin = stdin;
    stdin = input_file;

    int attempts = MAX_ATTEMPTS;
    LetterResult* result = askForLetter(0, 'e', &attempts, &ctx);

    CU_ASSERT_TRUE(result->success);
    CU_ASSERT_TRUE(result->shouldContinue);
    CU_ASSERT_EQUAL(attempts, MAX_ATTEMPTS);

    stdin = real_stdin;  
    fclose(input_file);
    remove("test_input.txt");
    free(result);
    free(ctx.currentWords);
    free(ctx.hiddenLetters);
}


void create_input(const char* input) {
    FILE* f = fopen("TestCUNIT/test_input.txt", "w");
    if (!f) {
        fprintf(stderr, "Erreur création fichier test\n");
        exit(EXIT_FAILURE);
    }
    fprintf(f, "%s", input);
    fclose(f);
    freopen("test_input.txt", "r", stdin);
}

int init_suite(void) {
    playerName = NULL;
    return 0;
}

int clean_suite(void) {
    if (playerName != NULL) {
        free(playerName);
        playerName = NULL;
    }
    remove("TestCUNIT/test_input.txt");
    return 0;
}

int main(void) {
    if (CU_initialize_registry() != CUE_SUCCESS) {
        fprintf(stderr, "Échec de l'initialisation de CUnit\n");
        return CU_get_error();
    }

    // loadWordSetsFromFile test's
    CU_pSuite fileSuite = CU_add_suite("File Loading Tests", NULL, NULL);
    CU_add_test(fileSuite, "Valid file", test_valid_file);
    CU_add_test(fileSuite, "Empty file", test_empty_file);
    CU_add_test(fileSuite, "Incomplete set", test_incomplete_set);

    // getPlayerName test's
    CU_pSuite inputSuite = CU_add_suite("Input Tests", init_suite, clean_suite);
    CU_add_test(inputSuite, "Normal input", test_normal_input);
    CU_add_test(inputSuite, "Empty input", test_empty_input);
    CU_add_test(inputSuite, "Long input", test_long_input);

    // askForLetter test's
    CU_pSuite gameSuite = CU_add_suite("askForLetter Test", init_suite, clean_suite);
    CU_add_test(gameSuite, "Bonne reponse", test_correct_guess);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}