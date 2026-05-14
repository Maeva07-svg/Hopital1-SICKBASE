#ifndef LABORATOIRE_H
#define LABORATOIRE_H

#include "utils.h"
#include "patient.h"
#include "personnel.h"

#define MAX_ANALYSES 5000
#define MAX_ANALYSES_PAR_PATIENT 50
#define MAX_RESULTAT 500
#define MAX_TYPE_ANALYSE 100

// Structure pour un type d'analyse
typedef struct
{
    int id_type;
    char nom[MAX_TYPE_ANALYSE];
    char description[300];
    char valeurs_normales[200];
    float prix;
    int duree_resultats;  // délai en heures
} TypeAnalyse;

// Structure pour une analyse demandée
typedef struct
{
    int id_analyse;
    int id_patient;
    int id_medecin;
    int id_type_analyse;
    char date_demande[20];
    char date_realisation[20];
    char date_resultat[20];
    char resultat[MAX_RESULTAT];
    char interpretation[300];
    char statut[20];        // "Demandee", "En cours", "Realisée", "Validee"
    char priorite[20];      // "Normale", "Urgente", "Tres urgente"
    char notes[300];
} Analyse;

// Variables globales
extern TypeAnalyse types_analyse[MAX_ANALYSES];
extern int nombreTypesAnalyse;
extern int dernierIDTypeAnalyse;
extern Analyse analyses[MAX_ANALYSES];
extern int nombreAnalyses;
extern int dernierIDAnalyse;

// Prototypes - Initialisation
void initialiserLaboratoire();
void initialiserTypesAnalyse();

// Prototypes - Gestion des types d'analyse (Admin uniquement)
void ajouterTypeAnalyse();
void modifierTypeAnalyse(int id_type);
void supprimerTypeAnalyse(int id_type);
void afficherTypesAnalyse();

// Prototypes - Demandes d'analyses (Médecin)
void demanderAnalyse(int id_patient, int id_medecin);
void modifierDemandeAnalyse(int id_analyse);
void annulerDemandeAnalyse(int id_analyse);
void afficherDemandesParMedecin(int id_medecin);
void afficherDemandesParPatient(int id_patient);  // Pour consultation

// Prototypes - Réalisation des analyses (Technicien uniquement)
void afficherAnalysesARealiser();
void commencerAnalyse(int id_analyse);
void saisirResultatAnalyse(int id_analyse);
void validerAnalyse(int id_analyse);
void afficherAnalyse(int id_analyse);
void afficherAnalysesEnCours();

// Prototypes - Consultation (Médecin, Patient, Admin)
void consulterResultatsPatient(int id_patient);
void afficherResultatsAnalyse(int id_analyse);

// Prototypes - Statistiques
void statistiquesLaboratoire();

// Prototypes - Menus spécifiques
void menuMedecinLaboratoire();      // Pour le médecin
void menuTechnicienLaboratoire();   // Pour le technicien
void menuAdminLaboratoire();        // Pour l'administrateur
void menuPatientLaboratoire();      // Pour le patient

// Sauvegarde
void sauvegarderTypesAnalyse();
void chargerTypesAnalyse();
void sauvegarderAnalyses();
void chargerAnalyses();

#endif
