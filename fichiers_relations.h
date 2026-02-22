#ifndef FICHIERS_RELATIONS_H
#define FICHIERS_RELATIONS_H

#include "consultations.h"
#include "ordonnances.h"
#include "interventions.h"
#include "archives.h"

#define FICHIER_CONSULTATIONS "consultations.dat"
#define FICHIER_ID_CONSULTATION "dernier_id_consultation.dat"
#define FICHIER_RENDEZVOUS "rendezvous.dat"
#define FICHIER_ID_RENDEZVOUS "dernier_id_rdv.dat"
#define FICHIER_ORDONNANCES "ordonnances.dat"
#define FICHIER_ID_ORDONNANCE "dernier_id_ordonnance.dat"
#define FICHIER_INTERVENTIONS "interventions.dat"
#define FICHIER_ID_INTERVENTION "dernier_id_intervention.dat"
#define FICHIER_PEC "prises_en_charge.dat"
#define FICHIER_ID_PEC "dernier_id_pec.dat"
#define FICHIER_ARCHIVES "archives.dat"
#define FICHIER_ID_ARCHIVE "dernier_id_archive.dat"

// Prototypes
void sauvegarderConsultations();
void chargerConsultations();
void sauvegarderDernierIDConsultation();
void chargerDernierIDConsultation();

void sauvegarderRendezVous();
void chargerRendezVous();
void sauvegarderDernierIDRendezVous();
void chargerDernierIDRendezVous();

void sauvegarderOrdonnances();
void chargerOrdonnances();
void sauvegarderDernierIDOrdonnance();
void chargerDernierIDOrdonnance();

void sauvegarderInterventions();
void chargerInterventions();
void sauvegarderDernierIDIntervention();
void chargerDernierIDIntervention();

void sauvegarderPrisesEnCharge();
void chargerPrisesEnCharge();
void sauvegarderDernierIDPriseEnCharge();
void chargerDernierIDPriseEnCharge();

void sauvegarderArchives();
void chargerArchives();
void sauvegarderDernierIDArchive();
void chargerDernierIDArchive();

#endif
