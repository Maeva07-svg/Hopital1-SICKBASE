#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "utils.h"
#include "patient.h"
#include "diagnostic.h"
#include "fichiers.h"

void menuPrincipal()
{
    int choix;

    do
    {
        system("cls");
        color(10, 0);
        printf("\n========================================\n");
        printf("    SYSTEME DE GESTION HOSPITALIER    \n");
        printf("      avec DIAGNOSTIC AUTOMATIQUE     \n");
        printf("               Version 3.0            \n");
        printf("========================================\n\n");
        color(7, 0);

        printf("1. Gestion des patients\n");
        printf("2. Diagnostic medical\n");
        printf("3. Gestion du personnel\n");
        printf("4. Gestion des medicaments\n");
        printf("5. Archives\n");
        printf("6. Quitter\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
        case 1:
            menuGestionPatients();
            break;
        case 2:
            menuDiagnosticMedical();
            break;
        case 3:
            printf("\nGestion du personnel (a implementer)\n");
            pause();
            break;
        case 4:
            printf("\nGestion des medicaments (a implementer)\n");
            pause();
            break;
        case 5:
            printf("\nArchives (a implementer)\n");
            pause();
            break;
        case 6:
            sauvegarderPatients();
            printf("\nDonnees sauvegardees. Au revoir!\n");
            break;
        default:
            printf("\nChoix invalide!\n");
            pause();
        }

    } while(choix != 6);
}

int main()
{
    system("cls");
    color(14, 0);
    printf("\nCHARGEMENT DU SYSTEME HOSPITALIER...\n");
    color(7, 0);

    // Initialiser la base de données des maladies
    initialiserMaladies();

    // Charger les données existantes
    chargerDernierID();
    chargerPatients();

    printf("\nSysteme de diagnostic medical charge!\n");
    printf("Base de connaissances: %d maladies\n", nombreMaladies);
    printf("Patients dans la base: %d\n", nombrePatients);

    Sleep(1500);

    menuPrincipal();

    return 0;
}
