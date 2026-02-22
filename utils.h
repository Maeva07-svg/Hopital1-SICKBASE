#ifndef UTILS_H
#define UTILS_H

// Constantes globales
#define MAX_PATIENTS 1000
#define MAX_PERSONNEL 500
#define MAX_CONSULTATIONS 5000
#define MAX_ORDONNANCES 3000
#define MAX_INTERVENTIONS 2000
#define MAX_ARCHIVES 10000

#define MAX_NAME 100
#define MAX_SYMPTOMS 20
#define MAX_SYMPTOM_LENGTH 100
#define MAX_MALADIES 10
#define MAX_SYMPTOMES_PAR_MALADIE 10

// Fichiers patients
#define FICHIER_PATIENTS "patients.dat"
#define FICHIER_ID "dernier_id.dat"

// Prototypes des fonctions utilitaires
void color(int couleurTexte, int couleurFond);
void viderBuffer();
void pause();
void clearInput();
int comparerSymptomes(char symptome1[], char symptome2[]);
void afficherTitre(char *titre);
void afficherSeparateur();

#endif
