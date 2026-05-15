#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#include "utils.h"
#include <windows.h>
#include "patient.h"

#define MAX_MALADIES               10
#define MAX_SYMPTOMES_PAR_MALADIE  10
#define MAX_QUESTIONS              20

/* ================================================================
   STRUCTURES EXISTANTES
   ================================================================ */
typedef struct {
    POINT points[100];
    int nb_points;
} DessinCorps;

typedef struct {
    COLORREF couleur;
    int epaisseur;
} StyleDessin;

typedef struct
{
    char nom[100];
    char symptomes[MAX_SYMPTOMES_PAR_MALADIE][MAX_SYMPTOM_LENGTH];
    int  nb_symptomes;
    char description[300];
    char traitement[300];
    char precautions[200];
} Maladie;

typedef struct
{
    char question[200];
    char mot_cle[50];
    int  id_maladie_associee;
} QuestionDiagnostic;

typedef struct
{
    int  id_diagnostic;
    int  id_patient;
    int  id_medecin;
    char date_diagnostic[20];
    char maladie[100];
    char description[500];
    char traitement_prescrit[500];
    char recommandations[500];
    char medicaments[300];
    char examens_complementaires[300];
    char prochain_rdv[20];
    char notes[300];
} DiagnosticMedical;

/* ================================================================
   NOUVELLES STRUCTURES — DIAGNOSTIC CORPOREL INTERACTIF (WIN32)
   ================================================================ */

#define NB_ZONES_CORPORELLES   16
#define MAX_SYMPTOMES_ZONE      3
#define MAX_DIAG_ZONE           3

/*
 * ZoneCorporelle : represente une zone anatomique cliquable.
 * Le polygone est defini dans le repere de la fenetre Win32
 * (coordonnees en pixels, origine en haut-gauche).
 */
typedef struct
{
    char   nom[60];
    POINT  polygone[20];          /* sommets du polygone de detection */
    int    nb_pts;
    char   symptomes[MAX_SYMPTOMES_ZONE][100];
    char   diagnostics_possibles[MAX_DIAG_ZONE][100];
    char   conseil[150];
    COLORREF couleur_zone;        /* couleur de surbrillance propre a la zone */
} ZoneCorporelle;

/*
 * EtatFenetreCorps : etat interne de la fenetre Win32 du diagnostic
 * corporel. Passe comme pointeur dans GWLP_USERDATA.
 */
typedef struct
{
    int zone_selectionnee;        /* -1 = aucune */
    int zone_survol;              /* -1 = aucune */
    int id_patient;               /* patient courant, -1 si mode libre */
    int id_medecin;               /* medecin connecte */
} EtatFenetreCorps;

/* ================================================================
   VARIABLES GLOBALES
   ================================================================ */

extern Maladie          maladies[MAX_MALADIES];
extern int              nombreMaladies;
extern DiagnosticMedical diagnostics[MAX_PATIENTS * 10];
extern int              nombreDiagnostics;
extern int              dernierIDDiagnostic;

/* Zones corporelles — definies dans diagnostic.c */
extern ZoneCorporelle   zonesCorps[NB_ZONES_CORPORELLES];

/* ================================================================
   PROTOTYPES — FONCTIONS EXISTANTES
   ================================================================ */

void initialiserMaladies(void);
void initialiserQuestionsDiagnostic(void);
void diagnosticGuideParQuestions(void);
void diagnosticDirectParSymptomes(void);
void diagnosticPatientExistant(int index_patient);
void afficherToutesMaladies(void);
void menuDiagnosticMedical(void);

void creerDiagnosticPourPatient(int id_patient, int id_medecin);
void afficherDiagnosticsPatient(int id_patient);
void afficherDetailDiagnostic(int id_diagnostic);
void modifierDiagnostic(int id_diagnostic);
void prescrireTraitement(int id_diagnostic);
void menuDiagnosticMedicalAvance(void);

void sauvegarderDiagnostics(void);
void chargerDiagnostics(void);

/* ================================================================
   PROTOTYPES — DIAGNOSTIC CORPOREL INTERACTIF (WIN32)
   ================================================================ */

/*
 * Point d'entree principal : ouvre la fenetre Win32 du corps humain.
 * id_patient = -1  => mode "diagnostic libre" (sans patient lie)
 * id_patient >= 0  => mode "diagnostic patient" (bouton Enregistrer actif)
 */
void lancerDiagnosticCorporel(int id_patient, int id_medecin);

/* Initialise le tableau zonesCorps[] avec les 16 zones anatomiques */
void initialiserZonesCorporelles(void);

/* Algorithme ray-casting : renvoie l'indice de la zone sous (mx,my), -1 sinon */
int  trouverZoneSousCurseur(int mx, int my);

/* Dessine le corps humain complet en GDI (silhouette + membres) */
void dessinerCorpsHumain(HDC hdc);

/* Surligne la zone idx avec la couleur donnee */
void dessinerSurbrillanceZone(HDC hdc, int idx, COLORREF couleur);

/* Dessine les petites etiquettes de zone sur le corps */
void dessinerEtiquettesZones(HDC hdc);

/* Dessine le panel lateral (symptomes / diagnostics / conseil) */
void dessinerPanelLateral(HDC hdc, int zone_selectionnee, int id_patient);

/* Procedure de fenetre Win32 du module */
LRESULT CALLBACK WndProcDiagCorps(HWND hwnd, UINT msg,
                                   WPARAM wp, LPARAM lp);

#endif /* DIAGNOSTIC_H */
