#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#include "utils.h"
#include "patient.h"

#define MAX_MALADIES 10
#define MAX_SYMPTOMES_PAR_MALADIE 10
#define MAX_QUESTIONS 20

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

// Structure pour une question de diagnostic
typedef struct
{
    char question[200];
    char mot_cle[50];           // Mot-clé associé à la question
    int id_maladie_associee;     // Maladie concernée
} QuestionDiagnostic;

// Structure pour un diagnostic médical personnalisé
typedef struct
{
    int id_diagnostic;
    int id_patient;
    int id_medecin;
    char date_diagnostic[20];
    char maladie[100];
    char description[500];
    char traitement_prescrit[500];
    char recommandations[500];
    char medicaments[300];
    char examens_complementaires[300];
    char prochain_rdv[20];
    char notes[300];
} DiagnosticMedical;

// Variables globales
extern Maladie maladies[MAX_MALADIES];
extern int nombreMaladies;
extern DiagnosticMedical diagnostics[MAX_PATIENTS * 10];
extern int nombreDiagnostics;
extern int dernierIDDiagnostic;

// Prototypes
void initialiserMaladies();
void initialiserQuestionsDiagnostic();
void diagnosticGuideParQuestions();
void diagnosticDirectParSymptomes();
void diagnosticPatientExistant(int index_patient);
void afficherToutesMaladies();
void menuDiagnosticMedical();

// Nouvelles fonctions pour diagnostic médical personnalisé
void creerDiagnosticPourPatient(int id_patient, int id_medecin);
void afficherDiagnosticsPatient(int id_patient);
void afficherDetailDiagnostic(int id_diagnostic);
void modifierDiagnostic(int id_diagnostic);
void prescrireTraitement(int id_diagnostic);
void menuDiagnosticMedicalAvance();

// Sauvegarde des diagnostics
void sauvegarderDiagnostics();
void chargerDiagnostics();

#endif
