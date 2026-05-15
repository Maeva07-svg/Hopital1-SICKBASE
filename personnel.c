#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "personnel.h"
#include "fichiers_personnel.h"
#include "utils.h"
#include "ui.h"

// Définition des variables globales
Employe personnel[MAX_PERSONNEL];
int nombreEmployes = 0;
int dernierIDEmploye = 0;

// Tableau des fonctions pour affichage
const char *noms_fonctions[] = {
    "Medecin",
    "Infirmier",
    "Administratif",
    "Technicien",
    "Chirurgien",
    "Radiologue",
    "Pharmacien",
    "Autre"
};

void initialiserFonctionsPersonnel()
{
    // Rien à initialiser
}

// ==================== FONCTIONS EXISTANTES ====================

void ajouterEmploye()
{
    system("cls");
    printf("\n=== AJOUT D'UN NOUVEL EMPLOYE ===\n\n");

    if (nombreEmployes >= MAX_PERSONNEL)
    {
        printf("ERREUR: Capacite maximale atteinte!\n");
        pause();
        return;
    }

    Employe e;
    memset(&e, 0, sizeof(Employe));

    e.id = ++dernierIDEmploye;
    e.actif = 1;

    printf("INFORMATIONS PERSONNELLES\n\n");

    printf("Nom: ");
    fgets(e.nom, MAX_NAME, stdin);
    e.nom[strcspn(e.nom, "\n")] = '\0';

    printf("Prenom: ");
    fgets(e.prenom, MAX_NAME, stdin);
    e.prenom[strcspn(e.prenom, "\n")] = '\0';

    printf("Date de naissance (JJ/MM/AAAA): ");
    fgets(e.date_naissance, 20, stdin);
    e.date_naissance[strcspn(e.date_naissance, "\n")] = '\0';

    printf("Genre (M/F): ");
    scanf("%c", &e.genre);
    viderBuffer();
    e.genre = toupper(e.genre);

    printf("Telephone: ");
    fgets(e.telephone, 20, stdin);
    e.telephone[strcspn(e.telephone, "\n")] = '\0';

    printf("Email: ");
    fgets(e.email, 100, stdin);
    e.email[strcspn(e.email, "\n")] = '\0';

    printf("Adresse: ");
    fgets(e.adresse, 200, stdin);
    e.adresse[strcspn(e.adresse, "\n")] = '\0';

    printf("\nINFORMATIONS PROFESSIONNELLES\n\n");

    printf("Fonction (1-Medecin, 2-Infirmier, 3-Admin, 4-Technicien,\n");
    printf("         5-Chirurgien, 6-Radiologue, 7-Pharmacien, 8-Autre): ");
    int choixFonction;
    scanf("%d", &choixFonction);
    viderBuffer();

    if (choixFonction >= 1 && choixFonction <= 8)
    {
        strcpy(e.fonction, noms_fonctions[choixFonction - 1]);
    }
    else
    {
        strcpy(e.fonction, "Autre");
    }

    printf("Specialite: ");
    fgets(e.specialite, MAX_SPECIALITE, stdin);
    e.specialite[strcspn(e.specialite, "\n")] = '\0';

    printf("Service: ");
    fgets(e.service, MAX_SERVICE, stdin);
    e.service[strcspn(e.service, "\n")] = '\0';

    printf("Diplomes: ");
    fgets(e.diplomes, MAX_DIPLOME, stdin);
    e.diplomes[strcspn(e.diplomes, "\n")] = '\0';

    printf("Numero d'ordre professionnel: ");
    fgets(e.numero_ordre, 30, stdin);
    e.numero_ordre[strcspn(e.numero_ordre, "\n")] = '\0';

    printf("Date d'embauche (JJ/MM/AAAA): ");
    fgets(e.date_embauche, 20, stdin);
    e.date_embauche[strcspn(e.date_embauche, "\n")] = '\0';

    printf("Salaire mensuel: ");
    scanf("%f", &e.salaire);
    viderBuffer();

    printf("Heures par semaine: ");
    scanf("%d", &e.heures_semaine);
    viderBuffer();

    printf("\nCONGES\n\n");
    printf("Jours de conges annuels: ");
    scanf("%d", &e.jours_conges_annuels);
    viderBuffer();

    e.jours_conges_restants = e.jours_conges_annuels;

    printf("\nHoraires de travail: ");
    fgets(e.horaires_travail, MAX_HORAIRES, stdin);
    e.horaires_travail[strcspn(e.horaires_travail, "\n")] = '\0';

    printf("\nNotes (optionnel):\n");
    fgets(e.notes, 500, stdin);
    e.notes[strcspn(e.notes, "\n")] = '\0';

    personnel[nombreEmployes] = e;
    nombreEmployes++;

    sauvegarderPersonnel();

    printf("\n=== EMPLOYE AJOUTE AVEC SUCCES ===\n");
    printf("ID: %d\n", e.id);
    printf("Nom: %s %s\n", e.prenom, e.nom);
    printf("Fonction: %s\n", e.fonction);
    printf("Service: %s\n", e.service);

    pause();
}

