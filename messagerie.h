#ifndef MESSAGERIE_H
#define MESSAGERIE_H

#define MAX_MSG 256

typedef struct {
    int expediteur_id;
    int recepteur_id;
    char contenu[MAX_MSG];
    int lu;
} Message;

// Fonctions
void envoyer_message(int expediteur_id, int recepteur_id, const char* contenu);
void lire_messages(int utilisateur_id);

#endif