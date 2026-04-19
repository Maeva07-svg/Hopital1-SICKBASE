#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "pharmacie.h"
#include "utils.h"
#include "patient.h"
#include "ordonnances.h"
#include "comptes.h"

// Définition des variables globales
MedicamentStock medicaments[MAX_MEDICAMENTS_STOCK];
int nombreMedicaments = 0;
int dernierIDMedicament = 0;
VenteMedicament ventes[MAX_MEDICAMENTS_STOCK];
int nombreVentes = 0;
int dernierIDVente = 0;

void initialiserPharmacie()
{
    chargerMedicaments();
    chargerVentes();

    if (nombreMedicaments == 0)
    {
        dernierIDMedicament = 0;
    }
}

// ================= GESTION DES MÉDICAMENTS =================

void ajouterMedicamentStock()
{
    system("cls");
    printf("\n=== AJOUT D'UN MEDICAMENT ===\n\n");

    if (nombreMedicaments >= MAX_MEDICAMENTS_STOCK)
    {
        printf("Capacite maximale atteinte!\n");
        pause();
        return;
    }

    MedicamentStock m;
    memset(&m, 0, sizeof(MedicamentStock));

    m.id = ++dernierIDMedicament;
    m.actif = 1;

    printf("Nom du medicament: ");
    fgets(m.nom, MAX_NOM_MEDICAMENT, stdin);
    m.nom[strcspn(m.nom, "\n")] = '\0';

    printf("Categorie (Antibiotique/Antalgique/etc.): ");
    fgets(m.categorie, MAX_CATEGORIE, stdin);
    m.categorie[strcspn(m.categorie, "\n")] = '\0';

    printf("Forme (Comprime/Sirop/Injectable/etc.): ");
    fgets(m.forme, MAX_FORME, stdin);
    m.forme[strcspn(m.forme, "\n")] = '\0';

    printf("Dosage (ex: 500mg, 1g, etc.): ");
    fgets(m.dosage, 50, stdin);
    m.dosage[strcspn(m.dosage, "\n")] = '\0';

    printf("Quantite en stock: ");
    scanf("%d", &m.quantite_stock);
    viderBuffer();

    printf("Seuil d'alerte: ");
    scanf("%d", &m.seuil_alerte);
    viderBuffer();

    printf("Prix unitaire (FCFA): ");
    scanf("%f", &m.prix_unitaire);
    viderBuffer();

    printf("Prix rembourse (FCFA): ");
    scanf("%f", &m.prix_rembourse);
    viderBuffer();

    printf("Date de peremption (JJ/MM/AAAA): ");
    fgets(m.date_peremption, 20, stdin);
    m.date_peremption[strcspn(m.date_peremption, "\n")] = '\0';

    printf("Fournisseur: ");
    fgets(m.fournisseur, MAX_FOURNISSEUR, stdin);
    m.fournisseur[strcspn(m.fournisseur, "\n")] = '\0';

    printf("Ordonnance requise? (O/N): ");
    char rep;
    scanf("%c", &rep);
    viderBuffer();
    m.ordonnance_requise = (toupper(rep) == 'O') ? 1 : 0;

    printf("Notes: ");
    fgets(m.notes, 200, stdin);
    m.notes[strcspn(m.notes, "\n")] = '\0';

    medicaments[nombreMedicaments] = m;
    nombreMedicaments++;

    sauvegarderMedicaments();

    printf("\n=== MEDICAMENT AJOUTE AVEC SUCCES ===\n");
    printf("ID: %d\n", m.id);
    printf("Nom: %s\n", m.nom);
    printf("Stock initial: %d\n", m.quantite_stock);

    pause();
}