void afficherListeEmployesSimple()
{
    system("cls");
    printf("\n=== LISTE DU PERSONNEL ===\n\n");

    if (nombreEmployes == 0)
    {
        printf("Aucun employe enregistre.\n");
        return;
    }

    printf("Nombre total d'employes: %d\n\n", nombreEmployes);
    printf("ID   | Nom & Prenom           | Fonction      | Service       | Actif\n");
    printf("-----|------------------------|---------------|---------------|------\n");

    for (int i = 0; i < nombreEmployes; i++)
    {
        printf("%-4d | %-10s %-10s | %-13s | %-13s | %s\n",
               personnel[i].id,
               personnel[i].prenom,
               personnel[i].nom,
               personnel[i].fonction,
               personnel[i].service,
               personnel[i].actif ? "Oui" : "Non");
    }
}

void afficherDetailsCompletEmploye(int index)
{
    if (index < 0 || index >= nombreEmployes)
    {
        printf("Erreur: Employe non trouve.\n");
        return;
    }

    Employe e = personnel[index];

    printf("\n========================================\n");
    printf("FICHE EMPLOYE COMPLETE - ID: %d\n", e.id);
    printf("========================================\n");

    printf("\nINFORMATIONS PERSONNELLES:\n");
    printf("  Nom: %s\n", e.nom);
    printf("  Prenom: %s\n", e.prenom);
    printf("  Date naissance: %s\n", e.date_naissance);
    printf("  Genre: %c\n", e.genre);
    printf("  Telephone: %s\n", e.telephone);
    printf("  Email: %s\n", e.email);
    printf("  Adresse: %s\n", e.adresse);

    printf("\nINFORMATIONS PROFESSIONNELLES:\n");
    printf("  Fonction: %s\n", e.fonction);
    printf("  Specialite: %s\n", e.specialite);
    printf("  Service: %s\n", e.service);
    printf("  Diplomes: %s\n", e.diplomes);
    printf("  N° Ordre: %s\n", e.numero_ordre);
    printf("  Date embauche: %s\n", e.date_embauche);
    printf("  Salaire: %.2f €\n", e.salaire);
    printf("  Heures/semaine: %d\n", e.heures_semaine);
    printf("  Statut: %s\n", e.actif ? "Actif" : "Inactif");

    printf("\nCONGES:\n");
    printf("  Conges annuels: %d jours\n", e.jours_conges_annuels);
    printf("  Conges restants: %d jours\n", e.jours_conges_restants);

    printf("\nHORAIRES:\n");
    printf("  %s\n", e.horaires_travail);

    if (strlen(e.notes) > 0)
    {
        printf("\nNOTES:\n");
        printf("  %s\n", e.notes);
    }

    printf("\n========================================\n");
}

int rechercherEmployeParID(int id)
{
    for (int i = 0; i < nombreEmployes; i++)
    {
        if (personnel[i].id == id)
        {
            return i;
        }
    }
    return -1;
}

int rechercherEmployeParNom(char *nom)
{
    char nomRecherche[MAX_NAME];
    strcpy(nomRecherche, nom);

    for(int i = 0; nomRecherche[i]; i++)
        nomRecherche[i] = tolower(nomRecherche[i]);

    for (int i = 0; i < nombreEmployes; i++)
    {
        char nomEmploye[MAX_NAME];
        strcpy(nomEmploye, personnel[i].nom);
        for(int j = 0; nomEmploye[j]; j++)
            nomEmploye[j] = tolower(nomEmploye[j]);

        if (strstr(nomEmploye, nomRecherche) != NULL)
        {
            return i;
        }
    }
    return -1;
}

