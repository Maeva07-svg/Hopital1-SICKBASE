#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "diagnostic.h"
#include "patient.h"
#include "fichiers.h"
#include "utils.h"
#include "comptes.h"

// Définition des variables globales
Maladie maladies[MAX_MALADIES];
int nombreMaladies = 8;
DiagnosticMedical diagnostics[MAX_PATIENTS * 10];
int nombreDiagnostics = 0;
int dernierIDDiagnostic = 0;

// Tableau des questions pour le diagnostic guidé
QuestionDiagnostic questions[MAX_QUESTIONS];
int nombreQuestions = 0;

void initialiserMaladies()
{
    // Maladie 1: Paludisme
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

    // Maladie 2: Typhoïde
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

    // Maladie 3: Angine
    strcpy(maladies[2].nom, "Angine");
    strcpy(maladies[2].symptomes[0], "maux de gorge");
    strcpy(maladies[2].symptomes[1], "difficulte a avaler");
    strcpy(maladies[2].symptomes[2], "fievre moderee");
    strcpy(maladies[2].symptomes[3], "ganglions enflés");
    maladies[2].nb_symptomes = 4;
    strcpy(maladies[2].description, "Inflammation des amygdales, virale ou bacterienne");
    strcpy(maladies[2].traitement, "Antibiotiques si bacterien, anti-inflammatoires");
    strcpy(maladies[2].precautions, "Repos, gargarismes, boissons chaudes");

    // Maladie 4: COVID-19
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

    // Maladie 5: Migraine
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

    // Maladie 6: Infection urinaire
    strcpy(maladies[5].nom, "Infection urinaire");
    strcpy(maladies[5].symptomes[0], "brulures urinaires");
    strcpy(maladies[5].symptomes[1], "envies frequentes d'uriner");
    strcpy(maladies[5].symptomes[2], "douleurs pelviennes");
    strcpy(maladies[5].symptomes[3], "urines troubles");
    maladies[5].nb_symptomes = 4;
    strcpy(maladies[5].description, "Infection bacterienne des voies urinaires");
    strcpy(maladies[5].traitement, "Antibiotiques, antalgiques urinaires");
    strcpy(maladies[5].precautions, "Boire beaucoup d'eau, hygiene intime");

    // Maladie 7: Grippe
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

    // Maladie 8: Gastroenterite
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

void initialiserQuestionsDiagnostic()
{
    nombreQuestions = 0;

    // Questions pour le Paludisme
    strcpy(questions[nombreQuestions].question, "Avez-vous de la fievre elevee (plus de 39°C) ?");
    strcpy(questions[nombreQuestions].mot_cle, "fievre elevee");
    questions[nombreQuestions].id_maladie_associee = 0;
    nombreQuestions++;

    strcpy(questions[nombreQuestions].question, "Ressentez-vous des frissons ?");
    strcpy(questions[nombreQuestions].mot_cle, "frissons");
    questions[nombreQuestions].id_maladie_associee = 0;
    nombreQuestions++;

    strcpy(questions[nombreQuestions].question, "Avez-vous des maux de tete intenses ?");
    strcpy(questions[nombreQuestions].mot_cle, "maux de tete");
    questions[nombreQuestions].id_maladie_associee = 0;
    nombreQuestions++;

    // Questions pour l'Angine
    strcpy(questions[nombreQuestions].question, "Avez-vous mal a la gorge ?");
    strcpy(questions[nombreQuestions].mot_cle, "maux de gorge");
    questions[nombreQuestions].id_maladie_associee = 2;
    nombreQuestions++;

    strcpy(questions[nombreQuestions].question, "Avez-vous du mal a avaler ?");
    strcpy(questions[nombreQuestions].mot_cle, "difficulte a avaler");
    questions[nombreQuestions].id_maladie_associee = 2;
    nombreQuestions++;

    // Questions pour la COVID-19
    strcpy(questions[nombreQuestions].question, "Avez-vous une toux seche ?");
    strcpy(questions[nombreQuestions].mot_cle, "toux seche");
    questions[nombreQuestions].id_maladie_associee = 3;
    nombreQuestions++;

    strcpy(questions[nombreQuestions].question, "Avez-vous perdu le gout ou l'odorat ?");
    strcpy(questions[nombreQuestions].mot_cle, "perte de gout");
    questions[nombreQuestions].id_maladie_associee = 3;
    nombreQuestions++;

    // Questions pour la Migraine
    strcpy(questions[nombreQuestions].question, "Avez-vous des maux de tete violents ?");
    strcpy(questions[nombreQuestions].mot_cle, "maux de tete severes");
    questions[nombreQuestions].id_maladie_associee = 4;
    nombreQuestions++;

    strcpy(questions[nombreQuestions].question, "Etes-vous sensible a la lumiere ?");
    strcpy(questions[nombreQuestions].mot_cle, "sensibilite a la lumiere");
    questions[nombreQuestions].id_maladie_associee = 4;
    nombreQuestions++;

    // Questions pour Infection urinaire
    strcpy(questions[nombreQuestions].question, "Ressentez-vous des brulures en urinant ?");
    strcpy(questions[nombreQuestions].mot_cle, "brulures urinaires");
    questions[nombreQuestions].id_maladie_associee = 5;
    nombreQuestions++;

    strcpy(questions[nombreQuestions].question, "Avez-vous envie d'uriner tres frequemment ?");
    strcpy(questions[nombreQuestions].mot_cle, "envies frequentes");
    questions[nombreQuestions].id_maladie_associee = 5;
    nombreQuestions++;

    // Questions pour la Grippe
    strcpy(questions[nombreQuestions].question, "Avez-vous des douleurs musculaires ?");
    strcpy(questions[nombreQuestions].mot_cle, "douleurs musculaires");
    questions[nombreQuestions].id_maladie_associee = 6;
    nombreQuestions++;

    // Questions pour Gastro-enterite
    strcpy(questions[nombreQuestions].question, "Avez-vous des nausees ou vomissements ?");
    strcpy(questions[nombreQuestions].mot_cle, "nausees");
    questions[nombreQuestions].id_maladie_associee = 7;
    nombreQuestions++;

    strcpy(questions[nombreQuestions].question, "Avez-vous la diarrhee ?");
    strcpy(questions[nombreQuestions].mot_cle, "diarrhee");
    questions[nombreQuestions].id_maladie_associee = 7;
    nombreQuestions++;
}

// NOUVELLE FONCTION: Diagnostic par questions-réponses
void diagnosticGuideParQuestions()
{
    system("cls");
    printf("\n=== DIAGNOSTIC GUIDE PAR QUESTIONS ===\n\n");
    printf("Repondez par O (Oui) ou N (Non) aux questions suivantes :\n\n");

    int reponses[MAX_QUESTIONS] = {0};
    int scores[MAX_MALADIES] = {0};

    for (int i = 0; i < nombreQuestions; i++)
    {
        printf("%d. %s (O/N): ", i + 1, questions[i].question);
        char reponse;
        scanf("%c", &reponse);
        viderBuffer();

        if (toupper(reponse) == 'O')
        {
            reponses[i] = 1;
            scores[questions[i].id_maladie_associee]++;
        }
    }

    printf("\n=== ANALYSE DES REPONSES ===\n\n");

    // Afficher les maladies probables
    int trouve = 0;
    for (int m = 0; m < nombreMaladies; m++)
    {
        if (scores[m] > 0)
        {
            float pourcentage = (float)scores[m] / maladies[m].nb_symptomes * 100;
            if (pourcentage >= 30)
            {
                trouve = 1;
                printf("\n-> %s (%.0f%% de probabilite)\n", maladies[m].nom, pourcentage);
                printf("   Symptomes correspondants: %d/%d\n", scores[m], maladies[m].nb_symptomes);
                printf("   Description: %s\n", maladies[m].description);
                printf("   Traitement: %s\n", maladies[m].traitement);
                printf("   Precautions: %s\n", maladies[m].precautions);
            }
        }
    }

    if (!trouve)
    {
        printf("Aucune maladie identifiee avec certitude.\n");
        printf("Consultez un medecin pour un examen approfondi.\n");
    }

    pause();
}

// Diagnostic direct par symptômes (version améliorée avec suggestions)
void diagnosticDirectParSymptomes()
{
    system("cls");
    printf("\n=== DIAGNOSTIC PAR SYMPTOMES ===\n\n");

    char symptomes_entres[MAX_SYMPTOMS][MAX_SYMPTOM_LENGTH];
    int nb_symptomes_entres = 0;

    printf("Entrez vos symptomes (un par ligne, 'fin' pour terminer):\n");
    printf("Suggestions: fievre, toux, maux de tete, fatigue, nausees, etc.\n\n");

    while (nb_symptomes_entres < MAX_SYMPTOMS)
    {
        printf("Symptome %d: ", nb_symptomes_entres + 1);
        fgets(symptomes_entres[nb_symptomes_entres], MAX_SYMPTOM_LENGTH, stdin);
        symptomes_entres[nb_symptomes_entres][strcspn(symptomes_entres[nb_symptomes_entres], "\n")] = '\0';

        if (strcmp(symptomes_entres[nb_symptomes_entres], "fin") == 0)
            break;

        if (strlen(symptomes_entres[nb_symptomes_entres]) > 0)
            nb_symptomes_entres++;
    }

    if (nb_symptomes_entres == 0)
    {
        printf("Aucun symptome entre.\n");
        pause();
        return;
    }

    printf("\n=== RESULTATS DU DIAGNOSTIC ===\n\n");

    int trouve = 0;
    for (int m = 0; m < nombreMaladies; m++)
    {
        int correspondances = 0;
        for (int s1 = 0; s1 < nb_symptomes_entres; s1++)
        {
            for (int s2 = 0; s2 < maladies[m].nb_symptomes; s2++)
            {
                if (comparerSymptomes(symptomes_entres[s1], maladies[m].symptomes[s2]))
                {
                    correspondances++;
                    break;
                }
            }
        }

        float score = (float)correspondances / maladies[m].nb_symptomes * 100;

        if (score >= 40.0)
        {
            trouve = 1;
            printf("\n-> %s (%.0f%% de correspondance)\n", maladies[m].nom, score);
            printf("   Description: %s\n", maladies[m].description);
            printf("   Traitement: %s\n", maladies[m].traitement);
            printf("   Precautions: %s\n", maladies[m].precautions);
        }
    }

    if (!trouve)
    {
        printf("Aucune maladie ne correspond suffisamment aux symptomes entres.\n");
        printf("Consultez un medecin pour un diagnostic precis.\n");
    }

    pause();
}

void diagnosticPatientExistant(int index_patient)
{
    if (index_patient < 0 || index_patient >= nombrePatients)
    {
        printf("Erreur: Patient non trouve.\n");
        return;
    }

    Patient *p = &patients[index_patient];

    if (p->nb_symptomes == 0)
    {
        printf("\nCe patient n'a aucun symptome enregistre.\n");
        pause();
        return;
    }

    system("cls");
    printf("\n=== DIAGNOSTIC POUR %s %s (ID: %d) ===\n\n",
           p->prenom, p->nom, p->id);

    printf("Symptomes du patient (%d):\n", p->nb_symptomes);
    for (int i = 0; i < p->nb_symptomes; i++)
    {
        printf("  - %s\n", p->symptomes[i]);
    }

    printf("\n=== ANALYSE EN COURS ===\n\n");

    int trouve_diagnostic = 0;
    float meilleur_score = 0.0;
    int meilleure_maladie = -1;

    for (int m = 0; m < nombreMaladies; m++)
    {
        int correspondances = 0;

        for (int s1 = 0; s1 < p->nb_symptomes; s1++)
        {
            for (int s2 = 0; s2 < maladies[m].nb_symptomes; s2++)
            {
                if (comparerSymptomes(p->symptomes[s1], maladies[m].symptomes[s2]))
                {
                    correspondances++;
                    break;
                }
            }
        }

        float score = (float)correspondances / maladies[m].nb_symptomes * 100;

        if (score >= 40.0)
        {
            trouve_diagnostic = 1;
            printf("\n-> %s (%.0f%% de correspondance)\n", maladies[m].nom, score);
            printf("   Symptomes correspondants: %d/%d\n", correspondances, maladies[m].nb_symptomes);

            if (score > meilleur_score)
            {
                meilleur_score = score;
                meilleure_maladie = m;
            }
        }
    }

    if (!trouve_diagnostic)
    {
        printf("\nAucune maladie ne correspond suffisamment aux symptomes du patient.\n");
    }
    else
    {
        printf("\n=== RECOMMANDATION ===\n");

        if (meilleur_score >= 70.0)
        {
            printf("\nDiagnostic probable: %s (%.0f%%)\n", maladies[meilleure_maladie].nom, meilleur_score);
            printf("Description: %s\n", maladies[meilleure_maladie].description);
            printf("Traitement: %s\n", maladies[meilleure_maladie].traitement);

            printf("\nVoulez-vous enregistrer ce diagnostic pour le patient? (O/N): ");
            char reponse;
            scanf("%c", &reponse);
            viderBuffer();

            if (toupper(reponse) == 'O')
            {
                strcpy(p->diagnostic, maladies[meilleure_maladie].nom);
                sauvegarderPatients();
                printf("Diagnostic enregistre avec succes!\n");
            }
        }
        else if (meilleur_score >= 50.0)
        {
            printf("\nPlusieurs maladies possibles. La plus probable: %s (%.0f%%)\n",
                   maladies[meilleure_maladie].nom, meilleur_score);
            printf("Il est recommande de faire des examens complementaires.\n");
        }
        else
        {
            printf("\nCorrespondance insuffisante pour un diagnostic precis.\n");
            printf("Consultez un medecin pour des examens plus pousses.\n");
        }
    }

    pause();
}

//Créer un diagnostic personnalisé pour un patient
void creerDiagnosticPourPatient(int id_patient, int id_medecin)
{
    system("cls");
    printf("\n=== CREATION D'UN DIAGNOSTIC MEDICAL ===\n\n");

    int idx_patient = rechercherParID(id_patient);
    if (idx_patient == -1)
    {
        printf("Patient non trouve.\n");
        pause();
        return;
    }

    printf("Patient: %s %s (ID: %d)\n\n", patients[idx_patient].prenom, patients[idx_patient].nom, id_patient);

    DiagnosticMedical diag;
    memset(&diag, 0, sizeof(DiagnosticMedical));

    diag.id_diagnostic = ++dernierIDDiagnostic;
    diag.id_patient = id_patient;
    diag.id_medecin = id_medecin;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(diag.date_diagnostic, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    printf("1. Diagnostic (maladie identifiee):\n   ");
    fgets(diag.maladie, 100, stdin);
    diag.maladie[strcspn(diag.maladie, "\n")] = '\0';

    printf("\n2. Description detaillee:\n   ");
    fgets(diag.description, 500, stdin);
    diag.description[strcspn(diag.description, "\n")] = '\0';

    printf("\n3. Traitement prescrit:\n   ");
    fgets(diag.traitement_prescrit, 500, stdin);
    diag.traitement_prescrit[strcspn(diag.traitement_prescrit, "\n")] = '\0';

    printf("\n4. Medicaments prescrits:\n   ");
    fgets(diag.medicaments, 300, stdin);
    diag.medicaments[strcspn(diag.medicaments, "\n")] = '\0';

    printf("\n5. Examens complementaires demandes:\n   ");
    fgets(diag.examens_complementaires, 300, stdin);
    diag.examens_complementaires[strcspn(diag.examens_complementaires, "\n")] = '\0';

    printf("\n6. Recommandations et conseils:\n   ");
    fgets(diag.recommandations, 500, stdin);
    diag.recommandations[strcspn(diag.recommandations, "\n")] = '\0';

    printf("\n7. Prochain rendez-vous (JJ/MM/AAAA) ou 'Aucun':\n   ");
    fgets(diag.prochain_rdv, 20, stdin);
    diag.prochain_rdv[strcspn(diag.prochain_rdv, "\n")] = '\0';

    printf("\n8. Notes complementaires:\n   ");
    fgets(diag.notes, 300, stdin);
    diag.notes[strcspn(diag.notes, "\n")] = '\0';

    diagnostics[nombreDiagnostics] = diag;
    nombreDiagnostics++;

    // Mettre à jour le diagnostic dans le dossier patient
    strcpy(patients[idx_patient].diagnostic, diag.maladie);
    sauvegarderPatients();
    sauvegarderDiagnostics();

    printf("\n=== DIAGNOSTIC ENREGISTRE AVEC SUCCES ===\n");
    printf("ID Diagnostic: %d\n", diag.id_diagnostic);

    pause();
}

// Afficher tous les diagnostics d'un patient
void afficherDiagnosticsPatient(int id_patient)
{
    system("cls");
    printf("\n=== HISTORIQUE DES DIAGNOSTICS ===\n");
    printf("Patient ID: %d\n\n", id_patient);

    int compteur = 0;
    for (int i = 0; i < nombreDiagnostics; i++)
    {
        if (diagnostics[i].id_patient == id_patient)
        {
            compteur++;
            printf("\n[%d] Diagnostic #%d - %s\n", compteur, diagnostics[i].id_diagnostic, diagnostics[i].date_diagnostic);
            printf("    Maladie: %s\n", diagnostics[i].maladie);
            printf("    Traitement: %.50s...\n", diagnostics[i].traitement_prescrit);
        }
    }

    if (compteur == 0)
    {
        printf("Aucun diagnostic enregistre pour ce patient.\n");
    }
    else
    {
        printf("\nEntrez l'ID du diagnostic pour voir les details (0 pour quitter): ");
        int id;
        scanf("%d", &id);
        viderBuffer();

        if (id > 0)
        {
            afficherDetailDiagnostic(id);
        }
    }

    pause();
}

// Afficher les détails complets d'un diagnostic
void afficherDetailDiagnostic(int id_diagnostic)
{
    for (int i = 0; i < nombreDiagnostics; i++)
    {
        if (diagnostics[i].id_diagnostic == id_diagnostic)
        {
            printf("\n========================================\n");
            printf("DIAGNOSTIC MEDICAL #%d\n", diagnostics[i].id_diagnostic);
            printf("========================================\n");
            printf("Date: %s\n", diagnostics[i].date_diagnostic);
            printf("Patient ID: %d\n", diagnostics[i].id_patient);
            printf("Medecin ID: %d\n", diagnostics[i].id_medecin);
            printf("\n--- DIAGNOSTIC ---\n");
            printf("%s\n", diagnostics[i].maladie);
            printf("\n--- DESCRIPTION ---\n");
            printf("%s\n", diagnostics[i].description);
            printf("\n--- TRAITEMENT ---\n");
            printf("%s\n", diagnostics[i].traitement_prescrit);
            printf("\n--- MEDICAMENTS ---\n");
            printf("%s\n", diagnostics[i].medicaments);
            printf("\n--- EXAMENS DEMANDES ---\n");
            printf("%s\n", diagnostics[i].examens_complementaires);
            printf("\n--- RECOMMANDATIONS ---\n");
            printf("%s\n", diagnostics[i].recommandations);
            if (strlen(diagnostics[i].prochain_rdv) > 0 && strcmp(diagnostics[i].prochain_rdv, "Aucun") != 0)
                printf("\n--- PROCHAIN RENDEZ-VOUS ---\n%s\n", diagnostics[i].prochain_rdv);
            if (strlen(diagnostics[i].notes) > 0)
                printf("\n--- NOTES ---\n%s\n", diagnostics[i].notes);
            printf("========================================\n");
            return;
        }
    }
    printf("Diagnostic non trouve.\n");
}

// Modifier un diagnostic existant
void modifierDiagnostic(int id_diagnostic)
{
    for (int i = 0; i < nombreDiagnostics; i++)
    {
        if (diagnostics[i].id_diagnostic == id_diagnostic)
        {
            printf("\n=== MODIFICATION DU DIAGNOSTIC #%d ===\n", id_diagnostic);
            printf("Patient ID: %d\n", diagnostics[i].id_patient);

            printf("\nNouveau diagnostic (actuel: %s):\n   ", diagnostics[i].maladie);
            fgets(diagnostics[i].maladie, 100, stdin);
            diagnostics[i].maladie[strcspn(diagnostics[i].maladie, "\n")] = '\0';
            if (strlen(diagnostics[i].maladie) == 0)
                strcpy(diagnostics[i].maladie, "Non modifie");

            printf("\nNouveau traitement (actuel: %.50s...):\n   ", diagnostics[i].traitement_prescrit);
            char temp[500];
            fgets(temp, 500, stdin);
            temp[strcspn(temp, "\n")] = '\0';
            if (strlen(temp) > 0)
                strcpy(diagnostics[i].traitement_prescrit, temp);

            sauvegarderDiagnostics();
            printf("Diagnostic modifie avec succes.\n");
            pause();
            return;
        }
    }
    printf("Diagnostic non trouve.\n");
    pause();
}

// Sauvegarde des diagnostics
void sauvegarderDiagnostics()
{
    FILE *f = fopen("diagnostics.dat", "wb");
    if (f == NULL) return;

    fwrite(&nombreDiagnostics, sizeof(int), 1, f);
    fwrite(diagnostics, sizeof(DiagnosticMedical), nombreDiagnostics, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_diagnostic.dat", "wb");
    if (fid != NULL)
    {
        fwrite(&dernierIDDiagnostic, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void chargerDiagnostics()
{
    FILE *f = fopen("diagnostics.dat", "rb");
    if (f == NULL) return;

    fread(&nombreDiagnostics, sizeof(int), 1, f);
    fread(diagnostics, sizeof(DiagnosticMedical), nombreDiagnostics, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_diagnostic.dat", "rb");
    if (fid != NULL)
    {
        fread(&dernierIDDiagnostic, sizeof(int), 1, fid);
        fclose(fid);
    }
}

// Menu principal du diagnostic
void menuDiagnosticMedical()
{
    int choix;

    do
    {
        system("cls");
        color(11, 0);
        printf("\n=== SYSTEME DE DIAGNOSTIC MEDICAL ===\n\n");
        color(7, 0);

        printf("1. Diagnostic guide par questions (simple)\n");
        printf("2. Diagnostic par saisie de symptomes\n");
        printf("3. Base de donnees des maladies\n");
        printf("4. Retour\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                diagnosticGuideParQuestions();
                break;
            case 2:
                diagnosticDirectParSymptomes();
                break;
            case 3:
                afficherToutesMaladies();
                pause();
                break;
            case 4:
                return;
            default:
                printf("Choix invalide.\n");
                pause();
        }

    } while(choix != 4);
}

// Menu avancé pour les médecins (diagnostic patient + historique)
void menuDiagnosticMedicalAvance()
{
    int choix;

    do
    {
        system("cls");
        color(11, 0);
        printf("\n=== DIAGNOSTIC MEDICAL AVANCE ===\n\n");
        color(7, 0);

        printf("1. Creer un diagnostic pour un patient\n");
        printf("2. Consulter historique des diagnostics d'un patient\n");
        printf("3. Modifier un diagnostic\n");
        printf("4. Diagnostic guide par questions\n");
        printf("5. Diagnostic par symptomes\n");
        printf("6. Base de donnees des maladies\n");
        printf("7. Retour\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                {
                    if (nombrePatients == 0)
                    {
                        printf("Aucun patient enregistre.\n");
                        pause();
                        break;
                    }
                    afficherListePatientsSimple();
                    printf("\nID du patient: ");
                    int id_patient;
                    scanf("%d", &id_patient);
                    viderBuffer();

                    if (rechercherParID(id_patient) != -1)
                    {
                        creerDiagnosticPourPatient(id_patient, utilisateur_actuel ? utilisateur_actuel->id_associe : 1);
                    }
                    else
                    {
                        printf("Patient non trouve.\n");
                        pause();
                    }
                }
                break;

            case 2:
                {
                    if (nombrePatients == 0)
                    {
                        printf("Aucun patient enregistre.\n");
                        pause();
                        break;
                    }
                    afficherListePatientsSimple();
                    printf("\nID du patient: ");
                    int id_patient;
                    scanf("%d", &id_patient);
                    viderBuffer();
                    afficherDiagnosticsPatient(id_patient);
                }
                break;

            case 3:
                {
                    printf("ID du diagnostic a modifier: ");
                    int id;
                    scanf("%d", &id);
                    viderBuffer();
                    modifierDiagnostic(id);
                }
                break;

            case 4:
                diagnosticGuideParQuestions();
                break;

            case 5:
                diagnosticDirectParSymptomes();
                break;

            case 6:
                afficherToutesMaladies();
                pause();
                break;

            case 7:
                return;

            default:
                printf("Choix invalide.\n");
                pause();
        }

    } while(choix != 7);
}

void afficherToutesMaladies()
{
    system("cls");
    printf("\n=== BASE DE DONNEES DES MALADIES ===\n\n");

    for (int i = 0; i < nombreMaladies; i++)
    {
        printf("\n%d. %s\n", i + 1, maladies[i].nom);
        printf("   Symptomes: ");
        for (int j = 0; j < maladies[i].nb_symptomes; j++)
        {
            printf("%s", maladies[i].symptomes[j]);
            if (j < maladies[i].nb_symptomes - 1) printf(", ");
        }
        printf("\n   Description: %s\n", maladies[i].description);
        printf("   Traitement: %s\n", maladies[i].traitement);
        printf("   Precautions: %s\n", maladies[i].precautions);
    }
}
