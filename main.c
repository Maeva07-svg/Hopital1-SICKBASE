#include <stdio.h>
#include "notification.h"
#include "messagerie.h"

int main() {
    // Notifications
    envoyer_notification(1, "Rappel", "Rendez-vous demain à 10h");
    envoyer_notification(1, "Consultation", "Votre consultation est confirmée");

    afficher_notifications(1);

    // Messagerie
    envoyer_message(100, 1, "Bonjour, votre médecin vous attend");
    envoyer_message(1, 100, "Merci, j'arrive");

    lire_messages(1);

    afficher_notifications(1);

    return 0;
}