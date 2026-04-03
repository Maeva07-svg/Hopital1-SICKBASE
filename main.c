#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "utils.h"
#include "comptes.h"
#include "patient.h"
#include "personnel.h"
#include "consultations.h"
#include "ordonnances.h"
#include "diagnostic.h"
#include "interventions.h"
#include "archives.h"
#include "fichiers.h"
#include "fichiers_personnel.h"
#include "fichiers_relations.h"
#include "fichiers_rdv.h"
#include "rdv_avance.h"

// Déclaration des fonctions externes
void menuGestionPatients();
void menuGestionPersonnel();
void menuConsultations();
void menuOrdonnances();
void menuDiagnosticMedical();
void menuInterventions();
void menuArchives();
void gererComptes();
void afficherMenuParProfil();

// Fonctions de vérification des droits pour les rendez-vous
int peutVoirRendezVousPatient(int id_patient)
{
    if (utilisateur_actuel == NULL) return 0;

    switch(utilisateur_actuel->profil)
    {
        case ROLE_SUPER_ADMIN:
        case ROLE_ADMINISTRATEUR:
            return 1;  // Accès total

        case ROLE_MEDECIN:
            return 1;  // Les médecins voient tous les patients

        case ROLE_PATIENT:
            return (utilisateur_actuel->id_associe == id_patient);  // Uniquement ses propres RDV

        default:
            return 0;  // Autres profils n'ont pas accès
    }
}

int peutVoirRendezVousMedecin(int id_medecin)
{
    if (utilisateur_actuel == NULL) return 0;

    switch(utilisateur_actuel->profil)
    {
        case ROLE_SUPER_ADMIN:
        case ROLE_ADMINISTRATEUR:
            return 1;  // Accès total

        case ROLE_MEDECIN:
            return (utilisateur_actuel->id_associe == id_medecin);  // Uniquement ses propres RDV

        default:
            return 0;
    }
}

int peutGererDisponibilites(int id_medecin)
{
    if (utilisateur_actuel == NULL) return 0;

    switch(utilisateur_actuel->profil)
    {
        case ROLE_SUPER_ADMIN:
        case ROLE_ADMINISTRATEUR:
            return 1;  // Accès total

        case ROLE_MEDECIN:
            return (utilisateur_actuel->id_associe == id_medecin);  // Uniquement ses propres disponibilités

        default:
            return 0;
    }
}

