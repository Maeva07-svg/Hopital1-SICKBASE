#ifndef FACTURATION_H
#define FACTURATION_H

#include "utils.h"
#include "patient.h"
#include "personnel.h"
#include "consultations.h"

#define MAX_FACTURES 5000
#define MAX_LIGNES_FACTURE 20

// Structure pour une ligne de facture (détail)
typedef struct {
    char description[200];
    int quantite;
    float prix_unitaire;
    float montant;
} LigneFacture;

// Structure pour une facture
typedef struct {
    int id_facture;
    int id_patient;
    int id_medecin;
    int id_consultation;      // Consultation associée (0 si non)
    char numero_facture[20];   // Format: FAC-YYYY-XXXX
    char date_emission[20];
    char date_echeance[20];
    char date_paiement[20];

    LigneFacture lignes[MAX_LIGNES_FACTURE];
    int nb_lignes;

    float sous_total;
    float remise;              // Pourcentage de remise
    float montant_remise;
    float tva;                 // Pourcentage TVA
    float montant_tva;
    float montant_total;
    float montant_pris_en_charge;  // Part assurance
    float montant_restant;

    char assurance[100];
    char numero_contrat[50];
    char mode_paiement[30];    // Espèces, Carte, Virement, etc.
    char statut[20];           // "En attente", "Payée", "Impayée", "Annulée"
    char notes[300];

    char date_creation[20];
    char date_modification[20];
} Facture;

// Variables globales
extern Facture factures[MAX_FACTURES];
extern int nombreFactures;
extern int dernierIDFacture;

// Prototypes
void initialiserFacturation();
void genererNumeroFacture(int id_facture, char *numero);

// Création et gestion
void creerFactureConsultation(int id_consultation);
void creerFacturePersonnalisee();
void ajouterLigneFacture(int id_facture);
void modifierFacture(int id_facture);
void annulerFacture(int id_facture);

// Consultation
void afficherFacture(int id_facture);
void afficherFacturesPatient(int id_patient);
void afficherFacturesEnAttente();
void afficherFacturesImpayees();
void rechercherFacture(char *numero);

// Paiement
void enregistrerPaiement(int id_facture);
void calculerMontants(Facture *f);

// Statistiques
void statistiquesFacturation();
void chiffreAffairesParPeriode(char *date_debut, char *date_fin);

// Menu principal
void menuFacturation();

// Sauvegarde
void sauvegarderFactures();
void chargerFactures();

#endif
