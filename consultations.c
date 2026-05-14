#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "consultations.h"
#include "ordonnances.h"
#include "interventions.h"
#include "fichiers_relations.h"
#include "utils.h"
#include "patient.h"
#include "personnel.h"
#include "fichiers.h"

// Définition des variables globales
Consultation consultations[MAX_CONSULTATIONS];
int nombreConsultations = 0;
int dernierIDConsultation = 0;
RendezVous rendezVous[MAX_CONSULTATIONS];
int nombreRendezVous = 0;
int dernierIDRendezVous = 0;

void initialiserConsultations()
{
    // Rien à initialiser pour l'instant
}

void planifierConsultation()
{
    system("cls");
    printf("\n=== PLANIFICATION D'UNE CONSULTATION ===\n\n");

    if (nombrePatients == 0)
    {
        printf("ERREUR: Aucun patient enregistre!\n");
        pause();
        return;
    }

    if (nombreEmployes == 0)
    {
        printf("ERREUR: Aucun medecin disponible!\n");
        pause();
        return;
    }

    afficherListePatientsSimple();

    int id_patient;
    printf("\nID du patient: ");
    scanf("%d", &id_patient);
    viderBuffer();

    int index_patient = rechercherParID(id_patient);
    if (index_patient == -1)
    {
        printf("Patient non trouve!\n");
        pause();
        return;
    }

    printf("\nMEDECINS DISPONIBLES:\n");
    printf("ID   | Nom & Prenom           | Specialite\n");
    printf("-----|------------------------|----------------\n");

    for (int j = 0; j < nombreEmployes; j++)
    {
        if (personnel[j].actif && strcmp(personnel[j].fonction, "Medecin") == 0)
        {
            printf("%-4d | %-10s %-10s | %s\n",
                   personnel[j].id,
                   personnel[j].prenom,
                   personnel[j].nom,
                   personnel[j].specialite);
        }
    }

    int id_medecin;
    printf("\nID du medecin: ");
    scanf("%d", &id_medecin);
    viderBuffer();

    int index_medecin = rechercherEmployeParID(id_medecin);
    if (index_medecin == -1 || !personnel[index_medecin].actif)
    {
        printf("Medecin non trouve ou inactif!\n");
        pause();
        return;
    }

    Consultation c;
    memset(&c, 0, sizeof(Consultation));

    c.id_consultation = ++dernierIDConsultation;
    c.id_patient = id_patient;
    c.id_medecin = id_medecin;

    printf("Date de la consultation (JJ/MM/AAAA): ");
    fgets(c.date_consultation, 20, stdin);
    c.date_consultation[strcspn(c.date_consultation, "\n")] = '\0';

    printf("Heure de la consultation (HH:MM): ");
    fgets(c.heure_consultation, 10, stdin);
    c.heure_consultation[strcspn(c.heure_consultation, "\n")] = '\0';

    printf("Motif de la consultation: ");
    fgets(c.motif, MAX_MOTIF, stdin);
    c.motif[strcspn(c.motif, "\n")] = '\0';

    printf("Duree prevue (minutes): ");
    scanf("%d", &c.duree_minutes);
    viderBuffer();

    printf("Cout de la consultation: ");
    scanf("%f", &c.cout);
    viderBuffer();

    strcpy(c.statut, "Planifié");

    consultations[nombreConsultations] = c;
    nombreConsultations++;

    RendezVous r;
    memset(&r, 0, sizeof(RendezVous));

    r.id_rdv = ++dernierIDRendezVous;
    r.id_patient = id_patient;
    r.id_medecin = id_medecin;
    strcpy(r.date, c.date_consultation);
    strcpy(r.heure, c.heure_consultation);
    strcpy(r.type_rdv, "Consultation");
    strcpy(r.motif, c.motif);
    strcpy(r.statut, "Confirmé");
    strcpy(r.rappel_envoye, "Non");

    rendezVous[nombreRendezVous] = r;
    nombreRendezVous++;

    sauvegarderConsultations();
    sauvegarderRendezVous();

    printf("\n=== CONSULTATION PLANIFIEE AVEC SUCCES ===\n");
    printf("ID Consultation: %d\n", c.id_consultation);
    printf("Patient: %s %s\n", patients[index_patient].prenom, patients[index_patient].nom);
    printf("Medecin: %s %s\n", personnel[index_medecin].prenom, personnel[index_medecin].nom);
    printf("Date: %s a %s\n", c.date_consultation, c.heure_consultation);

    pause();
}

