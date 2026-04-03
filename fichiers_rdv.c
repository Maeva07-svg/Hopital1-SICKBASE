#include <stdio.h>
#include <stdlib.h>
#include "fichiers_rdv.h"
#include "rdv_avance.h"

void sauvegarderCreneaux()
{
    FILE *fichier = fopen(FICHIER_CRENEAUX, "wb");
    if (fichier == NULL) return;

    fwrite(&nombreCreneaux, sizeof(int), 1, fichier);
    fwrite(creneaux, sizeof(Creneau), nombreCreneaux, fichier);
    fclose(fichier);

    FILE *fid = fopen(FICHIER_ID_CRENEAU, "wb");
    if (fid != NULL)
    {
        fwrite(&dernierIDCreneau, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void chargerCreneaux()
{
    FILE *fichier = fopen(FICHIER_CRENEAUX, "rb");
    if (fichier == NULL) return;

    fread(&nombreCreneaux, sizeof(int), 1, fichier);
    fread(creneaux, sizeof(Creneau), nombreCreneaux, fichier);
    fclose(fichier);

    FILE *fid = fopen(FICHIER_ID_CRENEAU, "rb");
    if (fid != NULL)
    {
        fread(&dernierIDCreneau, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void sauvegarderDernierIDCreneau()
{
    FILE *fichier = fopen(FICHIER_ID_CRENEAU, "wb");
    if (fichier != NULL)
    {
        fwrite(&dernierIDCreneau, sizeof(int), 1, fichier);
        fclose(fichier);
    }
}

void chargerDernierIDCreneau()
{
    FILE *fichier = fopen(FICHIER_ID_CRENEAU, "rb");
    if (fichier != NULL)
    {
        fread(&dernierIDCreneau, sizeof(int), 1, fichier);
        fclose(fichier);
    }
    else
    {
        dernierIDCreneau = 0;
    }
}

void sauvegarderRendezVousAvance()
{
    FILE *fichier = fopen(FICHIER_RDV_AVANCE, "wb");
    if (fichier == NULL) return;

    fwrite(&nombreRdvAvance, sizeof(int), 1, fichier);
    fwrite(rendezVousAvance, sizeof(RendezVousAvance), nombreRdvAvance, fichier);
    fclose(fichier);

    FILE *fid = fopen(FICHIER_ID_RDV_AVANCE, "wb");
    if (fid != NULL)
    {
        fwrite(&dernierIDRdvAvance, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void chargerRendezVousAvance()
{
    FILE *fichier = fopen(FICHIER_RDV_AVANCE, "rb");
    if (fichier == NULL) return;

    fread(&nombreRdvAvance, sizeof(int), 1, fichier);
    fread(rendezVousAvance, sizeof(RendezVousAvance), nombreRdvAvance, fichier);
    fclose(fichier);

    FILE *fid = fopen(FICHIER_ID_RDV_AVANCE, "rb");
    if (fid != NULL)
    {
        fread(&dernierIDRdvAvance, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void sauvegarderDernierIDRdvAvance()
{
    FILE *fichier = fopen(FICHIER_ID_RDV_AVANCE, "wb");
    if (fichier != NULL)
    {
        fwrite(&dernierIDRdvAvance, sizeof(int), 1, fichier);
        fclose(fichier);
    }
}

void chargerDernierIDRdvAvance()
{
    FILE *fichier = fopen(FICHIER_ID_RDV_AVANCE, "rb");
    if (fichier != NULL)
    {
        fread(&dernierIDRdvAvance, sizeof(int), 1, fichier);
        fclose(fichier);
    }
    else
    {
        dernierIDRdvAvance = 0;
    }
}
