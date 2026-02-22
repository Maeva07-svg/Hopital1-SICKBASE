#include <stdio.h>
#include <stdlib.h>
#include "fichiers_personnel.h"
#include "personnel.h"
#include "utils.h"

void sauvegarderDernierIDPersonnel()
{
    FILE *fichier = fopen(FICHIER_ID_PERSONNEL, "wb");
    if (fichier != NULL)
    {
        fwrite(&dernierIDEmploye, sizeof(int), 1, fichier);
        fclose(fichier);
    }
}

void chargerDernierIDPersonnel()
{
    FILE *fichier = fopen(FICHIER_ID_PERSONNEL, "rb");
    if (fichier != NULL)
    {
        fread(&dernierIDEmploye, sizeof(int), 1, fichier);
        fclose(fichier);
    }
    else
    {
        dernierIDEmploye = 0;
    }
}

void sauvegarderPersonnel()
{
    FILE *fichier = fopen(FICHIER_PERSONNEL, "wb");
    if (fichier == NULL)
    {
        printf("ERREUR: Impossible de sauvegarder le personnel!\n");
        return;
    }

    fwrite(&nombreEmployes, sizeof(int), 1, fichier);

    for (int i = 0; i < nombreEmployes; i++)
    {
        fwrite(&personnel[i], sizeof(Employe), 1, fichier);
    }

    fclose(fichier);
    sauvegarderDernierIDPersonnel();
}

void chargerPersonnel()
{
    FILE *fichier = fopen(FICHIER_PERSONNEL, "rb");
    if (fichier == NULL)
    {
        printf("Aucune donnee personnel existante. Initialisation d'une nouvelle base.\n");
        return;
    }

    fread(&nombreEmployes, sizeof(int), 1, fichier);

    for (int i = 0; i < nombreEmployes; i++)
    {
        fread(&personnel[i], sizeof(Employe), 1, fichier);
    }

    fclose(fichier);
}