void modifierMedicamentStock(int id)
{
    for (int i = 0; i < nombreMedicaments; i++)
    {
        if (medicaments[i].id == id)
        {
            MedicamentStock *m = &medicaments[i];
            int choix;

            do
            {
                system("cls");
                printf("\n=== MODIFICATION MEDICAMENT: %s ===\n", m->nom);
                printf("1. Nom (%s)\n", m->nom);
                printf("2. Categorie (%s)\n", m->categorie);
                printf("3. Forme (%s)\n", m->forme);
                printf("4. Dosage (%s)\n", m->dosage);
                printf("5. Prix unitaire (%.0f FCFA)\n", m->prix_unitaire);
                printf("6. Quantite stock (%d)\n", m->quantite_stock);
                printf("7. Seuil alerte (%d)\n", m->seuil_alerte);
                printf("8. Date peremption (%s)\n", m->date_peremption);
                printf("9. Fournisseur (%s)\n", m->fournisseur);
                printf("10. Ordonnance requise (%s)\n", m->ordonnance_requise ? "Oui" : "Non");
                printf("11. Notes\n");
                printf("12. Sauvegarder\n");
                printf("13. Annuler\n");
                printf("\nChoix: ");
                scanf("%d", &choix);
                viderBuffer();

                switch(choix)
                {
                    case 1:
                        printf("Nouveau nom: ");
                        fgets(m->nom, MAX_NOM_MEDICAMENT, stdin);
                        m->nom[strcspn(m->nom, "\n")] = '\0';
                        break;
                    case 2:
                        printf("Nouvelle categorie: ");
                        fgets(m->categorie, MAX_CATEGORIE, stdin);
                        m->categorie[strcspn(m->categorie, "\n")] = '\0';
                        break;
                    case 3:
                        printf("Nouvelle forme: ");
                        fgets(m->forme, MAX_FORME, stdin);
                        m->forme[strcspn(m->forme, "\n")] = '\0';
                        break;
                    case 4:
                        printf("Nouveau dosage: ");
                        fgets(m->dosage, 50, stdin);
                        m->dosage[strcspn(m->dosage, "\n")] = '\0';
                        break;
                    case 5:
                        printf("Nouveau prix unitaire: ");
                        scanf("%f", &m->prix_unitaire);
                        viderBuffer();
                        break;
                    case 6:
                        printf("Nouvelle quantite: ");
                        scanf("%d", &m->quantite_stock);
                        viderBuffer();
                        break;
                    case 7:
                        printf("Nouveau seuil alerte: ");
                        scanf("%d", &m->seuil_alerte);
                        viderBuffer();
                        break;
                    case 8:
                        printf("Nouvelle date peremption: ");
                        fgets(m->date_peremption, 20, stdin);
                        m->date_peremption[strcspn(m->date_peremption, "\n")] = '\0';
                        break;
                    case 9:
                        printf("Nouveau fournisseur: ");
                        fgets(m->fournisseur, MAX_FOURNISSEUR, stdin);
                        m->fournisseur[strcspn(m->fournisseur, "\n")] = '\0';
                        break;
                    case 10:
                        m->ordonnance_requise = !m->ordonnance_requise;
                        break;
                    case 11:
                        printf("Nouvelles notes: ");
                        fgets(m->notes, 200, stdin);
                        m->notes[strcspn(m->notes, "\n")] = '\0';
                        break;
                    case 12:
                        sauvegarderMedicaments();
                        printf("Modifications sauvegardees!\n");
                        pause();
                        return;
                    case 13:
                        return;
                }
            } while(1);
        }
    }
    printf("Medicament non trouve.\n");
    pause();
}

void supprimerMedicamentStock(int id)
{
    for (int i = 0; i < nombreMedicaments; i++)
    {
        if (medicaments[i].id == id)
        {
            printf("\n=== SUPPRESSION MEDICAMENT ===\n");
            printf("Medicament: %s\n", medicaments[i].nom);
            printf("Confirmer la suppression? (O/N): ");
            char conf;
            scanf("%c", &conf);
            viderBuffer();

            if (toupper(conf) == 'O')
            {
                for (int j = i; j < nombreMedicaments - 1; j++)
                {
                    medicaments[j] = medicaments[j + 1];
                }
                nombreMedicaments--;
                sauvegarderMedicaments();
                printf("Medicament supprime.\n");
            }
            else
            {
                printf("Suppression annulee.\n");
            }
            pause();
            return;
        }
    }
    printf("Medicament non trouve.\n");
    pause();
}

