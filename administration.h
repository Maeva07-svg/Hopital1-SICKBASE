#ifndef ADMINISTRATION_H
#define ADMINISTRATION_H

#include "utils.h"
#include "patient.h"
#include "personnel.h"
#include "consultations.h"
#include "ordonnances.h"
#include "interventions.h"
#include "facturation.h"
#include "laboratoire.h"
#include "pharmacie.h"
#include "infirmier.h"
#include "comptes.h"

// ================= STATISTIQUES AVANCÉES =================
void statistiquesAvancees();
void statistiquesActiviteJournaliere();
void statistiquesParPeriode(char *date_debut, char *date_fin);
void statistiquesFinancieres();
void statistiquesMedicales();
void statistiquesPersonnelAvance();
void statistiquesPatientParAge();
void exportStatistiquesCSV();

// ================= CONFIGURATION SYSTÈME =================
void configurationSysteme();
void configurerSeuilsAlerte();
void configurerDureeConservationArchives();
void configurerTarifs();
void gererParametresApplication();
void sauvegarderConfiguration();
void chargerConfiguration();
void reinitialiserConfiguration();
void afficherLogsSysteme();
void nettoyerLogsSysteme();

// ================= SAUVEGARDE GLOBALE =================
void sauvegardeGlobale();
void restaurerSauvegarde(char *fichier);
void planifierSauvegarde();

// ================= MAINTENANCE =================
void maintenanceBaseDonnees();
void reparerFichiers();
void verifierIntegriteDonnees();

#endif
