#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "patient.h"
#include "fichiers.h"
#include "diagnostic.h"
#include "utils.h"
#include "comptes.h"
#include "consultations.h"
#include "rdv_avance.h"

// Variables globales
Patient patients[MAX_PATIENTS];
int nombrePatients = 0;
int dernierID = 0;

// Déclarations externes
extern Employe personnel[MAX_PERSONNEL];
extern int nombreEmployes;
extern Consultation consultations[MAX_CONSULTATIONS];
extern int nombreConsultations;
extern RendezVous rendezVous[MAX_CONSULTATIONS];
extern int nombreRendezVous;

// Fonction pour générer un ID
int genererID()
{
    dernierID++;
    return dernierID;
}

// Chercher l'ID d'un médecin à partir de son nom
int getMedecinIDByName(char *nom_medecin)
{
    for (int i = 0; i < nombreEmployes; i++)
    {
        if (personnel[i].actif && strcmp(personnel[i].fonction, "Medecin") == 0)
        {
            char nom_complet[MAX_NAME];
            sprintf(nom_complet, "%s %s", personnel[i].prenom, personnel[i].nom);

            if (strcmp(nom_complet, nom_medecin) == 0 ||
                strcmp(personnel[i].nom, nom_medecin) == 0 ||
                strstr(nom_complet, nom_medecin) != NULL)
            {
                return personnel[i].id;
            }
        }
    }
    return 0;
}

// Vérifier si un patient est lié à un médecin (UNIQUEMENT par le nom)
int patientEstLieAuMedecin(int id_patient, int id_medecin)
{
    int idx_patient = rechercherParID(id_patient);
    if (idx_patient == -1) return 0;

    // Récupérer le nom du médecin
    int idx_medecin = rechercherEmployeParID(id_medecin);
    if (idx_medecin == -1) return 0;

    char nom_medecin_complet[MAX_NAME];
    sprintf(nom_medecin_complet, "%s %s", personnel[idx_medecin].prenom, personnel[idx_medecin].nom);

    // Vérifier si le médecin est le médecin traitant (par nom)
    if (strcmp(patients[idx_patient].medecin_traitant, nom_medecin_complet) == 0)
    {
        return 1;
    }

    // Vérifier si le médecin apparaît dans le champ medecin_traitant
    if (strstr(patients[idx_patient].medecin_traitant, personnel[idx_medecin].nom) != NULL)
    {
        return 1;
    }

    // Vérifier dans les consultations
    for (int i = 0; i < nombreConsultations; i++)
    {
        if (consultations[i].id_patient == id_patient && consultations[i].id_medecin == id_medecin)
        {
            return 1;
        }
    }

    // Vérifier dans les rendez-vous
    for (int i = 0; i < nombreRendezVous; i++)
    {
        if (rendezVous[i].id_patient == id_patient && rendezVous[i].id_medecin == id_medecin)
        {
            return 1;
        }
    }

    return 0;
}

// Afficher la liste des patients pour un médecin
void afficherListePatientsPourMedecin(int id_medecin)
{
    system("cls");
    printf("\n=== MES PATIENTS ===\n\n");

    int idx_medecin = rechercherEmployeParID(id_medecin);
    if (idx_medecin != -1)
    {
        printf("Medecin: Dr %s %s\n\n", personnel[idx_medecin].prenom, personnel[idx_medecin].nom);
    }

    int compteur = 0;
    printf("ID   | Nom & Prenom           | Age | Medecin Traitant\n");
    printf("-----|------------------------|-----|-----------------\n");

    for (int i = 0; i < nombrePatients; i++)
    {
        if (patientEstLieAuMedecin(patients[i].id, id_medecin))
        {
            compteur++;
            printf("%-4d | %-10s %-10s | %-3d | %s\n",
                   patients[i].id,
                   patients[i].prenom,
                   patients[i].nom,
                   patients[i].age,
                   patients[i].medecin_traitant);
        }
    }

    if (compteur == 0)
    {
        printf("Aucun patient lie a votre activite.\n");
    }
    else
    {
        printf("\nTotal: %d patient(s)\n", compteur);
    }

    printf("\nEntrez l'ID d'un patient pour voir son dossier complet (0 pour quitter): ");
    int id;
    scanf("%d", &id);
    viderBuffer();

    if (id > 0)
    {
        int index = rechercherParID(id);
        if (index != -1 && patientEstLieAuMedecin(id, id_medecin))
        {
            afficherDetailsCompletsPatient(index);
            pause();
        }
        else if (index != -1 && !patientEstLieAuMedecin(id, id_medecin))
        {
            printf("Acces non autorise. Ce patient ne vous est pas associe.\n");
            pause();
        }
        else
        {
            printf("Patient non trouve.\n");
            pause();
        }
    }
}

