/*
 * diagnostic.c  —  SICKBASE
 *
 * Contient :
 *   - Toutes les fonctions console existantes (inchangees)
 *   - Module diagnostic corporel interactif Win32 (NOUVEAU)
 *
 * Compilation (avec les autres .c du projet) :
 *   gcc *.c -o SICKBASE.exe -lgdi32 -luser32 -mwindows
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#include "diagnostic.h"
#include "patient.h"
#include "fichiers.h"
#include "utils.h"
#include "comptes.h"

/* ================================================================
   VARIABLES GLOBALES — EXISTANTES
   ================================================================ */

Maladie           maladies[MAX_MALADIES];
int               nombreMaladies    = 8;
DiagnosticMedical diagnostics[MAX_PATIENTS * 10];
int               nombreDiagnostics = 0;
int               dernierIDDiagnostic = 0;

QuestionDiagnostic questions[MAX_QUESTIONS];
int                nombreQuestions  = 0;

/* ================================================================
   VARIABLE GLOBALE — ZONES CORPORELLES (NOUVEAU)
   ================================================================ */

ZoneCorporelle zonesCorps[NB_ZONES_CORPORELLES];

/* ================================================================
   CONSTANTES INTERNES — MODULE WIN32
   ================================================================ */

#define CORPS_PANEL_X    460    /* debut du panel lateral (px) */
#define CORPS_PANEL_W    340    /* largeur du panel (px) */
#define CORPS_FENETRE_W  820    /* largeur totale fenetre */
#define CORPS_FENETRE_H  680    /* hauteur totale fenetre */

/* Palette de couleurs */
#define CLR_FOND_FENETRE  RGB(235, 245, 252)
#define CLR_CORPS_PEAU    RGB(255, 218, 180)
#define CLR_CORPS_CONTOUR RGB( 90,  55,  30)
#define CLR_SURVOL        RGB(255,  90,  60)
#define CLR_SELECTION     RGB( 30, 170,  90)
#define CLR_PANEL_FOND    RGB( 22,  52,  80)
#define CLR_PANEL_TITRE   RGB(  0, 195, 135)
#define CLR_SEPARATEUR    RGB(  0, 140, 100)
#define CLR_SYMPTOME      RGB(255, 210,  80)
#define CLR_DIAGNOSTIC_C  RGB( 90, 210, 255)
#define CLR_CONSEIL       RGB(255, 155,  80)
#define CLR_BLANC         RGB(255, 255, 255)
#define CLR_GRIS_CLAIR    RGB(190, 215, 235)

/* ================================================================
   FONCTIONS EXISTANTES (inchangees)
   ================================================================ */

void initialiserMaladies(void)
{
    /* Maladie 0 : Paludisme */
    strcpy(maladies[0].nom, "Paludisme");
    strcpy(maladies[0].symptomes[0], "fievre elevee");
    strcpy(maladies[0].symptomes[1], "frissons");
    strcpy(maladies[0].symptomes[2], "maux de tete");
    strcpy(maladies[0].symptomes[3], "vomissements");
    strcpy(maladies[0].symptomes[4], "fatigue intense");
    maladies[0].nb_symptomes = 5;
    strcpy(maladies[0].description, "Maladie parasitaire transmise par les moustiques");
    strcpy(maladies[0].traitement, "Medicaments antipaludeens (chloroquine, artemisinine)");
    strcpy(maladies[0].precautions, "Moustiquaire, repellents, consultation immediate");

    /* Maladie 1 : Typhoide */
    strcpy(maladies[1].nom, "Typhoide");
    strcpy(maladies[1].symptomes[0], "fievre progressive");
    strcpy(maladies[1].symptomes[1], "douleurs abdominales");
    strcpy(maladies[1].symptomes[2], "diarrhee");
    strcpy(maladies[1].symptomes[3], "maux de tete");
    strcpy(maladies[1].symptomes[4], "perte d'appetit");
    maladies[1].nb_symptomes = 5;
    strcpy(maladies[1].description, "Infection bacterienne due a Salmonella typhi");
    strcpy(maladies[1].traitement, "Antibiotiques (ciprofloxacine, ceftriaxone)");
    strcpy(maladies[1].precautions, "Eau potable, hygiene alimentaire");

    /* Maladie 2 : Angine */
    strcpy(maladies[2].nom, "Angine");
    strcpy(maladies[2].symptomes[0], "maux de gorge");
    strcpy(maladies[2].symptomes[1], "difficulte a avaler");
    strcpy(maladies[2].symptomes[2], "fievre moderee");
    strcpy(maladies[2].symptomes[3], "ganglions enfles");
    maladies[2].nb_symptomes = 4;
    strcpy(maladies[2].description, "Inflammation des amygdales, virale ou bacterienne");
    strcpy(maladies[2].traitement, "Antibiotiques si bacterien, anti-inflammatoires");
    strcpy(maladies[2].precautions, "Repos, gargarismes, boissons chaudes");

    /* Maladie 3 : COVID-19 */
    strcpy(maladies[3].nom, "COVID-19");
    strcpy(maladies[3].symptomes[0], "fievre");
    strcpy(maladies[3].symptomes[1], "toux seche");
    strcpy(maladies[3].symptomes[2], "fatigue");
    strcpy(maladies[3].symptomes[3], "perte de gout ou odorat");
    strcpy(maladies[3].symptomes[4], "difficultes respiratoires");
    maladies[3].nb_symptomes = 5;
    strcpy(maladies[3].description, "Maladie respiratoire due au coronavirus SARS-CoV-2");
    strcpy(maladies[3].traitement, "Traitement symptomatique, antiviraux, oxygenotherapie");
    strcpy(maladies[3].precautions, "Vaccination, masque, distanciation, isolement");

    /* Maladie 4 : Migraine */
    strcpy(maladies[4].nom, "Migraine");
    strcpy(maladies[4].symptomes[0], "maux de tete severes");
    strcpy(maladies[4].symptomes[1], "nausees");
    strcpy(maladies[4].symptomes[2], "vomissements");
    strcpy(maladies[4].symptomes[3], "sensibilite a la lumiere");
    strcpy(maladies[4].symptomes[4], "sensibilite au bruit");
    maladies[4].nb_symptomes = 5;
    strcpy(maladies[4].description, "Cephalee primaire intense et recurrente");
    strcpy(maladies[4].traitement, "Anti-inflammatoires, triptans, antalgiques");
    strcpy(maladies[4].precautions, "Eviter les declencheurs, repos dans le calme");

    /* Maladie 5 : Infection urinaire */
    strcpy(maladies[5].nom, "Infection urinaire");
    strcpy(maladies[5].symptomes[0], "brulures urinaires");
    strcpy(maladies[5].symptomes[1], "envies frequentes d'uriner");
    strcpy(maladies[5].symptomes[2], "douleurs pelviennes");
    strcpy(maladies[5].symptomes[3], "urines troubles");
    maladies[5].nb_symptomes = 4;
    strcpy(maladies[5].description, "Infection bacterienne des voies urinaires");
    strcpy(maladies[5].traitement, "Antibiotiques, antalgiques urinaires");
    strcpy(maladies[5].precautions, "Boire beaucoup d'eau, hygiene intime");

    /* Maladie 6 : Grippe */
    strcpy(maladies[6].nom, "Grippe");
    strcpy(maladies[6].symptomes[0], "fievre soudaine");
    strcpy(maladies[6].symptomes[1], "toux");
    strcpy(maladies[6].symptomes[2], "douleurs musculaires");
    strcpy(maladies[6].symptomes[3], "fatigue");
    strcpy(maladies[6].symptomes[4], "maux de tete");
    maladies[6].nb_symptomes = 5;
    strcpy(maladies[6].description, "Infection virale respiratoire saisonniere");
    strcpy(maladies[6].traitement, "Repos, paracetamol, antiviraux si necessaire");
    strcpy(maladies[6].precautions, "Vaccination annuelle, hygiene des mains");

    /* Maladie 7 : Gastroenterite */
    strcpy(maladies[7].nom, "Gastroenterite");
    strcpy(maladies[7].symptomes[0], "nausees");
    strcpy(maladies[7].symptomes[1], "vomissements");
    strcpy(maladies[7].symptomes[2], "diarrhee");
    strcpy(maladies[7].symptomes[3], "crampes abdominales");
    strcpy(maladies[7].symptomes[4], "fievre legere");
    maladies[7].nb_symptomes = 5;
    strcpy(maladies[7].description, "Inflammation de l'estomac et des intestins");
    strcpy(maladies[7].traitement, "Rehydratation, alimentation legere, antispasmodiques");
    strcpy(maladies[7].precautions, "Hygiene alimentaire, lavage des mains frequent");
}

