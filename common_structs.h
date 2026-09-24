#ifndef COMMON_STRUCTS_H
#define COMMON_STRUCTS_H

#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>


#define MAX_NAME_LENGTH 25
#define MAX_SCORES 20


#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

extern char* playerName;

typedef struct {
    char name[MAX_NAME_LENGTH];
    int score;
    int level;
} HighScore;

typedef struct {
    bool success;
    bool shouldContinue;
    int scoreChange;
    int* remainingAttempts;
    
} LetterResult;

typedef struct {
    int* level;
    signed short* fraiseTagada;
    int* round;
    char* currentWords;
    char* hiddenLetters;
    int lettersCount;
} GameContext;



#endif