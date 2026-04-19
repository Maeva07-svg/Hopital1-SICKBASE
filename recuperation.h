#ifndef RECUPERATION_H
#define RECUPERATION_H

#include "utils.h"
#include "comptes.h"

#define MAX_QUESTIONS_SECURITE 3
#define MAX_REPONSE 100

// Structure pour les questions de sécurité
typedef struct
{
    int id_question;
    char question[200];
} QuestionSecurite;

// Structure pour les réponses de sécurité d'un utilisateur
typedef struct
{
    int id_compte;
    int id_question;
    char reponse[MAX_REPONSE];
} ReponseSecurite;

// Variables globales
extern QuestionSecurite questionsSecurite[MAX_QUESTIONS_SECURITE];
extern int nombreQuestionsSecurite;
extern ReponseSecurite reponsesSecurite[MAX_COMPTES * MAX_QUESTIONS_SECURITE];
extern int nombreReponsesSecurite;

// Prototypes
void initialiserRecuperation();
void initialiserQuestionsSecurite();
void definirQuestionsSecurite(int id_compte);
int verifierReponsesSecurite(int id_compte);
void reinitialiserMotDePasse(int id_compte);
void menuMotDePasseOublie();

#endif
