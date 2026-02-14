#ifndef DIAGNOSTIC_H_INCLUDED
#define DIAGNOSTIC_H_INCLUDED


#include "utils.h"
#include "patient.h"

// Structure pour une maladie
typedef struct
{
    char nom[100];
    char symptomes[MAX_SYMPTOMES_PAR_MALADIE][MAX_SYMPTOM_LENGTH];
    int nb_symptomes;
    char description[300];
    char traitement[300];
    char precautions[200];
} Maladie;

// Variables globales
extern Maladie maladies[MAX_MALADIES];
extern int nombreMaladies;

// Prototypes
void initialiserMaladies();
void diagnosticParSymptomes();
void diagnosticPatientExistant(int index_patient);
void afficherToutesMaladies();
void menuDiagnosticMedical();


#endif // DIAGNOSTIC_H_INCLUDED