void rechercherEmploye()
{
    system("cls");
    printf("\n=== RECHERCHE D'EMPLOYE ===\n\n");

    if (nombreEmployes == 0)
    {
        printf("Aucun employe enregistre.\n");
        pause();
        return;
    }

    printf("1. Rechercher par ID\n");
    printf("2. Rechercher par nom\n");
    printf("Votre choix: ");

    int choix;
    scanf("%d", &choix);
    viderBuffer();

    int index = -1;

    if (choix == 1)
    {
        int id;
        printf("Entrez l'ID de l'employe: ");
        scanf("%d", &id);
        viderBuffer();
        index = rechercherEmployeParID(id);
    }
    else if (choix == 2)
    {
        char nom[MAX_NAME];
        printf("Entrez le nom de l'employe: ");
        fgets(nom, MAX_NAME, stdin);
        nom[strcspn(nom, "\n")] = '\0';
        index = rechercherEmployeParNom(nom);
    }

    if (index != -1)
    {
        afficherDetailsCompletEmploye(index);

        printf("\nOptions:\n");
        printf("1. Modifier cet employe\n");
        printf("2. Gerer les conges\n");
        printf("3. Supprimer/Desactiver\n");
        printf("4. Retour\n");
        printf("Votre choix: ");

        int action;
        scanf("%d", &action);
        viderBuffer();

        if (action == 1)
        {
            modifierEmploye(index);
        }
        else if (action == 2)
        {
            gererConges(index);
        }
        else if (action == 3)
        {
            supprimerEmploye(index);
        }
    }
    else
    {
        printf("Employe non trouve.\n");
        pause();
    }
}

void modifierEmploye(int index)
{
    if (index < 0 || index >= nombreEmployes)
    {
        printf("Erreur: Employe non trouve.\n");
        return;
    }

    Employe *e = &personnel[index];
    int choix;

    do
    {
        system("cls");
        printf("\n=== MODIFICATION D'UN EMPLOYE ===\n");
        printf("Employe: %s %s (ID: %d)\n\n", e->prenom, e->nom, e->id);

        printf("1. Nom (%s)\n", e->nom);
        printf("2. Prenom (%s)\n", e->prenom);
        printf("3. Telephone (%s)\n", e->telephone);
        printf("4. Email (%s)\n", e->email);
        printf("5. Adresse (%s)\n", e->adresse);
        printf("6. Fonction (%s)\n", e->fonction);
        printf("7. Specialite (%s)\n", e->specialite);
        printf("8. Service (%s)\n", e->service);
        printf("9. Salaire (%.2f)\n", e->salaire);
        printf("10. Heures/semaine (%d)\n", e->heures_semaine);
        printf("11. Statut (%s)\n", e->actif ? "Actif" : "Inactif");
        printf("12. Horaires (%s)\n", e->horaires_travail);
        printf("13. Notes\n");
        printf("14. Sauvegarder et quitter\n");
        printf("15. Annuler\n");
        printf("\nQuel champ modifier? (1-15): ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                printf("Nouveau nom: ");
                fgets(e->nom, MAX_NAME, stdin);
                e->nom[strcspn(e->nom, "\n")] = '\0';
                break;
            case 2:
                printf("Nouveau prenom: ");
                fgets(e->prenom, MAX_NAME, stdin);
                e->prenom[strcspn(e->prenom, "\n")] = '\0';
                break;
            case 3:
                printf("Nouveau telephone: ");
                fgets(e->telephone, 20, stdin);
                e->telephone[strcspn(e->telephone, "\n")] = '\0';
                break;
            case 4:
                printf("Nouvel email: ");
                fgets(e->email, 100, stdin);
                e->email[strcspn(e->email, "\n")] = '\0';
                break;
            case 5:
                printf("Nouvelle adresse: ");
                fgets(e->adresse, 200, stdin);
                e->adresse[strcspn(e->adresse, "\n")] = '\0';
                break;
            case 6:
                printf("Nouvelle fonction: ");
                fgets(e->fonction, 50, stdin);
                e->fonction[strcspn(e->fonction, "\n")] = '\0';
                break;
            case 7:
                printf("Nouvelle specialite: ");
                fgets(e->specialite, MAX_SPECIALITE, stdin);
                e->specialite[strcspn(e->specialite, "\n")] = '\0';
                break;
            case 8:
                printf("Nouveau service: ");
                fgets(e->service, MAX_SERVICE, stdin);
                e->service[strcspn(e->service, "\n")] = '\0';
                break;
            case 9:
                printf("Nouveau salaire: ");
                scanf("%f", &e->salaire);
                viderBuffer();
                break;
            case 10:
                printf("Nouvelles heures/semaine: ");
                scanf("%d", &e->heures_semaine);
                viderBuffer();
                break;
            case 11:
                e->actif = !e->actif;
                printf("Statut change en: %s\n", e->actif ? "Actif" : "Inactif");
                break;
            case 12:
                printf("Nouveaux horaires: ");
                fgets(e->horaires_travail, MAX_HORAIRES, stdin);
                e->horaires_travail[strcspn(e->horaires_travail, "\n")] = '\0';
                break;
            case 13:
                printf("Nouvelles notes: ");
                fgets(e->notes, 500, stdin);
                e->notes[strcspn(e->notes, "\n")] = '\0';
                break;
            case 14:
                sauvegarderPersonnel();
                printf("\nModifications sauvegardees avec succes!\n");
                return;
            case 15:
                printf("\nModifications annulees.\n");
                return;
            default:
                printf("Choix invalide.\n");
        }

        if (choix != 14 && choix != 15)
        {
            printf("\nModification effectuee. Continuer? (O/N): ");
            char continuer;
            scanf("%c", &continuer);
            viderBuffer();

            if (toupper(continuer) != 'O')
            {
                sauvegarderPersonnel();
                printf("Modifications sauvegardees.\n");
                pause();
                return;
            }
        }

    } while(choix != 14 && choix != 15);
}

