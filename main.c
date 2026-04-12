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
#include "facturation.h"

// Déclaration des fonctions externes
void menuGestionPatients();
void menuGestionPersonnel();
void menuConsultations();
void menuOrdonnances();
void menuDiagnosticMedical();
void menuDiagnosticMedicalAvance();
void menuInterventions();
void menuArchives();
void gererComptes();
void afficherMenuParProfil();
void menuFacturation();
void menuRendezVousAvance();
void completerDossierPatient(int id_compte);
void questionnaireMedical(int id_patient);

// Fonctions de vérification des droits pour les rendez-vous
int peutVoirRendezVousPatient(int id_patient)
{
    if (utilisateur_actuel == NULL) return 0;

    switch(utilisateur_actuel->profil)
    {
    case ROLE_SUPER_ADMIN:
    case ROLE_ADMINISTRATEUR:
        return 1;
    case ROLE_MEDECIN:
        return 1;
    case ROLE_PATIENT:
        return (utilisateur_actuel->id_associe == id_patient);
    default:
        return 0;
    }
}

int peutVoirRendezVousMedecin(int id_medecin)
{
    if (utilisateur_actuel == NULL) return 0;
    switch(utilisateur_actuel->profil)
    {
    case ROLE_SUPER_ADMIN:
    case ROLE_ADMINISTRATEUR:
        return 1;
    case ROLE_MEDECIN:
        return (utilisateur_actuel->id_associe == id_medecin);
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
        return 1;
    case ROLE_MEDECIN:
        return (utilisateur_actuel->id_associe == id_medecin);
    default:
        return 0;
    }
}

