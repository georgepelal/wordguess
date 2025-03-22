#include "hw1.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Struct to represent an equivalent class
typedef struct equivalentClass {
    char *name;          // Name of the class
    int numOfWords;      // Number of words in the class
    char **words;        // Array of words in the class
} EquivalentClass_t;

#ifdef DEBUG
// Prints the details of an equivalent class (used for debugging)
void printClass(EquivalentClass_t class) {
    printf("%s %3d", class.name, class.numOfWords);
    for (int j = 0; j < class.numOfWords; j++) {
        printf(" %s", class.words[j]);
    }
    printf("\n");
}
#endif

/* Validates the command-line arguments and initializes word length and number of rounds , returning OK if everything works*/
errorCodeT checkArguments(int argc, char *argv[], int *wordlength, int *numofrounds) {
    if (argc != 4) {
        return INVALID_ARGS;
    }
    int WordLength = atoi(argv[1]);
    int NumOfRounds = atoi(argv[2]);
    if (WordLength <= 0) {
        return INVALID_LEN;
    } else if (NumOfRounds <= 0) {
        return INVALID_TURNS;
    } else {
        *wordlength = WordLength;
        *numofrounds = NumOfRounds;
    }
    return OK;
}

// Checks if a pointer is NULL and exits the program if it is
void checkPointer(void *pointer) {
    if (pointer == NULL) {
        printf("Memory error.\n");
        exit(1);
    }
}

// Initializes the dictionary by reading words of the specified length from a .dict file using getWord functions
char **dictionary_init_(char filename[], const int wordLength, int *numOfWords) {
    char **dictionary = (char **)malloc(sizeof(char *));
    checkPointer(dictionary);
    int counter;
    char *word;

    while (1) {
        word = getWord(filename);
        if (word == NULL) {
            free(dictionary);
            free(word);
            return NULL;
        }
        if (strlen(word) != wordLength) {
            free(word);
        } else {
            break;
        }
    }

    for (counter = 0; word != NULL; counter++) {
        dictionary = (char **)realloc(dictionary, sizeof(char *) * (counter + 2));
        checkPointer(dictionary);
        dictionary[counter] = word;

        while (1) {
            word = getWord(filename);
            if (word == NULL) {
                break;
            }
            if (strlen(word) == wordLength) {
                break;
            } else {
                free(word);
            }
        }
    }

    *numOfWords = counter;
    return dictionary;
}

// Converts all words in the dictionary to uppercase
void dictionary_toUppercase_(char **dictionary, const int numOfWords, const int wordLength) {
    for (int i = 0; i < numOfWords; i++) {
        for (int j = 0; j < wordLength; j++) {
            if (dictionary[i][j] >= 'a') {
                dictionary[i][j] = dictionary[i][j] - 32;
            }
        }
    }
}

// Prints the used and unused letters
void printLetters(char *usedLetters) {
    printf("Used letters: ");
    int used;
    for (char letter = 'A'; letter <= 'Z'; letter++) {
        used = 0;
        for (int i = 0; i < strlen(usedLetters); i++) {
            if (letter == usedLetters[i]) {
                used = 1;
            }
        }
        if (used) {
            printf(" %c", letter);
        }
    }

    printf("\nUnused letters: ");
    for (char letter = 'A'; letter <= 'Z'; letter++) {
        used = 1;
        for (int i = 0; i < strlen(usedLetters); i++) {
            if (letter == usedLetters[i]) {
                used = 0;
            }
        }
        if (used) {
            printf(" %c", letter);
        }
    }
}

// Prints the current word with guessed letters and underscores
void printWord(EquivalentClass_t *classes, int wordLength) {
    printf("\n\nWord: ");
    for (int i = 0; i < wordLength; i++) {
        printf(" %c", classes[0].name[i]);
    }
    printf("\n");
}

