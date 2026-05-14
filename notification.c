#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "notification.h"
#include "utils.h"
#include "comptes.h"

// Définition des variables globales
Notification notifications[MAX_NOTIFICATIONS];
int nombreNotifications = 0;
int dernierIDNotification = 0;

void initialiserNotifications()
{
    chargerNotifications();
}

void envoyerNotification(int id_destinataire, int id_expediteur, const char* type, const char* message, int priorite)
{
    if (nombreNotifications >= MAX_NOTIFICATIONS)
    {
        printf("Erreur: Capacite maximale de notifications atteinte.\n");
        return;
    }

    Notification n;
    memset(&n, 0, sizeof(Notification));

    n.id_notification = ++dernierIDNotification;
    n.id_destinataire = id_destinataire;
    n.id_expediteur = id_expediteur;
    strcpy(n.type, type);
    strcpy(n.message, message);
    n.lu = 0;
    n.priorite = priorite;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(n.date_envoi, "%02d/%02d/%04d %02d:%02d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900, tm.tm_hour, tm.tm_min);

    notifications[nombreNotifications] = n;
    nombreNotifications++;

    sauvegarderNotifications();

    // Afficher immédiatement la notification si le destinataire est l'utilisateur actuel
    if (utilisateur_actuel != NULL && utilisateur_actuel->id_compte == id_destinataire)
    {
        color(11, 0);
        printf("\n [%s] %s\n", type, message);
        color(7, 0);
    }

}

void envoyerNotificationSysteme(int id_destinataire, const char* type, const char* message)
{
    envoyerNotification(id_destinataire, 0, type, message, 0);
}

void notifierNouveauMessage(int id_destinataire, int id_expediteur, char *message_court)
{
    char msg[500];
    sprintf(msg, " Nouveau message: %.30s", message_court);
    envoyerNotification(id_destinataire, id_expediteur, "Message", msg, 0);
}

void afficherNotifications(int id_destinataire)
{
    system("cls");
    printf("\n=== MES NOTIFICATIONS ===\n\n");

    int compteur = 0;
    printf("ID  | Date                     | Type                | Message\n");
    printf("----|--------------------------|---------------------|----------------------------------------------\n");

    for (int i = 0; i < nombreNotifications; i++)
    {
        if (notifications[i].id_destinataire == id_destinataire)
        {
            compteur++;

            // Couleur selon priorité
            if (notifications[i].priorite == 1)
                color(12, 0);  // Rouge pour haute priorité
            else if (!notifications[i].lu)
                color(14, 0);  // Jaune pour non lu
            else
                color(7, 0);   // Blanc pour lu

            printf("%-4d| %-24s | %-19s | %.50s\n",
                   notifications[i].id_notification,
                   notifications[i].date_envoi,
                   notifications[i].type,
                   notifications[i].message);
            color(7, 0);
        }
    }

    if (compteur == 0)
    {
        printf("Aucune notification.\n");
    }
    else
    {
        printf("\nTotal: %d notification(s)\n", compteur);
        printf("\n1. Marquer une notification comme lue\n");
        printf("2. Marquer toutes comme lues\n");
        printf("3. Supprimer une notification\n");
        printf("4. Retour\n");
        printf("\nChoix: ");

        int choix;
        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                {
                    int id;
                    printf("ID notification: ");
                    scanf("%d", &id);
                    viderBuffer();
                    marquerNotificationCommeLue(id);
                    afficherNotifications(id_destinataire);
                }
                break;
            case 2:
                marquerToutesNotificationsCommeLues(id_destinataire);
                afficherNotifications(id_destinataire);
                break;
            case 3:
                {
                    int id;
                    printf("ID notification: ");
                    scanf("%d", &id);
                    viderBuffer();
                    supprimerNotification(id);
                    afficherNotifications(id_destinataire);
                }
                break;
            case 4:
                break;
        }
    }

    pause();
}

void afficherNotificationsNonLues(int id_destinataire)
{
    int compteur = 0;
    printf("\n NOTIFICATIONS NON LUES:\n");
    for (int i = 0; i < nombreNotifications; i++)
    {
        if (notifications[i].id_destinataire == id_destinataire && notifications[i].lu == 0)
        {
            compteur++;
            printf("  - %s: %s\n", notifications[i].type, notifications[i].message);
        }
    }
    if (compteur == 0)
    {
        printf("  Aucune notification non lue.\n");
    }
}

void marquerNotificationCommeLue(int id_notification)
{
    for (int i = 0; i < nombreNotifications; i++)
    {
        if (notifications[i].id_notification == id_notification)
        {
            notifications[i].lu = 1;
            sauvegarderNotifications();
            printf("Notification marquee comme lue.\n");
            return;
        }
    }
    printf("Notification non trouvee.\n");
}

void marquerToutesNotificationsCommeLues(int id_destinataire)
{
    int modifie = 0;
    for (int i = 0; i < nombreNotifications; i++)
    {
        if (notifications[i].id_destinataire == id_destinataire && notifications[i].lu == 0)
        {
            notifications[i].lu = 1;
            modifie = 1;
        }
    }
    if (modifie)
    {
        sauvegarderNotifications();
        printf("Toutes les notifications marquees comme lues.\n");
    }
    else
    {
        printf("Aucune notification non lue.\n");
    }
}

