#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "rdv_avance.h"
#include "utils.h"
#include "patient.h"
#include "personnel.h"

// Définition des variables globales
Creneau creneaux[MAX_CRENEAUX * MAX_JOURS];
int nombreCreneaux = 0;
int dernierIDCreneau = 0;
RendezVousAvance rendezVousAvance[MAX_RDV_PAR_JOUR * MAX_JOURS];
int nombreRdvAvance = 0;
int dernierIDRdvAvance = 0;
Disponibilite disponibilites[MAX_PERSONNEL * MAX_JOURS];
int nombreDisponibilites = 0;

void initialiserModuleRdv()
{
    chargerCreneaux();
    chargerRendezVousAvance();
}

void genererCreneauxPourMedecin(int id_medecin, char *date)
{
    // Vérifier si les créneaux existent déjà
    for (int i = 0; i < nombreCreneaux; i++) {
        if (creneaux[i].id_medecin == id_medecin && strcmp(creneaux[i].date, date) == 0) {
            printf("Les creneaux pour cette date existent deja.\n");
            return;
        }
    }

    int nb_creneaux = ((HEURE_FIN - HEURE_DEBUT) * 60) / DUREE_CRENEAU;

    for (int i = 0; i < nb_creneaux; i++) {
        Creneau c;
        memset(&c, 0, sizeof(Creneau));

        c.id_creneau = ++dernierIDCreneau;
        strcpy(c.date, date);
        c.id_medecin = id_medecin;

        int heure = HEURE_DEBUT + (i * DUREE_CRENEAU) / 60;
        int minute = (i * DUREE_CRENEAU) % 60;
        sprintf(c.heure_debut, "%02d:%02d", heure, minute);

        int fin_heure = heure + (minute + DUREE_CRENEAU) / 60;
        int fin_minute = (minute + DUREE_CRENEAU) % 60;
        sprintf(c.heure_fin, "%02d:%02d", fin_heure, fin_minute);

        c.disponible = 1;
        c.id_rdv = 0;

        creneaux[nombreCreneaux++] = c;
    }

    sauvegarderCreneaux();
    printf("%d creneaux generes pour le %s.\n", nb_creneaux, date);
}

void genererCreneauxSemaine(int id_medecin, char *date_debut)
{
    int jour, mois, annee;
    sscanf(date_debut, "%d/%d/%d", &jour, &mois, &annee);

    for (int i = 0; i < 7; i++) {
        char date[20];
        sprintf(date, "%02d/%02d/%04d", jour + i, mois, annee);
        genererCreneauxPourMedecin(id_medecin, date);
    }
}

void afficherPlanningMedecin(int id_medecin, char *date)
{
    system("cls");

    int idx_medecin = rechercherEmployeParID(id_medecin);
    if (idx_medecin == -1) {
        printf("Medecin non trouve.\n");
        pause();
        return;
    }

    printf("\n=== PLANNING DU DR %s %s ===\n",
           personnel[idx_medecin].prenom, personnel[idx_medecin].nom);
    printf("Date: %s\n\n", date);

    printf("Heure      | Statut      | Patient\n");
    printf("-----------|-------------|------------------\n");

    int trouve = 0;
    for (int i = 0; i < nombreCreneaux; i++) {
        if (creneaux[i].id_medecin == id_medecin && strcmp(creneaux[i].date, date) == 0) {
            trouve = 1;
            char patient_info[25] = "Libre";

            if (creneaux[i].disponible == 0) {
                for (int j = 0; j < nombreRdvAvance; j++) {
                    if (rendezVousAvance[j].id_creneau == creneaux[i].id_creneau) {
                        int idx_patient = rechercherParID(rendezVousAvance[j].id_patient);
                        if (idx_patient != -1) {
                            sprintf(patient_info, "%s %s",
                                    patients[idx_patient].prenom,
                                    patients[idx_patient].nom);
                        }
                        break;
                    }
                }
                printf("%s - %s | RESERVE    | %s\n",
                       creneaux[i].heure_debut, creneaux[i].heure_fin, patient_info);
            } else {
                printf("%s - %s | DISPONIBLE | %s\n",
                       creneaux[i].heure_debut, creneaux[i].heure_fin, patient_info);
            }
        }
    }

    if (!trouve) {
        printf("Aucun creneau genere pour cette date.\n");
        printf("\nVoulez-vous generer les creneaux? (O/N): ");
        char reponse;
        scanf("%c", &reponse);
        viderBuffer();
        if (toupper(reponse) == 'O') {
            genererCreneauxPourMedecin(id_medecin, date);
            afficherPlanningMedecin(id_medecin, date);
        }
    }

    pause();
}

