#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "utils.h"

void color(int couleurTexte, int couleurFond)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int couleur = couleurTexte + (couleurFond * 16);
    SetConsoleTextAttribute(hConsole, couleur);
}

void viderBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pause()
{
    printf("\nAppuyez sur Entree pour continuer...");
    viderBuffer();
}

void clearInput()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int comparerSymptomes(char symptome1[], char symptome2[])
{
    char s1[MAX_SYMPTOM_LENGTH], s2[MAX_SYMPTOM_LENGTH];
    strcpy(s1, symptome1);
    strcpy(s2, symptome2);

    for(int i = 0; s1[i]; i++) s1[i] = tolower(s1[i]);
    for(int i = 0; s2[i]; i++) s2[i] = tolower(s2[i]);

    return (strstr(s1, s2) != NULL || strstr(s2, s1) != NULL);
}
