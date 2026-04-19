#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "recuperation.h"
#include "utils.h"
#include "comptes.h"

// Définition des variables globales
QuestionSecurite questionsSecurite[MAX_QUESTIONS_SECURITE];
int nombreQuestionsSecurite = 0;
ReponseSecurite reponsesSecurite[MAX_COMPTES * MAX_QUESTIONS_SECURITE];
int nombreReponsesSecurite = 0;

void initialiserQuestionsSecurite()
{
    // Question 1
    strcpy(questionsSecurite[0].question, "Quel est votre artiste prefere ?");
    questionsSecurite[0].id_question = 1;

    // Question 2
    strcpy(questionsSecurite[1].question, "Quelle est votre surnom ?");
    questionsSecurite[1].id_question = 2;

    // Question 3
    strcpy(questionsSecurite[2].question, "Quel est votre trait de personnalite ?");
    questionsSecurite[2].id_question = 3;

    nombreQuestionsSecurite = 3;
}

void initialiserRecuperation()
{
    initialiserQuestionsSecurite();
    chargerReponsesSecurite();
}

void definirQuestionsSecurite(int id_compte)
{
    system("cls");
    printf("\n=== DEFINITION DES QUESTIONS DE SECURITE ===\n\n");

    // Vérifier si l'utilisateur a déjà des questions
    for (int i = 0; i < nombreReponsesSecurite; i++)
    {
        if (reponsesSecurite[i].id_compte == id_compte)
        {
            printf("Vous avez deja des questions de securite enregistrees.\n");
            printf("Voulez-vous les modifier? (O/N): ");
            char rep;
            scanf("%c", &rep);
            viderBuffer();
            if (toupper(rep) != 'O')
                return;

            // Supprimer les anciennes réponses
            for (int j = 0; j < nombreReponsesSecurite; j++)
            {
                if (reponsesSecurite[j].id_compte == id_compte)
                {
                    for (int k = j; k < nombreReponsesSecurite - 1; k++)
                    {
                        reponsesSecurite[k] = reponsesSecurite[k + 1];
                    }
                    nombreReponsesSecurite--;
                    j--;
                }
            }
            break;
        }
    }

    printf("Choisissez 3 questions de securite et donnez vos reponses.\n");
    printf("Ces reponses vous permettront de reinitialiser votre mot de passe en cas d'oubli.\n\n");

    for (int i = 0; i < nombreQuestionsSecurite; i++)
    {
        printf("\n--- QUESTION %d ---\n", i + 1);
        printf("%s\n", questionsSecurite[i].question);
        printf("Votre reponse: ");

        char reponse[MAX_REPONSE];
        fgets(reponse, MAX_REPONSE, stdin);
        reponse[strcspn(reponse, "\n")] = '\0';

        // Convertir en minuscules pour uniformiser
        for (int j = 0; reponse[j]; j++)
            reponse[j] = tolower(reponse[j]);

        ReponseSecurite rs;
        rs.id_compte = id_compte;
        rs.id_question = questionsSecurite[i].id_question;
        strcpy(rs.reponse, reponse);

        reponsesSecurite[nombreReponsesSecurite] = rs;
        nombreReponsesSecurite++;
    }

    sauvegarderReponsesSecurite();

    printf("\n=== QUESTIONS DE SECURITE ENREGISTREES AVEC SUCCES ===\n");
    pause();
}

int verifierReponsesSecurite(int id_compte)
{
    system("cls");
    printf("\n=== VERIFICATION D'IDENTITE ===\n\n");
    printf("Repondez aux questions suivantes pour reinitialiser votre mot de passe.\n\n");

    int reponses_correctes = 0;
    int questions_posees = 0;

    for (int i = 0; i < nombreReponsesSecurite && questions_posees < 3; i++)
    {
        if (reponsesSecurite[i].id_compte == id_compte)
        {
            questions_posees++;

            // Trouver la question
            char question[200] = "";
            for (int j = 0; j < nombreQuestionsSecurite; j++)
            {
                if (questionsSecurite[j].id_question == reponsesSecurite[i].id_question)
                {
                    strcpy(question, questionsSecurite[j].question);
                    break;
                }
            }

            printf("%d. %s\n", questions_posees, question);
            printf("Reponse: ");
            char reponse[MAX_REPONSE];
            fgets(reponse, MAX_REPONSE, stdin);
            reponse[strcspn(reponse, "\n")] = '\0';

            // Convertir en minuscules
            for (int j = 0; reponse[j]; j++)
                reponse[j] = tolower(reponse[j]);

            if (strcmp(reponse, reponsesSecurite[i].reponse) == 0)
            {
                reponses_correctes++;
                printf("  Reponse correcte\n\n");
            }
            else
            {
                printf("  Reponse incorrecte\n\n");
            }
        }
    }

    return (reponses_correctes >= 2); // Au moins 2 réponses correctes sur 3
}

