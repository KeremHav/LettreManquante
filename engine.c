#define _POSIX_C_SOURCE 200809L  

#include "engine.h"


/**
 * Global player name variable that persists throughout the game session.
 * Must be freed using cleanup() when no longer needed.
 */
char* playerName = NULL;


/**
 * Frees memory allocated for word sets structure including:
 * - Individual word strings
 * - Hidden letters arrays
 * - The sets array itself
 * 
 * @param sets Pointer to array of word sets to free
 * @param count Number of sets in the array
 */
void freeWordSets(word_set* sets, int count) {
    for (int i = 0; i < count; i++) {
        free(sets[i].words);
        sets[i].words =NULL;
        free(sets[i].communLetters);
        sets[i].communLetters = NULL;
    }
    free(sets);
}


/**
 * Cleans up game resources including:
 * - Player name memory
 * Should be called before program exit
 */
void cleanup() {
    if (playerName != NULL) {
        free(playerName);
        playerName = NULL;
    }
}

/**
 * Main game loop that manages:
 * - Progress through difficulty levels
 * - Round management
 * - Score tracking
 * - Resource cleanup
 * Displays summary screen when game ends
 */
void play() {
    srand(time(NULL));
    signed short fraiseTagada = 0; 
    struct {
        const char* filename;
        int rounds;
    } levels[] = {
        {"fichiertexte/easy.txt", 3}, {"fichiertexte/medium.txt", 2}, {"fichiertexte/hard.txt", 1}
    };
    int level = 0;
    while(level < 3){
        int wordSetSize = 0;
        word_set* wordSets = loadWordSetsFromFile(levels[level].filename, &wordSetSize);
        if (wordSets == NULL){
            printf("Erreur pas de mot trouvé");
            exit(EXIT_FAILURE);
        }
        for (int round = 0; round < levels[level].rounds; round++) {
            system("@cls||clear");
            displayLevel(&level,&fraiseTagada,&round);
            unsigned short randomSet = rand() % wordSetSize;
            GameContext ctx = {
                .level = &level,
                .fraiseTagada = &fraiseTagada,
                .round = &round,
            };
            bool finished = playRound(&wordSets[randomSet], &fraiseTagada,&ctx );
            if (!finished) {
                freeWordSets(wordSets, wordSetSize);
                displayGameSummary(fraiseTagada,level);
                return; 
            }
        }
        freeWordSets(wordSets, wordSetSize);
        level++;
    }
    if(level > 2){
        level--;
    }
    displayGameSummary(fraiseTagada,level);
}


/**
 * Saves high score to binary file, maintaining sorted order.
 * Only saves if score qualifies for top MAX_SCORES.
 * 
 * @param name Player name to save (truncated to MAX_NAME_LENGTH-1)
 * @param score Score to save
 * @param level Level reach by the player
 */
void saveFileScore(const char* name, int score, int level) {
    HighScore scores[MAX_SCORES] = {0};
    int numScores = 0;

    FILE* file = fopen("scores.bin", "rb");
    if (file) {
        fread(&numScores, sizeof(int), 1, file);
        if (numScores > 0) {
            fread(scores, sizeof(HighScore), (numScores > MAX_SCORES) ? MAX_SCORES : numScores, file);
        }
        fclose(file);
    }else{
        printf("Erreur à l'ouverture de fichier");
        exit(EXIT_FAILURE);
    }

    if (numScores >= MAX_SCORES) {
        numScores = MAX_SCORES - 1; 
    }

    strncpy(scores[numScores].name, name, MAX_NAME_LENGTH - 1);
    scores[numScores].name[MAX_NAME_LENGTH - 1] = '\0';
    scores[numScores].score = score;
    scores[numScores].level = level;
    numScores++;

    for (int i = 0; i < numScores - 1; i++) {
        for (int j = 0; j < numScores - i - 1; j++) {
            if (scores[j].score < scores[j + 1].score) {
                HighScore temp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp;
            }
        }
    }

    file = fopen("scores.bin", "wb");
    if (file) {
        fwrite(&numScores, sizeof(int), 1, file);
        fwrite(scores, sizeof(HighScore), numScores, file);
        fclose(file);
    } else {
        printf("Erreur à l'ouverture du fichier pour écriture\n");
        exit(EXIT_FAILURE);
    }
}


