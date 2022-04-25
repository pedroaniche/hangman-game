#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "hangman.h"

char secretword[WORD_SIZE];
char guesses[26];
int guessesgiven = 0;

void opening() {
    printf("**********************\n");
    printf("*    Hangman Game    *\n");
    printf("**********************\n\n");
}

void takeguess() {
    char guess;
    scanf(" %c", &guess);
    if(letterexists(guess)) {
        printf("\nYou hit: the word has the letter %C\n\n", guess);
    } else {
        printf("\nYou wrong: the word DOESN'T have the letter %c\n\n", guess);
    }
    guesses[guessesgiven] = guess;
    guessesgiven++;
}

int alreadyguessed(char letter) {
    int found = 0;
    for(int j = 0; j < guessesgiven; j++) {
        if(guesses[j] == letter) {
            found = 1;
            break;
        }
    }
    return found;
}

void drawgallows() {
    int wrongs = wrongguesses();

    printf("  _______         \n");
    printf(" |/      |        \n");
    printf(" |      %C%C%C    \n", wrongs>=1?'(':' ', wrongs>=1?'_':' ',wrongs>=1?')':' ');
    printf(" |      %C%C%C    \n", wrongs>=3?'\\':' ', wrongs>=2?'|':' ',wrongs>=3?'/':' ');
    printf(" |       %C       \n", wrongs>=2?'|':' ');
    printf(" |      %C %C     \n", wrongs>=4?'/':' ', wrongs>=4?'\\':' ');
    printf(" |                \n");
    printf("_|___             \n");

    for(int i = 0; i < strlen(secretword); i++) {
        if(alreadyguessed(secretword[i])) {
            printf("%c ", secretword[i]);
        } else {
            printf("_ ");
        }
    }
    printf("\n");
}

void addword() {
    char wish;
    printf("Do you wish add new word in the game?\n(Y) Yes\n(N) No\n");
    scanf(" %c", &wish);
    if(wish == 'Y') {
        char newword[WORD_SIZE];
        printf("What's the new word? ");
        scanf("%s", newword);
        FILE* words = fopen("words.txt", "r+");
        if(words == 0) {
            printf("Sorry, database not available\n\n");
            exit(1);
        }
        int amount;
        fscanf(words, "%d", &amount);
        amount++;
        fseek(words, 0, SEEK_SET);
        fprintf(words, "%d", amount);
        fseek(words, 0, SEEK_END);
        fprintf(words, "\n%s", newword);
        fclose(words);
    }
}

void chooseword() {
    FILE* words = fopen("words.txt", "r");
    if(words == 0) {
        printf("Sorry, database not available\n\n");
        exit(1);
    }
    int amountwords;
    fscanf(words, "%d", &amountwords);
    srand(time(0));
    int random = rand() % amountwords;
    for(int i = 0; i <= random; i++) {
        fscanf(words, "%s", secretword);
    }
    fclose(words);
}

int hitword() {
    for (int i = 0; i < strlen(secretword); i++) {
        if(!alreadyguessed(secretword[i])) {
            return 0;
        }
    }
    return 1;
}

int letterexists(char letter) {
    for(int j = 0; j < strlen(secretword); j++) {
        if(letter == secretword[j]) {
            return 1;
        }
    }
    return 0;
}

int wrongguesses() {
    int wrongs = 0;
    for(int i = 0; i < guessesgiven; i++) {
        if(!letterexists(guesses[i])) {
            wrongs++;
        }
    }
    return wrongs;
}

int hanged() {
    return wrongguesses() >= 5;
}

void drawendgame() {
    if(hitword) {
        printf("\nCongratulation, you won!\n\n");

        printf("       ___________      \n");
        printf("      '._==_==_=_.'     \n");
        printf("      .-\\:      /-.    \n");
        printf("     | (|:.     |) |    \n");
        printf("      '-|:.     |-'     \n");
        printf("        \\::.    /      \n");
        printf("         '::. .'        \n");
        printf("           ) (          \n");
        printf("         _.' '._        \n");
        printf("        '-------'       \n\n");

    } else {
        printf("\nGosh, you were hanged!\n");
        printf("The word was --%s--\n\n", secretword);

        printf("          ______          \n");
        printf("       .-"      "-.       \n");
        printf("      /            \\     \n");
        printf("     |              |     \n");
        printf("     |,  .-.  .-.  ,|     \n");
        printf("     | )(__/  \\__)( |    \n");
        printf("     |/     /\\     \\|   \n");
        printf("     (_     ^^     _)     \n");
        printf("      \\__|IIIIII|__/     \n");
        printf("       | \\IIIIII/ |      \n");
        printf("       \\          /      \n");
        printf("        `--------`        \n");

    }
}

int main() {
    chooseword();
    opening();
    do {
        drawgallows();
        takeguess();
    } while(!hitword() && !hanged());
    drawendgame();
    addword();
}
