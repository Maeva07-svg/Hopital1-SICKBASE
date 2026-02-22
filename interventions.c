#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "interventions.h"
#include "fichiers_relations.h"
#include "utils.h"
#include "patient.h"
#include "personnel.h"

// Définition des variables globales
Intervention interventions[MAX_INTERVENTIONS];
int nombreInterventions = 0;
int dernierIDIntervention = 0;
PriseEnCharge prisesEnCharge[MAX_INTERVENTIONS];
int nombrePrisesEnCharge = 0;
int dernierIDPriseEnCharge = 0;

void initialiserInterventions()
{
    // Rien à initialiser
}

// ============================================
// FONCTIONS POUR LES INTERVENTIONS CHIRURGICALES
// ============================================

void planifierIntervention()
{
    system("cls");
    printf("\n=== PLANIFICATION D'UNE INTERVENTION CHIRURGICALE ===\n\n");

    if (nombrePatients == 0)
    {
        printf("ERREUR: Aucun patient enregistre!\n");
        pause();
        return;
    }

    // Vérifier s'il y a des chirurgiens disponibles
    int chirurgiens_dispo = 0;
    for (int i = 0; i < nombreEmployes; i++)
    {
        if (personnel[i].actif && strcmp(personnel[i].fonction, "Chirurgien") == 0)
        {
            chirurgiens_dispo = 1;
            break;
        }
    }

    if (!chirurgiens_dispo)
    {
        printf("ERREUR: Aucun chirurgien disponible!\n");
        pause();
        return;
    }

    Intervention i;
    memset(&i, 0, sizeof(Intervention));

    i.id_intervention = ++dernierIDIntervention;
    i.nb_infirmiers = 0;
    i.nb_materiel = 0;

    // Sélection du patient
    afficherListePatientsSimple();
    printf("\nID du patient: ");
    scanf("%d", &i.id_patient);
    viderBuffer();

    int index_patient = rechercherParID(i.id_patient);
    if (index_patient == -1)
    {
        printf("Patient non trouve!\n");
        dernierIDIntervention--;
        pause();
        return;
    }

    // Sélection du chirurgien
    printf("\nCHIRURGIENS DISPONIBLES:\n");
    printf("ID   | Nom & Prenom           | Specialite\n");
    printf("-----|------------------------|----------------\n");

    for (int j = 0; j < nombreEmployes; j++)
    {
        if (personnel[j].actif && strcmp(personnel[j].fonction, "Chirurgien") == 0)
        {
            printf("%-4d | %-10s %-10s | %s\n",
                   personnel[j].id,
                   personnel[j].prenom,
                   personnel[j].nom,
                   personnel[j].specialite);
        }
    }

    printf("\nID du chirurgien: ");
    scanf("%d", &i.id_chirurgien);
    viderBuffer();

    int index_chir = rechercherEmployeParID(i.id_chirurgien);
    if (index_chir == -1 || !personnel[index_chir].actif)
    {
        printf("Chirurgien non trouve ou inactif!\n");
        dernierIDIntervention--;
        pause();
        return;
    }

    // Sélection de l'anesthésiste (optionnel)
    printf("\nANESTHESISTES DISPONIBLES (0 pour aucun):\n");
    for (int j = 0; j < nombreEmployes; j++)
    {
        if (personnel[j].actif && strcmp(personnel[j].fonction, "Medecin") == 0 &&
            (strstr(personnel[j].specialite, "anesth") != NULL ||
             strstr(personnel[j].specialite, "Anesth") != NULL))
        {
            printf("ID %d: %s %s\n", personnel[j].id, personnel[j].prenom, personnel[j].nom);
        }
    }

    printf("\nID anesthesiste: ");
    scanf("%d", &i.id_anesthesiste);
    viderBuffer();

    // Informations sur l'intervention
    printf("\nINFORMATIONS SUR L'INTERVENTION:\n\n");

    printf("Date de l'intervention (JJ/MM/AAAA): ");
    fgets(i.date_intervention, 20, stdin);
    i.date_intervention[strcspn(i.date_intervention, "\n")] = '\0';

    printf("Heure debut (HH:MM): ");
    fgets(i.heure_debut, 10, stdin);
    i.heure_debut[strcspn(i.heure_debut, "\n")] = '\0';

    printf("Heure fin prevue (HH:MM): ");
    fgets(i.heure_fin, 10, stdin);
    i.heure_fin[strcspn(i.heure_fin, "\n")] = '\0';

    printf("Type d'intervention: ");
    fgets(i.type_intervention, 100, stdin);
    i.type_intervention[strcspn(i.type_intervention, "\n")] = '\0';

    printf("Description: ");
    fgets(i.description, 500, stdin);
    i.description[strcspn(i.description, "\n")] = '\0';

    printf("Salle d'operation: ");
    fgets(i.salle_operation, 30, stdin);
    i.salle_operation[strcspn(i.salle_operation, "\n")] = '\0';

    printf("Protocole anesthesie: ");
    fgets(i.protocole_anesthesie, 300, stdin);
    i.protocole_anesthesie[strcspn(i.protocole_anesthesie, "\n")] = '\0';

    printf("Niveau urgence (Faible/Moyen/Urgent/Tres urgent): ");
    fgets(i.niveau_urgence, 20, stdin);
    i.niveau_urgence[strcspn(i.niveau_urgence, "\n")] = '\0';

    printf("Cout estime: ");
    scanf("%f", &i.cout_estime);
    viderBuffer();

    // Ajout des infirmiers
    printf("\nAJOUT DES INFIRMIERS (0 pour terminer)\n");
    while (i.nb_infirmiers < MAX_EQUIPE)
    {
        printf("\nINFIRMIERS DISPONIBLES:\n");
        for (int j = 0; j < nombreEmployes; j++)
        {
            if (personnel[j].actif && strcmp(personnel[j].fonction, "Infirmier") == 0)
            {
                printf("ID %d: %s %s - %s\n",
                       personnel[j].id,
                       personnel[j].prenom,
                       personnel[j].nom,
                       personnel[j].service);
            }
        }

        printf("\nID infirmier %d (0 pour terminer): ", i.nb_infirmiers + 1);
        int id_inf;
        scanf("%d", &id_inf);
        viderBuffer();

        if (id_inf == 0) break;

        int index_inf = rechercherEmployeParID(id_inf);
        if (index_inf != -1 && personnel[index_inf].actif)
        {
            i.ids_infirmiers[i.nb_infirmiers] = id_inf;
            i.nb_infirmiers++;
        }
        else
        {
            printf("Infirmier non trouve ou inactif!\n");
        }
    }

    // Ajout du matériel
    printf("\nMATERIEL UTILISE (laisser vide pour terminer)\n");
    while (i.nb_materiel < MAX_MATERIEL)
    {
        printf("Materiel %d: ", i.nb_materiel + 1);
        char materiel[100];
        fgets(materiel, 100, stdin);
        materiel[strcspn(materiel, "\n")] = '\0';

        if (strlen(materiel) == 0) break;

        strcpy(i.materiel_utilise[i.nb_materiel], materiel);
        i.nb_materiel++;
    }

    printf("Prescriptions post-operatoires: ");
    fgets(i.prescriptions_post_op, 500, stdin);
    i.prescriptions_post_op[strcspn(i.prescriptions_post_op, "\n")] = '\0';

    printf("Date de sortie prevue (JJ/MM/AAAA): ");
    fgets(i.date_sortie, 20, stdin);
    i.date_sortie[strcspn(i.date_sortie, "\n")] = '\0';

    printf("Notes: ");
    fgets(i.notes, 500, stdin);
    i.notes[strcspn(i.notes, "\n")] = '\0';

    strcpy(i.statut, "Planifiée");

    interventions[nombreInterventions] = i;
    nombreInterventions++;

    sauvegarderInterventions();

    printf("\n=== INTERVENTION PLANIFIEE AVEC SUCCES ===\n");
    printf("ID Intervention: %d\n", i.id_intervention);
    printf("Patient: %s %s\n", patients[index_patient].prenom, patients[index_patient].nom);
    printf("Chirurgien: %s %s\n", personnel[index_chir].prenom, personnel[index_chir].nom);
    printf("Date: %s a %s\n", i.date_intervention, i.heure_debut);
    printf("Salle: %s\n", i.salle_operation);

    pause();
}

