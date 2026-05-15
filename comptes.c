#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "comptes.h"
#include "utils.h"
#include "patient.h"
#include "personnel.h"

// Définition des variables globales
CompteUtilisateur comptes[MAX_COMPTES];
int nombreComptes = 0;
int dernierIDCompte = 0;
CompteUtilisateur *utilisateur_actuel = NULL;

// Structure pour les droits (définie localement)
typedef struct {
    int peut_voir_patient;
    int peut_modifier_patient;
    int peut_voir_personnel;
    int peut_modifier_personnel;
    int peut_voir_consultations;
    int peut_creer_consultations;
    int peut_voir_ordonnances;
    int peut_prescrire;
    int peut_voir_interventions;
    int peut_planifier_interventions;
    int peut_voir_analyses;
    int peut_effectuer_analyses;
    int peut_gerer_stock;
    int peut_archiver;
    int peut_gerer_comptes;
    int peut_voir_statistiques;
} DroitsAcces;

// Fonction pour vérifier les droits
int verifierDroit(const char *action)
{
    if (utilisateur_actuel == NULL) return 0;

    DroitsAcces droits = {0};

    switch(utilisateur_actuel->profil)
    {
        case ROLE_PATIENT:
            droits.peut_voir_patient = 1;
            droits.peut_voir_ordonnances = 1;
            droits.peut_voir_consultations = 1;
            droits.peut_voir_analyses = 1;
            break;

        case ROLE_MEDECIN:
            droits.peut_voir_patient = 1;
            droits.peut_modifier_patient = 1;
            droits.peut_voir_consultations = 1;
            droits.peut_creer_consultations = 1;
            droits.peut_voir_ordonnances = 1;
            droits.peut_prescrire = 1;
            droits.peut_voir_interventions = 1;
            droits.peut_voir_analyses = 1;
            droits.peut_voir_statistiques = 1;
            break;

        case ROLE_INFIRMIER:
            droits.peut_voir_patient = 1;
            droits.peut_voir_consultations = 1;
            droits.peut_voir_ordonnances = 1;
            droits.peut_voir_interventions = 1;
            droits.peut_voir_analyses = 1;
            break;

        case ROLE_ADMINISTRATEUR:
            droits.peut_voir_patient = 1;
            droits.peut_modifier_patient = 1;
            droits.peut_voir_personnel = 1;
            droits.peut_modifier_personnel = 1;
            droits.peut_voir_consultations = 1;
            droits.peut_voir_ordonnances = 1;
            droits.peut_voir_interventions = 1;
            droits.peut_voir_analyses = 1;
            droits.peut_gerer_stock = 1;
            droits.peut_archiver = 1;
            droits.peut_gerer_comptes = 1;
            droits.peut_voir_statistiques = 1;
            break;

        case ROLE_TECHNICIEN_LABO:
            droits.peut_voir_patient = 1;
            droits.peut_voir_analyses = 1;
            droits.peut_effectuer_analyses = 1;
            break;

        case ROLE_PHARMACIEN:
            droits.peut_voir_ordonnances = 1;
            droits.peut_gerer_stock = 1;
            droits.peut_voir_patient = 1;
            break;

        case ROLE_SUPER_ADMIN:
            droits.peut_voir_patient = 1;
            droits.peut_modifier_patient = 1;
            droits.peut_voir_personnel = 1;
            droits.peut_modifier_personnel = 1;
            droits.peut_voir_consultations = 1;
            droits.peut_creer_consultations = 1;
            droits.peut_voir_ordonnances = 1;
            droits.peut_prescrire = 1;
            droits.peut_voir_interventions = 1;
            droits.peut_planifier_interventions = 1;
            droits.peut_voir_analyses = 1;
            droits.peut_effectuer_analyses = 1;
            droits.peut_gerer_stock = 1;
            droits.peut_archiver = 1;
            droits.peut_gerer_comptes = 1;
            droits.peut_voir_statistiques = 1;
            break;
    }

    if (strcmp(action, "voir_patient") == 0) return droits.peut_voir_patient;
    if (strcmp(action, "modifier_patient") == 0) return droits.peut_modifier_patient;
    if (strcmp(action, "voir_personnel") == 0) return droits.peut_voir_personnel;
    if (strcmp(action, "modifier_personnel") == 0) return droits.peut_modifier_personnel;
    if (strcmp(action, "voir_consultation") == 0) return droits.peut_voir_consultations;
    if (strcmp(action, "creer_consultation") == 0) return droits.peut_creer_consultations;
    if (strcmp(action, "voir_ordonnance") == 0) return droits.peut_voir_ordonnances;
    if (strcmp(action, "prescrire") == 0) return droits.peut_prescrire;
    if (strcmp(action, "voir_intervention") == 0) return droits.peut_voir_interventions;
    if (strcmp(action, "planifier_intervention") == 0) return droits.peut_planifier_interventions;
    if (strcmp(action, "voir_analyse") == 0) return droits.peut_voir_analyses;
    if (strcmp(action, "effectuer_analyse") == 0) return droits.peut_effectuer_analyses;
    if (strcmp(action, "gerer_stock") == 0) return droits.peut_gerer_stock;
    if (strcmp(action, "archiver") == 0) return droits.peut_archiver;
    if (strcmp(action, "gerer_comptes") == 0) return droits.peut_gerer_comptes;
    if (strcmp(action, "voir_statistiques") == 0) return droits.peut_voir_statistiques;

    return 0;
}

