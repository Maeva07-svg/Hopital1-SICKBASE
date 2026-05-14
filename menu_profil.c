#include <stdio.h>
#include "comptes.h"
#include "utils.h"

void afficherMenuParProfil()
{
    if (utilisateur_actuel == NULL) return;

    system("cls");
    color(10, 0);
    printf("\n========================================\n");
    printf("    SYSTEME DE GESTION HOSPITALIERE    \n");
    printf("========================================\n");
    color(7, 0);

    // Afficher les informations de l'utilisateur
    printf("\nUtilisateur: %s", utilisateur_actuel->login);
    printf("\nProfil: ");
    switch(utilisateur_actuel->profil)
    {
        case ROLE_PATIENT: printf("Patient\n"); break;
        case ROLE_MEDECIN: printf("Medecin\n"); break;
        case ROLE_INFIRMIER: printf("Infirmier\n"); break;
        case ROLE_ADMINISTRATEUR: printf("Administrateur\n"); break;
        case ROLE_TECHNICIEN_LABO: printf("Technicien de laboratoire\n"); break;
        case ROLE_PHARMACIEN: printf("Pharmacien\n"); break;
        case ROLE_SUPER_ADMIN: printf("Super Administrateur\n"); break;
    }
    printf("----------------------------------------\n\n");

    // Menu selon le profil
    switch(utilisateur_actuel->profil)
    {
        case ROLE_PATIENT:
            printf("1. Voir mon dossier medical\n");
            printf("2. Voir mes consultations\n");
            printf("3. Voir mes ordonnances\n");
            printf("4. Voir mes analyses\n");
            printf("5. Prendre rendez-vous\n");
            printf("6. Modifier mon mot de passe\n");
            printf("7. Deconnexion\n");
            break;

        case ROLE_MEDECIN:
            printf("1. Gestion des patients\n");
            printf("2. Consultations\n");
            printf("3. Prescriptions\n");
            printf("4. Diagnostic medical\n");
            printf("5. Demandes d'analyses\n");
            printf("6. Agenda\n");
            printf("7. Statistiques\n");
            printf("8. Modifier mot de passe\n");
            printf("9. Deconnexion\n");
            break;

        case ROLE_INFIRMIER:
            printf("1. Liste des patients\n");
            printf("2. Soins a administrer\n");
            printf("3. Constantes vitales\n");
            printf("4. Planning des soins\n");
            printf("5. Modifier mot de passe\n");
            printf("6. Deconnexion\n");
            break;

        case ROLE_ADMINISTRATEUR:
            printf("1. Gestion des patients\n");
            printf("2. Gestion du personnel\n");
            printf("3. Gestion des rendez-vous\n");
            printf("4. Gestion des consultations\n");
            printf("5. Gestion des comptes utilisateurs\n");
            printf("6. Archives\n");
            printf("7. Statistiques\n");
            printf("8. Modifier mot de passe\n");
            printf("9. Deconnexion\n");
            break;

        case ROLE_TECHNICIEN_LABO:
            printf("1. Liste des analyses a realiser\n");
            printf("2. Saisir resultats d'analyses\n");
            printf("3. Consulter historique analyses\n");
            printf("4. Modifier mot de passe\n");
            printf("5. Deconnexion\n");
            break;

        case ROLE_PHARMACIEN:
            printf("1. Gestion des ordonnances\n");
            printf("2. Gestion du stock\n");
            printf("3. Delivrance medicaments\n");
            printf("4. Alertes stock faible\n");
            printf("5. Modifier mot de passe\n");
            printf("6. Deconnexion\n");
            break;

        case ROLE_SUPER_ADMIN:
            printf("1. Gestion complete patients\n");
            printf("2. Gestion complete personnel\n");
            printf("3. Gestion consultations\n");
            printf("4. Gestion ordonnances\n");
            printf("5. Gestion interventions\n");
            printf("6. Gestion comptes utilisateurs\n");
            printf("7. Archives\n");
            printf("8. Statistiques avancees\n");
            printf("9. Configuration systeme\n");
            printf("10. Modifier mot de passe\n");
            printf("11. Deconnexion\n");
            break;
    }

    printf("\nVotre choix: ");
}
