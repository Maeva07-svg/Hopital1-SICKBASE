#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include "ui.h"
#include "comptes.h"

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
    ui_resetColor();
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
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║ UTILISATEUR : %-44s   ║\n", utilisateur_actuel->login);
    printf("║ PROFIL      : "); ui_setColor(profilCouleur); printf("%-44s", profilNom); ui_resetColor(); printf("   ║\n");
    printf("║ ID          : %-44d   ║\n", utilisateur_actuel->id_compte);
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
}

void ui_afficherMenuAuthentification() {
    system("cls");
    ui_setColor(UI_COLOR_TITLE);
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                         SICKBASE                             ║\n");
    printf("║              SYSTEME DE GESTION HOSPITALIERE                 ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();

    ui_setColor(UI_COLOR_MENU_OPTION);
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                      AUTHENTIFICATION                        ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║                                                              ║\n");
    printf("║    [1] Se connecter                                          ║\n");
    printf("║    [2] Creer un compte                                       ║\n");
    printf("║    [3] Mot de passe oublie                                   ║\n");
    printf("║    [4] Quitter                                               ║\n");
    printf("║                                                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
    printf("\n    Votre choix: ");
}

void ui_afficherMenuPatient() {
    ui_setColor(UI_COLOR_PATIENT);
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                      MENU PATIENT                            ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║                                                              ║\n");
    printf("║    [1] Voir mon dossier medical                              ║\n");
    printf("║    [2] Voir mes consultations                                ║\n");
    printf("║    [3] Voir mes ordonnances                                  ║\n");
    printf("║    [4] Voir mes analyses                                     ║\n");
    printf("║    [5] Prendre rendez-vous                                   ║\n");
    printf("║    [6] Voir mes rendez-vous                                  ║\n");
    printf("║    [7] Annuler un rendez-vous                                ║\n");
    printf("║    [8] Voir mes factures                                     ║\n");
    printf("║    [9] Payer une facture                                     ║\n");
    printf("║    [10] Messagerie                                           ║\n");
    printf("║    [11] Notifications                                        ║\n");
    printf("║    [12] Questions de securite                                ║\n");
    printf("║    [13] Modifier mon mot de passe                            ║\n");
    printf("║    [14] Deconnexion                                          ║\n");
    printf("║                                                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
}

void ui_afficherMenuMedecin() {
    ui_setColor(UI_COLOR_MEDECIN);
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                      MENU MEDECIN                            ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║                                                              ║\n");
    printf("║    [1] Gestion des patients                                  ║\n");
    printf("║    [2] Consultations                                         ║\n");
    printf("║    [3] Prescriptions                                         ║\n");
    printf("║    [4] Diagnostic medical                                    ║\n");
    printf("║    [5] Rendez-vous (Planning)                                ║\n");
    printf("║    [6] Mes disponibilites                                    ║\n");
    printf("║    [7] Mes rendez-vous                                       ║\n");
    printf("║    [8] Facturation                                           ║\n");
    printf("║    [9] Demandes d'analyses                                   ║\n");
    printf("║    [10] Messagerie                                           ║\n");
    printf("║    [11] Notifications                                        ║\n");
    printf("║    [12] Statistiques                                         ║\n");
    printf("║    [13] Questions de securite                                ║\n");
    printf("║    [14] Modifier mot de passe                                ║\n");
    printf("║    [15] Deconnexion                                          ║\n");
    printf("║                                                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
}

void ui_afficherMenuInfirmier() {
    ui_setColor(UI_COLOR_INFIRMIER);
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                     MENU INFIRMIER                           ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║                                                              ║\n");
    printf("║    [1] Liste des patients                                    ║\n");
    printf("║    [2] Soins a administrer                                   ║\n");
    printf("║    [3] Constantes vitales                                    ║\n");
    printf("║    [4] Planning des soins                                    ║\n");
    printf("║    [5] Messagerie                                            ║\n");
    printf("║    [6] Notifications                                         ║\n");
    printf("║    [7] Questions de securite                                 ║\n");
    printf("║    [8] Modifier mot de passe                                 ║\n");
    printf("║    [9] Deconnexion                                           ║\n");
    printf("║                                                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
}

void ui_afficherMenuAdministrateur() {
    ui_setColor(UI_COLOR_ADMIN);
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                   MENU ADMINISTRATEUR                        ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║                                                              ║\n");
    printf("║    [1] Gestion des patients                                  ║\n");
    printf("║    [2] Gestion du personnel                                  ║\n");
    printf("║    [3] Gestion des rendez-vous                               ║\n");
    printf("║    [4] Gestion des consultations                             ║\n");
    printf("║    [5] Gestion des comptes utilisateurs                      ║\n");
    printf("║    [6] Facturation                                           ║\n");
    printf("║    [7] Archives                                              ║\n");
    printf("║    [8] Statistiques                                          ║\n");
    printf("║    [9] Messagerie                                            ║\n");
    printf("║    [10] Notifications                                        ║\n");
    printf("║    [11] Questions de securite                                ║\n");
    printf("║    [12] Modifier mot de passe                                ║\n");
    printf("║    [13] Deconnexion                                          ║\n");
    printf("║                                                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
}

void ui_afficherMenuTechnicienLabo() {
    ui_setColor(UI_COLOR_LABO);
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║               MENU TECHNICIEN LABORATOIRE                    ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║                                                              ║\n");
    printf("║    [1] Liste des analyses a realiser                         ║\n");
    printf("║    [2] Saisir resultats d'analyses                           ║\n");
    printf("║    [3] Consulter historique analyses                         ║\n");
    printf("║    [4] Messagerie                                            ║\n");
    printf("║    [5] Notifications                                         ║\n");
    printf("║    [6] Questions de securite                                 ║\n");
    printf("║    [7] Modifier mot de passe                                 ║\n");
    printf("║    [8] Deconnexion                                           ║\n");
    printf("║                                                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
}

void ui_afficherMenuPharmacien() {
    ui_setColor(UI_COLOR_PHARMA);
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                     MENU PHARMACIEN                          ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║                                                              ║\n");
    printf("║    [1] Gestion des ordonnances                               ║\n");
    printf("║    [2] Gestion du stock                                      ║\n");
    printf("║    [3] Delivrance medicaments                                ║\n");
    printf("║    [4] Alertes stock faible                                  ║\n");
    printf("║    [5] Messagerie                                            ║\n");
    printf("║    [6] Notifications                                         ║\n");
    printf("║    [7] Questions de securite                                 ║\n");
    printf("║    [8] Modifier mot de passe                                 ║\n");
    printf("║    [9] Deconnexion                                           ║\n");
    printf("║                                                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
}

void ui_afficherMenuSuperAdmin() {
    ui_setColor(UI_COLOR_SUPER);
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                 MENU SUPER ADMINISTRATEUR                    ║\n");
    printf("╠══════════════════════════════════════════════════════════════╣\n");
    printf("║                                                              ║\n");
    printf("║    [1] Gestion complete patients                             ║\n");
    printf("║    [2] Gestion complete personnel                            ║\n");
    printf("║    [3] Gestion consultations                                 ║\n");
    printf("║    [4] Gestion ordonnances                                   ║\n");
    printf("║    [5] Gestion interventions                                 ║\n");
    printf("║    [6] Gestion comptes utilisateurs                          ║\n");
    printf("║    [7] Facturation                                           ║\n");
    printf("║    [8] Archives                                              ║\n");
    printf("║    [9] Gestion laboratoire                                   ║\n");
    printf("║    [10] Statistiques avancees                                ║\n");
    printf("║    [11] Configuration systeme                                ║\n");
    printf("║    [12] Gestion des rendez-vous                              ║\n");
    printf("║    [13] Messagerie                                           ║\n");
    printf("║    [14] Notifications                                        ║\n");
    printf("║    [15] Questions de securite                                ║\n");
    printf("║    [16] Modifier mot de passe                                ║\n");
    printf("║    [17] Deconnexion                                          ║\n");
    printf("║                                                              ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
}

void ui_afficherMenuPrincipal() {
    if (utilisateur_actuel == NULL) return;

    ui_afficherEntete();
    ui_afficherInfoUtilisateur();

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

    printf("\n    Votre choix: ");
}