void prendreRendezVousAvance()
{
    system("cls");
    printf("\n=== PRISE DE RENDEZ-VOUS AVANCEE ===\n\n");

    if (nombrePatients == 0) {
        printf("Aucun patient enregistre.\n");
        pause();
        return;
    }

    afficherListePatientsSimple();
    printf("\nID du patient: ");
    int id_patient;
    scanf("%d", &id_patient);
    viderBuffer();

    int idx_patient = rechercherParID(id_patient);
    if (idx_patient == -1) {
        printf("Patient non trouve.\n");
        pause();
        return;
    }

    printf("\nID du medecin: ");
    int id_medecin;
    scanf("%d", &id_medecin);
    viderBuffer();

    char date[20];
    printf("\nDate (JJ/MM/AAAA): ");
    fgets(date, 20, stdin);
    date[strcspn(date, "\n")] = '\0';

    // Afficher les créneaux disponibles
    printf("\nCRENEAUX DISPONIBLES:\n");
    int creneaux_dispo[MAX_CRENEAUX];
    int nb_dispo = 0;

    for (int i = 0; i < nombreCreneaux; i++) {
        if (creneaux[i].id_medecin == id_medecin &&
            strcmp(creneaux[i].date, date) == 0 &&
            creneaux[i].disponible == 1) {
            printf("%d. %s - %s\n", nb_dispo + 1, creneaux[i].heure_debut, creneaux[i].heure_fin);
            creneaux_dispo[nb_dispo++] = i;
        }
    }

    if (nb_dispo == 0) {
        printf("Aucun creneau disponible. Voulez-vous generer des creneaux? (O/N): ");
        char rep;
        scanf("%c", &rep);
        viderBuffer();
        if (toupper(rep) == 'O') {
            genererCreneauxPourMedecin(id_medecin, date);
            prendreRendezVousAvance();
        }
        return;
    }

    printf("\nChoisissez un creneau (1-%d): ", nb_dispo);
    int choix;
    scanf("%d", &choix);
    viderBuffer();

    if (choix < 1 || choix > nb_dispo) {
        printf("Choix invalide.\n");
        pause();
        return;
    }

    int idx_creneau = creneaux_dispo[choix - 1];

    RendezVousAvance rdv;
    memset(&rdv, 0, sizeof(RendezVousAvance));

    rdv.id_rdv = ++dernierIDRdvAvance;
    rdv.id_patient = id_patient;
    rdv.id_medecin = id_medecin;
    rdv.id_creneau = creneaux[idx_creneau].id_creneau;
    strcpy(rdv.date, date);
    strcpy(rdv.heure, creneaux[idx_creneau].heure_debut);
    rdv.duree = DUREE_CRENEAU;
    rdv.cout = 30.0;

    printf("Type (Consultation/Urgence/Controle/Suivi): ");
    fgets(rdv.type, 30, stdin);
    rdv.type[strcspn(rdv.type, "\n")] = '\0';

    printf("Motif: ");
    fgets(rdv.motif, 200, stdin);
    rdv.motif[strcspn(rdv.motif, "\n")] = '\0';

    strcpy(rdv.statut, "Confirmé");
    strcpy(rdv.rappel_envoye, "Non");

    creneaux[idx_creneau].disponible = 0;
    creneaux[idx_creneau].id_rdv = rdv.id_rdv;
    strcpy(creneaux[idx_creneau].motif, rdv.motif);

    rendezVousAvance[nombreRdvAvance++] = rdv;

    sauvegarderCreneaux();
    sauvegarderRendezVousAvance();

    printf("\n=== RENDEZ-VOUS PRIS AVEC SUCCES ===\n");
    printf("ID: %d\n", rdv.id_rdv);
    printf("Patient: %s %s\n", patients[idx_patient].prenom, patients[idx_patient].nom);
    printf("Date: %s a %s\n", rdv.date, rdv.heure);

    pause();
}

// Lister les rendez-vous d'un patient
void listerRendezVousPatient(int id_patient)
{
    system("cls");
    printf("\n=== RENDEZ-VOUS DU PATIENT ID: %d ===\n\n", id_patient);

    int compteur = 0;
    for (int i = 0; i < nombreRdvAvance; i++)
    {
        if (rendezVousAvance[i].id_patient == id_patient)
        {
            compteur++;
            printf("[%d] %s a %s - Medecin ID: %d - %s - %s\n",
                   compteur,
                   rendezVousAvance[i].date,
                   rendezVousAvance[i].heure,
                   rendezVousAvance[i].id_medecin,
                   rendezVousAvance[i].type,
                   rendezVousAvance[i].statut);
        }
    }

    if (compteur == 0)
    {
        printf("Aucun rendez-vous pour ce patient.\n");
    }

    pause();
}