void reinitialiserMotDePasse(int id_compte)
{
    for (int i = 0; i < nombreComptes; i++)
    {
        if (comptes[i].id_compte == id_compte)
        {
            printf("\n=== REINITIALISATION DU MOT DE PASSE ===\n");
            printf("Compte: %s\n", comptes[i].login);
            printf("Profil: ");
            switch(comptes[i].profil)
            {
                case ROLE_PATIENT: printf("Patient\n"); break;
                case ROLE_MEDECIN: printf("Medecin\n"); break;
                case ROLE_INFIRMIER: printf("Infirmier\n"); break;
                case ROLE_ADMINISTRATEUR: printf("Administrateur\n"); break;
                case ROLE_TECHNICIEN_LABO: printf("Technicien de laboratoire\n"); break;
                case ROLE_PHARMACIEN: printf("Pharmacien\n"); break;
                case ROLE_SUPER_ADMIN: printf("Super Administrateur\n"); break;
            }

            printf("\nNouveau mot de passe: ");
            char nouveau_mdp[MAX_MDP];
            fgets(nouveau_mdp, MAX_MDP, stdin);
            nouveau_mdp[strcspn(nouveau_mdp, "\n")] = '\0';

            printf("Confirmer le mot de passe: ");
            char confirmation[MAX_MDP];
            fgets(confirmation, MAX_MDP, stdin);
            confirmation[strcspn(confirmation, "\n")] = '\0';

            if (strcmp(nouveau_mdp, confirmation) == 0)
            {
                strcpy(comptes[i].mot_de_passe, nouveau_mdp);
                comptes[i].bloque = 0;
                comptes[i].tentative_echec = 0;
                sauvegarderComptes();
                printf("\n Mot de passe reinitialise avec succes!\n");
                printf("Vous pouvez maintenant vous connecter avec votre nouveau mot de passe.\n");
            }
            else
            {
                printf("\n Les mots de passe ne correspondent pas.\n");
            }
            return;
        }
    }
}

void menuMotDePasseOublie()
{
    system("cls");
    color(11, 0);
    printf("\n========================================\n");
    printf("         MOT DE PASSE OUBLIE           \n");
    printf("========================================\n\n");
    color(7, 0);

    printf("Entrez votre login: ");
    char login[MAX_LOGIN];
    fgets(login, MAX_LOGIN, stdin);
    login[strcspn(login, "\n")] = '\0';

    // Chercher le compte
    int id_compte = -1;
    for (int i = 0; i < nombreComptes; i++)
    {
        if (strcmp(comptes[i].login, login) == 0)
        {
            id_compte = comptes[i].id_compte;
            break;
        }
    }

    if (id_compte == -1)
    {
        printf("\n Aucun compte trouve avec ce login.\n");
        pause();
        return;
    }

    // Vérifier si l'utilisateur a des questions de sécurité
    int a_questions = 0;
    for (int i = 0; i < nombreReponsesSecurite; i++)
    {
        if (reponsesSecurite[i].id_compte == id_compte)
        {
            a_questions = 1;
            break;
        }
    }

    if (!a_questions)
    {
        printf("\n Aucune question de securite n'a ete configuree pour ce compte.\n");
        printf("Veuillez contacter l'administrateur.\n");
        pause();
        return;
    }

    // Vérifier les réponses
    if (verifierReponsesSecurite(id_compte))
    {
        reinitialiserMotDePasse(id_compte);
    }
    else
    {
        printf("\n Reponses incorrectes. Verification echouee.\n");
        printf("Veuillez reessayer ou contacter l'administrateur.\n");
    }

    pause();
}

// Sauvegarde des réponses de sécurité
void sauvegarderReponsesSecurite()
{
    FILE *f = fopen("reponses_securite.dat", "wb");
    if (f == NULL) return;

    fwrite(&nombreReponsesSecurite, sizeof(int), 1, f);
    fwrite(reponsesSecurite, sizeof(ReponseSecurite), nombreReponsesSecurite, f);
    fclose(f);
}

void chargerReponsesSecurite()
{
    FILE *f = fopen("reponses_securite.dat", "rb");
    if (f == NULL) return;

    fread(&nombreReponsesSecurite, sizeof(int), 1, f);
    fread(reponsesSecurite, sizeof(ReponseSecurite), nombreReponsesSecurite, f);
    fclose(f);
}
