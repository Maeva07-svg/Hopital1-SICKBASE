#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>

/* ================================================================
   ENUMERATION DES ECRANS
   Partagee entre tous les modules ui_*.c
   ================================================================ */
typedef enum {
    ECRAN_LOGIN,
    ECRAN_DASHBOARD,
    ECRAN_PATIENTS,
    ECRAN_PERSONNEL,
    ECRAN_CONSULTATIONS,
    ECRAN_DIAGNOSTIC,
    ECRAN_ORDONNANCES,
    ECRAN_PHARMACIE,
    ECRAN_LABORATOIRE,
    ECRAN_FACTURATION,
    ECRAN_MESSAGERIE,
    ECRAN_ARCHIVES,
    ECRAN_STATISTIQUES,
    ECRAN_QUITTER
} Ecran;

/* ================================================================
   CONTEXTE GLOBAL SDL2
   Passe a tous les modules ui_*
   ================================================================ */
typedef struct {
    SDL_Window   *fenetre;
    SDL_Renderer *rendu;
    TTF_Font     *font_sm;   /* 13px */
    TTF_Font     *font_md;   /* 17px */
    TTF_Font     *font_lg;   /* 24px */
    TTF_Font     *font_xl;   /* 32px */
    TTF_Font     *font_xxl;  /* 52px */
    Ecran         ecran_actuel;
    int           role_connecte;    /* 0-6 */
    int           id_connecte;      /* id du compte connecte */
    char          nom_connecte[64]; /* nom affiche dans dashboard */
} ContexteSDL;

/* ================================================================
   PROTOTYPES
   ================================================================ */

/* Initialise SDL2, TTF, cree fenetre + renderer */
int  initialiserSDL(ContexteSDL *ctx);

/* Libere toutes les ressources SDL2 */
void libererSDL(ContexteSDL *ctx);

/* Lance et gere l'ecran de connexion.
   Retourne 1 si connexion reussie, 0 si quitter. */
int  lancerEcranLogin(ContexteSDL *ctx);

#endif /* UI_LOGIN_H */
