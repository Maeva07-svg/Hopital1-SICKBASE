#include <stdio.h>
#include <stdlib.h>
#include "fichiers_relations.h"
#include "consultations.h"
#include "ordonnances.h"
#include "interventions.h"
#include "archives.h"

// ================= CONSULTATIONS =================
void sauvegarderConsultations()
{
    FILE *fichier = fopen(FICHIER_CONSULTATIONS, "wb");
    if (fichier == NULL) return;

    fwrite(&nombreConsultations, sizeof(int), 1, fichier);
    fwrite(consultations, sizeof(Consultation), nombreConsultations, fichier);
    fclose(fichier);

    FILE *fid = fopen(FICHIER_ID_CONSULTATION, "wb");
    if (fid != NULL)
    {
        fwrite(&dernierIDConsultation, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void chargerConsultations()
{
    FILE *fichier = fopen(FICHIER_CONSULTATIONS, "rb");
    if (fichier == NULL) return;

    fread(&nombreConsultations, sizeof(int), 1, fichier);
    fread(consultations, sizeof(Consultation), nombreConsultations, fichier);
    fclose(fichier);

    FILE *fid = fopen(FICHIER_ID_CONSULTATION, "rb");
    if (fid != NULL)
    {
        fread(&dernierIDConsultation, sizeof(int), 1, fid);
        fclose(fid);
    }
}

// ================= RENDEZ-VOUS =================
void sauvegarderRendezVous()
{
    FILE *fichier = fopen(FICHIER_RENDEZVOUS, "wb");
    if (fichier == NULL) return;

    fwrite(&nombreRendezVous, sizeof(int), 1, fichier);
    fwrite(rendezVous, sizeof(RendezVous), nombreRendezVous, fichier);
    fclose(fichier);

    FILE *fid = fopen(FICHIER_ID_RENDEZVOUS, "wb");
    if (fid != NULL)
    {
        fwrite(&dernierIDRendezVous, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void chargerRendezVous()
{
    FILE *fichier = fopen(FICHIER_RENDEZVOUS, "rb");
    if (fichier == NULL) return;

    fread(&nombreRendezVous, sizeof(int), 1, fichier);
    fread(rendezVous, sizeof(RendezVous), nombreRendezVous, fichier);
    fclose(fichier);

    FILE *fid = fopen(FICHIER_ID_RENDEZVOUS, "rb");
    if (fid != NULL)
    {
        fread(&dernierIDRendezVous, sizeof(int), 1, fid);
        fclose(fid);
    }
}

// ================= ORDONNANCES =================
void sauvegarderOrdonnances()
{
    FILE *fichier = fopen(FICHIER_ORDONNANCES, "wb");
    if (fichier == NULL) return;

    fwrite(&nombreOrdonnances, sizeof(int), 1, fichier);
    fwrite(ordonnances, sizeof(Ordonnance), nombreOrdonnances, fichier);
    fclose(fichier);

    FILE *fid = fopen(FICHIER_ID_ORDONNANCE, "wb");
    if (fid != NULL)
    {
        fwrite(&dernierIDOrdonnance, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void chargerOrdonnances()
{
    FILE *fichier = fopen(FICHIER_ORDONNANCES, "rb");
    if (fichier == NULL) return;

    fread(&nombreOrdonnances, sizeof(int), 1, fichier);
    fread(ordonnances, sizeof(Ordonnance), nombreOrdonnances, fichier);
    fclose(fichier);

    FILE *fid = fopen(FICHIER_ID_ORDONNANCE, "rb");
    if (fid != NULL)
    {
        fread(&dernierIDOrdonnance, sizeof(int), 1, fid);
        fclose(fid);
    }
}

// ================= INTERVENTIONS =================
void sauvegarderInterventions()
{
    FILE *fichier = fopen(FICHIER_INTERVENTIONS, "wb");
    if (fichier == NULL) return;

    fwrite(&nombreInterventions, sizeof(int), 1, fichier);
    fwrite(interventions, sizeof(Intervention), nombreInterventions, fichier);
    fclose(fichier);

    FILE *fid = fopen(FICHIER_ID_INTERVENTION, "wb");
    if (fid != NULL)
    {
        fwrite(&dernierIDIntervention, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void chargerInterventions()
{
    FILE *fichier = fopen(FICHIER_INTERVENTIONS, "rb");
    if (fichier == NULL) return;

    fread(&nombreInterventions, sizeof(int), 1, fichier);
    fread(interventions, sizeof(Intervention), nombreInterventions, fichier);
    fclose(fichier);

    FILE *fid = fopen(FICHIER_ID_INTERVENTION, "rb");
    if (fid != NULL)
    {
        fread(&dernierIDIntervention, sizeof(int), 1, fid);
        fclose(fid);
    }
}

// ================= PRISES EN CHARGE =================
void sauvegarderPrisesEnCharge()
{
    FILE *fichier = fopen(FICHIER_PEC, "wb");
    if (fichier == NULL) return;

    fwrite(&nombrePrisesEnCharge, sizeof(int), 1, fichier);
    fwrite(prisesEnCharge, sizeof(PriseEnCharge), nombrePrisesEnCharge, fichier);
    fclose(fichier);

    FILE *fid = fopen(FICHIER_ID_PEC, "wb");
    if (fid != NULL)
    {
        fwrite(&dernierIDPriseEnCharge, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void chargerPrisesEnCharge()
{
    FILE *fichier = fopen(FICHIER_PEC, "rb");
    if (fichier == NULL) return;

    fread(&nombrePrisesEnCharge, sizeof(int), 1, fichier);
    fread(prisesEnCharge, sizeof(PriseEnCharge), nombrePrisesEnCharge, fichier);
    fclose(fichier);

    FILE *fid = fopen(FICHIER_ID_PEC, "rb");
    if (fid != NULL)
    {
        fread(&dernierIDPriseEnCharge, sizeof(int), 1, fid);
        fclose(fid);
    }
}

// ================= ARCHIVES =================
void sauvegarderArchives()
{
    FILE *fichier = fopen(FICHIER_ARCHIVES, "wb");
    if (fichier == NULL) return;

    fwrite(&nombreArchives, sizeof(int), 1, fichier);
    fwrite(archives, sizeof(Archive), nombreArchives, fichier);
    fclose(fichier);

    FILE *fid = fopen(FICHIER_ID_ARCHIVE, "wb");
    if (fid != NULL)
    {
        fwrite(&dernierIDArchive, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void chargerArchives()
{
    FILE *fichier = fopen(FICHIER_ARCHIVES, "rb");
    if (fichier == NULL) return;

    fread(&nombreArchives, sizeof(int), 1, fichier);
    fread(archives, sizeof(Archive), nombreArchives, fichier);
    fclose(fichier);

    FILE *fid = fopen(FICHIER_ID_ARCHIVE, "rb");
    if (fid != NULL)
    {
        fread(&dernierIDArchive, sizeof(int), 1, fid);
        fclose(fid);
    }
}
