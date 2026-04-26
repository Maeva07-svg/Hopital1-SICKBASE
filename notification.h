#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "utils.h"
#include "comptes.h"

#define MAX_NOTIFICATIONS 5000
#define MAX_MESSAGE 500

// Types de notifications
typedef enum {
    NOTIF_RDV_CONFIRME,
    NOTIF_RDV_ANNULE,
    NOTIF_RESULTAT_ANALYSE,
    NOTIF_ORDONNANCE_DISPONIBLE,
    NOTIF_FACTURE_EMISE,
    NOTIF_RAPPEL_CONSULTATION,
    NOTIF_MESSAGE_NON_LU,
    NOTIF_SOIN_PRESCRIT,
    NOTIF_SOIN_REALISE
} TypeNotification;

// Structure pour une notification
typedef struct
{
    int id_notification;
    int id_destinataire;      // ID du compte destinataire
    int id_expediteur;        // ID du compte expéditeur (0 pour système)
    char type[30];
    char message[MAX_MESSAGE];
    char date_envoi[20];
    int lu;                    // 0 = non lu, 1 = lu
    int priorite;              // 0 = normale, 1 = haute
} Notification;

// Variables globales
extern Notification notifications[MAX_NOTIFICATIONS];
extern int nombreNotifications;
extern int dernierIDNotification;

// Prototypes
void initialiserNotifications();
void envoyerNotification(int id_destinataire, int id_expediteur, const char* type, const char* message, int priorite);
void envoyerNotificationSysteme(int id_destinataire, const char* type, const char* message);
void afficherNotifications(int id_destinataire);
void afficherNotificationsNonLues(int id_destinataire);
void marquerNotificationCommeLue(int id_notification);
void marquerToutesNotificationsCommeLues(int id_destinataire);
void supprimerNotification(int id_notification);
int hasNotificationsNonLues(int id_destinataire);

// Notifications automatiques
void notifierRdvConfirme(int id_patient, int id_medecin, char *date, char *heure);
void notifierRdvAnnule(int id_patient, int id_medecin, char *date);
void notifierResultatAnalyse(int id_patient, char *analyse, char *resultat);
void notifierOrdonnanceDisponible(int id_patient, int id_ordonnance);
void notifierFactureEmise(int id_patient, char *numero_facture, float montant);
void notifierSoinPrescrit(int id_patient, int id_medecin, char *type_soin);
void notifierSoinRealise(int id_patient, int id_infirmier, char *type_soin);
void notifierRappelConsultation(int id_patient, char *date, char *heure);

// Sauvegarde
void sauvegarderNotifications();
void chargerNotifications();

#endif