void supprimerEmploye(int index)
{
    if (index < 0 || index >= nombreEmployes)
    {
        printf("Erreur: Employe non trouve.\n");
        return;
    }

    printf("\n=== SUPPRESSION/DESACTIVATION D'EMPLOYE ===\n");
    printf("Employe: %s %s (ID: %d)\n", personnel[index].prenom, personnel[index].nom, personnel[index].id);
    printf("1. Supprimer definitivement\n");
    printf("2. Desactiver (garder trace)\n");
    printf("3. Annuler\n");
    printf("Votre choix: ");

    int choix;
    scanf("%d", &choix);
    viderBuffer();

    if (choix == 1)
    {
        printf("Confirmer suppression definitive? (O/N): ");
        char confirmation;
        scanf("%c", &confirmation);
        viderBuffer();

        if (toupper(confirmation) == 'O')
        {
            for (int i = index; i < nombreEmployes - 1; i++)
            {
                personnel[i] = personnel[i + 1];
            }
            nombreEmployes--;
            sauvegarderPersonnel();
            printf("Employe supprime avec succes.\n");
        }
    }
    else if (choix == 2)
    {
        personnel[index].actif = 0;
        sauvegarderPersonnel();
        printf("Employe desactive avec succes.\n");
    }

    pause();
}

void gererConges(int index)
{
    if (index < 0 || index >= nombreEmployes)
    {
        printf("Erreur: Employe non trouve.\n");
        return;
    }

    Employe *e = &personnel[index];

    system("cls");
    printf("\n=== GESTION DES CONGES ===\n");
    printf("Employe: %s %s\n\n", e->prenom, e->nom);
    printf("Conges annuels: %d jours\n", e->jours_conges_annuels);
    printf("Conges restants: %d jours\n\n", e->jours_conges_restants);

    printf("1. Prendre des conges\n");
    printf("2. Ajouter des conges\n");
    printf("3. Retour\n");
    printf("Votre choix: ");

    int choix;
    scanf("%d", &choix);
    viderBuffer();

    if (choix == 1)
    {
        int jours;
        printf("Nombre de jours a prendre: ");
        scanf("%d", &jours);
        viderBuffer();

        if (jours <= e->jours_conges_restants)
        {
            e->jours_conges_restants -= jours;
            sauvegarderPersonnel();
            printf("Conges enregistres. Reste: %d jours\n", e->jours_conges_restants);
        }
        else
        {
            printf("Solde de conges insuffisant!\n");
        }
    }
    else if (choix == 2)
    {
        int jours;
        printf("Nombre de jours a ajouter: ");
        scanf("%d", &jours);
        viderBuffer();

        e->jours_conges_restants += jours;
        e->jours_conges_annuels += jours;
        sauvegarderPersonnel();
        printf("Conges ajoutes. Nouveau solde: %d jours\n", e->jours_conges_restants);
    }

    pause();
}

void afficherEmployesParService(char *service)
{
    system("cls");
    printf("\n=== PERSONNEL DU SERVICE: %s ===\n\n", service);

    int compteur = 0;
    for (int i = 0; i < nombreEmployes; i++)
    {
        if (strcmp(personnel[i].service, service) == 0 && personnel[i].actif)
        {
            printf("%s %s - %s\n", personnel[i].prenom, personnel[i].nom, personnel[i].fonction);
            compteur++;
        }
    }

    if (compteur == 0)
    {
        printf("Aucun employe actif dans ce service.\n");
    }
    else
    {
        printf("\nTotal: %d employe(s)\n", compteur);
    }

    pause();
}

