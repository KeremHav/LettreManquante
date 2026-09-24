
#include "display.h"

/**
 * Displays the game splash screen from a file if available
 * or a default welcome message otherwise
 * Clears the screen before and after display
 */
void displaySplashScreen() {
    system("@cls||clear");
    FILE *file = fopen("fichiertexte/splash.txt", "r");
    if (file) {
        char line[256] = {0};
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }
        fclose(file);
        sleep(1);
    } else {
        printf("=== MOTS CACHÉS ===\n");
        printf("Un jeu de lettres cachées!\n\n");
        sleep(1);
    }
    system("@cls||clear");
}

/**
 * Displays the high scores table by reading from a binary file
 * Shows ranking, player names and scores
 * Wait for user input before returning
 */
void displayHighScores() {
    system("@cls||clear");
    printf("=== MEILLEURS SCORES ===\n\n");
    
    HighScore scores[MAX_SCORES] = {0};
    int numScores = 0;
    
    FILE* file = fopen("scores.bin", "rb");
    if (file) {
        fread(&numScores, sizeof(int), 1, file);
        if (numScores > 0) {
            fread(scores, sizeof(HighScore), numScores, file);
        }
        fclose(file);
    }else{
        printf("Erreur ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    
    if (numScores == 0) {
        printf("Aucun score enregistré pour le moment.\n");
    } else {
        printf("Rang  Nom du joueur          Score        Niveau Atteint%d\n",scores[1].level);
        printf("----------------------------------------------------------\n");
        for (int i = 0; i < numScores; i++) {
            const char* levelStr = "\0";
            switch(scores[i].level) {
                case 0: levelStr = "Facile"; break;
                case 1: levelStr = "Moyen"; break;
                case 2: levelStr = "Difficile"; break;
                default: levelStr = "Inconnu";
            }
            printf("%2d.   %-20s %5d                 %-9s\n", i+1, scores[i].name, scores[i].score, levelStr);
        }
    }
    printf("\nAppuyez sur Entrée pour continuer...");
    while (getchar() != '\n');
}


/**
 * Displays words with certain letters masked by colored numbers.
 * 
 * @param wordsStr The string containing words to display 
 * @param hiddenLetters Array of letters that should be hidden
 * @param lettersCount Number of letters in the hiddenLetters array
 */
void displayMaskedWord(const char *wordsStr, const char *hiddenLetters, int lettersCount) {
    printf(" ");
    for (int i = 0; wordsStr[i] != '\0'; i++) {
        if (wordsStr[i] == ' ') {
            printf("\n");

        }

        bool isHidden = false;
        for (int j = 0; j < lettersCount && !isHidden; j++) {
            if (wordsStr[i] == hiddenLetters[j]) {
                if(j == 0){
                    printf(RED "[%d]"RESET, j + 1);

                }else if(j == 1){
                    printf(GREEN "[%d]"RESET, j + 1);
                    
                }else{
                    printf(BLUE "[%d]"RESET, j + 1);
                   
                }
                isHidden = true;
            }
        }
        if (!isHidden) {
            printf("%c", wordsStr[i]);
        }
    }
    printf("\n");
}

/**
 * Displays words from a string, handling newline escape sequences.
 * 
 * @param wordsStr String containing words and \n escape sequences for newlines
 */
void displayWords(const char *wordsStr) {
    printf("\nLes mots étaient :\n");
    for (int i = 0; wordsStr[i] != '\0'; i++) {
        if (wordsStr[i] == ' ' ) {
            printf("\n");
            continue;
        }
        printf("%c", wordsStr[i]);
    }
    printf("\n");
}

/**
 * Displays the main menu interface and handles user navigation.
 * Shows options for rules, player name, high scores, new game and quit.
 * Manages menu state and transitions to other screens.
 */
void displayLevel(int *level,signed short *fraiseTagada, int* round){
    char* levelString = "\0";
    if(*level == 0){
            levelString = "Facile";
    }else if (*level == 1){
            levelString = "Moyen";
    }else{
            levelString = "Difficile";
    }
    printf("Joueur: %s\n", playerName);
    if(*fraiseTagada < 0){
        printf("Fraise tagada:" RED "%d \n" RESET , *fraiseTagada);
    }else{
        printf("Fraise tagada:%d \n" , *fraiseTagada);

    }
    printf("\n=== Niveau %s - Tour %d ===\n", levelString, *round + 1);

}

/**
 * Displays the game rules and instructions.
 * Includes information about gameplay mechanics and controls.
 * Waits for user confirmation before returning.
 */
void displayMenu() {
    bool menuActive = true;  
    while (menuActive) {
        system("@cls||clear");
        printf("\n=== MENU PRINCIPAL ===\n\n");
        if (playerName != NULL && strlen(playerName) > 0) {
            printf("Joueur: %s\n\n", playerName);
        }
        printf("1. Voir les règles\n");
        printf("2. Entrer/modifier le pseudonyme\n");
        printf("3. Voir les meilleurs scores\n");
        printf("4. Lancer une nouvelle partie\n");
        printf("5. Quitter le jeu\n\n");
        printf("Votre choix: ");
        char choice = fgetc(stdin);

        if (choice == '1') {
            displayRules();
        }
        else if (choice == '2') {
            getPlayerName();
        }
        else if (choice == '3') {
            displayHighScores();
        }
        else if (choice == '4') {
            if (playerName == NULL || strlen(playerName) == 0) {
                printf("\nVous devez d'abord entrer un pseudonyme!");
                sleep(1);
            } else {
                menuActive = false;
            }
        }
        else if (choice == '5') {
            cleanup();
            exit(EXIT_SUCCESS);
        }
        else {
            printf("\nChoix invalide, vous avez entré %c ! Veuillez entrez un nombre entre 1 - 4", choice);
        }
        int c = 0;
        while (c != '\n' && c != EOF){
            c = getchar();
        }
    }
}

/**
 * Displays the final game results including total fraise tagada earned/lost.
 * Saves high score if player has positive points.
 * 
 * @param fraiseTagada Final score to display (can be negative)
 */
void displayGameSummary(signed short fraiseTagada, int level) {
    system("@cls||clear");
    printf("\n=== Résultat final ===\n");
    if (fraiseTagada > 0) {
        printf("Fraises Tagada gagnées : %d\n", fraiseTagada);
    } else if (fraiseTagada == 0) {
        printf("\nÉgalité parfaite (%d fraises tagada)\n\n", fraiseTagada);
    } else {
        printf("\nTu me dois %d fraise(s) tagada !\n\n", -fraiseTagada);
    }
    
    if (playerName != NULL && fraiseTagada > 0) {
        saveFileScore(playerName, fraiseTagada, level);
    }
    
    printf("\nAppuyez sur Entrée pour continuer...");
    while (getchar() != '\n');
}


/**
 * Displays the complete game round interface including level info and masked words.
 * Clears screen and adds small delay before showing.
 * 
 * @param ctx Pointer to GameContext containing all current game state
 */
void displayGlobalRound(GameContext* ctx){
    sleep(2);
    system("@cls||clear");
    displayLevel(ctx->level,ctx->fraiseTagada,ctx->round);
    displayMaskedWord(ctx->currentWords,ctx->hiddenLetters,ctx->lettersCount);
}

/**
 * Displays the game rules and instructions with more details.
 * Includes comprehensive gameplay mechanics, scoring system and controls.
 * Waits for user confirmation before returning.
 */
void displayRules() {
    system("@cls||clear");
    printf("=== RÈGLES COMPLÈTES DU JEU MOTS CACHÉS ===\n\n");
    
    printf("🎯 OBJECTIF :\n");
    printf("   Trouver les lettres cachées dans les mots pour gagner des Fraises Tagada !\n\n");
    
    printf("🎮 MÉCANIQUES DE JEU :\n");
    printf("1. Les mots avec lettres cachées s'affichent avec des indicateurs colorés " RED"[1]" RESET", " GREEN"[2]" RESET", etc.\n");
    printf("2. Chaque indicateur correspond à une lettre à deviner\n");
    printf("3. Les couleurs différencient les lettres à trouver dans l'ordre\n\n");
    
    printf("📊 SYSTÈME DE SCORE :\n");
    printf("✔ Trouver une lettre : +1 Fraise Tagada\n");
    printf("✔ Trouver TOUTES les lettres d'un mot : Bonus +5 Fraises\n");
    printf("✘ Erreur : Perte d'un essai (sans perte de points)\n");
    printf("✘ Passer une lettre ('1') : -3 Fraises Tagada\n\n");
    
    printf("⚙ CONTROLES :\n");
    printf("▸ Entrez la lettre que vous pensez être cachée\n");
    printf("▸ '1' : Passer la lettre actuelle (pénalité)\n");
    printf("▸ '0' : Quitter la partie en cours\n\n");
    
    printf("PROGRESSION :\n");
    printf("- 3 niveaux de difficulté (Facile → Moyen → Difficile)\n");
    printf("- Nombre de tours par niveau : 3 (Facile), 2 (Moyen), 1 (Difficile)\n");
    printf("- Les mots deviennent plus longs et complexes à mesure que vous progressez\n\n");
    
    printf("CONSEILS :\n");
    printf("▸ Regardez les lettres visibles pour deviner les motifs de mots\n");
    printf("▸ Les mots sont généralement des termes courants\n");
    printf("▸ Attention aux lettres qui se répètent dans un même mot\n");
    
    printf("\nAppuyez sur Entrée pour retourner au menu...");
    while (getchar() != '\n');
}