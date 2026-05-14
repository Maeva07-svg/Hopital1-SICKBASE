#include <stdio.h>
#include <string.h>
#include "comptes.h"
#include "utils.h"

// Définition des droits par profil
DroitsAcces obtenirDroitsProfil(ProfilUtilisateur profil)
{
    DroitsAcces droits;
    memset(&droits, 0, sizeof(DroitsAcces));
    droits.profil = profil;

    switch(profil)
    {
        case ROLE_PATIENT:
            // Patient : uniquement ses propres données
            droits.peut_voir_patient = 1;        // Seulement le sien
            droits.peut_voir_ordonnances = 1;     // Ses ordonnances
            droits.peut_voir_consultations = 1;   // Ses consultations
            droits.peut_voir_analyses = 1;        // Ses analyses
            // Pas de droits de modification
            break;

        case ROLE_MEDECIN:
            // Médecin : accès large aux patients
            droits.peut_voir_patient = 1;
            droits.peut_modifier_patient = 1;
            droits.peut_voir_consultations = 1;
            droits.peut_creer_consultations = 1;
            droits.peut_voir_ordonnances = 1;
            droits.peut_prescrire = 1;
            droits.peut_voir_interventions = 1;
            droits.peut_voir_analyses = 1;
            droits.peut_voir_statistiques = 1;
            break;

        case ROLE_INFIRMIER:
            // Infirmier : soins et suivi
            droits.peut_voir_patient = 1;
            droits.peut_modifier_patient = 0;    // Ne modifie pas le dossier
            droits.peut_voir_consultations = 1;
            droits.peut_voir_ordonnances = 1;
            droits.peut_voir_interventions = 1;
            droits.peut_voir_analyses = 1;
            break;

        case ROLE_ADMINISTRATEUR:
            // Administrateur : gestion complète
            droits.peut_voir_patient = 1;
            droits.peut_modifier_patient = 1;
            droits.peut_voir_personnel = 1;
            droits.peut_modifier_personnel = 1;
            droits.peut_voir_consultations = 1;
            droits.peut_creer_consultations = 1;
            droits.peut_voir_ordonnances = 1;
            droits.peut_prescrire = 0;           // Ne prescrit pas
            droits.peut_voir_interventions = 1;
            droits.peut_planifier_interventions = 0;
            droits.peut_voir_analyses = 1;
            droits.peut_gerer_stock = 1;
            droits.peut_archiver = 1;
            droits.peut_gerer_comptes = 1;
            droits.peut_voir_statistiques = 1;
            break;

        case ROLE_TECHNICIEN_LABO:
            // Technicien de laboratoire : analyses uniquement
            droits.peut_voir_patient = 1;         // Pour identifier
            droits.peut_voir_analyses = 1;
            droits.peut_effectuer_analyses = 1;
            break;

        case ROLE_PHARMACIEN:
            // Pharmacien : gestion des médicaments
            droits.peut_voir_ordonnances = 1;
            droits.peut_gerer_stock = 1;
            droits.peut_voir_patient = 1;         // Pour délivrer
            break;

        case ROLE_SUPER_ADMIN:
            // Super administrateur : tous droits
            droits.peut_voir_patient = 1;
            droits.peut_modifier_patient = 1;
            droits.peut_voir_personnel = 1;
            droits.peut_modifier_personnel = 1;
            droits.peut_voir_consultations = 1;
            droits.peut_creer_consultations = 1;
            droits.peut_voir_ordonnances = 1;
            droits.peut_prescrire = 1;
            droits.peut_voir_interventions = 1;
            droits.peut_planifier_interventions = 1;
            droits.peut_voir_analyses = 1;
            droits.peut_effectuer_analyses = 1;
            droits.peut_gerer_stock = 1;
            droits.peut_archiver = 1;
            droits.peut_gerer_comptes = 1;
            droits.peut_voir_statistiques = 1;
            break;
    }

    return droits;
}

// Vérification des droits spécifiques
int verifierDroit(const char *action)
{
    if (utilisateur_actuel == NULL) return 0;

    DroitsAcces droits = obtenirDroitsProfil(utilisateur_actuel->profil);

    if (strcmp(action, "voir_patient") == 0) return droits.peut_voir_patient;
    if (strcmp(action, "modifier_patient") == 0) return droits.peut_modifier_patient;
    if (strcmp(action, "voir_personnel") == 0) return droits.peut_voir_personnel;
    if (strcmp(action, "modifier_personnel") == 0) return droits.peut_modifier_personnel;
    if (strcmp(action, "voir_consultation") == 0) return droits.peut_voir_consultations;
    if (strcmp(action, "creer_consultation") == 0) return droits.peut_creer_consultations;
    if (strcmp(action, "voir_ordonnance") == 0) return droits.peut_voir_ordonnances;
    if (strcmp(action, "prescrire") == 0) return droits.peut_prescrire;
    if (strcmp(action, "voir_intervention") == 0) return droits.peut_voir_interventions;
    if (strcmp(action, "planifier_intervention") == 0) return droits.peut_planifier_interventions;
    if (strcmp(action, "voir_analyse") == 0) return droits.peut_voir_analyses;
    if (strcmp(action, "effectuer_analyse") == 0) return droits.peut_effectuer_analyses;
    if (strcmp(action, "gerer_stock") == 0) return droits.peut_gerer_stock;
    if (strcmp(action, "archiver") == 0) return droits.peut_archiver;
    if (strcmp(action, "gerer_comptes") == 0) return droits.peut_gerer_comptes;
    if (strcmp(action, "voir_statistiques") == 0) return droits.peut_voir_statistiques;

    return 0;
}

// Vérifier si l'utilisateur peut accéder à un patient spécifique
int peutAccederPatient(int id_patient)
{
    if (utilisateur_actuel == NULL) return 0;

    // Super admin et admin ont accès à tous
    if (utilisateur_actuel->profil == ROLE_SUPER_ADMIN ||
        utilisateur_actuel->profil == ROLE_ADMINISTRATEUR)
        return 1;

    // Médecin et infirmier ont accès à tous les patients
    if (utilisateur_actuel->profil == ROLE_MEDECIN ||
        utilisateur_actuel->profil == ROLE_INFIRMIER)
        return 1;

    // Patient : uniquement ses propres données
    if (utilisateur_actuel->profil == ROLE_PATIENT)
        return (utilisateur_actuel->id_associe == id_patient);

    // Autres : accès limité pour identification
    if (utilisateur_actuel->profil == ROLE_TECHNICIEN_LABO ||
        utilisateur_actuel->profil == ROLE_PHARMACIEN)
        return 1;  // Peut voir pour identification

    return 0;
}

// Vérifier si l'utilisateur peut accéder à un personnel spécifique
int peutAccederPersonnel(int id_personnel)
{
    if (utilisateur_actuel == NULL) return 0;

    // Seul admin et super admin voient les données personnel
    if (utilisateur_actuel->profil == ROLE_SUPER_ADMIN ||
        utilisateur_actuel->profil == ROLE_ADMINISTRATEUR)
        return 1;

    // Un employé peut voir ses propres données
    if (utilisateur_actuel->id_associe == id_personnel &&
        (utilisateur_actuel->profil == ROLE_MEDECIN ||
         utilisateur_actuel->profil == ROLE_INFIRMIER ||
         utilisateur_actuel->profil == ROLE_TECHNICIEN_LABO ||
         utilisateur_actuel->profil == ROLE_PHARMACIEN))
        return 1;

    return 0;
}