/* ---------------------------------------------------------------- */

void initialiserQuestionsDiagnostic(void)
{
    nombreQuestions = 0;

#define AQ(q, mk, idm) \
    strcpy(questions[nombreQuestions].question, q); \
    strcpy(questions[nombreQuestions].mot_cle, mk); \
    questions[nombreQuestions].id_maladie_associee = idm; \
    nombreQuestions++;

    AQ("Avez-vous de la fievre elevee (plus de 39 C) ?",    "fievre elevee",            0)
    AQ("Ressentez-vous des frissons ?",                      "frissons",                 0)
    AQ("Avez-vous des maux de tete intenses ?",              "maux de tete",             0)
    AQ("Avez-vous mal a la gorge ?",                         "maux de gorge",            2)
    AQ("Avez-vous du mal a avaler ?",                        "difficulte a avaler",      2)
    AQ("Avez-vous une toux seche ?",                         "toux seche",               3)
    AQ("Avez-vous perdu le gout ou l'odorat ?",              "perte de gout",            3)
    AQ("Avez-vous des maux de tete violents ?",              "maux de tete severes",     4)
    AQ("Etes-vous sensible a la lumiere ?",                  "sensibilite a la lumiere", 4)
    AQ("Ressentez-vous des brulures en urinant ?",           "brulures urinaires",       5)
    AQ("Avez-vous envie d'uriner tres frequemment ?",        "envies frequentes",        5)
    AQ("Avez-vous des douleurs musculaires ?",               "douleurs musculaires",     6)
    AQ("Avez-vous des nausees ou vomissements ?",            "nausees",                  7)
    AQ("Avez-vous la diarrhee ?",                            "diarrhee",                 7)
#undef AQ
}

/* ---------------------------------------------------------------- */

void diagnosticGuideParQuestions(void)
{
    system("cls");
    printf("\n=== DIAGNOSTIC GUIDE PAR QUESTIONS ===\n\n");
    printf("Repondez par O (Oui) ou N (Non) :\n\n");

    int scores[MAX_MALADIES] = {0};

    for (int i = 0; i < nombreQuestions; i++) {
        printf("%d. %s (O/N): ", i + 1, questions[i].question);
        char rep;
        scanf("%c", &rep);
        viderBuffer();
        if (toupper(rep) == 'O')
            scores[questions[i].id_maladie_associee]++;
    }

    printf("\n=== ANALYSE ===\n\n");
    int trouve = 0;
    for (int m = 0; m < nombreMaladies; m++) {
        float pct = (float)scores[m] / maladies[m].nb_symptomes * 100.0f;
        if (pct >= 30.0f) {
            trouve = 1;
            printf("-> %s (%.0f%%)\n", maladies[m].nom, pct);
            printf("   Symptomes: %d/%d\n", scores[m], maladies[m].nb_symptomes);
            printf("   Description: %s\n", maladies[m].description);
            printf("   Traitement: %s\n", maladies[m].traitement);
            printf("   Precautions: %s\n\n", maladies[m].precautions);
        }
    }
    if (!trouve)
        printf("Aucune maladie identifiee. Consultez un medecin.\n");

    pause();
}

/* ---------------------------------------------------------------- */

void diagnosticDirectParSymptomes(void)
{
    system("cls");
    printf("\n=== DIAGNOSTIC PAR SYMPTOMES ===\n\n");

    char sym[MAX_SYMPTOMS][MAX_SYMPTOM_LENGTH];
    int  nb = 0;

    printf("Entrez vos symptomes (un par ligne, 'fin' pour terminer) :\n\n");
    while (nb < MAX_SYMPTOMS) {
        printf("Symptome %d: ", nb + 1);
        fgets(sym[nb], MAX_SYMPTOM_LENGTH, stdin);
        sym[nb][strcspn(sym[nb], "\n")] = '\0';
        if (strcmp(sym[nb], "fin") == 0) break;
        if (strlen(sym[nb]) > 0) nb++;
    }

    if (nb == 0) { printf("Aucun symptome entre.\n"); pause(); return; }

    printf("\n=== RESULTATS ===\n\n");
    int trouve = 0;
    for (int m = 0; m < nombreMaladies; m++) {
        int corr = 0;
        for (int s1 = 0; s1 < nb; s1++)
            for (int s2 = 0; s2 < maladies[m].nb_symptomes; s2++)
                if (comparerSymptomes(sym[s1], maladies[m].symptomes[s2])) { corr++; break; }

        float score = (float)corr / maladies[m].nb_symptomes * 100.0f;
        if (score >= 40.0f) {
            trouve = 1;
            printf("-> %s (%.0f%%)\n", maladies[m].nom, score);
            printf("   Description: %s\n", maladies[m].description);
            printf("   Traitement: %s\n", maladies[m].traitement);
            printf("   Precautions: %s\n\n", maladies[m].precautions);
        }
    }
    if (!trouve)
        printf("Aucune maladie ne correspond. Consultez un medecin.\n");

    pause();
}

/* ---------------------------------------------------------------- */

void diagnosticPatientExistant(int index_patient)
{
    if (index_patient < 0 || index_patient >= nombrePatients) {
        printf("Erreur: Patient non trouve.\n"); return;
    }
    Patient *p = &patients[index_patient];
    if (p->nb_symptomes == 0) {
        printf("\nCe patient n'a aucun symptome enregistre.\n"); pause(); return;
    }

    system("cls");
    printf("\n=== DIAGNOSTIC POUR %s %s (ID: %d) ===\n\n",
           p->prenom, p->nom, p->id);
    printf("Symptomes (%d) :\n", p->nb_symptomes);
    for (int i = 0; i < p->nb_symptomes; i++)
        printf("  - %s\n", p->symptomes[i]);

    printf("\n=== ANALYSE ===\n\n");
    int   trouve = 0;
    float meilleur = 0.0f;
    int   idMeil  = -1;

    for (int m = 0; m < nombreMaladies; m++) {
        int corr = 0;
        for (int s1 = 0; s1 < p->nb_symptomes; s1++)
            for (int s2 = 0; s2 < maladies[m].nb_symptomes; s2++)
                if (comparerSymptomes(p->symptomes[s1], maladies[m].symptomes[s2])) { corr++; break; }

        float score = (float)corr / maladies[m].nb_symptomes * 100.0f;
        if (score >= 40.0f) {
            trouve = 1;
            printf("-> %s (%.0f%%) — corr: %d/%d\n",
                   maladies[m].nom, score, corr, maladies[m].nb_symptomes);
            if (score > meilleur) { meilleur = score; idMeil = m; }
        }
    }

    if (!trouve) {
        printf("\nAucune maladie ne correspond. Examens complementaires recommandes.\n");
    } else {
        printf("\n=== RECOMMANDATION ===\n");
        if (meilleur >= 70.0f) {
            printf("\nDiagnostic probable : %s (%.0f%%)\n",
                   maladies[idMeil].nom, meilleur);
            printf("Traitement : %s\n", maladies[idMeil].traitement);
            printf("\nEnregistrer ce diagnostic ? (O/N) : ");
            char rep; scanf("%c", &rep); viderBuffer();
            if (toupper(rep) == 'O') {
                strcpy(p->diagnostic, maladies[idMeil].nom);
                sauvegarderPatients();
                printf("Diagnostic enregistre.\n");
            }
        } else if (meilleur >= 50.0f) {
            printf("\nPlusieurs maladies possibles. Examens complementaires conseilles.\n");
        } else {
            printf("\nCorrespondance insuffisante. Examen medical approfondi recommande.\n");
        }
    }
    pause();
}