// Ajouter un nouveau patient
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

    p.id = genererID();

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

    // Afficher la liste des médecins
    printf("LISTE DES MEDECINS DISPONIBLES:\n");
    for (int i = 0; i < nombreEmployes; i++)
    {
        if (personnel[i].actif && strcmp(personnel[i].fonction, "Medecin") == 0)
        {
            printf("  - Dr %s %s\n", personnel[i].prenom, personnel[i].nom);
        }
    }

    printf("\nEntrez le NOM du medecin traitant (ou 'aucun'): ");
    char nom_medecin[MAX_NAME];
    fgets(nom_medecin, MAX_NAME, stdin);
    nom_medecin[strcspn(nom_medecin, "\n")] = '\0';

    if (strcmp(nom_medecin, "aucun") != 0 && strlen(nom_medecin) > 0)
    {
        int id_med = getMedecinIDByName(nom_medecin);
        if (id_med != 0)
        {
            int idx_med = rechercherEmployeParID(id_med);
            if (idx_med != -1)
            {
                sprintf(p.medecin_traitant, "%s %s", personnel[idx_med].prenom, personnel[idx_med].nom);
                printf("Medecin assigne: Dr %s\n", p.medecin_traitant);
            }
        }
        else
        {
            printf("Medecin non trouve. Aucun medecin assigne.\n");
        }
    }

    if (strlen(p.medecin_traitant) == 0)
    {
        strcpy(p.medecin_traitant, "Aucun");
    }

    printf("\nDate d'admission (JJ/MM/AAAA): ");
    fgets(p.date_admission, 20, stdin);
    p.date_admission[strcspn(p.date_admission, "\n")] = '\0';

    printf("\nSYMPTOMES (Entrez 'fin' pour terminer)\n");
    p.nb_symptomes = 0;
    for (int i = 0; i < MAX_SYMPTOMS; i++)
    {
        printf("Symptome %d: ", i + 1);
        char temp[MAX_SYMPTOM_LENGTH];
        fgets(temp, MAX_SYMPTOM_LENGTH, stdin);
        temp[strcspn(temp, "\n")] = '\0';
        if (strcmp(temp, "fin") == 0 || strlen(temp) == 0) break;
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
    pause();
}

// Afficher la liste simple des patients
void afficherListePatientsSimple()
{
    system("cls");
    printf("\n=== LISTE DES PATIENTS ===\n\n");
    if (nombrePatients == 0)
    {
        printf("Aucun patient enregistre.\n");
        return;
    }
    printf("Nombre total: %d\n\n", nombrePatients);
    printf("ID   | Nom & Prenom           | Age | Medecin Traitant\n");
    printf("-----|------------------------|-----|-----------------\n");
    for (int i = 0; i < nombrePatients; i++)
    {
        printf("%-4d | %-10s %-10s | %-3d | %s\n",
               patients[i].id,
               patients[i].prenom,
               patients[i].nom,
               patients[i].age,
               patients[i].medecin_traitant);
    }
}

// Afficher les détails complets d'un patient
void afficherDetailsCompletsPatient(int index)
{
    if (index < 0 || index >= nombrePatients)
    {
        printf("Patient non trouve.\n");
        return;
    }
    Patient p = patients[index];
    printf("\n========================================\n");
    printf("FICHE PATIENT - ID: %d\n", p.id);
    printf("========================================\n");
    printf("Nom: %s %s\n", p.prenom, p.nom);
    printf("Age: %d ans\n", p.age);
    printf("Genre: %c\n", p.genre);
    printf("Tel: %s\n", p.telephone);
    printf("Email: %s\n", p.email);
    printf("Adresse: %s\n", p.adresse);
    printf("Date naissance: %s\n", p.date_naissance);
    printf("Groupe sanguin: %s\n", p.groupe_sanguin);
    printf("Medecin traitant: %s\n", p.medecin_traitant);
    printf("Date admission: %s\n", p.date_admission);
    printf("Diagnostic: %s\n", p.diagnostic);
    printf("Symptomes (%d):\n", p.nb_symptomes);
    for (int i = 0; i < p.nb_symptomes; i++)
        printf("  - %s\n", p.symptomes[i]);
    if (strlen(p.notes) > 0)
        printf("Notes: %s\n", p.notes);
    printf("========================================\n");
}