void afficherEmployesParFonction(char *fonction)
{
    system("cls");
    printf("\n=== PERSONNEL - FONCTION: %s ===\n\n", fonction);

    int compteur = 0;
    for (int i = 0; i < nombreEmployes; i++)
    {
        if (strcmp(personnel[i].fonction, fonction) == 0 && personnel[i].actif)
        {
            printf("%s %s - %s\n", personnel[i].prenom, personnel[i].nom, personnel[i].service);
            compteur++;
        }
    }

    if (compteur == 0)
    {
        printf("Aucun employe actif avec cette fonction.\n");
    }
    else
    {
        printf("\nTotal: %d employe(s)\n", compteur);
    }

    pause();
}

void afficherStatistiquesPersonnel()
{
    system("cls");
    printf("\n=== STATISTIQUES DU PERSONNEL ===\n\n");

    if (nombreEmployes == 0)
    {
        printf("Aucun employe enregistre.\n");
        pause();
        return;
    }

    printf("Nombre total d'employes: %d\n", nombreEmployes);

    int actifs = 0, inactifs = 0;
    int hommes = 0, femmes = 0;
    float totalSalaire = 0;

    int medecins = 0, infirmiers = 0, administratifs = 0;
    int techniciens = 0, chirurgiens = 0, radiologues = 0, pharmaciens = 0;
    int autres = 0;

    char services[MAX_PERSONNEL][MAX_SERVICE];
    int nb_services = 0;
    int comptes_services[MAX_PERSONNEL] = {0};

    for (int i = 0; i < nombreEmployes; i++)
    {
        if (personnel[i].actif) actifs++;
        else inactifs++;

        if (personnel[i].genre == 'M') hommes++;
        else if (personnel[i].genre == 'F') femmes++;

        totalSalaire += personnel[i].salaire;

        char fonction_temp[MAX_NAME];
        strcpy(fonction_temp, personnel[i].fonction);

        for (int j = 0; fonction_temp[j]; j++)
            fonction_temp[j] = tolower(fonction_temp[j]);

        if (strstr(fonction_temp, "medecin") != NULL || strstr(fonction_temp, "médecin") != NULL)
            medecins++;
        else if (strstr(fonction_temp, "infirmier") != NULL || strstr(fonction_temp, "infirmière") != NULL)
            infirmiers++;
        else if (strstr(fonction_temp, "administratif") != NULL || strstr(fonction_temp, "admin") != NULL)
            administratifs++;
        else if (strstr(fonction_temp, "technicien") != NULL || strstr(fonction_temp, "technicienne") != NULL)
            techniciens++;
        else if (strstr(fonction_temp, "chirurgien") != NULL)
            chirurgiens++;
        else if (strstr(fonction_temp, "radiologue") != NULL)
            radiologues++;
        else if (strstr(fonction_temp, "pharmacien") != NULL || strstr(fonction_temp, "pharmacienne") != NULL)
            pharmaciens++;
        else
            autres++;

        if (strlen(personnel[i].service) > 0)
        {
            int trouve = 0;
            for (int s = 0; s < nb_services; s++)
            {
                if (strcmp(services[s], personnel[i].service) == 0)
                {
                    comptes_services[s]++;
                    trouve = 1;
                    break;
                }
            }
            if (!trouve && nb_services < MAX_PERSONNEL)
            {
                strcpy(services[nb_services], personnel[i].service);
                comptes_services[nb_services] = 1;
                nb_services++;
            }
        }
    }

    printf("\n=== STATUT ===\n");
    printf("  Actifs: %d (%.1f%%)\n", actifs, (float)actifs/nombreEmployes*100);
    printf("  Inactifs: %d (%.1f%%)\n", inactifs, (float)inactifs/nombreEmployes*100);

    printf("\n=== REPARTITION PAR GENRE ===\n");
    printf("  Hommes: %d (%.1f%%)\n", hommes, (float)hommes/nombreEmployes*100);
    printf("  Femmes: %d (%.1f%%)\n", femmes, (float)femmes/nombreEmployes*100);

    printf("\n=== REPARTITION PAR FONCTION ===\n");
    printf("  Medecins: %d\n", medecins);
    printf("  Infirmiers: %d\n", infirmiers);
    printf("  Administratifs: %d\n", administratifs);
    printf("  Techniciens: %d\n", techniciens);
    printf("  Chirurgiens: %d\n", chirurgiens);
    printf("  Radiologues: %d\n", radiologues);
    printf("  Pharmaciens: %d\n", pharmaciens);
    if (autres > 0)
        printf("  Autres: %d\n", autres);

    printf("\n=== REPARTITION PAR SERVICE ===\n");
    if (nb_services > 0)
    {
        for (int s = 0; s < nb_services; s++)
        {
            printf("  %s: %d employe(s)\n", services[s], comptes_services[s]);
        }
    }
    else
    {
        printf("  Aucun service enregistre.\n");
    }

    printf("\n=== SALAIRES ===\n");
    printf("  Masse salariale mensuelle: %.2f €\n", totalSalaire);
    printf("  Salaire moyen: %.2f €\n", totalSalaire / nombreEmployes);

    int total_conges_annuels = 0;
    int total_conges_restants = 0;
    for (int i = 0; i < nombreEmployes; i++)
    {
        total_conges_annuels += personnel[i].jours_conges_annuels;
        total_conges_restants += personnel[i].jours_conges_restants;
    }

    printf("\n=== CONGES ===\n");
    printf("  Total conges annuels: %d jours\n", total_conges_annuels);
    printf("  Total conges restants: %d jours\n", total_conges_restants);
    printf("  Taux d'utilisation: %.1f%%\n",
           total_conges_annuels > 0 ? (float)(total_conges_annuels - total_conges_restants) / total_conges_annuels * 100 : 0);

    pause();
}