void afficherStock()
{
    system("cls");
    printf("\n=== STOCK DES MEDICAMENTS ===\n\n");

    if (nombreMedicaments == 0)
    {
        printf("Aucun medicament en stock.\n");
        pause();
        return;
    }

    printf("ID  | Nom                     | Stock  | Seuil | Prix    | Peremption\n");
    printf("----|-------------------------|--------|-------|---------|------------\n");

    for (int i = 0; i < nombreMedicaments; i++)
    {
        if (medicaments[i].quantite_stock <= medicaments[i].seuil_alerte)
            color(12, 0);
        else
            color(7, 0);

        printf("%-4d| %-23s | %6d | %5d | %7.0f | %s\n",
               medicaments[i].id,
               medicaments[i].nom,
               medicaments[i].quantite_stock,
               medicaments[i].seuil_alerte,
               medicaments[i].prix_unitaire,
               medicaments[i].date_peremption);
    }

    color(7, 0);
    printf("\nTotal: %d medicament(s)\n", nombreMedicaments);
    pause();
}

void afficherStockComplet()
{
    system("cls");
    printf("\n=== DETAIL COMPLET DU STOCK ===\n\n");

    if (nombreMedicaments == 0)
    {
        printf("Aucun medicament en stock.\n");
        pause();
        return;
    }

    for (int i = 0; i < nombreMedicaments; i++)
    {
        printf("\n========================================\n");
        printf("MEDICAMENT #%d\n", medicaments[i].id);
        printf("========================================\n");
        printf("Nom: %s\n", medicaments[i].nom);
        printf("Categorie: %s\n", medicaments[i].categorie);
        printf("Forme: %s\n", medicaments[i].forme);
        printf("Dosage: %s\n", medicaments[i].dosage);
        printf("Quantite stock: %d\n", medicaments[i].quantite_stock);
        printf("Seuil alerte: %d\n", medicaments[i].seuil_alerte);
        printf("Prix unitaire: %.0f FCFA\n", medicaments[i].prix_unitaire);
        printf("Prix rembourse: %.0f FCFA\n", medicaments[i].prix_rembourse);
        printf("Date peremption: %s\n", medicaments[i].date_peremption);
        printf("Fournisseur: %s\n", medicaments[i].fournisseur);
        printf("Ordonnance requise: %s\n", medicaments[i].ordonnance_requise ? "Oui" : "Non");
        if (strlen(medicaments[i].notes) > 0)
            printf("Notes: %s\n", medicaments[i].notes);
    }
    pause();
}

void rechercherMedicamentStock(char *nom)
{
    system("cls");
    printf("\n=== RECHERCHE MEDICAMENT ===\n");
    printf("Recherche: %s\n\n", nom);

    int trouve = 0;
    char nom_recherche[MAX_NOM_MEDICAMENT];
    strcpy(nom_recherche, nom);
    for (int i = 0; nom_recherche[i]; i++)
        nom_recherche[i] = tolower(nom_recherche[i]);

    for (int i = 0; i < nombreMedicaments; i++)
    {
        char nom_med[MAX_NOM_MEDICAMENT];
        strcpy(nom_med, medicaments[i].nom);
        for (int j = 0; nom_med[j]; j++)
            nom_med[j] = tolower(nom_med[j]);

        if (strstr(nom_med, nom_recherche) != NULL)
        {
            trouve = 1;
            printf("\nID: %d\n", medicaments[i].id);
            printf("Nom: %s\n", medicaments[i].nom);
            printf("Categorie: %s\n", medicaments[i].categorie);
            printf("Stock: %d\n", medicaments[i].quantite_stock);
            printf("Prix: %.0f FCFA\n", medicaments[i].prix_unitaire);
            printf("Peremption: %s\n", medicaments[i].date_peremption);
        }
    }

    if (!trouve)
        printf("Aucun medicament trouve.\n");

    pause();
}

void alerteStockFaible()
{
    system("cls");
    printf("\n=== ALERTES STOCK FAIBLE ===\n\n");

    int alerte = 0;
    printf("Medicaments avec stock <= seuil d'alerte:\n\n");

    for (int i = 0; i < nombreMedicaments; i++)
    {
        if (medicaments[i].quantite_stock <= medicaments[i].seuil_alerte)
        {
            alerte = 1;
            color(12, 0);
            printf("⚠️  %s\n", medicaments[i].nom);
            printf("   Stock actuel: %d / Seuil: %d\n",
                   medicaments[i].quantite_stock, medicaments[i].seuil_alerte);
            printf("   Fournisseur: %s\n", medicaments[i].fournisseur);
            color(7, 0);
            printf("\n");
        }
    }

    if (!alerte)
    {
        printf("Aucun stock faible detecte.\n");
    }

    pause();
}

