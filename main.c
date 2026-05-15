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
#include "infirmier.h"
#include "fichiers_relations.h"
#include "fichiers_rdv.h"
#include "rdv_avance.h"
#include "pharmacie.h"
#include "facturation.h"
#include "laboratoire.h"
#include "recuperation.h"
#include "administration.h"
#include "notification.h"
#include "messagerie.h"
#include "ui.h"
#include "sdl_ui.h"

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
void ui_afficherEcranDossierMedical();
void ui_afficherEcranMesConsultations();
void ui_afficherEcranMesOrdonnances();
void ui_afficherEcranMesAnalyses();
void ui_afficherEcranMesRendezVous();
void ui_afficherEcranMesFactures();
void ui_afficherEcranPersonnel();
void ui_afficherListePersonnel();
void ui_afficherEcranStatistiquesPersonnel();

void activerConsoleUTF8()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX fontInfo = {0};
    fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    fontInfo.dwFontSize.X = 0;
    fontInfo.dwFontSize.Y = 16;
    fontInfo.FontWeight = FW_NORMAL;
    wcscpy(fontInfo.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
}

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

    if (utilisateur_actuel->profil == ROLE_PATIENT && !estCompteComplet(utilisateur_actuel->id_compte))
    {
        ui_afficherEntete();
        printf("\n╔════════════════════════════════════════════════════════════════════════════════╗\n");
        printf("║                              BIENVENUE !                                      ║\n");
        printf("║              Veuillez completer votre dossier medical avant de continuer.    ║\n");
        printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
        completerDossierPatient(utilisateur_actuel->id_compte);
    }

    chargerNotifications();
    chargerMessages();

    int choix;
    do
    {
        ui_afficherEntete();
        ui_afficherInfoUtilisateur();

        if (hasNotificationsNonLues(utilisateur_actuel->id_compte) || hasNouveauxMessages(utilisateur_actuel->id_compte))
        {
            ui_setColor(UI_COLOR_WARNING);
            printf("\n╔════════════════════════════════════════════════════════════════════════════════╗\n");
            printf("║                         NOUVELLES NOTIFICATIONS                                ║\n");
            printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
            if (hasNotificationsNonLues(utilisateur_actuel->id_compte))
                printf("║     [!] Vous avez des notifications non lues !                              ║\n");
            if (hasNouveauxMessages(utilisateur_actuel->id_compte))
                printf("║     [i] Vous avez des nouveaux messages !                                  ║\n");
            printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
            ui_resetColor();
        }

        switch(utilisateur_actuel->profil)
        {
        case ROLE_PATIENT:
            ui_afficherMenuPatient();
            break;
        case ROLE_MEDECIN:
            ui_afficherMenuMedecin();
            break;
        case ROLE_INFIRMIER:
            ui_afficherMenuInfirmier();
            break;
        case ROLE_ADMINISTRATEUR:
            ui_afficherMenuAdministrateur();
            break;
        case ROLE_TECHNICIEN_LABO:
            ui_afficherMenuTechnicienLabo();
            break;
        case ROLE_PHARMACIEN:
            ui_afficherMenuPharmacien();
            break;
        case ROLE_SUPER_ADMIN:
            ui_afficherMenuSuperAdmin();
            break;
        default:
            ui_afficherMenuPatient();
        }

        printf("\n╔════════════════════════════════════════════════════════════════════════════════╗\n");
        printf("║                              Votre choix :                                     ║\n");
        printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
        printf("    ==> ");

        scanf("%d", &choix);
        viderBuffer();

        switch(utilisateur_actuel->profil)
        {
        // ==================== PATIENT ====================
        case ROLE_PATIENT:
            switch(choix)
            {
            case 1:
                ui_afficherEcranDossierMedical();
                pause();
                break;
            case 2:
                ui_afficherEcranMesConsultations();
                pause();
                break;
            case 3:
                ui_afficherEcranMesOrdonnances();
                pause();
                break;
            case 4:
                ui_afficherEcranMesAnalyses();
                pause();
                break;
            case 5:
                prendreRendezVousAvance();
                break;
            case 6:
                ui_afficherEcranMesRendezVous();
                listerRendezVousPatientAvance(utilisateur_actuel->id_associe);
                break;
            case 7:
            {
                int id_rdv;
                printf("\nID du rendez-vous a annuler: ");
                scanf("%d", &id_rdv);
                viderBuffer();
                annulerRendezVousAvance(id_rdv);
                break;
            }
            case 8:
                ui_afficherEcranMesFactures();
                afficherFacturesPatient(utilisateur_actuel->id_associe);
                break;
            case 9:
            {
                ui_afficherEcranMesFactures();
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
                    enregistrerPaiement(id_facture);
                else
                    ui_afficherMessageErreur("Facture non trouvee.");
                pause();
                break;
            }
            case 10:
                menuMessagerie(utilisateur_actuel->id_compte);
                break;
            case 11:
                afficherNotifications(utilisateur_actuel->id_compte);
                break;
            case 12:
                definirQuestionsSecurite(utilisateur_actuel->id_compte);
                break;
            case 13:
                modifierMotDePasse(utilisateur_actuel->id_compte);
                pause();
                break;
            case 14:
                ui_afficherAutoDiagnostic();
                break;  // ← NOUVEAU
            case 15:
                logout();
                return;
            default:
                ui_afficherMessageErreur("Choix invalide.");
                pause();
            }
            break;

        // ==================== MEDECIN ====================
        case ROLE_MEDECIN:
            switch(choix)
            {
            case 1:
                printf("Vous pouvez uniquement consulter les dossiers des patients qui vous sont associes.\n\n");
                afficherListePatientsPourMedecin(utilisateur_actuel->id_associe);
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
                    ui_afficherMessageErreur("Acces non autorise.");
                    pause();
                }
                break;
            case 6:
                if (peutGererDisponibilites(utilisateur_actuel->id_associe))
                    gererDisponibilitesMedecin(utilisateur_actuel->id_associe);
                else
                {
                    ui_afficherMessageErreur("Acces non autorise.");
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
                menuMedecinLaboratoire();
                break;
            case 10:
                menuMessagerie(utilisateur_actuel->id_compte);
                break;
            case 11:
                afficherNotifications(utilisateur_actuel->id_compte);
                break;
            case 12:
                ui_afficherEcranStatistiquesPersonnel();
                break;
            case 13:
                definirQuestionsSecurite(utilisateur_actuel->id_compte);
                break;
            case 14:
                modifierMotDePasse(utilisateur_actuel->id_compte);
                pause();
                break;
            case 15:
                logout();
                return;
            default:
                ui_afficherMessageErreur("Choix invalide.");
                pause();
            }
            break;

        // ==================== INFIRMIER ====================
        case ROLE_INFIRMIER:
            switch(choix)
            {
            case 1:
                afficherListePatientsSimple();
                pause();
                break;
            case 2:
                menuSoinsInfirmier();
                break;
            case 3:
                menuConstantesInfirmier();
                break;
            case 4:
                menuPlanningInfirmier();
                break;
            case 5:
                menuMessagerie(utilisateur_actuel->id_compte);
                break;
            case 6:
                afficherNotifications(utilisateur_actuel->id_compte);
                break;
            case 7:
                definirQuestionsSecurite(utilisateur_actuel->id_compte);
                break;
            case 8:
                modifierMotDePasse(utilisateur_actuel->id_compte);
                pause();
                break;
            case 9:
                logout();
                return;
            default:
                ui_afficherMessageErreur("Choix invalide.");
                pause();
            }
            break;

            // ==================== ADMINISTRATEUR ====================
            int choix_perso;
        case ROLE_ADMINISTRATEUR:

            switch(choix)
            {
            case 1:  // Gestion du personnel
                do
                {
                    ui_afficherEcranPersonnel();

                    printf("\n    Votre choix: ");
                    scanf("%d", &choix_perso);
                    viderBuffer();

                    switch(choix_perso)
                    {
                    case 1:  // Ajouter un employe
                        ajouterEmploye();
                        break;
                    case 2:  // Modifier un employe
                        if (nombreEmployes == 0)
                        {
                            ui_afficherMessageErreur("Aucun employe a modifier.");
                            pause();
                            break;
                        }
                        afficherListeEmployesSimple();
                        printf("\nEntrez l'ID de l'employe a modifier: ");
                        int id_modif;
                        scanf("%d", &id_modif);
                        viderBuffer();
                        int idx_modif = rechercherEmployeParID(id_modif);
                        if (idx_modif != -1)
                            modifierEmploye(idx_modif);
                        else
                            ui_afficherMessageErreur("Employe non trouve.");
                        break;
                    case 3:  // Supprimer un employe
                        if (nombreEmployes == 0)
                        {
                            ui_afficherMessageErreur("Aucun employe a supprimer.");
                            pause();
                            break;
                        }
                        afficherListeEmployesSimple();
                        printf("\nEntrez l'ID de l'employe a supprimer: ");
                        int id_supp;
                        scanf("%d", &id_supp);
                        viderBuffer();
                        int idx_supp = rechercherEmployeParID(id_supp);
                        if (idx_supp != -1)
                            supprimerEmploye(idx_supp);
                        else
                            ui_afficherMessageErreur("Employe non trouve.");
                        break;
                    case 4:  // Rechercher un employe
                        rechercherEmploye();
                        break;
                    case 5:  // Lister tous les employes
                        ui_afficherListePersonnel();
                        pause();
                        break;
                    case 6:  // Gerer les conges
                        if (nombreEmployes == 0)
                        {
                            ui_afficherMessageErreur("Aucun employe enregistre.");
                            pause();
                            break;
                        }
                        afficherListeEmployesSimple();
                        printf("\nEntrez l'ID de l'employe: ");
                        int id_conge;
                        scanf("%d", &id_conge);
                        viderBuffer();
                        int idx_conge = rechercherEmployeParID(id_conge);
                        if (idx_conge != -1)
                            gererConges(idx_conge);
                        else
                            ui_afficherMessageErreur("Employe non trouve.");
                        break;
                    case 7:  // Statistiques
                        ui_afficherEcranStatistiquesPersonnel();
                        break;
                    case 8:  // Retour
                        break;
                    default:
                        ui_afficherMessageErreur("Choix invalide.");
                    }
                }
                while(choix_perso!= 8);
                break;

            case 2:  // Gestion des patients
                menuGestionPatients();
                break;
            case 3:  // Gestion des rendez-vous
                menuRendezVousAvance();
                break;
            case 4:  // Gestion des consultations
                menuConsultations();
                break;
            case 5:  // Gestion des comptes utilisateurs
                gererComptes();
                break;
            case 6:  // Facturation
                menuFacturation();
                break;
            case 7:  // Archives
                menuArchives();
                break;
            case 8:  // Statistiques avancees
                statistiquesAvancees();
                break;
            case 9:  // Messagerie
                menuMessagerie(utilisateur_actuel->id_compte);
                break;
            case 10: // Notifications
                afficherNotifications(utilisateur_actuel->id_compte);
                break;
            case 11: // Questions de securite
                definirQuestionsSecurite(utilisateur_actuel->id_compte);
                break;
            case 12: // Modifier mot de passe
                modifierMotDePasse(utilisateur_actuel->id_compte);
                pause();
                break;
            case 13: // Deconnexion
                logout();
                return;
            default:
                ui_afficherMessageErreur("Choix invalide.");
                pause();
            }
            break;

        // ==================== TECHNICIEN LABO ====================
        case ROLE_TECHNICIEN_LABO:
            switch(choix)
            {
            case 1:
                afficherAnalysesARealiser();
                break;
            case 2:
            {
                int compteur = 0, ids_analyse[MAX_ANALYSES];
                for (int i = 0; i < nombreAnalyses; i++)
                {
                    if (strcmp(analyses[i].statut, "En cours") == 0)
                    {
                        ids_analyse[compteur++] = analyses[i].id_analyse;
                    }
                }
                if (compteur == 0)
                {
                    printf("Aucune analyse en cours.\n");
                    pause();
                    break;
                }
                printf("\nID de l'analyse: ");
                int id_a;
                scanf("%d", &id_a);
                viderBuffer();
                int trouve = 0;
                for (int i = 0; i < compteur; i++)
                    if (ids_analyse[i] == id_a)
                    {
                        trouve = 1;
                        break;
                    }
                if (trouve) saisirResultatAnalyse(id_a);
                else printf("Analyse non trouvee.\n");
                pause();
            }
            break;
            case 3:
            {
                int choix_histo;
                printf("\n1. Consulter une analyse\n2. Voir analyses realisees\n3. Voir analyses validees\n4. Statistiques\nChoix: ");
                scanf("%d", &choix_histo);
                viderBuffer();
                if (choix_histo == 1)
                {
                    int id_ana;
                    printf("ID analyse: ");
                    scanf("%d", &id_ana);
                    viderBuffer();
                    afficherAnalyse(id_ana);
                    pause();
                }
                else if (choix_histo == 2 || choix_histo == 3)
                    statistiquesLaboratoire();
                else if (choix_histo == 4)
                    statistiquesLaboratoire();
            }
            break;
            case 4:
                menuMessagerie(utilisateur_actuel->id_compte);
                break;
            case 5:
                afficherNotifications(utilisateur_actuel->id_compte);
                break;
            case 6:
                definirQuestionsSecurite(utilisateur_actuel->id_compte);
                break;
            case 7:
                modifierMotDePasse(utilisateur_actuel->id_compte);
                ui_afficherMessageSucces("Mot de passe modifie.");
                pause();
                break;
            case 8:
                logout();
                return;
            default:
                ui_afficherMessageErreur("Choix invalide.");
                pause();
            }
            break;

        // ==================== PHARMACIEN ====================
        case ROLE_PHARMACIEN:
            switch(choix)
            {
            case 1:
                menuOrdonnances();
                break;
            case 2:
                menuGestionStock();
                break;
            case 3:
                if (nombreOrdonnances == 0)
                {
                    ui_afficherMessageErreur("Aucune ordonnance.");
                    pause();
                    break;
                }
                printf("\nID ordonnance: ");
                int id_ord;
                scanf("%d", &id_ord);
                viderBuffer();
                printf("ID patient: ");
                int id_pat;
                scanf("%d", &id_pat);
                viderBuffer();
                delivrerMedicament(id_ord, id_pat);
                break;
            case 4:
                alerteStockFaible();
                break;
            case 5:
                menuMessagerie(utilisateur_actuel->id_compte);
                break;
            case 6:
                afficherNotifications(utilisateur_actuel->id_compte);
                break;
            case 7:
                definirQuestionsSecurite(utilisateur_actuel->id_compte);
                break;
            case 8:
                modifierMotDePasse(utilisateur_actuel->id_compte);
                pause();
                break;
            case 9:
                logout();
                return;
            default:
                ui_afficherMessageErreur("Choix invalide.");
                pause();
            }
            break;

        // ==================== SUPER ADMIN ====================
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
                menuAdminLaboratoire();
                break;
            case 10:
                statistiquesAvancees();
                break;
            case 11:
                configurationSysteme();
                break;
            case 12:
                menuRendezVousAvance();
                break;
            case 13:
                menuMessagerie(utilisateur_actuel->id_compte);
                break;
            case 14:
                afficherNotifications(utilisateur_actuel->id_compte);
                break;
            case 15:
                definirQuestionsSecurite(utilisateur_actuel->id_compte);
                break;
            case 16:
                modifierMotDePasse(utilisateur_actuel->id_compte);
                pause();
                break;
            case 17:
                logout();
                return;
            default:
                ui_afficherMessageErreur("Choix invalide.");
                pause();
            }
            break;

        default:
            ui_afficherMessageErreur("Profil non reconnu.");
            pause();
        }
    }
    while(1);
}

