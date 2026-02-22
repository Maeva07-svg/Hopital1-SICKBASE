#ifndef INTERVENTIONS_H
#define INTERVENTIONS_H

#include "utils.h"
#include "patient.h"
#include "personnel.h"

#define MAX_INTERVENTIONS 2000
#define MAX_EQUIPE 20
#define MAX_MATERIEL 50

// Structure pour une intervention chirurgicale
typedef struct
{
    int id_intervention;
    int id_patient;
    int id_chirurgien;
    int id_anesthesiste;
    int ids_infirmiers[MAX_EQUIPE];
    int nb_infirmiers;
    char date_intervention[20];
    char heure_debut[10];
    char heure_fin[10];
    char type_intervention[100];
    char description[500];
    char salle_operation[30];
    char materiel_utilise[MAX_MATERIEL][100];
    int nb_materiel;
    char protocole_anesthesie[300];
    char compte_rendu_operatoire[1000];
    char prescriptions_post_op[500];
    char date_sortie[20];
    char statut[30];                 // "Planifiée", "En cours", "Terminée", "Reportée"
    char niveau_urgence[20];          // "Faible", "Moyen", "Urgent", "Très urgent"
    float cout_estime;
    char notes[500];
} Intervention;

// Structure pour une prise en charge
typedef struct
{
    int id_prise_en_charge;
    int id_patient;
    int ids_medecins[MAX_EQUIPE];
    int nb_medecins;
    int ids_infirmiers[MAX_EQUIPE];
    int nb_infirmiers;
    char date_debut[20];
    char date_fin[20];
    char type_pec[50];                // "Hospitalisation", "Soins intensifs", "Réa", "Soins palliatifs"
    char service[50];
    char motif[200];
    char evolution[500];
    char traitements_administres[500];
    char examens_realises[300];
    char date_sortie_prevue[20];
    char date_sortie_reelle[20];
    char statut[30];                   // "En cours", "Terminée", "Transférée"
    float cout_journalier;
    char notes[500];
} PriseEnCharge;

// Variables globales
extern Intervention interventions[MAX_INTERVENTIONS];
extern int nombreInterventions;
extern int dernierIDIntervention;
extern PriseEnCharge prisesEnCharge[MAX_INTERVENTIONS];
extern int nombrePrisesEnCharge;
extern int dernierIDPriseEnCharge;

// Prototypes
void initialiserInterventions();
void planifierIntervention();
void ajouterIntervention();
void modifierIntervention(int index);
void afficherIntervention(int index);
void afficherInterventionsPatient(int id_patient);
void afficherInterventionsChirurgien(int id_chirurgien);
void commencerIntervention(int index);
void terminerIntervention(int index);
void reporterIntervention(int index);
void ajouterCompteRendu(int index);
void menuInterventions();

void demarrerPriseEnCharge();
void modifierPriseEnCharge(int index);
void afficherPriseEnCharge(int index);
void afficherPrisesEnChargePatient(int id_patient);
void terminerPriseEnCharge(int index);
void menuPrisesEnCharge();

#endif
