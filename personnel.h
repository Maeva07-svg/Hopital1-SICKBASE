#ifndef PERSONNEL_H
#define PERSONNEL_H

#include "utils.h"

// Constantes pour le personnel
#define MAX_PERSONNEL 500
#define MAX_SERVICE 50
#define MAX_SPECIALITE 100
#define MAX_DIPLOME 200
#define MAX_HORAIRES 50

// Structure pour un employé
typedef struct
{
    int id;
    char nom[MAX_NAME];
    char prenom[MAX_NAME];
    char date_naissance[20];
    char genre;
    char telephone[20];
    char email[100];
    char adresse[200];

    // Informations professionnelles
    char fonction[50];           // Médecin, Infirmier, Administratif, etc.
    char specialite[MAX_SPECIALITE];
    char service[MAX_SERVICE];
    char diplomes[MAX_DIPLOME];
    char numero_ordre[30];        // Numéro d'inscription à l'ordre des médecins
    char date_embauche[20];
    float salaire;
    int heures_semaine;

    // Disponibilités
    char horaires_travail[MAX_HORAIRES];
    int jours_conges_annuels;
    int jours_conges_restants;

    // Statut
    int actif;                    // 1 = actif, 0 = inactif
    char notes[500];
} Employe;

// Énumération pour les fonctions
typedef enum {
    MEDECIN,
    INFIRMIER,
    ADMINISTRATIF,
    TECHNICIEN,
    CHIRURGIEN,
    RADIOLOGUE,
    PHARMACIEN,
    AUTRE
} FonctionPersonnel;

// Variables globales (extern)
extern Employe personnel[MAX_PERSONNEL];
extern int nombreEmployes;
extern int dernierIDEmploye;

// ==================== FONCTIONS EXISTANTES ====================
void initialiserFonctionsPersonnel();
void ajouterEmploye();
void afficherListeEmployesSimple();
void afficherDetailsCompletEmploye(int index);
int rechercherEmployeParID(int id);
int rechercherEmployeParNom(char *nom);
void rechercherEmploye();
void modifierEmploye(int index);
void supprimerEmploye(int index);
void afficherStatistiquesPersonnel();
void menuGestionPersonnel();
void menuAffichagePersonnel();
void gererConges(int index);
void afficherEmployesParService(char *service);
void afficherEmployesParFonction(char *fonction);

// ==================== FONCTIONS UI POUR PERSONNEL ====================
void ui_afficherEcranPersonnel();           // Affiche le menu de gestion du personnel
void ui_afficherListePersonnel();           // Affiche la liste complète du personnel
void ui_afficherEcranAjouterEmploye();      // Affiche le formulaire d'ajout
void ui_afficherEcranModifierEmploye();     // Affiche le formulaire de modification
void ui_afficherEcranGererConges();         // Affiche l'écran de gestion des congés
void ui_afficherEcranStatistiquesPersonnel(); // Affiche les statistiques du personnel

#endif
