#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "infirmier.h"
#include "utils.h"
#include "patient.h"
#include "personnel.h"
#include "consultations.h"
#include "comptes.h"

// Définition des variables globales
ConstanteVital constantes[MAX_CONSTANTES];
int nombreConstantes = 0;
int dernierIDConstante = 0;
Soin soins[MAX_SOINS];
int nombreSoins = 0;
int dernierIDSoin = 0;
PlanningSoin plannings[MAX_SOINS];
int nombrePlannings = 0;
int dernierIDPlanning = 0;

void initialiserInfirmier()
{
    chargerConstantes();
    chargerSoins();
    chargerPlannings();
}

// ================= CONSTANTES VITALES =================

void saisirConstantesVitales(int id_patient)
{
    system("cls");
    printf("\n=== SAISIE DES CONSTANTES VITALES ===\n\n");

    int idx_patient = rechercherParID(id_patient);
    if (idx_patient == -1)
    {
        printf("Patient non trouve.\n");
        pause();
        return;
    }

    printf("Patient: %s %s\n\n", patients[idx_patient].prenom, patients[idx_patient].nom);

    ConstanteVital c;
    memset(&c, 0, sizeof(ConstanteVital));

    c.id_constante = ++dernierIDConstante;
    c.id_patient = id_patient;
    c.id_infirmier = utilisateur_actuel ? utilisateur_actuel->id_associe : 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(c.date_mesure, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    sprintf(c.heure_mesure, "%02d:%02d", tm.tm_hour, tm.tm_min);

    printf("--- MESURES ---\n");
    printf("Temperature (°C): ");
    scanf("%f", &c.temperature);
    viderBuffer();

    printf("Tension arterielle (systolique/diastolique):\n");
    printf("  Systolique (mmHg): ");
    scanf("%d", &c.tension_systolique);
    printf("  Diastolique (mmHg): ");
    scanf("%d", &c.tension_diastolique);
    viderBuffer();

    printf("Pouls (battements/min): ");
    scanf("%d", &c.pouls);
    viderBuffer();

    printf("Saturation en oxygene (%%): ");
    scanf("%d", &c.saturation);
    viderBuffer();

    printf("Poids (kg): ");
    scanf("%f", &c.poids);
    viderBuffer();

    printf("Taille (cm): ");
    scanf("%f", &c.taille);
    viderBuffer();

    printf("Glycemie (g/L): ");
    scanf("%f", &c.glycemie);
    viderBuffer();

    printf("\nNotes: ");
    fgets(c.notes, 200, stdin);
    c.notes[strcspn(c.notes, "\n")] = '\0';

    constantes[nombreConstantes] = c;
    nombreConstantes++;

    sauvegarderConstantes();

    printf("\n=== CONSTANTES ENREGISTREES ===\n");
    printf("Temperature: %.1f °C\n", c.temperature);
    printf("Tension: %d/%d mmHg\n", c.tension_systolique, c.tension_diastolique);
    printf("Pouls: %d bpm\n", c.pouls);

    if (c.temperature > 38.5)
        color(12, 0), printf("\n⚠️ ALERTE: Fievre elevee!\n"), color(7,0);
    if (c.tension_systolique > 160 || c.tension_diastolique > 100)
        color(12, 0), printf("\n⚠️ ALERTE: Hypertension!\n"), color(7,0);
    if (c.pouls > 120)
        color(12, 0), printf("\n⚠️ ALERTE: Tachycardie!\n"), color(7,0);
    if (c.saturation < 90)
        color(12, 0), printf("\n⚠️ ALERTE: Desaturation!\n"), color(7,0);

    pause();
}

void afficherHistoriqueConstantes(int id_patient)
{
    system("cls");
    printf("\n=== HISTORIQUE DES CONSTANTES ===\n");
    printf("Patient ID: %d\n\n", id_patient);

    int compteur = 0;
    printf("ID  | Date       | Heure  | Temp | Tension | Pouls | SpO2\n");
    printf("----|------------|--------|------|---------|-------|------\n");

    for (int i = 0; i < nombreConstantes; i++)
    {
        if (constantes[i].id_patient == id_patient)
        {
            compteur++;
            printf("%-4d| %-10s | %-6s | %4.1f | %3d/%-3d | %5d | %4d\n",
                   constantes[i].id_constante,
                   constantes[i].date_mesure,
                   constantes[i].heure_mesure,
                   constantes[i].temperature,
                   constantes[i].tension_systolique,
                   constantes[i].tension_diastolique,
                   constantes[i].pouls,
                   constantes[i].saturation);
        }
    }

    if (compteur == 0)
    {
        printf("Aucune constante enregistree.\n");
    }
    else
    {
        printf("\nTotal: %d mesure(s)\n", compteur);
        printf("\nEntrez l'ID pour voir les details (0 pour quitter): ");
        int id;
        scanf("%d", &id);
        viderBuffer();

        for (int i = 0; i < nombreConstantes; i++)
        {
            if (constantes[i].id_constante == id)
            {
                printf("\n========================================\n");
                printf("DETAILS CONSTANTES #%d\n", constantes[i].id_constante);
                printf("========================================\n");
                printf("Date: %s a %s\n", constantes[i].date_mesure, constantes[i].heure_mesure);
                printf("Temperature: %.1f °C\n", constantes[i].temperature);
                printf("Tension: %d/%d mmHg\n", constantes[i].tension_systolique, constantes[i].tension_diastolique);
                printf("Pouls: %d bpm\n", constantes[i].pouls);
                printf("Saturation: %d %%\n", constantes[i].saturation);
                printf("Poids: %.1f kg\n", constantes[i].poids);
                printf("Taille: %.1f cm\n", constantes[i].taille);
                printf("Glycemie: %.2f g/L\n", constantes[i].glycemie);
                if (strlen(constantes[i].notes) > 0)
                    printf("Notes: %s\n", constantes[i].notes);
                printf("========================================\n");
                break;
            }
        }
    }

    pause();
}

// ================= PRESCRIPTION DE SOINS (MÉDECIN) =================

void prescrireSoin(int id_patient, int id_medecin)
{
    system("cls");
    printf("\n=== PRESCRIPTION DE SOINS ===\n\n");

    int idx_patient = rechercherParID(id_patient);
    if (idx_patient == -1)
    {
        printf("Patient non trouve.\n");
        pause();
        return;
    }

    printf("Patient: %s %s\n\n", patients[idx_patient].prenom, patients[idx_patient].nom);

    Soin s;
    memset(&s, 0, sizeof(Soin));

    s.id_soin = ++dernierIDSoin;
    s.id_patient = id_patient;
    s.id_medecin_prescripteur = id_medecin;
    s.id_infirmier_realisateur = 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(s.date_prescription, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    printf("Type de soin (Pansement/Injection/Prelevement/Perfusion/Aide/Toilette): ");
    fgets(s.type_soin, 100, stdin);
    s.type_soin[strcspn(s.type_soin, "\n")] = '\0';

    printf("Description du soin: ");
    fgets(s.description, 300, stdin);
    s.description[strcspn(s.description, "\n")] = '\0';

    printf("Instructions pour l'infirmier: ");
    fgets(s.instructions, 300, stdin);
    s.instructions[strcspn(s.instructions, "\n")] = '\0';

    printf("Frequence (ex: Matin/Soir, Une seule fois): ");
    fgets(s.frequence, 100, stdin);
    s.frequence[strcspn(s.frequence, "\n")] = '\0';

    printf("Priorite (Normale/Urgente/Tres urgente): ");
    fgets(s.priorite, 20, stdin);
    s.priorite[strcspn(s.priorite, "\n")] = '\0';

    printf("Notes: ");
    fgets(s.notes, 300, stdin);
    s.notes[strcspn(s.notes, "\n")] = '\0';

    strcpy(s.statut, "Prescrit");

    soins[nombreSoins] = s;
    nombreSoins++;

    sauvegarderSoins();

    printf("\n=== SOIN PRESCRIT AVEC SUCCES ===\n");
    printf("ID Soin: %d\n", s.id_soin);
    printf("Type: %s\n", s.type_soin);
    printf("Priorite: %s\n", s.priorite);
    printf("\n✅ Le soin a ete ajoute a la liste des soins a realiser par l'infirmier.\n");

    pause();
}

void afficherPrescriptionsMedecin(int id_medecin)
{
    system("cls");
    printf("\n=== MES PRESCRIPTIONS DE SOINS ===\n\n");

    int compteur = 0;
    printf("ID  | Patient                 | Type                | Date presc. | Statut\n");
    printf("----|-------------------------|---------------------|-------------|-----------\n");

    for (int i = 0; i < nombreSoins; i++)
    {
        if (soins[i].id_medecin_prescripteur == id_medecin)
        {
            compteur++;
            int idx_patient = rechercherParID(soins[i].id_patient);
            char patient_nom[30] = "";
            if (idx_patient != -1)
                sprintf(patient_nom, "%s %s", patients[idx_patient].prenom, patients[idx_patient].nom);
            else
                sprintf(patient_nom, "ID %d", soins[i].id_patient);

            printf("%-4d| %-23s | %-19s | %-11s | %s\n",
                   soins[i].id_soin,
                   patient_nom,
                   soins[i].type_soin,
                   soins[i].date_prescription,
                   soins[i].statut);
        }
    }

    if (compteur == 0)
    {
        printf("Aucune prescription de soin.\n");
    }

    pause();
}

// ================= SOINS À REALISER (INFIRMIER) =================

void afficherSoinsARealiser()
{

    // Forcer le rechargement des donnees
    chargerSoins();

    system("cls");
    printf("\n=== SOINS A REALISER ===\n\n");

    // Vérifier s'il y a des soins enregistrés
    if (nombreSoins == 0)
    {
        printf("Aucun soin enregistre dans le systeme.\n");
        printf("Les soins apparaissent ici apres prescription par un medecin.\n");
        pause();
        return;
    }

    int compteur = 0;
    printf("ID  | Patient                 | Type                | Prescrit par | Priorite\n");
    printf("----|-------------------------|---------------------|--------------|------------\n");

    for (int i = 0; i < nombreSoins; i++)
    {
        // Afficher les soins avec statut "Prescrit" (et pas encore realises)
        if (strcmp(soins[i].statut, "Prescrit") == 0)
        {
            compteur++;

            // Récupérer le nom du patient
            int idx_patient = rechercherParID(soins[i].id_patient);
            char patient_nom[30] = "";
            if (idx_patient != -1)
                sprintf(patient_nom, "%s %s", patients[idx_patient].prenom, patients[idx_patient].nom);
            else
                sprintf(patient_nom, "ID %d", soins[i].id_patient);

            // Récupérer le nom du médecin prescripteur
            int idx_medecin = rechercherEmployeParID(soins[i].id_medecin_prescripteur);
            char medecin_nom[30] = "";
            if (idx_medecin != -1)
                sprintf(medecin_nom, "Dr %s", personnel[idx_medecin].nom);
            else
                sprintf(medecin_nom, "ID %d", soins[i].id_medecin_prescripteur);

            // Couleur selon priorite
            if (strcmp(soins[i].priorite, "Tres urgente") == 0)
                color(12, 0);  // Rouge
            else if (strcmp(soins[i].priorite, "Urgente") == 0)
                color(14, 0);  // Jaune
            else
                color(7, 0);   // Blanc

            printf("%-4d| %-23s | %-19s | %-12s | %s\n",
                   soins[i].id_soin,
                   patient_nom,
                   soins[i].type_soin,
                   medecin_nom,
                   soins[i].priorite);
            color(7, 0);
        }
    }

    if (compteur == 0)
    {
        printf("Aucun soin a realiser pour le moment.\n");
        printf("Les soins apparaissent ici apres prescription par un medecin.\n");

        // DEBUG: Afficher tous les soins existants pour diagnostic
        printf("\n=== INFORMATION DE DEBUG ===\n");
        printf("Nombre total de soins dans le systeme: %d\n", nombreSoins);
        for (int i = 0; i < nombreSoins; i++)
        {
            printf("Soin #%d: Patient=%d, Type=%s, Statut='%s', Prescrit par=%d\n",
                   soins[i].id_soin,
                   soins[i].id_patient,
                   soins[i].type_soin,
                   soins[i].statut,
                   soins[i].id_medecin_prescripteur);
        }
        printf("===============================\n");
    }
    else
    {
        printf("\nTotal: %d soin(s) a realiser\n", compteur);

        printf("\nEntrez l'ID du soin a realiser (0 pour quitter): ");
        int id;
        scanf("%d", &id);
        viderBuffer();

        if (id > 0)
        {
            // Vérifier que le soin existe et est prescrit
            int trouve = 0;
            for (int i = 0; i < nombreSoins; i++)
            {
                if (soins[i].id_soin == id && strcmp(soins[i].statut, "Prescrit") == 0)
                {
                    trouve = 1;
                    realiserSoin(id);
                    break;
                }
            }
            if (!trouve)
            {
                printf("Soin non trouve ou deja realise.\n");
                pause();
            }
        }
    }

    pause();
}

void realiserSoin(int id_soin)
{
    for (int i = 0; i < nombreSoins; i++)
    {
        if (soins[i].id_soin == id_soin && strcmp(soins[i].statut, "Prescrit") == 0)
        {
            system("cls");
            printf("\n=== REALISATION DU SOIN ===\n\n");

            int idx_patient = rechercherParID(soins[i].id_patient);
            printf("Patient: %s %s\n", patients[idx_patient].prenom, patients[idx_patient].nom);
            printf("Type de soin: %s\n", soins[i].type_soin);
            printf("Description: %s\n", soins[i].description);
            printf("Instructions: %s\n", soins[i].instructions);
            printf("Frequence: %s\n", soins[i].frequence);

            printf("\nConfirmez-vous la realisation de ce soin? (O/N): ");
            char conf;
            scanf("%c", &conf);
            viderBuffer();

            if (toupper(conf) == 'O')
            {
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                sprintf(soins[i].date_realisation, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
                soins[i].id_infirmier_realisateur = utilisateur_actuel ? utilisateur_actuel->id_associe : 0;
                strcpy(soins[i].statut, "Realise");

                sauvegarderSoins();

                printf("\n✅ Soin realise avec succes!\n");
                printf("Le medecin pourra consulter l'historique.\n");
            }
            else
            {
                printf("Realisation annulee.\n");
            }
            pause();
            return;
        }
    }
    printf("Soin non trouve ou deja realise.\n");
    pause();
}

void afficherSoinsRealises()
{
    system("cls");
    printf("\n=== SOINS REALISES ===\n\n");

    int compteur = 0;
    printf("ID  | Patient                 | Type                | Realise le  | Par\n");
    printf("----|-------------------------|---------------------|-------------|-----------\n");

    for (int i = 0; i < nombreSoins; i++)
    {
        if (strcmp(soins[i].statut, "Realise") == 0)
        {
            compteur++;
            int idx_patient = rechercherParID(soins[i].id_patient);
            char patient_nom[30] = "";
            if (idx_patient != -1)
                sprintf(patient_nom, "%s %s", patients[idx_patient].prenom, patients[idx_patient].nom);
            else
                sprintf(patient_nom, "ID %d", soins[i].id_patient);

            int idx_infirmier = rechercherEmployeParID(soins[i].id_infirmier_realisateur);
            char infirmier_nom[30] = "";
            if (idx_infirmier != -1)
                sprintf(infirmier_nom, "%s %s", personnel[idx_infirmier].prenom, personnel[idx_infirmier].nom);
            else
                sprintf(infirmier_nom, "ID %d", soins[i].id_infirmier_realisateur);

            printf("%-4d| %-23s | %-19s | %-11s | %s\n",
                   soins[i].id_soin,
                   patient_nom,
                   soins[i].type_soin,
                   soins[i].date_realisation,
                   infirmier_nom);
        }
    }

    if (compteur == 0)
    {
        printf("Aucun soin realise.\n");
    }

    pause();
}

void afficherSoinsParPatient(int id_patient)
{
    system("cls");
    printf("\n=== SOINS DU PATIENT ===\n\n");

    int compteur = 0;
    printf("ID  | Type                | Prescrit le | Realise le | Par\n");
    printf("----|---------------------|-------------|------------|-----------\n");

    for (int i = 0; i < nombreSoins; i++)
    {
        if (soins[i].id_patient == id_patient)
        {
            compteur++;
            char realise_le[15] = "-";
            if (strlen(soins[i].date_realisation) > 0)
                strcpy(realise_le, soins[i].date_realisation);

            int idx_medecin = rechercherEmployeParID(soins[i].id_medecin_prescripteur);
            char medecin_nom[30] = "";
            if (idx_medecin != -1)
                sprintf(medecin_nom, "Dr %s", personnel[idx_medecin].nom);
            else
                sprintf(medecin_nom, "ID %d", soins[i].id_medecin_prescripteur);

            printf("%-4d| %-19s | %-11s | %-10s | %s\n",
                   soins[i].id_soin,
                   soins[i].type_soin,
                   soins[i].date_prescription,
                   realise_le,
                   medecin_nom);
        }
    }

    if (compteur == 0)
    {
        printf("Aucun soin pour ce patient.\n");
    }
    else
    {
        printf("\nTotal: %d soin(s)\n", compteur);
    }

    pause();
}

void planifierSoin(int id_soin)
{
    for (int i = 0; i < nombreSoins; i++)
    {
        if (soins[i].id_soin == id_soin)
        {
            PlanningSoin p;
            memset(&p, 0, sizeof(PlanningSoin));

            p.id_planning = ++dernierIDPlanning;
            p.id_soin = id_soin;

            printf("\n=== PLANIFICATION DU SOIN ===\n");
            printf("Date (JJ/MM/AAAA): ");
            fgets(p.date_prevue, 20, stdin);
            p.date_prevue[strcspn(p.date_prevue, "\n")] = '\0';

            printf("Heure (HH:MM): ");
            fgets(p.heure_prevue, 10, stdin);
            p.heure_prevue[strcspn(p.heure_prevue, "\n")] = '\0';

            p.realise = 0;

            strcpy(soins[i].statut, "Planifie");

            plannings[nombrePlannings] = p;
            nombrePlannings++;

            sauvegarderSoins();
            sauvegarderPlannings();

            printf("Soin planifie avec succes.\n");
            return;
        }
    }
}

void afficherPlanningInfirmierJour(int id_infirmier, char *date)
{
    system("cls");
    printf("\n=== PLANNING DES SOINS ===\n");
    printf("Date: %s\n\n", date);

    int compteur = 0;
    printf("Heure    | Patient                 | Type de soin        | Statut\n");
    printf("---------|-------------------------|---------------------|-----------\n");

    for (int i = 0; i < nombrePlannings; i++)
    {
        if (strcmp(plannings[i].date_prevue, date) == 0)
        {
            for (int j = 0; j < nombreSoins; j++)
            {
                if (soins[j].id_soin == plannings[i].id_soin)
                {
                    compteur++;
                    int idx_patient = rechercherParID(soins[j].id_patient);
                    char patient_nom[30] = "";
                    if (idx_patient != -1)
                        sprintf(patient_nom, "%s %s", patients[idx_patient].prenom, patients[idx_patient].nom);
                    else
                        sprintf(patient_nom, "ID %d", soins[j].id_patient);

                    char statut[15];
                    if (plannings[i].realise)
                        strcpy(statut, "REALISE");
                    else
                        strcpy(statut, "A REALISER");

                    if (plannings[i].realise)
                        color(10, 0);
                    else
                        color(12, 0);

                    printf("%-7s | %-23s | %-19s | %s\n",
                           plannings[i].heure_prevue,
                           patient_nom,
                           soins[j].type_soin,
                           statut);
                    color(7, 0);
                    break;
                }
            }
        }
    }

    if (compteur == 0)
    {
        printf("Aucun soin planifie pour cette date.\n");
    }

    pause();
}

void menuPlanningInfirmier()
{
    int choix;
    do
    {
        system("cls");
        printf("\n=== PLANNING DES SOINS ===\n\n");
        printf("1. Voir planning du jour\n");
        printf("2. Voir planning d'une date specifique\n");
        printf("3. Retour\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
        case 1:
        {
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            char date[20];
            sprintf(date, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
            afficherPlanningInfirmierJour(utilisateur_actuel ? utilisateur_actuel->id_associe : 0, date);
        }
        break;
        case 2:
        {
            char date[20];
            printf("Date (JJ/MM/AAAA): ");
            fgets(date, 20, stdin);
            date[strcspn(date, "\n")] = '\0';
            afficherPlanningInfirmierJour(utilisateur_actuel ? utilisateur_actuel->id_associe : 0, date);
        }
        break;
        case 3:
            return;
        default:
            printf("Choix invalide.\n");
            pause();
        }
    }
    while(choix != 3);
}

// ================= STATISTIQUES =================

void statistiquesInfirmier()
{
    system("cls");
    printf("\n=== STATISTIQUES INFIRMIER ===\n\n");

    int prescrits = 0, realises = 0, planifies = 0;
    int soins_urgents = 0, soins_tres_urgents = 0;

    for (int i = 0; i < nombreSoins; i++)
    {
        if (strcmp(soins[i].statut, "Prescrit") == 0) prescrits++;
        else if (strcmp(soins[i].statut, "Realise") == 0) realises++;
        else if (strcmp(soins[i].statut, "Planifie") == 0) planifies++;

        if (strcmp(soins[i].priorite, "Urgente") == 0) soins_urgents++;
        else if (strcmp(soins[i].priorite, "Tres urgente") == 0) soins_tres_urgents++;
    }

    printf("Soins prescrits: %d\n", prescrits);
    printf("Soins planifies: %d\n", planifies);
    printf("Soins realises: %d\n", realises);
    printf("Taux de realisation: %.1f%%\n",
           (prescrits + planifies) > 0 ?
           (float)realises / (prescrits + planifies + realises) * 100 : 0);

    printf("\nSoins urgents: %d\n", soins_urgents);
    printf("Soins tres urgents: %d\n", soins_tres_urgents);
    printf("\nMesures de constantes: %d\n", nombreConstantes);

    pause();
}

// ================= MENUS =================

void menuConstantesInfirmier()
{
    int choix;
    do
    {
        system("cls");
        printf("\n=== CONSTANTES VITALES ===\n\n");
        printf("1. Saisir constantes d'un patient\n");
        printf("2. Historique des constantes\n");
        printf("3. Retour\n");
        printf("\nChoix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
        case 1:
        {
            if (nombrePatients == 0)
            {
                printf("Aucun patient.\n");
                pause();
                break;
            }
            afficherListePatientsSimple();
            printf("\nID patient: ");
            int id;
            scanf("%d", &id);
            viderBuffer();
            if (rechercherParID(id) != -1)
                saisirConstantesVitales(id);
            else
                printf("Patient non trouve.\n");
            pause();
        }
        break;
        case 2:
        {
            if (nombrePatients == 0)
            {
                printf("Aucun patient.\n");
                pause();
                break;
            }
            afficherListePatientsSimple();
            printf("\nID patient: ");
            int id;
            scanf("%d", &id);
            viderBuffer();
            afficherHistoriqueConstantes(id);
        }
        break;
        case 3:
            return;
        default:
            printf("Choix invalide.\n");
            pause();
        }
    }
    while(choix != 3);
}

void menuSoinsInfirmier()
{
    int choix;
    do
    {
        system("cls");
        printf("\n=== GESTION DES SOINS ===\n\n");
        printf("1. Soins a realiser\n");
        printf("2. Soins realises\n");
        printf("3. Soins d'un patient\n");
        printf("4. Planning des soins\n");
        printf("5. Retour\n");
        printf("\nChoix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
        case 1:
            afficherSoinsARealiser();
            break;
        case 2:
            afficherSoinsRealises();
            break;
        case 3:
        {
            if (nombrePatients == 0)
            {
                printf("Aucun patient.\n");
                pause();
                break;
            }
            afficherListePatientsSimple();
            printf("\nID patient: ");
            int id;
            scanf("%d", &id);
            viderBuffer();
            afficherSoinsParPatient(id);
        }
        break;
        case 4:
            menuPlanningInfirmier();
            break;
        case 5:
            return;
        default:
            printf("Choix invalide.\n");
            pause();
        }
    }
    while(choix != 5);
}

void menuInfirmier()
{
    int choix;

    printf("DEBUG: INFIRMIER - nombreSoins au chargement = %d\n", nombreSoins);

    do
    {
        system("cls");
        color(11, 0);
        printf("\n========================================\n");
        printf("          MODULE INFIRMIER             \n");
        printf("========================================\n\n");
        color(7, 0);

        printf("1. Constantes vitales\n");
        printf("2. Gestion des soins\n");
        printf("3. Liste des patients\n");
        printf("4. Statistiques\n");
        printf("5. Modifier mot de passe\n");
        printf("6. Deconnexion\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
        case 1:
            menuConstantesInfirmier();
            break;
        case 2:
            menuSoinsInfirmier();
            break;
        case 3:
            afficherListePatientsSimple();
            pause();
            break;
        case 4:
            statistiquesInfirmier();
            break;
        case 5:
            modifierMotDePasse(utilisateur_actuel->id_compte);
            pause();
            break;
        case 6:
            logout();
            return;
        default:
            printf("Choix invalide.\n");
            pause();
        }

    }
    while(choix != 6);
}

// ================= SAUVEGARDE =================

void sauvegarderConstantes()
{
    FILE *f = fopen("constantes.dat", "wb");
    if (f == NULL) return;

    fwrite(&nombreConstantes, sizeof(int), 1, f);
    fwrite(constantes, sizeof(ConstanteVital), nombreConstantes, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_constante.dat", "wb");
    if (fid != NULL)
    {
        fwrite(&dernierIDConstante, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void chargerConstantes()
{
    FILE *f = fopen("constantes.dat", "rb");
    if (f == NULL) return;

    fread(&nombreConstantes, sizeof(int), 1, f);
    fread(constantes, sizeof(ConstanteVital), nombreConstantes, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_constante.dat", "rb");
    if (fid != NULL)
    {
        fread(&dernierIDConstante, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void sauvegarderSoins()
{
    FILE *f = fopen("soins.dat", "wb");
    if (f == NULL) return;

    fwrite(&nombreSoins, sizeof(int), 1, f);
    fwrite(soins, sizeof(Soin), nombreSoins, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_soin.dat", "wb");
    if (fid != NULL)
    {
        fwrite(&dernierIDSoin, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void chargerSoins()
{
    FILE *f = fopen("soins.dat", "rb");
    if (f == NULL){
       printf("DEBUG: Fichier soins.dat non trouve ou vide\n");
       return;
    }

    fread(&nombreSoins, sizeof(int), 1, f);
    fread(soins, sizeof(Soin), nombreSoins, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_soin.dat", "rb");
    if (fid != NULL)
    {
        fread(&dernierIDSoin, sizeof(int), 1, fid);
        fclose(fid);
    }
    printf("DEBUG: Chargement soins - nombreSoins = %d\n", nombreSoins);
}

void sauvegarderPlannings()
{
    FILE *f = fopen("plannings_soins.dat", "wb");
    if (f == NULL) return;

    fwrite(&nombrePlannings, sizeof(int), 1, f);
    fwrite(plannings, sizeof(PlanningSoin), nombrePlannings, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_planning.dat", "wb");
    if (fid != NULL)
    {
        fwrite(&dernierIDPlanning, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void chargerPlannings()
{
    FILE *f = fopen("plannings_soins.dat", "rb");
    if (f == NULL) return;

    fread(&nombrePlannings, sizeof(int), 1, f);
    fread(plannings, sizeof(PlanningSoin), nombrePlannings, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_planning.dat", "rb");
    if (fid != NULL)
    {
        fread(&dernierIDPlanning, sizeof(int), 1, fid);
        fclose(fid);
    }
}
