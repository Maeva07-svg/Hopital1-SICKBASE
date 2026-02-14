#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "patient.h"
#include "fichiers.h"
#include "diagnostic.h"
#include "utils.h"

// Définition des variables globales
Patient patients[MAX_PATIENTS];
int nombrePatients = 0;
int dernierID = 0;

void ajouterNouveauPatient()
{
    system("cls");
    printf("\n=== AJOUT D'UN NOUVEAU PATIENT ===\n\n");

    if (nombrePatients >= MAX_PATIENTS)
    {
        printf("ERREUR: Capacite maximale atteinte!\n");
        pause();
        return;
    }

    Patient p;
    memset(&p, 0, sizeof(Patient));

    p.id = ++dernierID;

    printf("INFORMATIONS PERSONNELLES\n\n");

    printf("Nom: ");
    fgets(p.nom, MAX_NAME, stdin);
    p.nom[strcspn(p.nom, "\n")] = '\0';

    printf("Prenom: ");
    fgets(p.prenom, MAX_NAME, stdin);
    p.prenom[strcspn(p.prenom, "\n")] = '\0';

    printf("Age: ");
    scanf("%d", &p.age);
    viderBuffer();

    printf("Genre (M/F): ");
    scanf("%c", &p.genre);
    viderBuffer();
    p.genre = toupper(p.genre);

    printf("Date de naissance (JJ/MM/AAAA): ");
    fgets(p.date_naissance, 20, stdin);
    p.date_naissance[strcspn(p.date_naissance, "\n")] = '\0';

    printf("Groupe sanguin (ex: A+, O-, etc.): ");
    fgets(p.groupe_sanguin, 5, stdin);
    p.groupe_sanguin[strcspn(p.groupe_sanguin, "\n")] = '\0';

    printf("Telephone: ");
    fgets(p.telephone, 20, stdin);
    p.telephone[strcspn(p.telephone, "\n")] = '\0';

    printf("Email: ");
    fgets(p.email, 100, stdin);
    p.email[strcspn(p.email, "\n")] = '\0';

    printf("Adresse: ");
    fgets(p.adresse, 200, stdin);
    p.adresse[strcspn(p.adresse, "\n")] = '\0';

    printf("\nINFORMATIONS MEDICALES\n\n");

    printf("Medecin traitant: ");
    fgets(p.medecin_traitant, MAX_NAME, stdin);
    p.medecin_traitant[strcspn(p.medecin_traitant, "\n")] = '\0';

    printf("Date d'admission (JJ/MM/AAAA): ");
    fgets(p.date_admission, 20, stdin);
    p.date_admission[strcspn(p.date_admission, "\n")] = '\0';

    printf("\nSYMPTOMES (Entrez 'fin' pour terminer)\n");
    printf("Exemples: fievre, toux, maux de tete, nausees, etc.\n\n");

    p.nb_symptomes = 0;
    for (int i = 0; i < MAX_SYMPTOMS; i++)
    {
        printf("Symptome %d: ", i + 1);
        char temp[MAX_SYMPTOM_LENGTH];
        fgets(temp, MAX_SYMPTOM_LENGTH, stdin);
        temp[strcspn(temp, "\n")] = '\0';

        if (strcmp(temp, "fin") == 0 || strlen(temp) == 0)
        {
            break;
        }

        strcpy(p.symptomes[i], temp);
        p.nb_symptomes++;
    }

    strcpy(p.diagnostic, "En attente de diagnostic");

    printf("\nNotes medicales (optionnel):\n");
    fgets(p.notes, 500, stdin);
    p.notes[strcspn(p.notes, "\n")] = '\0';

    patients[nombrePatients] = p;
    nombrePatients++;

    sauvegarderPatients();

    printf("\n=== PATIENT AJOUTE AVEC SUCCES ===\n");
    printf("ID Patient: %d\n", p.id);
    printf("Nom: %s %s\n", p.prenom, p.nom);
    printf("Age: %d ans\n", p.age);
    printf("Genre: %c\n", p.genre);
    printf("Symptomes enregistres: %d\n", p.nb_symptomes);

    if (p.nb_symptomes > 0)
    {
        printf("\nVoulez-vous effectuer un diagnostic maintenant? (O/N): ");
        char reponse;
        scanf("%c", &reponse);
        viderBuffer();

        if (toupper(reponse) == 'O')
        {
            diagnosticPatientExistant(nombrePatients - 1);
        }
    }

    pause();
}

