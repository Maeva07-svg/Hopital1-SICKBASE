#ifndef PATIENT_H
#define PATIENT_H

#include "utils.h"

#define MAX_PATIENTS 1000
#define MAX_NAME 100
#define MAX_SYMPTOMS 20
#define MAX_SYMPTOM_LENGTH 100

// Structure Patient (inchangée - compatible avec anciens fichiers)
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

// Variables globales
extern Patient patients[MAX_PATIENTS];
extern int nombrePatients;
extern int dernierID;

// Prototypes existants
void ajouterNouveauPatient();
void afficherListePatientsSimple();
void afficherListePatientsPourMedecin(int id_medecin);
void afficherDetailsCompletsPatient(int index);
int rechercherParID(int id);
void rechercherPatient();
void modifierPatient(int index);
void supprimerPatient(int index);
void menuAffichagePatients();
void menuGestionPatients();
void afficherStatistiques();
int genererID();
void completerDossierPatient(int id_compte);
void questionnaireMedical(int id_patient);
int patientEstLieAuMedecin(int id_patient, int id_medecin);
void modifierMedecinTraitant(int id_patient);
int getMedecinIDByName(char *nom_medecin);

// ========== FONCTIONS UI POUR PATIENT (à implémenter dans patient.c) ==========
void ui_afficherEcranDossierMedical();      // Affiche le dossier médical du patient connecté
void ui_afficherEcranMesConsultations();    // Affiche les consultations du patient
void ui_afficherEcranMesOrdonnances();      // Affiche les ordonnances du patient
void ui_afficherEcranMesAnalyses();         // Affiche les analyses du patient
void ui_afficherEcranMesRendezVous();       // Affiche les rendez-vous du patient
void ui_afficherEcranMesFactures();         // Affiche les factures du patient

#endif