void supprimerNotification(int id_notification)
{
    for (int i = 0; i < nombreNotifications; i++)
    {
        if (notifications[i].id_notification == id_notification)
        {
            for (int j = i; j < nombreNotifications - 1; j++)
            {
                notifications[j] = notifications[j + 1];
            }
            nombreNotifications--;
            sauvegarderNotifications();
            printf("Notification supprimee.\n");
            return;
        }
    }
    printf("Notification non trouvee.\n");
}

int hasNotificationsNonLues(int id_destinataire)
{
    for (int i = 0; i < nombreNotifications; i++)
    {
        if (notifications[i].id_destinataire == id_destinataire && notifications[i].lu == 0)
        {
            return 1;
        }
    }
    return 0;
}

// ================= NOTIFICATIONS AUTOMATIQUES =================

void notifierRdvConfirme(int id_patient, int id_medecin, char *date, char *heure)
{
    char message[500];
    sprintf(message, "Votre rendez-vous avec le medecin ID %d est confirme pour le %s a %s.", id_medecin, date, heure);
    envoyerNotification(id_patient, id_medecin, "RDV Confirme", message, 0);

    // Notification pour le médecin
    sprintf(message, "Le patient ID %d a confirme son rendez-vous pour le %s a %s.", id_patient, date, heure);
    envoyerNotification(id_medecin, id_patient, "RDV Confirme", message, 0);
}

void notifierRdvAnnule(int id_patient, int id_medecin, char *date)
{
    char message[500];
    sprintf(message, "Votre rendez-vous du %s a ete annule.", date);
    envoyerNotification(id_patient, id_medecin, "RDV Annule", message, 1);

    sprintf(message, "Le patient ID %d a annule son rendez-vous du %s.", id_patient, date);
    envoyerNotification(id_medecin, id_patient, "RDV Annule", message, 1);
}

void notifierResultatAnalyse(int id_patient, char *analyse, char *resultat)
{
    char message[500];
    sprintf(message, "Les resultats de votre analyse '%s' sont disponibles. Resultat: %s", analyse, resultat);
    envoyerNotificationSysteme(id_patient, "Resultat Analyse", message);
}

void notifierOrdonnanceDisponible(int id_patient, int id_ordonnance)
{
    char message[500];
    sprintf(message, "Votre ordonnance #%d est disponible en pharmacie.", id_ordonnance);
    envoyerNotificationSysteme(id_patient, "Ordonnance", message);
}

void notifierFactureEmise(int id_patient, char *numero_facture, float montant)
{
    char message[500];
    sprintf(message, "Une facture de %.0f FCFA a ete emise. Numero: %s", montant, numero_facture);
    envoyerNotificationSysteme(id_patient, "Facture", message);
}

void notifierSoinPrescrit(int id_patient, int id_medecin, char *type_soin)
{
    char message[500];
    sprintf(message, "Un soin de type '%s' vous a ete prescrit par le medecin ID %d.", type_soin, id_medecin);
    envoyerNotification(id_patient, id_medecin, "Soin Prescrit", message, 0);

    // Notification pour l'infirmier
    // Rechercher un infirmier disponible (simplifié)
    for (int i = 0; i < nombreEmployes; i++)
    {
        if (personnel[i].actif && strcmp(personnel[i].fonction, "Infirmier") == 0)
        {
            sprintf(message, "Un soin de type '%s' a ete prescrit pour le patient ID %d.", type_soin, id_patient);
            envoyerNotification(personnel[i].id, id_medecin, "Soin a Realiser", message, 1);
            break;
        }
    }
}

void notifierSoinRealise(int id_patient, int id_infirmier, char *type_soin)
{
    char message[500];
    sprintf(message, "Le soin de type '%s' a ete realise par l'infirmier ID %d.", type_soin, id_infirmier);
    envoyerNotification(id_patient, id_infirmier, "Soin Realise", message, 0);
}

void notifierRappelConsultation(int id_patient, char *date, char *heure)
{
    char message[500];
    sprintf(message, "RAPPEL: Vous avez une consultation le %s a %s. Veuillez vous presenter a l'accueil.", date, heure);
    envoyerNotificationSysteme(id_patient, "Rappel Consultation", message);
}

// ================= SAUVEGARDE =================

void sauvegarderNotifications()
{
    FILE *f = fopen("notifications.dat", "wb");
    if (f == NULL) return;

    fwrite(&nombreNotifications, sizeof(int), 1, f);
    fwrite(notifications, sizeof(Notification), nombreNotifications, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_notification.dat", "wb");
    if (fid != NULL)
    {
        fwrite(&dernierIDNotification, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void chargerNotifications()
{
    FILE *f = fopen("notifications.dat", "rb");
    if (f == NULL) return;

    fread(&nombreNotifications, sizeof(int), 1, f);
    fread(notifications, sizeof(Notification), nombreNotifications, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_notification.dat", "rb");
    if (fid != NULL)
    {
        fread(&dernierIDNotification, sizeof(int), 1, fid);
        fclose(fid);
    }
}
