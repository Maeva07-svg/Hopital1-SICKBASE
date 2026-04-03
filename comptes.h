#ifndef COMPTES_H
#define COMPTES_H

#include "utils.h"
#include "patient.h"
#include "personnel.h"

#define MAX_COMPTES 2000
#define MAX_LOGIN 50
#define MAX_MDP 100
#define MAX_HISTORIQUE_CONNEXION 100

// Types de profils
typedef enum {
    ROLE_PATIENT = 1,
    ROLE_MEDECIN,
    ROLE_INFIRMIER,
    ROLE_ADMINISTRATEUR,
    ROLE_TECHNICIEN_LABO,
    ROLE_PHARMACIEN,
    ROLE_SUPER_ADMIN
} ProfilUtilisateur;

// Structure pour un compte utilisateur
typedef struct {
    int id_compte;
    char login[MAX_LOGIN];
    char mot_de_passe[MAX_MDP];
    ProfilUtilisateur profil;
    int id_associe;
    int actif;
    char date_creation[20];
    char date_derniere_connexion[20];
    int tentative_echec;
    int bloque;
    char historique_connexions[MAX_HISTORIQUE_CONNEXION][30];
    int nb_connexions;
    char notes[200];
} CompteUtilisateur;

// Variables globales
extern CompteUtilisateur comptes[MAX_COMPTES];
extern int nombreComptes;
extern int dernierIDCompte;
extern CompteUtilisateur *utilisateur_actuel;

// Prototypes - Authentification
void initialiserComptes();
void creerCompteAdministrateur();
void creerComptePatient(int id_patient);
void creerComptePersonnel(int id_personnel, ProfilUtilisateur profil);
int login();
void logout();
void modifierMotDePasse(int id_compte);
void desactiverCompte(int id_compte);
void activerCompte(int id_compte);
void afficherHistoriqueConnexions(int id_compte);
void journaliserConnexion(int id_compte, int succes);
void gererComptes();
void afficherMenuParProfil();
int verifierDroit(const char *action);

// Fonctions pour les fichiers
void sauvegarderComptes();
void chargerComptes();
void sauvegarderDernierIDCompte();
void chargerDernierIDCompte();

#endif
