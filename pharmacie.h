#ifndef PHARMACIE_H
#define PHARMACIE_H

#include "utils.h"
#include "patient.h"
#include "ordonnances.h"

#define MAX_MEDICAMENTS_STOCK 2000
#define MAX_NOM_MEDICAMENT 100
#define MAX_FOURNISSEUR 100
#define MAX_CATEGORIE 50
#define MAX_FORME 30

// Structure pour un médicament en stock (indépendante de celle dans ordonnances.h)
typedef struct
{
    int id;
    char nom[MAX_NOM_MEDICAMENT];
    char categorie[MAX_CATEGORIE];
    char forme[MAX_FORME];
    char dosage[50];
    int quantite_stock;
    int seuil_alerte;
    float prix_unitaire;
    float prix_rembourse;
    char date_peremption[20];
    char fournisseur[MAX_FOURNISSEUR];
    int ordonnance_requise;
    char notes[200];
    int actif;
} MedicamentStock;

// Structure pour une vente/délivrance
typedef struct
{
    int id_vente;
    int id_medicament;
    int id_patient;
    int id_ordonnance;
    int quantite;
    float prix_total;
    char date_vente[20];
    char statut[20];
    char notes[100];
} VenteMedicament;

// Variables globales
extern MedicamentStock medicaments[MAX_MEDICAMENTS_STOCK];
extern int nombreMedicaments;
extern int dernierIDMedicament;
extern VenteMedicament ventes[MAX_MEDICAMENTS_STOCK];
extern int nombreVentes;
extern int dernierIDVente;

// Prototypes - Gestion des médicaments
void initialiserPharmacie();
void ajouterMedicament(int index_ordonnance);
void modifierMedicament(int id);
void supprimerMedicament(int id);
void afficherStock();
void afficherStockComplet();
void rechercherMedicament(char *nom);
void alerteStockFaible();
void approvisionnerMedicament(int id);
void modifierSeuilAlerte(int id);

// Prototypes - Délivrance
void delivrerMedicament(int id_ordonnance, int id_patient);
void enregistrerVente(int id_medicament, int id_patient, int quantite);
void afficherVentesParPatient(int id_patient);
void afficherVentesParMedicament(int id_medicament);
void statistiquesPharmacie();

// Prototypes - Menu
void menuPharmacien();
void menuGestionStock();

// Sauvegarde
void sauvegarderMedicaments();
void chargerMedicaments();
void sauvegarderVentes();
void chargerVentes();

#endif