void approvisionnerMedicament(int id)
{
    for (int i = 0; i < nombreMedicaments; i++)
    {
        if (medicaments[i].id == id)
        {
            printf("\n=== APPROVISIONNEMENT ===\n");
            printf("Medicament: %s\n", medicaments[i].nom);
            printf("Stock actuel: %d\n", medicaments[i].quantite_stock);

            int quantite;
            printf("Quantite a ajouter: ");
            scanf("%d", &quantite);
            viderBuffer();

            medicaments[i].quantite_stock += quantite;
            sauvegarderMedicaments();

            printf("Nouveau stock: %d\n", medicaments[i].quantite_stock);
            pause();
            return;
        }
    }
    printf("Medicament non trouve.\n");
    pause();
}

void modifierSeuilAlerte(int id)
{
    for (int i = 0; i < nombreMedicaments; i++)
    {
        if (medicaments[i].id == id)
        {
            printf("\n=== MODIFICATION SEUIL ALERTE ===\n");
            printf("Medicament: %s\n", medicaments[i].nom);
            printf("Seuil actuel: %d\n", medicaments[i].seuil_alerte);

            printf("Nouveau seuil: ");
            scanf("%d", &medicaments[i].seuil_alerte);
            viderBuffer();

            sauvegarderMedicaments();
            printf("Seuil modifie avec succes.\n");
            pause();
            return;
        }
    }
    printf("Medicament non trouve.\n");
    pause();
}

// ================= DÉLIVRANCE =================

void delivrerMedicament(int id_ordonnance, int id_patient)
{
    system("cls");
    printf("\n=== DELIVRANCE DE MEDICAMENTS ===\n\n");

    int idx_ord = -1;
    for (int i = 0; i < nombreOrdonnances; i++)
    {
        if (ordonnances[i].id_ordonnance == id_ordonnance)
        {
            idx_ord = i;
            break;
        }
    }

    if (idx_ord == -1)
    {
        printf("Ordonnance non trouvee.\n");
        pause();
        return;
    }

    if (ordonnances[idx_ord].id_patient != id_patient)
    {
        printf("Cette ordonnance n'appartient pas a ce patient.\n");
        pause();
        return;
    }

    printf("Ordonnance #%d du %s\n", id_ordonnance, ordonnances[idx_ord].date_prescription);
    printf("Patient ID: %d\n\n", id_patient);

    printf("Medicaments prescrits:\n");
    for (int i = 0; i < ordonnances[idx_ord].nb_medicaments; i++)
    {
        printf("  - %s %s - %s\n",
               ordonnances[idx_ord].medicaments[i].nom,
               ordonnances[idx_ord].medicaments[i].dosage,
               ordonnances[idx_ord].medicaments[i].posologie);
    }

    printf("\nVoulez-vous delivrer ces medicaments? (O/N): ");
    char conf;
    scanf("%c", &conf);
    viderBuffer();

    if (toupper(conf) == 'O')
    {
        for (int i = 0; i < ordonnances[idx_ord].nb_medicaments; i++)
        {
            char *nom_med = ordonnances[idx_ord].medicaments[i].nom;
            int trouve = 0;

            for (int j = 0; j < nombreMedicaments; j++)
            {
                if (strcmp(medicaments[j].nom, nom_med) == 0)
                {
                    trouve = 1;
                    if (medicaments[j].quantite_stock > 0)
                    {
                        medicaments[j].quantite_stock--;
                        enregistrerVente(medicaments[j].id, id_patient, 1);
                        printf("  - %s delivre. Stock restant: %d\n",
                               nom_med, medicaments[j].quantite_stock);
                    }
                    else
                    {
                        printf("  - %s: STOCK EPUISE!\n", nom_med);
                    }
                    break;
                }
            }
            if (!trouve)
            {
                printf("  - %s: Medicament non trouve en stock!\n", nom_med);
            }
        }

        sauvegarderMedicaments();
        printf("\nDelivrance terminee.\n");
    }
    else
    {
        printf("Delivrance annulee.\n");
    }

    pause();
}

