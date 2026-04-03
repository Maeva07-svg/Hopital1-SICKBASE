#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "archives.h"
#include "fichiers_relations.h"
#include "utils.h"
#include "patient.h"
#include "consultations.h"
#include "ordonnances.h"
#include "interventions.h"
#include "comptes.h"

Archive archives[MAX_ARCHIVES];
int nombreArchives = 0;
int dernierIDArchive = 0;

void initialiserArchives()
{
    // Rien à initialiser
}

void archiverPatient(int id_patient)
{
    int index = rechercherParID(id_patient);
    if (index == -1)
    {
        printf("Patient non trouve.\n");
        return;
    }

    Archive a;
    memset(&a, 0, sizeof(Archive));

    a.id_archive = ++dernierIDArchive;
    strcpy(a.type_archive, "Patient");
    a.id_element = id_patient;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    // Utilisation de snprintf pour plus de sécurité
    snprintf(a.date_archivage, sizeof(a.date_archivage), "%02d/%02d/%04d",
             tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    strcpy(a.raison, "Archivage patient");

    if (utilisateur_actuel != NULL)
    {
        strcpy(a.responsable_archivage, utilisateur_actuel->login);
    }
    else
    {
        strcpy(a.responsable_archivage, "Systeme");
    }

    a.retention_ans = 10;

    char resume[5000] = "";
    snprintf(resume, sizeof(resume),
             "Patient: %s %s\nID: %d\nDate naissance: %s\nTelephone: %s\nDiagnostic: %s\n",
             patients[index].prenom, patients[index].nom,
             patients[index].id,
             patients[index].date_naissance,
             patients[index].telephone,
             patients[index].diagnostic);

    strcpy(a.donnees_sauvegardees, resume);
    strcpy(a.statut, "Archivé");

    archives[nombreArchives] = a;
    nombreArchives++;

    sauvegarderArchives();

    printf("Patient archive avec succes. ID Archive: %d\n", a.id_archive);
}

void archiverConsultation(int id_consultation)
{
    int index = -1;
    for (int i = 0; i < nombreConsultations; i++)
    {
        if (consultations[i].id_consultation == id_consultation)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("Consultation non trouvee.\n");
        return;
    }

    Archive a;
    memset(&a, 0, sizeof(Archive));

    a.id_archive = ++dernierIDArchive;
    strcpy(a.type_archive, "Consultation");
    a.id_element = id_consultation;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(a.date_archivage, sizeof(a.date_archivage), "%02d/%02d/%04d",
             tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    strcpy(a.raison, "Archivage consultation");

    if (utilisateur_actuel != NULL)
    {
        strcpy(a.responsable_archivage, utilisateur_actuel->login);
    }
    else
    {
        strcpy(a.responsable_archivage, "Systeme");
    }

    a.retention_ans = 10;

    char resume[5000] = "";
    snprintf(resume, sizeof(resume),
             "Consultation ID: %d\nPatient ID: %d\nMedecin ID: %d\nDate: %s\nMotif: %s\nDiagnostic: %s\n",
             consultations[index].id_consultation,
             consultations[index].id_patient,
             consultations[index].id_medecin,
             consultations[index].date_consultation,
             consultations[index].motif,
             consultations[index].diagnostic);

    strcpy(a.donnees_sauvegardees, resume);
    strcpy(a.statut, "Archivé");

    archives[nombreArchives] = a;
    nombreArchives++;

    sauvegarderArchives();

    printf("Consultation archivee avec succes. ID Archive: %d\n", a.id_archive);
}

void archiverOrdonnance(int id_ordonnance)
{
    int index = -1;
    for (int i = 0; i < nombreOrdonnances; i++)
    {
        if (ordonnances[i].id_ordonnance == id_ordonnance)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("Ordonnance non trouvee.\n");
        return;
    }

    Archive a;
    memset(&a, 0, sizeof(Archive));

    a.id_archive = ++dernierIDArchive;
    strcpy(a.type_archive, "Ordonnance");
    a.id_element = id_ordonnance;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(a.date_archivage, sizeof(a.date_archivage), "%02d/%02d/%04d",
             tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    strcpy(a.raison, "Archivage ordonnance");

    if (utilisateur_actuel != NULL)
    {
        strcpy(a.responsable_archivage, utilisateur_actuel->login);
    }
    else
    {
        strcpy(a.responsable_archivage, "Systeme");
    }

    a.retention_ans = 10;

    char resume[5000] = "";
    snprintf(resume, sizeof(resume),
             "Ordonnance ID: %d\nPatient ID: %d\nMedecin ID: %d\nDate: %s\nDiagnostic: %s\nNombre medicaments: %d\n",
             ordonnances[index].id_ordonnance,
             ordonnances[index].id_patient,
             ordonnances[index].id_medecin,
             ordonnances[index].date_prescription,
             ordonnances[index].diagnostic,
             ordonnances[index].nb_medicaments);

    strcpy(a.donnees_sauvegardees, resume);
    strcpy(a.statut, "Archivé");

    archives[nombreArchives] = a;
    nombreArchives++;

    sauvegarderArchives();

    printf("Ordonnance archivee avec succes. ID Archive: %d\n", a.id_archive);
}

void archiverIntervention(int id_intervention)
{
    int index = -1;
    for (int i = 0; i < nombreInterventions; i++)
    {
        if (interventions[i].id_intervention == id_intervention)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("Intervention non trouvee.\n");
        return;
    }

    Archive a;
    memset(&a, 0, sizeof(Archive));

    a.id_archive = ++dernierIDArchive;
    strcpy(a.type_archive, "Intervention");
    a.id_element = id_intervention;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(a.date_archivage, sizeof(a.date_archivage), "%02d/%02d/%04d",
             tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    strcpy(a.raison, "Archivage intervention");

    if (utilisateur_actuel != NULL)
    {
        strcpy(a.responsable_archivage, utilisateur_actuel->login);
    }
    else
    {
        strcpy(a.responsable_archivage, "Systeme");
    }

    a.retention_ans = 10;

    char resume[5000] = "";
    snprintf(resume, sizeof(resume),
             "Intervention ID: %d\nPatient ID: %d\nChirurgien ID: %d\nDate: %s\nType: %s\nStatut: %s\n",
             interventions[index].id_intervention,
             interventions[index].id_patient,
             interventions[index].id_chirurgien,
             interventions[index].date_intervention,
             interventions[index].type_intervention,
             interventions[index].statut);

    strcpy(a.donnees_sauvegardees, resume);
    strcpy(a.statut, "Archivé");

    archives[nombreArchives] = a;
    nombreArchives++;

    sauvegarderArchives();

    printf("Intervention archivee avec succes. ID Archive: %d\n", a.id_archive);
}

void archiverDonneesAnciennes(int jours_anciennete)
{
    printf("Fonction d'archivage automatique en developpement.\n");
}

void rechercherDansArchives(char *mot_cle)
{
    system("cls");
    printf("\n=== RECHERCHE DANS LES ARCHIVES ===\n");
    printf("Recherche: %s\n\n", mot_cle);

    int compteur = 0;
    for (int i = 0; i < nombreArchives; i++)
    {
        if (strstr(archives[i].donnees_sauvegardees, mot_cle) != NULL ||
            strstr(archives[i].type_archive, mot_cle) != NULL)
        {
            compteur++;
            printf("\n[%d] Archive #%d - %s\n", compteur, archives[i].id_archive, archives[i].type_archive);
            printf("    Date archivage: %s\n", archives[i].date_archivage);
            printf("    Responsable: %s\n", archives[i].responsable_archivage);
            printf("    Raison: %s\n", archives[i].raison);
            printf("    Resume: %.200s\n", archives[i].donnees_sauvegardees);
        }
    }

    if (compteur == 0)
    {
        printf("Aucune archive correspondante.\n");
    }

    pause();
}

void restaurerArchive(int index)
{
    if (index < 0 || index >= nombreArchives)
    {
        printf("Archive non trouvee.\n");
        return;
    }

    printf("\n=== RESTAURATION D'ARCHIVE ===\n");
    printf("Archive #%d - %s\n", archives[index].id_archive, archives[index].type_archive);
    printf("Confirmer la restauration? (O/N): ");

    char confirmation;
    scanf("%c", &confirmation);
    viderBuffer();

    if (toupper(confirmation) == 'O')
    {
        strcpy(archives[index].statut, "Restauré");
        sauvegarderArchives();
        printf("Archive restauree avec succes.\n");
    }
    else
    {
        printf("Restauration annulee.\n");
    }

    pause();
}

void purgerArchives(int jours_conservation)
{
    printf("\n=== PURGE DES ARCHIVES ===\n");
    printf("Suppression des archives de plus de %d jours...\n", jours_conservation);
    printf("Fonction de purge en developpement.\n");
    pause();
}

void exporterArchiveTexte(int index)
{
    if (index < 0 || index >= nombreArchives)
    {
        printf("Archive non trouvee.\n");
        return;
    }

    char nom_fichier[100];
    snprintf(nom_fichier, sizeof(nom_fichier), "archive_%d.txt", archives[index].id_archive);

    FILE *f = fopen(nom_fichier, "w");
    if (f == NULL)
    {
        printf("Erreur: Impossible de creer le fichier.\n");
        return;
    }

    fprintf(f, "========================================\n");
    fprintf(f, "ARCHIVE #%d\n", archives[index].id_archive);
    fprintf(f, "========================================\n");
    fprintf(f, "Type: %s\n", archives[index].type_archive);
    fprintf(f, "ID Element: %d\n", archives[index].id_element);
    fprintf(f, "Date archivage: %s\n", archives[index].date_archivage);
    fprintf(f, "Responsable: %s\n", archives[index].responsable_archivage);
    fprintf(f, "Raison: %s\n", archives[index].raison);
    fprintf(f, "Conservation: %d ans\n", archives[index].retention_ans);
    fprintf(f, "Statut: %s\n", archives[index].statut);
    fprintf(f, "\n--- DONNEES ---\n");
    fprintf(f, "%s\n", archives[index].donnees_sauvegardees);
    fprintf(f, "========================================\n");

    fclose(f);

    printf("Archive exportee vers %s\n", nom_fichier);
    pause();
}

void menuArchives()
{
    int choix;

    do
    {
        system("cls");
        color(14, 0);
        printf("\n=== GESTION DES ARCHIVES ===\n\n");
        color(7, 0);

        printf("1. Archiver un patient\n");
        printf("2. Archiver une consultation\n");
        printf("3. Archiver une ordonnance\n");
        printf("4. Archiver une intervention\n");
        printf("5. Lister toutes les archives\n");
        printf("6. Rechercher dans les archives\n");
        printf("7. Restaurer une archive\n");
        printf("8. Exporter une archive\n");
        printf("9. Statistiques des archives\n");
        printf("10. Retour\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                {
                    if (nombrePatients == 0)
                    {
                        printf("Aucun patient a archiver.\n");
                        pause();
                        break;
                    }
                    afficherListePatientsSimple();
                    printf("\nID Patient a archiver: ");
                    int id;
                    scanf("%d", &id);
                    viderBuffer();
                    archiverPatient(id);
                    pause();
                }
                break;

            case 2:
                {
                    if (nombreConsultations == 0)
                    {
                        printf("Aucune consultation a archiver.\n");
                        pause();
                        break;
                    }
                    printf("ID Consultation a archiver: ");
                    int id;
                    scanf("%d", &id);
                    viderBuffer();
                    archiverConsultation(id);
                    pause();
                }
                break;

            case 3:
                {
                    if (nombreOrdonnances == 0)
                    {
                        printf("Aucune ordonnance a archiver.\n");
                        pause();
                        break;
                    }
                    printf("ID Ordonnance a archiver: ");
                    int id;
                    scanf("%d", &id);
                    viderBuffer();
                    archiverOrdonnance(id);
                    pause();
                }
                break;

            case 4:
                {
                    if (nombreInterventions == 0)
                    {
                        printf("Aucune intervention a archiver.\n");
                        pause();
                        break;
                    }
                    printf("ID Intervention a archiver: ");
                    int id;
                    scanf("%d", &id);
                    viderBuffer();
                    archiverIntervention(id);
                    pause();
                }
                break;

            case 5:
                system("cls");
                printf("\n=== LISTE DES ARCHIVES ===\n\n");
                if (nombreArchives == 0)
                {
                    printf("Aucune archive.\n");
                }
                else
                {
                    printf("ID   | Type         | Date archivage | Responsable | Statut\n");
                    printf("-----|--------------|----------------|-------------|--------\n");
                    for (int i = 0; i < nombreArchives; i++)
                    {
                        printf("%-4d | %-12s | %-14s | %-11s | %s\n",
                               archives[i].id_archive,
                               archives[i].type_archive,
                               archives[i].date_archivage,
                               archives[i].responsable_archivage,
                               archives[i].statut);
                    }
                    printf("\nTotal: %d archive(s)\n", nombreArchives);
                }
                pause();
                break;

            case 6:
                {
                    char mot_cle[100];
                    printf("Mot cle a rechercher: ");
                    fgets(mot_cle, 100, stdin);
                    mot_cle[strcspn(mot_cle, "\n")] = '\0';
                    rechercherDansArchives(mot_cle);
                }
                break;

            case 7:
                {
                    if (nombreArchives == 0)
                    {
                        printf("Aucune archive.\n");
                        pause();
                        break;
                    }
                    int id;
                    printf("ID Archive a restaurer: ");
                    scanf("%d", &id);
                    viderBuffer();

                    int index = -1;
                    for (int i = 0; i < nombreArchives; i++)
                    {
                        if (archives[i].id_archive == id)
                        {
                            index = i;
                            break;
                        }
                    }

                    if (index != -1)
                    {
                        restaurerArchive(index);
                    }
                    else
                    {
                        printf("Archive non trouvee.\n");
                        pause();
                    }
                }
                break;

            case 8:
                {
                    if (nombreArchives == 0)
                    {
                        printf("Aucune archive.\n");
                        pause();
                        break;
                    }
                    int id;
                    printf("ID Archive a exporter: ");
                    scanf("%d", &id);
                    viderBuffer();

                    int index = -1;
                    for (int i = 0; i < nombreArchives; i++)
                    {
                        if (archives[i].id_archive == id)
                        {
                            index = i;
                            break;
                        }
                    }

                    if (index != -1)
                    {
                        exporterArchiveTexte(index);
                    }
                    else
                    {
                        printf("Archive non trouvee.\n");
                        pause();
                    }
                }
                break;

            case 9:
                system("cls");
                printf("\n=== STATISTIQUES DES ARCHIVES ===\n\n");
                printf("Nombre total d'archives: %d\n", nombreArchives);
                printf("Capacite restante: %d/%d\n", MAX_ARCHIVES - nombreArchives, MAX_ARCHIVES);

                int patients_arch = 0, consultations_arch = 0, ordonnances_arch = 0, interventions_arch = 0;
                for (int i = 0; i < nombreArchives; i++)
                {
                    if (strcmp(archives[i].type_archive, "Patient") == 0) patients_arch++;
                    else if (strcmp(archives[i].type_archive, "Consultation") == 0) consultations_arch++;
                    else if (strcmp(archives[i].type_archive, "Ordonnance") == 0) ordonnances_arch++;
                    else if (strcmp(archives[i].type_archive, "Intervention") == 0) interventions_arch++;
                }

                printf("\nRepartition par type:\n");
                printf("  Patients: %d\n", patients_arch);
                printf("  Consultations: %d\n", consultations_arch);
                printf("  Ordonnances: %d\n", ordonnances_arch);
                printf("  Interventions: %d\n", interventions_arch);

                pause();
                break;

            case 10:
                return;

            default:
                printf("Choix invalide.\n");
                pause();
        }

    } while(choix != 10);
}