void menuAffichagePersonnel()
{
    int choix;

    do
    {
        system("cls");
        printf("\n=== AFFICHAGE DU PERSONNEL ===\n\n");
        printf("1. Liste simple du personnel\n");
        printf("2. Rechercher un employe\n");
        printf("3. Afficher par service\n");
        printf("4. Afficher par fonction\n");
        printf("5. Statistiques\n");
        printf("6. Retour\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
        case 1:
            afficherListeEmployesSimple();
            pause();
            break;
        case 2:
            rechercherEmploye();
            break;
        case 3:
        {
            char service[MAX_SERVICE];
            printf("Nom du service: ");
            fgets(service, MAX_SERVICE, stdin);
            service[strcspn(service, "\n")] = '\0';
            afficherEmployesParService(service);
            break;
        }
        case 4:
        {
            char fonction[50];
            printf("Fonction: ");
            fgets(fonction, 50, stdin);
            fonction[strcspn(fonction, "\n")] = '\0';
            afficherEmployesParFonction(fonction);
            break;
        }
        case 5:
            afficherStatistiquesPersonnel();
            break;
        case 6:
            return;
        default:
            printf("Choix invalide.\n");
            pause();
        }

    } while(choix != 6);
}

void menuGestionPersonnel()
{
    int choix;

    do
    {
        system("cls");
        color(11, 0);
        printf("\n=== GESTION DU PERSONNEL ===\n\n");
        color(7, 0);

        printf("1. Ajouter un employe\n");
        printf("2. Afficher/Rechercher\n");
        printf("3. Modifier un employe\n");
        printf("4. Gerer les conges\n");
        printf("5. Supprimer/Desactiver\n");
        printf("6. Statistiques\n");
        printf("7. Retour au menu principal\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
        case 1:
            ajouterEmploye();
            break;
        case 2:
            menuAffichagePersonnel();
            break;
        case 3:
            if (nombreEmployes == 0)
            {
                printf("Aucun employe a modifier.\n");
                pause();
                break;
            }
            afficherListeEmployesSimple();
            printf("\nEntrez l'ID de l'employe a modifier: ");
            int id;
            scanf("%d", &id);
            viderBuffer();
            int index = rechercherEmployeParID(id);
            if (index != -1)
            {
                modifierEmploye(index);
            }
            else
            {
                printf("Employe non trouve.\n");
                pause();
            }
            break;
        case 4:
            if (nombreEmployes == 0)
            {
                printf("Aucun employe enregistre.\n");
                pause();
                break;
            }
            afficherListeEmployesSimple();
            printf("\nEntrez l'ID de l'employe: ");
            int id_conge;
            scanf("%d", &id_conge);
            viderBuffer();
            int idx_conge = rechercherEmployeParID(id_conge);
            if (idx_conge != -1)
            {
                gererConges(idx_conge);
            }
            else
            {
                printf("Employe non trouve.\n");
                pause();
            }
            break;
        case 5:
            if (nombreEmployes == 0)
            {
                printf("Aucun employe a supprimer.\n");
                pause();
                break;
            }
            afficherListeEmployesSimple();
            printf("\nEntrez l'ID de l'employe: ");
            int id_supp;
            scanf("%d", &id_supp);
            viderBuffer();
            int idx_supp = rechercherEmployeParID(id_supp);
            if (idx_supp != -1)
            {
                supprimerEmploye(idx_supp);
            }
            else
            {
                printf("Employe non trouve.\n");
                pause();
            }
            break;
        case 6:
            afficherStatistiquesPersonnel();
            break;
        case 7:
            sauvegarderPersonnel();
            return;
        default:
            printf("Choix invalide.\n");
            pause();
        }

    } while(choix != 7);
}