void enregistrerVente(int id_medicament, int id_patient, int quantite)
{
    VenteMedicament v;
    memset(&v, 0, sizeof(VenteMedicament));

    v.id_vente = ++dernierIDVente;
    v.id_medicament = id_medicament;
    v.id_patient = id_patient;
    v.quantite = quantite;

    for (int i = 0; i < nombreMedicaments; i++)
    {
        if (medicaments[i].id == id_medicament)
        {
            v.prix_total = medicaments[i].prix_unitaire * quantite;
            break;
        }
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(v.date_vente, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    strcpy(v.statut, "Délivré");

    ventes[nombreVentes] = v;
    nombreVentes++;

    sauvegarderVentes();
}

void afficherVentesParPatient(int id_patient)
{
    system("cls");
    printf("\n=== VENTES PAR PATIENT ===\n\n");

    int compteur = 0;
    float total = 0;

    printf("ID  | Medicament              | Date       | Quantite | Prix\n");
    printf("----|-------------------------|------------|----------|--------\n");

    for (int i = 0; i < nombreVentes; i++)
    {
        if (ventes[i].id_patient == id_patient)
        {
            compteur++;
            total += ventes[i].prix_total;

            char nom_med[MAX_NOM_MEDICAMENT] = "Inconnu";
            for (int j = 0; j < nombreMedicaments; j++)
            {
                if (medicaments[j].id == ventes[i].id_medicament)
                {
                    strcpy(nom_med, medicaments[j].nom);
                    break;
                }
            }

            printf("%-4d| %-23s | %-10s | %7d | %7.0f\n",
                   ventes[i].id_vente,
                   nom_med,
                   ventes[i].date_vente,
                   ventes[i].quantite,
                   ventes[i].prix_total);
        }
    }

    if (compteur == 0)
    {
        printf("Aucune vente pour ce patient.\n");
    }
    else
    {
        printf("\nTotal: %d vente(s) - Montant total: %.0f FCFA\n", compteur, total);
    }

    pause();
}

void afficherVentesParMedicament(int id_medicament)
{
    system("cls");
    printf("\n=== VENTES PAR MEDICAMENT ===\n\n");

    int compteur = 0;
    int total_quantite = 0;
    float total_prix = 0;

    char nom_med[MAX_NOM_MEDICAMENT] = "Inconnu";
    for (int j = 0; j < nombreMedicaments; j++)
    {
        if (medicaments[j].id == id_medicament)
        {
            strcpy(nom_med, medicaments[j].nom);
            break;
        }
    }

    printf("Medicament: %s\n\n", nom_med);
    printf("Date       | Patient ID | Quantite | Prix\n");
    printf("------------|------------|----------|--------\n");

    for (int i = 0; i < nombreVentes; i++)
    {
        if (ventes[i].id_medicament == id_medicament)
        {
            compteur++;
            total_quantite += ventes[i].quantite;
            total_prix += ventes[i].prix_total;

            printf("%-10s | %10d | %7d | %7.0f\n",
                   ventes[i].date_vente,
                   ventes[i].id_patient,
                   ventes[i].quantite,
                   ventes[i].prix_total);
        }
    }

    if (compteur == 0)
    {
        printf("Aucune vente pour ce medicament.\n");
    }
    else
    {
        printf("\nTotal: %d vente(s) - Quantite totale: %d - Montant: %.0f FCFA\n",
               compteur, total_quantite, total_prix);
    }

    pause();
}

void statistiquesPharmacie()
{
    system("cls");
    printf("\n=== STATISTIQUES DE LA PHARMACIE ===\n\n");

    int stock_total = 0;
    int nb_stock_faible = 0;
    float valeur_stock = 0;

    for (int i = 0; i < nombreMedicaments; i++)
    {
        stock_total += medicaments[i].quantite_stock;
        valeur_stock += medicaments[i].quantite_stock * medicaments[i].prix_unitaire;
        if (medicaments[i].quantite_stock <= medicaments[i].seuil_alerte)
            nb_stock_faible++;
    }

    printf("=== STOCK ===\n");
    printf("  Nombre de medicaments: %d\n", nombreMedicaments);
    printf("  Unites totales en stock: %d\n", stock_total);
    printf("  Valeur totale du stock: %.0f FCFA\n", valeur_stock);
    printf("  Medicaments en stock faible: %d\n", nb_stock_faible);

    int total_ventes = 0;
    float chiffre_affaires = 0;

    for (int i = 0; i < nombreVentes; i++)
    {
        total_ventes++;
        chiffre_affaires += ventes[i].prix_total;
    }

    printf("\n=== VENTES ===\n");
    printf("  Nombre total de ventes: %d\n", total_ventes);
    printf("  Chiffre d'affaires: %.0f FCFA\n", chiffre_affaires);

    if (total_ventes > 0)
    {
        printf("  Panier moyen: %.0f FCFA\n", chiffre_affaires / total_ventes);
    }

    pause();
}

// ================= SOUS-MENU STOCK =================

void menuGestionStock()
{
    int choix;
    do
    {
        system("cls");
        color(10, 0);
        printf("\n=== GESTION DU STOCK ===\n\n");
        color(7, 0);

        printf("1. Ajouter un medicament\n");
        printf("2. Modifier un medicament\n");
        printf("3. Supprimer un medicament\n");
        printf("4. Afficher le stock\n");
        printf("5. Afficher details complets\n");
        printf("6. Rechercher un medicament\n");
        printf("7. Approvisionner un medicament\n");
        printf("8. Modifier seuil d'alerte\n");
        printf("9. Retour\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                ajouterMedicamentStock();
                break;
            case 2:
                {
                    afficherStock();
                    printf("\nID du medicament a modifier: ");
                    int id;
                    scanf("%d", &id);
                    viderBuffer();
                    modifierMedicamentStock(id);
                }
                break;
            case 3:
                {
                    afficherStock();
                    printf("\nID du medicament a supprimer: ");
                    int id;
                    scanf("%d", &id);
                    viderBuffer();
                    supprimerMedicamentStock(id);
                }
                break;
            case 4:
                afficherStock();
                break;
            case 5:
                afficherStockComplet();
                break;
            case 6:
                {
                    char nom[100];
                    printf("Nom du medicament: ");
                    fgets(nom, 100, stdin);
                    nom[strcspn(nom, "\n")] = '\0';
                    rechercherMedicamentStock(nom);
                }
                break;
            case 7:
                {
                    afficherStock();
                    printf("\nID du medicament: ");
                    int id;
                    scanf("%d", &id);
                    viderBuffer();
                    approvisionnerMedicament(id);
                }
                break;
            case 8:
                {
                    afficherStock();
                    printf("\nID du medicament: ");
                    int id;
                    scanf("%d", &id);
                    viderBuffer();
                    modifierSeuilAlerte(id);
                }
                break;
            case 9:
                return;
            default:
                printf("Choix invalide.\n");
                pause();
        }
    } while(choix != 9);
}

// ================= MENU PHARMACIEN CORRIGÉ =================

void menuPharmacien()
{
    int choix;

    do
    {
        system("cls");
        color(10, 0);
        printf("\n========================================\n");
        printf("          MODULE PHARMACIE              \n");
        printf("========================================\n\n");
        color(7, 0);

        printf("1. Gestion des ordonnances\n");
        printf("2. Ajouter un medicament\n");
        printf("3. Modifier un medicament\n");
        printf("4. Supprimer un medicament\n");
        printf("5. Afficher le stock\n");
        printf("6. Rechercher un medicament\n");
        printf("7. Approvisionner un medicament\n");
        printf("8. Alertes stock faible\n");
        printf("9. Delivrance de medicaments\n");
        printf("10. Ventes par patient\n");
        printf("11. Ventes par medicament\n");
        printf("12. Statistiques\n");
        printf("13. Modifier mot de passe\n");
        printf("14. Deconnexion\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                // Appel direct à la fonction de gestion des ordonnances
                // Sans passer par menuOrdonnances() qui a sa propre boucle
                {
                    // Ici tu peux appeler une fonction spécifique
                    // Ou afficher un message temporaire
                    printf("\nModule ordonnances (appel direct)\n");
                    printf("Fonctionnalite a developper\n");
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar();
                }
                break;
            case 2:
                ajouterMedicamentStock();
                break;
            case 3:
                {
                    afficherStock();
                    printf("\nID du medicament a modifier: ");
                    int id;
                    scanf("%d", &id);
                    viderBuffer();
                    modifierMedicamentStock(id);
                }
                break;
            case 4:
                {
                    afficherStock();
                    printf("\nID du medicament a supprimer: ");
                    int id;
                    scanf("%d", &id);
                    viderBuffer();
                    supprimerMedicamentStock(id);
                }
                break;
            case 5:
                afficherStock();
                break;
            case 6:
                {
                    char nom[100];
                    printf("Nom du medicament: ");
                    fgets(nom, 100, stdin);
                    nom[strcspn(nom, "\n")] = '\0';
                    rechercherMedicamentStock(nom);
                }
                break;
            case 7:
                {
                    afficherStock();
                    printf("\nID du medicament: ");
                    int id;
                    scanf("%d", &id);
                    viderBuffer();
                    approvisionnerMedicament(id);
                }
                break;
            case 8:
                alerteStockFaible();
                break;
            case 9:
                {
                    if (nombreOrdonnances == 0)
                    {
                        printf("Aucune ordonnance enregistree.\n");
                        printf("\nAppuyez sur Entree pour continuer...");
                        getchar();
                        break;
                    }
                    printf("\nID de l'ordonnance: ");
                    int id_ord;
                    scanf("%d", &id_ord);
                    viderBuffer();
                    printf("ID du patient: ");
                    int id_pat;
                    scanf("%d", &id_pat);
                    viderBuffer();
                    delivrerMedicament(id_ord, id_pat);
                }
                break;
            case 10:
                {
                    printf("ID du patient: ");
                    int id;
                    scanf("%d", &id);
                    viderBuffer();
                    afficherVentesParPatient(id);
                }
                break;
            case 11:
                {
                    afficherStock();
                    printf("\nID du medicament: ");
                    int id;
                    scanf("%d", &id);
                    viderBuffer();
                    afficherVentesParMedicament(id);
                }
                break;
            case 12:
                statistiquesPharmacie();
                break;
            case 13:
                modifierMotDePasse(utilisateur_actuel->id_compte);
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
                break;
            case 14:
                printf("Deconnexion...\n");
                return;
            default:
                printf("Choix invalide.\n");
                printf("\nAppuyez sur Entree pour continuer...");
                getchar();
        }

    } while(choix != 14);
}

// ================= SAUVEGARDE =================

void sauvegarderMedicaments()
{
    FILE *f = fopen("medicaments.dat", "wb");
    if (f == NULL) return;

    fwrite(&nombreMedicaments, sizeof(int), 1, f);
    fwrite(medicaments, sizeof(MedicamentStock), nombreMedicaments, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_medicament.dat", "wb");
    if (fid != NULL)
    {
        fwrite(&dernierIDMedicament, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void chargerMedicaments()
{
    FILE *f = fopen("medicaments.dat", "rb");
    if (f == NULL) return;

    fread(&nombreMedicaments, sizeof(int), 1, f);
    fread(medicaments, sizeof(MedicamentStock), nombreMedicaments, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_medicament.dat", "rb");
    if (fid != NULL)
    {
        fread(&dernierIDMedicament, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void sauvegarderVentes()
{
    FILE *f = fopen("ventes.dat", "wb");
    if (f == NULL) return;

    fwrite(&nombreVentes, sizeof(int), 1, f);
    fwrite(ventes, sizeof(VenteMedicament), nombreVentes, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_vente.dat", "wb");
    if (fid != NULL)
    {
        fwrite(&dernierIDVente, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void chargerVentes()
{
    FILE *f = fopen("ventes.dat", "rb");
    if (f == NULL) return;

    fread(&nombreVentes, sizeof(int), 1, f);
    fread(ventes, sizeof(VenteMedicament), nombreVentes, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_vente.dat", "rb");
    if (fid != NULL)
    {
        fread(&dernierIDVente, sizeof(int), 1, fid);
        fclose(fid);
    }
}