void afficherListePatientsSimple()
{
    system("cls");
    printf("\n=== LISTE DES PATIENTS ===\n\n");

    if (nombrePatients == 0)
    {
        printf("Aucun patient enregistre dans le systeme.\n");
        return;
    }

    printf("Nombre total de patients: %d\n\n", nombrePatients);

    printf("ID   | Nom & Prenom           | Age | Genre | Diagnostic\n");
    printf("-----|------------------------|-----|-------|-------------------\n");

    for (int i = 0; i < nombrePatients; i++)
    {
        printf("%-4d | %-10s %-10s | %-3d |   %c   | %s\n",
               patients[i].id,
               patients[i].prenom,
               patients[i].nom,
               patients[i].age,
               patients[i].genre,
               patients[i].diagnostic);
    }
}

void afficherDetailsCompletsPatient(int index)
{
    if (index < 0 || index >= nombrePatients)
    {
        printf("Erreur: Patient non trouve.\n");
        return;
    }

    Patient p = patients[index];

    printf("\n========================================\n");
    printf("FICHE PATIENT COMPLETE - ID: %d\n", p.id);
    printf("========================================\n");

    printf("\nINFORMATIONS PERSONNELLES:\n");
    printf("  Nom: %s\n", p.nom);
    printf("  Prenom: %s\n", p.prenom);
    printf("  Age: %d ans\n", p.age);
    printf("  Genre: %c\n", p.genre);
    printf("  Date de naissance: %s\n", p.date_naissance);
    printf("  Groupe sanguin: %s\n", p.groupe_sanguin);
    printf("  Telephone: %s\n", p.telephone);
    printf("  Email: %s\n", p.email);
    printf("  Adresse: %s\n", p.adresse);

    printf("\nINFORMATIONS MEDICALES:\n");
    printf("  Medecin traitant: %s\n", p.medecin_traitant);
    printf("  Date d'admission: %s\n", p.date_admission);
    printf("  Diagnostic: %s\n", p.diagnostic);

    printf("\nSYMPTOMES (%d):\n", p.nb_symptomes);
    if (p.nb_symptomes == 0)
    {
        printf("  Aucun symptome enregistre\n");
    }
    else
    {
        for (int i = 0; i < p.nb_symptomes; i++)
        {
            printf("  %d. %s\n", i + 1, p.symptomes[i]);
        }
    }

    if (strlen(p.notes) > 0)
    {
        printf("\nNOTES MEDICALES:\n");
        printf("  %s\n", p.notes);
    }

    printf("\n========================================\n");
}

int rechercherParID(int id)
{
    for (int i = 0; i < nombrePatients; i++)
    {
        if (patients[i].id == id)
        {
            return i;
        }
    }
    return -1;
}

void rechercherPatient()
{
    system("cls");
    printf("\n=== RECHERCHE DE PATIENT ===\n\n");

    if (nombrePatients == 0)
    {
        printf("Aucun patient enregistre.\n");
        pause();
        return;
    }

    int id;
    printf("Entrez l'ID du patient: ");
    scanf("%d", &id);
    viderBuffer();

    int index = rechercherParID(id);

    if (index != -1)
    {
        afficherDetailsCompletsPatient(index);

        printf("\nOptions:\n");
        printf("1. Modifier ce patient\n");
        printf("2. Effectuer un diagnostic\n");
        printf("3. Supprimer ce patient\n");
        printf("4. Retour\n");
        printf("Votre choix: ");

        int action;
        scanf("%d", &action);
        viderBuffer();

        if (action == 1)
        {
            modifierPatient(index);
        }
        else if (action == 2)
        {
            diagnosticPatientExistant(index);
        }
        else if (action == 3)
        {
            supprimerPatient(index);
        }
    }
    else
    {
        printf("Patient non trouve.\n");
        pause();
    }
}