// Rechercher un patient par ID
int rechercherParID(int id)
{
    for (int i = 0; i < nombrePatients; i++)
        if (patients[i].id == id) return i;
    return -1;
}

// Rechercher un patient (menu)
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
    printf("ID du patient: ");
    scanf("%d", &id);
    viderBuffer();
    int index = rechercherParID(id);
    if (index != -1)
    {
        afficherDetailsCompletsPatient(index);
        printf("\n1. Modifier  2. Diagnostiquer  3. Supprimer  4. Retour: ");
        int action;
        scanf("%d", &action);
        viderBuffer();
        if (action == 1) modifierPatient(index);
        else if (action == 2) diagnosticPatientExistant(index);
        else if (action == 3) supprimerPatient(index);
    }
    else
    {
        printf("Patient non trouve.\n");
        pause();
    }
}

// Modifier un patient
void modifierPatient(int index)
{
    if (index < 0 || index >= nombrePatients) return;
    Patient *p = &patients[index];
    int choix;
    do
    {
        system("cls");
        printf("\n=== MODIFICATION PATIENT %s %s ===\n", p->prenom, p->nom);
        printf("1. Nom (%s)\n2. Prenom (%s)\n3. Age (%d)\n4. Telephone (%s)\n5. Adresse (%s)\n",
               p->nom, p->prenom, p->age, p->telephone, p->adresse);
        printf("6. Medecin traitant (%s)\n7. Diagnostic (%s)\n8. Notes\n9. Sauvegarder\n10. Annuler\nChoix: ",
               p->medecin_traitant, p->diagnostic);
        scanf("%d", &choix);
        viderBuffer();
        switch(choix)
        {
            case 1: printf("Nom: "); fgets(p->nom, MAX_NAME, stdin); p->nom[strcspn(p->nom,"\n")]='\0'; break;
            case 2: printf("Prenom: "); fgets(p->prenom, MAX_NAME, stdin); p->prenom[strcspn(p->prenom,"\n")]='\0'; break;
            case 3: printf("Age: "); scanf("%d",&p->age); viderBuffer(); break;
            case 4: printf("Telephone: "); fgets(p->telephone,20,stdin); p->telephone[strcspn(p->telephone,"\n")]='\0'; break;
            case 5: printf("Adresse: "); fgets(p->adresse,200,stdin); p->adresse[strcspn(p->adresse,"\n")]='\0'; break;
            case 6: modifierMedecinTraitant(p->id); break;
            case 7: printf("Diagnostic: "); fgets(p->diagnostic,200,stdin); p->diagnostic[strcspn(p->diagnostic,"\n")]='\0'; break;
            case 8: printf("Notes: "); fgets(p->notes,500,stdin); p->notes[strcspn(p->notes,"\n")]='\0'; break;
            case 9: sauvegarderPatients(); printf("Sauvegarde OK\n"); return;
            case 10: return;
        }
    } while(1);
}

// Supprimer un patient
void supprimerPatient(int index)
{
    if (index < 0 || index >= nombrePatients) return;
    printf("\nSupprimer %s %s? (O/N): ", patients[index].prenom, patients[index].nom);
    char conf;
    scanf("%c", &conf);
    viderBuffer();
    if (toupper(conf) == 'O')
    {
        for (int i = index; i < nombrePatients - 1; i++)
            patients[i] = patients[i + 1];
        nombrePatients--;
        sauvegarderPatients();
        printf("Patient supprime.\n");
    }
    pause();
}

// Modifier le médecin traitant
void modifierMedecinTraitant(int id_patient)
{
    int idx = rechercherParID(id_patient);
    if (idx == -1) return;

    printf("\n=== MODIFICATION MEDECIN TRAITANT ===\n");
    printf("Medecin actuel: %s\n", patients[idx].medecin_traitant);

    // Afficher la liste des médecins
    printf("\nMEDECINS DISPONIBLES:\n");
    for (int i = 0; i < nombreEmployes; i++)
    {
        if (personnel[i].actif && strcmp(personnel[i].fonction, "Medecin") == 0)
        {
            printf("  - Dr %s %s\n", personnel[i].prenom, personnel[i].nom);
        }
    }

    printf("\nEntrez le NOM du nouveau medecin (ou 'aucun'): ");
    char nom_med[MAX_NAME];
    fgets(nom_med, MAX_NAME, stdin);
    nom_med[strcspn(nom_med, "\n")] = '\0';

    if (strcmp(nom_med, "aucun") == 0 || strlen(nom_med) == 0)
    {
        strcpy(patients[idx].medecin_traitant, "Aucun");
        printf("Medecin traitant supprime.\n");
    }
    else
    {
        int id_med = getMedecinIDByName(nom_med);
        if (id_med != 0)
        {
            int idx_med = rechercherEmployeParID(id_med);
            if (idx_med != -1)
            {
                sprintf(patients[idx].medecin_traitant, "%s %s", personnel[idx_med].prenom, personnel[idx_med].nom);
                printf("Medecin assigne: Dr %s\n", patients[idx].medecin_traitant);
            }
        }
        else
        {
            printf("Medecin non trouve.\n");
        }
    }
    sauvegarderPatients();
    pause();
}

