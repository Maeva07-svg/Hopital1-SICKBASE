#ifndef ORDONNANCES_H
#define ORDONNANCES_H

#include "utils.h"
#include "patient.h"
#include "personnel.h"

#define MAX_ORDONNANCES 3000
#define MAX_MEDICAMENTS 20
#define MAX_POSOLOGIE 100

// Structure pour un médicament
typedef struct
{
    char nom[100];
    char dosage[50];
    char forme[50];               // Comprimé, sirop, injectable, etc.
    char posologie[MAX_POSOLOGIE];
    char duree_traitement[50];
    char instructions[200];
} Medicament;

// Structure pour une ordonnance
typedef struct
{
    int id_ordonnance;
    int id_patient;
    int id_medecin;
    int id_consultation;           // Lien avec la consultation
    char date_prescription[20];
    char date_debut[20];
    char date_fin[20];
    Medicament medicaments[MAX_MEDICAMENTS];
    int nb_medicaments;
    char instructions_generales[500];
    char diagnostic[200];
    int renouvelable;               // 0/1
    int nombre_renouvellements;
    char statut[20];                 // "Active", "Terminée", "Annulée"
    char notes[300];
} Ordonnance;

// Variables globales
extern Ordonnance ordonnances[MAX_ORDONNANCES];
extern int nombreOrdonnances;
extern int dernierIDOrdonnance;

// Prototypes
void initialiserOrdonnances();
void creerOrdonnance(int id_patient, int id_medecin, int id_consultation);
void ajouterMedicament();
void modifierOrdonnance(int index);
void afficherOrdonnance(int index);
void afficherOrdonnancesPatient(int id_patient);
void afficherOrdonnancesMedecin(int id_medecin);
void renouvelerOrdonnance(int index);
void terminerOrdonnance(int index);
void imprimerOrdonnance(int index);
void menuOrdonnances();

#endif
