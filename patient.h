#ifndef PATIENT_H_INCLUDED
#define PATIENT_H_INCLUDED
#include "utils.h"

// Structure Patient
typedef struct
{
    int id;
    char nom[MAX_NAME];
    char prenom[MAX_NAME];
    int age;
    char genre;
    char telephone[20];
    char email[100];
    char adresse[200];
    char date_naissance[20];
    char groupe_sanguin[5];
    char symptomes[MAX_SYMPTOMS][MAX_SYMPTOM_LENGTH];
    int nb_symptomes;
    char diagnostic[200];
    char medecin_traitant[MAX_NAME];
    char date_admission[20];
    char notes[500];
} Patient;

// Variables globales (déclarées comme extern)
extern Patient patients[MAX_PATIENTS];
extern int nombrePatients;
extern int dernierID;

// Prototypes des fonctions patients
void ajouterNouveauPatient();
void afficherListePatientsSimple();
void afficherDetailsCompletsPatient(int index);
int rechercherParID(int id);
void rechercherPatient();
void modifierPatient(int index);
void supprimerPatient(int index);
void menuAffichagePatients();
void menuGestionPatients();
void afficherStatistiques();

#endif // PATIENT_H_INCLUDED
