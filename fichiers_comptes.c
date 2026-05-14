#include <stdio.h>
#include <stdlib.h>
#include "comptes.h"

#define FICHIER_COMPTES "comptes.dat"
#define FICHIER_ID_COMPTE "dernier_id_compte.dat"

void sauvegarderComptes()
{
    FILE *fichier = fopen(FICHIER_COMPTES, "wb");
    if (fichier == NULL)
    {
        printf("Erreur: Impossible de sauvegarder les comptes!\n");
        return;
    }

    fwrite(&nombreComptes, sizeof(int), 1, fichier);
    fwrite(comptes, sizeof(CompteUtilisateur), nombreComptes, fichier);
    fclose(fichier);

    FILE *fid = fopen(FICHIER_ID_COMPTE, "wb");
    if (fid != NULL)
    {
        fwrite(&dernierIDCompte, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void chargerComptes()
{
    FILE *fichier = fopen(FICHIER_COMPTES, "rb");
    if (fichier == NULL)
    {
        nombreComptes = 0;
        return;
    }

    fread(&nombreComptes, sizeof(int), 1, fichier);
    fread(comptes, sizeof(CompteUtilisateur), nombreComptes, fichier);
    fclose(fichier);

    FILE *fid = fopen(FICHIER_ID_COMPTE, "rb");
    if (fid != NULL)
    {
        fread(&dernierIDCompte, sizeof(int), 1, fid);
        fclose(fid);
    }
    else
    {
        dernierIDCompte = nombreComptes;
    }
}

void sauvegarderDernierIDCompte()
{
    FILE *fichier = fopen(FICHIER_ID_COMPTE, "wb");
    if (fichier != NULL)
    {
        fwrite(&dernierIDCompte, sizeof(int), 1, fichier);
        fclose(fichier);
    }
}

void chargerDernierIDCompte()
{
    FILE *fichier = fopen(FICHIER_ID_COMPTE, "rb");
    if (fichier != NULL)
    {
        fread(&dernierIDCompte, sizeof(int), 1, fichier);
        fclose(fichier);
    }
    else
    {
        dernierIDCompte = 0;
    }
}
