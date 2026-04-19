#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "laboratoire.h"
#include "utils.h"
#include "patient.h"
#include "personnel.h"
#include "consultations.h"
#include "comptes.h"

// Définition des variables globales
TypeAnalyse types_analyse[MAX_ANALYSES];
int nombreTypesAnalyse = 0;
int dernierIDTypeAnalyse = 0;
Analyse analyses[MAX_ANALYSES];
int nombreAnalyses = 0;
int dernierIDAnalyse = 0;

void initialiserLaboratoire()
{
    chargerTypesAnalyse();
    chargerAnalyses();

    if (nombreTypesAnalyse == 0)
    {
        initialiserTypesAnalyse();
    }
}

void initialiserTypesAnalyse()
{
    // Type 1: NFS
    strcpy(types_analyse[0].nom, "NFS");
    strcpy(types_analyse[0].description, "Numeration Formule Sanguine");
    strcpy(types_analyse[0].valeurs_normales, "GR: 4.5-5.5M/µL, GB: 4-10K/µL");
    types_analyse[0].prix = 5000;
    types_analyse[0].duree_resultats = 4;
    types_analyse[0].id_type = ++dernierIDTypeAnalyse;
    nombreTypesAnalyse++;

    // Type 2: Glycémie
    strcpy(types_analyse[1].nom, "Glycemie");
    strcpy(types_analyse[1].description, "Taux de glucose dans le sang");
    strcpy(types_analyse[1].valeurs_normales, "0.70-1.10 g/L");
    types_analyse[1].prix = 3000;
    types_analyse[1].duree_resultats = 2;
    types_analyse[1].id_type = ++dernierIDTypeAnalyse;
    nombreTypesAnalyse++;

    // Type 3: Bilan lipidique
    strcpy(types_analyse[2].nom, "Bilan lipidique");
    strcpy(types_analyse[2].description, "Cholesterol, triglycerides");
    strcpy(types_analyse[2].valeurs_normales, "Chol total <2g/L");
    types_analyse[2].prix = 8000;
    types_analyse[2].duree_resultats = 6;
    types_analyse[2].id_type = ++dernierIDTypeAnalyse;
    nombreTypesAnalyse++;

    // Type 4: Bilan rénal
    strcpy(types_analyse[3].nom, "Bilan renal");
    strcpy(types_analyse[3].description, "Creatinine, uree");
    strcpy(types_analyse[3].valeurs_normales, "Creatinine: 6-12 mg/L");
    types_analyse[3].prix = 7000;
    types_analyse[3].duree_resultats = 5;
    types_analyse[3].id_type = ++dernierIDTypeAnalyse;
    nombreTypesAnalyse++;

    // Type 5: Test COVID
    strcpy(types_analyse[4].nom, "Test COVID-19");
    strcpy(types_analyse[4].description, "PCR SARS-CoV-2");
    strcpy(types_analyse[4].valeurs_normales, "Negatif");
    types_analyse[4].prix = 15000;
    types_analyse[4].duree_resultats = 24;
    types_analyse[4].id_type = ++dernierIDTypeAnalyse;
    nombreTypesAnalyse++;

    sauvegarderTypesAnalyse();
}

// ================= VÉRIFICATION DES DROITS (MÉDECIN) =================

int medecinEstLieAuPatient(int id_medecin, int id_patient)
{
    // Vérifier si le médecin a une consultation avec ce patient
    for (int i = 0; i < nombreConsultations; i++)
    {
        if (consultations[i].id_medecin == id_medecin && consultations[i].id_patient == id_patient)
        {
            return 1;
        }
    }

    // Vérifier si le médecin a un rendez-vous avec ce patient
    for (int i = 0; i < nombreRendezVous; i++)
    {
        if (rendezVous[i].id_medecin == id_medecin && rendezVous[i].id_patient == id_patient)
        {
            return 1;
        }
    }

    return 0;
}

// ================= FONCTIONS POUR LE MÉDECIN =================