void ajouterConsultation(int id_patient, int id_medecin)
{
    system("cls");
    printf("\n=== AJOUT D'UNE CONSULTATION EFFECTUEE ===\n\n");

    Consultation c;
    memset(&c, 0, sizeof(Consultation));

    c.id_consultation = ++dernierIDConsultation;
    c.id_patient = id_patient;
    c.id_medecin = id_medecin;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(c.date_consultation, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    sprintf(c.heure_consultation, "%02d:%02d", tm.tm_hour, tm.tm_min);

    printf("Motif de la consultation: ");
    fgets(c.motif, MAX_MOTIF, stdin);
    c.motif[strcspn(c.motif, "\n")] = '\0';

    printf("Observations: ");
    fgets(c.observations, MAX_OBSERVATIONS, stdin);
    c.observations[strcspn(c.observations, "\n")] = '\0';

    printf("Diagnostic: ");
    fgets(c.diagnostic, 200, stdin);
    c.diagnostic[strcspn(c.diagnostic, "\n")] = '\0';

    printf("Prescriptions: ");
    fgets(c.prescriptions, 500, stdin);
    c.prescriptions[strcspn(c.prescriptions, "\n")] = '\0';

    printf("Examens demandes: ");
    fgets(c.examens_demandes, 300, stdin);
    c.examens_demandes[strcspn(c.examens_demandes, "\n")] = '\0';

    printf("Prochain rendez-vous (JJ/MM/AAAA): ");
    fgets(c.prochain_rdv, 20, stdin);
    c.prochain_rdv[strcspn(c.prochain_rdv, "\n")] = '\0';

    printf("Duree (minutes): ");
    scanf("%d", &c.duree_minutes);
    viderBuffer();

    printf("Cout: ");
    scanf("%f", &c.cout);
    viderBuffer();

    strcpy(c.statut, "Effectué");

    consultations[nombreConsultations] = c;
    nombreConsultations++;

    int index_patient = rechercherParID(id_patient);
    if (index_patient != -1 && strlen(c.diagnostic) > 0)
    {
        strcpy(patients[index_patient].diagnostic, c.diagnostic);
        sauvegarderPatients();
    }

    sauvegarderConsultations();

    printf("\n=== CONSULTATION ENREGISTREE AVEC SUCCES ===\n");
    printf("ID Consultation: %d\n", c.id_consultation);

    pause();
}

void modifierConsultation(int index)
{
    if (index < 0 || index >= nombreConsultations)
    {
        printf("Erreur: Consultation non trouvee.\n");
        return;
    }

    Consultation *c = &consultations[index];
    int choix;

    do
    {
        system("cls");
        printf("\n=== MODIFICATION CONSULTATION ===\n");
        printf("Consultation ID: %d\n\n", c->id_consultation);

        printf("1. Date (%s)\n", c->date_consultation);
        printf("2. Heure (%s)\n", c->heure_consultation);
        printf("3. Motif (%s)\n", c->motif);
        printf("4. Observations (%s)\n", c->observations);
        printf("5. Diagnostic (%s)\n", c->diagnostic);
        printf("6. Prescriptions (%s)\n", c->prescriptions);
        printf("7. Examens demandes (%s)\n", c->examens_demandes);
        printf("8. Prochain RDV (%s)\n", c->prochain_rdv);
        printf("9. Duree (%d min)\n", c->duree_minutes);
        printf("10. Cout (%.2f €)\n", c->cout);
        printf("11. Statut (%s)\n", c->statut);
        printf("12. Notes (%s)\n", c->notes);
        printf("13. Sauvegarder et quitter\n");
        printf("14. Annuler\n");
        printf("\nQuel champ modifier? (1-14): ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                printf("Nouvelle date: ");
                fgets(c->date_consultation, 20, stdin);
                c->date_consultation[strcspn(c->date_consultation, "\n")] = '\0';
                break;
            case 2:
                printf("Nouvelle heure: ");
                fgets(c->heure_consultation, 10, stdin);
                c->heure_consultation[strcspn(c->heure_consultation, "\n")] = '\0';
                break;
            case 3:
                printf("Nouveau motif: ");
                fgets(c->motif, MAX_MOTIF, stdin);
                c->motif[strcspn(c->motif, "\n")] = '\0';
                break;
            case 4:
                printf("Nouvelles observations: ");
                fgets(c->observations, MAX_OBSERVATIONS, stdin);
                c->observations[strcspn(c->observations, "\n")] = '\0';
                break;
            case 5:
                printf("Nouveau diagnostic: ");
                fgets(c->diagnostic, 200, stdin);
                c->diagnostic[strcspn(c->diagnostic, "\n")] = '\0';
                break;
            case 6:
                printf("Nouvelles prescriptions: ");
                fgets(c->prescriptions, 500, stdin);
                c->prescriptions[strcspn(c->prescriptions, "\n")] = '\0';
                break;
            case 7:
                printf("Nouveaux examens: ");
                fgets(c->examens_demandes, 300, stdin);
                c->examens_demandes[strcspn(c->examens_demandes, "\n")] = '\0';
                break;
            case 8:
                printf("Nouveau prochain RDV: ");
                fgets(c->prochain_rdv, 20, stdin);
                c->prochain_rdv[strcspn(c->prochain_rdv, "\n")] = '\0';
                break;
            case 9:
                printf("Nouvelle duree: ");
                scanf("%d", &c->duree_minutes);
                viderBuffer();
                break;
            case 10:
                printf("Nouveau cout: ");
                scanf("%f", &c->cout);
                viderBuffer();
                break;
            case 11:
                printf("Nouveau statut (Planifie/Effectue/Annule/Reporte): ");
                fgets(c->statut, 20, stdin);
                c->statut[strcspn(c->statut, "\n")] = '\0';
                break;
            case 12:
                printf("Nouvelles notes: ");
                fgets(c->notes, 500, stdin);
                c->notes[strcspn(c->notes, "\n")] = '\0';
                break;
            case 13:
                sauvegarderConsultations();
                printf("\nModifications sauvegardees!\n");
                return;
            case 14:
                return;
        }
    } while(1);
}

void annulerConsultation(int index)
{
    if (index < 0 || index >= nombreConsultations)
    {
        printf("Erreur: Consultation non trouvee.\n");
        return;
    }

    printf("\n=== ANNULATION CONSULTATION ===\n");
    printf("Confirmer l'annulation? (O/N): ");

    char confirmation;
    scanf("%c", &confirmation);
    viderBuffer();

    if (toupper(confirmation) == 'O')
    {
        strcpy(consultations[index].statut, "Annulé");
        sauvegarderConsultations();
        printf("Consultation annulee.\n");
    }
    else
    {
        printf("Annulation abandonnee.\n");
    }

    pause();
}

void consulterConsultationsPatient(int id_patient)
{
    system("cls");
    printf("\n=== CONSULTATIONS DU PATIENT ID: %d ===\n\n", id_patient);

    int compteur = 0;
    for (int i = 0; i < nombreConsultations; i++)
    {
        if (consultations[i].id_patient == id_patient)
        {
            compteur++;
            printf("\n[%d] Consultation #%d\n", compteur, consultations[i].id_consultation);
            printf("  Date: %s a %s\n", consultations[i].date_consultation, consultations[i].heure_consultation);
            printf("  Medecin ID: %d\n", consultations[i].id_medecin);
            printf("  Motif: %s\n", consultations[i].motif);
            printf("  Diagnostic: %s\n", consultations[i].diagnostic);
            printf("  Statut: %s\n", consultations[i].statut);
        }
    }

    if (compteur == 0)
    {
        printf("Aucune consultation pour ce patient.\n");
    }

    pause();
}

void consulterConsultationsMedecin(int id_medecin)
{
    system("cls");
    printf("\n=== CONSULTATIONS DU MEDECIN ID: %d ===\n\n", id_medecin);

    int compteur = 0;
    for (int i = 0; i < nombreConsultations; i++)
    {
        if (consultations[i].id_medecin == id_medecin)
        {
            compteur++;
            printf("\n[%d] Consultation #%d\n", compteur, consultations[i].id_consultation);
            printf("  Date: %s a %s\n", consultations[i].date_consultation, consultations[i].heure_consultation);
            printf("  Patient ID: %d\n", consultations[i].id_patient);
            printf("  Motif: %s\n", consultations[i].motif);
            printf("  Statut: %s\n", consultations[i].statut);
        }
    }

    if (compteur == 0)
    {
        printf("Aucune consultation pour ce medecin.\n");
    }

    pause();
}

void consulterConsultationsDate(char *date)
{
    system("cls");
    printf("\n=== CONSULTATIONS DU %s ===\n\n", date);

    int compteur = 0;
    for (int i = 0; i < nombreConsultations; i++)
    {
        if (strcmp(consultations[i].date_consultation, date) == 0)
        {
            compteur++;
            printf("\n[%d] Consultation #%d\n", compteur, consultations[i].id_consultation);
            printf("  Heure: %s\n", consultations[i].heure_consultation);
            printf("  Patient ID: %d\n", consultations[i].id_patient);
            printf("  Medecin ID: %d\n", consultations[i].id_medecin);
            printf("  Motif: %s\n", consultations[i].motif);
            printf("  Statut: %s\n", consultations[i].statut);
        }
    }

    if (compteur == 0)
    {
        printf("Aucune consultation a cette date.\n");
    }

    pause();
}

void afficherDetailsConsultation(int index)
{
    if (index < 0 || index >= nombreConsultations)
    {
        printf("Erreur: Consultation non trouvee.\n");
        return;
    }

    Consultation c = consultations[index];

    printf("\n========================================\n");
    printf("DETAILS CONSULTATION #%d\n", c.id_consultation);
    printf("========================================\n");

    printf("Patient ID: %d\n", c.id_patient);
    printf("Medecin ID: %d\n", c.id_medecin);
    printf("Date: %s a %s\n", c.date_consultation, c.heure_consultation);
    printf("Motif: %s\n", c.motif);
    printf("Observations: %s\n", c.observations);
    printf("Diagnostic: %s\n", c.diagnostic);
    printf("Prescriptions: %s\n", c.prescriptions);
    printf("Examens demandes: %s\n", c.examens_demandes);
    printf("Prochain RDV: %s\n", c.prochain_rdv);
    printf("Duree: %d minutes\n", c.duree_minutes);
    printf("Cout: %.2f €\n", c.cout);
    printf("Statut: %s\n", c.statut);

    if (strlen(c.notes) > 0)
    {
        printf("Notes: %s\n", c.notes);
    }

    printf("========================================\n");
}

void planifierRendezVous()
{
    system("cls");
    printf("\n=== PLANIFICATION RENDEZ-VOUS ===\n\n");

    RendezVous r;
    memset(&r, 0, sizeof(RendezVous));

    r.id_rdv = ++dernierIDRendezVous;

    printf("ID Patient: ");
    scanf("%d", &r.id_patient);
    viderBuffer();

    if (rechercherParID(r.id_patient) == -1)
    {
        printf("Patient non trouve!\n");
        dernierIDRendezVous--;
        pause();
        return;
    }

    printf("ID Medecin: ");
    scanf("%d", &r.id_medecin);
    viderBuffer();

    if (rechercherEmployeParID(r.id_medecin) == -1)
    {
        printf("Medecin non trouve!\n");
        dernierIDRendezVous--;
        pause();
        return;
    }

    printf("Date (JJ/MM/AAAA): ");
    fgets(r.date, 20, stdin);
    r.date[strcspn(r.date, "\n")] = '\0';

    printf("Heure (HH:MM): ");
    fgets(r.heure, 10, stdin);
    r.heure[strcspn(r.heure, "\n")] = '\0';

    printf("Type de rendez-vous: ");
    fgets(r.type_rdv, 50, stdin);
    r.type_rdv[strcspn(r.type_rdv, "\n")] = '\0';

    printf("Motif: ");
    fgets(r.motif, MAX_MOTIF, stdin);
    r.motif[strcspn(r.motif, "\n")] = '\0';

    strcpy(r.statut, "Confirmé");
    strcpy(r.rappel_envoye, "Non");

    rendezVous[nombreRendezVous] = r;
    nombreRendezVous++;

    sauvegarderRendezVous();

    printf("\nRendez-vous planifie avec succes! ID: %d\n", r.id_rdv);
    pause();
}

void confirmerRendezVous(int index)
{
    if (index < 0 || index >= nombreRendezVous)
    {
        printf("Erreur: Rendez-vous non trouve.\n");
        return;
    }

    strcpy(rendezVous[index].statut, "Confirmé");
    sauvegarderRendezVous();
    printf("Rendez-vous confirme.\n");
    pause();
}

void annulerRendezVous(int index)
{
    if (index < 0 || index >= nombreRendezVous)
    {
        printf("Erreur: Rendez-vous non trouve.\n");
        return;
    }

    printf("\nConfirmer l'annulation? (O/N): ");
    char confirmation;
    scanf("%c", &confirmation);
    viderBuffer();

    if (toupper(confirmation) == 'O')
    {
        strcpy(rendezVous[index].statut, "Annulé");
        sauvegarderRendezVous();
        printf("Rendez-vous annule.\n");
    }

    pause();
}

void listerRendezVousJour(char *date)
{
    system("cls");
    printf("\n=== RENDEZ-VOUS DU %s ===\n\n", date);

    int compteur = 0;
    for (int i = 0; i < nombreRendezVous; i++)
    {
        if (strcmp(rendezVous[i].date, date) == 0 &&
            strcmp(rendezVous[i].statut, "Confirmé") == 0)
        {
            compteur++;
            printf("[%d] %s - Patient %d - Medecin %d - %s\n",
                   compteur,
                   rendezVous[i].heure,
                   rendezVous[i].id_patient,
                   rendezVous[i].id_medecin,
                   rendezVous[i].motif);
        }
    }

    if (compteur == 0)
    {
        printf("Aucun rendez-vous ce jour.\n");
    }

    pause();
}

void menuConsultations()
{
    int choix;

    do
    {
        system("cls");
        color(13, 0);
        printf("\n=== GESTION DES CONSULTATIONS ===\n\n");
        color(7, 0);

        printf("1. Planifier une consultation\n");
        printf("2. Enregistrer une consultation effectuee\n");
        printf("3. Consulter les consultations d'un patient\n");
        printf("4. Consulter les consultations d'un medecin\n");
        printf("5. Consulter les consultations par date\n");
        printf("6. Modifier une consultation\n");
        printf("7. Annuler une consultation\n");
        printf("8. Gestion des rendez-vous\n");
        printf("9. Retour\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                planifierConsultation();
                break;
            case 2:
            {
                if (nombrePatients == 0 || nombreEmployes == 0)
                {
                    printf("Patient ou medecin manquant.\n");
                    pause();
                    break;
                }
                int id_p, id_m;
                printf("ID Patient: ");
                scanf("%d", &id_p);
                printf("ID Medecin: ");
                scanf("%d", &id_m);
                viderBuffer();
                ajouterConsultation(id_p, id_m);
                break;
            }
            case 3:
            {
                if (nombreConsultations == 0)
                {
                    printf("Aucune consultation.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Patient: ");
                scanf("%d", &id);
                viderBuffer();
                consulterConsultationsPatient(id);
                break;
            }
            case 4:
            {
                if (nombreConsultations == 0)
                {
                    printf("Aucune consultation.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Medecin: ");
                scanf("%d", &id);
                viderBuffer();
                consulterConsultationsMedecin(id);
                break;
            }
            case 5:
            {
                if (nombreConsultations == 0)
                {
                    printf("Aucune consultation.\n");
                    pause();
                    break;
                }
                char date[20];
                printf("Date (JJ/MM/AAAA): ");
                fgets(date, 20, stdin);
                date[strcspn(date, "\n")] = '\0';
                consulterConsultationsDate(date);
                break;
            }
            case 6:
            {
                if (nombreConsultations == 0)
                {
                    printf("Aucune consultation.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Consultation: ");
                scanf("%d", &id);
                viderBuffer();

                int index = -1;
                for (int i = 0; i < nombreConsultations; i++)
                {
                    if (consultations[i].id_consultation == id)
                    {
                        index = i;
                        break;
                    }
                }

                if (index != -1)
                {
                    modifierConsultation(index);
                }
                else
                {
                    printf("Consultation non trouvee.\n");
                    pause();
                }
                break;
            }
            case 7:
            {
                if (nombreConsultations == 0)
                {
                    printf("Aucune consultation.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Consultation: ");
                scanf("%d", &id);
                viderBuffer();

                int index = -1;
                for (int i = 0; i < nombreConsultations; i++)
                {
                    if (consultations[i].id_consultation == id)
                    {
                        index = i;
                        break;
                    }
                }

                if (index != -1)
                {
                    annulerConsultation(index);
                }
                else
                {
                    printf("Consultation non trouvee.\n");
                    pause();
                }
                break;
            }
            case 8:
                menuRendezVous();
                break;
            case 9:
                return;
            default:
                printf("Choix invalide.\n");
                pause();
        }
    } while(choix != 9);
}

void menuRendezVous()
{
    int choix;

    do
    {
        system("cls");
        printf("\n=== GESTION DES RENDEZ-VOUS ===\n\n");

        printf("1. Planifier un rendez-vous\n");
        printf("2. Lister rendez-vous du jour\n");
        printf("3. Confirmer un rendez-vous\n");
        printf("4. Annuler un rendez-vous\n");
        printf("5. Retour\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                planifierRendezVous();
                break;
            case 2:
            {
                char date[20];
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                sprintf(date, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

                printf("Date par defaut (aujourd'hui): %s\n", date);
                printf("Entrer une autre date (ou Entree pour aujourd'hui): ");
                char saisie[20];
                fgets(saisie, 20, stdin);
                if (strlen(saisie) > 1)
                {
                    saisie[strcspn(saisie, "\n")] = '\0';
                    listerRendezVousJour(saisie);
                }
                else
                {
                    listerRendezVousJour(date);
                }
                break;
            }
            case 3:
            {
                int id;
                printf("ID Rendez-vous: ");
                scanf("%d", &id);
                viderBuffer();

                int index = -1;
                for (int i = 0; i < nombreRendezVous; i++)
                {
                    if (rendezVous[i].id_rdv == id)
                    {
                        index = i;
                        break;
                    }
                }

                if (index != -1)
                {
                    confirmerRendezVous(index);
                }
                else
                {
                    printf("Rendez-vous non trouve.\n");
                    pause();
                }
                break;
            }
            case 4:
            {
                int id;
                printf("ID Rendez-vous: ");
                scanf("%d", &id);
                viderBuffer();

                int index = -1;
                for (int i = 0; i < nombreRendezVous; i++)
                {
                    if (rendezVous[i].id_rdv == id)
                    {
                        index = i;
                        break;
                    }
                }

                if (index != -1)
                {
                    annulerRendezVous(index);
                }
                else
                {
                    printf("Rendez-vous non trouve.\n");
                    pause();
                }
                break;
            }
            case 5:
                return;
            default:
                printf("Choix invalide.\n");
                pause();
        }
    } while(choix != 5);
}