// Menu affichage patients
void menuAffichagePatients()
{
    int choix;
    do
    {
        system("cls");
        printf("\n=== AFFICHAGE PATIENTS ===\n");
        printf("1. Liste simple\n2. Rechercher\n3. Retour\nChoix: ");
        scanf("%d", &choix);
        viderBuffer();
        switch(choix)
        {
            case 1: afficherListePatientsSimple(); pause(); break;
            case 2: rechercherPatient(); break;
            case 3: return;
        }
    } while(1);
}

// Menu gestion patients
void menuGestionPatients()
{
    // Si c'est un médecin, rediriger vers la liste restreinte
    if (utilisateur_actuel != NULL && utilisateur_actuel->profil == ROLE_MEDECIN)
    {
        afficherListePatientsPourMedecin(utilisateur_actuel->id_associe);
        return;
    }

    int choix;
    do
    {
        system("cls");
        printf("\n=== GESTION PATIENTS ===\n");
        printf("1. Ajouter\n2. Afficher/Rechercher\n3. Statistiques\n4. Sauvegarder\n5. Retour\nChoix: ");
        scanf("%d", &choix);
        viderBuffer();
        switch(choix)
        {
            case 1: ajouterNouveauPatient(); break;
            case 2: menuAffichagePatients(); break;
            case 3: afficherStatistiques(); break;
            case 4: sauvegarderPatients(); printf("Sauvegarde OK\n"); pause(); break;
            case 5: return;
        }
    } while(1);
}

// Statistiques
void afficherStatistiques()
{
    system("cls");
    printf("\n=== STATISTIQUES ===\n");
    printf("Total patients: %d\n", nombrePatients);
    if (nombrePatients > 0)
    {
        int hommes=0, femmes=0, totalAge=0;
        for (int i=0; i<nombrePatients; i++)
        {
            if (patients[i].genre=='M') hommes++;
            else femmes++;
            totalAge += patients[i].age;
        }
        printf("Hommes: %d (%.1f%%)\n", hommes, (float)hommes/nombrePatients*100);
        printf("Femmes: %d (%.1f%%)\n", femmes, (float)femmes/nombrePatients*100);
        printf("Age moyen: %.1f ans\n", (float)totalAge/nombrePatients);
    }
    pause();
}

// Questionnaire medical
void questionnaireMedical(int id_patient)
{
    system("cls");
    printf("\n=== QUESTIONNAIRE MEDICAL ===\n");
    printf("Repondez par O (Oui) ou N (Non)\n\n");

    const char *questions[] = {
        "Avez-vous de la fievre ?",
        "Avez-vous de la toux ?",
        "Avez-vous des maux de tete ?",
        "Ressentez-vous une fatigue ?",
        "Avez-vous des douleurs musculaires ?",
        "Avez-vous des nausees ?",
        "Avez-vous mal a la gorge ?",
        "Avez-vous du mal a respirer ?"
    };
    int nb_questions = 8;
    char symptomes[MAX_SYMPTOMS][MAX_SYMPTOM_LENGTH];
    int nb_symptomes = 0;

    for (int i = 0; i < nb_questions; i++)
    {
        printf("%s (O/N): ", questions[i]);
        char rep;
        scanf("%c", &rep);
        viderBuffer();
        if (toupper(rep) == 'O')
        {
            if (strstr(questions[i], "fievre")) strcpy(symptomes[nb_symptomes++], "fievre");
            else if (strstr(questions[i], "toux")) strcpy(symptomes[nb_symptomes++], "toux");
            else if (strstr(questions[i], "maux de tete")) strcpy(symptomes[nb_symptomes++], "maux de tete");
            else if (strstr(questions[i], "fatigue")) strcpy(symptomes[nb_symptomes++], "fatigue");
            else if (strstr(questions[i], "douleurs musculaires")) strcpy(symptomes[nb_symptomes++], "douleurs musculaires");
            else if (strstr(questions[i], "nausees")) strcpy(symptomes[nb_symptomes++], "nausees");
            else if (strstr(questions[i], "mal a la gorge")) strcpy(symptomes[nb_symptomes++], "maux de gorge");
            else if (strstr(questions[i], "mal a respirer")) strcpy(symptomes[nb_symptomes++], "difficultes respiratoires");
        }
    }

    int idx = rechercherParID(id_patient);
    if (idx != -1)
    {
        patients[idx].nb_symptomes = nb_symptomes;
        for (int i = 0; i < nb_symptomes; i++)
            strcpy(patients[idx].symptomes[i], symptomes[i]);
        sauvegarderPatients();
        printf("\n%d symptome(s) enregistre(s).\n", nb_symptomes);
    }
    pause();
}

