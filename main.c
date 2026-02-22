#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "utils.h"
#include "patient.h"
#include "diagnostic.h"
#include "fichiers.h"
#include "personnel.h"
#include "fichiers_personnel.h"
#include "consultations.h"
#include "ordonnances.h"
#include "interventions.h"
#include "archives.h"
#include "fichiers_relations.h"

void menuPrincipal()
{
    int choix;

    do
    {
        system("cls");
        color(10, 0);
        printf("\n========================================\n");
        printf("    SYSTEME DE GESTION HOSPITALIER    \n");
        printf("           VERSION COMPLETE 5.0       \n");
        printf("========================================\n\n");
        color(7, 0);

        printf("1. Gestion des patients\n");
        printf("2. Gestion du personnel\n");
        printf("3. Consultations & Rendez-vous\n");
        printf("4. Ordonnances & Prescriptions\n");
        printf("5. Interventions & Prises en charge\n");
        printf("6. Diagnostic medical\n");
        printf("7. Archives\n");
        printf("8. Statistiques generales\n");
        printf("9. Quitter\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
        case 1:
            menuGestionPatients();
            break;
        case 2:
            menuGestionPersonnel();
            break;
        case 3:
            menuConsultations();
            break;
        case 4:
            menuOrdonnances();
            break;
        case 5:
            menuInterventions();  // Maintenant actif
            break;
        case 6:
            menuDiagnosticMedical();
            break;
        case 7:
            // menuArchives(); (à activer quand implémenté)
            printf("\nModule Archives (en cours de developpement)\n");
            pause();
            break;
        case 8:
            // Afficher statistiques globales
            printf("\nSTATISTIQUES GENERALES\n\n");
            printf("Patients: %d\n", nombrePatients);
            printf("Personnel: %d\n", nombreEmployes);
            printf("Consultations: %d\n", nombreConsultations);
            printf("Ordonnances: %d\n", nombreOrdonnances);
            printf("Rendez-vous: %d\n", nombreRendezVous);
            pause();
            break;
        case 9:
            // Sauvegarder toutes les données
            sauvegarderPatients();
            sauvegarderPersonnel();
            sauvegarderConsultations();
            sauvegarderRendezVous();
            sauvegarderOrdonnances();
            sauvegarderInterventions();
            sauvegarderPrisesEnCharge();
            sauvegarderArchives();
            printf("\nToutes les donnees ont ete sauvegardees.\n");
            printf("Au revoir!\n");
            break;
        default:
            printf("\nChoix invalide!\n");
            pause();
        }

    }
    while(choix != 9);
}

int main()
{
    system("cls");
    color(14, 0);
    printf("\nCHARGEMENT DU SYSTEME HOSPITALIER...\n");
    color(7, 0);

    // Initialiser tous les modules
    initialiserMaladies();
    initialiserFonctionsPersonnel();
    initialiserConsultations();
    initialiserOrdonnances();
    initialiserInterventions();
    initialiserArchives();

    // Charger toutes les données
    chargerDernierID();
    chargerPatients();
    chargerDernierIDPersonnel();
    chargerPersonnel();

    chargerConsultations();
    chargerRendezVous();
    chargerOrdonnances();
    chargerInterventions();
    chargerPrisesEnCharge();
    chargerArchives();

    printf("\n========================================\n");
    printf("SYSTEME CHARGE AVEC SUCCES!\n");
    printf("========================================\n");
    printf("Patients: %d\n", nombrePatients);
    printf("Personnel: %d\n", nombreEmployes);
    printf("Consultations: %d\n", nombreConsultations);
    printf("Ordonnances: %d\n", nombreOrdonnances);
    printf("Interventions: %d\n", nombreInterventions);
    printf("Archives: %d\n", nombreArchives);
    printf("========================================\n");

    Sleep(2000);

    menuPrincipal();

    return 0;
}