// Prompts the user to guess a letter and ensures it is valid
char guessLetter(char *usedLetters) {
    char letter;
    int cont;
    while (1) {
        cont = 1;
        printf("\nGuess a letter: ");
        letter = getchar();
        while (letter == '\n') {
            letter = getchar();
        }

        if (((letter < 'A') || (letter > 'z')) || ((letter > 'Z') && (letter < 'a'))) {
            printf("%c is not a letter.\n", letter);
            cont = 0;
        }

        for (int i = 0; i < strlen(usedLetters); i++) {
            if (letter == usedLetters[i]) {
                printf("You have already guessed %c.\n", letter);
                cont = 0;
            } else if (letter - 32 == usedLetters[i]) {
                printf("You have already guessed %c.\n", letter - 32);
                cont = 0;
            }
        }
        if (cont) {
            break;
        }
    }
    // Always return uppercase
    if (letter > 'Z') {
        return (letter - 32);
    }
    return letter;
}

// Initializes the starting equivalent class with all words and a blank name
void classes_init_(EquivalentClass_t *classes, char **dictionary, const int numOfWords, const int wordLength) {
    classes[0].numOfWords = 0;
    classes[0].words = (char **)malloc(sizeof(char *));
    checkPointer(classes[0].words);
    for (int i = 0; i < numOfWords; i++) {
        classes[0].words = (char **)realloc(classes[0].words, sizeof(char *) * (i + 1));
        checkPointer(classes[0].words);
        classes[0].words[i] = dictionary[i];
        classes[0].numOfWords++;
    }
    // Sets the starting class name to '_ _ _ _ _'
    classes[0].name = (char *)malloc(sizeof(char));
    checkPointer(classes[0].name);
    for (int i = 0; i < wordLength; i++) {
        classes[0].name[i] = '_';
        classes[0].name = (char *)realloc(classes[0].name, sizeof(char) * (i + 2));
        checkPointer(classes[0].name);
    }
    classes[0].name[wordLength] = '\0';
}

// Frees the memory allocated for an equivalent class
void classes_free_(EquivalentClass_t class_to_free) {
    free(class_to_free.words);
    free(class_to_free.name);
}

// Sorts the equivalent classes alphabetically by their names
void classes_sort_(EquivalentClass_t *classes, const int numOfClasses) {
    EquivalentClass_t temp;
    for (int i = 0; i < numOfClasses; i++) {
        for (int j = i + 1; j < numOfClasses; j++) {
            if (strcmp(classes[i].name, classes[j].name) > 0) {
                // Swap the classes
                temp = classes[i];
                classes[i] = classes[j];
                classes[j] = temp;
            }
        }
    }
}

// Sorts the words within an equivalent class alphabetically
void classes_sortWords_(EquivalentClass_t class_to_sort) {
    char *temp;
    for (int i = 0; i < class_to_sort.numOfWords - 1; i++) {
        for (int j = i + 1; j < class_to_sort.numOfWords; j++) {
            if (strcmp(class_to_sort.words[i], class_to_sort.words[j]) > 0) {
                // Swap the words
                temp = class_to_sort.words[i];
                class_to_sort.words[i] = class_to_sort.words[j];
                class_to_sort.words[j] = temp;
            }
        }
    }
}

