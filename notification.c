#include <stdio.h>
#include <string.h>
#include "notification.h"

#define MAX_NOTIFICATIONS 100

static Notification notifications[MAX_NOTIFICATIONS];
static int compteur = 0;

void envoyer_notification(int patient_id, const char* type, const char* message) {
    if (compteur < MAX_NOTIFICATIONS) {
        notifications[compteur].patient_id = patient_id;
        strcpy(notifications[compteur].type, type);
        strcpy(notifications[compteur].message, message);
        notifications[compteur].lu = 0;

        compteur++;
        printf("Notification envoyÃ©e au patient %d\n", patient_id);
    }
}

void afficher_notifications(int patient_id) {
    printf("\nNotifications du patient %d :\n", patient_id);
    
    for (int i = 0; i < compteur; i++) {
        if (notifications[i].patient_id == patient_id) {
            printf("- [%s] %s (%s)\n",
                   notifications[i].type,
                   notifications[i].message,
                   notifications[i].lu ? "Lu" : "Non lu");
        }
    }
}

void marquer_comme_lu(int patient_id) {
    for (int i = 0; i < compteur; i++) {
        if (notifications[i].patient_id == patient_id) {
            notifications[i].lu = 1;
        }
    }
}