/**
 * Loads word sets from text file with format:
 * Line 1: Words (space separated)
 * Line 2: Letters to hide
 * Line 3: Number of hidden letters
 * Blank line between sets
 * 
 * @param filename File to load from
 * @param setCount Output parameter for number of loaded sets
 * @return Dynamically allocated array of word sets
 * @note Exits program on file/memory errors
 */
word_set* loadWordSetsFromFile(const char* filename, int* setCount) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Erreur d'ouverture du fichier %s\n", filename);
        exit(EXIT_FAILURE);
    }
    word_set* sets =  (word_set*)calloc(MAX_SETS, sizeof(word_set));
    if (!sets) {
        fclose(f);
        printf("Erreur d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }
    *setCount = 0;
    char line[MAX_LENGTH * 3] = {0}; 
    while (*setCount < MAX_SETS && fgets(line, sizeof(line), f)) {
        if (line[0] == '\n') continue;
        char* words = strdup(line);
        words[strcspn(words, "\n")] = '\0';

        if (fgets(line, sizeof(line), f)) {
            char* hiddenLetters = strdup(line);
            hiddenLetters[strcspn(hiddenLetters, "\n")] = '\0';
            if (fgets(line, sizeof(line), f)) {
                int lettersCount = atoi(line);
                sets[*setCount].words = words;
                sets[*setCount].communLetters = hiddenLetters;
                sets[*setCount].lettersCount = lettersCount;
                (*setCount)++;
            } else {
                free(hiddenLetters);
                free(words);
            }
        } else {
            free(words);
        }
        fgets(line, sizeof(line), f);
    }
    fclose(f);
    return sets;
}

/**
 * Manages a single game round including:
 * - Displaying masked words
 * - Letter guessing logic
 * - Attempt tracking
 * - Score calculation
 * @param currentSet Word set for this round
 * @param fraiseTagada Pointer to running score
 * @param ctx Game context for display
 * @return false if player quits mid-round, true otherwise
 */
bool playRound(word_set* currentSet, signed short* fraiseTagada, GameContext* ctx) {
    printf("\n=== Nouveau tour ===\n");
    displayMaskedWord(currentSet->words, currentSet->communLetters, currentSet->lettersCount);
    bool allCorrect = true;
    int currentLetter = 0;
    int attemptsLeft = MAX_ATTEMPTS;
    ctx->currentWords = currentSet->words;
    ctx->hiddenLetters = currentSet->communLetters;
    ctx->lettersCount = currentSet->lettersCount;
    while (currentLetter < currentSet->lettersCount && attemptsLeft > 0) {
        LetterResult* result = askForLetter(currentLetter, currentSet->communLetters[currentLetter], &attemptsLeft, ctx);
        if (result->scoreChange == -3) {  
            currentLetter++;  
            attemptsLeft = MAX_ATTEMPTS;  
        } 
        else if (!result->shouldContinue) {  
            free(result);
            return false;
        } 
        *fraiseTagada += result->scoreChange;
        allCorrect = allCorrect && result->success;
        if (result->success) {
            currentLetter++;
            attemptsLeft = MAX_ATTEMPTS; 
            if(ctx->lettersCount > 1){
            (*fraiseTagada)++; 
            }
        } 
        if(attemptsLeft == 0){
            currentLetter++;
        }
        free(result);
    }
    if(attemptsLeft == 0 ){
            displayWords(currentSet->words);
            sleep(2);
    }
    if (allCorrect) {
        if(ctx->lettersCount > 1){
            printf("\n+5 Fraise Tagada pour avoir trouvé toutes les lettres !\n");
        }else{
            printf("\n+5 Fraise Tagada pour avoir trouvé la lettre !\n");
        }
        (*fraiseTagada) += 5;
        sleep(1);
    }
    return true;
}