// Fonction pour afficher le menu principal selon le profil (version avec Rendez-vous Avancé)
void afficherMenuPrincipal()
{
    if (utilisateur_actuel == NULL) return;

    int choix;

    do
    {
        system("cls");
        color(10, 0);
        printf("\n========================================\n");
        printf("    SYSTEME DE GESTION HOSPITALIERE    \n");
        printf("============SICKBASE================\n");
        color(7, 0);

        // Afficher les informations de l'utilisateur
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

        // Menu selon le profil
        switch(utilisateur_actuel->profil)
        {
            case ROLE_PATIENT:
                printf("1. Voir mon dossier medical\n");
                printf("2. Voir mes consultations\n");
                printf("3. Voir mes ordonnances\n");
                printf("4. Voir mes analyses\n");
                printf("5. Prendre rendez-vous\n");
                printf("6. Voir mes rendez-vous\n");
                printf("7. Annuler un rendez-vous\n");
                printf("8. Modifier mon mot de passe\n");
                printf("9. Deconnexion\n");
                break;

            case ROLE_MEDECIN:
                printf("1. Gestion des patients\n");
                printf("2. Consultations\n");
                printf("3. Prescriptions\n");
                printf("4. Diagnostic medical\n");
                printf("5. Rendez-vous (Planning)\n");
                printf("6. Mes disponibilites\n");
                printf("7. Mes rendez-vous\n");
                printf("8. Demandes d'analyses\n");
                printf("9. Statistiques\n");
                printf("10. Modifier mot de passe\n");
                printf("11. Deconnexion\n");
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
                printf("10. Gestion des rendez-vous\n");
                printf("11. Modifier mot de passe\n");
                printf("12. Deconnexion\n");
                break;
        }

        printf("\nVotre choix: ");
        scanf("%d", &choix);
        viderBuffer();

        switch(utilisateur_actuel->profil)
        {
            case ROLE_PATIENT:
                switch(choix)
                {
                    case 1:
                        if (verifierDroit("voir_patient"))
                        {
                            int index = rechercherParID(utilisateur_actuel->id_associe);
                            if (index != -1)
                            {
                                afficherDetailsCompletsPatient(index);
                                pause();
                            }
                            else
                            {
                                printf("Dossier patient non trouve.\n");
                                pause();
                            }
                        }
                        break;
                    case 2:
                        consulterConsultationsPatient(utilisateur_actuel->id_associe);
                        break;
                    case 3:
                        afficherOrdonnancesPatient(utilisateur_actuel->id_associe);
                        break;
                    case 4:
                        printf("\nModule analyses en developpement...\n");
                        pause();
                        break;
                    case 5:
                        prendreRendezVousAvance();
                        break;
                    case 6:
                        listerRendezVousPatient(utilisateur_actuel->id_associe);
                        break;
                    case 7:
                        {
                            int id_rdv;
                            printf("ID du rendez-vous a annuler: ");
                            scanf("%d", &id_rdv);
                            viderBuffer();
                            annulerRendezVousAvance(id_rdv);
                        }
                        break;
                    case 8:
                        modifierMotDePasse(utilisateur_actuel->id_compte);
                        pause();
                        break;
                    case 9:
                        logout();
                        return;
                    default:
                        printf("Choix invalide.\n");
                        pause();
                }
                break;

            case ROLE_MEDECIN:
                switch(choix)
                {
                    case 1: menuGestionPatients(); break;
                    case 2: menuConsultations(); break;
                    case 3: menuOrdonnances(); break;
                    case 4: menuDiagnosticMedical(); break;
                    case 5:
                        if (peutGererDisponibilites(utilisateur_actuel->id_associe))
                        {
                            afficherPlanningMedecin(utilisateur_actuel->id_associe, NULL);
                        }
                        else
                        {
                            printf("Acces non autorise.\n");
                            pause();
                        }
                        break;
                    case 6:
                        if (peutGererDisponibilites(utilisateur_actuel->id_associe))
                        {
                            gererDisponibilitesMedecin(utilisateur_actuel->id_associe);
                        }
                        else
                        {
                            printf("Acces non autorise.\n");
                            pause();
                        }
                        break;
                    case 7:
                        listerRendezVousMedecin(utilisateur_actuel->id_associe);
                        break;
                    case 8:
                        printf("\nModule demandes d'analyses en developpement...\n");
                        pause();
                        break;
                    case 9:
                        printf("\nStatistiques medicales en developpement...\n");
                        pause();
                        break;
                    case 10:
                        modifierMotDePasse(utilisateur_actuel->id_compte);
                        pause();
                        break;
                    case 11:
                        logout();
                        return;
                    default:
                        printf("Choix invalide.\n");
                        pause();
                }
                break;

            case ROLE_INFIRMIER:
                switch(choix)
                {
                    case 1:
                        afficherListePatientsSimple();
                        pause();
                        break;
                    case 2:
                        printf("\nModule soins a administrer en developpement...\n");
                        pause();
                        break;
                    case 3:
                        printf("\nModule constantes vitales en developpement...\n");
                        pause();
                        break;
                    case 4:
                        printf("\nModule planning des soins en developpement...\n");
                        pause();
                        break;
                    case 5:
                        modifierMotDePasse(utilisateur_actuel->id_compte);
                        pause();
                        break;
                    case 6:
                        logout();
                        return;
                    default:
                        printf("Choix invalide.\n");
                        pause();
                }
                break;

            case ROLE_ADMINISTRATEUR:
                switch(choix)
                {
                    case 1: menuGestionPatients(); break;
                    case 2: menuGestionPersonnel(); break;
                    case 3:
                        menuRendezVousAvance();
                        break;
                    case 4: menuConsultations(); break;
                    case 5: gererComptes(); break;
                    case 6: menuArchives(); break;
                    case 7:
                        printf("\nStatistiques administratives en developpement...\n");
                        pause();
                        break;
                    case 8:
                        modifierMotDePasse(utilisateur_actuel->id_compte);
                        pause();
                        break;
                    case 9:
                        logout();
                        return;
                    default:
                        printf("Choix invalide.\n");
                        pause();
                }
                break;

            case ROLE_TECHNICIEN_LABO:
                switch(choix)
                {
                    case 1:
                        printf("\nModule analyses a realiser en developpement...\n");
                        pause();
                        break;
                    case 2:
                        printf("\nModule saisie resultats en developpement...\n");
                        pause();
                        break;
                    case 3:
                        printf("\nModule historique analyses en developpement...\n");
                        pause();
                        break;
                    case 4:
                        modifierMotDePasse(utilisateur_actuel->id_compte);
                        pause();
                        break;
                    case 5:
                        logout();
                        return;
                    default:
                        printf("Choix invalide.\n");
                        pause();
                }
                break;

            case ROLE_PHARMACIEN:
                switch(choix)
                {
                    case 1: menuOrdonnances(); break;
                    case 2:
                        printf("\nModule gestion du stock en developpement...\n");
                        pause();
                        break;
                    case 3:
                        printf("\nModule delivrance medicaments en developpement...\n");
                        pause();
                        break;
                    case 4:
                        printf("\nModule alertes stock faible en developpement...\n");
                        pause();
                        break;
                    case 5:
                        modifierMotDePasse(utilisateur_actuel->id_compte);
                        pause();
                        break;
                    case 6:
                        logout();
                        return;
                    default:
                        printf("Choix invalide.\n");
                        pause();
                }
                break;

            case ROLE_SUPER_ADMIN:
                switch(choix)
                {
                    case 1: menuGestionPatients(); break;
                    case 2: menuGestionPersonnel(); break;
                    case 3: menuConsultations(); break;
                    case 4: menuOrdonnances(); break;
                    case 5: menuInterventions(); break;
                    case 6: gererComptes(); break;
                    case 7: menuArchives(); break;
                    case 8:
                        printf("\nStatistiques avancees en developpement...\n");
                        pause();
                        break;
                    case 9:
                        printf("\nConfiguration systeme en developpement...\n");
                        pause();
                        break;
                    case 10:
                        menuRendezVousAvance();
                        break;
                    case 11:
                        modifierMotDePasse(utilisateur_actuel->id_compte);
                        pause();
                        break;
                    case 12:
                        logout();
                        return;
                    default:
                        printf("Choix invalide.\n");
                        pause();
                }
                break;

            default:
                printf("Profil non reconnu.\n");
                pause();
        }

    } while(1);
}