// ==================== FONCTIONS UI POUR PERSONNEL ====================

void ui_afficherEcranPersonnel()
{
    ui_afficherEntete();
    ui_setColor(UI_COLOR_TITLE);
    printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                         GESTION DU PERSONNEL                                   ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    ui_resetColor();
    printf("║                                                                                ║\n");
    printf("║     [1]  Ajouter un employe                                                    ║\n");
    printf("║     [2]  Modifier un employe                                                   ║\n");
    printf("║     [3]  Supprimer un employe                                                  ║\n");
    printf("║     [4]  Rechercher un employe                                                 ║\n");
    printf("║     [5]  Lister tous les employes                                              ║\n");
    printf("║     [6]  Gerer les conges                                                      ║\n");
    printf("║     [7]  Statistiques personnel                                                ║\n");
    printf("║     [8]  Retour                                                                ║\n");
    printf("║                                                                                ║\n");
    ui_setColor(8);
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
    printf("\n    Votre choix: ");
}

void ui_afficherListePersonnel()
{
    ui_afficherEntete();
    ui_setColor(UI_COLOR_TITLE);
    printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                         LISTE DU PERSONNEL                                     ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    ui_resetColor();

    if (nombreEmployes == 0)
    {
        ui_setColor(UI_COLOR_WARNING);
        printf("║                                                                                ║\n");
        printf("║                        Aucun employe enregistre                                ║\n");
        printf("║                                                                                ║\n");
        ui_setColor(8);
        printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
        ui_resetColor();
        return;
    }

    ui_setColor(UI_COLOR_INFO);
    printf("║                                                                                ║\n");
    printf("║     ID  │ Nom & Prenom           │ Fonction      │ Service       │ Actif       ║\n");
    printf("║    ─────┼────────────────────────┼───────────────┼───────────────┼─────────────║\n");
    ui_resetColor();

    for (int i = 0; i < nombreEmployes; i++)
    {
        if (i % 2 == 0)
            ui_setColor(UI_COLOR_PATIENT);
        else
            ui_setColor(UI_COLOR_MEDECIN);

        printf("║    %4d │ %-10s %-10s │ %-13s │ %-13s │ %-9s ║\n",
               personnel[i].id,
               personnel[i].prenom,
               personnel[i].nom,
               personnel[i].fonction,
               personnel[i].service,
               personnel[i].actif ? "Actif" : "Inactif");
        ui_resetColor();
    }

    ui_setColor(8);
    printf("║                                                                                ║\n");
    printf("║    Total: %d employe(s)                                                        ║\n", nombreEmployes);
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
}

void ui_afficherEcranAjouterEmploye()
{
    ui_afficherEntete();
    ui_setColor(UI_COLOR_TITLE);
    printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                         AJOUT D'UN EMPLOYE                                     ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    ui_resetColor();
    printf("║                                                                                ║\n");
    printf("║    [1]  Ajouter un medecin                                                     ║\n");
    printf("║    [2]  Ajouter un infirmier                                                   ║\n");
    printf("║    [3]  Ajouter un administrateur                                              ║\n");
    printf("║    [4]  Ajouter un technicien                                                  ║\n");
    printf("║    [5]  Ajouter un chirurgien                                                  ║\n");
    printf("║    [6]  Ajouter un radiologue                                                  ║\n");
    printf("║    [7]  Ajouter un pharmacien                                                  ║\n");
    printf("║    [8]  Retour                                                                 ║\n");
    printf("║                                                                                ║\n");
    ui_setColor(8);
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
    printf("\n    Votre choix: ");
}

void ui_afficherEcranModifierEmploye()
{
    ui_afficherEntete();
    ui_setColor(UI_COLOR_TITLE);
    printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                         MODIFICATION D'UN EMPLOYE                              ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    ui_resetColor();
    printf("║                                                                                ║\n");
    printf("║    Entrez l'ID de l'employe a modifier:                                        ║\n");
    printf("║    ┌────────────────────────────────────────────────────────────────────────┐  ║\n");
    printf("║    │                                                                        │  ║\n");
    printf("║    └────────────────────────────────────────────────────────────────────────┘  ║\n");
    printf("║                                                                                ║\n");
    ui_setColor(8);
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
    printf("\n    ID: ");
}

