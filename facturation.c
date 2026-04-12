#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "facturation.h"
#include "utils.h"
#include "patient.h"
#include "personnel.h"
#include "consultations.h"

// Définition des variables globales
Facture factures[MAX_FACTURES];
int nombreFactures = 0;
int dernierIDFacture = 0;

void initialiserFacturation()
{
    chargerFactures();
}

void genererNumeroFacture(int id_facture, char *numero)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(numero, "FAC-%04d-%06d", tm.tm_year + 1900, id_facture);
}

void calculerMontants(Facture *f)
{
    // Calcul du sous-total
    f->sous_total = 0;
    for (int i = 0; i < f->nb_lignes; i++)
    {
        f->lignes[i].montant = f->lignes[i].quantite * f->lignes[i].prix_unitaire;
        f->sous_total += f->lignes[i].montant;
    }

    // Calcul de la remise
    f->montant_remise = f->sous_total * (f->remise / 100);

    // Calcul de la TVA
    float apres_remise = f->sous_total - f->montant_remise;
    f->montant_tva = apres_remise * (f->tva / 100);

    // Calcul du total
    f->montant_total = apres_remise + f->montant_tva;
    f->montant_restant = f->montant_total - f->montant_pris_en_charge;
}

void creerFactureConsultation(int id_consultation)
{
    // Trouver la consultation
    int idx_consultation = -1;
    for (int i = 0; i < nombreConsultations; i++)
    {
        if (consultations[i].id_consultation == id_consultation)
        {
            idx_consultation = i;
            break;
        }
    }

    if (idx_consultation == -1)
    {
        printf("Consultation non trouvee.\n");
        pause();
        return;
    }

    Facture f;
    memset(&f, 0, sizeof(Facture));

    f.id_facture = ++dernierIDFacture;
    f.id_patient = consultations[idx_consultation].id_patient;
    f.id_medecin = consultations[idx_consultation].id_medecin;
    f.id_consultation = id_consultation;
    f.nb_lignes = 0;
    f.remise = 0;
    f.tva = 18;  // TVA par défaut 18%
    f.montant_pris_en_charge = 0;

    // Générer le numéro de facture
    genererNumeroFacture(f.id_facture, f.numero_facture);

    // Date d'émission
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(f.date_emission, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    sprintf(f.date_creation, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    // Date d'échéance (30 jours après)
    tm.tm_mday += 30;
    mktime(&tm);
    sprintf(f.date_echeance, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    // Ajouter la ligne consultation
    strcpy(f.lignes[0].description, "Consultation medicale");
    f.lignes[0].quantite = 1;
    f.lignes[0].prix_unitaire = consultations[idx_consultation].cout;
    f.nb_lignes = 1;

    strcpy(f.statut, "En attente");

    calculerMontants(&f);

    factures[nombreFactures] = f;
    nombreFactures++;

    sauvegarderFactures();

    printf("\n=== FACTURE CREE AVEC SUCCES ===\n");
    printf("Numero: %s\n", f.numero_facture);
    printf("Montant total: %.2f FCFA\n", f.montant_total);
    printf("A payer: %.2f FCFA\n", f.montant_restant);
    printf("Echeance: %s\n", f.date_echeance);

    pause();
}

void creerFacturePersonnalisee()
{
    system("cls");
    printf("\n=== CREATION FACTURE PERSONNALISEE ===\n\n");

    if (nombrePatients == 0)
    {
        printf("Aucun patient enregistre.\n");
        pause();
        return;
    }

    Facture f;
    memset(&f, 0, sizeof(Facture));

    f.id_facture = ++dernierIDFacture;
    f.nb_lignes = 0;

    genererNumeroFacture(f.id_facture, f.numero_facture);

    // Sélection du patient
    afficherListePatientsSimple();
    printf("\nID du patient: ");
    scanf("%d", &f.id_patient);
    viderBuffer();

    if (rechercherParID(f.id_patient) == -1)
    {
        printf("Patient non trouve.\n");
        dernierIDFacture--;
        pause();
        return;
    }

    printf("ID du medecin (0 si non concerne): ");
    scanf("%d", &f.id_medecin);
    viderBuffer();

    // Date d'émission
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(f.date_emission, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    sprintf(f.date_creation, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    // Date d'échéance
    tm.tm_mday += 30;
    mktime(&tm);
    sprintf(f.date_echeance, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    printf("TVA (%%): ");
    scanf("%f", &f.tva);
    viderBuffer();

    printf("Remise (%%): ");
    scanf("%f", &f.remise);
    viderBuffer();

    printf("Prise en charge assurance (FCFA): ");
    scanf("%f", &f.montant_pris_en_charge);
    viderBuffer();

    printf("Assurance: ");
    fgets(f.assurance, 100, stdin);
    f.assurance[strcspn(f.assurance, "\n")] = '\0';

    printf("Numero contrat: ");
    fgets(f.numero_contrat, 50, stdin);
    f.numero_contrat[strcspn(f.numero_contrat, "\n")] = '\0';

    // Ajout des lignes
    char continuer = 'O';
    while (continuer == 'O' && f.nb_lignes < MAX_LIGNES_FACTURE)
    {
        printf("\n--- LIGNE %d ---\n", f.nb_lignes + 1);
        printf("Description: ");
        fgets(f.lignes[f.nb_lignes].description, 200, stdin);
        f.lignes[f.nb_lignes].description[strcspn(f.lignes[f.nb_lignes].description, "\n")] = '\0';

        printf("Quantite: ");
        scanf("%d", &f.lignes[f.nb_lignes].quantite);
        viderBuffer();

        printf("Prix unitaire (FCFA): ");
        scanf("%f", &f.lignes[f.nb_lignes].prix_unitaire);
        viderBuffer();

        f.nb_lignes++;

        if (f.nb_lignes < MAX_LIGNES_FACTURE)
        {
            printf("\nAjouter une autre ligne? (O/N): ");
            scanf("%c", &continuer);
            viderBuffer();
            continuer = toupper(continuer);
        }
    }

    printf("\nNotes: ");
    fgets(f.notes, 300, stdin);
    f.notes[strcspn(f.notes, "\n")] = '\0';

    strcpy(f.statut, "En attente");

    calculerMontants(&f);

    factures[nombreFactures] = f;
    nombreFactures++;

    sauvegarderFactures();

    printf("\n=== FACTURE CREE AVEC SUCCES ===\n");
    printf("Numero: %s\n", f.numero_facture);
    printf("Total: %.2f FCFA\n", f.montant_total);
    printf("A payer: %.2f FCFA\n", f.montant_restant);

    pause();
}

void ajouterLigneFacture(int id_facture)
{
    for (int i = 0; i < nombreFactures; i++)
    {
        if (factures[i].id_facture == id_facture)
        {
            if (factures[i].nb_lignes >= MAX_LIGNES_FACTURE)
            {
                printf("Nombre maximum de lignes atteint.\n");
                return;
            }

            printf("\n--- NOUVELLE LIGNE ---\n");
            printf("Description: ");
            fgets(factures[i].lignes[factures[i].nb_lignes].description, 200, stdin);
            factures[i].lignes[factures[i].nb_lignes].description[strcspn(factures[i].lignes[factures[i].nb_lignes].description, "\n")] = '\0';

            printf("Quantite: ");
            scanf("%d", &factures[i].lignes[factures[i].nb_lignes].quantite);
            viderBuffer();

            printf("Prix unitaire (FCFA): ");
            scanf("%f", &factures[i].lignes[factures[i].nb_lignes].prix_unitaire);
            viderBuffer();

            factures[i].nb_lignes++;

            calculerMontants(&factures[i]);

            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            sprintf(factures[i].date_modification, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

            sauvegarderFactures();
            printf("Ligne ajoutee avec succes.\n");
            return;
        }
    }
    printf("Facture non trouvee.\n");
}

void afficherFacture(int id_facture)
{
    for (int i = 0; i < nombreFactures; i++)
    {
        if (factures[i].id_facture == id_facture)
        {
            Facture f = factures[i];
            int idx_patient = rechercherParID(f.id_patient);

            printf("\n========================================\n");
            printf("              FACTURE N° %s\n", f.numero_facture);
            printf("========================================\n");
            printf("Date d'emission: %s\n", f.date_emission);
            printf("Date d'echeance: %s\n", f.date_echeance);
            printf("Statut: %s\n", f.statut);
            printf("----------------------------------------\n");

            printf("\nPATIENT:\n");
            if (idx_patient != -1)
                printf("  %s %s (ID: %d)\n", patients[idx_patient].prenom, patients[idx_patient].nom, f.id_patient);
            else
                printf("  ID: %d\n", f.id_patient);

            if (f.id_medecin > 0)
            {
                int idx_medecin = rechercherEmployeParID(f.id_medecin);
                printf("\nMEDECIN:\n");
                if (idx_medecin != -1)
                    printf("  Dr %s %s\n", personnel[idx_medecin].prenom, personnel[idx_medecin].nom);
                else
                    printf("  ID: %d\n", f.id_medecin);
            }

            printf("\nDETAILS:\n");
            printf("----------------------------------------\n");
            for (int j = 0; j < f.nb_lignes; j++)
            {
                printf("%s x%d = %.2f FCFA\n",
                       f.lignes[j].description,
                       f.lignes[j].quantite,
                       f.lignes[j].montant);
            }
            printf("----------------------------------------\n");
            printf("Sous-total: %.2f FCFA\n", f.sous_total);
            if (f.remise > 0)
                printf("Remise (%.0f%%): -%.2f FCFA\n", f.remise, f.montant_remise);
            if (f.tva > 0)
                printf("TVA (%.0f%%): +%.2f FCFA\n", f.tva, f.montant_tva);
            printf("TOTAL: %.2f FCFA\n", f.montant_total);

            if (f.montant_pris_en_charge > 0)
            {
                printf("\nPRISE EN CHARGE:\n");
                printf("  Assurance: %s\n", f.assurance);
                printf("  Contrat: %s\n", f.numero_contrat);
                printf("  Montant pris en charge: %.2f FCFA\n", f.montant_pris_en_charge);
            }

            printf("\nRESTANT A PAYER: %.2f FCFA\n", f.montant_restant);

            if (strlen(f.notes) > 0)
                printf("\nNOTES: %s\n", f.notes);

            if (strlen(f.date_paiement) > 0)
                printf("Date de paiement: %s\n", f.date_paiement);

            printf("========================================\n");
            return;
        }
    }
    printf("Facture non trouvee.\n");
}

void afficherFacturesPatient(int id_patient)
{
    system("cls");
    printf("\n=== FACTURES DU PATIENT ===\n\n");

    int compteur = 0;
    printf("N° Facture     | Date       | Total     | Restant   | Statut\n");
    printf("---------------|------------|-----------|-----------|------------\n");

    for (int i = 0; i < nombreFactures; i++)
    {
        if (factures[i].id_patient == id_patient)
        {
            compteur++;
            printf("%-14s | %-10s | %9.0f | %9.0f | %s\n",
                   factures[i].numero_facture,
                   factures[i].date_emission,
                   factures[i].montant_total,
                   factures[i].montant_restant,
                   factures[i].statut);
        }
    }

    if (compteur == 0)
    {
        printf("Aucune facture pour ce patient.\n");
    }
    else
    {
        printf("\nTotal: %d facture(s)\n", compteur);
    }

    pause();
}

void afficherFacturesEnAttente()
{
    system("cls");
    printf("\n=== FACTURES EN ATTENTE ===\n\n");

    int compteur = 0;
    printf("N° Facture     | Patient        | Date       | Total     | Echeance\n");
    printf("---------------|----------------|------------|-----------|------------\n");

    for (int i = 0; i < nombreFactures; i++)
    {
        if (strcmp(factures[i].statut, "En attente") == 0)
        {
            compteur++;
            int idx_patient = rechercherParID(factures[i].id_patient);
            char nom_patient[50] = "";
            if (idx_patient != -1)
                sprintf(nom_patient, "%s %s", patients[idx_patient].prenom, patients[idx_patient].nom);
            else
                sprintf(nom_patient, "ID %d", factures[i].id_patient);

            printf("%-14s | %-14s | %-10s | %9.0f | %s\n",
                   factures[i].numero_facture,
                   nom_patient,
                   factures[i].date_emission,
                   factures[i].montant_total,
                   factures[i].date_echeance);
        }
    }

    if (compteur == 0)
    {
        printf("Aucune facture en attente.\n");
    }

    pause();
}

void afficherFacturesImpayees()
{
    system("cls");
    printf("\n=== FACTURES IMPAYEES ===\n\n");

    int compteur = 0;
    float total_impaye = 0;

    printf("N° Facture     | Patient        | Date       | Restant   | Echeance\n");
    printf("---------------|----------------|------------|-----------|------------\n");

    for (int i = 0; i < nombreFactures; i++)
    {
        if (factures[i].montant_restant > 0.01 && strcmp(factures[i].statut, "Annulée") != 0)
        {
            compteur++;
            total_impaye += factures[i].montant_restant;

            int idx_patient = rechercherParID(factures[i].id_patient);
            char nom_patient[50] = "";
            if (idx_patient != -1)
                sprintf(nom_patient, "%s %s", patients[idx_patient].prenom, patients[idx_patient].nom);
            else
                sprintf(nom_patient, "ID %d", factures[i].id_patient);

            printf("%-14s | %-14s | %-10s | %9.0f | %s\n",
                   factures[i].numero_facture,
                   nom_patient,
                   factures[i].date_emission,
                   factures[i].montant_restant,
                   factures[i].date_echeance);
        }
    }

    if (compteur == 0)
    {
        printf("Aucune facture impayee.\n");
    }
    else
    {
        printf("\nTotal impaye: %.2f FCFA\n", total_impaye);
    }

    pause();
}

void enregistrerPaiement(int id_facture)
{
    for (int i = 0; i < nombreFactures; i++)
    {
        if (factures[i].id_facture == id_facture)
        {
            if (factures[i].montant_restant <= 0.01)
            {
                printf("Cette facture est deja entierement payee.\n");
                pause();
                return;
            }

            printf("\n=== ENREGISTREMENT PAIEMENT ===\n");
            printf("Facture N° %s\n", factures[i].numero_facture);
            printf("Montant restant: %.2f FCFA\n", factures[i].montant_restant);

            float montant;
            printf("Montant paye: ");
            scanf("%f", &montant);
            viderBuffer();

            if (montant > factures[i].montant_restant)
            {
                printf("Le montant depasse le restant dû.\n");
                printf("Voulez-vous prendre l'excedent? (O/N): ");
                char rep;
                scanf("%c", &rep);
                viderBuffer();
                if (toupper(rep) != 'O')
                {
                    return;
                }
            }

            factures[i].montant_restant -= montant;
            if (factures[i].montant_restant <= 0.01)
            {
                factures[i].montant_restant = 0;
                strcpy(factures[i].statut, "Payée");
            }

            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            sprintf(factures[i].date_paiement, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

            printf("Mode de paiement (Especes/Carte/Virement): ");
            fgets(factures[i].mode_paiement, 30, stdin);
            factures[i].mode_paiement[strcspn(factures[i].mode_paiement, "\n")] = '\0';

            sauvegarderFactures();

            printf("\nPaiement enregistre avec succes!\n");
            if (factures[i].montant_restant > 0)
                printf("Restant a payer: %.2f FCFA\n", factures[i].montant_restant);
            else
                printf("Facture completement soldée.\n");

            pause();
            return;
        }
    }
    printf("Facture non trouvee.\n");
    pause();
}

void modifierFacture(int id_facture)
{
    for (int i = 0; i < nombreFactures; i++)
    {
        if (factures[i].id_facture == id_facture)
        {
            int choix;
            do
            {
                system("cls");
                printf("\n=== MODIFICATION FACTURE N° %s ===\n", factures[i].numero_facture);
                printf("1. Ajouter une ligne\n");
                printf("2. Modifier remise (actuelle: %.1f%%)\n", factures[i].remise);
                printf("3. Modifier TVA (actuelle: %.1f%%)\n", factures[i].tva);
                printf("4. Modifier prise en charge (%.2f)\n", factures[i].montant_pris_en_charge);
                printf("5. Modifier statut (%s)\n", factures[i].statut);
                printf("6. Notes\n");
                printf("7. Sauvegarder et quitter\n");
                printf("8. Annuler\n");
                printf("\nChoix: ");
                scanf("%d", &choix);
                viderBuffer();

                switch(choix)
                {
                    case 1:
                        ajouterLigneFacture(id_facture);
                        break;
                    case 2:
                        printf("Nouvelle remise (%%): ");
                        scanf("%f", &factures[i].remise);
                        viderBuffer();
                        calculerMontants(&factures[i]);
                        break;
                    case 3:
                        printf("Nouvelle TVA (%%): ");
                        scanf("%f", &factures[i].tva);
                        viderBuffer();
                        calculerMontants(&factures[i]);
                        break;
                    case 4:
                        printf("Nouveau montant pris en charge: ");
                        scanf("%f", &factures[i].montant_pris_en_charge);
                        viderBuffer();
                        calculerMontants(&factures[i]);
                        break;
                    case 5:
                        printf("Statut (En attente/Payee/Impayee/Annulee): ");
                        fgets(factures[i].statut, 20, stdin);
                        factures[i].statut[strcspn(factures[i].statut, "\n")] = '\0';
                        break;
                    case 6:
                        printf("Nouvelles notes: ");
                        fgets(factures[i].notes, 300, stdin);
                        factures[i].notes[strcspn(factures[i].notes, "\n")] = '\0';
                        break;
                    case 7:
                        sauvegarderFactures();
                        printf("Modifications sauvegardees.\n");
                        return;
                    case 8:
                        return;
                }
            } while(1);
        }
    }
    printf("Facture non trouvee.\n");
    pause();
}

void annulerFacture(int id_facture)
{
    for (int i = 0; i < nombreFactures; i++)
    {
        if (factures[i].id_facture == id_facture)
        {
            printf("\n=== ANNULATION FACTURE ===\n");
            printf("Facture N° %s\n", factures[i].numero_facture);
            printf("Montant: %.2f FCFA\n", factures[i].montant_total);
            printf("Confirmer l'annulation? (O/N): ");

            char confirmation;
            scanf("%c", &confirmation);
            viderBuffer();

            if (toupper(confirmation) == 'O')
            {
                strcpy(factures[i].statut, "Annulée");
                sauvegarderFactures();
                printf("Facture annulee avec succes.\n");
            }
            else
            {
                printf("Annulation abandonnee.\n");
            }
            pause();
            return;
        }
    }
    printf("Facture non trouvee.\n");
    pause();
}

void rechercherFacture(char *numero)
{
    system("cls");
    printf("\n=== RECHERCHE FACTURE ===\n");
    printf("Numero: %s\n\n", numero);

    for (int i = 0; i < nombreFactures; i++)
    {
        if (strstr(factures[i].numero_facture, numero) != NULL)
        {
            afficherFacture(factures[i].id_facture);
            return;
        }
    }
    printf("Facture non trouvee.\n");
    pause();
}

void statistiquesFacturation()
{
    system("cls");
    printf("\n=== STATISTIQUES DE FACTURATION ===\n\n");

    if (nombreFactures == 0)
    {
        printf("Aucune facture enregistree.\n");
        pause();
        return;
    }

    int en_attente = 0, payees = 0, impayees = 0, annulees = 0;
    float total_facture = 0, total_percu = 0, total_impaye = 0;

    for (int i = 0; i < nombreFactures; i++)
    {
        total_facture += factures[i].montant_total;
        total_percu += (factures[i].montant_total - factures[i].montant_restant);
        total_impaye += factures[i].montant_restant;

        if (strcmp(factures[i].statut, "En attente") == 0) en_attente++;
        else if (strcmp(factures[i].statut, "Payée") == 0) payees++;
        else if (strcmp(factures[i].statut, "Impayée") == 0) impayees++;
        else if (strcmp(factures[i].statut, "Annulée") == 0) annulees++;
    }

    printf("=== PAR STATUT ===\n");
    printf("  En attente: %d (%.1f%%)\n", en_attente, (float)en_attente/nombreFactures*100);
    printf("  Payees: %d (%.1f%%)\n", payees, (float)payees/nombreFactures*100);
    printf("  Impayees: %d (%.1f%%)\n", impayees, (float)impayees/nombreFactures*100);
    printf("  Annulees: %d (%.1f%%)\n", annulees, (float)annulees/nombreFactures*100);

    printf("\n=== MONTANTS ===\n");
    printf("  Total facture: %.2f FCFA\n", total_facture);
    printf("  Total percu: %.2f FCFA\n", total_percu);
    printf("  Total impaye: %.2f FCFA\n", total_impaye);
    printf("  Taux de recouvrement: %.1f%%\n", (total_facture > 0 ? total_percu/total_facture*100 : 0));

    printf("\nNombre total de factures: %d\n", nombreFactures);

    pause();
}

void chiffreAffairesParPeriode(char *date_debut, char *date_fin)
{
    system("cls");
    printf("\n=== CHIFFRE D'AFFAIRES ===\n");
    printf("Periode: %s au %s\n\n", date_debut, date_fin);

    float total = 0;
    int compteur = 0;

    for (int i = 0; i < nombreFactures; i++)
    {
        if (strcmp(factures[i].statut, "Payée") == 0)
        {
            // Comparaison simple des dates (à améliorer)
            if (strcmp(factures[i].date_paiement, date_debut) >= 0 &&
                strcmp(factures[i].date_paiement, date_fin) <= 0)
            {
                total += (factures[i].montant_total - factures[i].montant_restant);
                compteur++;
            }
        }
    }

    printf("Nombre de paiements: %d\n", compteur);
    printf("Chiffre d'affaires: %.2f FCFA\n", total);

    pause();
}

void menuFacturation()
{
    int choix;

    do
    {
        system("cls");
        color(10, 0);
        printf("\n========================================\n");
        printf("          MODULE FACTURATION           \n");
        printf("========================================\n\n");
        color(7, 0);

        printf("1. Creer facture depuis consultation\n");
        printf("2. Creer facture personnalisee\n");
        printf("3. Afficher factures d'un patient\n");
        printf("4. Afficher factures en attente\n");
        printf("5. Afficher factures impayees\n");
        printf("6. Enregistrer un paiement\n");
        printf("7. Modifier une facture\n");
        printf("8. Annuler une facture\n");
        printf("9. Rechercher une facture\n");
        printf("10. Statistiques de facturation\n");
        printf("11. Chiffre d'affaires par periode\n");
        printf("12. Retour\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                if (nombreConsultations == 0)
                {
                    printf("Aucune consultation.\n");
                    pause();
                    break;
                }
                printf("ID Consultation: ");
                int id_cons;
                scanf("%d", &id_cons);
                viderBuffer();
                creerFactureConsultation(id_cons);
                break;

            case 2:
                creerFacturePersonnalisee();
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
                    printf("\nID Patient: ");
                    int id;
                    scanf("%d", &id);
                    viderBuffer();
                    afficherFacturesPatient(id);
                }
                break;

            case 4:
                afficherFacturesEnAttente();
                break;

            case 5:
                afficherFacturesImpayees();
                break;

            case 6:
                {
                    char num[20];
                    printf("Numero facture: ");
                    fgets(num, 20, stdin);
                    num[strcspn(num, "\n")] = '\0';

                    int id = -1;
                    for (int i = 0; i < nombreFactures; i++)
                    {
                        if (strcmp(factures[i].numero_facture, num) == 0)
                        {
                            id = factures[i].id_facture;
                            break;
                        }
                    }

                    if (id != -1)
                        enregistrerPaiement(id);
                    else
                    {
                        printf("Facture non trouvee.\n");
                        pause();
                    }
                }
                break;

            case 7:
                {
                    char num[20];
                    printf("Numero facture: ");
                    fgets(num, 20, stdin);
                    num[strcspn(num, "\n")] = '\0';

                    int id = -1;
                    for (int i = 0; i < nombreFactures; i++)
                    {
                        if (strcmp(factures[i].numero_facture, num) == 0)
                        {
                            id = factures[i].id_facture;
                            break;
                        }
                    }

                    if (id != -1)
                        modifierFacture(id);
                    else
                    {
                        printf("Facture non trouvee.\n");
                        pause();
                    }
                }
                break;

            case 8:
                {
                    char num[20];
                    printf("Numero facture: ");
                    fgets(num, 20, stdin);
                    num[strcspn(num, "\n")] = '\0';

                    int id = -1;
                    for (int i = 0; i < nombreFactures; i++)
                    {
                        if (strcmp(factures[i].numero_facture, num) == 0)
                        {
                            id = factures[i].id_facture;
                            break;
                        }
                    }

                    if (id != -1)
                        annulerFacture(id);
                    else
                    {
                        printf("Facture non trouvee.\n");
                        pause();
                    }
                }
                break;

            case 9:
                {
                    char num[20];
                    printf("Numero facture: ");
                    fgets(num, 20, stdin);
                    num[strcspn(num, "\n")] = '\0';
                    rechercherFacture(num);
                }
                break;

            case 10:
                statistiquesFacturation();
                break;

            case 11:
                {
                    char debut[20], fin[20];
                    printf("Date debut (JJ/MM/AAAA): ");
                    fgets(debut, 20, stdin);
                    debut[strcspn(debut, "\n")] = '\0';
                    printf("Date fin (JJ/MM/AAAA): ");
                    fgets(fin, 20, stdin);
                    fin[strcspn(fin, "\n")] = '\0';
                    chiffreAffairesParPeriode(debut, fin);
                }
                break;

            case 12:
                return;

            default:
                printf("Choix invalide.\n");
                pause();
        }

    } while(choix != 12);
}

void sauvegarderFactures()
{
    FILE *f = fopen("factures.dat", "wb");
    if (f == NULL) return;

    fwrite(&nombreFactures, sizeof(int), 1, f);
    fwrite(factures, sizeof(Facture), nombreFactures, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_facture.dat", "wb");
    if (fid != NULL)
    {
        fwrite(&dernierIDFacture, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void chargerFactures()
{
    FILE *f = fopen("factures.dat", "rb");
    if (f == NULL) return;

    fread(&nombreFactures, sizeof(int), 1, f);
    fread(factures, sizeof(Facture), nombreFactures, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_facture.dat", "rb");
    if (fid != NULL)
    {
        fread(&dernierIDFacture, sizeof(int), 1, fid);
        fclose(fid);
    }
}
