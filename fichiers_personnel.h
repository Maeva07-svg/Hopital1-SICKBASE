#ifndef FICHIERS_PERSONNEL_H_INCLUDED
#define FICHIERS_PERSONNEL_H_INCLUDED

#include "personnel.h"

#define FICHIER_PERSONNEL "personnel.dat"
#define FICHIER_ID_PERSONNEL "dernier_id_personnel.dat"

// Prototypes
void sauvegarderPersonnel();
void chargerPersonnel();
void sauvegarderDernierIDPersonnel();
void chargerDernierIDPersonnel();

#endif // FICHIERS_PERSONNEL_H_INCLUDED
