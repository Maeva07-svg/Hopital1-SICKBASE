#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "comptes.h"
#include "utils.h"
#include "patient.h"
#include "personnel.h"

// Définition des variables globales
CompteUtilisateur comptes[MAX_COMPTES];
int nombreComptes = 0;
int dernierIDCompte = 0;
CompteUtilisateur *utilisateur_actuel = NULL;

void initialiserComptes()
{
    // Charger les comptes existants
    chargerComptes();

    // Si aucun compte, créer le compte super admin par défaut
    if (nombreComptes == 0)
    {
        creerCompteAdministrateur();
        printf("Compte administrateur par defaut cree.\n");
        printf("Login: admin | Mot de passe: admin123\n");
    }
}

void creerCompteAdministrateur()
{
    CompteUtilisateur compte;
    memset(&compte, 0, sizeof(CompteUtilisateur));

    compte.id_compte = ++dernierIDCompte;
    strcpy(compte.login, "admin");
    strcpy(compte.mot_de_passe, "admin123");  // En production, hacher
    compte.profil = ROLE_SUPER_ADMIN;
    compte.id_associe = 0;
    compte.actif = 1;
    compte.tentative_echec = 0;
    compte.bloque = 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(compte.date_creation, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    comptes[nombreComptes] = compte;
    nombreComptes++;

    sauvegarderComptes();
}

void creerComptePatient(int id_patient)
{
    if (id_patient <= 0) return;

    // Vérifier si un compte existe déjà
    for (int i = 0; i < nombreComptes; i++)
    {
        if (comptes[i].profil == ROLE_PATIENT && comptes[i].id_associe == id_patient)
        {
            printf("Un compte patient existe deja pour cet ID.\n");
            return;
        }
    }

    CompteUtilisateur compte;
    memset(&compte, 0, sizeof(CompteUtilisateur));

    compte.id_compte = ++dernierIDCompte;

    // Générer login automatique
    int idx_patient = rechercherParID(id_patient);
    if (idx_patient != -1)
    {
        sprintf(compte.login, "patient_%d_%s", id_patient, patients[idx_patient].nom);
    }
    else
    {
        sprintf(compte.login, "patient_%d", id_patient);
    }

    // Mot de passe par défaut
    strcpy(compte.mot_de_passe, "patient123");
    compte.profil = ROLE_PATIENT;
    compte.id_associe = id_patient;
    compte.actif = 1;
    compte.tentative_echec = 0;
    compte.bloque = 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(compte.date_creation, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    comptes[nombreComptes] = compte;
    nombreComptes++;

    sauvegarderComptes();

    printf("Compte patient cree avec succes!\n");
    printf("Login: %s\n", compte.login);
    printf("Mot de passe par defaut: patient123\n");
}

void creerComptePersonnel(int id_personnel, ProfilUtilisateur profil)
{
    if (id_personnel <= 0) return;

    // Vérifier si un compte existe déjà
    for (int i = 0; i < nombreComptes; i++)
    {
        if (comptes[i].id_associe == id_personnel &&
            (comptes[i].profil == ROLE_MEDECIN ||
             comptes[i].profil == ROLE_INFIRMIER ||
             comptes[i].profil == ROLE_TECHNICIEN_LABO ||
             comptes[i].profil == ROLE_PHARMACIEN))
        {
            printf("Un compte personnel existe deja.\n");
            return;
        }
    }

    CompteUtilisateur compte;
    memset(&compte, 0, sizeof(CompteUtilisateur));

    compte.id_compte = ++dernierIDCompte;

    // Générer login automatique
    int idx_personnel = rechercherEmployeParID(id_personnel);
    if (idx_personnel != -1)
    {
        sprintf(compte.login, "%s_%d_%s",
                profil == ROLE_MEDECIN ? "medecin" :
                profil == ROLE_INFIRMIER ? "infirmier" :
                profil == ROLE_TECHNICIEN_LABO ? "technicien" : "pharmacien",
                id_personnel,
                personnel[idx_personnel].nom);
    }
    else
    {
        sprintf(compte.login, "user_%d", id_personnel);
    }

    strcpy(compte.mot_de_passe, "change_me");
    compte.profil = profil;
    compte.id_associe = id_personnel;
    compte.actif = 1;
    compte.tentative_echec = 0;
    compte.bloque = 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(compte.date_creation, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    comptes[nombreComptes] = compte;
    nombreComptes++;

    sauvegarderComptes();

    printf("Compte personnel cree avec succes!\n");
    printf("Login: %s\n", compte.login);
    printf("Mot de passe par defaut: change_me\n");
}

int login()
{
    system("cls");
    color(11, 0);
    printf("\n========================================\n");
    printf("         AUTHENTIFICATION               \n");
    printf("========================================\n\n");
    color(7, 0);

    char login[MAX_LOGIN];
    char mdp[MAX_MDP];

    printf("Login: ");
    fgets(login, MAX_LOGIN, stdin);
    login[strcspn(login, "\n")] = '\0';

    printf("Mot de passe: ");
    // Masquage du mot de passe (optionnel)
    fgets(mdp, MAX_MDP, stdin);
    mdp[strcspn(mdp, "\n")] = '\0';

    // Rechercher le compte
    for (int i = 0; i < nombreComptes; i++)
    {
        if (strcmp(comptes[i].login, login) == 0)
        {
            // Vérifier si le compte est bloqué
            if (comptes[i].bloque)
            {
                printf("\nCompte bloque. Contactez l'administrateur.\n");
                pause();
                return 0;
            }

            // Vérifier si actif
            if (!comptes[i].actif)
            {
                printf("\nCompte desactive.\n");
                pause();
                return 0;
            }

            // Vérifier mot de passe
            if (strcmp(comptes[i].mot_de_passe, mdp) == 0)
            {
                // Connexion réussie
                comptes[i].tentative_echec = 0;
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                sprintf(comptes[i].date_derniere_connexion, "%02d/%02d/%04d %02d:%02d",
                        tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min);

                journaliserConnexion(comptes[i].id_compte, 1);
                utilisateur_actuel = &comptes[i];
                sauvegarderComptes();

                printf("\nConnexion reussie! Bienvenue %s.\n", login);
                pause();
                return 1;
            }
            else
            {
                // Échec de connexion
                comptes[i].tentative_echec++;
                if (comptes[i].tentative_echec >= 3)
                {
                    comptes[i].bloque = 1;
                    printf("\nTrop de tentatives. Compte bloque.\n");
                }
                else
                {
                    printf("\nMot de passe incorrect. Tentative %d/3.\n",
                           comptes[i].tentative_echec);
                }
                journaliserConnexion(comptes[i].id_compte, 0);
                sauvegarderComptes();
                pause();
                return 0;
            }
        }
    }

    printf("\nLogin ou mot de passe incorrect.\n");
    pause();
    return 0;
}

void logout()
{
    if (utilisateur_actuel != NULL)
    {
        printf("Deconnexion de %s...\n", utilisateur_actuel->login);
        utilisateur_actuel = NULL;
        pause();
    }
}

void modifierMotDePasse(int id_compte)
{
    for (int i = 0; i < nombreComptes; i++)
    {
        if (comptes[i].id_compte == id_compte)
        {
            char ancien_mdp[MAX_MDP];
            char nouveau_mdp[MAX_MDP];
            char confirmation[MAX_MDP];

            printf("Ancien mot de passe: ");
            fgets(ancien_mdp, MAX_MDP, stdin);
            ancien_mdp[strcspn(ancien_mdp, "\n")] = '\0';

            if (strcmp(comptes[i].mot_de_passe, ancien_mdp) != 0)
            {
                printf("Ancien mot de passe incorrect.\n");
                return;
            }

            printf("Nouveau mot de passe: ");
            fgets(nouveau_mdp, MAX_MDP, stdin);
            nouveau_mdp[strcspn(nouveau_mdp, "\n")] = '\0';

            printf("Confirmer: ");
            fgets(confirmation, MAX_MDP, stdin);
            confirmation[strcspn(confirmation, "\n")] = '\0';

            if (strcmp(nouveau_mdp, confirmation) == 0)
            {
                strcpy(comptes[i].mot_de_passe, nouveau_mdp);
                sauvegarderComptes();
                printf("Mot de passe modifie avec succes.\n");
            }
            else
            {
                printf("Les mots de passe ne correspondent pas.\n");
            }
            return;
        }
    }
}

void journaliserConnexion(int id_compte, int succes)
{
    for (int i = 0; i < nombreComptes; i++)
    {
        if (comptes[i].id_compte == id_compte && comptes[i].nb_connexions < MAX_HISTORIQUE_CONNEXION)
        {
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            char log[30];
            sprintf(log, "%02d/%02d/%02d %02d:%02d %s",
                    tm.tm_mday, tm.tm_mon+1, tm.tm_year%100,
                    tm.tm_hour, tm.tm_min,
                    succes ? "OK" : "ECHEC");
            strcpy(comptes[i].historique_connexions[comptes[i].nb_connexions], log);
            comptes[i].nb_connexions++;
            return;
        }
    }
}
