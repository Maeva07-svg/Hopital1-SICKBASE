#ifndef FICHIERS_RDV_H
#define FICHIERS_RDV_H

#include "rdv_avance.h"

#define FICHIER_CRENEAUX "creneaux.dat"
#define FICHIER_ID_CRENEAU "dernier_id_creneau.dat"
#define FICHIER_RDV_AVANCE "rdv_avance.dat"
#define FICHIER_ID_RDV_AVANCE "dernier_id_rdv_avance.dat"
#define FICHIER_DISPONIBILITES "disponibilites.dat"

void sauvegarderCreneaux();
void chargerCreneaux();
void sauvegarderDernierIDCreneau();
void chargerDernierIDCreneau();

void sauvegarderRendezVousAvance();
void chargerRendezVousAvance();
void listerRendezVousPatient(int id_patient);
void sauvegarderDernierIDRdvAvance();
void chargerDernierIDRdvAvance();

void sauvegarderDisponibilites();
void chargerDisponibilites();

#endif