// Completer dossier patient
void completerDossierPatient(int id_compte)
{
    system("cls");
    printf("\n=== COMPLETION DOSSIER MEDICAL ===\n\n");

    Patient p;
    memset(&p, 0, sizeof(Patient));
    p.id = genererID();

    printf("Nom: "); fgets(p.nom, MAX_NAME, stdin); p.nom[strcspn(p.nom,"\n")]='\0';
    printf("Prenom: "); fgets(p.prenom, MAX_NAME, stdin); p.prenom[strcspn(p.prenom,"\n")]='\0';
    printf("Age: "); scanf("%d",&p.age); viderBuffer();
    printf("Genre (M/F): "); scanf("%c",&p.genre); viderBuffer(); p.genre=toupper(p.genre);
    printf("Telephone: "); fgets(p.telephone,20,stdin); p.telephone[strcspn(p.telephone,"\n")]='\0';
    printf("Email: "); fgets(p.email,100,stdin); p.email[strcspn(p.email,"\n")]='\0';
    printf("Adresse: "); fgets(p.adresse,200,stdin); p.adresse[strcspn(p.adresse,"\n")]='\0';
    printf("Date naissance (JJ/MM/AAAA): "); fgets(p.date_naissance,20,stdin); p.date_naissance[strcspn(p.date_naissance,"\n")]='\0';
    printf("Groupe sanguin: "); fgets(p.groupe_sanguin,5,stdin); p.groupe_sanguin[strcspn(p.groupe_sanguin,"\n")]='\0';
    printf("Date admission (JJ/MM/AAAA): "); fgets(p.date_admission,20,stdin); p.date_admission[strcspn(p.date_admission,"\n")]='\0';

    // Selection medecin traitant
    printf("\nMEDECINS DISPONIBLES:\n");
    for (int i = 0; i < nombreEmployes; i++)
    {
        if (personnel[i].actif && strcmp(personnel[i].fonction, "Medecin") == 0)
        {
            printf("  - Dr %s %s\n", personnel[i].prenom, personnel[i].nom);
        }
    }
    printf("\nNom du medecin traitant (ou 'aucun'): ");
    char nom_med[MAX_NAME];
    fgets(nom_med, MAX_NAME, stdin);
    nom_med[strcspn(nom_med, "\n")] = '\0';

    if (strcmp(nom_med, "aucun") != 0 && strlen(nom_med) > 0)
    {
        int id_med = getMedecinIDByName(nom_med);
        if (id_med != 0)
        {
            int idx_med = rechercherEmployeParID(id_med);
            if (idx_med != -1)
            {
                sprintf(p.medecin_traitant, "%s %s", personnel[idx_med].prenom, personnel[idx_med].nom);
                printf("Medecin assigne: Dr %s\n", p.medecin_traitant);
            }
        }
        else
        {
            printf("Medecin non trouve.\n");
        }
    }

    if (strlen(p.medecin_traitant) == 0)
    {
        strcpy(p.medecin_traitant, "Aucun");
    }

    strcpy(p.diagnostic, "En attente");
    p.nb_symptomes = 0;

    patients[nombrePatients] = p;
    nombrePatients++;

    for (int i = 0; i < nombreComptes; i++)
    {
        if (comptes[i].id_compte == id_compte)
        {
            comptes[i].id_associe = p.id;
            comptes[i].compte_complet = 1;
            break;
        }
    }

    sauvegarderPatients();
    sauvegarderComptes();

    printf("\nDossier cree ! ID Patient: %d\n", p.id);
    if (strcmp(p.medecin_traitant, "Aucun") != 0)
        printf("Medecin traitant: %s\n", p.medecin_traitant);

    printf("\nQuestionnaire medical:\n");
    questionnaireMedical(p.id);
}
