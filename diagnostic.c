#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "diagnostic.h"
#include "patient.h"
#include "fichiers.h"
#include "utils.h"

Maladie maladies[MAX_MALADIES];
int nombreMaladies = 8;

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

void diagnosticParSymptomes()
{
    system("cls");
    printf("\n=== DIAGNOSTIC PAR SYMPTOMES ===\n\n");

    char symptomes_entres[MAX_SYMPTOMS][MAX_SYMPTOM_LENGTH];
    int nb_symptomes_entres = 0;

    printf("Entrez vos symptomes (un par ligne, 'fin' pour terminer):\n");
    printf("Exemples: fievre, toux, maux de tete, nausees, etc.\n\n");

    while (nb_symptomes_entres < MAX_SYMPTOMS)
    {
        printf("Symptome %d: ", nb_symptomes_entres + 1);
        fgets(symptomes_entres[nb_symptomes_entres], MAX_SYMPTOM_LENGTH, stdin);
        symptomes_entres[nb_symptomes_entres][strcspn(symptomes_entres[nb_symptomes_entres], "\n")] = '\0';

        if (strcmp(symptomes_entres[nb_symptomes_entres], "fin") == 0)
        {
            break;
        }

        if (strlen(symptomes_entres[nb_symptomes_entres]) > 0)
        {
            nb_symptomes_entres++;
        }
    }

    if (nb_symptomes_entres == 0)
    {
        printf("\nAucun symptome entre.\n");
        pause();
        return;
    }

    printf("\n=== ANALYSE EN COURS ===\n");
    printf("\nSymptomes entres (%d):\n", nb_symptomes_entres);
    for (int i = 0; i < nb_symptomes_entres; i++)
    {
        printf("  - %s\n", symptomes_entres[i]);
    }

    printf("\n=== RESULTATS DU DIAGNOSTIC ===\n\n");

    int trouve_diagnostic = 0;
    float meilleur_score = 0.0;
    int meilleure_maladie = -1;

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
            trouve_diagnostic = 1;
            printf("\n-> %s (%.0f%% de correspondance)\n", maladies[m].nom, score);
            printf("   Symptomes correspondants: %d/%d\n", correspondances, maladies[m].nb_symptomes);
            printf("   Description: %s\n", maladies[m].description);
            printf("   Traitement recommande: %s\n", maladies[m].traitement);
            printf("   Precautions: %s\n", maladies[m].precautions);

            if (score > meilleur_score)
            {
                meilleur_score = score;
                meilleure_maladie = m;
            }
        }
    }

    if (!trouve_diagnostic)
    {
        printf("\nAucune maladie ne correspond suffisamment aux symptomes entres.\n");
        printf("Consultez un medecin pour un diagnostic precis.\n");
    }
    else if (meilleure_maladie != -1 && meilleur_score >= 70.0)
    {
        printf("\n=== DIAGNOSTIC PROBABLE ===\n");
        printf("Maladie la plus probable: %s (%.0f%%)\n", maladies[meilleure_maladie].nom, meilleur_score);
        printf("Il est fortement recommande de consulter un medecin.\n");
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

void menuDiagnosticMedical()
{
    int choix;

    do
    {
        system("cls");
        color(11, 0);
        printf("\n=== SYSTEME DE DIAGNOSTIC MEDICAL ===\n\n");
        color(7, 0);

        printf("1. Diagnostic par symptomes (nouveau patient)\n");
        printf("2. Diagnostic d'un patient existant\n");
        printf("3. Voir la base de donnees des maladies\n");
        printf("4. Retour au menu principal\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
        case 1:
            diagnosticParSymptomes();
            break;
        case 2:
            if (nombrePatients == 0)
            {
                printf("\nAucun patient enregistre.\n");
                printf("Ajoutez d'abord un patient.\n");
                pause();
            }
            else
            {
                afficherListePatientsSimple();
                printf("\nEntrez l'ID du patient pour diagnostic: ");
                int id;
                scanf("%d", &id);
                viderBuffer();

                int index = rechercherParID(id);
                if (index != -1)
                {
                    diagnosticPatientExistant(index);
                }
                else
                {
                    printf("Patient non trouve.\n");
                    pause();
                }
            }
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
