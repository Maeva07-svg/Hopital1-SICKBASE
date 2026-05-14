typedef struct {
    int id;
    char message[200];
    char date[20];
    int lu; // 0 = non lu, 1 = lu
} Notification;
void ajouterNotification(Notification notifications[], int *n, char message[], char date[]) {
    notifications[*n].id = *n + 1;
    strcpy(notifications[*n].message, message);
    strcpy(notifications[*n].date, date);
    notifications[*n].lu = 0;
    (*n)++;
}
//Fonction Afficher les notifications
void afficherNotifications(Notification notifications[], int n) {
    for(int i = 0; i < n; i++) {
        printf("ID: %d\n", notifications[i].id);
        printf("Message: %s\n", notifications[i].message);
        printf("Date: %s\n", notifications[i].date);
        printf("Statut: %s\n\n", notifications[i].lu ? "Lu" : "Non lu");
    }
}
Fonction : Marquer comme lu
void marquerCommeLu(Notification notifications[], int n, int id) {
    for(int i = 0; i < n; i++) {
        if(notifications[i].id == id) {
            notifications[i].lu = 1;
        }
    }
}
Messagerie Médecin

Structure message :
typedef struct {
    int id;
    char expediteur[50];
    char destinataire[50];
    char contenu[200];
    char date[20];
} Message;
//Envoyer un message
void envoyerMessage(Message messages[], int *n, char exp[], char dest[], char contenu[], char date[]) {
    messages[*n].id = *n + 1;
    strcpy(messages[*n].expediteur, exp);
    strcpy(messages[*n].destinataire, dest);
    strcpy(messages[*n].contenu, contenu);
    strcpy(messages[*n].date, date);
    (*n)++;
}
//Afficher les messages du médecin
void afficherMessages(Message messages[], int n, char nomMedecin[]) {
    for(int i = 0; i < n; i++) {
        if(strcmp(messages[i].destinataire, nomMedecin) == 0) {
            printf("De: %s\n", messages[i].expediteur);
            printf("Message: %s\n", messages[i].contenu);
            printf("Date: %s\n\n", messages[i].date);
        }
    }
}