void modifierPatient(int index)
{
    if (index < 0 || index >= nombrePatients)
    {
        printf("Erreur: Patient non trouve.\n");
        return;
    }

    Patient *p = &patients[index];
    int choix;
    char temp[500];

    do
    {
        system("cls");
        printf("\n=== MODIFICATION DU PATIENT ===\n");
        printf("Patient: %s %s (ID: %d)\n\n", p->prenom, p->nom, p->id);

        printf("1. Nom (%s)\n", p->nom);
        printf("2. Prenom (%s)\n", p->prenom);
        printf("3. Age (%d ans)\n", p->age);
        printf("4. Genre (%c)\n", p->genre);
        printf("5. Date de naissance (%s)\n", p->date_naissance);
        printf("6. Groupe sanguin (%s)\n", p->groupe_sanguin);
        printf("7. Telephone (%s)\n", p->telephone);
        printf("8. Email (%s)\n", p->email);
        printf("9. Adresse (%s)\n", p->adresse);
        printf("10. Medecin traitant (%s)\n", p->medecin_traitant);
        printf("11. Date d'admission (%s)\n", p->date_admission);
        printf("12. Diagnostic (%s)\n", p->diagnostic);
        printf("13. Symptomes (%d symptomes)\n", p->nb_symptomes);
        printf("14. Notes (%s)\n", strlen(p->notes) > 20 ? "...(tronque)" : p->notes);
        printf("15. Sauvegarder et quitter\n");
        printf("16. Annuler\n");
        printf("\nQuel champ modifier? (1-16): ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                printf("Nouveau nom: ");
                fgets(p->nom, MAX_NAME, stdin);
                p->nom[strcspn(p->nom, "\n")] = '\0';
                break;

            case 2:
                printf("Nouveau prenom: ");
                fgets(p->prenom, MAX_NAME, stdin);
                p->prenom[strcspn(p->prenom, "\n")] = '\0';
                break;

            case 3:
                printf("Nouvel age: ");
                scanf("%d", &p->age);
                viderBuffer();
                break;

            case 4:
                printf("Nouveau genre (M/F): ");
                scanf("%c", &p->genre);
                viderBuffer();
                p->genre = toupper(p->genre);
                break;

            case 5:
                printf("Nouvelle date de naissance (JJ/MM/AAAA): ");
                fgets(p->date_naissance, 20, stdin);
                p->date_naissance[strcspn(p->date_naissance, "\n")] = '\0';
                break;

            case 6:
                printf("Nouveau groupe sanguin: ");
                fgets(p->groupe_sanguin, 5, stdin);
                p->groupe_sanguin[strcspn(p->groupe_sanguin, "\n")] = '\0';
                break;

            case 7:
                printf("Nouveau telephone: ");
                fgets(p->telephone, 20, stdin);
                p->telephone[strcspn(p->telephone, "\n")] = '\0';
                break;

            case 8:
                printf("Nouvel email: ");
                fgets(p->email, 100, stdin);
                p->email[strcspn(p->email, "\n")] = '\0';
                break;

            case 9:
                printf("Nouvelle adresse: ");
                fgets(p->adresse, 200, stdin);
                p->adresse[strcspn(p->adresse, "\n")] = '\0';
                break;

            case 10:
                printf("Nouveau medecin traitant: ");
                fgets(p->medecin_traitant, MAX_NAME, stdin);
                p->medecin_traitant[strcspn(p->medecin_traitant, "\n")] = '\0';
                break;

            case 11:
                printf("Nouvelle date d'admission (JJ/MM/AAAA): ");
                fgets(p->date_admission, 20, stdin);
                p->date_admission[strcspn(p->date_admission, "\n")] = '\0';
                break;

            case 12:
                printf("Nouveau diagnostic: ");
                fgets(p->diagnostic, 200, stdin);
                p->diagnostic[strcspn(p->diagnostic, "\n")] = '\0';
                break;

            case 13:
                printf("\n=== GESTION DES SYMPTOMES ===\n");
                printf("1. Ajouter un symptome\n");
                printf("2. Voir la liste des symptomes\n");
                printf("3. Supprimer tous les symptomes\n");
                printf("Votre choix: ");

                int choixSympt;
                scanf("%d", &choixSympt);
                viderBuffer();

                if (choixSympt == 1 && p->nb_symptomes < MAX_SYMPTOMS)
                {
                    printf("Nouveau symptome: ");
                    fgets(temp, MAX_SYMPTOM_LENGTH, stdin);
                    temp[strcspn(temp, "\n")] = '\0';
                    strcpy(p->symptomes[p->nb_symptomes], temp);
                    p->nb_symptomes++;
                }
                else if (choixSympt == 2)
                {
                    printf("\nSymptomes actuels (%d):\n", p->nb_symptomes);
                    for (int i = 0; i < p->nb_symptomes; i++)
                    {
                        printf("%d. %s\n", i+1, p->symptomes[i]);
                    }
                    pause();
                }
                else if (choixSympt == 3)
                {
                    p->nb_symptomes = 0;
                    printf("Tous les symptomes ont ete supprimes.\n");
                    pause();
                }
                break;

            case 14:
                printf("Nouvelles notes: ");
                fgets(p->notes, 500, stdin);
                p->notes[strcspn(p->notes, "\n")] = '\0';
                break;

            case 15:
                sauvegarderPatients();
                printf("\nModifications sauvegardees avec succes!\n");
                return;

            case 16:
                printf("\nModifications annulees.\n");
                return;

            default:
                printf("Choix invalide.\n");
        }

        if (choix != 15 && choix != 16)
        {
            printf("\nModification effectuee. Continuer? (O/N): ");
            char continuer;
            scanf("%c", &continuer);
            viderBuffer();

            if (toupper(continuer) != 'O')
            {
                sauvegarderPatients();
                printf("Modifications sauvegardees.\n");
                pause();
                return;
            }
        }

    } while(choix != 15 && choix != 16);
}