void annulerRendezVousAvance(int id_rdv)
{
    for (int i = 0; i < nombreRdvAvance; i++) {
        if (rendezVousAvance[i].id_rdv == id_rdv) {
            printf("\nAnnuler le rendez-vous du %s a %s? (O/N): ",
                   rendezVousAvance[i].date, rendezVousAvance[i].heure);
            char confirmation;
            scanf("%c", &confirmation);
            viderBuffer();

            if (toupper(confirmation) == 'O') {
                strcpy(rendezVousAvance[i].statut, "Annulé");

                for (int j = 0; j < nombreCreneaux; j++) {
                    if (creneaux[j].id_creneau == rendezVousAvance[i].id_creneau) {
                        creneaux[j].disponible = 1;
                        creneaux[j].id_rdv = 0;
                        break;
                    }
                }

                sauvegarderCreneaux();
                sauvegarderRendezVousAvance();
                printf("Rendez-vous annule.\n");
            }
            pause();
            return;
        }
    }
    printf("Rendez-vous non trouve.\n");
    pause();
}

void confirmerRendezVousAvance(int id_rdv)
{
    for (int i = 0; i < nombreRdvAvance; i++) {
        if (rendezVousAvance[i].id_rdv == id_rdv) {
            strcpy(rendezVousAvance[i].statut, "Confirmé");
            sauvegarderRendezVousAvance();
            printf("Rendez-vous confirme.\n");
            pause();
            return;
        }
    }
    printf("Rendez-vous non trouve.\n");
    pause();
}

void gererDisponibilitesMedecin(int id_medecin)
{
    int choix;
    do {
        system("cls");
        printf("\n=== GESTION DES DISPONIBILITES ===\n");
        printf("1. Generer creneaux pour un jour\n");
        printf("2. Generer creneaux pour une semaine\n");
        printf("3. Voir planning\n");
        printf("4. Retour\n");
        printf("Choix: ");
        scanf("%d", &choix);
        viderBuffer();

        switch(choix) {
            case 1: {
                char date[20];
                printf("Date (JJ/MM/AAAA): ");
                fgets(date, 20, stdin);
                date[strcspn(date, "\n")] = '\0';
                genererCreneauxPourMedecin(id_medecin, date);
                pause();
                break;
            }
            case 2: {
                char date[20];
                printf("Date debut (JJ/MM/AAAA): ");
                fgets(date, 20, stdin);
                date[strcspn(date, "\n")] = '\0';
                genererCreneauxSemaine(id_medecin, date);
                pause();
                break;
            }
            case 3: {
                char date[20];
                printf("Date (JJ/MM/AAAA): ");
                fgets(date, 20, stdin);
                date[strcspn(date, "\n")] = '\0';
                afficherPlanningMedecin(id_medecin, date);
                break;
            }
            case 4:
                return;
        }
    } while(choix != 4);
}

void menuRendezVousAvance()
{
    int choix;
    do {
        system("cls");
        printf("\n=== RENDEZ-VOUS AVANCE ===\n");
        printf("1. Prendre rendez-vous\n");
        printf("2. Gerer disponibilites d'un medecin\n");
        printf("3. Voir planning d'un medecin\n");
        printf("4. Annuler un rendez-vous\n");
        printf("5. Retour\n");
        printf("Choix: ");
        scanf("%d", &choix);
        viderBuffer();

        switch(choix) {
            case 1:
                prendreRendezVousAvance();
                break;
            case 2: {
                int id;
                printf("ID Medecin: ");
                scanf("%d", &id);
                viderBuffer();
                gererDisponibilitesMedecin(id);
                break;
            }
            case 3: {
                int id;
                char date[20];
                printf("ID Medecin: ");
                scanf("%d", &id);
                viderBuffer();
                printf("Date (JJ/MM/AAAA): ");
                fgets(date, 20, stdin);
                date[strcspn(date, "\n")] = '\0';
                afficherPlanningMedecin(id, date);
                break;
            }
            case 4: {
                int id;
                printf("ID Rendez-vous: ");
                scanf("%d", &id);
                viderBuffer();
                annulerRendezVousAvance(id);
                break;
            }
            case 5:
                return;
        }
    } while(choix != 5);
}