// Generates new equivalent classes based on the guessed letter
EquivalentClass_t *classes_generate_(EquivalentClass_t *classes, char **dictionary, const char guessLetter, const int wordLength, int *numOfClasses) {
    *numOfClasses = 1;
    char name_tmp[wordLength + 1];
    for (int i = 0; i < classes[0].numOfWords; i++) { // for each word

        strcpy(name_tmp, classes[0].name);
        for (int j = 0; j < wordLength; j++) { // for each letter

            if ((classes[0].words[i][j]) == guessLetter) {
                name_tmp[j] = guessLetter;
            }
        }

        if (strcmp(name_tmp, classes[0].name) != 0) { // check if the letter does not exist in the word

            // Add the word to an already existing class, check if the class wanted already exists
            int classExists = 0;
            for (int k = 1; k < *numOfClasses; k++) {
                if (strcmp(name_tmp, classes[k].name) == 0) { // the class already exists
                    // add the word to the correct class
                    classes[k].numOfWords++;
                    classes[k].words = (char **)realloc(classes[k].words, sizeof(char *) * classes[k].numOfWords);
                    checkPointer(classes[k].words);
                    classes[k].words[classes[k].numOfWords - 1] = classes[0].words[i];
                    classExists = 1;
                    break;
                }
            }

            // the class does not exist, create a new one
            if (classExists == 0) {
                *numOfClasses += 1;
                classes = (EquivalentClass_t *)realloc(classes, sizeof(EquivalentClass_t) * (*numOfClasses));
                checkPointer(classes);
                classes[*numOfClasses - 1].numOfWords = 1;
                classes[*numOfClasses - 1].name = malloc(sizeof(char) * wordLength + 1);
                checkPointer(classes[*numOfClasses - 1].name);

                strcpy(classes[*numOfClasses - 1].name, name_tmp); // set new class name
                classes[*numOfClasses - 1].words = (char **)malloc(sizeof(char *));
                checkPointer(classes[*numOfClasses - 1].words);
                classes[*numOfClasses - 1].words[0] = classes[0].words[i];
            }

            // remove it from the previous one reallocating the memory
            classes[0].numOfWords--;
            classes[0].words[i] = classes[0].words[classes[0].numOfWords];
            classes[0].words = (char **)realloc(classes[0].words, sizeof(char *) * (classes[0].numOfWords));
            i = -1;
        }
    }

    if (classes[0].numOfWords == 0) { // delete the starting class if it is empty
        classes_free_(classes[0]);
        classes[0] = classes[*numOfClasses - 1];
        *numOfClasses -= 1;
        classes = (EquivalentClass_t *)realloc(classes, sizeof(EquivalentClass_t) * (*numOfClasses));
        checkPointer(classes);
    }
    return classes;
}

// Chooses the final equivalent class based on the number of words and spaces left
EquivalentClass_t *classes_chooseFinal_(EquivalentClass_t *classes, int *numOfClasses, const int wordLength) {
    int mostWords = classes[0].numOfWords;
    // keep only the classes with the most words
    for (int i = 0; i < *numOfClasses; i++) {
        if (classes[i].numOfWords < mostWords) {
            // delete the class if it has less words
            classes_free_(classes[i]);
            classes[i] = classes[(*numOfClasses) - 1];
            *numOfClasses -= 1;
            classes = (EquivalentClass_t *)realloc(classes, sizeof(EquivalentClass_t) * (*numOfClasses));
            checkPointer(classes);
            i = -1;
        } else if (classes[i].numOfWords > mostWords) {
            mostWords = classes[i].numOfWords;
            i = -1;
        }
    }
    if (*numOfClasses == 1)
        return classes;
    // if some classes have equal number of words keep the one with most spaces left
    int counter, mostSpaces = 0, spaceCounter;
    for (counter = 0; counter < *numOfClasses; counter++) {
        classes_sort_(classes, *numOfClasses);
        spaceCounter = 0;
        for (int i = 0; i < wordLength; i++) {
            if (classes[counter].name[i] == '_') {
                spaceCounter++;
            }
        }
        if (spaceCounter >= mostSpaces) {
            if (counter == 0) { // in case it is already the first class there is no need to move it
                mostSpaces = spaceCounter;
            } else {
                mostSpaces = spaceCounter;
                classes_free_(classes[0]);
                classes[0] = classes[counter];
                classes[counter] = classes[*numOfClasses - 1];
                *numOfClasses -= 1;
                classes = (EquivalentClass_t *)realloc(classes, sizeof(EquivalentClass_t) * (*numOfClasses));
                checkPointer(classes);
                counter = -1;
            }
        }
    }

    for (int i = 1; i < *numOfClasses; i++) {
        if (strcmp(classes[i].name, classes[0].name) != 0) {
            classes_free_(classes[i]);
        }
    }

    *numOfClasses = 1;
    classes = (EquivalentClass_t *)realloc(classes, sizeof(EquivalentClass_t));
    checkPointer(classes);
    return classes;
}

// Checks if the player has won the game
int checkForWin(EquivalentClass_t *classes, const int wordLength) {
    for (int i = 0; i < wordLength; i++) {
        if (classes[0].name[i] == '_') {
            return 0;
        }
    }
    printf("\nYou won! The word is %s.\n", classes[0].name);
    return 1;
}

