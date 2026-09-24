#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "common_structs.h"
#include "display.h"


extern void getPlayerName();
extern LetterResult* askForLetter(int letterIndex, char expectedLetter, int* remainingAttempts,GameContext* ctx);

#endif