// Fonction pour afficher le menu d'authentification
void afficherMenuAuthentification()
{
    system("cls");
    color(11, 0);
    printf("\n========================================\n");
    printf("    SYSTEME DE GESTION HOSPITALIERE    \n");
    printf("           VERSION SECURISEE 6.0       \n");
    printf("========================================\n\n");
    color(7, 0);

    printf("1. Se connecter\n");
    printf("2. Creer un nouveau compte\n");
    printf("3. Quitter\n");
    printf("\nVotre choix: ");
}

// Fonction pour créer un compte (menu interactif)
void creerNouveauCompte()
{
    system("cls");
    printf("\n=== CREATION D'UN NOUVEAU COMPTE ===\n\n");

    printf("Type de compte:\n");
    printf("1. Patient\n");
    printf("2. Medecin\n");
    printf("3. Infirmier\n");
    printf("4. Technicien de laboratoire\n");
    printf("5. Pharmacien\n");
    printf("6. Retour\n");
    printf("\nVotre choix: ");

    int choix;
    scanf("%d", &choix);
    viderBuffer();

    switch(choix)
    {
        case 1: // Patient
            {
                if (nombrePatients == 0)
                {
                    printf("\nAucun patient enregistre. Veuillez d'abord enregistrer un patient.\n");
                    pause();
                    return;
                }
                afficherListePatientsSimple();
                printf("\nID du patient: ");
                int id;
                scanf("%d", &id);
                viderBuffer();

                int index = rechercherParID(id);
                if (index != -1)
                {
                    creerComptePatient(id);
                }
                else
                {
                    printf("Patient non trouve.\n");
                }
                pause();
            }
            break;

        case 2: // Medecin
            {
                if (nombreEmployes == 0)
                {
                    printf("\nAucun employe enregistre. Veuillez d'abord enregistrer un medecin.\n");
                    pause();
                    return;
                }
                afficherListeEmployesSimple();
                printf("\nID du medecin: ");
                int id;
                scanf("%d", &id);
                viderBuffer();

                int index = rechercherEmployeParID(id);
                if (index != -1 && strcmp(personnel[index].fonction, "Medecin") == 0)
                {
                    creerComptePersonnel(id, ROLE_MEDECIN);
                }
                else
                {
                    printf("Medecin non trouve.\n");
                }
                pause();
            }
            break;

        case 3: // Infirmier
            {
                if (nombreEmployes == 0)
                {
                    printf("\nAucun employe enregistre. Veuillez d'abord enregistrer un infirmier.\n");
                    pause();
                    return;
                }
                afficherListeEmployesSimple();
                printf("\nID de l'infirmier: ");
                int id;
                scanf("%d", &id);
                viderBuffer();

                int index = rechercherEmployeParID(id);
                if (index != -1 && strcmp(personnel[index].fonction, "Infirmier") == 0)
                {
                    creerComptePersonnel(id, ROLE_INFIRMIER);
                }
                else
                {
                    printf("Infirmier non trouve.\n");
                }
                pause();
            }
            break;

        case 4: // Technicien labo
            {
                if (nombreEmployes == 0)
                {
                    printf("\nAucun employe enregistre. Veuillez d'abord enregistrer un technicien.\n");
                    pause();
                    return;
                }
                afficherListeEmployesSimple();
                printf("\nID du technicien: ");
                int id;
                scanf("%d", &id);
                viderBuffer();

                int index = rechercherEmployeParID(id);
                if (index != -1 && strcmp(personnel[index].fonction, "Technicien") == 0)
                {
                    creerComptePersonnel(id, ROLE_TECHNICIEN_LABO);
                }
                else
                {
                    printf("Technicien non trouve.\n");
                }
                pause();
            }
            break;

        case 5: // Pharmacien
            {
                if (nombreEmployes == 0)
                {
                    printf("\nAucun employe enregistre. Veuillez d'abord enregistrer un pharmacien.\n");
                    pause();
                    return;
                }
                afficherListeEmployesSimple();
                printf("\nID du pharmacien: ");
                int id;
                scanf("%d", &id);
                viderBuffer();

                int index = rechercherEmployeParID(id);
                if (index != -1 && strcmp(personnel[index].fonction, "Pharmacien") == 0)
                {
                    creerComptePersonnel(id, ROLE_PHARMACIEN);
                }
                else
                {
                    printf("Pharmacien non trouve.\n");
                }
                pause();
            }
            break;

        case 6:
            return;

        default:
            printf("Choix invalide.\n");
            pause();
    }
}