void afficherMenuAuthentification()
{
    ui_afficherMenuAuthentification();
}

void creerNouveauCompte()
{
    system("cls");
    ui_setColor(UI_COLOR_TITLE);
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                    CREATION D'UN COMPTE                       ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
    printf("\nTYPE DE COMPTE:\n");
    printf("1. Patient (deja enregistre)\n");
    printf("2. Nouveau patient\n");
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
    case 1:
        if (nombrePatients == 0)
        {
            ui_afficherMessageErreur("Aucun patient.");
            pause();
            return;
        }
        afficherListePatientsSimple();
        printf("\nID patient: ");
        int id_pat;
        scanf("%d", &id_pat);
        viderBuffer();
        int index = rechercherParID(id_pat);
        if (index != -1)
        {
            int existe = 0;
            for (int i = 0; i < nombreComptes; i++)
                if (comptes[i].id_associe == id_pat && comptes[i].profil == ROLE_PATIENT)
                {
                    existe = 1;
                    break;
                }
            if (existe) ui_afficherMessageErreur("Compte existe deja.");
            else
            {
                creerComptePatient(id_pat);
                ui_afficherMessageSucces("Compte cree !");
            }
        }
        else ui_afficherMessageErreur("Patient non trouve.");
        pause();
        break;
    case 2:
        creerComptePatientAuto();
        break;
    case 3:
        if (nombreEmployes == 0)
        {
            ui_afficherMessageErreur("Aucun medecin.");
            pause();
            return;
        }
        afficherListeEmployesSimple();
        printf("\nID medecin: ");
        int id_med;
        scanf("%d", &id_med);
        viderBuffer();
        int idx = rechercherEmployeParID(id_med);
        if (idx != -1 && strcmp(personnel[idx].fonction, "Medecin") == 0)
        {
            int existe = 0;
            for (int i = 0; i < nombreComptes; i++)
                if (comptes[i].id_associe == id_med && comptes[i].profil == ROLE_MEDECIN)
                {
                    existe = 1;
                    break;
                }
            if (existe) ui_afficherMessageErreur("Compte existe deja.");
            else
            {
                creerComptePersonnel(id_med, ROLE_MEDECIN);
                ui_afficherMessageSucces("Compte cree !");
            }
        }
        else ui_afficherMessageErreur("Medecin non trouve.");
        pause();
        break;
    case 4:
        if (nombreEmployes == 0)
        {
            ui_afficherMessageErreur("Aucun infirmier.");
            pause();
            return;
        }
        afficherListeEmployesSimple();
        printf("\nID infirmier: ");
        int id_inf;
        scanf("%d", &id_inf);
        viderBuffer();
        int idx2 = rechercherEmployeParID(id_inf);
        if (idx2 != -1 && strcmp(personnel[idx2].fonction, "Infirmier") == 0)
        {
            int existe = 0;
            for (int i = 0; i < nombreComptes; i++)
                if (comptes[i].id_associe == id_inf && comptes[i].profil == ROLE_INFIRMIER)
                {
                    existe = 1;
                    break;
                }
            if (existe) ui_afficherMessageErreur("Compte existe deja.");
            else
            {
                creerComptePersonnel(id_inf, ROLE_INFIRMIER);
                ui_afficherMessageSucces("Compte cree !");
            }
        }
        else ui_afficherMessageErreur("Infirmier non trouve.");
        pause();
        break;
    case 5:
        if (nombreEmployes == 0)
        {
            ui_afficherMessageErreur("Aucun technicien.");
            pause();
            return;
        }
        afficherListeEmployesSimple();
        printf("\nID technicien: ");
        int id_tech;
        scanf("%d", &id_tech);
        viderBuffer();
        int idx3 = rechercherEmployeParID(id_tech);
        if (idx3 != -1 && strcmp(personnel[idx3].fonction, "Technicien") == 0)
        {
            int existe = 0;
            for (int i = 0; i < nombreComptes; i++)
                if (comptes[i].id_associe == id_tech && comptes[i].profil == ROLE_TECHNICIEN_LABO)
                {
                    existe = 1;
                    break;
                }
            if (existe) ui_afficherMessageErreur("Compte existe deja.");
            else
            {
                creerComptePersonnel(id_tech, ROLE_TECHNICIEN_LABO);
                ui_afficherMessageSucces("Compte cree !");
            }
        }
        else ui_afficherMessageErreur("Technicien non trouve.");
        pause();
        break;
    case 6:
        if (nombreEmployes == 0)
        {
            ui_afficherMessageErreur("Aucun pharmacien.");
            pause();
            return;
        }
        afficherListeEmployesSimple();
        printf("\nID pharmacien: ");
        int id_pharm;
        scanf("%d", &id_pharm);
        viderBuffer();
        int idx4 = rechercherEmployeParID(id_pharm);
        if (idx4 != -1 && strcmp(personnel[idx4].fonction, "Pharmacien") == 0)
        {
            int existe = 0;
            for (int i = 0; i < nombreComptes; i++)
                if (comptes[i].id_associe == id_pharm && comptes[i].profil == ROLE_PHARMACIEN)
                {
                    existe = 1;
                    break;
                }
            if (existe) ui_afficherMessageErreur("Compte existe deja.");
            else
            {
                creerComptePersonnel(id_pharm, ROLE_PHARMACIEN);
                ui_afficherMessageSucces("Compte cree !");
            }
        }
        else ui_afficherMessageErreur("Pharmacien non trouve.");
        pause();
        break;
    case 7:
        return;
    default:
        ui_afficherMessageErreur("Choix invalide.");
        pause();
    }
}