// Asks the player if they want to play again
int playAgain() {
    char answer;
    do {
        printf("Play again? (y/n)\n");
        answer = getchar();
        while (answer == '\n') {
            answer = getchar();
        }
    } while ((answer != 'y') && (answer != 'Y') && (answer != 'n') && (answer != 'N'));

    if ((answer == 'y') || (answer == 'Y')) {
        return 1;
    }
    return 0;
}

// Plays a single round of the game
EquivalentClass_t *playRound(EquivalentClass_t *classes, int round, int numOfRounds, char *usedLetters, const int wordLength, char **dictionary, int *numOfClasses) {
    printf("\n== ROUND %d/%d ==\n\n", round, numOfRounds);
    printLetters(usedLetters);
    printWord(classes, wordLength);
    char guess;
    guess = guessLetter(usedLetters);

    usedLetters[round - 1] = guess;

    classes = classes_generate_(classes, dictionary, guess, wordLength, numOfClasses);

    classes_sort_(classes, *numOfClasses);

    // sort the classes
    for (int i = 0; i < *numOfClasses; i++) {
        classes_sortWords_(classes[i]);
    }

#ifdef DEBUG
    printf("\n== CLASSES ==\n");
    printf("%d\n\n", *numOfClasses);
    for (int i = 0; i < *numOfClasses; i++) {
        printClass(classes[i]);
    }
#endif

    classes = classes_chooseFinal_(classes, numOfClasses, wordLength);

#ifdef DEBUG
    printf("\n== CHOSEN ==\n");
    printClass(classes[0]);
#endif

    return classes;
}

// Main game loop that manages the game state and scores
void playGame(char **dictionary, const int numOfWords, const int wordLength, const int numOfRounds, int *playerScore, int *machineScore, int *numOfClasses) {
    EquivalentClass_t *classes = (EquivalentClass_t *)malloc(sizeof(EquivalentClass_t));
    checkPointer(classes);
    char *usedLetters = (char *)malloc(sizeof(char));
    checkPointer(usedLetters);
    usedLetters[0] = '\0';

    // create the starting class
    classes_init_(classes, dictionary, numOfWords, wordLength);
    classes_sortWords_(classes[0]);

    // play the game
    int win = 0;
    for (int round = 1; round <= numOfRounds; round++) {
        usedLetters = (char *)realloc(usedLetters, sizeof(char) * (round + 1));
        checkPointer(usedLetters);
        usedLetters[round] = '\0';
        classes = playRound(classes, round, numOfRounds, usedLetters, wordLength, dictionary, 
                            numOfClasses);
        win = checkForWin(classes, wordLength);
        if (win == 1) {
            *playerScore += 1;
            break;
        }
    }
    if (win == 0) {
        printf("\nYou lost! The word is %s.\n", classes[0].words[0]);
        *machineScore += 1;
    }

    printf("\n\nYOU: %d - ME: %d\n\n", *playerScore, *machineScore);

    // free classes and used letters
    classes_free_(classes[0]);
    free(classes);
    free(usedLetters);
    if (playAgain()) {
        playGame(dictionary, numOfWords, wordLength, numOfRounds, playerScore, machineScore, 
                 numOfClasses);
    }

    return;
}

// Main function to start the program
int main(int argc, char *argv[]) {
    int wordLength, numOfRounds, numOfWords, playerScore = 0, machineScore = 0, numOfClasses = 0;

    // Check the arguments using checkArguments, prints error message if something went wrong
    errorCodeT argResult = checkArguments(argc, argv, &wordLength, &numOfRounds);

    if (argResult != OK) {
        printErrorMsg(argResult);
        return 1;
    }

    char **dictionary = dictionary_init_(argv[3], wordLength, &numOfWords);
    dictionary_toUppercase_(dictionary, numOfWords, wordLength);

    if (dictionary == NULL) {
        printf("No words of length %d.\n", wordLength);
        return 2;
    }

    playGame(dictionary, numOfWords, wordLength, numOfRounds, &playerScore, &machineScore, 
             &numOfClasses);

    // Free dictionary
    for (int i = 0; i < numOfWords; i++) {
        free(dictionary[i]);
    }
    free(dictionary);

    return 0;
}