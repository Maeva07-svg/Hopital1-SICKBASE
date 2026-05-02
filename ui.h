#ifndef UI_H
#define UI_H

#include "comptes.h"

// Couleurs Windows
#define UI_COLOR_DEFAULT 7
#define UI_COLOR_TITLE 11
#define UI_COLOR_MENU_OPTION 10
#define UI_COLOR_PATIENT 9
#define UI_COLOR_MEDECIN 11
#define UI_COLOR_INFIRMIER 13
#define UI_COLOR_ADMIN 14
#define UI_COLOR_LABO 12
#define UI_COLOR_PHARMA 10
#define UI_COLOR_SUPER 5
#define UI_COLOR_SUCCESS 10
#define UI_COLOR_ERROR 12
#define UI_COLOR_WARNING 14
#define UI_COLOR_INFO 11

// Fonctions UI
void ui_setColor(int couleur);
void ui_resetColor();
void ui_afficherMessageSucces(char *message);
void ui_afficherMessageErreur(char *message);
void ui_afficherMessageInfo(char *message);
void ui_afficherEntete();  // CHANGÉ : plus de paramètres
void ui_afficherInfoUtilisateur();
void ui_afficherMenuAuthentification();
void ui_afficherMenuPatient();
void ui_afficherMenuMedecin();
void ui_afficherMenuInfirmier();
void ui_afficherMenuAdministrateur();
void ui_afficherMenuTechnicienLabo();
void ui_afficherMenuPharmacien();
void ui_afficherMenuSuperAdmin();
void ui_afficherMenuPrincipal();

#endif
