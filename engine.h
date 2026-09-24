#ifndef ENGINE_H
#define ENGINE_H


#include "common_structs.h"
#include "display.h"


#define MAX_HIDDEN_LETTERS 3
#define MAX_LENGTH 50
#define MAX_SETS 100
#define MAX_ATTEMPTS 3





typedef struct {
    char* words;
    char* communLetters;
    int lettersCount;
} word_set;




extern char* playerName;

extern void play();
extern void freeWordSets(word_set* sets, int count);
extern void displayMaskedWord(const char *wordsStr, const char *hiddenLetters, int lettersCount);
extern LetterResult* askForLetter(int letterIndex, char expectedLetter, int* remainingAttempts,GameContext* ctx);
extern word_set* loadWordSetsFromFile(const char* filename, int* setCount);
extern bool playRound(word_set* currentSet, signed short* fraiseTagada, GameContext* ctx);
extern void cleanup();
extern void saveFileScore(const char* name, int score,int level);

#endif