void supprimerPatient(int index)
{
    if (index < 0 || index >= nombrePatients)
    {
        printf("Erreur: Patient non trouve.\n");
        return;
    }

    printf("\n=== SUPPRESSION DE PATIENT ===\n");
    printf("Patient: %s %s (ID: %d)\n", patients[index].prenom, patients[index].nom, patients[index].id);
    printf("Etes-vous sur de vouloir supprimer ce patient? (O/N): ");

    char confirmation;
    scanf("%c", &confirmation);
    viderBuffer();

    if (toupper(confirmation) == 'O')
    {
        for (int i = index; i < nombrePatients - 1; i++)
        {
            patients[i] = patients[i + 1];
        }

        nombrePatients--;
        sauvegarderPatients();

        printf("Patient supprime avec succes.\n");
    }
    else
    {
        printf("Suppression annulee.\n");
    }

    pause();
}

void afficherStatistiques()
{
    system("cls");
    printf("\n=== STATISTIQUES ===\n\n");
    printf("Nombre total de patients: %d\n", nombrePatients);
    printf("Dernier ID utilise: %d\n", dernierID);

    if (nombrePatients > 0)
    {
        int avec_diagnostic = 0, sans_diagnostic = 0;
        int hommes = 0, femmes = 0;
        int totalAge = 0;

        for (int i = 0; i < nombrePatients; i++)
        {
            if (patients[i].genre == 'M') hommes++;
            else femmes++;

            if (strcmp(patients[i].diagnostic, "En attente de diagnostic") != 0)
                avec_diagnostic++;
            else sans_diagnostic++;

            totalAge += patients[i].age;
        }

        printf("\nRepartition par genre:\n");
        printf("  Hommes: %d (%.1f%%)\n", hommes, (float)hommes/nombrePatients*100);
        printf("  Femmes: %d (%.1f%%)\n", femmes, (float)femmes/nombrePatients*100);

        printf("\nEtat des diagnostics:\n");
        printf("  Avec diagnostic: %d (%.1f%%)\n", avec_diagnostic, (float)avec_diagnostic/nombrePatients*100);
        printf("  Sans diagnostic: %d (%.1f%%)\n", sans_diagnostic, (float)sans_diagnostic/nombrePatients*100);

        printf("\nAge moyen des patients: %.1f ans\n", (float)totalAge/nombrePatients);

        int enfants = 0, adultes = 0, seniors = 0;
        for (int i = 0; i < nombrePatients; i++)
        {
            if (patients[i].age < 18) enfants++;
            else if (patients[i].age < 60) adultes++;
            else seniors++;
        }

        printf("\nRepartition par age:\n");
        printf("  Enfants (<18 ans): %d (%.1f%%)\n", enfants, (float)enfants/nombrePatients*100);
        printf("  Adultes (18-59 ans): %d (%.1f%%)\n", adultes, (float)adultes/nombrePatients*100);
        printf("  Seniors (60+ ans): %d (%.1f%%)\n", seniors, (float)seniors/nombrePatients*100);
    }

    printf("\nCapacite restante: %d/%d patients\n", MAX_PATIENTS - nombrePatients, MAX_PATIENTS);

    pause();
}

void menuAffichagePatients()
{
    int choix;

    do
    {
        system("cls");
        printf("\n=== AFFICHAGE DES PATIENTS ===\n\n");
        printf("1. Liste simple des patients\n");
        printf("2. Rechercher un patient\n");
        printf("3. Retour\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
        case 1:
            afficherListePatientsSimple();
            pause();
            break;
        case 2:
            rechercherPatient();
            break;
        case 3:
            return;
        default:
            printf("Choix invalide.\n");
            pause();
        }

    } while(choix != 3);
}

void menuGestionPatients()
{
    int choix;

    do
    {
        system("cls");
        printf("\n=== GESTION DES PATIENTS ===\n\n");
        printf("1. Ajouter un nouveau patient\n");
        printf("2. Afficher/rechercher patients\n");
        printf("3. Statistiques\n");
        printf("4. Sauvegarder les donnees\n");
        printf("5. Retour au menu principal\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
        case 1:
            ajouterNouveauPatient();
            break;
        case 2:
            menuAffichagePatients();
            break;
        case 3:
            afficherStatistiques();
            break;
        case 4:
            sauvegarderPatients();
            printf("Donnees sauvegardees avec succes!\n");
            pause();
            break;
        case 5:
            return;
        default:
            printf("Choix invalide.\n");
            pause();
        }

    } while(choix != 5);
}
