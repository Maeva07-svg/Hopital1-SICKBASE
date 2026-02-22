#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "archives.h"
#include "fichiers_relations.h"
#include "utils.h"

Archive archives[MAX_ARCHIVES];
int nombreArchives = 0;
int dernierIDArchive = 0;

void initialiserArchives()
{
    // Rien à initialiser
}

void archiverPatient(int id_patient)
{
    int index = rechercherParID(id_patient);
    if (index == -1)
    {
        printf("Patient non trouve.\n");
        return;
    }

    Archive a;
    memset(&a, 0, sizeof(Archive));

    a.id_archive = ++dernierIDArchive;
    strcpy(a.type_archive, "Patient");
    a.id_element = id_patient;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(a.date_archivage, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    strcpy(a.raison, "Archivage patient");
    strcpy(a.responsable_archivage, "Systeme");
    a.retention_ans = 10;  // 10 ans de conservation

    // Créer un résumé des données
    char resume[5000] = "";
    sprintf(resume, "Patient: %s %s\nID: %d\nDate naissance: %s\nTelephone: %s\nDiagnostic: %s\n",
            patients[index].prenom, patients[index].nom,
            patients[index].id,
            patients[index].date_naissance,
            patients[index].telephone,
            patients[index].diagnostic);

    strcpy(a.donnees_sauvegardees, resume);
    strcpy(a.statut, "Archivé");

    archives[nombreArchives] = a;
    nombreArchives++;

    sauvegarderArchives();

    printf("Patient archive avec succes. ID Archive: %d\n", a.id_archive);
}
