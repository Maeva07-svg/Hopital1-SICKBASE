#ifndef MESSAGERIE_H
#define MESSAGERIE_H

#include "utils.h"
#include "comptes.h"
#include "notification.h"

#define MAX_MESSAGES 5000
#define MAX_MSG_CONTENU 500
#define MAX_CONTACTS 100

// Structure pour un message (comme WhatsApp)
typedef struct
{
    int id_message;
    int id_expediteur;
    int id_destinataire;
    char contenu[MAX_MSG_CONTENU];
    char date_envoi[20];
    char heure_envoi[10];
    int lu;                     // 0 = non lu, 1 = lu
    int supprime_expediteur;
    int supprime_destinataire;
    int modifie;                // 1 = message modifié
    char date_modification[20];
    char heure_modification[10];
} Message;

// Variables globales
extern Message messages[MAX_MESSAGES];
extern int nombreMessages;
extern int dernierIDMessage;

// Prototypes
void initialiserMessagerie();
void menuMessagerie(int id_utilisateur);
void afficherConversations(int id_utilisateur);
void afficherConversation(int id_utilisateur, int id_contact);
void envoyerMessage(int id_expediteur, int id_destinataire, const char *contenu);
void modifierMessage(int id_message, int id_expediteur, const char *nouveau_contenu);
void supprimerMessage(int id_message, int id_utilisateur);
void supprimerConversation(int id_utilisateur, int id_contact);
void repondreMessage(int id_message, int id_expediteur);
void envoyerMessageRapide(int id_expediteur);
void rechercherMessage(char *mot_cle);
int hasNouveauxMessages(int id_utilisateur);

// Sauvegarde
void sauvegarderMessages();
void chargerMessages();

#endif