void ui_afficherEcranGererConges()
{
    ui_afficherEntete();
    ui_setColor(UI_COLOR_TITLE);
    printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                         GESTION DES CONGES                                     ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    ui_resetColor();
    printf("║                                                                                ║\n");
    printf("║    Entrez l'ID de l'employe:                                                   ║\n");
    printf("║    ┌────────────────────────────────────────────────────────────────────────┐  ║\n");
    printf("║    │                                                                        │  ║\n");
    printf("║    └────────────────────────────────────────────────────────────────────────┘  ║\n");
    printf("║                                                                                ║\n");
    printf("║    [1]  Prendre des conges                                                     ║\n");
    printf("║    [2]  Ajouter des conges                                                     ║\n");
    printf("║    [3]  Retour                                                                 ║\n");
    printf("║                                                                                ║\n");
    ui_setColor(8);
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
    printf("\n    Votre choix: ");
}

void ui_afficherEcranStatistiquesPersonnel()
{
    ui_afficherEntete();
    ui_setColor(UI_COLOR_TITLE);
    printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║                      STATISTIQUES DU PERSONNEL                                 ║\n");
    printf("╠════════════════════════════════════════════════════════════════════════════════╣\n");
    ui_resetColor();

    if (nombreEmployes == 0)
    {
        ui_setColor(UI_COLOR_WARNING);
        printf("║                                                                                ║\n");
        printf("║                        Aucun employe enregistre                                ║\n");
        printf("║                                                                                ║\n");
        ui_setColor(8);
        printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
        ui_resetColor();
        return;
    }

    int actifs = 0, inactifs = 0;
    int hommes = 0, femmes = 0;
    float totalSalaire = 0;
    int medecins = 0, infirmiers = 0, autres_fonctions = 0;

    for (int i = 0; i < nombreEmployes; i++)
    {
        if (personnel[i].actif) actifs++;
        else inactifs++;
        if (personnel[i].genre == 'M') hommes++;
        else if (personnel[i].genre == 'F') femmes++;
        totalSalaire += personnel[i].salaire;

        if (strstr(personnel[i].fonction, "Medecin") != NULL) medecins++;
        else if (strstr(personnel[i].fonction, "Infirmier") != NULL) infirmiers++;
        else autres_fonctions++;
    }

    printf("║                                                                                ║\n");
    printf("║    ┌────────────────────────────────────────────────────────────────────────┐  ║\n");
    printf("║    │  STATUT                                                                │  ║\n");
    printf("║    ├────────────────────────────────────────────────────────────────────────┤  ║\n");
    printf("║    │  Total employes : %-51d │ ║\n", nombreEmployes);
    printf("║    │  Actifs          : %-51d │ ║\n", actifs);
    printf("║    │  Inactifs        : %-51d │ ║\n", inactifs);
    printf("║    └────────────────────────────────────────────────────────────────────────┘  ║\n");
    printf("║                                                                                ║\n");
    printf("║    ┌────────────────────────────────────────────────────────────────────────┐  ║\n");
    printf("║    │  REPARTITION PAR GENRE                                                 │  ║\n");
    printf("║    ├────────────────────────────────────────────────────────────────────────┤  ║\n");
    printf("║    │  Hommes          : %-51d │ ║\n", hommes);
    printf("║    │  Femmes          : %-51d │ ║\n", femmes);
    printf("║    └────────────────────────────────────────────────────────────────────────┘  ║\n");
    printf("║                                                                                ║\n");
    printf("║    ┌────────────────────────────────────────────────────────────────────────┐  ║\n");
    printf("║    │  REPARTITION PAR FONCTION                                              │  ║\n");
    printf("║    ├────────────────────────────────────────────────────────────────────────┤  ║\n");
    printf("║    │  Medecins        : %-51d │ ║\n", medecins);
    printf("║    │  Infirmiers      : %-51d │ ║\n", infirmiers);
    printf("║    │  Autres          : %-51d │ ║\n", autres_fonctions);
    printf("║    └────────────────────────────────────────────────────────────────────────┘  ║\n");
    printf("║                                                                                ║\n");
    printf("║    ┌────────────────────────────────────────────────────────────────────────┐  ║\n");
    printf("║    │  SALAIRES                                                              │  ║\n");
    printf("║    ├────────────────────────────────────────────────────────────────────────┤  ║\n");
    printf("║    │  Masse salariale : %-51.2f € │ ║\n", totalSalaire);
    printf("║    │  Salaire moyen   : %-51.2f € │ ║\n", totalSalaire / nombreEmployes);
    printf("║    └────────────────────────────────────────────────────────────────────────┘  ║\n");
    printf("║                                                                                ║\n");
    ui_setColor(8);
    printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
    ui_resetColor();
}
