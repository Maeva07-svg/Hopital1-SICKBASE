#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#define MAX_MESSAGE 256

typedef struct {
    int patient_id;
    char type[50];
    char message[MAX_MESSAGE];
    int lu; // 0 = non lu, 1 = lu
} Notification;

// Fonctions
void envoyer_notification(int patient_id, const char* type, const char* message);
void afficher_notifications(int patient_id);
void marquer_comme_lu(int patient_id);

#endif
