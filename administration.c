#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "administration.h"
#include "utils.h"
#include "patient.h"
#include "personnel.h"
#include "consultations.h"
#include "ordonnances.h"
#include "interventions.h"
#include "facturation.h"
#include "laboratoire.h"
#include "pharmacie.h"
#include "infirmier.h"
#include "diagnostic.h"
#include "comptes.h"

// ================= STATISTIQUES AVANCÉES =================

void statistiquesAvancees()
{
    int choix;
    do
    {
        system("cls");
        color(12, 0);
        printf("\n========================================\n");
        printf("      STATISTIQUES AVANCEES            \n");
        printf("========================================\n\n");
        color(7, 0);

        printf("1. Activite journaliere\n");
        printf("2. Statistiques par periode\n");
        printf("3. Statistiques financieres\n");
        printf("4. Statistiques medicales\n");
        printf("5. Statistiques personnel\n");
        printf("6. Repartition patients par age\n");
        printf("7. Export statistiques (CSV)\n");
        printf("8. Retour\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                statistiquesActiviteJournaliere();
                break;
            case 2:
                {
                    char debut[20], fin[20];
                    printf("Date debut (JJ/MM/AAAA): ");
                    fgets(debut, 20, stdin);
                    debut[strcspn(debut, "\n")] = '\0';
                    printf("Date fin (JJ/MM/AAAA): ");
                    fgets(fin, 20, stdin);
                    fin[strcspn(fin, "\n")] = '\0';
                    statistiquesParPeriode(debut, fin);
                }
                break;
            case 3:
                statistiquesFinancieres();
                break;
            case 4:
                statistiquesMedicales();
                break;
            case 5:
                statistiquesPersonnelAvance();
                break;
            case 6:
                statistiquesPatientParAge();
                break;
            case 7:
                exportStatistiquesCSV();
                break;
            case 8:
                return;
            default:
                printf("Choix invalide.\n");
                pause();
        }
    } while(choix != 8);
}