int main()
{
    system("cls");
    color(14, 0);
    printf("\n========================================\n");
    printf("               SICKBASE    \n");
    printf("           VERSION SECURISEE 3.0       \n");
    printf("========================================\n");
    printf("\nCHARGEMENT DES MODULES...\n");
    color(7, 0);

    // Initialisation des modules
    printf("  - Initialisation des maladies... ");
    initialiserMaladies();
    printf("OK\n");

    printf("  - Initialisation du personnel... ");
    initialiserFonctionsPersonnel();
    printf("OK\n");

    printf("  - Initialisation des consultations... ");
    initialiserConsultations();
    printf("OK\n");

    printf("  - Initialisation des rendez-vous avances... ");
    initialiserModuleRdv();
    printf("OK\n");

    printf("  - Initialisation des ordonnances... ");
    initialiserOrdonnances();
    printf("OK\n");

    printf("  - Initialisation des interventions... ");
    initialiserInterventions();
    printf("OK\n");

    printf("  - Initialisation des archives... ");
    initialiserArchives();
    printf("OK\n");

    printf("  - Initialisation des comptes... ");
    initialiserComptes();
    printf("OK\n");

    // Chargement des donnees
    printf("\nCHARGEMENT DES DONNEES...\n");

    printf("  - Chargement des patients... ");
    chargerDernierID();
    chargerPatients();
    printf("%d patient(s) charge(s)\n", nombrePatients);

    printf("  - Chargement du personnel... ");
    chargerDernierIDPersonnel();
    chargerPersonnel();
    printf("%d employe(s) charge(s)\n", nombreEmployes);

    printf("  - Chargement des consultations... ");
    chargerConsultations();
    chargerRendezVous();
    printf("%d consultation(s), %d rendez-vous\n", nombreConsultations, nombreRendezVous);

    printf("  - Chargement des rendez-vous avances... ");
    chargerCreneaux();
    chargerRendezVousAvance();
    printf("%d creneaux, %d rdv\n", nombreCreneaux, nombreRdvAvance);

    printf("  - Chargement des ordonnances... ");
    chargerOrdonnances();
    printf("%d ordonnance(s)\n", nombreOrdonnances);

    printf("  - Chargement des interventions... ");
    chargerInterventions();
    chargerPrisesEnCharge();
    printf("%d intervention(s), %d prise(s) en charge\n", nombreInterventions, nombrePrisesEnCharge);

    printf("  - Chargement des archives... ");
    chargerArchives();
    printf("%d archive(s)\n", nombreArchives);

    printf("  - Chargement des comptes... ");
    chargerComptes();
    printf("%d compte(s) actif(s)\n", nombreComptes);

    printf("\n========================================\n");
    printf("SYSTEME PRET.\n");
    printf("========================================\n\n");

    Sleep(2000);

    // Boucle d'authentification avec menu
    int choix_auth;
    while (1)
    {
        afficherMenuAuthentification();
        scanf("%d", &choix_auth);
        viderBuffer();

        switch(choix_auth)
        {
            case 1: // Se connecter
                if (login())
                {
                    afficherMenuPrincipal();
                }
                break;

            case 2: // Creer un compte
                creerNouveauCompte();
                break;

            case 3: // Quitter
                printf("\nAu revoir!\n");
                // Sauvegarde finale
                sauvegarderPatients();
                sauvegarderPersonnel();
                sauvegarderConsultations();
                sauvegarderRendezVous();
                sauvegarderOrdonnances();
                sauvegarderInterventions();
                sauvegarderPrisesEnCharge();
                sauvegarderArchives();
                sauvegarderComptes();
                sauvegarderCreneaux();
                sauvegarderRendezVousAvance();
                return 0;

            default:
                printf("\nChoix invalide.\n");
                pause();
        }
    }

    return 0;
}
