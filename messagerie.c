#include <stdio.h>
#include <string.h>
#include "messagerie.h"
#include "notification.h"

#define MAX_MESSAGES 100

static Message messages[MAX_MESSAGES];
static int compteur_msg = 0;

void envoyer_message(int expediteur_id, int recepteur_id, const char* contenu) {
    if (compteur_msg < MAX_MESSAGES) {
        messages[compteur_msg].expediteur_id = expediteur_id;
        messages[compteur_msg].recepteur_id = recepteur_id;
        strcpy(messages[compteur_msg].contenu, contenu);
        messages[compteur_msg].lu = 0;

        compteur_msg++;

        // Notifier le recepteur
        envoyer_notification(recepteur_id, "message", "Nouveau message reÃ§u");

        printf("Message envoyÃ© de %d Ã  %d\n", expediteur_id, recepteur_id);
    }
}

void lire_messages(int utilisateur_id) {
    printf("\nMessages pour utilisateur %d :\n", utilisateur_id);

    for (int i = 0; i < compteur_msg; i++) {
        if (messages[i].recepteur_id == utilisateur_id) {
            printf("De %d : %s (%s)\n",
                   messages[i].expediteur_id,
                   messages[i].contenu,
                   messages[i].lu ? "Lu" : "Non lu");

            messages[i].lu = 1;
        }
    }
}
