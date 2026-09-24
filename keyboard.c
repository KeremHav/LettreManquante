#include "keyboard.h"

/**
 * Prompts user to enter a player name and stores it globally.
 * 
 * - Clears screen before prompting
 * - Handles input buffer cleanup
 * - Manages memory allocation/reallocation for playerName
 * - Enforces maximum length (MAX_NAME_LENGTH-1)
 * - Exits on memory allocation failure
 */
void getPlayerName() {
    system("@cls||clear");
    printf("Entrez votre pseudonyme (max %d caractères): ", MAX_NAME_LENGTH-1);
    
    char buffer[MAX_NAME_LENGTH] = {0};
    scanf(" %24[^\n]", buffer);
    
    
    if (playerName != NULL) {
        free(playerName);
    }
    
    playerName = malloc(strlen(buffer) + 1);
    if (playerName == NULL) {
        printf("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }
    
    strcpy(playerName, buffer);
}
/**
 * Handles letter guessing logic for hidden words.
 * 
 * @param letterIndex Index of the letter being guessed (for display)
 * @param expectedLetter Correct letter that should be guessed
 * @param remainingAttempts Pointer to remaining guess attempts (modified)
 * @param ctx Game context for display updates
 * 
 * @return LetterResult* containing:
 *   - success: whether guess was correct
 *   - shouldContinue: whether game should proceed
 *   - scoreChange: points gained/lost
 *   - remainingAttempts: updated attempts
 * 
 */
LetterResult* askForLetter(int letterIndex, char expectedLetter, int* remainingAttempts,GameContext* ctx) {
    LetterResult* result = calloc(1,sizeof(LetterResult));
    if (result == NULL) {
        printf("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }
    *result = (LetterResult){
    .success = false,
    .shouldContinue = true,
    .scoreChange = 0,
    .remainingAttempts = remainingAttempts
    };
    result->success = false;
    result->shouldContinue = true;
    result->scoreChange = 0;
    result->remainingAttempts = remainingAttempts;
    bool inputFinish = false;
    while (*remainingAttempts > 0 && !result->success && !inputFinish) {
        if (letterIndex == 0){
            printf("\nQuel est la lettre" RED " [%d] " RESET "? (%d essai(s) restant(s)) : ", letterIndex + 1, *remainingAttempts);
        }else if(letterIndex == 1){
            printf("\nQuel est la lettre" GREEN " [%d] " RESET "? (%d essai(s) restant(s)) : ", letterIndex + 1, *remainingAttempts);
        }else{
            printf("\nQuel est la lettre" BLUE " [%d] " RESET "? (%d essai(s) restant(s)) : ", letterIndex + 1, *remainingAttempts);
        }
        char guess = '\0';
        if (scanf(" %c", &guess) == 1) {
            while (getchar() != '\n');
            if(guess == '0'){
                result->shouldContinue = false;
                inputFinish = true;
                continue;
            }
            if (!isalpha(guess) && guess != '1') {
                printf("Caractère %c est non valide. Veuillez entrer une lettre ou '1' pour passer ou '0' pour quitter.\n", guess);
                sleep(2);
                system("@cls||clear");
                displayGlobalRound(ctx);
                continue;
            }
            if (guess == '1') {
                result->scoreChange = -3;
                result->success = false;
                inputFinish = true;
                printf("La lettre était '%c' !. TU ME DOIS 3 FRAISES !\n", expectedLetter);
                displayGlobalRound(ctx);
            }
            else if (tolower(guess) == tolower(expectedLetter)) {
                printf("Correcte !\n");
                result->success = true;
                inputFinish = true;
            }
            else {
                (*remainingAttempts)--;
                printf("Incorrecte ! ");
                if (*remainingAttempts > 0) {
                    printf("Essaie encore.\n");
                    displayGlobalRound(ctx);
                } else {
                    inputFinish = true;
                }
            }
            if(guess!='1') sleep(1);
        }
    }
    return result;
}