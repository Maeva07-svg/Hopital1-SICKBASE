#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "ordonnances.h"
#include "fichiers_relations.h"
#include "utils.h"
#include "patient.h"
#include "personnel.h"

// Définition des variables globales
Ordonnance ordonnances[MAX_ORDONNANCES];
int nombreOrdonnances = 0;
int dernierIDOrdonnance = 0;

void initialiserOrdonnances()
{
    // Rien à initialiser
}

void creerOrdonnance(int id_patient, int id_medecin, int id_consultation)
{
    system("cls");
    printf("\n=== CREATION D'UNE ORDONNANCE ===\n\n");

    Ordonnance o;
    memset(&o, 0, sizeof(Ordonnance));

    o.id_ordonnance = ++dernierIDOrdonnance;
    o.id_patient = id_patient;
    o.id_medecin = id_medecin;
    o.id_consultation = id_consultation;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(o.date_prescription, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    printf("Date debut traitement (JJ/MM/AAAA): ");
    fgets(o.date_debut, 20, stdin);
    o.date_debut[strcspn(o.date_debut, "\n")] = '\0';

    printf("Date fin traitement (JJ/MM/AAAA): ");
    fgets(o.date_fin, 20, stdin);
    o.date_fin[strcspn(o.date_fin, "\n")] = '\0';

    printf("Diagnostic lie: ");
    fgets(o.diagnostic, 200, stdin);
    o.diagnostic[strcspn(o.diagnostic, "\n")] = '\0';

    printf("Instructions generales: ");
    fgets(o.instructions_generales, 500, stdin);
    o.instructions_generales[strcspn(o.instructions_generales, "\n")] = '\0';

    printf("Ordonnance renouvelable? (O/N): ");
    char rep;
    scanf("%c", &rep);
    viderBuffer();

    o.renouvelable = (toupper(rep) == 'O') ? 1 : 0;

    if (o.renouvelable)
    {
        printf("Nombre de renouvellements: ");
        scanf("%d", &o.nombre_renouvellements);
        viderBuffer();
    }

    // Saisie des médicaments
    o.nb_medicaments = 0;
    char continuer = 'O';

    while (continuer == 'O' && o.nb_medicaments < MAX_MEDICAMENTS)
    {
        printf("\n--- MEDICAMENT %d ---\n", o.nb_medicaments + 1);

        printf("Nom du medicament: ");
        fgets(o.medicaments[o.nb_medicaments].nom, 100, stdin);
        o.medicaments[o.nb_medicaments].nom[strcspn(o.medicaments[o.nb_medicaments].nom, "\n")] = '\0';

        printf("Dosage (ex: 500mg): ");
        fgets(o.medicaments[o.nb_medicaments].dosage, 50, stdin);
        o.medicaments[o.nb_medicaments].dosage[strcspn(o.medicaments[o.nb_medicaments].dosage, "\n")] = '\0';

        printf("Forme (Comprime/Sirop/Injectable/...): ");
        fgets(o.medicaments[o.nb_medicaments].forme, 50, stdin);
        o.medicaments[o.nb_medicaments].forme[strcspn(o.medicaments[o.nb_medicaments].forme, "\n")] = '\0';

        printf("Posologie (ex: 1 comprime matin et soir): ");
        fgets(o.medicaments[o.nb_medicaments].posologie, MAX_POSOLOGIE, stdin);
        o.medicaments[o.nb_medicaments].posologie[strcspn(o.medicaments[o.nb_medicaments].posologie, "\n")] = '\0';

        printf("Duree traitement: ");
        fgets(o.medicaments[o.nb_medicaments].duree_traitement, 50, stdin);
        o.medicaments[o.nb_medicaments].duree_traitement[strcspn(o.medicaments[o.nb_medicaments].duree_traitement, "\n")] = '\0';

        printf("Instructions particulieres: ");
        fgets(o.medicaments[o.nb_medicaments].instructions, 200, stdin);
        o.medicaments[o.nb_medicaments].instructions[strcspn(o.medicaments[o.nb_medicaments].instructions, "\n")] = '\0';

        o.nb_medicaments++;

        if (o.nb_medicaments < MAX_MEDICAMENTS)
        {
            printf("\nAjouter un autre medicament? (O/N): ");
            scanf("%c", &continuer);
            viderBuffer();
            continuer = toupper(continuer);
        }
    }

    strcpy(o.statut, "Active");

    ordonnances[nombreOrdonnances] = o;
    nombreOrdonnances++;

    sauvegarderOrdonnances();

    printf("\n=== ORDONNANCE CREE AVEC SUCCES ===\n");
    printf("ID Ordonnance: %d\n", o.id_ordonnance);
    printf("Nombre de medicaments: %d\n", o.nb_medicaments);

    pause();
}

void ajouterMedicament(int index_ordonnance)
{
    if (index_ordonnance < 0 || index_ordonnance >= nombreOrdonnances)
    {
        printf("Erreur: Ordonnance non trouvee.\n");
        return;
    }

    Ordonnance *o = &ordonnances[index_ordonnance];

    if (o->nb_medicaments >= MAX_MEDICAMENTS)
    {
        printf("Nombre maximum de medicaments atteint.\n");
        return;
    }

    printf("\n--- NOUVEAU MEDICAMENT ---\n");

    printf("Nom: ");
    fgets(o->medicaments[o->nb_medicaments].nom, 100, stdin);
    o->medicaments[o->nb_medicaments].nom[strcspn(o->medicaments[o->nb_medicaments].nom, "\n")] = '\0';

    printf("Dosage: ");
    fgets(o->medicaments[o->nb_medicaments].dosage, 50, stdin);
    o->medicaments[o->nb_medicaments].dosage[strcspn(o->medicaments[o->nb_medicaments].dosage, "\n")] = '\0';

    printf("Forme: ");
    fgets(o->medicaments[o->nb_medicaments].forme, 50, stdin);
    o->medicaments[o->nb_medicaments].forme[strcspn(o->medicaments[o->nb_medicaments].forme, "\n")] = '\0';

    printf("Posologie: ");
    fgets(o->medicaments[o->nb_medicaments].posologie, MAX_POSOLOGIE, stdin);
    o->medicaments[o->nb_medicaments].posologie[strcspn(o->medicaments[o->nb_medicaments].posologie, "\n")] = '\0';

    printf("Duree: ");
    fgets(o->medicaments[o->nb_medicaments].duree_traitement, 50, stdin);
    o->medicaments[o->nb_medicaments].duree_traitement[strcspn(o->medicaments[o->nb_medicaments].duree_traitement, "\n")] = '\0';

    printf("Instructions: ");
    fgets(o->medicaments[o->nb_medicaments].instructions, 200, stdin);
    o->medicaments[o->nb_medicaments].instructions[strcspn(o->medicaments[o->nb_medicaments].instructions, "\n")] = '\0';

    o->nb_medicaments++;
    sauvegarderOrdonnances();

    printf("Medicament ajoute avec succes.\n");
    pause();
}

void modifierOrdonnance(int index)
{
    if (index < 0 || index >= nombreOrdonnances)
    {
        printf("Erreur: Ordonnance non trouvee.\n");
        return;
    }

    Ordonnance *o = &ordonnances[index];
    int choix;

    do
    {
        system("cls");
        printf("\n=== MODIFICATION ORDONNANCE #%d ===\n", o->id_ordonnance);

        printf("1. Date debut (%s)\n", o->date_debut);
        printf("2. Date fin (%s)\n", o->date_fin);
        printf("3. Diagnostic (%s)\n", o->diagnostic);
        printf("4. Instructions generales (%s)\n", o->instructions_generales);
        printf("5. Renouvelable (%s)\n", o->renouvelable ? "Oui" : "Non");
        printf("6. Statut (%s)\n", o->statut);
        printf("7. Ajouter un medicament\n");
        printf("8. Notes\n");
        printf("9. Sauvegarder et quitter\n");
        printf("10. Annuler\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                printf("Nouvelle date debut: ");
                fgets(o->date_debut, 20, stdin);
                o->date_debut[strcspn(o->date_debut, "\n")] = '\0';
                break;
            case 2:
                printf("Nouvelle date fin: ");
                fgets(o->date_fin, 20, stdin);
                o->date_fin[strcspn(o->date_fin, "\n")] = '\0';
                break;
            case 3:
                printf("Nouveau diagnostic: ");
                fgets(o->diagnostic, 200, stdin);
                o->diagnostic[strcspn(o->diagnostic, "\n")] = '\0';
                break;
            case 4:
                printf("Nouvelles instructions: ");
                fgets(o->instructions_generales, 500, stdin);
                o->instructions_generales[strcspn(o->instructions_generales, "\n")] = '\0';
                break;
            case 5:
                o->renouvelable = !o->renouvelable;
                printf("Renouvelable: %s\n", o->renouvelable ? "Oui" : "Non");
                break;
            case 6:
                printf("Statut (Active/Terminee/Annulee): ");
                fgets(o->statut, 20, stdin);
                o->statut[strcspn(o->statut, "\n")] = '\0';
                break;
            case 7:
                if (o->nb_medicaments < MAX_MEDICAMENTS)
                    ajouterMedicament(index);
                else
                    printf("Maximum atteint!\n");
                break;
            case 8:
                printf("Nouvelles notes: ");
                fgets(o->notes, 300, stdin);
                o->notes[strcspn(o->notes, "\n")] = '\0';
                break;
            case 9:
                sauvegarderOrdonnances();
                return;
            case 10:
                return;
        }
    } while(1);
}

void afficherOrdonnance(int index)
{
    if (index < 0 || index >= nombreOrdonnances)
    {
        printf("Erreur: Ordonnance non trouvee.\n");
        return;
    }

    Ordonnance o = ordonnances[index];

    printf("\n========================================\n");
    printf("ORDONNANCE #%d\n", o.id_ordonnance);
    printf("========================================\n");

    printf("Patient: %d\n", o.id_patient);
    printf("Medecin: %d\n", o.id_medecin);
    printf("Date prescription: %s\n", o.date_prescription);
    printf("Traitement du %s au %s\n", o.date_debut, o.date_fin);
    printf("Diagnostic: %s\n", o.diagnostic);

    printf("\nMEDICAMENTS (%d):\n", o.nb_medicaments);
    for (int i = 0; i < o.nb_medicaments; i++)
    {
        printf("\n%d. %s %s\n", i+1, o.medicaments[i].nom, o.medicaments[i].dosage);
        printf("   Forme: %s\n", o.medicaments[i].forme);
        printf("   Posologie: %s\n", o.medicaments[i].posologie);
        printf("   Duree: %s\n", o.medicaments[i].duree_traitement);
        printf("   Instructions: %s\n", o.medicaments[i].instructions);
    }

    printf("\nInstructions generales: %s\n", o.instructions_generales);
    printf("Renouvelable: %s (%d fois)\n", o.renouvelable ? "Oui" : "Non", o.nombre_renouvellements);
    printf("Statut: %s\n", o.statut);

    if (strlen(o.notes) > 0)
        printf("Notes: %s\n", o.notes);

    printf("========================================\n");
}

void afficherOrdonnancesPatient(int id_patient)
{
    system("cls");
    printf("\n=== ORDONNANCES DU PATIENT %d ===\n\n", id_patient);

    int compteur = 0;
    for (int i = 0; i < nombreOrdonnances; i++)
    {
        if (ordonnances[i].id_patient == id_patient)
        {
            compteur++;
            printf("\n[%d] Ordonnance #%d - %s\n", compteur, ordonnances[i].id_ordonnance, ordonnances[i].date_prescription);
            printf("   Medecin: %d - %d medicaments - %s\n",
                   ordonnances[i].id_medecin,
                   ordonnances[i].nb_medicaments,
                   ordonnances[i].statut);
        }
    }

    if (compteur == 0)
    {
        printf("Aucune ordonnance pour ce patient.\n");
    }

    pause();
}

void afficherOrdonnancesMedecin(int id_medecin)
{
    system("cls");
    printf("\n=== ORDONNANCES DU MEDECIN %d ===\n\n", id_medecin);

    int compteur = 0;
    for (int i = 0; i < nombreOrdonnances; i++)
    {
        if (ordonnances[i].id_medecin == id_medecin)
        {
            compteur++;
            printf("\n[%d] Ordonnance #%d - %s\n", compteur, ordonnances[i].id_ordonnance, ordonnances[i].date_prescription);
            printf("   Patient: %d - %d medicaments - %s\n",
                   ordonnances[i].id_patient,
                   ordonnances[i].nb_medicaments,
                   ordonnances[i].statut);
        }
    }

    if (compteur == 0)
    {
        printf("Aucune ordonnance pour ce medecin.\n");
    }

    pause();
}

void renouvelerOrdonnance(int index)
{
    if (index < 0 || index >= nombreOrdonnances)
    {
        printf("Erreur: Ordonnance non trouvee.\n");
        return;
    }

    Ordonnance *o = &ordonnances[index];

    if (!o->renouvelable || o->nombre_renouvellements <= 0)
    {
        printf("Cette ordonnance n'est pas renouvelable.\n");
        return;
    }

    o->nombre_renouvellements--;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(o->date_prescription, "%02d/%02d/%04d (renouvellement)", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    sauvegarderOrdonnances();
    printf("Ordonnance renouvelee avec succes.\n");
    pause();
}

void terminerOrdonnance(int index)
{
    if (index < 0 || index >= nombreOrdonnances)
    {
        printf("Erreur: Ordonnance non trouvee.\n");
        return;
    }

    strcpy(ordonnances[index].statut, "Terminée");
    sauvegarderOrdonnances();
    printf("Ordonnance terminee.\n");
    pause();
}

void imprimerOrdonnance(int index)
{
    if (index < 0 || index >= nombreOrdonnances)
    {
        printf("Erreur: Ordonnance non trouvee.\n");
        return;
    }

    Ordonnance o = ordonnances[index];

    printf("\n\n");
    printf("========================================\n");
    printf("           ORDONNANCE MEDICALE          \n");
    printf("========================================\n\n");

    int index_patient = rechercherParID(o.id_patient);
    int index_medecin = rechercherEmployeParID(o.id_medecin);

    if (index_patient != -1)
    {
        printf("Patient: %s %s\n", patients[index_patient].prenom, patients[index_patient].nom);
        printf("Né(e) le: %s\n", patients[index_patient].date_naissance);
    }

    if (index_medecin != -1)
    {
        printf("Medecin: Dr %s %s\n", personnel[index_medecin].prenom, personnel[index_medecin].nom);
        printf("Specialite: %s\n", personnel[index_medecin].specialite);
        printf("N° Ordre: %s\n", personnel[index_medecin].numero_ordre);
    }

    printf("\nDate: %s\n", o.date_prescription);
    printf("Diagnostic: %s\n\n", o.diagnostic);

    printf("PRESCRIPTIONS:\n");
    printf("--------------\n");

    for (int i = 0; i < o.nb_medicaments; i++)
    {
        printf("%s %s - %s\n", o.medicaments[i].nom, o.medicaments[i].dosage, o.medicaments[i].forme);
        printf("  Posologie: %s\n", o.medicaments[i].posologie);
        printf("  Duree: %s\n", o.medicaments[i].duree_traitement);
        if (strlen(o.medicaments[i].instructions) > 0)
            printf("  Note: %s\n", o.medicaments[i].instructions);
        printf("\n");
    }

    if (strlen(o.instructions_generales) > 0)
    {
        printf("INSTRUCTIONS:\n");
        printf("%s\n", o.instructions_generales);
    }

    printf("\nTraitement du %s au %s\n", o.date_debut, o.date_fin);
    printf("\n========================================\n");
    printf("Cachet du medecin & signature\n");
    printf("========================================\n\n");

    pause();
}

void menuOrdonnances()
{
    int choix;

    do
    {
        system("cls");
        color(12, 0); // Rouge
        printf("\n=== GESTION DES ORDONNANCES ===\n\n");
        color(7, 0);

        printf("1. Creer une ordonnance\n");
        printf("2. Afficher ordonnances d'un patient\n");
        printf("3. Afficher ordonnances d'un medecin\n");
        printf("4. Consulter une ordonnance\n");
        printf("5. Modifier une ordonnance\n");
        printf("6. Ajouter un medicament\n");
        printf("7. Renouveler une ordonnance\n");
        printf("8. Terminer une ordonnance\n");
        printf("9. Imprimer une ordonnance\n");
        printf("10. Retour\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
            {
                if (nombrePatients == 0 || nombreEmployes == 0)
                {
                    printf("Patient ou medecin manquant.\n");
                    pause();
                    break;
                }
                int id_p, id_m, id_c = 0;
                printf("ID Patient: ");
                scanf("%d", &id_p);
                printf("ID Medecin: ");
                scanf("%d", &id_m);
                printf("ID Consultation (0 si aucune): ");
                scanf("%d", &id_c);
                viderBuffer();
                creerOrdonnance(id_p, id_m, id_c);
                break;
            }
            case 2:
            {
                if (nombreOrdonnances == 0)
                {
                    printf("Aucune ordonnance.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Patient: ");
                scanf("%d", &id);
                viderBuffer();
                afficherOrdonnancesPatient(id);
                break;
            }
            case 3:
            {
                if (nombreOrdonnances == 0)
                {
                    printf("Aucune ordonnance.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Medecin: ");
                scanf("%d", &id);
                viderBuffer();
                afficherOrdonnancesMedecin(id);
                break;
            }
            case 4:
            {
                if (nombreOrdonnances == 0)
                {
                    printf("Aucune ordonnance.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Ordonnance: ");
                scanf("%d", &id);
                viderBuffer();

                int index = -1;
                for (int i = 0; i < nombreOrdonnances; i++)
                {
                    if (ordonnances[i].id_ordonnance == id)
                    {
                        index = i;
                        break;
                    }
                }

                if (index != -1)
                {
                    afficherOrdonnance(index);
                    pause();
                }
                else
                {
                    printf("Ordonnance non trouvee.\n");
                    pause();
                }
                break;
            }
            case 5:
            {
                if (nombreOrdonnances == 0)
                {
                    printf("Aucune ordonnance.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Ordonnance: ");
                scanf("%d", &id);
                viderBuffer();

                int index = -1;
                for (int i = 0; i < nombreOrdonnances; i++)
                {
                    if (ordonnances[i].id_ordonnance == id)
                    {
                        index = i;
                        break;
                    }
                }

                if (index != -1)
                {
                    modifierOrdonnance(index);
                }
                else
                {
                    printf("Ordonnance non trouvee.\n");
                }
                break;
            }
            case 6:
            {
                if (nombreOrdonnances == 0)
                {
                    printf("Aucune ordonnance.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Ordonnance: ");
                scanf("%d", &id);
                viderBuffer();

                int index = -1;
                for (int i = 0; i < nombreOrdonnances; i++)
                {
                    if (ordonnances[i].id_ordonnance == id)
                    {
                        index = i;
                        break;
                    }
                }

                if (index != -1)
                {
                    ajouterMedicament(index);
                }
                else
                {
                    printf("Ordonnance non trouvee.\n");
                }
                break;
            }
            case 7:
            {
                if (nombreOrdonnances == 0)
                {
                    printf("Aucune ordonnance.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Ordonnance: ");
                scanf("%d", &id);
                viderBuffer();

                int index = -1;
                for (int i = 0; i < nombreOrdonnances; i++)
                {
                    if (ordonnances[i].id_ordonnance == id)
                    {
                        index = i;
                        break;
                    }
                }

                if (index != -1)
                {
                    renouvelerOrdonnance(index);
                }
                else
                {
                    printf("Ordonnance non trouvee.\n");
                }
                break;
            }
            case 8:
            {
                if (nombreOrdonnances == 0)
                {
                    printf("Aucune ordonnance.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Ordonnance: ");
                scanf("%d", &id);
                viderBuffer();

                int index = -1;
                for (int i = 0; i < nombreOrdonnances; i++)
                {
                    if (ordonnances[i].id_ordonnance == id)
                    {
                        index = i;
                        break;
                    }
                }

                if (index != -1)
                {
                    terminerOrdonnance(index);
                }
                else
                {
                    printf("Ordonnance non trouvee.\n");
                }
                break;
            }
            case 9:
            {
                if (nombreOrdonnances == 0)
                {
                    printf("Aucune ordonnance.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Ordonnance: ");
                scanf("%d", &id);
                viderBuffer();

                int index = -1;
                for (int i = 0; i < nombreOrdonnances; i++)
                {
                    if (ordonnances[i].id_ordonnance == id)
                    {
                        index = i;
                        break;
                    }
                }

                if (index != -1)
                {
                    imprimerOrdonnance(index);
                }
                else
                {
                    printf("Ordonnance non trouvee.\n");
                }
                break;
            }
            case 10:
                return;
            default:
                printf("Choix invalide.\n");
                pause();
        }
    } while(choix != 10);
}
