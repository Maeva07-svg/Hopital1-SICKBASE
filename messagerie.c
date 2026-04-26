#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "messagerie.h"
#include "notification.h"
#include "utils.h"
#include "comptes.h"

// Définition des variables globales
Message messages[MAX_MESSAGES];
int nombreMessages = 0;
int dernierIDMessage = 0;

void initialiserMessagerie()
{
    chargerMessages();
}

void envoyerMessage(int id_expediteur, int id_destinataire, const char *contenu)
{
    if (nombreMessages >= MAX_MESSAGES)
    {
        printf("Erreur: Capacite maximale de messages atteinte.\n");
        pause();
        return;
    }

    Message m;
    memset(&m, 0, sizeof(Message));

    m.id_message = ++dernierIDMessage;
    m.id_expediteur = id_expediteur;
    m.id_destinataire = id_destinataire;
    strcpy(m.contenu, contenu);
    m.lu = 0;
    m.supprime_expediteur = 0;
    m.supprime_destinataire = 0;
    m.modifie = 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(m.date_envoi, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
    sprintf(m.heure_envoi, "%02d:%02d", tm.tm_hour, tm.tm_min);

    messages[nombreMessages] = m;
    nombreMessages++;

    sauvegarderMessages();

    printf("\n Message envoye!\n");

    // Envoyer une notification au destinataire
    char notif_msg[500];
    sprintf(notif_msg, "Nouveau message de %s",
            id_expediteur == utilisateur_actuel->id_compte ? "vous" : "un contact");
    envoyerNotification(id_destinataire, id_expediteur, "Message", notif_msg, 0);
}

void modifierMessage(int id_message, int id_expediteur, const char *nouveau_contenu)
{
    for (int i = 0; i < nombreMessages; i++)
    {
        if (messages[i].id_message == id_message && messages[i].id_expediteur == id_expediteur)
        {
            strcpy(messages[i].contenu, nouveau_contenu);
            messages[i].modifie = 1;

            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            sprintf(messages[i].date_modification, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
            sprintf(messages[i].heure_modification, "%02d:%02d", tm.tm_hour, tm.tm_min);

            sauvegarderMessages();
            printf("\n Message modifie (edite)\n");

            // Notification de modification
            char notif_msg[500];
            sprintf(notif_msg, "Un message a ete modifie");
            envoyerNotification(messages[i].id_destinataire, id_expediteur, "Message modifie", notif_msg, 0);
            return;
        }
    }
    printf("Message non trouve ou vous n'etes pas l'auteur.\n");
}

void supprimerMessage(int id_message, int id_utilisateur)
{
    for (int i = 0; i < nombreMessages; i++)
    {
        if (messages[i].id_message == id_message)
        {
            if (messages[i].id_expediteur == id_utilisateur)
            {
                messages[i].supprime_expediteur = 1;
                printf("Message supprime pour vous.\n");
            }
            else if (messages[i].id_destinataire == id_utilisateur)
            {
                messages[i].supprime_destinataire = 1;
                printf("Message supprime pour vous.\n");
            }

            if (messages[i].supprime_expediteur && messages[i].supprime_destinataire)
            {
                for (int j = i; j < nombreMessages - 1; j++)
                {
                    messages[j] = messages[j + 1];
                }
                nombreMessages--;
            }

            sauvegarderMessages();
            return;
        }
    }
    printf("Message non trouve.\n");
}

void supprimerConversation(int id_utilisateur, int id_contact)
{
    int supprimes = 0;
    for (int i = 0; i < nombreMessages; i++)
    {
        if ((messages[i].id_expediteur == id_utilisateur && messages[i].id_destinataire == id_contact) ||
            (messages[i].id_expediteur == id_contact && messages[i].id_destinataire == id_utilisateur))
        {
            if (messages[i].id_expediteur == id_utilisateur)
                messages[i].supprime_expediteur = 1;
            if (messages[i].id_destinataire == id_utilisateur)
                messages[i].supprime_destinataire = 1;
            supprimes++;
        }
    }

    if (supprimes > 0)
    {
        sauvegarderMessages();
        printf(" Conversation supprimee.\n");
    }
    else
    {
        printf("Aucune conversation trouvee.\n");
    }
}

void afficherConversations(int id_utilisateur)
{
    system("cls");
    printf("\n==========================================================\n");
    printf("|                     MESSAGERIE (SICKBASE)                  |\n");

    // Créer une liste unique de contacts
    int contacts[MAX_CONTACTS];
    char contacts_nom[MAX_CONTACTS][100];
    char dernier_msg[MAX_CONTACTS][MAX_MSG_CONTENU];
    char dernier_date[MAX_CONTACTS][30];
    int nb_non_lus[MAX_CONTACTS] = {0};
    int id_dernier_msg[MAX_CONTACTS] = {0};
    int nb_contacts = 0;

    for (int i = 0; i < nombreMessages; i++)
    {
        int id_contact = -1;

        if (messages[i].id_expediteur == id_utilisateur && !messages[i].supprime_expediteur)
            id_contact = messages[i].id_destinataire;
        else if (messages[i].id_destinataire == id_utilisateur && !messages[i].supprime_destinataire)
            id_contact = messages[i].id_expediteur;
        else
            continue;

        // Ajouter le contact s'il n'existe pas
        int existe = 0;
        for (int j = 0; j < nb_contacts; j++)
        {
            if (contacts[j] == id_contact)
            {
                existe = 1;
                // Compter les messages non lus
                if (messages[i].id_destinataire == id_utilisateur && messages[i].lu == 0)
                    nb_non_lus[j]++;

                // Mettre à jour le dernier message si plus récent
                if (messages[i].id_message > id_dernier_msg[j])
                {
                    id_dernier_msg[j] = messages[i].id_message;
                    strcpy(dernier_msg[j], messages[i].contenu);
                    sprintf(dernier_date[j], "%s %s", messages[i].date_envoi, messages[i].heure_envoi);
                }
                break;
            }
        }

        if (!existe && nb_contacts < MAX_CONTACTS)
        {
            contacts[nb_contacts] = id_contact;
            // Trouver le nom du contact
            int trouve = 0;
            for (int k = 0; k < nombreComptes; k++)
            {
                if (comptes[k].id_compte == id_contact)
                {
                    strcpy(contacts_nom[nb_contacts], comptes[k].login);
                    trouve = 1;
                    break;
                }
            }
            if (!trouve)
                sprintf(contacts_nom[nb_contacts], "Utilisateur %d", id_contact);

            nb_non_lus[nb_contacts] = (messages[i].id_destinataire == id_utilisateur && messages[i].lu == 0) ? 1 : 0;
            strcpy(dernier_msg[nb_contacts], messages[i].contenu);
            sprintf(dernier_date[nb_contacts], "%s %s", messages[i].date_envoi, messages[i].heure_envoi);
            id_dernier_msg[nb_contacts] = messages[i].id_message;
            nb_contacts++;
        }
    }

    if (nb_contacts == 0)
    {
        printf(" Aucune conversation.\n");
        printf("\n1. Nouveau message\n");
        printf("2. Retour\n");
        printf("\nChoix: ");
        int choix;
        scanf("%d", &choix);
        viderBuffer();
        if (choix == 1)
        {
            envoyerMessageRapide(id_utilisateur);
            afficherConversations(id_utilisateur);
        }
        return;
    }

    // Trier les conversations par date (plus récente en premier)
    for (int i = 0; i < nb_contacts - 1; i++)
    {
        for (int j = i + 1; j < nb_contacts; j++)
        {
            if (id_dernier_msg[i] < id_dernier_msg[j])
            {
                // Échanger
                int temp_id = contacts[i];
                contacts[i] = contacts[j];
                contacts[j] = temp_id;

                char temp_nom[100];
                strcpy(temp_nom, contacts_nom[i]);
                strcpy(contacts_nom[i], contacts_nom[j]);
                strcpy(contacts_nom[j], temp_nom);

                char temp_msg[MAX_MSG_CONTENU];
                strcpy(temp_msg, dernier_msg[i]);
                strcpy(dernier_msg[i], dernier_msg[j]);
                strcpy(dernier_msg[j], temp_msg);

                char temp_date[30];
                strcpy(temp_date, dernier_date[i]);
                strcpy(dernier_date[i], dernier_date[j]);
                strcpy(dernier_date[j], temp_date);

                int temp_non_lus = nb_non_lus[i];
                nb_non_lus[i] = nb_non_lus[j];
                nb_non_lus[j] = temp_non_lus;

                int temp_id_msg = id_dernier_msg[i];
                id_dernier_msg[i] = id_dernier_msg[j];
                id_dernier_msg[j] = temp_id_msg;
            }
        }
    }

    printf("===============================================================\n");
    printf("| N- | Contact                  |  Dernier message           |\n");
    printf("|----|---------------------------|--------------------------|\n");

    for (int i = 0; i < nb_contacts; i++)
    {
        char non_lus_str[10] = "";
        if (nb_non_lus[i] > 0)
            sprintf(non_lus_str, " (%d)", nb_non_lus[i]);

        printf("| %2d | %-26s %s | %.25s |\n",
               i + 1,
               contacts_nom[i],
               non_lus_str,
               dernier_msg[i]);
    }
    printf("------------------------------------------------------------\n");

    printf("\n1.  Ouvrir une conversation\n");
    printf("2.    Nouveau message\n");
    printf("3.    Rechercher un message\n");
    printf("4.    Supprimer une conversation\n");
    printf("5.    Retour\n");
    printf("\nChoix: ");

    int choix;
    scanf("%d", &choix);
    viderBuffer();

    switch(choix)
    {
        case 1:
            {
                printf("Numero de la conversation: ");
                int num;
                scanf("%d", &num);
                viderBuffer();
                if (num >= 1 && num <= nb_contacts)
                {
                    afficherConversation(id_utilisateur, contacts[num-1]);
                }
                else
                {
                    printf("Numero invalide.\n");
                    pause();
                }
                afficherConversations(id_utilisateur);
            }
            break;
        case 2:
            envoyerMessageRapide(id_utilisateur);
            afficherConversations(id_utilisateur);
            break;
        case 3:
            {
                char mot[100];
                printf("Mot a rechercher: ");
                fgets(mot, 100, stdin);
                mot[strcspn(mot, "\n")] = '\0';
                rechercherMessage(mot);
                afficherConversations(id_utilisateur);
            }
            break;
        case 4:
            {
                printf("Numero de la conversation a supprimer: ");
                int num;
                scanf("%d", &num);
                viderBuffer();
                if (num >= 1 && num <= nb_contacts)
                {
                    supprimerConversation(id_utilisateur, contacts[num-1]);
                }
                pause();
                afficherConversations(id_utilisateur);
            }
            break;
        case 5:
            return;
        default:
            printf("Choix invalide.\n");
            pause();
            afficherConversations(id_utilisateur);
    }
}

int hasNouveauxMessages(int id_utilisateur)
{
    for (int i = 0; i < nombreMessages; i++)
    {
        if (messages[i].id_destinataire == id_utilisateur && messages[i].lu == 0 && !messages[i].supprime_destinataire)
        {
            return 1;
        }
    }
    return 0;
}

void afficherConversation(int id_utilisateur, int id_contact)
{
    system("cls");

    // Trouver le nom du contact
    char contact_nom[100] = "";
    for (int i = 0; i < nombreComptes; i++)
    {
        if (comptes[i].id_compte == id_contact)
        {
            strcpy(contact_nom, comptes[i].login);
            break;
        }
    }
    if (strlen(contact_nom) == 0)
        sprintf(contact_nom, "Utilisateur %d", id_contact);

    printf("\n==========================================================\n");
    printf("||                     Conversation avec %-30s ||\n", contact_nom);
    printf("==========================================================\\n");

    // Afficher tous les messages de la conversation
    int messages_affiches = 0;

    for (int i = 0; i < nombreMessages; i++)
    {
        if (((messages[i].id_expediteur == id_utilisateur && messages[i].id_destinataire == id_contact && !messages[i].supprime_expediteur) ||
             (messages[i].id_expediteur == id_contact && messages[i].id_destinataire == id_utilisateur && !messages[i].supprime_destinataire)))
        {
            messages_affiches++;

            // Marquer comme lu
            if (messages[i].id_destinataire == id_utilisateur && messages[i].lu == 0)
            {
                messages[i].lu = 1;
            }

            // Afficher le message
            if (messages[i].id_expediteur == id_utilisateur)
                printf("== Moi (%s %s)\n", messages[i].date_envoi, messages[i].heure_envoi);
            else
                printf("== %s (%s %s)\n", contact_nom, messages[i].date_envoi, messages[i].heure_envoi);

            printf("| %s\n", messages[i].contenu);

            if (messages[i].modifie)
                printf("| (modifie le %s %s)\n", messages[i].date_modification, messages[i].heure_modification);

            printf("| [ID: %d]\n", messages[i].id_message);
            printf("==========================================================\n");
        }
    }

    if (messages_affiches == 0)
    {
        printf(" Aucun message dans cette conversation.\n\n");
    }

    printf("\n==========================================================\n");
    printf("| 1.  Ecrire un message                                    |\n");
    printf("| 2.  Modifier un message                                  |\n");
    printf("| 3.  Supprimer un message                                 |\n");
    printf("| 4.  Retour                                               |\n");
    printf("========================================================== \n");
    printf("\nChoix: ");

    int choix;
    scanf("%d", &choix);
    viderBuffer();

    switch(choix)
    {
        case 1:
            {
                printf("\n Votre message: ");
                char msg[MAX_MSG_CONTENU];
                fgets(msg, MAX_MSG_CONTENU, stdin);
                msg[strcspn(msg, "\n")] = '\0';
                if (strlen(msg) > 0)
                {
                    envoyerMessage(id_utilisateur, id_contact, msg);
                }
                afficherConversation(id_utilisateur, id_contact);
            }
            break;
        case 2:
            {
                int id_msg;
                printf("ID du message a modifier: ");
                scanf("%d", &id_msg);
                viderBuffer();
                printf("Nouveau contenu: ");
                char nouveau[MAX_MSG_CONTENU];
                fgets(nouveau, MAX_MSG_CONTENU, stdin);
                nouveau[strcspn(nouveau, "\n")] = '\0';
                modifierMessage(id_msg, id_utilisateur, nouveau);
                afficherConversation(id_utilisateur, id_contact);
            }
            break;
        case 3:
            {
                int id_msg;
                printf("ID du message a supprimer: ");
                scanf("%d", &id_msg);
                viderBuffer();
                supprimerMessage(id_msg, id_utilisateur);
                afficherConversation(id_utilisateur, id_contact);
            }
            break;
        case 4:
            return;
        default:
            printf("Choix invalide.\n");
            pause();
            afficherConversation(id_utilisateur, id_contact);
    }
}

void envoyerMessageRapide(int id_expediteur)
{
    system("cls");
    printf("\n=== NOUVEAU MESSAGE ===\n\n");

    // Afficher la liste des utilisateurs
    printf("LISTE DES UTILISATEURS:\n");
    printf("ID   | Login\n");
    printf("-----|------------------\n");
    for (int i = 0; i < nombreComptes; i++)
    {
        if (comptes[i].id_compte != id_expediteur && comptes[i].actif)
        {
            printf("%-4d | %s\n", comptes[i].id_compte, comptes[i].login);
        }
    }

    printf("\nID du destinataire: ");
    int id_dest;
    scanf("%d", &id_dest);
    viderBuffer();

    // Vérifier si le destinataire existe
    int existe = 0;
    for (int i = 0; i < nombreComptes; i++)
    {
        if (comptes[i].id_compte == id_dest && comptes[i].actif)
        {
            existe = 1;
            break;
        }
    }

    if (!existe)
    {
        printf("Destinataire non trouve.\n");
        pause();
        return;
    }

    printf("Message: ");
    char msg[MAX_MSG_CONTENU];
    fgets(msg, MAX_MSG_CONTENU, stdin);
    msg[strcspn(msg, "\n")] = '\0';

    if (strlen(msg) > 0)
    {
        envoyerMessage(id_expediteur, id_dest, msg);
    }
}

void rechercherMessage(char *mot_cle)
{
    system("cls");
    printf("\n=== RECHERCHE DE MESSAGES ===\n");
    printf("Mot cle: %s\n\n", mot_cle);

    int compteur = 0;
    for (int i = 0; i < nombreMessages; i++)
    {
        if (strstr(messages[i].contenu, mot_cle) != NULL)
        {
            compteur++;
            printf("[%d] %s -> %s: %s\n",
                   messages[i].id_message,
                   messages[i].date_envoi,
                   messages[i].id_expediteur == utilisateur_actuel->id_compte ? "Moi" : "Contact",
                   messages[i].contenu);
        }
    }

    if (compteur == 0)
    {
        printf("Aucun message trouve contenant '%s'.\n", mot_cle);
    }
    else
    {
        printf("\nTotal: %d message(s) trouve(s)\n", compteur);
    }

    pause();
}

void menuMessagerie(int id_utilisateur)
{
    afficherConversations(id_utilisateur);
}

// ================= SAUVEGARDE =================

void sauvegarderMessages()
{
    FILE *f = fopen("messages.dat", "wb");
    if (f == NULL) return;

    fwrite(&nombreMessages, sizeof(int), 1, f);
    fwrite(messages, sizeof(Message), nombreMessages, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_message.dat", "wb");
    if (fid != NULL)
    {
        fwrite(&dernierIDMessage, sizeof(int), 1, fid);
        fclose(fid);
    }
}

void chargerMessages()
{
    FILE *f = fopen("messages.dat", "rb");
    if (f == NULL) return;

    fread(&nombreMessages, sizeof(int), 1, f);
    fread(messages, sizeof(Message), nombreMessages, f);
    fclose(f);

    FILE *fid = fopen("dernier_id_message.dat", "rb");
    if (fid != NULL)
    {
        fread(&dernierIDMessage, sizeof(int), 1, fid);
        fclose(fid);
    }
}
