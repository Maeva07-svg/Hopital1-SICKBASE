#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "ui.h"
#include "comptes.h"
#include "consultations.h"
#include "notification.h"
#include "messagerie.h"
#include "patient.h"
#include "personnel.h"
#include "diagnostic.h"

// ==================== FONCTIONS DE BASE ====================

void ui_setColor(int couleur) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, couleur);
}

void ui_resetColor() {
    ui_setColor(7);
}

void ui_afficherMessageSucces(char *message) {
    ui_setColor(UI_COLOR_SUCCESS);
    printf("\n[SUCCES] %s\n", message);
    ui_resetColor();
}

void ui_afficherMessageErreur(char *message) {
    ui_setColor(UI_COLOR_ERROR);
    printf("\n[ERREUR] %s\n", message);
    ui_resetColor();
}

void ui_afficherMessageInfo(char *message) {
    ui_setColor(UI_COLOR_INFO);
    printf("\n[INFO] %s\n", message);
    ui_resetColor();
}

void ui_afficherEntete() {
    system("cls");
    ui_setColor(UI_COLOR_TITLE);
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                    SICKBASE                                    ║\n");
    printf("║                         SYSTEME DE GESTION HOSPITALIERE                        ║\n");
    printf("║                                   VERSION 3.3                                  ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
    printf("\n");
}

void ui_afficherInfoUtilisateur() {
    if (utilisateur_actuel == NULL) return;

    char profilNom[30] = "";
    int profilCouleur = 7;

    switch(utilisateur_actuel->profil) {
        case ROLE_PATIENT: strcpy(profilNom, "PATIENT"); profilCouleur = UI_COLOR_PATIENT; break;
        case ROLE_MEDECIN: strcpy(profilNom, "MEDECIN"); profilCouleur = UI_COLOR_MEDECIN; break;
        case ROLE_INFIRMIER: strcpy(profilNom, "INFIRMIER"); profilCouleur = UI_COLOR_INFIRMIER; break;
        case ROLE_ADMINISTRATEUR: strcpy(profilNom, "ADMINISTRATEUR"); profilCouleur = UI_COLOR_ADMIN; break;
        case ROLE_TECHNICIEN_LABO: strcpy(profilNom, "TECHNICIEN LABO"); profilCouleur = UI_COLOR_LABO; break;
        case ROLE_PHARMACIEN: strcpy(profilNom, "PHARMACIEN"); profilCouleur = UI_COLOR_PHARMA; break;
        case ROLE_SUPER_ADMIN: strcpy(profilNom, "SUPER ADMIN"); profilCouleur = UI_COLOR_SUPER; break;
        default: strcpy(profilNom, "INCONNU");
    }

    ui_setColor(8);
    printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║ "); ui_setColor(UI_COLOR_TITLE); printf("UTILISATEUR : %s", utilisateur_actuel->login); ui_resetColor();
    printf("                                                            ║\n");
    printf("║ "); ui_setColor(UI_COLOR_TITLE); printf("PROFIL      : "); ui_resetColor();
    ui_setColor(profilCouleur); printf("%s", profilNom); ui_resetColor();
    printf("                                                      ║\n");
    printf("║ "); ui_setColor(UI_COLOR_TITLE); printf("ID          : %d", utilisateur_actuel->id_compte); ui_resetColor();
    printf("                                                                ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
}

// ==================== MENU AUTHENTIFICATION ====================

void ui_afficherMenuAuthentification() {
    system("cls");
    ui_afficherEntete();

    ui_setColor(UI_COLOR_MENU_OPTION);
    printf("\n╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                            AUTHENTIFICATION                                    ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║                                                                                ║\n");
    printf("║                         [1]  Se connecter                                      ║\n");
    printf("║                         [2]  Creer un compte                                   ║\n");
    printf("║                         [3]  Mot de passe oublie                               ║\n");
    printf("║                         [4]  Quitter                                           ║\n");
    printf("║                                                                                ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
    printf("\n                          Votre choix : ");
}

// ==================== MENU PATIENT ====================

void ui_afficherMenuPatient() {
    ui_setColor(UI_COLOR_PATIENT);
    printf("\n╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                           MENU PATIENT                                         ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║                                                                                ║\n");
    printf("║     [ 1]  Voir mon dossier medical                                             ║\n");
    printf("║     [ 2]  Voir mes consultations                                               ║\n");
    printf("║     [ 3]  Voir mes ordonnances                                                 ║\n");
    printf("║     [ 4]  Voir mes analyses                                                    ║\n");
    printf("║     [ 5]  Prendre rendez-vous                                                  ║\n");
    printf("║     [ 6]  Voir mes rendez-vous                                                 ║\n");
    printf("║     [ 7]  Annuler un rendez-vous                                               ║\n");
    printf("║     [ 8]  Voir mes factures                                                    ║\n");
    printf("║     [ 9]  Payer une facture                                                    ║\n");
    printf("║     [10]  Messagerie                                                           ║\n");
    printf("║     [11]  Notifications                                                        ║\n");
    printf("║     [12]  Questions de securite                                                ║\n");
    printf("║     [13]  Modifier mon mot de passe                                            ║\n");
    printf("║     [14]  [AUTO-DIAGNOSTIC] - Evaluer mes symptomes                            ║\n");
    printf("║     [15]  Deconnexion                                                          ║\n");
    printf("║                                                                                ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
}

// ==================== MENU TECHNICIEN LABO ====================

void ui_afficherMenuTechnicienLabo() {
    ui_setColor(UI_COLOR_LABO);
    printf("\n╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                      MENU TECHNICIEN LABORATOIRE                               ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║                                                                                ║\n");
    printf("║     [ 1]  Liste des analyses a realiser                                        ║\n");
    printf("║     [ 2]  Analyses en cours                                                    ║\n");
    printf("║     [ 3]  Menu d'analyses                                                      ║\n");
    printf("║     [ 4]  Messagerie                                                           ║\n");
    printf("║     [ 5]  Notifications                                                        ║\n");
    printf("║     [ 6]  Questions de securite                                                ║\n");
    printf("║     [ 7]  Modifier mot de passe                                                ║\n");
    printf("║     [ 8]  Deconnexion                                                          ║\n");
    printf("║                                                                                ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
}

// ==================== MENU INFIRMIER ====================

void ui_afficherMenuInfirmier() {
    ui_setColor(UI_COLOR_INFIRMIER);
    printf("\n╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                           MENU INFIRMIER                                       ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║                                                                                ║\n");
    printf("║     [ 1]  Liste des patients                                                   ║\n");
    printf("║     [ 2]  Menu des soins                                                       ║\n");
    printf("║     [ 3]  Constantes vitales                                                   ║\n");
    printf("║     [ 4]  Planning des soins                                                   ║\n");
    printf("║     [ 5]  Messagerie                                                           ║\n");
    printf("║     [ 6]  Notifications                                                        ║\n");
    printf("║     [ 7]  Questions de securite                                                ║\n");
    printf("║     [ 8]  Modifier mot de passe                                                ║\n");
    printf("║     [ 9]  Deconnexion                                                          ║\n");
    printf("║                                                                                ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
}

// ==================== MENU ADMINISTRATEUR ====================

void ui_afficherMenuAdministrateur() {
    ui_setColor(UI_COLOR_ADMIN);
    printf("\n╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                         MENU ADMINISTRATEUR                                    ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║                                                                                ║\n");
    printf("║     [ 1]  Gestion des patients                                                 ║\n");
    printf("║     [ 2]  Gestion du personnel                                                 ║\n");
    printf("║     [ 3]  Gestion des rendez-vous                                              ║\n");
    printf("║     [ 4]  Gestion des consultations                                            ║\n");
    printf("║     [ 5]  Gestion des ordonnances                                              ║\n");
    printf("║     [ 6]  Facturation                                                          ║\n");
    printf("║     [ 7]  Archives                                                             ║\n");
    printf("║     [ 8]  Statistiques                                                         ║\n");
    printf("║     [ 9]  Messagerie                                                           ║\n");
    printf("║     [10]  Notifications                                                        ║\n");
    printf("║     [11]  Questions de securite                                                ║\n");
    printf("║     [12]  Modifier mot de passe                                                ║\n");
    printf("║     [13]  Deconnexion                                                          ║\n");
    printf("║                                                                                ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
}

// ==================== MENU PHARMACIEN ====================

void ui_afficherMenuPharmacien() {
    ui_setColor(UI_COLOR_PHARMA);
    printf("\n╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                           MENU PHARMACIEN                                      ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║                                                                                ║\n");
    printf("║     [ 1]  Gestion des ordonnances                                              ║\n");
    printf("║     [ 2]  Gestion du stock                                                     ║\n");
    printf("║     [ 3]  Delivrance medicaments                                               ║\n");
    printf("║     [ 4]  Alertes stock faible                                                 ║\n");
    printf("║     [ 5]  Messagerie                                                           ║\n");
    printf("║     [ 6]  Notifications                                                        ║\n");
    printf("║     [ 7]  Questions de securite                                                ║\n");
    printf("║     [ 8]  Modifier mot de passe                                                ║\n");
    printf("║     [ 9]  Deconnexion                                                          ║\n");
    printf("║                                                                                ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
}

// ==================== MENU SUPER ADMIN ====================

void ui_afficherMenuSuperAdmin() {
    ui_setColor(UI_COLOR_SUPER);
    printf("\n╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                       MENU SUPER ADMINISTRATEUR                                ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║                                                                                ║\n");
    printf("║     [ 1]  Gestion complete patients                                            ║\n");
    printf("║     [ 2]  Gestion complete personnel                                           ║\n");
    printf("║     [ 3]  Gestion consultations                                                ║\n");
    printf("║     [ 4]  Gestion ordonnances                                                  ║\n");
    printf("║     [ 5]  Gestion interventions                                                ║\n");
    printf("║     [ 6]  Gestion comptes utilisateurs                                         ║\n");
    printf("║     [ 7]  Facturation                                                          ║\n");
    printf("║     [ 8]  Archives                                                             ║\n");
    printf("║     [ 9]  Gestion laboratoire                                                  ║\n");
    printf("║     [10]  Statistiques avancees                                                ║\n");
    printf("║     [11]  Configuration systeme                                                ║\n");
    printf("║     [12]  Gestion des rendez-vous                                              ║\n");
    printf("║     [13]  Messagerie                                                           ║\n");
    printf("║     [14]  Notifications                                                        ║\n");
    printf("║     [15]  Questions de securite                                                ║\n");
    printf("║     [16]  Modifier mot de passe                                                ║\n");
    printf("║     [17]  Deconnexion                                                          ║\n");
    printf("║                                                                                ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
}


// ==================== FONCTIONS MEDECIN ====================

void ui_afficherMenuMedecin() {
    ui_setColor(UI_COLOR_MEDECIN);
    printf("\n╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                           MENU MEDECIN                                         ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║                                                                                ║\n");
    printf("║     [ 1]  Gestion des patients                                                 ║\n");
    printf("║     [ 2]  Consultations                                                        ║\n");
    printf("║     [ 3]  Prescriptions                                                        ║\n");
    printf("║     [ 4]  Diagnostic medical                                                   ║\n");
    printf("║     [ 5]  Rendez-vous (Planning)                                               ║\n");
    printf("║     [ 6]  Mes disponibilites                                                   ║\n");
    printf("║     [ 7]  Mes rendez-vous                                                      ║\n");
    printf("║     [ 8]  Facturation                                                          ║\n");
    printf("║     [ 9]  Demandes d'analyses                                                  ║\n");
    printf("║     [10]  Messagerie                                                           ║\n");
    printf("║     [11]  Notifications                                                        ║\n");
    printf("║     [12]  Statistiques                                                         ║\n");
    printf("║     [13]  Questions de securite                                                ║\n");
    printf("║     [14]  Modifier mot de passe                                                ║\n");
    printf("║     [15]  Deconnexion                                                          ║\n");
    printf("║                                                                                ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
}

void ui_afficherEcranStatistiques() {
    ui_afficherEntete();
    ui_setColor(UI_COLOR_TITLE);
    printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                           STATISTIQUES                                         ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    printf("║                                                                                ║\n");
    printf("║    ┌────────────────────────────────────────────────────────────────────────┐  ║\n");
    printf("║    │                         PATIENTS                                       │  ║\n");
    printf("║    ├────────────────────────────────────────────────────────────────────────┤  ║\n");
    printf("║    │   Total patients : %-53d │ ║\n", nombrePatients);
    printf("║    └────────────────────────────────────────────────────────────────────────┘  ║\n");
    printf("║                                                                                ║\n");
    printf("║    ┌────────────────────────────────────────────────────────────────────────┐  ║\n");
    printf("║    │                         PERSONNEL                                      │  ║\n");
    printf("║    ├────────────────────────────────────────────────────────────────────────┤  ║\n");
    printf("║    │   Total employes : %-52d │ ║\n", nombreEmployes);
    printf("║    └────────────────────────────────────────────────────────────────────────┘  ║\n");
    printf("║                                                                                ║\n");
    printf("║    ┌────────────────────────────────────────────────────────────────────────┐  ║\n");
    printf("║    │                         CONSULTATIONS                                  │  ║\n");
    printf("║    ├────────────────────────────────────────────────────────────────────────┤  ║\n");
    printf("║    │   Total consultations : %-50d │ ║\n", nombreConsultations);
    printf("║    └────────────────────────────────────────────────────────────────────────┘  ║\n");
    printf("║                                                                                ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
}

// ==================== AUTO-DIAGNOSTIC PATIENT ====================

void autoDiagnosticParQuestions()
{
    system("cls");
    ui_afficherEntete();
    ui_setColor(UI_COLOR_TITLE);
    printf("\n╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                    AUTO-DIAGNOSTIC - QUESTIONNAIRE MEDICAL                     ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    ui_resetColor();
    printf("║                                                                                ║\n");
    printf("║  Repondez par O (Oui) ou N (Non)                                               ║\n");
    printf("║  Ce diagnostic est purement indicatif et ne sera pas enregistre.               ║\n");
    printf("║                                                                                ║\n");

    const char *questions[] = {
        "Avez-vous de la fievre elevee (plus de 39C) ?",
        "Avez-vous des frissons ?",
        "Avez-vous des maux de tete intenses ?",
        "Avez-vous une toux seche ?",
        "Avez-vous du mal a respirer ?",
        "Avez-vous perdu le gout ou l'odorat ?",
        "Avez-vous des douleurs musculaires ?",
        "Avez-vous des nausees ou vomissements ?",
        "Avez-vous mal a la gorge ?",
        "Avez-vous des douleurs abdominales ?",
        "Avez-vous la diarrhee ?",
        "Ressentez-vous une fatigue intense ?"
    };
    int nb_questions = 12;
    int scores[10] = {0}; // Pour 10 maladies

    printf("║                                                                                ║\n");
    for (int i = 0; i < nb_questions; i++) {
        printf("║  %d. %-60s ", i+1, questions[i]);
        char rep;
        scanf(" %c", &rep);
        viderBuffer();

        // Score pour différentes maladies
        if (toupper(rep) == 'O') {
            if (i == 0 || i == 1) scores[0]++;  // Paludisme
            if (i == 3 || i == 5) scores[1]++;  // COVID
            if (i == 8) scores[2]++;             // Angine
            if (i == 9 || i == 10) scores[3]++;  // Gastro
            if (i == 2) scores[4]++;              // Migraine
            if (i == 4) scores[5]++;              // Problème respiratoire
            if (i == 6) scores[6]++;              // Grippe
            if (i == 11) scores[7]++;             // Fatigue chronique
        }
    }

    printf("║                                                                                ║\n");
    ui_setColor(UI_COLOR_INFO);
    printf("║  ┌────────────────────────────────────────────────────────────────────────┐    ║\n");
    printf("║  │                         RESULTATS INDICATIFS                           │    ║\n");
    printf("║  ├────────────────────────────────────────────────────────────────────────┤    ║\n");
    ui_resetColor();

    const char *maladies[] = {
        "Paludisme", "COVID-19", "Angine", "Gastro-enterite",
        "Migraine", "Infection respiratoire", "Grippe", "Fatigue chronique"
    };

    int trouve = 0;
    for (int i = 0; i < 8; i++) {
        if (scores[i] > 0) {
            trouve = 1;
            printf("║  │  - %-20s : %d symptome(s) correspondant(s)                     │    ║\n",
                   maladies[i], scores[i]);
        }
    }

    if (!trouve) {
        printf("║  │  Aucune correspondance significative. Consultez un medecin.        │    ║\n");
    }

    printf("║  └────────────────────────────────────────────────────────────────────────┘    ║\n");
    ui_setColor(UI_COLOR_WARNING);
    printf("║                                                                                ║\n");
    printf("║  [ATTENTION] Ce diagnostic est purement indicatif.                             ║\n");
    printf("║  Pour un diagnostic fiable, consultez un medecin.                              ║\n");
    ui_resetColor();
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");

    printf("\n    Appuyez sur ENTREE pour continuer...");
    getchar();
    pause();
}

void autoDiagnosticParSymptomes()
{
    system("cls");
    ui_afficherEntete();
    ui_setColor(UI_COLOR_TITLE);
    printf("\n╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                    AUTO-DIAGNOSTIC - ANALYSE DES SYMPTOMES                     ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    ui_resetColor();
    printf("║                                                                                ║\n");
    printf("║  Entrez vos symptomes (un par ligne). Tapez 'fin' pour terminer.               ║\n");
    printf("║  Ce diagnostic est purement indicatif et ne sera pas enregistre.               ║\n");
    printf("║                                                                                ║\n");

    char symptomes[20][100];
    int nb = 0;

    for (int i = 0; i < 20; i++) {
        printf("║  Symptome %d: ", i+1);
        fgets(symptomes[i], 100, stdin);
        symptomes[i][strcspn(symptomes[i], "\n")] = '\0';
        if (strcmp(symptomes[i], "fin") == 0) break;
        if (strlen(symptomes[i]) > 0) nb++;
    }

    printf("║                                                                                ║\n");
    ui_setColor(UI_COLOR_INFO);
    printf("║  ┌────────────────────────────────────────────────────────────────────────┐    ║\n");
    printf("║  │                         ANALYSE DES SYMPTOMES                          │    ║\n");
    printf("║  ├────────────────────────────────────────────────────────────────────────┤    ║\n");
    ui_resetColor();

    // Maladies prédéfinies
    const char *maladies_symptomes[][5] = {
        {"fievre", "frisson", "sueur", "fatigue", "tete"},           // Paludisme
        {"fievre", "toux seche", "fatigue", "gorge", "respiration"}, // COVID
        {"gorge", "avaler", "fievre", "ganglion", "toux"},            // Angine
        {"vomissement", "nausee", "diarrhee", "estomac", "crampe"}    // Gastro
    };
    const char *maladies_noms[] = {"Paludisme", "COVID-19", "Angine", "Gastro-enterite"};

    for (int m = 0; m < 4; m++) {
        int correspondances = 0;
        for (int s1 = 0; s1 < nb; s1++) {
            for (int s2 = 0; s2 < 5; s2++) {
                if (strlen(maladies_symptomes[m][s2]) > 0 &&
                    strstr(strlwr(symptomes[s1]), maladies_symptomes[m][s2]) != NULL) {
                    correspondances++;
                    break;
                }
            }
        }
        if (correspondances > 0) {
            printf("║  │  - %-20s : %d symptome(s) correspondant(s)                 │    ║\n",
                   maladies_noms[m], correspondances);
        }
    }

    printf("║  └────────────────────────────────────────────────────────────────────────┘    ║\n");
    ui_setColor(UI_COLOR_WARNING);
    printf("║                                                                                ║\n");
    printf("║  [ATTENTION] Ce diagnostic est purement indicatif.                             ║\n");
    printf("║  Pour un diagnostic fiable, consultez un medecin.                              ║\n");
    ui_resetColor();
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");

    printf("\n    Appuyez sur ENTREE pour continuer...");
    getchar();
    pause();
}

void autoDiagnosticParCorps()
{
    // Appel à la fonction de diagnostic corporel en mode libre (sans enregistrement)
    lancerDiagnosticCorporel(-1, 0);
}

void ui_afficherAutoDiagnostic()
{
    int choix;
    do {
        system("cls");
        ui_afficherEntete();
        ui_setColor(UI_COLOR_PATIENT);
        printf("\n╔════════════════════════════════════════════════════════════════════════════════╗\n");
        printf("║                         AUTO-DIAGNOSTIC MEDICAL                                ║\n");
        printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
        ui_resetColor();
        printf("║                                                                                ║\n");
        printf("║     [1]  Diagnostic par questionnaire medical                                  ║\n");
        printf("║     [2]  Diagnostic par analyse des symptomes                                  ║\n");
        printf("║     [3]  Diagnostic par zone corporelle (interface interactive)                ║\n");
        printf("║     [4]  Retour au menu principal                                              ║\n");
        printf("║                                                                                ║\n");
        ui_setColor(UI_COLOR_WARNING);
        printf("║     [INFO] Ces diagnostics sont PUREMENT INDICATIFS.                           ║\n");
        printf("║            Ils ne sont pas enregistres dans votre dossier medical.             ║\n");
        ui_resetColor();
        printf("║                                                                                ║\n");
        ui_setColor(8);
        printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
        ui_resetColor();
        printf("\n    Votre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix) {
            case 1:
                autoDiagnosticParQuestions();
                break;
            case 2:
                autoDiagnosticParSymptomes();
                break;
            case 3:
                autoDiagnosticParCorps();
                break;
            case 4:
                return;
            default:
                ui_afficherMessageErreur("Choix invalide.");
                pause();
        }
    } while(choix != 4);
}

// ==================== MENU PRINCIPAL ====================

void ui_afficherMenuPrincipal() {
    if (utilisateur_actuel == NULL) return;

    ui_afficherEntete();
    ui_afficherInfoUtilisateur();

    // Notifications
    if (hasNotificationsNonLues(utilisateur_actuel->id_compte) || hasNouveauxMessages(utilisateur_actuel->id_compte)) {
        ui_setColor(UI_COLOR_WARNING);
        printf("\n╔════════════════════════════════════════════════════════════════════════════════╗\n");
        printf("║                          NOUVELLES NOTIFICATIONS                               ║\n");
        printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
        printf("║                                                                                ║\n");
        if (hasNotificationsNonLues(utilisateur_actuel->id_compte)) {
            printf("║     [!] Vous avez des notifications non lues !                             ║\n");
        }
        if (hasNouveauxMessages(utilisateur_actuel->id_compte)) {
            printf("║     [i] Vous avez des nouveaux messages !                                  ║\n");
        }
        printf("║                                                                                ║\n");
        printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
        ui_resetColor();
    }

    switch(utilisateur_actuel->profil) {
        case ROLE_PATIENT: ui_afficherMenuPatient(); break;
        case ROLE_MEDECIN: ui_afficherMenuMedecin(); break;
        case ROLE_INFIRMIER: ui_afficherMenuInfirmier(); break;
        case ROLE_ADMINISTRATEUR: ui_afficherMenuAdministrateur(); break;
        case ROLE_TECHNICIEN_LABO: ui_afficherMenuTechnicienLabo(); break;
        case ROLE_PHARMACIEN: ui_afficherMenuPharmacien(); break;
        case ROLE_SUPER_ADMIN: ui_afficherMenuSuperAdmin(); break;
        default: ui_afficherMenuPatient();
    }

    printf("\n");
    ui_setColor(UI_COLOR_TITLE);
    printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                              Votre choix :                                     ║\n");
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
    printf("\n    ==> ");
}
