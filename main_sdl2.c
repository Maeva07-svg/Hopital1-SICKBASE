/*
 * SICKBASE - Point d'entree principal SDL2
 * Fichier : main.c  (remplace l'ancien main.c console)
 *
 * Compilation complete :
 *   gcc main.c ui_login.c patient.c personnel.c consultations.c \
 *       rdv_avance.c ordonnances.c diagnostic.c interventions.c  \
 *       infirmier.c pharmacie.c laboratoire.c facturation.c      \
 *       messagerie.c notification.c administration.c archives.c  \
 *       comptes.c droits.c utils.c fichiers.c fichiers_personnel.c\
 *       fichiers_rdv.c fichiers_relations.c fichiers_comptes.c   \
 *       recuperation.c menu_profil.c                             \
 *       -o SICKBASE.exe                                          \
 *       -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_gfx      \
 *       -I"C:/msys64/mingw64/include"                           \
 *       -L"C:/msys64/mingw64/lib" -mwindows
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "ui_login.h"

/* Modules metier existants — inchanges */
#include "patient.h"
#include "personnel.h"
#include "consultations.h"
#include "ordonnances.h"
#include "diagnostic.h"
#include "pharmacie.h"
#include "laboratoire.h"
#include "facturation.h"
#include "messagerie.h"
#include "notification.h"
#include "administration.h"
#include "archives.h"
#include "comptes.h"
#include "utils.h"
#include "fichiers.h"
#include "fichiers_personnel.h"
#include "fichiers_rdv.h"

/* ================================================================
   CHARGEMENT DES POLICES
   ================================================================ */
static TTF_Font *chargerPolice(int taille)
{
    const char *chemins[] = {
        "C:/Windows/Fonts/segoeui.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/tahoma.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "DejaVuSans.ttf",
        NULL
    };
    for (int i = 0; chemins[i]; i++) {
        TTF_Font *f = TTF_OpenFont(chemins[i], taille);
        if (f) return f;
    }
    fprintf(stderr, "AVERTISSEMENT: Police taille %d non trouvee.\n", taille);
    return NULL;
}

/* ================================================================
   INITIALISATION SDL2
   ================================================================ */
int initialiserSDL(ContexteSDL *ctx)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        fprintf(stderr, "SDL_Init erreur: %s\n", SDL_GetError());
        return 0;
    }

    if (TTF_Init() < 0) {
        fprintf(stderr, "TTF_Init erreur: %s\n", TTF_GetError());
        SDL_Quit();
        return 0;
    }

    /* Antialiasing */
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    ctx->fenetre = SDL_CreateWindow(
        "SICKBASE  -  Systeme de Gestion Hospitaliere",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        900, 600,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!ctx->fenetre) {
        fprintf(stderr, "SDL_CreateWindow erreur: %s\n", SDL_GetError());
        TTF_Quit(); SDL_Quit();
        return 0;
    }

    ctx->rendu = SDL_CreateRenderer(
        ctx->fenetre, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!ctx->rendu) {
        fprintf(stderr, "SDL_CreateRenderer erreur: %s\n", SDL_GetError());
        SDL_DestroyWindow(ctx->fenetre);
        TTF_Quit(); SDL_Quit();
        return 0;
    }

    SDL_SetRenderDrawBlendMode(ctx->rendu, SDL_BLENDMODE_BLEND);

    /* Chargement des polices */
    ctx->font_sm  = chargerPolice(13);
    ctx->font_md  = chargerPolice(17);
    ctx->font_lg  = chargerPolice(24);
    ctx->font_xl  = chargerPolice(32);
    ctx->font_xxl = chargerPolice(52);

    ctx->ecran_actuel  = ECRAN_LOGIN;
    ctx->role_connecte = -1;
    ctx->id_connecte   = -1;
    memset(ctx->nom_connecte, 0, sizeof(ctx->nom_connecte));

    return 1;
}

/* ================================================================
   LIBERATION SDL2
   ================================================================ */
void libererSDL(ContexteSDL *ctx)
{
    if (ctx->font_sm)  TTF_CloseFont(ctx->font_sm);
    if (ctx->font_md)  TTF_CloseFont(ctx->font_md);
    if (ctx->font_lg)  TTF_CloseFont(ctx->font_lg);
    if (ctx->font_xl)  TTF_CloseFont(ctx->font_xl);
    if (ctx->font_xxl) TTF_CloseFont(ctx->font_xxl);

    if (ctx->rendu)   SDL_DestroyRenderer(ctx->rendu);
    if (ctx->fenetre) SDL_DestroyWindow(ctx->fenetre);

    TTF_Quit();
    SDL_Quit();
}

/* ================================================================
   CHARGEMENT DES DONNEES METIER
   (reprend ce que faisait l'ancien main.c console)
   ================================================================ */
