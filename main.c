
#include "engine.h"
#include "display.h"
#include "keyboard.h"



/**
 * @copyright Kerem Erkart
 * @author Kerem Erkart
 * 
 * \mainpage Hidden Letter Game
 * 
 * \section intro_sec Introduction
 * A word puzzle game where players uncover hidden letters in words.
 * 
 * \section display 
 * - Words are displayed with certain letters hidden
 * - Players guess letters marked with colored numbers [1], [2], etc.
 * - 3 attempts per letter
 * - Earn "fraise tagada" points for correct guesses
 * 
 * \section engine main function of the game
 * - Three difficulty levels
 * - Persistent high score system
 * - Dynamic word loading from files
 * - Colorful console interface
 * 
 * \section keyboard.c key press by the player
 * - Enter letters to guess
 * - '1' to skip letter (-3 points)
 * - '0' to quit game
 */
int main() {
    atexit(cleanup); 
    displaySplashScreen();
    
    bool replay = true;
    while (replay) {
        displayMenu();
        play();
        displayHighScores();
        printf("\nVoulez-vous rejouer ? (o/n): ");
        char response = '\0';
        scanf(" %c", &response);
        while (getchar() != '\n');
        if (tolower(response) != 'o') {
            replay = false;
        }
    }
    
    return EXIT_SUCCESS;
}