/* ---------------------------------------------------------------- */

void creerDiagnosticPourPatient(int id_patient, int id_medecin)
{
    system("cls");
    printf("\n=== CREATION D'UN DIAGNOSTIC MEDICAL ===\n\n");

    int idx = rechercherParID(id_patient);
    if (idx == -1) { printf("Patient non trouve.\n"); pause(); return; }

    printf("Patient : %s %s (ID: %d)\n\n",
           patients[idx].prenom, patients[idx].nom, id_patient);

    DiagnosticMedical diag;
    memset(&diag, 0, sizeof(DiagnosticMedical));

    diag.id_diagnostic = ++dernierIDDiagnostic;
    diag.id_patient    = id_patient;
    diag.id_medecin    = id_medecin;

    time_t t = time(NULL);
    struct tm tm_info = *localtime(&t);
    sprintf(diag.date_diagnostic, "%02d/%02d/%04d",
            tm_info.tm_mday, tm_info.tm_mon + 1, tm_info.tm_year + 1900);

#define LIG(prompt, champ, taille) \
    printf(prompt); fgets(diag.champ, taille, stdin); \
    diag.champ[strcspn(diag.champ, "\n")] = '\0';

    LIG("1. Diagnostic (maladie) :\n   ",              maladie,                 100)
    LIG("\n2. Description detaillee :\n   ",            description,             500)
    LIG("\n3. Traitement prescrit :\n   ",              traitement_prescrit,     500)
    LIG("\n4. Medicaments :\n   ",                      medicaments,             300)
    LIG("\n5. Examens complementaires :\n   ",          examens_complementaires, 300)
    LIG("\n6. Recommandations :\n   ",                  recommandations,         500)
    LIG("\n7. Prochain RDV (JJ/MM/AAAA ou 'Aucun') :\n   ", prochain_rdv,       20)
    LIG("\n8. Notes :\n   ",                            notes,                   300)
#undef LIG

    diagnostics[nombreDiagnostics++] = diag;
    strcpy(patients[idx].diagnostic, diag.maladie);
    sauvegarderPatients();
    sauvegarderDiagnostics();

    printf("\n=== DIAGNOSTIC #%d ENREGISTRE ===\n", diag.id_diagnostic);
    pause();
}

/* ---------------------------------------------------------------- */

void afficherDiagnosticsPatient(int id_patient)
{
    system("cls");
    printf("\n=== HISTORIQUE DIAGNOSTICS — Patient ID: %d ===\n\n", id_patient);

    int cpt = 0;
    for (int i = 0; i < nombreDiagnostics; i++) {
        if (diagnostics[i].id_patient == id_patient) {
            cpt++;
            printf("[%d] Diag #%d — %s — %s\n", cpt,
                   diagnostics[i].id_diagnostic,
                   diagnostics[i].date_diagnostic,
                   diagnostics[i].maladie);
            printf("    Traitement : %.60s...\n", diagnostics[i].traitement_prescrit);
        }
    }
    if (cpt == 0) { printf("Aucun diagnostic enregistre.\n"); pause(); return; }

    printf("\nID diagnostic a consulter (0 = quitter) : ");
    int id; scanf("%d", &id); viderBuffer();
    if (id > 0) afficherDetailDiagnostic(id);
    pause();
}

/* ---------------------------------------------------------------- */

void afficherDetailDiagnostic(int id_diagnostic)
{
    for (int i = 0; i < nombreDiagnostics; i++) {
        if (diagnostics[i].id_diagnostic == id_diagnostic) {
            printf("\n========================================\n");
            printf("DIAGNOSTIC MEDICAL #%d\n", diagnostics[i].id_diagnostic);
            printf("========================================\n");
            printf("Date           : %s\n", diagnostics[i].date_diagnostic);
            printf("Patient ID     : %d\n", diagnostics[i].id_patient);
            printf("Medecin ID     : %d\n", diagnostics[i].id_medecin);
            printf("\n--- DIAGNOSTIC ---\n%s\n", diagnostics[i].maladie);
            printf("\n--- DESCRIPTION ---\n%s\n", diagnostics[i].description);
            printf("\n--- TRAITEMENT ---\n%s\n", diagnostics[i].traitement_prescrit);
            printf("\n--- MEDICAMENTS ---\n%s\n", diagnostics[i].medicaments);
            printf("\n--- EXAMENS ---\n%s\n", diagnostics[i].examens_complementaires);
            printf("\n--- RECOMMANDATIONS ---\n%s\n", diagnostics[i].recommandations);
            if (strlen(diagnostics[i].prochain_rdv) > 0 &&
                strcmp(diagnostics[i].prochain_rdv, "Aucun") != 0)
                printf("\n--- PROCHAIN RDV ---\n%s\n", diagnostics[i].prochain_rdv);
            if (strlen(diagnostics[i].notes) > 0)
                printf("\n--- NOTES ---\n%s\n", diagnostics[i].notes);
            printf("========================================\n");
            return;
        }
    }
    printf("Diagnostic non trouve.\n");
}

/* ---------------------------------------------------------------- */

void modifierDiagnostic(int id_diagnostic)
{
    for (int i = 0; i < nombreDiagnostics; i++) {
        if (diagnostics[i].id_diagnostic == id_diagnostic) {
            printf("\n=== MODIFICATION DIAGNOSTIC #%d ===\n", id_diagnostic);
            printf("Nouveau diagnostic (actuel : %s) :\n   ", diagnostics[i].maladie);
            fgets(diagnostics[i].maladie, 100, stdin);
            diagnostics[i].maladie[strcspn(diagnostics[i].maladie, "\n")] = '\0';

            char tmp[500];
            printf("Nouveau traitement (actuel : %.50s...) :\n   ",
                   diagnostics[i].traitement_prescrit);
            fgets(tmp, 500, stdin);
            tmp[strcspn(tmp, "\n")] = '\0';
            if (strlen(tmp) > 0) strcpy(diagnostics[i].traitement_prescrit, tmp);

            sauvegarderDiagnostics();
            printf("Modification enregistree.\n");
            pause(); return;
        }
    }
    printf("Diagnostic non trouve.\n"); pause();
}

/* ---------------------------------------------------------------- */