void initialiserComptes()
{
    chargerComptes();

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
    strcpy(compte.mot_de_passe, "admin123");
    compte.profil = ROLE_SUPER_ADMIN;
    compte.id_associe = 0;
    compte.actif = 1;
    compte.tentative_echec = 0;
    compte.bloque = 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(compte.date_creation, sizeof(compte.date_creation), "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    comptes[nombreComptes] = compte;
    nombreComptes++;

    sauvegarderComptes();
}

void creerComptePatient(int id_patient)
{
    if (id_patient <= 0) return;

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

    int idx_patient = rechercherParID(id_patient);
    if (idx_patient != -1)
    {
        snprintf(compte.login, sizeof(compte.login), "patient_%d_%s", id_patient, patients[idx_patient].nom);
    }
    else
    {
        snprintf(compte.login, sizeof(compte.login), "patient_%d", id_patient);
    }

    strcpy(compte.mot_de_passe, "patient123");
    compte.profil = ROLE_PATIENT;
    compte.id_associe = id_patient;
    compte.actif = 1;
    compte.tentative_echec = 0;
    compte.bloque = 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(compte.date_creation, sizeof(compte.date_creation), "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    comptes[nombreComptes] = compte;
    nombreComptes++;

    sauvegarderComptes();

    printf("Compte patient cree avec succes!\n");
    printf("Login: %s\n", compte.login);
    printf("Mot de passe par defaut: patient123\n");
}

// Création automatique d'un compte patient sans patient pré-existant
void creerComptePatientAuto()
{
    system("cls");
    printf("\n=== CREATION D'UN COMPTE PATIENT ===\n\n");

    CompteUtilisateur compte;
    memset(&compte, 0, sizeof(CompteUtilisateur));

    compte.id_compte = ++dernierIDCompte;
    compte.profil = ROLE_PATIENT;
    compte.actif = 1;
    compte.tentative_echec = 0;
    compte.bloque = 0;
    compte.compte_complet = 0;  // Compte incomplet, à compléter

    printf("CREATION DE VOTRE COMPTE\n");
    printf("------------------------\n");

    printf("Choisissez votre login: ");
    fgets(compte.login, MAX_LOGIN, stdin);
    compte.login[strcspn(compte.login, "\n")] = '\0';

    // Vérifier si le login existe déjà
    for (int i = 0; i < nombreComptes; i++)
    {
        if (strcmp(comptes[i].login, compte.login) == 0)
        {
            printf("Ce login existe deja. Veuillez en choisir un autre.\n");
            dernierIDCompte--;
            pause();
            return;
        }
    }

    printf("Choisissez votre mot de passe: ");
    fgets(compte.mot_de_passe, MAX_MDP, stdin);
    compte.mot_de_passe[strcspn(compte.mot_de_passe, "\n")] = '\0';

    printf("Confirmez votre mot de passe: ");
    char confirmation[MAX_MDP];
    fgets(confirmation, MAX_MDP, stdin);
    confirmation[strcspn(confirmation, "\n")] = '\0';

    if (strcmp(compte.mot_de_passe, confirmation) != 0)
    {
        printf("Les mots de passe ne correspondent pas.\n");
        dernierIDCompte--;
        pause();
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(compte.date_creation, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    // ID associé temporaire (sera mis à jour après création du dossier patient)
    compte.id_associe = 0;

    comptes[nombreComptes] = compte;
    nombreComptes++;

    sauvegarderComptes();

    printf("\n=== COMPTE CREE AVEC SUCCES ===\n");
    printf("Login: %s\n", compte.login);
    printf("Veuillez vous connecter pour completer votre dossier medical.\n");

    pause();
}

// Vérifier si un compte patient est complet
int estCompteComplet(int id_compte)
{
    for (int i = 0; i < nombreComptes; i++)
    {
        if (comptes[i].id_compte == id_compte && comptes[i].profil == ROLE_PATIENT)
        {
            return comptes[i].compte_complet;
        }
    }
    return 0;
}

// Marquer un compte comme complet
void marquerCompteComplet(int id_compte)
{
    for (int i = 0; i < nombreComptes; i++)
    {
        if (comptes[i].id_compte == id_compte)
        {
            comptes[i].compte_complet = 1;
            sauvegarderComptes();
            break;
        }
    }
}

void creerComptePersonnel(int id_personnel, ProfilUtilisateur profil)
{
    if (id_personnel <= 0) return;

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

    int idx_personnel = rechercherEmployeParID(id_personnel);
    if (idx_personnel != -1)
    {
        const char *prefixe = "";
        switch(profil)
        {
            case ROLE_MEDECIN: prefixe = "medecin"; break;
            case ROLE_INFIRMIER: prefixe = "infirmier"; break;
            case ROLE_TECHNICIEN_LABO: prefixe = "technicien"; break;
            case ROLE_PHARMACIEN: prefixe = "pharmacien"; break;
            default: prefixe = "user";
        }
        snprintf(compte.login, sizeof(compte.login), "%s_%d_%s", prefixe, id_personnel, personnel[idx_personnel].nom);
    }
    else
    {
        snprintf(compte.login, sizeof(compte.login), "user_%d", id_personnel);
    }

    strcpy(compte.mot_de_passe, "change_me");
    compte.profil = profil;
    compte.id_associe = id_personnel;
    compte.actif = 1;
    compte.tentative_echec = 0;
    compte.bloque = 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(compte.date_creation, sizeof(compte.date_creation), "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    comptes[nombreComptes] = compte;
    nombreComptes++;

    sauvegarderComptes();

    printf("Compte personnel cree avec succes!\n");
    printf("Login: %s\n", compte.login);
    printf("Mot de passe par defaut: change_me\n");
}

// ==================== FONCTION LOGIN EXISTANTE (CONSERVÉE) ====================
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
    fgets(mdp, MAX_MDP, stdin);
    mdp[strcspn(mdp, "\n")] = '\0';

    for (int i = 0; i < nombreComptes; i++)
    {
        if (strcmp(comptes[i].login, login) == 0)
        {
            if (comptes[i].bloque)
            {
                printf("\nCompte bloque. Contactez l'administrateur.\n");
                pause();
                return 0;
            }

            if (!comptes[i].actif)
            {
                printf("\nCompte desactive.\n");
                pause();
                return 0;
            }

            if (strcmp(comptes[i].mot_de_passe, mdp) == 0)
            {
                comptes[i].tentative_echec = 0;
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                snprintf(comptes[i].date_derniere_connexion, sizeof(comptes[i].date_derniere_connexion),
                         "%02d/%02d/%04d %02d:%02d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min);

                journaliserConnexion(comptes[i].id_compte, 1);
                utilisateur_actuel = &comptes[i];
                sauvegarderComptes();

                printf("\nConnexion reussie! Bienvenue %s.\n", login);
                pause();
                return 1;
            }
            else
            {
                comptes[i].tentative_echec++;
                if (comptes[i].tentative_echec >= 3)
                {
                    comptes[i].bloque = 1;
                    printf("\nTrop de tentatives. Compte bloque.\n");
                }
                else
                {
                    printf("\nMot de passe incorrect. Tentative %d/3.\n", comptes[i].tentative_echec);
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

// ==================== NOUVELLE FONCTION POUR SDL2 ====================
int login_with_params(char* login, char* password)
{
    for (int i = 0; i < nombreComptes; i++)
    {
        if (strcmp(comptes[i].login, login) == 0)
        {
            if (comptes[i].bloque)
            {
                printf("Compte bloque. Contactez l'administrateur.\n");
                return 0;
            }

            if (!comptes[i].actif)
            {
                printf("Compte desactive.\n");
                return 0;
            }

            if (strcmp(comptes[i].mot_de_passe, password) == 0)
            {
                comptes[i].tentative_echec = 0;
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                snprintf(comptes[i].date_derniere_connexion, sizeof(comptes[i].date_derniere_connexion),
                         "%02d/%02d/%04d %02d:%02d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min);

                journaliserConnexion(comptes[i].id_compte, 1);
                utilisateur_actuel = &comptes[i];
                sauvegarderComptes();

                printf("Connexion reussie! Bienvenue %s.\n", login);
                return 1;
            }
            else
            {
                comptes[i].tentative_echec++;
                if (comptes[i].tentative_echec >= 3)
                {
                    comptes[i].bloque = 1;
                    printf("Trop de tentatives. Compte bloque.\n");
                }
                else
                {
                    printf("Mot de passe incorrect. Tentative %d/3.\n", comptes[i].tentative_echec);
                }
                journaliserConnexion(comptes[i].id_compte, 0);
                sauvegarderComptes();
                return 0;
            }
        }
    }

    printf("Login ou mot de passe incorrect.\n");
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

void desactiverCompte(int id_compte)
{
    for (int i = 0; i < nombreComptes; i++)
    {
        if (comptes[i].id_compte == id_compte)
        {
            comptes[i].actif = 0;
            sauvegarderComptes();
            printf("Compte desactive.\n");
            return;
        }
    }
}

void activerCompte(int id_compte)
{
    for (int i = 0; i < nombreComptes; i++)
    {
        if (comptes[i].id_compte == id_compte)
        {
            comptes[i].actif = 1;
            comptes[i].bloque = 0;
            comptes[i].tentative_echec = 0;
            sauvegarderComptes();
            printf("Compte active.\n");
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
            snprintf(log, sizeof(log), "%02d/%02d/%02d %02d:%02d %s",
                     tm.tm_mday, tm.tm_mon+1, tm.tm_year%100,
                     tm.tm_hour, tm.tm_min,
                     succes ? "OK" : "ECHEC");
            strcpy(comptes[i].historique_connexions[comptes[i].nb_connexions], log);
            comptes[i].nb_connexions++;
            return;
        }
    }
}

void afficherHistoriqueConnexions(int id_compte)
{
    for (int i = 0; i < nombreComptes; i++)
    {
        if (comptes[i].id_compte == id_compte)
        {
            printf("\nHistorique des connexions pour %s:\n", comptes[i].login);
            for (int j = 0; j < comptes[i].nb_connexions; j++)
            {
                printf("  %s\n", comptes[i].historique_connexions[j]);
            }
            if (comptes[i].nb_connexions == 0)
            {
                printf("  Aucune connexion enregistree.\n");
            }
            return;
        }
    }
}

void gererComptes()
{
    int choix;

    do
    {
        system("cls");
        color(12, 0);
        printf("\n=== GESTION DES COMPTES UTILISATEURS ===\n\n");
        color(7, 0);

        printf("1. Lister tous les comptes\n");
        printf("2. Creer un compte patient\n");
        printf("3. Creer un compte medecin\n");
        printf("4. Creer un compte infirmier\n");
        printf("5. Creer un compte technicien labo\n");
        printf("6. Creer un compte pharmacien\n");
        printf("7. Creer un compte administrateur\n");
        printf("8. Desactiver un compte\n");
        printf("9. Activer un compte\n");
        printf("10. Modifier mot de passe\n");
        printf("11. Afficher historique connexions\n");
        printf("12. Retour\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                system("cls");
                printf("\n=== LISTE DES COMPTES ===\n\n");
                printf("ID  | Login                 | Profil        | Associe | Actif | Bloque\n");
                printf("----|-----------------------|---------------|---------|-------|-------\n");
                for (int i = 0; i < nombreComptes; i++)
                {
                    const char *profil = "";
                    switch(comptes[i].profil)
                    {
                        case ROLE_PATIENT: profil = "Patient"; break;
                        case ROLE_MEDECIN: profil = "Medecin"; break;
                        case ROLE_INFIRMIER: profil = "Infirmier"; break;
                        case ROLE_ADMINISTRATEUR: profil = "Admin"; break;
                        case ROLE_TECHNICIEN_LABO: profil = "Technicien"; break;
                        case ROLE_PHARMACIEN: profil = "Pharmacien"; break;
                        case ROLE_SUPER_ADMIN: profil = "SuperAdmin"; break;
                    }
                    printf("%-4d| %-21s| %-13s| %-7d| %-5s| %s\n",
                           comptes[i].id_compte,
                           comptes[i].login,
                           profil,
                           comptes[i].id_associe,
                           comptes[i].actif ? "Oui" : "Non",
                           comptes[i].bloque ? "Oui" : "Non");
                }
                pause();
                break;

            case 2:
                {
                    int id;
                    afficherListePatientsSimple();
                    printf("ID Patient: ");
                    scanf("%d", &id);
                    viderBuffer();
                    creerComptePatient(id);
                    pause();
                }
                break;

            case 3:
                {
                    int id;
                    afficherListeEmployesSimple();
                    printf("ID Employe (medecin): ");
                    scanf("%d", &id);
                    viderBuffer();
                    creerComptePersonnel(id, ROLE_MEDECIN);
                    pause();
                }
                break;

            case 4:
                {
                    int id;
                    afficherListeEmployesSimple();
                    printf("ID Employe (infirmier): ");
                    scanf("%d", &id);
                    viderBuffer();
                    creerComptePersonnel(id, ROLE_INFIRMIER);
                    pause();
                }
                break;

            case 5:
                {
                    int id;
                    afficherListeEmployesSimple();
                    printf("ID Employe (technicien): ");
                    scanf("%d", &id);
                    viderBuffer();
                    creerComptePersonnel(id, ROLE_TECHNICIEN_LABO);
                    pause();
                }
                break;

            case 6:
                {
                    int id;
                    afficherListeEmployesSimple();
                    printf("ID Employe (pharmacien): ");
                    scanf("%d", &id);
                    viderBuffer();
                    creerComptePersonnel(id, ROLE_PHARMACIEN);
                    pause();
                }
                break;

            case 7:
                {
                    int id;
                    printf("ID Employe (0 pour super admin): ");
                    scanf("%d", &id);
                    viderBuffer();
                    if (id == 0)
                        creerCompteAdministrateur();
                    else
                        creerComptePersonnel(id, ROLE_ADMINISTRATEUR);
                    pause();
                }
                break;

            case 8:
                {
                    int id;
                    printf("ID Compte a desactiver: ");
                    scanf("%d", &id);
                    viderBuffer();
                    desactiverCompte(id);
                    pause();
                }
                break;

            case 9:
                {
                    int id;
                    printf("ID Compte a activer: ");
                    scanf("%d", &id);
                    viderBuffer();
                    activerCompte(id);
                    pause();
                }
                break;

            case 10:
                {
                    int id;
                    printf("ID Compte: ");
                    scanf("%d", &id);
                    viderBuffer();
                    modifierMotDePasse(id);
                    pause();
                }
                break;

            case 11:
                {
                    int id;
                    printf("ID Compte: ");
                    scanf("%d", &id);
                    viderBuffer();
                    afficherHistoriqueConnexions(id);
                    pause();
                }
                break;

            case 12:
                return;

            default:
                printf("Choix invalide.\n");
                pause();
        }

    } while(choix != 12);
}

void afficherMenuParProfil()
{
    if (utilisateur_actuel == NULL) return;

    system("cls");
    color(10, 0);
    printf("\n========================================\n");
    printf("    SYSTEME DE GESTION HOSPITALIERE    \n");
    printf("========================================\n");
    color(7, 0);

    printf("\nUtilisateur: %s", utilisateur_actuel->login);
    printf("\nProfil: ");
    switch(utilisateur_actuel->profil)
    {
        case ROLE_PATIENT: printf("Patient\n"); break;
        case ROLE_MEDECIN: printf("Medecin\n"); break;
        case ROLE_INFIRMIER: printf("Infirmier\n"); break;
        case ROLE_ADMINISTRATEUR: printf("Administrateur\n"); break;
        case ROLE_TECHNICIEN_LABO: printf("Technicien de laboratoire\n"); break;
        case ROLE_PHARMACIEN: printf("Pharmacien\n"); break;
        case ROLE_SUPER_ADMIN: printf("Super Administrateur\n"); break;
    }
    printf("----------------------------------------\n\n");

    switch(utilisateur_actuel->profil)
    {
        case ROLE_PATIENT:
            printf("1. Voir mon dossier medical\n");
            printf("2. Voir mes consultations\n");
            printf("3. Voir mes ordonnances\n");
            printf("4. Voir mes analyses\n");
            printf("5. Prendre rendez-vous\n");
            printf("6. Modifier mon mot de passe\n");
            printf("7. Deconnexion\n");
            break;

        case ROLE_MEDECIN:
            printf("1. Gestion des patients\n");
            printf("2. Consultations\n");
            printf("3. Prescriptions\n");
            printf("4. Diagnostic medical\n");
            printf("5. Demandes d'analyses\n");
            printf("6. Agenda\n");
            printf("7. Statistiques\n");
            printf("8. Modifier mot de passe\n");
            printf("9. Deconnexion\n");
            break;

        case ROLE_INFIRMIER:
            printf("1. Liste des patients\n");
            printf("2. Soins a administrer\n");
            printf("3. Constantes vitales\n");
            printf("4. Planning des soins\n");
            printf("5. Modifier mot de passe\n");
            printf("6. Deconnexion\n");
            break;

        case ROLE_ADMINISTRATEUR:
            printf("1. Gestion des patients\n");
            printf("2. Gestion du personnel\n");
            printf("3. Gestion des rendez-vous\n");
            printf("4. Gestion des consultations\n");
            printf("5. Gestion des comptes utilisateurs\n");
            printf("6. Archives\n");
            printf("7. Statistiques\n");
            printf("8. Modifier mot de passe\n");
            printf("9. Deconnexion\n");
            break;

        case ROLE_TECHNICIEN_LABO:
            printf("1. Liste des analyses a realiser\n");
            printf("2. Saisir resultats d'analyses\n");
            printf("3. Consulter historique analyses\n");
            printf("4. Modifier mot de passe\n");
            printf("5. Deconnexion\n");
            break;

        case ROLE_PHARMACIEN:
            printf("1. Gestion des ordonnances\n");
            printf("2. Gestion du stock\n");
            printf("3. Delivrance medicaments\n");
            printf("4. Alertes stock faible\n");
            printf("5. Modifier mot de passe\n");
            printf("6. Deconnexion\n");
            break;

        case ROLE_SUPER_ADMIN:
            printf("1. Gestion complete patients\n");
            printf("2. Gestion complete personnel\n");
            printf("3. Gestion consultations\n");
            printf("4. Gestion ordonnances\n");
            printf("5. Gestion interventions\n");
            printf("6. Gestion comptes utilisateurs\n");
            printf("7. Archives\n");
            printf("8. Statistiques avancees\n");
            printf("9. Configuration systeme\n");
            printf("10. Modifier mot de passe\n");
            printf("11. Deconnexion\n");
            break;
    }

    printf("\nVotre choix: ");
}
