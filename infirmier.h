#ifndef INFIRMIER_H
#define INFIRMIER_H

#include "utils.h"
#include "patient.h"
#include "personnel.h"
#include "consultations.h"
#include "interventions.h"

#define MAX_SOINS 5000
#define MAX_CONSTANTES 1000
#define MAX_PRESCRIPTION_SOIN 500

// Structure pour les constantes vitales
typedef struct
{
    int id_constante;
    int id_patient;
    int id_infirmier;
    char date_mesure[20];
    char heure_mesure[10];
    float temperature;      // °C
    int tension_systolique; // mmHg
    int tension_diastolique; // mmHg
    int pouls;              // battements/min
    int saturation;         // % oxygène
    float poids;            // kg
    float taille;           // cm
    float glycemie;         // g/L
    char notes[200];
} ConstanteVital;

// Structure pour les soins à administrer
typedef struct
{
    int id_soin;
    int id_patient;
    int id_medecin_prescripteur;
    int id_infirmier_realisateur;
    char date_prescription[20];
    char date_realisation[20];
    char type_soin[100];     // Pansement, Injection, Prise de sang, etc.
    char description[300];
    char posologie[200];
    char frequence[100];      // Matin/Soir, Toutes les X heures
    char statut[20];          // "Prescrit", "En attente", "Réalisé", "Reporté"
    char priorite[20];        // "Normale", "Urgente", "Très urgente"
    char notes[300];
} Soin;

// Structure pour le planning des soins
typedef struct
{
    int id_planning;
    int id_soin;
    char date_prevue[20];
    char heure_prevue[10];
    int realise;              // 0 = non, 1 = oui
    char observations[300];
} PlanningSoin;

// Variables globales
extern ConstanteVital constantes[MAX_CONSTANTES];
extern int nombreConstantes;
extern int dernierIDConstante;
extern Soin soins[MAX_SOINS];
extern int nombreSoins;
extern int dernierIDSoin;
extern PlanningSoin plannings[MAX_SOINS];
extern int nombrePlannings;
extern int dernierIDPlanning;

// Prototypes - Constantes vitales
void initialiserInfirmier();
void saisirConstantesVitales(int id_patient);
void afficherHistoriqueConstantes(int id_patient);
void afficherDernieresConstantes(int id_patient);
void modifierConstantes(int id_constante);

// Prototypes - Soins
void prescrireSoin(int id_patient, int id_medecin);
void modifierPrescriptionSoin(int id_soin);
void annulerPrescriptionSoin(int id_soin);
void afficherSoinsAPreparer();
void afficherSoinsARealiser();
void realiserSoin(int id_soin);
void reporterSoin(int id_soin);
void afficherSoinsParPatient(int id_patient);
void afficherSoinsParMedecin(int id_medecin);

// Prototypes - Planning
void planifierSoin(int id_soin);
void afficherPlanningInfirmier(int id_infirmier, char *date);
void afficherPlanningSemaine(int id_infirmier, char *date_debut);
void menuPlanningInfirmier();
void afficherPlanningInfirmierJour(int id_infirmier, char *date);

// Prototypes - Statistiques
void statistiquesInfirmier();

// Prototypes - Menu
void menuInfirmier();
void menuSoinsInfirmier();
void menuConstantesInfirmier();

// Sauvegarde
void sauvegarderConstantes();
void chargerConstantes();
void sauvegarderSoins();
void chargerSoins();
void sauvegarderPlannings();
void chargerPlannings();

#endif