void sauvegarderDiagnostics(void)
{
    FILE *f = fopen("diagnostics.dat", "wb");
    if (!f) return;
    fwrite(&nombreDiagnostics, sizeof(int), 1, f);
    fwrite(diagnostics, sizeof(DiagnosticMedical), nombreDiagnostics, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_diagnostic.dat", "wb");
    if (fid) { fwrite(&dernierIDDiagnostic, sizeof(int), 1, fid); fclose(fid); }
}

void chargerDiagnostics(void)
{
    FILE *f = fopen("diagnostics.dat", "rb");
    if (!f) return;
    fread(&nombreDiagnostics, sizeof(int), 1, f);
    fread(diagnostics, sizeof(DiagnosticMedical), nombreDiagnostics, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_diagnostic.dat", "rb");
    if (fid) { fread(&dernierIDDiagnostic, sizeof(int), 1, fid); fclose(fid); }
}

/* ---------------------------------------------------------------- */

void afficherToutesMaladies(void)
{
    system("cls");
    printf("\n=== BASE DE DONNEES DES MALADIES ===\n\n");
    for (int i = 0; i < nombreMaladies; i++) {
        printf("%d. %s\n   Symptomes : ", i + 1, maladies[i].nom);
        for (int j = 0; j < maladies[i].nb_symptomes; j++) {
            printf("%s", maladies[i].symptomes[j]);
            if (j < maladies[i].nb_symptomes - 1) printf(", ");
        }
        printf("\n   Description : %s\n", maladies[i].description);
        printf("   Traitement  : %s\n", maladies[i].traitement);
        printf("   Precautions : %s\n\n", maladies[i].precautions);
    }
}

/* ---------------------------------------------------------------- */

void menuDiagnosticMedical(void)
{
    int choix;
    do {
        system("cls");
        color(11, 0);
        printf("\n=== SYSTEME DE DIAGNOSTIC MEDICAL ===\n\n");
        color(7, 0);
        printf("1. Diagnostic guide par questions\n");
        printf("2. Diagnostic par saisie de symptomes\n");
        printf("3. Base de donnees des maladies\n");
        printf("4. Diagnostic corporel interactif (NOUVEAU)\n");
        printf("5. Retour\n");
        printf("\nVotre choix : ");
        scanf("%d", &choix); viderBuffer();

        switch (choix) {
            case 1: diagnosticGuideParQuestions();   break;
            case 2: diagnosticDirectParSymptomes();  break;
            case 3: afficherToutesMaladies(); pause(); break;
            case 4: lancerDiagnosticCorporel(-1, 0); break;
            case 5: return;
            default: printf("Choix invalide.\n"); pause();
        }
    } while (choix != 5);
}

/* ---------------------------------------------------------------- */

void menuDiagnosticMedicalAvance(void)
{
    int choix;
    do {
        system("cls");
        color(11, 0);
        printf("\n=== DIAGNOSTIC MEDICAL AVANCE ===\n\n");
        color(7, 0);
        printf("1. Creer un diagnostic pour un patient\n");
        printf("2. Historique diagnostics d'un patient\n");
        printf("3. Modifier un diagnostic\n");
        printf("4. Diagnostic guide par questions\n");
        printf("5. Diagnostic par symptomes\n");
        printf("6. Base de donnees des maladies\n");
        printf("7. Diagnostic corporel interactif (NOUVEAU)\n");
        printf("8. Retour\n");
        printf("\nVotre choix : ");
        scanf("%d", &choix); viderBuffer();

        switch (choix) {
            case 1: {
                if (nombrePatients == 0) { printf("Aucun patient.\n"); pause(); break; }
                afficherListePatientsSimple();
                printf("\nID du patient : "); int id; scanf("%d", &id); viderBuffer();
                if (rechercherParID(id) != -1)
                    creerDiagnosticPourPatient(id,
                        utilisateur_actuel ? utilisateur_actuel->id_associe : 1);
                else { printf("Patient non trouve.\n"); pause(); }
                break;
            }
            case 2: {
                if (nombrePatients == 0) { printf("Aucun patient.\n"); pause(); break; }
                afficherListePatientsSimple();
                printf("\nID du patient : "); int id; scanf("%d", &id); viderBuffer();
                afficherDiagnosticsPatient(id);
                break;
            }
            case 3: {
                printf("ID du diagnostic a modifier : "); int id; scanf("%d", &id); viderBuffer();
                modifierDiagnostic(id); break;
            }
            case 4: diagnosticGuideParQuestions();  break;
            case 5: diagnosticDirectParSymptomes(); break;
            case 6: afficherToutesMaladies(); pause(); break;
            case 7: {
                /* Propose de choisir un patient ou mode libre */
                printf("\nLier a un patient ? (O/N) : "); char r; scanf("%c",&r); viderBuffer();
                if (toupper(r) == 'O') {
                    afficherListePatientsSimple();
                    printf("ID patient : "); int id; scanf("%d",&id); viderBuffer();
                    lancerDiagnosticCorporel(id,
                        utilisateur_actuel ? utilisateur_actuel->id_associe : 1);
                } else {
                    lancerDiagnosticCorporel(-1, 0);
                }
                break;
            }
            case 8: return;
            default: printf("Choix invalide.\n"); pause();
        }
    } while (choix != 8);
}

/* ================================================================
   MODULE DIAGNOSTIC CORPOREL INTERACTIF — WIN32  (NOUVEAU)
   ================================================================ */

/* ── Geometrie utilitaire ─────────────────────────────────────── */

/*
 * Ray-casting : renvoie 1 si le point (px,py) est
 * a l'interieur du polygone defini par poly[0..nb-1].
 */
static BOOL ptDansPolygone(const POINT *poly, int nb, int px, int py)
{
    BOOL dedans = FALSE;
    for (int i = 0, j = nb - 1; i < nb; j = i++) {
        if (((poly[i].y > py) != (poly[j].y > py)) &&
            (px < (poly[j].x - poly[i].x) * (py - poly[i].y)
                  / (poly[j].y - poly[i].y) + poly[i].x))
            dedans = !dedans;
    }
    return dedans;
}

/* ── Initialisation des zones ─────────────────────────────────── */

void initialiserZonesCorporelles(void)
{
    /*
     * Coordonnees des polygones en pixels dans la fenetre
     * (corps centre sur X≈200, vertical de Y=35 a Y=645).
     */

    int i = 0;

    /* 0 — Tete */
    strcpy(zonesCorps[i].nom, "Tete");
    POINT ptTete[] = {{160,35},{240,35},{248,60},{248,90},{240,110},{210,120},{190,120},{160,110},{152,90},{152,60}};
    memcpy(zonesCorps[i].polygone, ptTete, sizeof(ptTete));
    zonesCorps[i].nb_pts = 10;
    strcpy(zonesCorps[i].symptomes[0], "Maux de tete, migraines");
    strcpy(zonesCorps[i].symptomes[1], "Vertiges, troubles visuels");
    strcpy(zonesCorps[i].symptomes[2], "Fievre, confusion mentale");
    strcpy(zonesCorps[i].diagnostics_possibles[0], "Migraine, hypertension arterielle");
    strcpy(zonesCorps[i].diagnostics_possibles[1], "AVC, meningite");
    strcpy(zonesCorps[i].diagnostics_possibles[2], "Traumatisme cranien");
    strcpy(zonesCorps[i].conseil, "Consulter en urgence si douleur soudaine et violente.");
    zonesCorps[i].couleur_zone = RGB(255, 160, 130);
    i++;

    /* 1 — Cou */
    strcpy(zonesCorps[i].nom, "Cou");
    POINT ptCou[] = {{183,118},{217,118},{222,143},{178,143}};
    memcpy(zonesCorps[i].polygone, ptCou, sizeof(ptCou));
    zonesCorps[i].nb_pts = 4;
    strcpy(zonesCorps[i].symptomes[0], "Douleur cervicale, raideur");
    strcpy(zonesCorps[i].symptomes[1], "Difficultes a avaler");
    strcpy(zonesCorps[i].symptomes[2], "Ganglions enfles");
    strcpy(zonesCorps[i].diagnostics_possibles[0], "Torticolis, cervicalgie");
    strcpy(zonesCorps[i].diagnostics_possibles[1], "Angine, pharyngite");
    strcpy(zonesCorps[i].diagnostics_possibles[2], "Adenopathie, thyroidite");
    strcpy(zonesCorps[i].conseil, "Eviter les mouvements brusques. Collier cervical si besoin.");
    zonesCorps[i].couleur_zone = RGB(255, 140, 110);
    i++;

    /* 2 — Epaule gauche */
    strcpy(zonesCorps[i].nom, "Epaule gauche");
    POINT ptEpG[] = {{118,143},{178,143},{178,198},{118,188}};
    memcpy(zonesCorps[i].polygone, ptEpG, sizeof(ptEpG));
    zonesCorps[i].nb_pts = 4;
    strcpy(zonesCorps[i].symptomes[0], "Douleur a la mobilisation");
    strcpy(zonesCorps[i].symptomes[1], "Craquements, instabilite");
    strcpy(zonesCorps[i].symptomes[2], "Limitation des mouvements");
    strcpy(zonesCorps[i].diagnostics_possibles[0], "Tendinite, bursite");
    strcpy(zonesCorps[i].diagnostics_possibles[1], "Luxation, rupture coiffe");
    strcpy(zonesCorps[i].diagnostics_possibles[2], "Arthrose, periarthrite");
    strcpy(zonesCorps[i].conseil, "Repos 48h, glace 15 min. Kinesitherapie recommandee.");
    zonesCorps[i].couleur_zone = RGB(180, 210, 255);
    i++;

    /* 3 — Epaule droite */
    strcpy(zonesCorps[i].nom, "Epaule droite");
    POINT ptEpD[] = {{222,143},{282,143},{282,188},{222,198}};
    memcpy(zonesCorps[i].polygone, ptEpD, sizeof(ptEpD));
    zonesCorps[i].nb_pts = 4;
    strcpy(zonesCorps[i].symptomes[0], "Douleur a la mobilisation");
    strcpy(zonesCorps[i].symptomes[1], "Craquements, instabilite");
    strcpy(zonesCorps[i].symptomes[2], "Limitation des mouvements");
    strcpy(zonesCorps[i].diagnostics_possibles[0], "Tendinite, bursite");
    strcpy(zonesCorps[i].diagnostics_possibles[1], "Luxation, rupture coiffe");
    strcpy(zonesCorps[i].diagnostics_possibles[2], "Arthrose, periarthrite");
    strcpy(zonesCorps[i].conseil, "Repos 48h, glace 15 min. Kinesitherapie recommandee.");
    zonesCorps[i].couleur_zone = RGB(180, 210, 255);
    i++;

    /* 4 — Thorax / Coeur */
    strcpy(zonesCorps[i].nom, "Thorax / Coeur");
    POINT ptTho[] = {{178,143},{222,143},{228,245},{172,245}};
    memcpy(zonesCorps[i].polygone, ptTho, sizeof(ptTho));
    zonesCorps[i].nb_pts = 4;
    strcpy(zonesCorps[i].symptomes[0], "Douleur thoracique, oppression");
    strcpy(zonesCorps[i].symptomes[1], "Palpitations, essoufflement");
    strcpy(zonesCorps[i].symptomes[2], "Douleur irradiant dans le bras gauche");
    strcpy(zonesCorps[i].diagnostics_possibles[0], "Angine de poitrine, infarctus");
    strcpy(zonesCorps[i].diagnostics_possibles[1], "Arythmie, pericardite");
    strcpy(zonesCorps[i].diagnostics_possibles[2], "Pneumonie, embolie pulmonaire");
    strcpy(zonesCorps[i].conseil, "URGENCE si douleur brutale. Appeler le 15 immediatement.");
    zonesCorps[i].couleur_zone = RGB(255, 70, 70);
    i++;

    /* 5 — Abdomen haut */
    strcpy(zonesCorps[i].nom, "Abdomen haut");
    POINT ptAbH[] = {{172,245},{228,245},{232,308},{168,308}};
    memcpy(zonesCorps[i].polygone, ptAbH, sizeof(ptAbH));
    zonesCorps[i].nb_pts = 4;
    strcpy(zonesCorps[i].symptomes[0], "Brulures epigastriques, reflux");
    strcpy(zonesCorps[i].symptomes[1], "Nausees, vomissements");
    strcpy(zonesCorps[i].symptomes[2], "Ballonnements apres repas");
    strcpy(zonesCorps[i].diagnostics_possibles[0], "Gastrite, ulcere gastrique");
    strcpy(zonesCorps[i].diagnostics_possibles[1], "RGO, hernie hiatale");
    strcpy(zonesCorps[i].diagnostics_possibles[2], "Pancreatite, cholecystite");
    strcpy(zonesCorps[i].conseil, "Eviter alcool, epices. Repas fractionnes conseilles.");
    zonesCorps[i].couleur_zone = RGB(255, 195, 70);
    i++;

    /* 6 — Abdomen bas */
    strcpy(zonesCorps[i].nom, "Abdomen bas");
    POINT ptAbB[] = {{168,308},{232,308},{228,375},{172,375}};
    memcpy(zonesCorps[i].polygone, ptAbB, sizeof(ptAbB));
    zonesCorps[i].nb_pts = 4;
    strcpy(zonesCorps[i].symptomes[0], "Douleurs abdominales basses");
    strcpy(zonesCorps[i].symptomes[1], "Troubles du transit, diarrhee");
    strcpy(zonesCorps[i].symptomes[2], "Crampes, ballonnements");
    strcpy(zonesCorps[i].diagnostics_possibles[0], "Appendicite (droite), colite");
    strcpy(zonesCorps[i].diagnostics_possibles[1], "Syndrome du colon irritable");
    strcpy(zonesCorps[i].diagnostics_possibles[2], "Infection urinaire, kyste ovarien");
    strcpy(zonesCorps[i].conseil, "Urgence si douleur localisee a droite et fievre.");
    zonesCorps[i].couleur_zone = RGB(255, 170, 55);
    i++;

    /* 7 — Bras gauche */
    strcpy(zonesCorps[i].nom, "Bras gauche");
    POINT ptBrG[] = {{88,188},{118,188},{125,328},{88,328}};
    memcpy(zonesCorps[i].polygone, ptBrG, sizeof(ptBrG));
    zonesCorps[i].nb_pts = 4;
    strcpy(zonesCorps[i].symptomes[0], "Douleur, engourdissement");
    strcpy(zonesCorps[i].symptomes[1], "Faiblesse musculaire");
    strcpy(zonesCorps[i].symptomes[2], "Fourmillements dans le bras");
    strcpy(zonesCorps[i].diagnostics_possibles[0], "Fracture, entorse");
    strcpy(zonesCorps[i].diagnostics_possibles[1], "Nevralgie, compression radiculaire");
    strcpy(zonesCorps[i].diagnostics_possibles[2], "Tendinite, epicondylite");
    strcpy(zonesCorps[i].conseil, "Immobiliser en cas de traumatisme. Radiographie conseilee.");
    zonesCorps[i].couleur_zone = RGB(160, 220, 195);
    i++;

    /* 8 — Bras droit */
    strcpy(zonesCorps[i].nom, "Bras droit");
    POINT ptBrD[] = {{282,188},{312,188},{312,328},{275,328}};
    memcpy(zonesCorps[i].polygone, ptBrD, sizeof(ptBrD));
    zonesCorps[i].nb_pts = 4;
    strcpy(zonesCorps[i].symptomes[0], "Douleur, engourdissement");
    strcpy(zonesCorps[i].symptomes[1], "Faiblesse musculaire");
    strcpy(zonesCorps[i].symptomes[2], "Fourmillements dans le bras");
    strcpy(zonesCorps[i].diagnostics_possibles[0], "Fracture, entorse");
    strcpy(zonesCorps[i].diagnostics_possibles[1], "Nevralgie, compression radiculaire");
    strcpy(zonesCorps[i].diagnostics_possibles[2], "Tendinite, epicondylite");
    strcpy(zonesCorps[i].conseil, "Immobiliser en cas de traumatisme. Radiographie conseillee.");
    zonesCorps[i].couleur_zone = RGB(160, 220, 195);
    i++;

    /* 9 — Main gauche */
    strcpy(zonesCorps[i].nom, "Main gauche");
    POINT ptMnG[] = {{80,326},{127,326},{130,388},{77,388}};
    memcpy(zonesCorps[i].polygone, ptMnG, sizeof(ptMnG));
    zonesCorps[i].nb_pts = 4;
    strcpy(zonesCorps[i].symptomes[0], "Douleur articulaire des doigts");
    strcpy(zonesCorps[i].symptomes[1], "Engourdissement, picotements");
    strcpy(zonesCorps[i].symptomes[2], "Gonflement des articulations");
    strcpy(zonesCorps[i].diagnostics_possibles[0], "Canal carpien, arthrite");
    strcpy(zonesCorps[i].diagnostics_possibles[1], "Maladie de Raynaud");
    strcpy(zonesCorps[i].diagnostics_possibles[2], "Polyarthrite rhumatoide");
    strcpy(zonesCorps[i].conseil, "Attelle de repos la nuit pour le canal carpien.");
    zonesCorps[i].couleur_zone = RGB(140, 200, 175);
    i++;

    /* 10 — Main droite */
    strcpy(zonesCorps[i].nom, "Main droite");
    POINT ptMnD[] = {{273,326},{320,326},{323,388},{270,388}};
    memcpy(zonesCorps[i].polygone, ptMnD, sizeof(ptMnD));
    zonesCorps[i].nb_pts = 4;
    strcpy(zonesCorps[i].symptomes[0], "Douleur articulaire des doigts");
    strcpy(zonesCorps[i].symptomes[1], "Engourdissement, picotements");
    strcpy(zonesCorps[i].symptomes[2], "Gonflement des articulations");
    strcpy(zonesCorps[i].diagnostics_possibles[0], "Canal carpien, arthrite");
    strcpy(zonesCorps[i].diagnostics_possibles[1], "Maladie de Raynaud");
    strcpy(zonesCorps[i].diagnostics_possibles[2], "Polyarthrite rhumatoide");
    strcpy(zonesCorps[i].conseil, "Attelle de repos la nuit pour le canal carpien.");
    zonesCorps[i].couleur_zone = RGB(140, 200, 175);
    i++;

    /* 11 — Bassin / Hanches */
    strcpy(zonesCorps[i].nom, "Bassin / Hanches");
    POINT ptBas[] = {{158,373},{242,373},{250,435},{150,435}};
    memcpy(zonesCorps[i].polygone, ptBas, sizeof(ptBas));
    zonesCorps[i].nb_pts = 4;
    strcpy(zonesCorps[i].symptomes[0], "Douleur au niveau du bassin");
    strcpy(zonesCorps[i].symptomes[1], "Douleur irradiant dans la jambe");
    strcpy(zonesCorps[i].symptomes[2], "Difficulte a marcher");
    strcpy(zonesCorps[i].diagnostics_possibles[0], "Coxarthrose, bursite");
    strcpy(zonesCorps[i].diagnostics_possibles[1], "Sciatique, hernie discale");
    strcpy(zonesCorps[i].diagnostics_possibles[2], "Fracture du col du femur");
    strcpy(zonesCorps[i].conseil, "Consultation orthopedique recommandee. Radio du bassin.");
    zonesCorps[i].couleur_zone = RGB(210, 170, 255);
    i++;

    /* 12 — Cuisse gauche */
    strcpy(zonesCorps[i].nom, "Cuisse gauche");
    POINT ptCuG[] = {{148,433},{193,433},{190,547},{142,547}};
    memcpy(zonesCorps[i].polygone, ptCuG, sizeof(ptCuG));
    zonesCorps[i].nb_pts = 4;
    strcpy(zonesCorps[i].symptomes[0], "Douleur musculaire profonde");
    strcpy(zonesCorps[i].symptomes[1], "Crampes, contractures");
    strcpy(zonesCorps[i].symptomes[2], "Gonflement, ecchymose");
    strcpy(zonesCorps[i].diagnostics_possibles[0], "Claquage, elongation");
    strcpy(zonesCorps[i].diagnostics_possibles[1], "Thrombose veineuse (phlébite)");
    strcpy(zonesCorps[i].diagnostics_possibles[2], "Fracture du femur");
    strcpy(zonesCorps[i].conseil, "URGENCE si jambe rouge et gonflee : risque phlebite.");
    zonesCorps[i].couleur_zone = RGB(175, 200, 240);
    i++;

    /* 13 — Cuisse droite */
    strcpy(zonesCorps[i].nom, "Cuisse droite");
    POINT ptCuD[] = {{207,433},{252,433},{258,547},{210,547}};
    memcpy(zonesCorps[i].polygone, ptCuD, sizeof(ptCuD));
    zonesCorps[i].nb_pts = 4;
    strcpy(zonesCorps[i].symptomes[0], "Douleur musculaire profonde");
    strcpy(zonesCorps[i].symptomes[1], "Crampes, contractures");
    strcpy(zonesCorps[i].symptomes[2], "Gonflement, ecchymose");
    strcpy(zonesCorps[i].diagnostics_possibles[0], "Claquage, elongation");
    strcpy(zonesCorps[i].diagnostics_possibles[1], "Thrombose veineuse (phlébite)");
    strcpy(zonesCorps[i].diagnostics_possibles[2], "Fracture du femur");
    strcpy(zonesCorps[i].conseil, "URGENCE si jambe rouge et gonflee : risque phlebite.");
    zonesCorps[i].couleur_zone = RGB(175, 200, 240);
    i++;

    /* 14 — Genou & Jambe gauche */
    strcpy(zonesCorps[i].nom, "Genou & Jambe gauche");
    POINT ptJbG[] = {{138,545},{192,545},{188,648},{132,648}};
    memcpy(zonesCorps[i].polygone, ptJbG, sizeof(ptJbG));
    zonesCorps[i].nb_pts = 4;
    strcpy(zonesCorps[i].symptomes[0], "Douleur au genou a la flexion");
    strcpy(zonesCorps[i].symptomes[1], "Gonflement articulaire");
    strcpy(zonesCorps[i].symptomes[2], "Douleur du mollet");
    strcpy(zonesCorps[i].diagnostics_possibles[0], "Meniscopathie, ligament croise");
    strcpy(zonesCorps[i].diagnostics_possibles[1], "Arthrose, kyste de Baker");
    strcpy(zonesCorps[i].diagnostics_possibles[2], "Tendinite rotulienne");
    strcpy(zonesCorps[i].conseil, "Glace 15 min si gonflement aigu. Eviter les escaliers.");
    zonesCorps[i].couleur_zone = RGB(155, 185, 230);
    i++;

    /* 15 — Genou & Jambe droite */
    strcpy(zonesCorps[i].nom, "Genou & Jambe droite");
    POINT ptJbD[] = {{210,545},{262,545},{268,648},{216,648}};
    memcpy(zonesCorps[i].polygone, ptJbD, sizeof(ptJbD));
    zonesCorps[i].nb_pts = 4;
    strcpy(zonesCorps[i].symptomes[0], "Douleur au genou a la flexion");
    strcpy(zonesCorps[i].symptomes[1], "Gonflement articulaire");
    strcpy(zonesCorps[i].symptomes[2], "Douleur du mollet");
    strcpy(zonesCorps[i].diagnostics_possibles[0], "Meniscopathie, ligament croise");
    strcpy(zonesCorps[i].diagnostics_possibles[1], "Arthrose, kyste de Baker");
    strcpy(zonesCorps[i].diagnostics_possibles[2], "Tendinite rotulienne");
    strcpy(zonesCorps[i].conseil, "Glace 15 min si gonflement aigu. Eviter les escaliers.");
    zonesCorps[i].couleur_zone = RGB(155, 185, 230);
    /* i++ — derniere zone */
}

/* ── Trouver la zone sous le curseur ─────────────────────────── */

int trouverZoneSousCurseur(int mx, int my)
{
    if (mx >= CORPS_PANEL_X) return -1;  /* dans le panel */
    for (int i = 0; i < NB_ZONES_CORPORELLES; i++)
        if (ptDansPolygone(zonesCorps[i].polygone, zonesCorps[i].nb_pts, mx, my))
            return i;
    return -1;
}

/* ── Dessin du corps en GDI ───────────────────────────────────── */

void dessinerCorpsHumain(HDC hdc)
{
    HBRUSH brPeau   = CreateSolidBrush(CLR_CORPS_PEAU);
    HPEN   stylo    = CreatePen(PS_SOLID, 2, CLR_CORPS_CONTOUR);
    SelectObject(hdc, stylo);
    SelectObject(hdc, brPeau);

    /* Tete */
    Ellipse(hdc, 152, 35, 248, 122);

    /* Cou */
    POINT ptCou[] = {{183,119},{217,119},{220,145},{180,145}};
    Polygon(hdc, ptCou, 4);

    /* Torse */
    POINT ptTorse[] = {{118,143},{282,143},{265,378},{135,378}};
    Polygon(hdc, ptTorse, 4);

    /* Bras gauche */
    POINT ptBrG[] = {{86,186},{120,186},{128,330},{86,330}};
    Polygon(hdc, ptBrG, 4);

    /* Main gauche */
    POINT ptMnG[] = {{78,328},{130,328},{133,390},{75,390}};
    Polygon(hdc, ptMnG, 4);

    /* Bras droit */
    POINT ptBrD[] = {{280,186},{314,186},{314,330},{272,330}};
    Polygon(hdc, ptBrD, 4);

    /* Main droite */
    POINT ptMnD[] = {{270,328},{322,328},{325,390},{267,390}};
    Polygon(hdc, ptMnD, 4);

    /* Bassin */
    POINT ptBassin[] = {{132,375},{268,375},{258,437},{142,437}};
    Polygon(hdc, ptBassin, 4);

    /* Cuisse gauche */
    POINT ptCuG[] = {{140,435},{196,435},{193,550},{134,550}};
    Polygon(hdc, ptCuG, 4);

    /* Cuisse droite */
    POINT ptCuD[] = {{204,435},{260,435},{266,550},{207,550}};
    Polygon(hdc, ptCuD, 4);

    /* Jambe gauche */
    POINT ptJbG[] = {{130,548},{195,548},{191,650},{124,650}};
    Polygon(hdc, ptJbG, 4);

    /* Jambe droite */
    POINT ptJbD[] = {{205,548},{270,548},{276,650},{211,650}};
    Polygon(hdc, ptJbD, 4);

    DeleteObject(brPeau);
    DeleteObject(stylo);
}

/* ── Surbrillance d'une zone ─────────────────────────────────── */

void dessinerSurbrillanceZone(HDC hdc, int idx, COLORREF couleur)
{
    if (idx < 0 || idx >= NB_ZONES_CORPORELLES) return;
    ZoneCorporelle *z = &zonesCorps[idx];

    HBRUSH br    = CreateSolidBrush(couleur);
    HPEN   stylo = CreatePen(PS_SOLID, 3, couleur);

    HRGN rgn = CreatePolygonRgn(z->polygone, z->nb_pts, WINDING);
    FillRgn(hdc, rgn, br);
    DeleteObject(rgn);

    SelectObject(hdc, stylo);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Polygon(hdc, z->polygone, z->nb_pts);

    DeleteObject(br);
    DeleteObject(stylo);
}

/* ── Etiquettes sur le corps ─────────────────────────────────── */

void dessinerEtiquettesZones(HDC hdc)
{
    SetBkMode(hdc, TRANSPARENT);
    HFONT f = CreateFont(10, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, "Segoe UI");
    SelectObject(hdc, f);
    SetTextColor(hdc, RGB(40, 40, 80));

    struct { int x; int y; const char *lbl; } lbl[] = {
        {170, 68,  "Tete"},
        {180,128,  "Cou"},
        { 90,165,  "Ep.G"},
        {237,165,  "Ep.D"},
        {174,190,  "Thorax"},
        {173,272,  "Abd.haut"},
        {173,337,  "Abd.bas"},
        { 50,255,  "Bras G"},
        {302,255,  "Bras D"},
        { 50,353,  "Main G"},
        {300,353,  "Main D"},
        {169,400,  "Bassin"},
        {142,488,  "Cuisse G"},
        {213,488,  "Cuisse D"},
        {132,595,  "Jambe G"},
        {213,595,  "Jambe D"},
    };
    for (int i = 0; i < 16; i++)
        TextOut(hdc, lbl[i].x, lbl[i].y, lbl[i].lbl, (int)strlen(lbl[i].lbl));

    DeleteObject(f);
}

/* ── Panel lateral ───────────────────────────────────────────── */

static void txtColor(HDC hdc, int x, int y, int w, int h,
                     const char *txt, COLORREF col, int sz, BOOL gras)
{
    HFONT f = CreateFont(sz, 0, 0, 0, gras ? FW_BOLD : FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, "Segoe UI");
    SelectObject(hdc, f);
    SetTextColor(hdc, col);
    RECT r = {x, y, x + w, y + h};
    DrawText(hdc, txt, -1, &r, DT_LEFT | DT_WORDBREAK);
    DeleteObject(f);
}

void dessinerPanelLateral(HDC hdc, int zone_sel, int id_patient)
{
    /* Fond panel */
    HBRUSH br = CreateSolidBrush(CLR_PANEL_FOND);
    RECT   rp = {CORPS_PANEL_X, 0, CORPS_PANEL_X + CORPS_PANEL_W, CORPS_FENETRE_H};
    FillRect(hdc, &rp, br);
    DeleteObject(br);

    /* Ligne de separation */
    HPEN sp = CreatePen(PS_SOLID, 3, CLR_SEPARATEUR);
    SelectObject(hdc, sp);
    MoveToEx(hdc, CORPS_PANEL_X, 0, NULL);
    LineTo(hdc, CORPS_PANEL_X, CORPS_FENETRE_H);
    DeleteObject(sp);

    SetBkMode(hdc, TRANSPARENT);
    int px = CORPS_PANEL_X + 14;
    int pw = CORPS_PANEL_W - 24;

    if (zone_sel < 0) {
        /* Invite */
        txtColor(hdc, px, 20, pw, 50, "Diagnostic Corporel",
                 CLR_PANEL_TITRE, 20, TRUE);

        /* Croix medicale */
        HPEN cp = CreatePen(PS_SOLID, 7, CLR_PANEL_TITRE);
        SelectObject(hdc, cp);
        int cx = CORPS_PANEL_X + CORPS_PANEL_W / 2, cy = 170;
        MoveToEx(hdc, cx, cy - 32, NULL); LineTo(hdc, cx, cy + 32);
        MoveToEx(hdc, cx - 32, cy, NULL); LineTo(hdc, cx + 32, cy);
        DeleteObject(cp);

        txtColor(hdc, px, 220, pw, 120,
                 "Cliquez sur une zone\ndu corps pour afficher\nles symptomes et\ndiagnostics associes.",
                 CLR_GRIS_CLAIR, 14, FALSE);

        txtColor(hdc, px, 360, pw, 40,
                 "Clic droit = deselectionner",
                 CLR_GRIS_CLAIR, 11, FALSE);
        return;
    }

    ZoneCorporelle *z = &zonesCorps[zone_sel];
    int y = 15;

    /* Titre zone */
    txtColor(hdc, px, y, pw, 36, z->nom, CLR_PANEL_TITRE, 22, TRUE);
    y += 40;

    /* Separateur */
    HPEN lp = CreatePen(PS_SOLID, 1, CLR_SEPARATEUR);
    SelectObject(hdc, lp);
    MoveToEx(hdc, px, y, NULL); LineTo(hdc, px + pw, y);
    y += 10;
    DeleteObject(lp);

    /* Symptomes */
    txtColor(hdc, px, y, pw, 22, "SYMPTOMES ASSOCIES", CLR_SYMPTOME, 13, TRUE);
    y += 26;
    for (int i = 0; i < MAX_SYMPTOMES_ZONE; i++) {
        char buf[120]; snprintf(buf, sizeof(buf), "  . %s", z->symptomes[i]);
        txtColor(hdc, px, y, pw, 36, buf, CLR_BLANC, 13, FALSE);
        y += 38;
    }

    /* Separateur */
    HPEN lp2 = CreatePen(PS_SOLID, 1, CLR_SEPARATEUR);
    SelectObject(hdc, lp2);
    MoveToEx(hdc, px, y, NULL); LineTo(hdc, px + pw, y);
    y += 10;
    DeleteObject(lp2);

    /* Diagnostics */
    txtColor(hdc, px, y, pw, 22, "DIAGNOSTICS POSSIBLES", CLR_DIAGNOSTIC_C, 13, TRUE);
    y += 26;
    for (int i = 0; i < MAX_DIAG_ZONE; i++) {
        char buf[120]; snprintf(buf, sizeof(buf), "  . %s", z->diagnostics_possibles[i]);
        txtColor(hdc, px, y, pw, 36, buf, CLR_GRIS_CLAIR, 13, FALSE);
        y += 38;
    }

    /* Separateur */
    HPEN lp3 = CreatePen(PS_SOLID, 1, CLR_SEPARATEUR);
    SelectObject(hdc, lp3);
    MoveToEx(hdc, px, y, NULL); LineTo(hdc, px + pw, y);
    y += 10;
    DeleteObject(lp3);

    /* Conseil */
    txtColor(hdc, px, y, pw, 22, "CONSEIL MEDICAL", CLR_CONSEIL, 13, TRUE);
    y += 26;
    txtColor(hdc, px, y, pw, 70, z->conseil, RGB(255, 195, 155), 12, FALSE);
    y += 75;

    /* Bouton "Enregistrer comme symptome" si patient lie */
    if (id_patient >= 0) {
        HBRUSH brBtn = CreateSolidBrush(CLR_PANEL_TITRE);
        RECT   rBtn  = {px, CORPS_FENETRE_H - 75,
                        px + pw, CORPS_FENETRE_H - 35};
        FillRect(hdc, &rBtn, brBtn);
        DeleteObject(brBtn);
        txtColor(hdc, px + 5, CORPS_FENETRE_H - 72, pw - 10, 34,
                 "-> Creer consultation pour ce patient",
                 RGB(10, 30, 50), 13, TRUE);
    } else {
        txtColor(hdc, px, CORPS_FENETRE_H - 55, pw, 40,
                 "Mode libre. Associez un patient\ndepuis le menu avance.",
                 CLR_GRIS_CLAIR, 11, FALSE);
    }
}

/* ── Procedure Win32 principale ──────────────────────────────── */

LRESULT CALLBACK WndProcDiagCorps(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    EtatFenetreCorps *etat = (EtatFenetreCorps *)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (msg) {

    case WM_MOUSEMOVE: {
        int mx = LOWORD(lp), my = HIWORD(lp);
        int ancien = etat->zone_survol;
        etat->zone_survol = trouverZoneSousCurseur(mx, my);
        SetCursor(LoadCursor(NULL,
            etat->zone_survol >= 0 ? IDC_HAND : IDC_ARROW));
        if (etat->zone_survol != ancien)
            InvalidateRect(hwnd, NULL, FALSE);
        break;
    }

    case WM_LBUTTONDOWN: {
        int mx = LOWORD(lp), my = HIWORD(lp);
        if (mx >= CORPS_PANEL_X) break;
        int z = trouverZoneSousCurseur(mx, my);
        if (z != etat->zone_selectionnee) {
            etat->zone_selectionnee = z;
            InvalidateRect(hwnd, NULL, FALSE);
        }
        break;
    }

    case WM_RBUTTONDOWN:
        etat->zone_selectionnee = -1;
        InvalidateRect(hwnd, NULL, FALSE);
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdcE = BeginPaint(hwnd, &ps);

        /* Double buffering — supprime le clignotement */
        HDC     hdcM = CreateCompatibleDC(hdcE);
        HBITMAP bmp  = CreateCompatibleBitmap(hdcE,
                            CORPS_FENETRE_W, CORPS_FENETRE_H);
        SelectObject(hdcM, bmp);

        /* Fond */
        HBRUSH brf = CreateSolidBrush(CLR_FOND_FENETRE);
        RECT   rf  = {0, 0, CORPS_FENETRE_W, CORPS_FENETRE_H};
        FillRect(hdcM, &rf, brf);
        DeleteObject(brf);

        /* Titre fenetre */
        SetBkMode(hdcM, TRANSPARENT);
        HFONT ft = CreateFont(15, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH, "Segoe UI");
        SelectObject(hdcM, ft);
        SetTextColor(hdcM, RGB(30, 70, 120));
        TextOut(hdcM, 8, 6, "SICKBASE  .  Diagnostic Corporel Interactif", 43);
        DeleteObject(ft);

        /* Corps */
        dessinerCorpsHumain(hdcM);

        /* Survol (rouge) */
        if (etat->zone_survol >= 0 &&
            etat->zone_survol != etat->zone_selectionnee)
            dessinerSurbrillanceZone(hdcM, etat->zone_survol, CLR_SURVOL);

        /* Selection (vert) */
        if (etat->zone_selectionnee >= 0)
            dessinerSurbrillanceZone(hdcM, etat->zone_selectionnee, CLR_SELECTION);

        /* Etiquettes */
        dessinerEtiquettesZones(hdcM);

        /* Panel */
        dessinerPanelLateral(hdcM, etat->zone_selectionnee, etat->id_patient);

        /* Flush */
        BitBlt(hdcE, 0, 0, CORPS_FENETRE_W, CORPS_FENETRE_H, hdcM, 0, 0, SRCCOPY);
        DeleteDC(hdcM);
        DeleteObject(bmp);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_SETCURSOR: return TRUE;
    case WM_ERASEBKGND: return 1;

    case WM_DESTROY:
        if (etat) free(etat);
        /* Ne pas appeler PostQuitMessage — on est une sous-fenetre de SICKBASE */
        break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wp, lp);
}

/* ── Point d'entree public ───────────────────────────────────── */

void lancerDiagnosticCorporel(int id_patient, int id_medecin)
{
    initialiserZonesCorporelles();

    HINSTANCE hInst = GetModuleHandle(NULL);

    /* Enregistrement classe (une seule fois) */
    WNDCLASSEX wc = {0};
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.lpfnWndProc   = WndProcDiagCorps;
    wc.hInstance     = hInst;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "SBDiagCorps";
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    RegisterClassEx(&wc);   /* ignore l'echec si deja enregistree */

    /* Centrage */
    int sx = GetSystemMetrics(SM_CXSCREEN);
    int sy = GetSystemMetrics(SM_CYSCREEN);

    HWND hwnd = CreateWindowEx(
        0, "SBDiagCorps",
        "SICKBASE  -  Diagnostic Corporel Interactif",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        (sx - CORPS_FENETRE_W) / 2,
        (sy - CORPS_FENETRE_H) / 2,
        CORPS_FENETRE_W, CORPS_FENETRE_H,
        NULL, NULL, hInst, NULL);

    if (!hwnd) return;

    /* Etat interne */
    EtatFenetreCorps *etat = (EtatFenetreCorps *)malloc(sizeof(EtatFenetreCorps));
    etat->zone_selectionnee = -1;
    etat->zone_survol       = -1;
    etat->id_patient        = id_patient;
    etat->id_medecin        = id_medecin;
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)etat);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    /* Boucle de messages locale (bloquante jusqu'a fermeture) */
    MSG msg;
    while (GetMessage(&msg, hwnd, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