void demanderAnalyse(int id_patient, int id_medecin)
{
    system("cls");
    printf("\n=== DEMANDE D'ANALYSE ===\n\n");

    int idx_patient = rechercherParID(id_patient);
    if (idx_patient == -1)
    {
        printf("Patient non trouve.\n");
        pause();
        return;
    }

    // Vérifier que le médecin a le droit de prescrire pour ce patient
    if (!medecinEstLieAuPatient(id_medecin, id_patient))
    {
        printf("Acces non autorise. Vous n'etes pas le medecin traitant de ce patient.\n");
        pause();
        return;
    }

    printf("Patient: %s %s\n\n", patients[idx_patient].prenom, patients[idx_patient].nom);

    // Afficher les types d'analyse disponibles
    printf("TYPES D'ANALYSE DISPONIBLES:\n");
    printf("ID   | Nom                    | Prix      | Delai\n");
    printf("-----|------------------------|-----------|--------\n");

    for (int i = 0; i < nombreTypesAnalyse; i++)
    {
        printf("%-4d | %-22s | %9.0f | %dh\n",
               types_analyse[i].id_type,
               types_analyse[i].nom,
               types_analyse[i].prix,
               types_analyse[i].duree_resultats);
    }

    printf("\nID du type d'analyse: ");
    int id_type;
    scanf("%d", &id_type);
    viderBuffer();

    // Vérifier si le type existe
    int type_existe = 0;
    for (int i = 0; i < nombreTypesAnalyse; i++)
    {
        if (types_analyse[i].id_type == id_type)
        {
            type_existe = 1;
            break;
        }
    }

    if (!type_existe)
    {
        printf("Type d'analyse invalide.\n");
        pause();
        return;
    }

    Analyse a;
    memset(&a, 0, sizeof(Analyse));

    a.id_analyse = ++dernierIDAnalyse;
    a.id_patient = id_patient;
    a.id_medecin = id_medecin;
    a.id_type_analyse = id_type;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(a.date_demande, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    printf("Priorite (Normale/Urgente/Tres urgente): ");
    fgets(a.priorite, 20, stdin);
    a.priorite[strcspn(a.priorite, "\n")] = '\0';

    printf("Notes particulieres: ");
    fgets(a.notes, 300, stdin);
    a.notes[strcspn(a.notes, "\n")] = '\0';

    strcpy(a.statut, "Demandee");

    analyses[nombreAnalyses] = a;
    nombreAnalyses++;

    sauvegarderAnalyses();

    printf("\n=== DEMANDE D'ANALYSE ENREGISTREE ===\n");
    printf("ID Analyse: %d\n", a.id_analyse);
    printf("Prix: %.0f FCFA\n", types_analyse[id_type-1].prix);
    printf("Statut: %s\n", a.statut);
    printf("\n La demande a ete envoyee au laboratoire.\n");

    pause();
}

void afficherDemandesParMedecin(int id_medecin)
{
    system("cls");
    printf("\n=== MES DEMANDES D'ANALYSE ===\n\n");

    int compteur = 0;
    printf("ID  | Patient                 | Type                | Date       | Statut\n");
    printf("----|-------------------------|---------------------|------------|-----------\n");

    for (int i = 0; i < nombreAnalyses; i++)
    {
        if (analyses[i].id_medecin == id_medecin)
        {
            compteur++;
            int idx_patient = rechercherParID(analyses[i].id_patient);
            char patient_nom[30] = "";
            if (idx_patient != -1)
                sprintf(patient_nom, "%s %s", patients[idx_patient].prenom, patients[idx_patient].nom);
            else
                sprintf(patient_nom, "ID %d", analyses[i].id_patient);

            char type_nom[100] = "Inconnu";
            for (int j = 0; j < nombreTypesAnalyse; j++)
            {
                if (types_analyse[j].id_type == analyses[i].id_type_analyse)
                {
                    strcpy(type_nom, types_analyse[j].nom);
                    break;
                }
            }

            printf("%-4d| %-23s | %-19s | %-10s | %s\n",
                   analyses[i].id_analyse,
                   patient_nom,
                   type_nom,
                   analyses[i].date_demande,
                   analyses[i].statut);
        }
    }

    if (compteur == 0)
    {
        printf("Aucune demande d'analyse.\n");
    }

    pause();
}

void consulterResultatsPatient(int id_patient)
{
    system("cls");
    printf("\n=== RESULTATS D'ANALYSE ===\n");
    printf("Patient ID: %d\n\n", id_patient);

    int compteur = 0;
    printf("ID  | Type                | Date demande | Date resultat | Statut\n");
    printf("----|---------------------|--------------|---------------|-----------\n");

    for (int i = 0; i < nombreAnalyses; i++)
    {
        if (analyses[i].id_patient == id_patient)
        {
            compteur++;
            char type_nom[100] = "Inconnu";
            for (int j = 0; j < nombreTypesAnalyse; j++)
            {
                if (types_analyse[j].id_type == analyses[i].id_type_analyse)
                {
                    strcpy(type_nom, types_analyse[j].nom);
                    break;
                }
            }

            printf("%-4d| %-19s | %-12s | %-13s | %s\n",
                   analyses[i].id_analyse,
                   type_nom,
                   analyses[i].date_demande,
                   analyses[i].date_resultat,
                   analyses[i].statut);

            // Afficher un message si les résultats sont disponibles
            if (strcmp(analyses[i].statut, "Validee") == 0)
            {
                printf("    Resultats disponibles - Cliquez pour voir les details\n");
            }
        }
    }

    if (compteur == 0)
    {
        printf("Aucune analyse demandee pour ce patient.\n");
    }
    else
    {
        printf("\nEntrez l'ID de l'analyse pour voir les resultats (0 pour quitter): ");
        int id;
        scanf("%d", &id);
        viderBuffer();

        if (id > 0)
        {
            afficherResultatsAnalyse(id);
        }
    }

    pause();
}

void afficherResultatsAnalyse(int id_analyse)
{
    for (int i = 0; i < nombreAnalyses; i++)
    {
        if (analyses[i].id_analyse == id_analyse)
        {
            printf("\n========================================\n");
            printf("         RESULTAT D'ANALYSE            \n");
            printf("========================================\n");

            int idx_patient = rechercherParID(analyses[i].id_patient);
            printf("Patient: %s %s\n", patients[idx_patient].prenom, patients[idx_patient].nom);

            char type_nom[100] = "Inconnu";
            float prix = 0;
            for (int j = 0; j < nombreTypesAnalyse; j++)
            {
                if (types_analyse[j].id_type == analyses[i].id_type_analyse)
                {
                    strcpy(type_nom, types_analyse[j].nom);
                    prix = types_analyse[j].prix;
                    break;
                }
            }
            printf("Analyse: %s\n", type_nom);
            printf("Prix: %.0f FCFA\n", prix);
            printf("Date demande: %s\n", analyses[i].date_demande);
            printf("Date resultat: %s\n", analyses[i].date_resultat);
            printf("Medecin demandeur ID: %d\n", analyses[i].id_medecin);

            if (strcmp(analyses[i].statut, "Validée") == 0)
            {
                printf("\n--- RESULTATS ---\n");
                printf("%s\n", analyses[i].resultat);

                printf("\n--- INTERPRETATION ---\n");
                printf("%s\n", analyses[i].interpretation);

                printf("\n--- VALEURS NORMALES DE REFERENCE ---\n");
                for (int j = 0; j < nombreTypesAnalyse; j++)
                {
                    if (types_analyse[j].id_type == analyses[i].id_type_analyse)
                    {
                        printf("%s\n", types_analyse[j].valeurs_normales);
                        break;
                    }
                }
            }
            else
            {
                printf("\n Resultats non encore disponibles.\n");
                printf("Statut actuel: %s\n", analyses[i].statut);
            }
            printf("========================================\n");
            return;
        }
    }
    printf("Analyse non trouvee.\n");
}

// ================= FONCTIONS POUR LE TECHNICIEN =================

void afficherAnalysesARealiser()
{
    system("cls");
    printf("\n=== ANALYSES A REALISER ===\n\n");

    int compteur = 0;
    printf("ID  | Patient                 | Type                | Demande le  | Priorite\n");
    printf("----|-------------------------|---------------------|-------------|------------\n");

    for (int i = 0; i < nombreAnalyses; i++)
    {
        if (strcmp(analyses[i].statut, "Demandée") == 0)
        {
            compteur++;
            int idx_patient = rechercherParID(analyses[i].id_patient);
            char patient_nom[30] = "";
            if (idx_patient != -1)
                sprintf(patient_nom, "%s %s", patients[idx_patient].prenom, patients[idx_patient].nom);
            else
                sprintf(patient_nom, "ID %d", analyses[i].id_patient);

            char type_nom[100] = "Inconnu";
            for (int j = 0; j < nombreTypesAnalyse; j++)
            {
                if (types_analyse[j].id_type == analyses[i].id_type_analyse)
                {
                    strcpy(type_nom, types_analyse[j].nom);
                    break;
                }
            }

            if (strcmp(analyses[i].priorite, "Tres urgente") == 0)
                color(12, 0);
            else if (strcmp(analyses[i].priorite, "Urgente") == 0)
                color(14, 0);
            else
                color(7, 0);

            printf("%-4d| %-23s | %-19s | %-11s | %s\n",
                   analyses[i].id_analyse,
                   patient_nom,
                   type_nom,
                   analyses[i].date_demande,
                   analyses[i].priorite);
            color(7, 0);
        }
    }

    if (compteur == 0)
    {
        printf("Aucune analyse a realiser.\n");
    }
    else
    {
        printf("\nTotal: %d analyse(s) a realiser\n", compteur);

        printf("\nEntrez l'ID de l'analyse a commencer (0 pour quitter): ");
        int id;
        scanf("%d", &id);
        viderBuffer();

        if (id > 0)
        {
            commencerAnalyse(id);
        }
    }

    pause();
}

void commencerAnalyse(int id_analyse)
{
    for (int i = 0; i < nombreAnalyses; i++)
    {
        if (analyses[i].id_analyse == id_analyse && strcmp(analyses[i].statut, "Demandée") == 0)
        {
            system("cls");
            printf("\n=== COMMENCER L'ANALYSE ===\n");

            int idx_patient = rechercherParID(analyses[i].id_patient);
            printf("Patient: %s %s\n", patients[idx_patient].prenom, patients[idx_patient].nom);

            char type_nom[100] = "Inconnu";
            for (int j = 0; j < nombreTypesAnalyse; j++)
            {
                if (types_analyse[j].id_type == analyses[i].id_type_analyse)
                {
                    strcpy(type_nom, types_analyse[j].nom);
                    printf("Analyse: %s\n", type_nom);
                    printf("Valeurs normales: %s\n", types_analyse[j].valeurs_normales);
                    break;
                }
            }

            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            sprintf(analyses[i].date_realisation, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

            strcpy(analyses[i].statut, "En cours");
            sauvegarderAnalyses();

            printf("\n Analyse commencee avec succes.\n");
            printf("Statut actuel: En cours\n");
            printf("\nVoulez-vous saisir les resultats maintenant? (O/N): ");
            char rep;
            scanf("%c", &rep);
            viderBuffer();

            if (toupper(rep) == 'O')
            {
                saisirResultatAnalyse(id_analyse);
            }
            else
            {
                printf("Vous pourrez saisir les resultats plus tard dans 'Saisir resultats d'analyses'.\n");
                pause();
            }
            return;
        }
    }
    printf("Analyse non trouvee ou deja commencee.\n");
    pause();
}

void saisirResultatAnalyse(int id_analyse)
{
    for (int i = 0; i < nombreAnalyses; i++)
    {
        if (analyses[i].id_analyse == id_analyse && strcmp(analyses[i].statut, "En cours") == 0)
        {
            system("cls");
            printf("\n=== SAISIE DES RESULTATS ===\n\n");

            int idx_patient = rechercherParID(analyses[i].id_patient);
            printf("Patient: %s %s\n", patients[idx_patient].prenom, patients[idx_patient].nom);

            char type_nom[100] = "Inconnu";
            for (int j = 0; j < nombreTypesAnalyse; j++)
            {
                if (types_analyse[j].id_type == analyses[i].id_type_analyse)
                {
                    strcpy(type_nom, types_analyse[j].nom);
                    printf("Analyse: %s\n", type_nom);
                    printf("Valeurs normales: %s\n", types_analyse[j].valeurs_normales);
                    break;
                }
            }

            printf("\n RESULTATS DE L'ANALYSE:\n");
            fgets(analyses[i].resultat, MAX_RESULTAT, stdin);
            analyses[i].resultat[strcspn(analyses[i].resultat, "\n")] = '\0';

            printf("\n INTERPRETATION:\n");
            fgets(analyses[i].interpretation, 300, stdin);
            analyses[i].interpretation[strcspn(analyses[i].interpretation, "\n")] = '\0';

            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            sprintf(analyses[i].date_resultat, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

            strcpy(analyses[i].statut, "Réalisee");
            sauvegarderAnalyses();

            printf("\n Resultats enregistres avec succes.\n");
            printf("L'analyse est maintenant en attente de validation.\n");
            printf("Une fois validee, le medecin pourra consulter les resultats.\n");
            return;
        }
    }
    printf("Analyse non trouvee ou non en cours.\n");
    pause();
}

void validerAnalyse(int id_analyse)
{
    for (int i = 0; i < nombreAnalyses; i++)
    {
        if (analyses[i].id_analyse == id_analyse && strcmp(analyses[i].statut, "Réalisée") == 0)
        {
            system("cls");
            printf("\n=== VALIDATION DE L'ANALYSE ===\n\n");

            int idx_patient = rechercherParID(analyses[i].id_patient);
            printf("Patient: %s %s\n", patients[idx_patient].prenom, patients[idx_patient].nom);

            char type_nom[100] = "Inconnu";
            for (int j = 0; j < nombreTypesAnalyse; j++)
            {
                if (types_analyse[j].id_type == analyses[i].id_type_analyse)
                {
                    strcpy(type_nom, types_analyse[j].nom);
                    break;
                }
            }
            printf("Analyse: %s\n", type_nom);

            printf("\n--- RESULTATS ---\n");
            printf("%s\n", analyses[i].resultat);

            printf("\n--- INTERPRETATION ---\n");
            printf("%s\n", analyses[i].interpretation);

            printf("\nConfirmez-vous la validation de cette analyse? (O/N): ");
            char conf;
            scanf("%c", &conf);
            viderBuffer();

            if (toupper(conf) == 'O')
            {
                strcpy(analyses[i].statut, "Validée");
                sauvegarderAnalyses();
                printf("\n Analyse validee avec succes.\n");
                printf("Le medecin peut maintenant consulter les resultats.\n");
            }
            else
            {
                printf("Validation annulee.\n");
            }
            pause();
            return;
        }
    }
    printf("Analyse non trouvee ou non realisee.\n");
    pause();
}

void afficherAnalysesEnCours()
{
    system("cls");
    printf("\n=== ANALYSES EN COURS ===\n\n");

    int compteur = 0;
    printf("ID  | Patient                 | Type                | Date realisation\n");
    printf("----|-------------------------|---------------------|------------------\n");

    for (int i = 0; i < nombreAnalyses; i++)
    {
        if (strcmp(analyses[i].statut, "En cours") == 0)
        {
            compteur++;
            int idx_patient = rechercherParID(analyses[i].id_patient);
            char patient_nom[30] = "";
            if (idx_patient != -1)
                sprintf(patient_nom, "%s %s", patients[idx_patient].prenom, patients[idx_patient].nom);
            else
                sprintf(patient_nom, "ID %d", analyses[i].id_patient);

            char type_nom[100] = "Inconnu";
            for (int j = 0; j < nombreTypesAnalyse; j++)
            {
                if (types_analyse[j].id_type == analyses[i].id_type_analyse)
                {
                    strcpy(type_nom, types_analyse[j].nom);
                    break;
                }
            }

            printf("%-4d| %-23s | %-19s | %-12s\n",
                   analyses[i].id_analyse,
                   patient_nom,
                   type_nom,
                   analyses[i].date_realisation);
        }
    }

    if (compteur == 0)
    {
        printf("Aucune analyse en cours.\n");
    }

    pause();
}

void afficherAnalyse(int id_analyse)
{
    for (int i = 0; i < nombreAnalyses; i++)
    {
        if (analyses[i].id_analyse == id_analyse)
        {
            system("cls");
            printf("\n========================================\n");
            printf("         DETAIL DE L'ANALYSE           \n");
            printf("========================================\n");

            int idx_patient = rechercherParID(analyses[i].id_patient);
            printf("Patient: %s %s\n", patients[idx_patient].prenom, patients[idx_patient].nom);

            char type_nom[100] = "Inconnu";
            for (int j = 0; j < nombreTypesAnalyse; j++)
            {
                if (types_analyse[j].id_type == analyses[i].id_type_analyse)
                {
                    strcpy(type_nom, types_analyse[j].nom);
                    break;
                }
            }
            printf("Analyse: %s\n", type_nom);
            printf("Date demande: %s\n", analyses[i].date_demande);
            printf("Date realisation: %s\n", analyses[i].date_realisation);
            printf("Date resultat: %s\n", analyses[i].date_resultat);
            printf("Statut: %s\n", analyses[i].statut);
            printf("Priorite: %s\n", analyses[i].priorite);
            printf("Medecin demandeur ID: %d\n", analyses[i].id_medecin);

            if (strlen(analyses[i].resultat) > 0)
            {
                printf("\n--- RESULTATS ---\n");
                printf("%s\n", analyses[i].resultat);
                printf("\n--- INTERPRETATION ---\n");
                printf("%s\n", analyses[i].interpretation);
            }

            if (strlen(analyses[i].notes) > 0)
            {
                printf("\n--- NOTES ---\n");
                printf("%s\n", analyses[i].notes);
            }
            printf("========================================\n");
            return;
        }
    }
    printf("Analyse non trouvee.\n");
}

// ================= FONCTIONS POUR L'ADMINISTRATEUR =================

void ajouterTypeAnalyse()
{
    system("cls");
    printf("\n=== AJOUT D'UN TYPE D'ANALYSE ===\n\n");

    TypeAnalyse t;
    memset(&t, 0, sizeof(TypeAnalyse));

    t.id_type = ++dernierIDTypeAnalyse;

    printf("Nom: ");
    fgets(t.nom, MAX_TYPE_ANALYSE, stdin);
    t.nom[strcspn(t.nom, "\n")] = '\0';

    printf("Description: ");
    fgets(t.description, 300, stdin);
    t.description[strcspn(t.description, "\n")] = '\0';

    printf("Valeurs normales: ");
    fgets(t.valeurs_normales, 200, stdin);
    t.valeurs_normales[strcspn(t.valeurs_normales, "\n")] = '\0';

    printf("Prix (FCFA): ");
    scanf("%f", &t.prix);
    viderBuffer();

    printf("Delai des resultats (heures): ");
    scanf("%d", &t.duree_resultats);
    viderBuffer();

    types_analyse[nombreTypesAnalyse] = t;
    nombreTypesAnalyse++;

    sauvegarderTypesAnalyse();

    printf("\n Type d'analyse ajoute avec succes! ID: %d\n", t.id_type);
    pause();
}

void afficherTypesAnalyseAdmin()
{
    system("cls");
    printf("\n=== TYPES D'ANALYSE ===\n\n");

    printf("ID  | Nom                     | Prix      | Delai\n");
    printf("----|-------------------------|-----------|--------\n");

    for (int i = 0; i < nombreTypesAnalyse; i++)
    {
        printf("%-4d| %-23s | %9.0f | %dh\n",
               types_analyse[i].id_type,
               types_analyse[i].nom,
               types_analyse[i].prix,
               types_analyse[i].duree_resultats);
    }

    printf("\nTotal: %d type(s) d'analyse\n", nombreTypesAnalyse);
    pause();
}

// ================= STATISTIQUES =================

void statistiquesLaboratoire()
{
    system("cls");
    printf("\n=== STATISTIQUES DU LABORATOIRE ===\n\n");

    int demandees = 0, en_cours = 0, realisees = 0, validees = 0;
    float chiffre_affaires = 0;

    for (int i = 0; i < nombreAnalyses; i++)
    {
        if (strcmp(analyses[i].statut, "Demandee") == 0) demandees++;
        else if (strcmp(analyses[i].statut, "En cours") == 0) en_cours++;
        else if (strcmp(analyses[i].statut, "Réalisee") == 0) realisees++;
        else if (strcmp(analyses[i].statut, "Validee") == 0) validees++;

        if (strcmp(analyses[i].statut, "Validee") == 0)
        {
            for (int j = 0; j < nombreTypesAnalyse; j++)
            {
                if (types_analyse[j].id_type == analyses[i].id_type_analyse)
                {
                    chiffre_affaires += types_analyse[j].prix;
                    break;
                }
            }
        }
    }

    printf("=== PAR STATUT ===\n");
    printf("  Demandees: %d\n", demandees);
    printf("  En cours: %d\n", en_cours);
    printf("  Realisees: %d\n", realisees);
    printf("  Validees: %d\n", validees);

    printf("\n=== FINANCIER ===\n");
    printf("  Chiffre d'affaires: %.0f FCFA\n", chiffre_affaires);
    printf("  Nombre total d'analyses: %d\n", nombreAnalyses);

    pause();
}

// ================= MENUS SPÉCIFIQUES =================

void menuMedecinLaboratoire()
{
    int choix;
    do
    {
        system("cls");
        color(11, 0);
        printf("\n=== LABORATOIRE (MEDECIN) ===\n\n");
        color(7, 0);

        printf("1. Demander une analyse\n");
        printf("2. Consulter mes demandes\n");
        printf("3. Consulter resultats d'un patient\n");
        printf("4. Retour\n");
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
                    afficherListePatientsPourMedecin(utilisateur_actuel->id_associe);
                    printf("\nID du patient: ");
                    int id;
                    scanf("%d", &id);
                    viderBuffer();
                    if (rechercherParID(id) != -1 && medecinEstLieAuPatient(utilisateur_actuel->id_associe, id))
                    {
                        demanderAnalyse(id, utilisateur_actuel->id_associe);
                    }
                    else
                    {
                        printf("Patient non trouve ou non associe a vous.\n");
                        pause();
                    }
                }
                break;
            case 2:
                afficherDemandesParMedecin(utilisateur_actuel->id_associe);
                break;
            case 3:
                {
                    if (nombrePatients == 0)
                    {
                        printf("Aucun patient enregistre.\n");
                        pause();
                        break;
                    }
                    afficherListePatientsPourMedecin(utilisateur_actuel->id_associe);
                    printf("\nID du patient: ");
                    int id;
                    scanf("%d", &id);
                    viderBuffer();
                    if (rechercherParID(id) != -1 && medecinEstLieAuPatient(utilisateur_actuel->id_associe, id))
                    {
                        consulterResultatsPatient(id);
                    }
                    else
                    {
                        printf("Patient non trouve ou non associe a vous.\n");
                        pause();
                    }
                }
                break;
            case 4:
                return;
            default:
                printf("Choix invalide.\n");
                pause();
        }
    } while(choix != 4);
}

void menuTechnicienLaboratoire()
{
    int choix;
    do
    {
        system("cls");
        color(11, 0);
        printf("\n=== LABORATOIRE (TECHNICIEN) ===\n\n");
        color(7, 0);

        printf("1. Analyses a realiser\n");
        printf("2. Saisir resultats d'analyses\n");
        printf("3. Valider une analyse\n");
        printf("4. Consulter une analyse\n");
        printf("5. Statistiques\n");
        printf("6. Retour\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                afficherAnalysesARealiser();
                break;
            case 2:
                {
                    afficherAnalysesEnCours();
                    printf("\nID de l'analyse: ");
                    int id;
                    scanf("%d", &id);
                    viderBuffer();
                    saisirResultatAnalyse(id);
                }
                break;
            case 3:
                {
                    printf("ID de l'analyse a valider: ");
                    int id;
                    scanf("%d", &id);
                    viderBuffer();
                    validerAnalyse(id);
                }
                break;
            case 4:
                {
                    printf("ID de l'analyse: ");
                    int id;
                    scanf("%d", &id);
                    viderBuffer();
                    afficherAnalyse(id);
                    pause();
                }
                break;
            case 5:
                statistiquesLaboratoire();
                break;
            case 6:
                return;
            default:
                printf("Choix invalide.\n");
                pause();
        }
    } while(choix != 6);
}

void menuAdminLaboratoire()
{
    int choix;
    do
    {
        system("cls");
        color(11, 0);
        printf("\n=== LABORATOIRE (ADMINISTRATEUR) ===\n\n");
        color(7, 0);

        printf("1. Ajouter un type d'analyse\n");
        printf("2. Lister les types d'analyse\n");
        printf("3. Statistiques\n");
        printf("4. Retour\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                ajouterTypeAnalyse();
                break;
            case 2:
                afficherTypesAnalyseAdmin();
                break;
            case 3:
                statistiquesLaboratoire();
                break;
            case 4:
                return;
            default:
                printf("Choix invalide.\n");
                pause();
        }
    } while(choix != 4);
}

void menuPatientLaboratoire()
{
    int choix;
    do
    {
        system("cls");
        color(11, 0);
        printf("\n=== MES RESULTATS D'ANALYSE ===\n\n");
        color(7, 0);

        printf("1. Consulter mes resultats\n");
        printf("2. Retour\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                consulterResultatsPatient(utilisateur_actuel->id_associe);
                break;
            case 2:
                return;
            default:
                printf("Choix invalide.\n");
                pause();
        }
    } while(choix != 2);
}

// ================= SAUVEGARDE =================

void sauvegarderTypesAnalyse()
{
    FILE *f = fopen("types_analyse.dat", "wb");
    if (f == NULL) return;

    fwrite(&nombreTypesAnalyse, sizeof(int), 1, f);
    fwrite(types_analyse, sizeof(TypeAnalyse), nombreTypesAnalyse, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_type_analyse.dat", "wb");
    if (fid != NULL)
    {
        fwrite(&dernierIDTypeAnalyse, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void chargerTypesAnalyse()
{
    FILE *f = fopen("types_analyse.dat", "rb");
    if (f == NULL) return;

    fread(&nombreTypesAnalyse, sizeof(int), 1, f);
    fread(types_analyse, sizeof(TypeAnalyse), nombreTypesAnalyse, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_type_analyse.dat", "rb");
    if (fid != NULL)
    {
        fread(&dernierIDTypeAnalyse, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void sauvegarderAnalyses()
{
    FILE *f = fopen("analyses.dat", "wb");
    if (f == NULL) return;

    fwrite(&nombreAnalyses, sizeof(int), 1, f);
    fwrite(analyses, sizeof(Analyse), nombreAnalyses, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_analyse.dat", "wb");
    if (fid != NULL)
    {
        fwrite(&dernierIDAnalyse, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void chargerAnalyses()
{
    FILE *f = fopen("analyses.dat", "rb");
    if (f == NULL) return;

    fread(&nombreAnalyses, sizeof(int), 1, f);
    fread(analyses, sizeof(Analyse), nombreAnalyses, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_analyse.dat", "rb");
    if (fid != NULL)
    {
        fread(&dernierIDAnalyse, sizeof(int), 1, fid);
        fclose(fid);
    }
}