static void chargerDonnees(void)
{
    /* Patients */
    chargerPatients();

    /* Personnel */
    chargerPersonnel();

    /* Rendez-vous */
    chargerRendezVous();

    /* Diagnostics */
    chargerDiagnostics();

    /* Comptes utilisateurs */
    chargerComptes();

    /* Maladies (base de regles) */
    initialiserMaladies();
    initialiserQuestionsDiagnostic();

    /* Zones corporelles SDL2 */
    initialiserZonesCorporelles();
}

/* ================================================================
   BOUCLE PRINCIPALE DE NAVIGATION ENTRE ECRANS
   ================================================================ */
static void boucleNavigation(ContexteSDL *ctx)
{
    while (ctx->ecran_actuel != ECRAN_QUITTER)
    {
        switch (ctx->ecran_actuel)
        {
        /* ── Connexion ── */
        case ECRAN_LOGIN:
            if (!lancerEcranLogin(ctx))
                ctx->ecran_actuel = ECRAN_QUITTER;
            break;

        /* ── Dashboard ── */
        case ECRAN_DASHBOARD:
            /*
             * TODO : appeler lancerDashboard(ctx)
             * quand ui_dashboard.c sera code.
             * Pour l'instant placeholder :
             */
            {
                SDL_Event e;
                int attente = 1;
                while (attente) {
                    while (SDL_PollEvent(&e)) {
                        if (e.type == SDL_QUIT)
                            ctx->ecran_actuel = ECRAN_QUITTER, attente = 0;
                        if (e.type == SDL_KEYDOWN &&
                            e.key.keysym.sym == SDLK_ESCAPE)
                            ctx->ecran_actuel = ECRAN_LOGIN, attente = 0;
                    }
                    SDL_SetRenderDrawColor(ctx->rendu, 235, 242, 250, 255);
                    SDL_RenderClear(ctx->rendu);

                    /* Message temporaire */
                    SDL_Color bleu = {25, 95, 180, 255};
                    SDL_Color gris = {120, 140, 160, 255};
                    /* drawText simplifie — a remplacer par ui_dashboard */
                    SDL_RenderPresent(ctx->rendu);
                    SDL_Delay(16);
                }
            }
            break;

        /* ── Modules a venir ── */
        case ECRAN_PATIENTS:
            /* TODO : lancerEcranPatients(ctx); */
            ctx->ecran_actuel = ECRAN_DASHBOARD;
            break;

        case ECRAN_PERSONNEL:
            /* TODO : lancerEcranPersonnel(ctx); */
            ctx->ecran_actuel = ECRAN_DASHBOARD;
            break;

        case ECRAN_CONSULTATIONS:
            /* TODO : lancerEcranConsultations(ctx); */
            ctx->ecran_actuel = ECRAN_DASHBOARD;
            break;

        case ECRAN_DIAGNOSTIC:
            /* Module diagnostic corporel SDL2 deja code */
            lancerDiagnosticCorporel(ctx->id_connecte, ctx->role_connecte);
            ctx->ecran_actuel = ECRAN_DASHBOARD;
            break;

        case ECRAN_ORDONNANCES:
            /* TODO : lancerEcranOrdonnances(ctx); */
            ctx->ecran_actuel = ECRAN_DASHBOARD;
            break;

        case ECRAN_PHARMACIE:
            /* TODO : lancerEcranPharmacie(ctx); */
            ctx->ecran_actuel = ECRAN_DASHBOARD;
            break;

        case ECRAN_LABORATOIRE:
            /* TODO : lancerEcranLaboratoire(ctx); */
            ctx->ecran_actuel = ECRAN_DASHBOARD;
            break;

        case ECRAN_FACTURATION:
            /* TODO : lancerEcranFacturation(ctx); */
            ctx->ecran_actuel = ECRAN_DASHBOARD;
            break;

        case ECRAN_MESSAGERIE:
            /* TODO : lancerEcranMessagerie(ctx); */
            ctx->ecran_actuel = ECRAN_DASHBOARD;
            break;

        case ECRAN_ARCHIVES:
            /* TODO : lancerEcranArchives(ctx); */
            ctx->ecran_actuel = ECRAN_DASHBOARD;
            break;

        case ECRAN_STATISTIQUES:
            /* TODO : lancerEcranStatistiques(ctx); */
            ctx->ecran_actuel = ECRAN_DASHBOARD;
            break;

        default:
            ctx->ecran_actuel = ECRAN_QUITTER;
            break;
        }
    }
}

/* ================================================================
   POINT D'ENTREE
   ================================================================ */
int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    ContexteSDL ctx = {0};

    /* 1. Initialiser SDL2 */
    if (!initialiserSDL(&ctx)) {
        fprintf(stderr, "Echec initialisation SDL2.\n");
        return EXIT_FAILURE;
    }

    /* 2. Charger les donnees metier depuis les .dat */
    chargerDonnees();

    /* 3. Lancer la navigation entre ecrans */
    SDL_StartTextInput();
    boucleNavigation(&ctx);
    SDL_StopTextInput();

    /* 4. Liberer les ressources */
    libererSDL(&ctx);

    return EXIT_SUCCESS;
}
