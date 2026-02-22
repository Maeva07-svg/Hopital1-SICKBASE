#ifndef CONSULTATIONS_H
#define CONSULTATIONS_H

#include "utils.h"
#include "patient.h"
#include "personnel.h"

#define MAX_CONSULTATIONS 5000
#define MAX_MOTIF 200
#define MAX_OBSERVATIONS 500

// Structure pour une consultation
typedef struct
{
    int id_consultation;
    int id_patient;
    int id_medecin;
    char date_consultation[20];
    char heure_consultation[10];
    char motif[MAX_MOTIF];
    char observations[MAX_OBSERVATIONS];
    char diagnostic[200];
    char prescriptions[500];      // Liste des médicaments prescrits
    char examens_demandes[300];    // Examens complémentaires
    char prochain_rdv[20];         // Date du prochain rendez-vous
    int duree_minutes;              // Durée de la consultation
    char statut[20];                // "Planifié", "Effectué", "Annulé", "Reporté"
    float cout;                      // Coût de la consultation
    char notes[500];
} Consultation;

// Structure pour un rendez-vous
typedef struct
{
    int id_rdv;
    int id_patient;
    int id_medecin;
    char date[20];
    char heure[10];
    char type_rdv[50];              // "Consultation", "Suivi", "Urgence", "Contrôle"
    char motif[MAX_MOTIF];
    char statut[20];                 // "Confirmé", "En attente", "Annulé", "Reporté"
    char rappel_envoye[5];           // "Oui"/"Non"
    char notes[200];
} RendezVous;

// Variables globales
extern Consultation consultations[MAX_CONSULTATIONS];
extern int nombreConsultations;
extern int dernierIDConsultation;
extern RendezVous rendezVous[MAX_CONSULTATIONS];
extern int nombreRendezVous;
extern int dernierIDRendezVous;

// Prototypes
void initialiserConsultations();
void planifierConsultation();
void ajouterConsultation(int id_patient, int id_medecin);
void modifierConsultation(int index);
void annulerConsultation(int index);
void consulterConsultationsPatient(int id_patient);
void consulterConsultationsMedecin(int id_medecin);
void consulterConsultationsDate(char *date);
void afficherDetailsConsultation(int index);
void planifierRendezVous();
void confirmerRendezVous(int index);
void annulerRendezVous(int index);
void listerRendezVousJour(char *date);
void listerRendezVousMedecin(int id_medecin);
void menuConsultations();
void menuRendezVous();

#endif
