#ifndef RDV_AVANCE_H
#define RDV_AVANCE_H

#include "utils.h"
#include "patient.h"
#include "personnel.h"
#include "consultations.h"

#define MAX_CRENEAUX 50
#define MAX_RDV_PAR_JOUR 30
#define MAX_JOURS 365
#define HEURE_DEBUT 8      // 8h00
#define HEURE_FIN 18       // 18h00
#define DUREE_CRENEAU 30   // 30 minutes par créneau

// Structure pour un créneau horaire
typedef struct {
    int id_creneau;
    char date[20];
    char heure_debut[10];
    char heure_fin[10];
    int id_medecin;
    int disponible;         // 1 = disponible, 0 = réservé
    int id_rdv;             // ID du rendez-vous si réservé
    char motif[100];
} Creneau;

// Structure pour un rendez-vous avancé
typedef struct {
    int id_rdv;
    int id_patient;
    int id_medecin;
    int id_creneau;
    char date[20];
    char heure[10];
    char motif[200];
    char type[30];
    char statut[20];
    char rappel_envoye[5];
    char notes[200];
    int duree;
    float cout;
    char date_creation[20];
} RendezVousAvance;

// Structure pour les disponibilités d'un médecin
typedef struct {
    int id_medecin;
    char date[20];
    int creneaux_disponibles[MAX_CRENEAUX];
    int nb_creneaux;
} Disponibilite;

// Variables globales
extern Creneau creneaux[MAX_CRENEAUX * MAX_JOURS];
extern int nombreCreneaux;
extern int dernierIDCreneau;
extern RendezVousAvance rendezVousAvance[MAX_RDV_PAR_JOUR * MAX_JOURS];
extern int nombreRdvAvance;
extern int dernierIDRdvAvance;
extern Disponibilite disponibilites[MAX_PERSONNEL * MAX_JOURS];
extern int nombreDisponibilites;

// Prototypes
void initialiserModuleRdv();
void genererCreneauxPourMedecin(int id_medecin, char *date);
void genererCreneauxSemaine(int id_medecin, char *date_debut);
void afficherPlanningMedecin(int id_medecin, char *date);
void afficherPlanningSemaine(int id_medecin, char *date_debut);
void prendreRendezVousAvance();
void annulerRendezVousAvance(int id_rdv);
void confirmerRendezVousAvance(int id_rdv);
void modifierRendezVousAvance(int id_rdv);
void listerRendezVousMedecinAvance(int id_medecin);      // Renommé
void listerRendezVousPatientAvance(int id_patient);      // Renommé
void listerRendezVousJourAvance(char *date);             // Renommé
void envoyerRappels();
void gererDisponibilitesMedecin(int id_medecin);
void statistiquesRendezVous();
void menuRendezVousAvance();

// Fonctions de fichiers
void sauvegarderCreneaux();
void chargerCreneaux();
void sauvegarderRendezVousAvance();
void chargerRendezVousAvance();

#endif
