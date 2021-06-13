#include<stdlib.h>
#include<windows.h>
#include <conio.h>
#include <ctype.h>
#include <stdio.h>

char *m[] = {"encrypt file", "decrypt file", "work in console", "exit"};
#define KEY_ARROW_RIGHT 77
#define KEY_ARROW_LEFT 75
#define KEY_ENTER 13
#define KEY_ESC 27
#define ATTR1 014
#define MENU_OX 25
#define MENU_OY 13
#define ENCRYPT 0
#define DECRYPT 1
#define INPUT_WORD 2
#define EXIT 3

int SYMBOLS_TO_REPLACE;

using namespace std;

CONSOLE_SCREEN_BUFFER_INFO cs;

void encrypt_decrypt_word();

void encrypt();

void decrypt();

char *cesarsEncryption(char line[], int symbolsReplace);

char *cesarsDecryption(char line[], int symbolsReplace);

void draw_menu(HANDLE h, COORD c, int chosen_item, int am_of_itms, int lenOfOne) {

    SetConsoleTextAttribute(h, ATTR1);
    COORD c2 = c;
    for (int i = 0; i < am_of_itms; i++) {
        c2.X = c.X + i * lenOfOne;
        SetConsoleCursorPosition(h, c2);
        for (int j = 0; m[i][j] != NULL; ++j) {
            printf("%c", m[i][j]);
        }
    }
    c2.X = c.X + chosen_item * lenOfOne;
    SetConsoleTextAttribute(h, BACKGROUND_INTENSITY);
    SetConsoleCursorPosition(h, c2);
    for (int j = 0; m[chosen_item][j] != NULL; ++j) {
        printf("%c", m[chosen_item][j]);
    }
};


int main() {
    char symb;
    printf("Input amount of symbols to shift in Cesar`s encryption: \n");
    scanf("%c", &symb);
    if (isdigit(symb)) {
        SYMBOLS_TO_REPLACE = symb - 48;
    }
    int i = 0;
    while (!isdigit(symb)) {
        if (i % 2) {
            printf("Wrong symbol. Try again. \n");
        }
        scanf("%c", &symb);
        if (isdigit(symb)) {
            SYMBOLS_TO_REPLACE = symb - 48;
        }
        i++;
    }
    system("cls");
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD source = {MENU_OX, MENU_OY};
    COORD curr = source;
    int fl = 1, chosen_item = 3, am_of_itmms = 4, len = 20;
    char ch;

    SetConsoleTextAttribute(h, ATTR1);
    system("cls");
    draw_menu(h, curr, am_of_itmms - 1, am_of_itmms, len);
    while (fl) {
        if (_kbhit()) {
            ch = _getch();
            switch (ch) {
                case KEY_ESC: {
                    fl = 0;
                    break;
                }
                case KEY_ENTER: {
                    switch (chosen_item) {
                        case ENCRYPT: {
                            system("cls");
                            encrypt();
                            break;
                        }
                        case DECRYPT: {
                            system("cls");
                            decrypt();
                            break;
                        }
                        case INPUT_WORD: {
                            system("cls");
                            encrypt_decrypt_word();
                            break;
                        }
                        case EXIT: {
                            return 0;
                        }
                    }
                }
                case KEY_ARROW_RIGHT: {
                    if (chosen_item == am_of_itmms - 1)chosen_item = 0;
                    else chosen_item++;
                    break;
                }
                case KEY_ARROW_LEFT: {
                    if (chosen_item == 0)chosen_item = am_of_itmms - 1;
                    else chosen_item--;
                    break;
                }
            }
            SetConsoleTextAttribute(h, ATTR1);
            system("cls");
            draw_menu(h, curr, chosen_item, am_of_itmms, len);
        }
    }
    COORD coord_esc = {30, 25};
    SetConsoleCursorPosition(h, coord_esc);
}

void decrypt() {
    char *newWord = new char[1024];
    FILE *fp;
    const static int maxString = 1024;
    char buf[maxString];
    fp = fopen("Decrypt.txt", "r");
    printf("Decrypted file:\n        ");
    while ((fgets(buf, maxString, fp))) {
        strcpy(newWord, cesarsDecryption(buf, 1));
        printf("%s", newWord);
    }
    fclose(fp);
    delete[] newWord;
    printf("\n\n\n\n");
    system("pause");
    system("cls");
}

void encrypt() {
    char *newWord = new char[1024];
    FILE *fp;
    const static int maxString = 1024;
    char buf[maxString];
    fp = fopen("Encrypt.txt", "r");
    int i = 0;
    printf("Encrypted file:\n");
    while ((fgets(buf, maxString, fp))) {
        if (i % 11)
            printf("\n");
        strcpy(newWord, cesarsEncryption(buf, SYMBOLS_TO_REPLACE));
        printf("%s", newWord);
        i++;
    }
    fclose(fp);

    printf("\n\n\n\n");
    delete[] newWord;
    system("pause");
    system("cls");
}

void encrypt_decrypt_word() {
    char choice;
    printf("Input 'e' to encrypt or 'd' to decrypt word: \n");
    chooseOption:
    scanf("%c", &choice);
    int i = 0;
    switch (choice) {
        case 'e': {
            int symbolsReplace;
            char someLine[1024];
            char *newLine;
            printf("Input your word: \n");
            scanf("%s", someLine);

            printf("Input amount of symbols to shift in Cesar`s encryption: \n");
            scanf("%i", &symbolsReplace);

            newLine = cesarsEncryption(someLine, symbolsReplace);
            printf("This is your encrypted word: %s\n", newLine);

            delete[] newLine;
            break;
        }
        case 'd': {
            int symbolsReplace;
            char someLine[1024];
            char *newLine2;
            printf("Input your word: \n");
            scanf("%s", someLine);
            getch();
            printf("Input amount of symbols that were shifted in Cesar`s encryption: \n");
            scanf("%i", &symbolsReplace);

            newLine2 = cesarsDecryption(someLine, symbolsReplace);
            printf("This is your decrypted word: %s\n", newLine2);
            delete[] newLine2;
            break;
        }
        default: {
            i++;
            goto chooseOption;
        }
    }
    printf("\n\n\n\n");
    system("pause");
    system("cls");
    system("pause");
}

char *cesarsDecryption(char line[], int symbolsReplace) {
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] != ' ' && line[i] != ',' && line[i] != '.' &&
            line[i] != '_' && line[i] != '-' && line[i] != '!' && line[i] != '?')
            line[i] = line[i] - symbolsReplace;
    }
    return line;
}

char *cesarsEncryption(char line[], int symbolsReplace) {
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] != ' ' && line[i] != ',' && line[i] != '.' &&
            line[i] != '_' && line[i] != '-' && line[i] != '!' && line[i] != '?')
            line[i] = line[i] + symbolsReplace;
    }
    return line;
}