void ajouterIntervention()
{
    // Similaire à planifier mais pour une intervention déjà effectuée
    system("cls");
    printf("\n=== ENREGISTREMENT D'UNE INTERVENTION EFFECTUEE ===\n\n");

    // Récupérer la date actuelle
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    Intervention i;
    memset(&i, 0, sizeof(Intervention));

    i.id_intervention = ++dernierIDIntervention;
    i.nb_infirmiers = 0;
    i.nb_materiel = 0;

    afficherListePatientsSimple();
    printf("\nID du patient: ");
    scanf("%d", &i.id_patient);
    viderBuffer();

    int index_patient = rechercherParID(i.id_patient);
    if (index_patient == -1)
    {
        printf("Patient non trouve!\n");
        dernierIDIntervention--;
        pause();
        return;
    }

    printf("\nID du chirurgien: ");
    scanf("%d", &i.id_chirurgien);
    viderBuffer();

    printf("\nID anesthesiste (0 si aucun): ");
    scanf("%d", &i.id_anesthesiste);
    viderBuffer();

    // Date automatique = aujourd'hui
    sprintf(i.date_intervention, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    printf("Heure debut (HH:MM): ");
    fgets(i.heure_debut, 10, stdin);
    i.heure_debut[strcspn(i.heure_debut, "\n")] = '\0';

    printf("Heure fin (HH:MM): ");
    fgets(i.heure_fin, 10, stdin);
    i.heure_fin[strcspn(i.heure_fin, "\n")] = '\0';

    printf("Type d'intervention: ");
    fgets(i.type_intervention, 100, stdin);
    i.type_intervention[strcspn(i.type_intervention, "\n")] = '\0';

    printf("Description: ");
    fgets(i.description, 500, stdin);
    i.description[strcspn(i.description, "\n")] = '\0';

    printf("Salle d'operation: ");
    fgets(i.salle_operation, 30, stdin);
    i.salle_operation[strcspn(i.salle_operation, "\n")] = '\0';

    printf("Compte rendu operatoire: ");
    fgets(i.compte_rendu_operatoire, 1000, stdin);
    i.compte_rendu_operatoire[strcspn(i.compte_rendu_operatoire, "\n")] = '\0';

    printf("Prescriptions post-op: ");
    fgets(i.prescriptions_post_op, 500, stdin);
    i.prescriptions_post_op[strcspn(i.prescriptions_post_op, "\n")] = '\0';

    strcpy(i.statut, "Terminée");

    interventions[nombreInterventions] = i;
    nombreInterventions++;

    sauvegarderInterventions();

    printf("\nIntervention enregistree avec succes! ID: %d\n", i.id_intervention);
    pause();
}

void afficherIntervention(int index)
{
    if (index < 0 || index >= nombreInterventions)
    {
        printf("Erreur: Intervention non trouvee.\n");
        return;
    }

    Intervention i = interventions[index];

    printf("\n========================================\n");
    printf("INTERVENTION CHIRURGICALE #%d\n", i.id_intervention);
    printf("========================================\n");

    int idx_patient = rechercherParID(i.id_patient);
    int idx_chir = rechercherEmployeParID(i.id_chirurgien);
    int idx_anes = rechercherEmployeParID(i.id_anesthesiste);

    printf("\nPATIENT:\n");
    if (idx_patient != -1)
        printf("  %s %s (ID: %d)\n", patients[idx_patient].prenom, patients[idx_patient].nom, i.id_patient);
    else
        printf("  ID Patient: %d\n", i.id_patient);

    printf("\nEQUIPE MEDICALE:\n");
    if (idx_chir != -1)
        printf("  Chirurgien: Dr %s %s\n", personnel[idx_chir].prenom, personnel[idx_chir].nom);
    else
        printf("  Chirurgien ID: %d\n", i.id_chirurgien);

    if (i.id_anesthesiste != 0)
    {
        if (idx_anes != -1)
            printf("  Anesthesiste: Dr %s %s\n", personnel[idx_anes].prenom, personnel[idx_anes].nom);
        else
            printf("  Anesthesiste ID: %d\n", i.id_anesthesiste);
    }

    if (i.nb_infirmiers > 0)
    {
        printf("  Infirmiers:\n");
        for (int j = 0; j < i.nb_infirmiers; j++)
        {
            int idx_inf = rechercherEmployeParID(i.ids_infirmiers[j]);
            if (idx_inf != -1)
                printf("    - %s %s\n", personnel[idx_inf].prenom, personnel[idx_inf].nom);
            else
                printf("    - ID: %d\n", i.ids_infirmiers[j]);
        }
    }

    printf("\nINTERVENTION:\n");
    printf("  Date: %s\n", i.date_intervention);
    printf("  Horaire: %s - %s\n", i.heure_debut, i.heure_fin);
    printf("  Type: %s\n", i.type_intervention);
    printf("  Salle: %s\n", i.salle_operation);
    printf("  Description: %s\n", i.description);
    printf("  Niveau urgence: %s\n", i.niveau_urgence);
    printf("  Statut: %s\n", i.statut);

    if (i.nb_materiel > 0)
    {
        printf("\nMATERIEL UTILISE:\n");
        for (int j = 0; j < i.nb_materiel; j++)
        {
            printf("  - %s\n", i.materiel_utilise[j]);
        }
    }

    printf("\nPROTOCOLE ANESTHESIE:\n");
    printf("  %s\n", i.protocole_anesthesie);

    printf("\nCOMPTE RENDU OPERATOIRE:\n");
    printf("  %s\n", i.compte_rendu_operatoire);

    printf("\nPRESCRIPTIONS POST-OPERATOIRES:\n");
    printf("  %s\n", i.prescriptions_post_op);

    printf("\nSORTIE:\n");
    printf("  Date sortie prevue: %s\n", i.date_sortie);

    if (strlen(i.notes) > 0)
    {
        printf("\nNOTES:\n");
        printf("  %s\n", i.notes);
    }

    printf("\nCOUT ESTIME: %.2f €\n", i.cout_estime);
    printf("========================================\n");
}

void afficherInterventionsPatient(int id_patient)
{
    system("cls");
    printf("\n=== INTERVENTIONS DU PATIENT ID: %d ===\n\n", id_patient);

    int compteur = 0;
    for (int i = 0; i < nombreInterventions; i++)
    {
        if (interventions[i].id_patient == id_patient)
        {
            compteur++;
            printf("\n[%d] Intervention #%d\n", compteur, interventions[i].id_intervention);
            printf("  Date: %s\n", interventions[i].date_intervention);
            printf("  Type: %s\n", interventions[i].type_intervention);
            printf("  Chirurgien ID: %d\n", interventions[i].id_chirurgien);
            printf("  Statut: %s\n", interventions[i].statut);
        }
    }

    if (compteur == 0)
    {
        printf("Aucune intervention pour ce patient.\n");
    }

    pause();
}

void afficherInterventionsChirurgien(int id_chirurgien)
{
    system("cls");
    printf("\n=== INTERVENTIONS DU CHIRURGIEN ID: %d ===\n\n", id_chirurgien);

    int compteur = 0;
    for (int i = 0; i < nombreInterventions; i++)
    {
        if (interventions[i].id_chirurgien == id_chirurgien)
        {
            compteur++;
            printf("\n[%d] Intervention #%d\n", compteur, interventions[i].id_intervention);
            printf("  Patient ID: %d\n", interventions[i].id_patient);
            printf("  Date: %s\n", interventions[i].date_intervention);
            printf("  Type: %s\n", interventions[i].type_intervention);
            printf("  Statut: %s\n", interventions[i].statut);
        }
    }

    if (compteur == 0)
    {
        printf("Aucune intervention pour ce chirurgien.\n");
    }

    pause();
}

void modifierIntervention(int index)
{
    if (index < 0 || index >= nombreInterventions)
    {
        printf("Erreur: Intervention non trouvee.\n");
        return;
    }

    Intervention *i = &interventions[index];
    int choix;

    do
    {
        system("cls");
        printf("\n=== MODIFICATION INTERVENTION #%d ===\n", i->id_intervention);

        printf("1. Date (%s)\n", i->date_intervention);
        printf("2. Heure debut (%s)\n", i->heure_debut);
        printf("3. Heure fin (%s)\n", i->heure_fin);
        printf("4. Type intervention (%s)\n", i->type_intervention);
        printf("5. Description\n");
        printf("6. Salle (%s)\n", i->salle_operation);
        printf("7. Protocole anesthesie\n");
        printf("8. Compte rendu operatoire\n");
        printf("9. Prescriptions post-op\n");
        printf("10. Date sortie (%s)\n", i->date_sortie);
        printf("11. Statut (%s)\n", i->statut);
        printf("12. Cout (%.2f)\n", i->cout_estime);
        printf("13. Notes\n");
        printf("14. Sauvegarder et quitter\n");
        printf("15. Annuler\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                printf("Nouvelle date: ");
                fgets(i->date_intervention, 20, stdin);
                i->date_intervention[strcspn(i->date_intervention, "\n")] = '\0';
                break;
            case 2:
                printf("Nouvelle heure debut: ");
                fgets(i->heure_debut, 10, stdin);
                i->heure_debut[strcspn(i->heure_debut, "\n")] = '\0';
                break;
            case 3:
                printf("Nouvelle heure fin: ");
                fgets(i->heure_fin, 10, stdin);
                i->heure_fin[strcspn(i->heure_fin, "\n")] = '\0';
                break;
            case 4:
                printf("Nouveau type: ");
                fgets(i->type_intervention, 100, stdin);
                i->type_intervention[strcspn(i->type_intervention, "\n")] = '\0';
                break;
            case 5:
                printf("Nouvelle description: ");
                fgets(i->description, 500, stdin);
                i->description[strcspn(i->description, "\n")] = '\0';
                break;
            case 6:
                printf("Nouvelle salle: ");
                fgets(i->salle_operation, 30, stdin);
                i->salle_operation[strcspn(i->salle_operation, "\n")] = '\0';
                break;
            case 7:
                printf("Nouveau protocole: ");
                fgets(i->protocole_anesthesie, 300, stdin);
                i->protocole_anesthesie[strcspn(i->protocole_anesthesie, "\n")] = '\0';
                break;
            case 8:
                printf("Nouveau compte rendu: ");
                fgets(i->compte_rendu_operatoire, 1000, stdin);
                i->compte_rendu_operatoire[strcspn(i->compte_rendu_operatoire, "\n")] = '\0';
                break;
            case 9:
                printf("Nouvelles prescriptions: ");
                fgets(i->prescriptions_post_op, 500, stdin);
                i->prescriptions_post_op[strcspn(i->prescriptions_post_op, "\n")] = '\0';
                break;
            case 10:
                printf("Nouvelle date sortie: ");
                fgets(i->date_sortie, 20, stdin);
                i->date_sortie[strcspn(i->date_sortie, "\n")] = '\0';
                break;
            case 11:
                printf("Nouveau statut (Planifiée/En cours/Terminée/Reportée): ");
                fgets(i->statut, 30, stdin);
                i->statut[strcspn(i->statut, "\n")] = '\0';
                break;
            case 12:
                printf("Nouveau cout: ");
                scanf("%f", &i->cout_estime);
                viderBuffer();
                break;
            case 13:
                printf("Nouvelles notes: ");
                fgets(i->notes, 500, stdin);
                i->notes[strcspn(i->notes, "\n")] = '\0';
                break;
            case 14:
                sauvegarderInterventions();
                printf("\nModifications sauvegardees!\n");
                return;
            case 15:
                return;
        }
    } while(1);
}

void commencerIntervention(int index)
{
    if (index < 0 || index >= nombreInterventions)
    {
        printf("Erreur: Intervention non trouvee.\n");
        return;
    }

    Intervention *i = &interventions[index];

    if (strcmp(i->statut, "Planifiée") != 0)
    {
        printf("Cette intervention ne peut pas etre commencee (statut: %s).\n", i->statut);
        pause();
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(i->heure_debut, "%02d:%02d", tm.tm_hour, tm.tm_min);

    strcpy(i->statut, "En cours");
    sauvegarderInterventions();

    printf("Intervention #%d commencer a %s.\n", i->id_intervention, i->heure_debut);
    pause();
}

void terminerIntervention(int index)
{
    if (index < 0 || index >= nombreInterventions)
    {
        printf("Erreur: Intervention non trouvee.\n");
        return;
    }

    Intervention *i = &interventions[index];

    if (strcmp(i->statut, "En cours") != 0)
    {
        printf("Cette intervention ne peut pas etre terminee (statut: %s).\n", i->statut);
        pause();
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(i->heure_fin, "%02d:%02d", tm.tm_hour, tm.tm_min);

    printf("Compte rendu operatoire: ");
    fgets(i->compte_rendu_operatoire, 1000, stdin);
    i->compte_rendu_operatoire[strcspn(i->compte_rendu_operatoire, "\n")] = '\0';

    printf("Prescriptions post-operatoires: ");
    fgets(i->prescriptions_post_op, 500, stdin);
    i->prescriptions_post_op[strcspn(i->prescriptions_post_op, "\n")] = '\0';

    strcpy(i->statut, "Terminée");
    sauvegarderInterventions();

    printf("Intervention #%d terminee a %s.\n", i->id_intervention, i->heure_fin);
    pause();
}

void reporterIntervention(int index)
{
    if (index < 0 || index >= nombreInterventions)
    {
        printf("Erreur: Intervention non trouvee.\n");
        return;
    }

    Intervention *i = &interventions[index];

    printf("Nouvelle date (JJ/MM/AAAA): ");
    fgets(i->date_intervention, 20, stdin);
    i->date_intervention[strcspn(i->date_intervention, "\n")] = '\0';

    printf("Nouvelle heure (HH:MM): ");
    fgets(i->heure_debut, 10, stdin);
    i->heure_debut[strcspn(i->heure_debut, "\n")] = '\0';

    printf("Motif du report: ");
    char motif[200];
    fgets(motif, 200, stdin);
    motif[strcspn(motif, "\n")] = '\0';

    strcat(i->notes, " [Report: ");
    strcat(i->notes, motif);
    strcat(i->notes, "]");

    strcpy(i->statut, "Reportée");
    sauvegarderInterventions();

    printf("Intervention reportee au %s a %s.\n", i->date_intervention, i->heure_debut);
    pause();
}

void ajouterCompteRendu(int index)
{
    if (index < 0 || index >= nombreInterventions)
    {
        printf("Erreur: Intervention non trouvee.\n");
        return;
    }

    Intervention *i = &interventions[index];

    printf("Compte rendu operatoire:\n");
    fgets(i->compte_rendu_operatoire, 1000, stdin);
    i->compte_rendu_operatoire[strcspn(i->compte_rendu_operatoire, "\n")] = '\0';

    sauvegarderInterventions();
    printf("Compte rendu ajoute avec succes.\n");
    pause();
}

// ============================================
// FONCTIONS POUR LES PRISES EN CHARGE
// ============================================

void demarrerPriseEnCharge()
{
    system("cls");
    printf("\n=== DEMARRER UNE PRISE EN CHARGE ===\n\n");

    if (nombrePatients == 0)
    {
        printf("ERREUR: Aucun patient enregistre!\n");
        pause();
        return;
    }

    PriseEnCharge p;
    memset(&p, 0, sizeof(PriseEnCharge));

    p.id_prise_en_charge = ++dernierIDPriseEnCharge;
    p.nb_medecins = 0;
    p.nb_infirmiers = 0;

    afficherListePatientsSimple();
    printf("\nID du patient: ");
    scanf("%d", &p.id_patient);
    viderBuffer();

    int index_patient = rechercherParID(p.id_patient);
    if (index_patient == -1)
    {
        printf("Patient non trouve!\n");
        dernierIDPriseEnCharge--;
        pause();
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(p.date_debut, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    printf("Type de prise en charge (Hospitalisation/Soins intensifs/Reanimation/Soins palliatifs): ");
    fgets(p.type_pec, 50, stdin);
    p.type_pec[strcspn(p.type_pec, "\n")] = '\0';

    printf("Service: ");
    fgets(p.service, 50, stdin);
    p.service[strcspn(p.service, "\n")] = '\0';

    printf("Motif: ");
    fgets(p.motif, 200, stdin);
    p.motif[strcspn(p.motif, "\n")] = '\0';

    printf("Date de sortie prevue (JJ/MM/AAAA): ");
    fgets(p.date_sortie_prevue, 20, stdin);
    p.date_sortie_prevue[strcspn(p.date_sortie_prevue, "\n")] = '\0';

    printf("Cout journalier: ");
    scanf("%f", &p.cout_journalier);
    viderBuffer();

    // Ajout des médecins
    printf("\nAJOUT DES MEDECINS (0 pour terminer)\n");
    while (p.nb_medecins < MAX_EQUIPE)
    {
        printf("\nID medecin %d (0 pour terminer): ", p.nb_medecins + 1);
        int id_med;
        scanf("%d", &id_med);
        viderBuffer();

        if (id_med == 0) break;

        int index_med = rechercherEmployeParID(id_med);
        if (index_med != -1 && personnel[index_med].actif)
        {
            p.ids_medecins[p.nb_medecins] = id_med;
            p.nb_medecins++;
        }
        else
        {
            printf("Medecin non trouve ou inactif!\n");
        }
    }

    // Ajout des infirmiers
    printf("\nAJOUT DES INFIRMIERS (0 pour terminer)\n");
    while (p.nb_infirmiers < MAX_EQUIPE)
    {
        printf("\nID infirmier %d (0 pour terminer): ", p.nb_infirmiers + 1);
        int id_inf;
        scanf("%d", &id_inf);
        viderBuffer();

        if (id_inf == 0) break;

        int index_inf = rechercherEmployeParID(id_inf);
        if (index_inf != -1 && personnel[index_inf].actif)
        {
            p.ids_infirmiers[p.nb_infirmiers] = id_inf;
            p.nb_infirmiers++;
        }
        else
        {
            printf("Infirmier non trouve ou inactif!\n");
        }
    }

    printf("\nTraitements administres: ");
    fgets(p.traitements_administres, 500, stdin);
    p.traitements_administres[strcspn(p.traitements_administres, "\n")] = '\0';

    printf("Examens realises: ");
    fgets(p.examens_realises, 300, stdin);
    p.examens_realises[strcspn(p.examens_realises, "\n")] = '\0';

    printf("Notes: ");
    fgets(p.notes, 500, stdin);
    p.notes[strcspn(p.notes, "\n")] = '\0';

    strcpy(p.statut, "En cours");

    prisesEnCharge[nombrePrisesEnCharge] = p;
    nombrePrisesEnCharge++;

    sauvegarderPrisesEnCharge();

    printf("\n=== PRISE EN CHARGE DEMARREE AVEC SUCCES ===\n");
    printf("ID Prise en charge: %d\n", p.id_prise_en_charge);
    printf("Patient: %s %s\n", patients[index_patient].prenom, patients[index_patient].nom);
    printf("Type: %s\n", p.type_pec);
    printf("Service: %s\n", p.service);

    pause();
}

void afficherPriseEnCharge(int index)
{
    if (index < 0 || index >= nombrePrisesEnCharge)
    {
        printf("Erreur: Prise en charge non trouvee.\n");
        return;
    }

    PriseEnCharge p = prisesEnCharge[index];

    printf("\n========================================\n");
    printf("PRISE EN CHARGE #%d\n", p.id_prise_en_charge);
    printf("========================================\n");

    int idx_patient = rechercherParID(p.id_patient);

    printf("\nPATIENT:\n");
    if (idx_patient != -1)
        printf("  %s %s (ID: %d)\n", patients[idx_patient].prenom, patients[idx_patient].nom, p.id_patient);
    else
        printf("  ID Patient: %d\n", p.id_patient);

    printf("\nINFORMATIONS:\n");
    printf("  Date debut: %s\n", p.date_debut);
    printf("  Type: %s\n", p.type_pec);
    printf("  Service: %s\n", p.service);
    printf("  Motif: %s\n", p.motif);
    printf("  Statut: %s\n", p.statut);

    if (p.nb_medecins > 0)
    {
        printf("\nMEDECINS:\n");
        for (int i = 0; i < p.nb_medecins; i++)
        {
            int idx_med = rechercherEmployeParID(p.ids_medecins[i]);
            if (idx_med != -1)
                printf("  - Dr %s %s\n", personnel[idx_med].prenom, personnel[idx_med].nom);
            else
                printf("  - ID: %d\n", p.ids_medecins[i]);
        }
    }

    if (p.nb_infirmiers > 0)
    {
        printf("\nINFIRMIERS:\n");
        for (int i = 0; i < p.nb_infirmiers; i++)
        {
            int idx_inf = rechercherEmployeParID(p.ids_infirmiers[i]);
            if (idx_inf != -1)
                printf("  - %s %s\n", personnel[idx_inf].prenom, personnel[idx_inf].nom);
            else
                printf("  - ID: %d\n", p.ids_infirmiers[i]);
        }
    }

    printf("\nTRAITEMENTS:\n");
    printf("  %s\n", p.traitements_administres);

    printf("\nEXAMENS:\n");
    printf("  %s\n", p.examens_realises);

    printf("\nEVOLUTION:\n");
    printf("  %s\n", p.evolution);

    printf("\nSORTIE:\n");
    printf("  Prevue: %s\n", p.date_sortie_prevue);
    if (strlen(p.date_sortie_reelle) > 0)
        printf("  Reelle: %s\n", p.date_sortie_reelle);

    if (strlen(p.notes) > 0)
    {
        printf("\nNOTES:\n");
        printf("  %s\n", p.notes);
    }

    printf("\nCOUT JOURNALIER: %.2f €\n", p.cout_journalier);
    printf("========================================\n");
}

void afficherPrisesEnChargePatient(int id_patient)
{
    system("cls");
    printf("\n=== PRISES EN CHARGE DU PATIENT ID: %d ===\n\n", id_patient);

    int compteur = 0;
    for (int i = 0; i < nombrePrisesEnCharge; i++)
    {
        if (prisesEnCharge[i].id_patient == id_patient)
        {
            compteur++;
            printf("\n[%d] Prise en charge #%d\n", compteur, prisesEnCharge[i].id_prise_en_charge);
            printf("  Debut: %s\n", prisesEnCharge[i].date_debut);
            printf("  Type: %s\n", prisesEnCharge[i].type_pec);
            printf("  Service: %s\n", prisesEnCharge[i].service);
            printf("  Statut: %s\n", prisesEnCharge[i].statut);
        }
    }

    if (compteur == 0)
    {
        printf("Aucune prise en charge pour ce patient.\n");
    }

    pause();
}

void modifierPriseEnCharge(int index)
{
    if (index < 0 || index >= nombrePrisesEnCharge)
    {
        printf("Erreur: Prise en charge non trouvee.\n");
        return;
    }

    PriseEnCharge *p = &prisesEnCharge[index];
    int choix;

    do
    {
        system("cls");
        printf("\n=== MODIFICATION PRISE EN CHARGE #%d ===\n", p->id_prise_en_charge);

        printf("1. Type pec (%s)\n", p->type_pec);
        printf("2. Service (%s)\n", p->service);
        printf("3. Motif (%s)\n", p->motif);
        printf("4. Evolution\n");
        printf("5. Traitements\n");
        printf("6. Examens\n");
        printf("7. Date sortie prevue (%s)\n", p->date_sortie_prevue);
        printf("8. Date sortie reelle (%s)\n", p->date_sortie_reelle);
        printf("9. Cout journalier (%.2f)\n", p->cout_journalier);
        printf("10. Statut (%s)\n", p->statut);
        printf("11. Notes\n");
        printf("12. Sauvegarder et quitter\n");
        printf("13. Annuler\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                printf("Nouveau type: ");
                fgets(p->type_pec, 50, stdin);
                p->type_pec[strcspn(p->type_pec, "\n")] = '\0';
                break;
            case 2:
                printf("Nouveau service: ");
                fgets(p->service, 50, stdin);
                p->service[strcspn(p->service, "\n")] = '\0';
                break;
            case 3:
                printf("Nouveau motif: ");
                fgets(p->motif, 200, stdin);
                p->motif[strcspn(p->motif, "\n")] = '\0';
                break;
            case 4:
                printf("Evolution: ");
                fgets(p->evolution, 500, stdin);
                p->evolution[strcspn(p->evolution, "\n")] = '\0';
                break;
            case 5:
                printf("Traitements: ");
                fgets(p->traitements_administres, 500, stdin);
                p->traitements_administres[strcspn(p->traitements_administres, "\n")] = '\0';
                break;
            case 6:
                printf("Examens: ");
                fgets(p->examens_realises, 300, stdin);
                p->examens_realises[strcspn(p->examens_realises, "\n")] = '\0';
                break;
            case 7:
                printf("Nouvelle date sortie prevue: ");
                fgets(p->date_sortie_prevue, 20, stdin);
                p->date_sortie_prevue[strcspn(p->date_sortie_prevue, "\n")] = '\0';
                break;
            case 8:
                printf("Date sortie reelle: ");
                fgets(p->date_sortie_reelle, 20, stdin);
                p->date_sortie_reelle[strcspn(p->date_sortie_reelle, "\n")] = '\0';
                break;
            case 9:
                printf("Nouveau cout journalier: ");
                scanf("%f", &p->cout_journalier);
                viderBuffer();
                break;
            case 10:
                printf("Nouveau statut (En cours/Terminee/Transferee): ");
                fgets(p->statut, 30, stdin);
                p->statut[strcspn(p->statut, "\n")] = '\0';
                break;
            case 11:
                printf("Nouvelles notes: ");
                fgets(p->notes, 500, stdin);
                p->notes[strcspn(p->notes, "\n")] = '\0';
                break;
            case 12:
                sauvegarderPrisesEnCharge();
                printf("\nModifications sauvegardees!\n");
                return;
            case 13:
                return;
        }
    } while(1);
}

void terminerPriseEnCharge(int index)
{
    if (index < 0 || index >= nombrePrisesEnCharge)
    {
        printf("Erreur: Prise en charge non trouvee.\n");
        return;
    }

    PriseEnCharge *p = &prisesEnCharge[index];

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(p->date_sortie_reelle, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    printf("Evolution finale: ");
    fgets(p->evolution, 500, stdin);
    p->evolution[strcspn(p->evolution, "\n")] = '\0';

    strcpy(p->statut, "Terminée");
    sauvegarderPrisesEnCharge();

    printf("Prise en charge terminee le %s.\n", p->date_sortie_reelle);
    pause();
}

// ============================================
// MENUS
// ============================================

void menuInterventions()
{
    int choix;

    do
    {
        system("cls");
        color(9, 0); // Bleu clair
        printf("\n=== GESTION DES INTERVENTIONS CHIRURGICALES ===\n\n");
        color(7, 0);

        printf("1. Planifier une intervention\n");
        printf("2. Enregistrer une intervention effectuee\n");
        printf("3. Afficher les interventions d'un patient\n");
        printf("4. Afficher les interventions d'un chirurgien\n");
        printf("5. Consulter une intervention\n");
        printf("6. Modifier une intervention\n");
        printf("7. Commencer une intervention\n");
        printf("8. Terminer une intervention\n");
        printf("9. Reporter une intervention\n");
        printf("10. Ajouter compte rendu\n");
        printf("11. Gestion des prises en charge\n");
        printf("12. Retour\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                planifierIntervention();
                break;
            case 2:
                ajouterIntervention();
                break;
            case 3:
            {
                if (nombreInterventions == 0)
                {
                    printf("Aucune intervention.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Patient: ");
                scanf("%d", &id);
                viderBuffer();
                afficherInterventionsPatient(id);
                break;
            }
            case 4:
            {
                if (nombreInterventions == 0)
                {
                    printf("Aucune intervention.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Chirurgien: ");
                scanf("%d", &id);
                viderBuffer();
                afficherInterventionsChirurgien(id);
                break;
            }
            case 5:
            {
                if (nombreInterventions == 0)
                {
                    printf("Aucune intervention.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Intervention: ");
                scanf("%d", &id);
                viderBuffer();

                int index = -1;
                for (int i = 0; i < nombreInterventions; i++)
                {
                    if (interventions[i].id_intervention == id)
                    {
                        index = i;
                        break;
                    }
                }

                if (index != -1)
                {
                    afficherIntervention(index);
                    pause();
                }
                else
                {
                    printf("Intervention non trouvee.\n");
                    pause();
                }
                break;
            }
            case 6:
            {
                if (nombreInterventions == 0)
                {
                    printf("Aucune intervention.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Intervention: ");
                scanf("%d", &id);
                viderBuffer();

                int index = -1;
                for (int i = 0; i < nombreInterventions; i++)
                {
                    if (interventions[i].id_intervention == id)
                    {
                        index = i;
                        break;
                    }
                }

                if (index != -1)
                {
                    modifierIntervention(index);
                }
                else
                {
                    printf("Intervention non trouvee.\n");
                    pause();
                }
                break;
            }
            case 7:
            {
                if (nombreInterventions == 0)
                {
                    printf("Aucune intervention.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Intervention: ");
                scanf("%d", &id);
                viderBuffer();

                int index = -1;
                for (int i = 0; i < nombreInterventions; i++)
                {
                    if (interventions[i].id_intervention == id)
                    {
                        index = i;
                        break;
                    }
                }

                if (index != -1)
                {
                    commencerIntervention(index);
                }
                else
                {
                    printf("Intervention non trouvee.\n");
                    pause();
                }
                break;
            }
            case 8:
            {
                if (nombreInterventions == 0)
                {
                    printf("Aucune intervention.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Intervention: ");
                scanf("%d", &id);
                viderBuffer();

                int index = -1;
                for (int i = 0; i < nombreInterventions; i++)
                {
                    if (interventions[i].id_intervention == id)
                    {
                        index = i;
                        break;
                    }
                }

                if (index != -1)
                {
                    terminerIntervention(index);
                }
                else
                {
                    printf("Intervention non trouvee.\n");
                    pause();
                }
                break;
            }
            case 9:
            {
                if (nombreInterventions == 0)
                {
                    printf("Aucune intervention.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Intervention: ");
                scanf("%d", &id);
                viderBuffer();

                int index = -1;
                for (int i = 0; i < nombreInterventions; i++)
                {
                    if (interventions[i].id_intervention == id)
                    {
                        index = i;
                        break;
                    }
                }

                if (index != -1)
                {
                    reporterIntervention(index);
                }
                else
                {
                    printf("Intervention non trouvee.\n");
                    pause();
                }
                break;
            }
            case 10:
            {
                if (nombreInterventions == 0)
                {
                    printf("Aucune intervention.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Intervention: ");
                scanf("%d", &id);
                viderBuffer();

                int index = -1;
                for (int i = 0; i < nombreInterventions; i++)
                {
                    if (interventions[i].id_intervention == id)
                    {
                        index = i;
                        break;
                    }
                }

                if (index != -1)
                {
                    ajouterCompteRendu(index);
                }
                else
                {
                    printf("Intervention non trouvee.\n");
                    pause();
                }
                break;
            }
            case 11:
                menuPrisesEnCharge();
                break;
            case 12:
                return;
            default:
                printf("Choix invalide.\n");
                pause();
        }
    } while(choix != 12);
}

void menuPrisesEnCharge()
{
    int choix;

    do
    {
        system("cls");
        color(13, 0); // Magenta
        printf("\n=== GESTION DES PRISES EN CHARGE ===\n\n");
        color(7, 0);

        printf("1. Demarrer une prise en charge\n");
        printf("2. Afficher les prises en charge d'un patient\n");
        printf("3. Consulter une prise en charge\n");
        printf("4. Modifier une prise en charge\n");
        printf("5. Terminer une prise en charge\n");
        printf("6. Retour\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                demarrerPriseEnCharge();
                break;
            case 2:
            {
                if (nombrePrisesEnCharge == 0)
                {
                    printf("Aucune prise en charge.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Patient: ");
                scanf("%d", &id);
                viderBuffer();
                afficherPrisesEnChargePatient(id);
                break;
            }
            case 3:
            {
                if (nombrePrisesEnCharge == 0)
                {
                    printf("Aucune prise en charge.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Prise en charge: ");
                scanf("%d", &id);
                viderBuffer();

                int index = -1;
                for (int i = 0; i < nombrePrisesEnCharge; i++)
                {
                    if (prisesEnCharge[i].id_prise_en_charge == id)
                    {
                        index = i;
                        break;
                    }
                }

                if (index != -1)
                {
                    afficherPriseEnCharge(index);
                    pause();
                }
                else
                {
                    printf("Prise en charge non trouvee.\n");
                    pause();
                }
                break;
            }
            case 4:
            {
                if (nombrePrisesEnCharge == 0)
                {
                    printf("Aucune prise en charge.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Prise en charge: ");
                scanf("%d", &id);
                viderBuffer();

                int index = -1;
                for (int i = 0; i < nombrePrisesEnCharge; i++)
                {
                    if (prisesEnCharge[i].id_prise_en_charge == id)
                    {
                        index = i;
                        break;
                    }
                }

                if (index != -1)
                {
                    modifierPriseEnCharge(index);
                }
                else
                {
                    printf("Prise en charge non trouvee.\n");
                    pause();
                }
                break;
            }
            case 5:
            {
                if (nombrePrisesEnCharge == 0)
                {
                    printf("Aucune prise en charge.\n");
                    pause();
                    break;
                }
                int id;
                printf("ID Prise en charge: ");
                scanf("%d", &id);
                viderBuffer();

                int index = -1;
                for (int i = 0; i < nombrePrisesEnCharge; i++)
                {
                    if (prisesEnCharge[i].id_prise_en_charge == id)
                    {
                        index = i;
                        break;
                    }
                }

                if (index != -1)
                {
                    terminerPriseEnCharge(index);
                }
                else
                {
                    printf("Prise en charge non trouvee.\n");
                    pause();
                }
                break;
            }
            case 6:
                return;
            default:
                printf("Choix invalide.\n");
                pause();
        }
    } while(choix != 6);
}