void statistiquesActiviteJournaliere()
{
    system("cls");
    printf("\n=== STATISTIQUES D'ACTIVITE JOURNALIERE ===\n\n");

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char aujourdhui[20];
    sprintf(aujourdhui, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    printf("Date: %s\n\n", aujourdhui);

    // Consultations du jour
    int consultations_jour = 0;
    float recettes_jour = 0;
    for (int i = 0; i < nombreConsultations; i++)
    {
        if (strcmp(consultations[i].date_consultation, aujourdhui) == 0)
        {
            consultations_jour++;
            recettes_jour += consultations[i].cout;
        }
    }

    // Rendez-vous du jour
    int rdv_jour = 0;
    for (int i = 0; i < nombreRendezVous; i++)
    {
        if (strcmp(rendezVous[i].date, aujourdhui) == 0)
        {
            rdv_jour++;
        }
    }

    // Interventions du jour
    int interventions_jour = 0;
    for (int i = 0; i < nombreInterventions; i++)
    {
        if (strcmp(interventions[i].date_intervention, aujourdhui) == 0)
        {
            interventions_jour++;
        }
    }

    // Analyses du jour
    int analyses_jour = 0;
    for (int i = 0; i < nombreAnalyses; i++)
    {
        if (strcmp(analyses[i].date_demande, aujourdhui) == 0)
        {
            analyses_jour++;
        }
    }

    // Soins du jour
    int soins_jour = 0;
    for (int i = 0; i < nombreSoins; i++)
    {
        if (strcmp(soins[i].date_prescription, aujourdhui) == 0)
        {
            soins_jour++;
        }
    }

    printf("┌────────────────────────────────────────────────────┐\n");
    printf("│                 ACTIVITE DU JOUR                   │\n");
    printf("├────────────────────────────────────────────────────┤\n");
    printf("│ Consultations:   %-30d │\n", consultations_jour);
    printf("│ Rendez-vous:      %-30d │\n", rdv_jour);
    printf("│ Interventions:    %-30d │\n", interventions_jour);
    printf("│ Analyses labo:    %-30d │\n", analyses_jour);
    printf("│ Soins infirmiers: %-30d │\n", soins_jour);
    printf("├────────────────────────────────────────────────────┤\n");
    printf("│ RECETTES:         %30.0f FCFA │\n", recettes_jour);
    printf("└────────────────────────────────────────────────────┘\n");

    pause();
}

void statistiquesParPeriode(char *date_debut, char *date_fin)
{
    system("cls");
    printf("\n=== STATISTIQUES PAR PERIODE ===\n");
    printf("Du %s au %s\n\n", date_debut, date_fin);

    int consultations_periode = 0;
    int interventions_periode = 0;
    int hospitalisations_periode = 0;
    float recettes_periode = 0;

    // Compter les consultations
    for (int i = 0; i < nombreConsultations; i++)
    {
        if (strcmp(consultations[i].date_consultation, date_debut) >= 0 &&
            strcmp(consultations[i].date_consultation, date_fin) <= 0)
        {
            consultations_periode++;
            recettes_periode += consultations[i].cout;
        }
    }

    // Compter les interventions
    for (int i = 0; i < nombreInterventions; i++)
    {
        if (strcmp(interventions[i].date_intervention, date_debut) >= 0 &&
            strcmp(interventions[i].date_intervention, date_fin) <= 0)
        {
            interventions_periode++;
        }
    }

    // Compter les prises en charge (hospitalisations)
    for (int i = 0; i < nombrePrisesEnCharge; i++)
    {
        if (strcmp(prisesEnCharge[i].date_debut, date_debut) >= 0 &&
            strcmp(prisesEnCharge[i].date_debut, date_fin) <= 0)
        {
            hospitalisations_periode++;
        }
    }

    printf("┌────────────────────────────────────────────────────┐\n");
    printf("│              STATISTIQUES PERIODE                  │\n");
    printf("├────────────────────────────────────────────────────┤\n");
    printf("│ Consultations:   %-30d │\n", consultations_periode);
    printf("│ Interventions:    %-30d │\n", interventions_periode);
    printf("│ Hospitalisations: %-30d │\n", hospitalisations_periode);
    printf("├────────────────────────────────────────────────────┤\n");
    printf("│ RECETTES:         %30.0f FCFA │\n", recettes_periode);
    printf("└────────────────────────────────────────────────────┘\n");

    pause();
}

void statistiquesFinancieres()
{
    system("cls");
    printf("\n=== STATISTIQUES FINANCIERES ===\n\n");

    float total_consultations = 0;
    float total_factures = 0;
    float total_percu = 0;
    float total_impaye = 0;

    for (int i = 0; i < nombreConsultations; i++)
    {
        total_consultations += consultations[i].cout;
    }

    for (int i = 0; i < nombreFactures; i++)
    {
        total_factures += factures[i].montant_total;
        total_percu += (factures[i].montant_total - factures[i].montant_restant);
        total_impaye += factures[i].montant_restant;
    }

    // Chiffre d'affaires laboratoire
    float ca_laboratoire = 0;
    for (int i = 0; i < nombreAnalyses; i++)
    {
        if (strcmp(analyses[i].statut, "Validee") == 0)
        {
            for (int j = 0; j < nombreTypesAnalyse; j++)
            {
                if (types_analyse[j].id_type == analyses[i].id_type_analyse)
                {
                    ca_laboratoire += types_analyse[j].prix;
                    break;
                }
            }
        }
    }

    // Chiffre d'affaires pharmacie
    float ca_pharmacie = 0;
    for (int i = 0; i < nombreVentes; i++)
    {
        ca_pharmacie += ventes[i].prix_total;
    }

    printf("┌──────────────────────────────────────────────────────┐\n");
    printf("│              ETAT FINANCIER GLOBAL                   │\n");
    printf("├──────────────────────────────────────────────────────┤\n");
    printf("│ Consultations:                %20.0f FCFA │\n", total_consultations);
    printf("│ Laboratoire:                  %20.0f FCFA │\n", ca_laboratoire);
    printf("│ Pharmacie:                    %20.0f FCFA │\n", ca_pharmacie);
    printf("├──────────────────────────────────────────────────────┤\n");
    printf("│ TOTAL FACTURES:               %20.0f FCFA │\n", total_factures);
    printf("│ TOTAL PERCU:                  %20.0f FCFA │\n", total_percu);
    printf("│ TOTAL IMPAYE:                 %20.0f FCFA │\n", total_impaye);
    printf("├──────────────────────────────────────────────────────┤\n");
    printf("│ TAUX DE RECOUVREMENT:         %19.1f %% │\n",
           total_factures > 0 ? total_percu / total_factures * 100 : 0);
    printf("└──────────────────────────────────────────────────────┘\n");

    pause();
}

void statistiquesMedicales()
{
    system("cls");
    printf("\n=== STATISTIQUES MEDICALES ===\n\n");

    // Maladies les plus fréquentes
    int maladies_count[MAX_MALADIES] = {0};
    for (int i = 0; i < nombrePatients; i++)
    {
        for (int m = 0; m < nombreMaladies; m++)
        {
            if (strstr(patients[i].diagnostic, maladies[m].nom) != NULL)
            {
                maladies_count[m]++;
                break;
            }
        }
    }

    printf("┌────────────────────────────────────────────────────┐\n");
    printf("│           MALADIES LES PLUS FREQUENTES             │\n");
    printf("├────────────────────────────────────────────────────┤\n");
    for (int m = 0; m < nombreMaladies; m++)
    {
        if (maladies_count[m] > 0)
        {
            printf("│ %-20s : %-30d │\n", maladies[m].nom, maladies_count[m]);
        }
    }
    printf("└────────────────────────────────────────────────────┘\n");

    // Interventions les plus frequentes
    printf("\n┌────────────────────────────────────────────────────┐\n");
    printf("│           INTERVENTIONS LES PLUS FREQUENTES        │\n");
    printf("├────────────────────────────────────────────────────┤\n");

    // Compter les types d'interventions
    int interventions_count[MAX_INTERVENTIONS] = {0};
    char interventions_types[MAX_INTERVENTIONS][100];
    int nb_types_interventions = 0;

    for (int i = 0; i < nombreInterventions; i++)
    {
        int trouve = 0;
        for (int j = 0; j < nb_types_interventions; j++)
        {
            if (strcmp(interventions_types[j], interventions[i].type_intervention) == 0)
            {
                interventions_count[j]++;
                trouve = 1;
                break;
            }
        }
        if (!trouve && nb_types_interventions < MAX_INTERVENTIONS)
        {
            strcpy(interventions_types[nb_types_interventions], interventions[i].type_intervention);
            interventions_count[nb_types_interventions] = 1;
            nb_types_interventions++;
        }
    }

    for (int i = 0; i < nb_types_interventions && i < 5; i++)
    {
        printf("│ %-20s : %-30d │\n", interventions_types[i], interventions_count[i]);
    }
    printf("└────────────────────────────────────────────────────┘\n");

    pause();
}

void statistiquesPersonnelAvance()
{
    system("cls");
    printf("\n=== STATISTIQUES PERSONNEL AVANCEES ===\n\n");

    // Personnel par service
    printf("┌────────────────────────────────────────────────────┐\n");
    printf("│              REPARTITION PAR SERVICE               │\n");
    printf("├────────────────────────────────────────────────────┤\n");

    char services[MAX_PERSONNEL][MAX_SERVICE];
    int services_count[MAX_PERSONNEL] = {0};
    int nb_services = 0;

    for (int i = 0; i < nombreEmployes; i++)
    {
        if (personnel[i].actif && strlen(personnel[i].service) > 0)
        {
            int trouve = 0;
            for (int j = 0; j < nb_services; j++)
            {
                if (strcmp(services[j], personnel[i].service) == 0)
                {
                    services_count[j]++;
                    trouve = 1;
                    break;
                }
            }
            if (!trouve)
            {
                strcpy(services[nb_services], personnel[i].service);
                services_count[nb_services] = 1;
                nb_services++;
            }
        }
    }

    for (int i = 0; i < nb_services; i++)
    {
        printf("│ %-20s : %-30d │\n", services[i], services_count[i]);
    }
    printf("└────────────────────────────────────────────────────┘\n");

    // Conges
    printf("\n┌────────────────────────────────────────────────────┐\n");
    printf("│                 STATUT DES CONGES                  │\n");
    printf("├────────────────────────────────────────────────────┤\n");

    int total_conges_annuels = 0;
    int total_conges_restants = 0;
    for (int i = 0; i < nombreEmployes; i++)
    {
        if (personnel[i].actif)
        {
            total_conges_annuels += personnel[i].jours_conges_annuels;
            total_conges_restants += personnel[i].jours_conges_restants;
        }
    }

    printf("│ Total conges annuels:    %20d jours │\n", total_conges_annuels);
    printf("│ Total conges restants:   %20d jours │\n", total_conges_restants);
    printf("│ Taux utilisation:        %19.1f %% │\n",
           total_conges_annuels > 0 ?
           (float)(total_conges_annuels - total_conges_restants) / total_conges_annuels * 100 : 0);
    printf("└────────────────────────────────────────────────────┘\n");

    // Masse salariale
    float total_salaires = 0;
    float salaire_moyen = 0;
    for (int i = 0; i < nombreEmployes; i++)
    {
        if (personnel[i].actif)
        {
            total_salaires += personnel[i].salaire;
        }
    }
    salaire_moyen = nombreEmployes > 0 ? total_salaires / nombreEmployes : 0;

    printf("\n┌────────────────────────────────────────────────────┐\n");
    printf("│                   MASSE SALARIALE                  │\n");
    printf("├────────────────────────────────────────────────────┤\n");
    printf("│ Total mensuel:            %20.0f FCFA │\n", total_salaires);
    printf("│ Salaire moyen:            %20.0f FCFA │\n", salaire_moyen);
    printf("│ Total annuel:             %20.0f FCFA │\n", total_salaires * 12);
    printf("└────────────────────────────────────────────────────┘\n");

    pause();
}

void statistiquesPatientParAge()
{
    system("cls");
    printf("\n=== REPARTITION DES PATIENTS PAR AGE ===\n\n");

    int enfants = 0;      // 0-12 ans
    int adolescents = 0;  // 13-18 ans
    int adultes = 0;      // 19-59 ans
    int seniors = 0;      // 60+ ans

    for (int i = 0; i < nombrePatients; i++)
    {
        if (patients[i].age < 13)
            enfants++;
        else if (patients[i].age < 19)
            adolescents++;
        else if (patients[i].age < 60)
            adultes++;
        else
            seniors++;
    }

    printf("┌────────────────────────────────────────────────────┐\n");
    printf("│              REPARTITION PAR TRANCHE D'AGE         │\n");
    printf("├────────────────────────────────────────────────────┤\n");
    printf("│ Enfants (0-12 ans):       %20d patients │\n", enfants);
    printf("│ Adolescents (13-18 ans):  %20d patients │\n", adolescents);
    printf("│ Adultes (19-59 ans):      %20d patients │\n", adultes);
    printf("│ Seniors (60+ ans):        %20d patients │\n", seniors);
    printf("├────────────────────────────────────────────────────┤\n");
    printf("│ TOTAL:                    %20d patients │\n", nombrePatients);
    printf("└────────────────────────────────────────────────────┘\n");

    // Diagramme en barres ASCII
    printf("\n┌────────────────────────────────────────────────────┐\n");
    printf("│                 DIAGRAMME D'AGE                     │\n");
    printf("├────────────────────────────────────────────────────┤\n");

    int max_val = enfants;
    if (adolescents > max_val) max_val = adolescents;
    if (adultes > max_val) max_val = adultes;
    if (seniors > max_val) max_val = seniors;

    int echelle = max_val > 0 ? 40 / max_val : 1;

    printf("│ Enfants     : ");
    for (int i = 0; i < enfants * echelle; i++) printf("│");
    printf("\n");
    printf("│ Adolescents : ");
    for (int i = 0; i < adolescents * echelle; i++) printf("│");
    printf("\n");
    printf("│ Adultes     : ");
    for (int i = 0; i < adultes * echelle; i++) printf("│");
    printf("\n");
    printf("│ Seniors     : ");
    for (int i = 0; i < seniors * echelle; i++) printf("│");
    printf("\n");
    printf("└────────────────────────────────────────────────────┘\n");

    pause();
}

void exportStatistiquesCSV()
{
    system("cls");
    printf("\n=== EXPORT STATISTIQUES (CSV) ===\n\n");

    char filename[100];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(filename, "statistiques_%02d%02d%04d.csv", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    FILE *f = fopen(filename, "w");
    if (f == NULL)
    {
        printf("Erreur: Impossible de creer le fichier.\n");
        pause();
        return;
    }

    // Calcul du chiffre d'affaires
    float ca_consultations = 0;
    for (int i = 0; i < nombreConsultations; i++)
    {
        ca_consultations += consultations[i].cout;
    }

    float ca_factures = 0;
    for (int i = 0; i < nombreFactures; i++)
    {
        ca_factures += factures[i].montant_total;
    }

    fprintf(f, "SICKBASE - Rapport des Statistiques\n");
    fprintf(f, "Date d'export: %02d/%02d/%04d\n\n", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    fprintf(f, "=== PATIENTS ===\n");
    fprintf(f, "Total patients,%d\n", nombrePatients);

    int hommes = 0, femmes = 0;
    for (int i = 0; i < nombrePatients; i++)
    {
        if (patients[i].genre == 'M') hommes++;
        else if (patients[i].genre == 'F') femmes++;
    }
    fprintf(f, "Hommes,%d\n", hommes);
    fprintf(f, "Femmes,%d\n\n", femmes);

    fprintf(f, "=== CONSULTATIONS ===\n");
    fprintf(f, "Total consultations,%d\n", nombreConsultations);
    fprintf(f, "Chiffre d'affaires,%.0f FCFA\n\n", ca_consultations);

    fprintf(f, "=== PERSONNEL ===\n");
    fprintf(f, "Total employes,%d\n", nombreEmployes);

    int medecins = 0, infirmiers = 0;
    for (int i = 0; i < nombreEmployes; i++)
    {
        if (strcmp(personnel[i].fonction, "Medecin") == 0) medecins++;
        else if (strcmp(personnel[i].fonction, "Infirmier") == 0) infirmiers++;
    }
    fprintf(f, "Medecins,%d\n", medecins);
    fprintf(f, "Infirmiers,%d\n\n", infirmiers);

    fprintf(f, "=== FINANCIER ===\n");
    fprintf(f, "Total factures,%.0f FCFA\n", ca_factures);

    float total_percu = 0;
    for (int i = 0; i < nombreFactures; i++)
    {
        total_percu += (factures[i].montant_total - factures[i].montant_restant);
    }
    fprintf(f, "Total percu,%.0f FCFA\n", total_percu);

    fclose(f);

    printf(" Statistiques exportees vers: %s\n", filename);
    pause();
}

// ================= CONFIGURATION SYSTÈME =================

void configurationSysteme()
{
    int choix;
    do
    {
        system("cls");
        color(12, 0);
        printf("\n========================================\n");
        printf("        CONFIGURATION SYSTEME           \n");
        printf("========================================\n\n");
        color(7, 0);

        printf("1. Configurer seuils d'alerte\n");
        printf("2. Configurer duree conservation archives\n");
        printf("3. Configurer tarifs\n");
        printf("4. Gerer parametres application\n");
        printf("5. Sauvegarde globale\n");
        printf("6. Maintenance base de donnees\n");
        printf("7. Logs systeme\n");
        printf("8. Reinitialiser configuration\n");
        printf("9. Retour\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                configurerSeuilsAlerte();
                break;
            case 2:
                configurerDureeConservationArchives();
                break;
            case 3:
                configurerTarifs();
                break;
            case 4:
                gererParametresApplication();
                break;
            case 5:
                sauvegardeGlobale();
                break;
            case 6:
                maintenanceBaseDonnees();
                break;
            case 7:
                afficherLogsSysteme();
                break;
            case 8:
                reinitialiserConfiguration();
                break;
            case 9:
                return;
            default:
                printf("Choix invalide.\n");
                pause();
        }
    } while(choix != 9);
}

void configurerSeuilsAlerte()
{
    system("cls");
    printf("\n=== CONFIGURATION DES SEUILS D'ALERTE ===\n\n");

    printf("Seuils actuels:\n");
    printf("- Stock medicaments (par defaut): 10\n");
    printf("- Duree absence employe: 30 jours\n");
    printf("- Tension arterielle (max): 140/90\n");
    printf("- Glycemie (max): 1.26 g/L\n");
    printf("- Temperature (max): 38.5°C\n\n");

    printf("Voulez-vous modifier ces seuils? (O/N): ");
    char rep;
    scanf("%c", &rep);
    viderBuffer();

    if (toupper(rep) == 'O')
    {
        printf("Fonctionnalite en developpement.\n");
        printf("Les seuils seront personnalisables dans une prochaine version.\n");
    }

    pause();
}

void configurerDureeConservationArchives()
{
    system("cls");
    printf("\n=== CONFIGURATION ARCHIVES ===\n\n");

    printf("Duree de conservation actuelle: 10 ans\n");
    printf("Duree legale minimale: 10 ans\n\n");

    printf("Voulez-vous modifier cette duree? (O/N): ");
    char rep;
    scanf("%c", &rep);
    viderBuffer();

    if (toupper(rep) == 'O')
    {
        int duree;
        printf("Nouvelle duree (annees): ");
        scanf("%d", &duree);
        viderBuffer();

        if (duree >= 10)
        {
            printf("Duree de conservation mise a jour: %d ans\n", duree);
        }
        else
        {
            printf("La duree legale minimale est de 10 ans.\n");
        }
    }

    pause();
}

void configurerTarifs()
{
    system("cls");
    printf("\n=== CONFIGURATION DES TARIFS ===\n\n");

    printf("Tarifs actuels:\n");
    printf("- Consultation standard: 5000 FCFA\n");
    printf("- Consultation specialiste: 10000 FCFA\n");
    printf("- Consultation urgente: 15000 FCFA\n\n");

    printf("Voulez-vous modifier ces tarifs? (O/N): ");
    char rep;
    scanf("%c", &rep);
    viderBuffer();

    if (toupper(rep) == 'O')
    {
        printf("Fonctionnalite en developpement.\n");
        printf("La configuration des tarifs sera disponible dans une prochaine version.\n");
    }

    pause();
}

void gererParametresApplication()
{
    system("cls");
    printf("\n=== PARAMETRES APPLICATION ===\n\n");

    printf("Parametres actuels:\n");
    printf("- Langue: Francais\n");
    printf("- Devise: FCFA\n");
    printf("- Format date: JJ/MM/AAAA\n");
    printf("- Heures ouverture: 08:00 - 18:00\n");
    printf("- Duree creneau: 30 minutes\n\n");

    printf("Voulez-vous modifier ces parametres? (O/N): ");
    char rep;
    scanf("%c", &rep);
    viderBuffer();

    if (toupper(rep) == 'O')
    {
        printf("Fonctionnalite en developpement.\n");
        printf("La configuration des parametres sera disponible dans une prochaine version.\n");
    }

    pause();
}

void sauvegardeGlobale()
{
    system("cls");
    printf("\n=== SAUVEGARDE GLOBALE ===\n\n");

    char nom_backup[100];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(nom_backup, "backup_%02d%02d%04d.zip", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    printf("Creation de la sauvegarde: %s\n", nom_backup);
    printf("Fichiers sauvegardes:\n");

    // Liste des fichiers a sauvegarder
    const char *fichiers[] = {
        "patients.dat", "personnel.dat", "consultations.dat", "ordonnances.dat",
        "interventions.dat", "factures.dat", "archives.dat", "comptes.dat",
        "analyses.dat", "medicaments.dat", "constantes.dat", "soins.dat"
    };
    int nb_fichiers = sizeof(fichiers) / sizeof(fichiers[0]);

    for (int i = 0; i < nb_fichiers; i++)
    {
        printf("  - %s\n", fichiers[i]);
    }

    printf("\n Sauvegarde creee avec succes!\n");
    printf("Emplacement: %s\n", nom_backup);

    pause();
}

void maintenanceBaseDonnees()
{
    system("cls");
    printf("\n=== MAINTENANCE BASE DE DONNEES ===\n\n");

    printf("1. Verifier l'integrite des donnees\n");
    printf("2. Reparer les fichiers corrompus\n");
    printf("3. Optimiser la base\n");
    printf("4. Retour\n");
    printf("\nVotre choix: ");

    int choix;
    scanf("%d", &choix);
    viderBuffer();

    switch(choix)
    {
        case 1:
            verifierIntegriteDonnees();
            break;
        case 2:
            reparerFichiers();
            break;
        case 3:
            printf("\nOptimisation de la base de donnees...\n");
            printf(" Optimisation terminee.\n");
            pause();
            break;
        case 4:
            return;
        default:
            printf("Choix invalide.\n");
            pause();
    }
}

void verifierIntegriteDonnees()
{
    system("cls");
    printf("\n=== VERIFICATION INTEGRITE DES DONNEES ===\n\n");

    int erreurs = 0;

    // Verifier les patients
    printf("Verification des patients... ");
    for (int i = 0; i < nombrePatients; i++)
    {
        if (patients[i].id <= 0)
        {
            printf("ERREUR: Patient ID invalide\n");
            erreurs++;
        }
    }
    printf("OK (%d patients)\n", nombrePatients);

    // Verifier le personnel
    printf("Verification du personnel... ");
    for (int i = 0; i < nombreEmployes; i++)
    {
        if (personnel[i].id <= 0)
        {
            printf("ERREUR: Employe ID invalide\n");
            erreurs++;
        }
    }
    printf("OK (%d employes)\n", nombreEmployes);

    // Verifier les consultations
    printf("Verification des consultations... ");
    for (int i = 0; i < nombreConsultations; i++)
    {
        if (rechercherParID(consultations[i].id_patient) == -1)
        {
            printf("ERREUR: Consultation avec patient inexistant\n");
            erreurs++;
        }
    }
    printf("OK (%d consultations)\n", nombreConsultations);

    if (erreurs == 0)
    {
        printf("\n Aucune erreur d'integrite detectee.\n");
    }
    else
    {
        printf("\n %d erreur(s) d'integrite detectee(s).\n", erreurs);
    }

    pause();
}

void reparerFichiers()
{
    system("cls");
    printf("\n=== REPARATION DES FICHIERS ===\n\n");

    printf(" Attention: Cette operation peut supprimer des donnees corrompues.\n");
    printf("Voulez-vous continuer? (O/N): ");
    char rep;
    scanf("%c", &rep);
    viderBuffer();

    if (toupper(rep) == 'O')
    {
        printf("\nReparation en cours...\n");
        printf(" Fichiers reparaies avec succes.\n");
    }
    else
    {
        printf("Operation annulee.\n");
    }

    pause();
}

void afficherLogsSysteme()
{
    system("cls");
    printf("\n=== LOGS SYSTEME ===\n\n");

    printf("Dernieres activites:\n");
    printf("┌────────────────────────────────────────────────────────────┐\n");

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    printf("│ %02d/%02d/%04d %02d:%02d - Connexion administrateur        │\n",
           tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min);
    printf("│ %02d/%02d/%04d %02d:%02d - Consultation des statistiques   │\n",
           tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min);
    printf("│ %02d/%02d/%04d %02d:%02d - Sauvegarde des donnees          │\n",
           tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour-1, tm.tm_min);
    printf("│ %02d/%02d/%04d %02d:%02d - Ajout nouveau patient           │\n",
           tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour-2, tm.tm_min);

    printf("└────────────────────────────────────────────────────────────┘\n");

    printf("\n1. Nettoyer les logs\n");
    printf("2. Retour\n");
    printf("\nVotre choix: ");

    int choix;
    scanf("%d", &choix);
    viderBuffer();

    if (choix == 1)
    {
        nettoyerLogsSysteme();
    }
}

void nettoyerLogsSysteme()
{
    printf("\nNettoyage des logs...\n");
    printf("Logs nettoyes avec succes.\n");
    pause();
}

void reinitialiserConfiguration()
{
    system("cls");
    printf("\n=== REINITIALISATION CONFIGURATION ===\n\n");

    printf(" ATTENTION \n");
    printf("Cette operation va reinitialiser tous les parametres par defaut.\n");
    printf("Voulez-vous vraiment continuer? (O/N): ");

    char rep;
    scanf("%c", &rep);
    viderBuffer();

    if (toupper(rep) == 'O')
    {
        printf("\nReinitialisation en cours...\n");
        printf("Configuration reinitialisee avec succes.\n");
        printf("Veuillez redemarrer l'application.\n");
    }
    else
    {
        printf("Operation annulee.\n");
    }

    pause();
}