int main(int argc, char* argv[])
{
    activerConsoleUTF8();
    system("cls");
    ui_setColor(UI_COLOR_TITLE);
    printf("\n+--------------------------------------------------+\n");
    printf("|                    CHARGEMENT...                 |\n");
    printf("+--------------------------------------------------+\n");
    ui_resetColor();

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

    printf("  - Initialisation de la pharmacie... ");
    initialiserPharmacie();
    printf("OK\n");

    printf("  - Initialisation des comptes... ");
    initialiserComptes();
    printf("OK\n");

    printf("  - Initialisation du laboratoire... ");
    initialiserLaboratoire();
    printf("OK\n");

    printf("  - Initialisation du module infirmier... ");
    initialiserInfirmier();
    printf("OK\n");

    printf("  - Initialisation de la recuperation... ");
    initialiserRecuperation();
    printf("OK\n");

    printf("  - Initialisation des notifications... ");
    initialiserNotifications();
    printf("OK\n");

    printf("  - Initialisation de la messagerie... ");
    initialiserMessagerie();
    printf("OK\n");

    printf("\nCHARGEMENT DES DONNEES...\n");

    printf("  - Chargement des patients... ");
    chargerDernierID();
    chargerPatients();
    printf("%d patient(s)\n", nombrePatients);

    printf("  - Chargement du personnel... ");
    chargerDernierIDPersonnel();
    chargerPersonnel();
    printf("%d employe(s)\n", nombreEmployes);

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
    printf("%d intervention(s), %d prise(s)\n", nombreInterventions, nombrePrisesEnCharge);

    printf("  - Chargement des factures... ");
    chargerFactures();
    printf("%d facture(s)\n", nombreFactures);

    printf("  - Chargement des archives... ");
    chargerArchives();
    printf("%d archive(s)\n", nombreArchives);

    printf("  - Chargement des comptes... ");
    chargerComptes();
    printf("%d compte(s) actif(s)\n", nombreComptes);

    printf("  - Chargement des notifications... ");
    chargerNotifications();
    printf("OK\n");

    printf("  - Chargement des messages... ");
    chargerMessages();
    printf("OK\n");

    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                       SYSTEME PRET                           ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    Sleep(2000);

    int choix_auth;
    while (1)
    {
        afficherMenuAuthentification();
        scanf("%d", &choix_auth);
        viderBuffer();

        switch(choix_auth)
        {
        case 1:
            if (login()) afficherMenuPrincipal();
            break;
        case 2:
            creerNouveauCompte();
            break;
        case 3:
            menuMotDePasseOublie();
            break;
        case 4:
            ui_afficherMessageSucces("Au revoir!");
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
            sauvegarderMedicaments();
            sauvegarderVentes();
            sauvegarderConstantes();
            sauvegarderSoins();
            sauvegarderPlannings();
            sauvegarderReponsesSecurite();
            sauvegarderNotifications();
            sauvegarderMessages();
            return 0;
        default:
            ui_afficherMessageErreur("Choix invalide.");
            pause();
        }
    }
}

