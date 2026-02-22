#ifndef ARCHIVES_H
#define ARCHIVES_H

#include "utils.h"
#include "patient.h"
#include "personnel.h"
#include "consultations.h"
#include "ordonnances.h"
#include "interventions.h"

#define MAX_ARCHIVES 10000

// Structure pour l'archivage
typedef struct
{
    int id_archive;
    char type_archive[30];           // "Patient", "Consultation", "Ordonnance", etc.
    int id_element;                   // ID de l'élément archivé
    char date_archivage[20];
    char raison[200];
    char responsable_archivage[100];
    char chemin_fichier[300];         // Si exporté dans un fichier séparé
    char donnees_sauvegardees[5000];   // Résumé des données
    int retention_ans;                 // Années de conservation
    char statut[20];                    // "Actif", "Archivé", "Purgé"
} Archive;

// Variables globales
extern Archive archives[MAX_ARCHIVES];
extern int nombreArchives;
extern int dernierIDArchive;

// Prototypes
void initialiserArchives();
void archiverPatient(int id_patient);
void archiverConsultation(int id_consultation);
void archiverOrdonnance(int id_ordonnance);
void archiverIntervention(int id_intervention);
void archiverDonneesAnciennes(int jours_anciennete);
void rechercherDansArchives(char *mot_cle);
void restaurerArchive(int index);
void purgerArchives(int jours_conservation);
void exporterArchiveTexte(int index);
void menuArchives();

#endif
