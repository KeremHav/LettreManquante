#ifndef DISPLAY_H
#define DISPLAY_H
#include "common_structs.h"
#include "keyboard.h"
#include "engine.h"



extern void displaySplashScreen();
extern void displayHighScores();
extern void displayMenu();
extern void displayRules();
extern void displayMaskedWord(const char *wordsStr, const char *hiddenLetters, int lettersCount);
extern void displayWords(const char *wordsStr);
extern void displayGameSummary(signed short fraiseTagada, int level);
extern void displayLevel(int *level,signed short *fraiseTagada, int* round);
extern void displayGlobalRound(GameContext* ctx);


#endif
