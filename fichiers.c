#include <stdio.h>
#include <stdlib.h>
#include "fichiers.h"
#include "patient.h"
#include "utils.h"

void sauvegarderDernierID()
{
    FILE *fichier = fopen(FICHIER_ID, "wb");
    if (fichier != NULL)
    {
        fwrite(&dernierID, sizeof(int), 1, fichier);
        fclose(fichier);
    }
}

void chargerDernierID()
{
    FILE *fichier = fopen(FICHIER_ID, "rb");
    if (fichier != NULL)
    {
        fread(&dernierID, sizeof(int), 1, fichier);
        fclose(fichier);
    }
    else
    {
        dernierID = 0;
    }
}

void sauvegarderPatients()
{
    FILE *fichier = fopen(FICHIER_PATIENTS, "wb");
    if (fichier == NULL)
    {
        printf("ERREUR: Impossible de sauvegarder les patients!\n");
        return;
    }

    fwrite(&nombrePatients, sizeof(int), 1, fichier);

    for (int i = 0; i < nombrePatients; i++)
    {
        fwrite(&patients[i], sizeof(Patient), 1, fichier);
    }

    fclose(fichier);
    sauvegarderDernierID();
}

void chargerPatients()
{
    FILE *fichier = fopen(FICHIER_PATIENTS, "rb");
    if (fichier == NULL)
    {
        printf("Aucune donnee existante. Initialisation d'une nouvelle base.\n");
        return;
    }

    fread(&nombrePatients, sizeof(int), 1, fichier);

    for (int i = 0; i < nombrePatients; i++)
    {
        fread(&patients[i], sizeof(Patient), 1, fichier);
    }

    fclose(fichier);
}