// Fonction pour afficher le menu principal selon le profil
void afficherMenuPrincipal()
{
    if (utilisateur_actuel == NULL) return;

    // Si c'est un patient avec compte incomplet, rediriger vers completion
    if (utilisateur_actuel->profil == ROLE_PATIENT && !estCompteComplet(utilisateur_actuel->id_compte))
    {
        printf("\n=== BIENVENUE ! ===\n");
        printf("Veuillez completer votre dossier medical avant de continuer.\n\n");
        completerDossierPatient(utilisateur_actuel->id_compte);
    }

    int choix;
    do
    {
        system("cls");
        color(10, 0);
        printf("\n========================================\n");
        printf("         SYSTEME DE GESTION            \n");
        printf("           HOSPITALIERE SICKBASE       \n");
        printf("========================================\n");
        color(7, 0);

        printf("\nUtilisateur: %s", utilisateur_actuel->login);
        printf("\nProfil: ");
        switch(utilisateur_actuel->profil)
        {
        case ROLE_PATIENT:
            printf("Patient\n");
            break;
        case ROLE_MEDECIN:
            printf("Medecin\n");
            break;
        case ROLE_INFIRMIER:
            printf("Infirmier\n");
            break;
        case ROLE_ADMINISTRATEUR:
            printf("Administrateur\n");
            break;
        case ROLE_TECHNICIEN_LABO:
            printf("Technicien de laboratoire\n");
            break;
        case ROLE_PHARMACIEN:
            printf("Pharmacien\n");
            break;
        case ROLE_SUPER_ADMIN:
            printf("Super Administrateur\n");
            break;
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
            printf("6. Voir mes rendez-vous\n");
            printf("7. Annuler un rendez-vous\n");
            printf("8. Voir mes factures\n");
            printf("9. Payer une facture\n");
            printf("10. Modifier mon mot de passe\n");
            printf("11. Deconnexion\n");
            break;

        case ROLE_MEDECIN:
            printf("1. Gestion des patients\n");
            printf("2. Consultations\n");
            printf("3. Prescriptions\n");
            printf("4. Diagnostic medical\n");
            printf("5. Rendez-vous (Planning)\n");
            printf("6. Mes disponibilites\n");
            printf("7. Mes rendez-vous\n");
            printf("8. Facturation\n");
            printf("9. Demandes d'analyses\n");
            printf("10. Statistiques\n");
            printf("11. Modifier mot de passe\n");
            printf("12. Deconnexion\n");
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
            printf("6. Facturation\n");
            printf("7. Archives\n");
            printf("8. Statistiques\n");
            printf("9. Modifier mot de passe\n");
            printf("10. Deconnexion\n");
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
            printf("7. Facturation\n");
            printf("8. Archives\n");
            printf("9. Statistiques avancees\n");
            printf("10. Configuration systeme\n");
            printf("11. Gestion des rendez-vous\n");
            printf("12. Modifier mot de passe\n");
            printf("13. Deconnexion\n");
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
                listerRendezVousPatientAvance(utilisateur_actuel->id_associe);
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
                afficherFacturesPatient(utilisateur_actuel->id_associe);
                break;
            case 9:
            {
                afficherFacturesPatient(utilisateur_actuel->id_associe);
                printf("\nEntrez le numero de la facture a payer: ");
                char num[20];
                fgets(num, 20, stdin);
                num[strcspn(num, "\n")] = '\0';

                int id_facture = -1;
                for (int i = 0; i < nombreFactures; i++)
                {
                    if (strcmp(factures[i].numero_facture, num) == 0 && factures[i].id_patient == utilisateur_actuel->id_associe)
                    {
                        id_facture = factures[i].id_facture;
                        break;
                    }
                }

                if (id_facture != -1)
                {
                    enregistrerPaiement(id_facture);
                }
                else
                {
                    printf("Facture non trouvee ou n'appartient pas a ce patient.\n");
                    pause();
                }
            }
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

        case ROLE_MEDECIN:
            switch(choix)
            {
            case 1:  // Gestion des patients - Version limitée pour médecin
            {
                printf("\n=== GESTION DES PATIENTS (Mode Consultation) ===\n");
                printf("Vous pouvez uniquement consulter les dossiers des patients qui vous sont associes.\n\n");
                afficherListePatientsPourMedecin(utilisateur_actuel->id_associe);
            }
            break;
            case 2:
                menuConsultations();
                break;
            case 3:
                menuOrdonnances();
                break;
            case 4:
                menuDiagnosticMedicalAvance();
                break;
            case 5:
                if (peutGererDisponibilites(utilisateur_actuel->id_associe))
                {
                    char date[20];
                    printf("Date (JJ/MM/AAAA): ");
                    fgets(date, 20, stdin);
                    date[strcspn(date, "\n")] = '\0';
                    afficherPlanningMedecin(utilisateur_actuel->id_associe, date);
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
                listerRendezVousMedecinAvance(utilisateur_actuel->id_associe);
                break;
            case 8:
                menuFacturation();
                break;
            case 9:
                printf("\nModule demandes d'analyses en developpement...\n");
                pause();
                break;
            case 10:
                printf("\nStatistiques medicales en developpement...\n");
                pause();
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
            case 1:
                menuGestionPatients();
                break;
            case 2:
                menuGestionPersonnel();
                break;
            case 3:
                menuRendezVousAvance();
                break;
            case 4:
                menuConsultations();
                break;
            case 5:
                gererComptes();
                break;
            case 6:
                menuFacturation();
                break;
            case 7:
                menuArchives();
                break;
            case 8:
                printf("\nStatistiques administratives en developpement...\n");
                pause();
                break;
            case 9:
                modifierMotDePasse(utilisateur_actuel->id_compte);
                pause();
                break;
            case 10:
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
            case 1:
                menuOrdonnances();
                break;
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
            case 1:
                menuGestionPatients();
                break;
            case 2:
                menuGestionPersonnel();
                break;
            case 3:
                menuConsultations();
                break;
            case 4:
                menuOrdonnances();
                break;
            case 5:
                menuInterventions();
                break;
            case 6:
                gererComptes();
                break;
            case 7:
                menuFacturation();
                break;
            case 8:
                menuArchives();
                break;
            case 9:
                printf("\nStatistiques avancees en developpement...\n");
                pause();
                break;
            case 10:
                printf("\nConfiguration systeme en developpement...\n");
                pause();
                break;
            case 11:
                menuRendezVousAvance();
                break;
            case 12:
                modifierMotDePasse(utilisateur_actuel->id_compte);
                pause();
                break;
            case 13:
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
    }
    while(1);
}

// Fonction pour afficher le menu d'authentification
void afficherMenuAuthentification()
{
    system("cls");
    color(11, 0);
    printf("\n========================================\n");
    printf("              SICKBASE                  \n");
    printf("    SYSTEME DE GESTION HOSPITALIERE     \n");
    printf("           VERSION SECURISEE 3.0        \n");
    printf("========================================\n\n");
    color(7, 0);

    printf("1. Se connecter\n");
    printf("2. Creer un compte\n");
    printf("3. Quitter\n");
    printf("\nVotre choix: ");
}

// Fonction pour créer un compte (menu interactif)
void creerNouveauCompte()
{
    system("cls");
    printf("\n=== CREATION D'UN COMPTE ===\n\n");

    printf("TYPE DE COMPTE:\n");
    printf("1. Patient (deja enregistre dans le systeme)\n");
    printf("2. Nouveau patient (creer mon dossier medical)\n");
    printf("3. Medecin\n");
    printf("4. Infirmier\n");
    printf("5. Technicien de laboratoire\n");
    printf("6. Pharmacien\n");
    printf("7. Retour\n");
    printf("\nVotre choix: ");

    int choix;
    scanf("%d", &choix);
    viderBuffer();

    switch(choix)
    {
    case 1: // Patient déjà existant
    {
        if (nombrePatients == 0)
        {
            printf("\nAucun patient enregistre dans le systeme.\n");
            printf("Veuillez choisir l'option 'Nouveau patient'.\n");
            pause();
            return;
        }
        afficherListePatientsSimple();
        printf("\nEntrez l'ID du patient: ");
        int id;
        scanf("%d", &id);
        viderBuffer();

        int index = rechercherParID(id);
        if (index != -1)
        {
            // Vérifier si un compte existe déjà
            int compte_existe = 0;
            for (int i = 0; i < nombreComptes; i++)
            {
                if (comptes[i].id_associe == id && comptes[i].profil == ROLE_PATIENT)
                {
                    compte_existe = 1;
                    break;
                }
            }

            if (compte_existe)
            {
                printf("Un compte existe deja pour ce patient.\n");
            }
            else
            {
                creerComptePatient(id);
            }
        }
        else
        {
            printf("Patient non trouve.\n");
        }
        pause();
    }
    break;

    case 2: // Nouveau patient (création auto)
        creerComptePatientAuto();
        break;

    case 3: // Medecin
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
            // Vérifier si un compte existe déjà
            int compte_existe = 0;
            for (int i = 0; i < nombreComptes; i++)
            {
                if (comptes[i].id_associe == id && comptes[i].profil == ROLE_MEDECIN)
                {
                    compte_existe = 1;
                    break;
                }
            }

            if (compte_existe)
            {
                printf("Un compte existe deja pour ce medecin.\n");
            }
            else
            {
                creerComptePersonnel(id, ROLE_MEDECIN);
            }
        }
        else
        {
            printf("Medecin non trouve.\n");
        }
        pause();
    }
    break;

    case 4: // Infirmier
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
            int compte_existe = 0;
            for (int i = 0; i < nombreComptes; i++)
            {
                if (comptes[i].id_associe == id && comptes[i].profil == ROLE_INFIRMIER)
                {
                    compte_existe = 1;
                    break;
                }
            }

            if (compte_existe)
            {
                printf("Un compte existe deja pour cet infirmier.\n");
            }
            else
            {
                creerComptePersonnel(id, ROLE_INFIRMIER);
            }
        }
        else
        {
            printf("Infirmier non trouve.\n");
        }
        pause();
    }
    break;

    case 5: // Technicien labo
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
            int compte_existe = 0;
            for (int i = 0; i < nombreComptes; i++)
            {
                if (comptes[i].id_associe == id && comptes[i].profil == ROLE_TECHNICIEN_LABO)
                {
                    compte_existe = 1;
                    break;
                }
            }

            if (compte_existe)
            {
                printf("Un compte existe deja pour ce technicien.\n");
            }
            else
            {
                creerComptePersonnel(id, ROLE_TECHNICIEN_LABO);
            }
        }
        else
        {
            printf("Technicien non trouve.\n");
        }
        pause();
    }
    break;

    case 6: // Pharmacien
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
            int compte_existe = 0;
            for (int i = 0; i < nombreComptes; i++)
            {
                if (comptes[i].id_associe == id && comptes[i].profil == ROLE_PHARMACIEN)
                {
                    compte_existe = 1;
                    break;
                }
            }

            if (compte_existe)
            {
                printf("Un compte existe deja pour ce pharmacien.\n");
            }
            else
            {
                creerComptePersonnel(id, ROLE_PHARMACIEN);
            }
        }
        else
        {
            printf("Pharmacien non trouve.\n");
        }
        pause();
    }
    break;

    case 7:
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
    printf("               SICKBASE                \n");
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

    printf("  - Initialisation de la facturation... ");
    initialiserFacturation();
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

    printf("  - Chargement des factures... ");
    chargerFactures();
    printf("%d facture(s)\n", nombreFactures);

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
        case 1:
            if (login())
            {
                afficherMenuPrincipal();
            }
            break;

        case 2:
            creerNouveauCompte();
            break;

        case 3:
            printf("\nAu revoir!\n");
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
            sauvegarderFactures();
            return 0;

        default:
            printf("\nChoix invalide.\n");
            pause();
        }
    }

    return 0;
}
