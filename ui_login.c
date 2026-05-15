/*
 * SICKBASE - Ecran de Connexion SDL2
 * Fichier : ui_login.c
 *
 * Compilation :
 *   gcc ui_login.c -o login_test.exe \
 *       -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_gfx \
 *       -I"C:/msys64/mingw64/include" \
 *       -L"C:/msys64/mingw64/lib" -mwindows
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================================================================
   CONSTANTES
   ================================================================ */

#define WIN_W        900
#define WIN_H        600
#define MAX_INPUT    64

/* Couleurs */
#define C_BG         235, 242, 250   /* fond gris bleuté      */
#define C_PANEL      255, 255, 255   /* panneau blanc         */
#define C_BLEU       25,  95,  180   /* bleu principal        */
#define C_VERT       0,   155, 100   /* vert médical          */
#define C_VERT_LT    0,   200, 130   /* vert clair            */
#define C_ROUGE      210, 45,  45    /* rouge erreur          */
#define C_GRIS       120, 140, 160   /* gris texte secondaire */
#define C_GRIS_LT    220, 230, 240   /* gris bord champ       */
#define C_TEXTE      25,  35,  55    /* texte principal       */
#define C_BLANC      255, 255, 255
#define C_OMBRE      0,   0,   0     /* ombre */

/* ================================================================
   STRUCTURES
   ================================================================ */

typedef enum {
    ECRAN_LOGIN,
    ECRAN_DASHBOARD,
    ECRAN_QUITTER
} Ecran;

typedef struct {
    char  texte[MAX_INPUT];
    int   longueur;
    int   actif;        /* 1 = champ sélectionné */
    SDL_Rect rect;
    int   mot_de_passe; /* 1 = masquer avec *** */
} ChampTexte;

typedef struct {
    SDL_Rect  rect;
    char      label[32];
    int       survol;
    int       actif;
    SDL_Color couleur;
    SDL_Color couleur_hover;
} Bouton;

typedef struct {
    SDL_Window   *fenetre;
    SDL_Renderer *rendu;
    TTF_Font     *font_sm;    /* 13px */
    TTF_Font     *font_md;    /* 17px */
    TTF_Font     *font_lg;    /* 24px */
    TTF_Font     *font_xl;    /* 36px */
    TTF_Font     *font_xxl;   /* 52px */
    Ecran         ecran;
    ChampTexte    champ_id;
    ChampTexte    champ_mdp;
    Bouton        btn_connexion;
    Bouton        btn_mdp_oublie;
    char          message_erreur[128];
    int           role_selectionne; /* 0-6 */
    int           btn_role_survol;
    Uint32        temps_erreur;
    float         anim_logo;    /* animation croix */
    int           mdp_visible;  /* toggle affichage mdp */
} App;

/* Noms des rôles */
static const char *ROLES[] = {
    "Super Admin", "Administrateur", "Medecin",
    "Infirmier", "Technicien Labo", "Pharmacien", "Patient"
};
static const SDL_Color COULEURS_ROLE[] = {
    {180, 40,  40,  255},  /* rouge   Super Admin  */
    {25,  95,  180, 255},  /* bleu    Admin        */
    {0,   155, 100, 255},  /* vert    Medecin      */
    {0,   160, 175, 255},  /* cyan    Infirmier    */
    {180, 130, 0,   255},  /* ambre   Labo         */
    {120, 60,  180, 255},  /* violet  Pharmacien   */
    {60,  130, 200, 255},  /* bleu lt Patient      */
};

/* ================================================================
   UTILITAIRES DE DESSIN
   ================================================================ */

static TTF_Font *chargerPolice(int taille) {
    const char *chemins[] = {
        "C:/Windows/Fonts/segoeui.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/tahoma.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "DejaVuSans.ttf", NULL
    };
    for (int i = 0; chemins[i]; i++) {
        TTF_Font *f = TTF_OpenFont(chemins[i], taille);
        if (f) return f;
    }
    return NULL;
}

static void fillRect(SDL_Renderer *r, SDL_Rect rect, SDL_Color c) {
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(r, &rect);
}

static void drawBorder(SDL_Renderer *r, SDL_Rect rect,
                       SDL_Color c, int epaisseur) {
    SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
    for (int i = 0; i < epaisseur; i++) {
        SDL_Rect rr = {rect.x+i, rect.y+i,
                       rect.w-2*i, rect.h-2*i};
        SDL_RenderDrawRect(r, &rr);
    }
}

static void drawText(SDL_Renderer *r, TTF_Font *f,
                     const char *txt, int x, int y, SDL_Color c) {
    if (!f || !txt || !strlen(txt)) return;
    SDL_Surface *s = TTF_RenderUTF8_Blended(f, txt, c);
    if (!s) return;
    SDL_Texture *t = SDL_CreateTextureFromSurface(r, s);
    SDL_Rect dst = {x, y, s->w, s->h};
    SDL_RenderCopy(r, t, NULL, &dst);
    SDL_DestroyTexture(t);
    SDL_FreeSurface(s);
}

static void drawTextCenter(SDL_Renderer *r, TTF_Font *f,
                           const char *txt, SDL_Rect zone, SDL_Color c) {
    if (!f || !txt || !strlen(txt)) return;
    int tw, th;
    TTF_SizeUTF8(f, txt, &tw, &th);
    int x = zone.x + (zone.w - tw) / 2;
    int y = zone.y + (zone.h - th) / 2;
    drawText(r, f, txt, x, y, c);
}

/* Ombre portée douce */
static void drawOmbre(SDL_Renderer *r, SDL_Rect rect, int rayon) {
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    for (int i = rayon; i > 0; i--) {
        Uint8 alpha = (Uint8)(40 * (rayon - i + 1) / rayon);
        SDL_SetRenderDrawColor(r, 0, 20, 60, alpha);
        SDL_Rect rr = {rect.x - i, rect.y + i,
                       rect.w + i*2, rect.h};
        SDL_RenderDrawRect(r, &rr);
    }
}

/* Rectangle arrondi plein via SDL2_gfx */
static void fillRoundRect(SDL_Renderer *r, int x, int y,
                          int w, int h, int rad, SDL_Color c) {
    roundedBoxRGBA(r, x, y, x+w, y+h, rad, c.r, c.g, c.b, c.a);
}
static void drawRoundRect(SDL_Renderer *r, int x, int y,
                          int w, int h, int rad, SDL_Color c) {
    roundedRectangleRGBA(r, x, y, x+w, y+h, rad, c.r, c.g, c.b, c.a);
}

/* ================================================================
   DESSIN FOND (panneau gauche décoratif + fond droit)
   ================================================================ */
static void dessinerFond(App *app) {
    SDL_Renderer *r = app->rendu;

    /* Fond général */
    SDL_SetRenderDrawColor(r, C_BG, 255);
    SDL_RenderClear(r);

    /* ── Panneau gauche décoratif ── */
    fillRoundRect(r, 0, 0, 340, WIN_H, 0,
                  (SDL_Color){20, 75, 150, 255});

    /* Dégradé simulé par bandes */
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    for (int i = 0; i < 340; i++) {
        Uint8 alpha = (Uint8)(60 * i / 340);
        SDL_SetRenderDrawColor(r, 0, 180, 120, alpha);
        SDL_RenderDrawLine(r, i, 0, i, WIN_H);
    }

    /* Cercles décoratifs */
    circleRGBA(r, 80,  520, 200, 255, 255, 255, 15);
    circleRGBA(r, 80,  520, 160, 255, 255, 255, 12);
    circleRGBA(r, 300, -50, 180, 255, 255, 255, 12);
    circleRGBA(r, 300, -50, 130, 255, 255, 255, 10);

    /* ── Croix médicale animée ── */
    int cx = 170, cy = 180;
    int cr = (int)(55 + 5 * SDL_sinf(app->anim_logo));

    /* Cercle fond croix */
    filledCircleRGBA(r, cx, cy, cr + 8,
                     255, 255, 255, 25);
    filledCircleRGBA(r, cx, cy, cr,
                     255, 255, 255, 40);

    /* Croix */
    int bv = 20, bh = 50;
    boxRGBA(r, cx-bv/2, cy-bh, cx+bv/2, cy+bh,
            255, 255, 255, 220);
    boxRGBA(r, cx-bh, cy-bv/2, cx+bh, cy+bv/2,
            255, 255, 255, 220);
    /* Contour croix */
    rectangleRGBA(r, cx-bv/2, cy-bh, cx+bv/2, cy+bh,
                  200, 240, 220, 180);
    rectangleRGBA(r, cx-bh, cy-bv/2, cx+bh, cy+bv/2,
                  200, 240, 220, 180);

    /* ── Texte panneau gauche ── */
    SDL_Color blanc = {255, 255, 255, 255};
    SDL_Color blanc_fade = {200, 230, 255, 255};

    drawText(r, app->font_xxl, "SICK", 38, 290, blanc);
    drawText(r, app->font_xxl, "BASE", 38, 345, (SDL_Color){0, 220, 155, 255});

    drawText(r, app->font_md,
             "Systeme de Gestion", 38, 415, blanc_fade);
    drawText(r, app->font_md,
             "Hospitaliere", 38, 440, blanc_fade);

    /* Petites puces */
    const char *features[] = {
        "15+ modules integres",
        "7 roles utilisateurs",
        "Diagnostic interactif",
        NULL
    };
    int fy = 490;
    for (int i = 0; features[i]; i++) {
        filledCircleRGBA(r, 46, fy + 8, 4, 0, 220, 155, 255);
        drawText(r, app->font_sm, features[i],
                 58, fy, (SDL_Color){180, 220, 255, 255});
        fy += 24;
    }

    /* Version */
    drawText(r, app->font_sm, "v2.0  -  2025/2026",
             38, WIN_H - 30, (SDL_Color){140, 180, 220, 255});
}

/* ================================================================
   DESSIN PANNEAU LOGIN (droite)
   ================================================================ */
static void dessinerPanneauLogin(App *app) {
    SDL_Renderer *r = app->rendu;

    /* Panneau blanc avec ombre */
    SDL_Rect panel = {370, 60, 490, 480};
    drawOmbre(r, panel, 12);
    fillRoundRect(r, panel.x, panel.y, panel.w, panel.h, 16,
                  (SDL_Color){255, 255, 255, 255});

    /* Barre top colorée */
    fillRoundRect(r, panel.x, panel.y, panel.w, 8, 4,
                  (SDL_Color){0, 155, 100, 255});

    SDL_Color bleu   = {C_BLEU,  255};
    SDL_Color vert   = {C_VERT,  255};
    SDL_Color texte  = {C_TEXTE, 255};
    SDL_Color gris   = {C_GRIS,  255};

    /* Titre */
    drawText(r, app->font_xl, "Connexion",
             panel.x + 30, panel.y + 28, bleu);
    drawText(r, app->font_sm,
             "Bienvenue sur SICKBASE. Identifiez-vous.",
             panel.x + 30, panel.y + 72, gris);

    /* ── Sélecteur de rôle ── */
    drawText(r, app->font_sm, "ROLE",
             panel.x + 30, panel.y + 105, gris);

    int rx = panel.x + 30;
    int ry = panel.y + 122;
    int rbw = 68, rbh = 28;

    for (int i = 0; i < 7; i++) {
        int col = i % 4, row = i / 4;
        int bx = rx + col * (rbw + 6);
        int by = ry + row * (rbh + 6);

        SDL_Color cc = COULEURS_ROLE[i];

        if (app->role_selectionne == i) {
            /* Actif */
            fillRoundRect(r, bx, by, rbw, rbh, 6, cc);
            drawText(r, app->font_sm, ROLES[i],
                     bx + 4, by + 7,
                     (SDL_Color){255, 255, 255, 255});
        } else {
            /* Inactif */
            fillRoundRect(r, bx, by, rbw, rbh, 6,
                          (SDL_Color){235, 242, 250, 255});
            drawRoundRect(r, bx, by, rbw, rbh, 6,
                          (SDL_Color){cc.r, cc.g, cc.b, 120});
            drawText(r, app->font_sm, ROLES[i],
                     bx + 4, by + 7,
                     (SDL_Color){cc.r, cc.g, cc.b, 255});
        }
    }

    /* ── Champ Identifiant ── */
    int cy_id = panel.y + 208;
    drawText(r, app->font_sm, "IDENTIFIANT",
             panel.x + 30, cy_id, gris);

    SDL_Color bord_id = app->champ_id.actif
        ? (SDL_Color){C_VERT, 255}
        : (SDL_Color){C_GRIS_LT, 255};

    fillRoundRect(r, panel.x + 30, cy_id + 20,
                  430, 44, 8, (SDL_Color){245, 248, 252, 255});
    drawRoundRect(r, panel.x + 30, cy_id + 20,
                  430, 44, 8, bord_id);

    /* Icône utilisateur */
    drawText(r, app->font_md, "@",
             panel.x + 45, cy_id + 31, gris);

    /* Texte saisi ou placeholder */
    if (app->champ_id.longueur > 0) {
        drawText(r, app->font_md, app->champ_id.texte,
                 panel.x + 72, cy_id + 31, texte);
    } else {
        drawText(r, app->font_md, "Entrez votre identifiant",
                 panel.x + 72, cy_id + 31, (SDL_Color){190, 200, 215, 255});
    }

    /* Curseur clignotant */
    if (app->champ_id.actif && (SDL_GetTicks() / 500) % 2 == 0) {
        int tw = 0;
        if (app->champ_id.longueur > 0)
            TTF_SizeUTF8(app->font_md, app->champ_id.texte, &tw, NULL);
        SDL_SetRenderDrawColor(r, C_TEXTE, 255);
        SDL_RenderDrawLine(r, panel.x + 72 + tw, cy_id + 30,
                              panel.x + 72 + tw, cy_id + 56);
    }

    /* ── Champ Mot de passe ── */
    int cy_mdp = cy_id + 80;
    drawText(r, app->font_sm, "MOT DE PASSE",
             panel.x + 30, cy_mdp, gris);

    SDL_Color bord_mdp = app->champ_mdp.actif
        ? (SDL_Color){C_VERT, 255}
        : (SDL_Color){C_GRIS_LT, 255};

    fillRoundRect(r, panel.x + 30, cy_mdp + 20,
                  430, 44, 8, (SDL_Color){245, 248, 252, 255});
    drawRoundRect(r, panel.x + 30, cy_mdp + 20,
                  430, 44, 8, bord_mdp);

    /* Icône cadenas */
    drawText(r, app->font_md, "*",
             panel.x + 45, cy_mdp + 31, gris);

    /* Texte masqué */
    char masque[MAX_INPUT] = "";
    if (!app->mdp_visible) {
        for (int i = 0; i < app->champ_mdp.longueur; i++)
            strcat(masque, "●");
    } else {
        strcpy(masque, app->champ_mdp.texte);
    }

    if (app->champ_mdp.longueur > 0) {
        drawText(r, app->font_md, masque,
                 panel.x + 72, cy_mdp + 31, texte);
    } else {
        drawText(r, app->font_md, "Entrez votre mot de passe",
                 panel.x + 72, cy_mdp + 31,
                 (SDL_Color){190, 200, 215, 255});
    }

    /* Curseur MDP */
    if (app->champ_mdp.actif && (SDL_GetTicks() / 500) % 2 == 0) {
        int tw = 0;
        if (strlen(masque) > 0)
            TTF_SizeUTF8(app->font_md, masque, &tw, NULL);
        SDL_SetRenderDrawColor(r, C_TEXTE, 255);
        SDL_RenderDrawLine(r, panel.x + 72 + tw, cy_mdp + 30,
                              panel.x + 72 + tw, cy_mdp + 56);
    }

    /* Bouton afficher/masquer mdp */
    SDL_Color colOeil = app->mdp_visible
        ? (SDL_Color){C_VERT, 255}
        : (SDL_Color){C_GRIS, 255};
    drawText(r, app->font_sm,
             app->mdp_visible ? "[Masquer]" : "[Afficher]",
             panel.x + 385, cy_mdp + 33, colOeil);

    /* ── Message d'erreur ── */
    if (strlen(app->message_erreur) > 0) {
        Uint32 elapsed = SDL_GetTicks() - app->temps_erreur;
        if (elapsed < 3000) {
            fillRoundRect(r, panel.x + 30, cy_mdp + 75,
                          430, 36, 6,
                          (SDL_Color){255, 235, 235, 255});
            drawRoundRect(r, panel.x + 30, cy_mdp + 75,
                          430, 36, 6,
                          (SDL_Color){C_ROUGE, 200});
            drawText(r, app->font_sm, app->message_erreur,
                     panel.x + 45, cy_mdp + 86,
                     (SDL_Color){C_ROUGE, 255});
        } else {
            app->message_erreur[0] = '\0';
        }
    }

    /* ── Bouton Connexion ── */
    SDL_Color btn_col = app->btn_connexion.survol
        ? (SDL_Color){C_VERT_LT, 255}
        : (SDL_Color){C_VERT,    255};

    fillRoundRect(r, panel.x + 30, panel.y + 390,
                  430, 50, 10, btn_col);

    /* Effet brillance haut du bouton */
    SDL_SetRenderDrawBlendMode(r, SDL_BLENDMODE_BLEND);
    fillRoundRect(r, panel.x + 30, panel.y + 390,
                  430, 25, 10,
                  (SDL_Color){255, 255, 255, 25});

    drawTextCenter(r, app->font_lg, "Se connecter",
                   (SDL_Rect){panel.x+30, panel.y+390, 430, 50},
                   (SDL_Color){255, 255, 255, 255});

    /* ── Lien mot de passe oublié ── */
    SDL_Color lien = app->btn_mdp_oublie.survol
        ? (SDL_Color){C_VERT, 255}
        : (SDL_Color){C_BLEU, 255};
    drawTextCenter(r, app->font_sm, "Mot de passe oublie ?",
                   (SDL_Rect){panel.x+30, panel.y+452, 430, 22}, lien);

    /* Soulignement si survol */
    if (app->btn_mdp_oublie.survol) {
        int tw, th;
        TTF_SizeUTF8(app->font_sm, "Mot de passe oublie ?", &tw, &th);
        int lx = panel.x + 30 + (430 - tw) / 2;
        SDL_SetRenderDrawColor(r, C_VERT, 255);
        SDL_RenderDrawLine(r, lx, panel.y + 452 + th,
                              lx + tw, panel.y + 452 + th);
    }

    /* Mise à jour des rects pour la détection */
    app->btn_connexion.rect  = (SDL_Rect){panel.x+30, panel.y+390, 430, 50};
    app->btn_mdp_oublie.rect = (SDL_Rect){panel.x+30, panel.y+452, 430, 22};
    app->champ_id.rect       = (SDL_Rect){panel.x+30, cy_id+20,    430, 44};
    app->champ_mdp.rect      = (SDL_Rect){panel.x+30, cy_mdp+20,   430, 44};
}

/* ================================================================
   GESTION DES CLICS SUR LES RÔLES
   ================================================================ */
static void gererClicRole(App *app, int mx, int my) {
    int rx = 370 + 30;
    int ry = 60 + 122;
    int rbw = 68, rbh = 28;

    for (int i = 0; i < 7; i++) {
        int col = i % 4, row = i / 4;
        int bx = rx + col * (rbw + 6);
        int by = ry + row * (rbh + 6);
        if (mx >= bx && mx <= bx + rbw &&
            my >= by && my <= by + rbh) {
            app->role_selectionne = i;
            return;
        }
    }
}

/* ================================================================
   GESTION CLIC AFFICHER/MASQUER MDP
   ================================================================ */
static int clicAfficherMdp(App *app, int mx, int my) {
    /* Zone bouton [Afficher]/[Masquer] */
    SDL_Rect zone = {370 + 385, 60 + 208 + 80 + 20 + 13, 80, 18};
    return (mx >= zone.x && mx <= zone.x + zone.w &&
            my >= zone.y && my <= zone.y + zone.h);
}

/* ================================================================
   VALIDATION CONNEXION (simplifiée — à connecter à comptes.c)
   ================================================================ */
static void validerConnexion(App *app) {
    if (app->champ_id.longueur == 0) {
        strcpy(app->message_erreur, "  Veuillez entrer votre identifiant.");
        app->temps_erreur = SDL_GetTicks();
        return;
    }
    if (app->champ_mdp.longueur == 0) {
        strcpy(app->message_erreur, "  Veuillez entrer votre mot de passe.");
        app->temps_erreur = SDL_GetTicks();
        return;
    }

    /* TODO : appeler authentifier(champ_id, champ_mdp, role) */
    /* Simulation : admin/admin passe */
    if (strcmp(app->champ_id.texte,  "admin") == 0 &&
        strcmp(app->champ_mdp.texte, "admin") == 0) {
        app->ecran = ECRAN_DASHBOARD;
    } else {
        strcpy(app->message_erreur,
               "  Identifiant ou mot de passe incorrect.");
        app->temps_erreur = SDL_GetTicks();
        /* Vide le champ mdp */
        memset(app->champ_mdp.texte, 0, MAX_INPUT);
        app->champ_mdp.longueur = 0;
    }
}

/* ================================================================
   BOUCLE PRINCIPALE
   ================================================================ */
static void boucle(App *app) {
    SDL_Event e;
    Uint32 dernier = SDL_GetTicks();

    while (app->ecran != ECRAN_QUITTER) {

        /* Animation logo */
        Uint32 maintenant = SDL_GetTicks();
        float dt = (maintenant - dernier) / 1000.0f;
        dernier = maintenant;
        app->anim_logo += dt * 2.0f;

        /* ── Événements ── */
        while (SDL_PollEvent(&e)) {
            switch (e.type) {

            case SDL_QUIT:
                app->ecran = ECRAN_QUITTER;
                break;

            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    app->ecran = ECRAN_QUITTER;
                } else if (e.key.keysym.sym == SDLK_TAB) {
                    /* Tab = passer d'un champ à l'autre */
                    if (app->champ_id.actif) {
                        app->champ_id.actif  = 0;
                        app->champ_mdp.actif = 1;
                    } else {
                        app->champ_id.actif  = 1;
                        app->champ_mdp.actif = 0;
                    }
                } else if (e.key.keysym.sym == SDLK_RETURN) {
                    validerConnexion(app);
                } else if (e.key.keysym.sym == SDLK_BACKSPACE) {
                    if (app->champ_id.actif && app->champ_id.longueur > 0) {
                        app->champ_id.texte[--app->champ_id.longueur] = '\0';
                    } else if (app->champ_mdp.actif && app->champ_mdp.longueur > 0) {
                        app->champ_mdp.texte[--app->champ_mdp.longueur] = '\0';
                    }
                }
                break;

            case SDL_TEXTINPUT:
                if (app->champ_id.actif &&
                    app->champ_id.longueur < MAX_INPUT - 1) {
                    strcat(app->champ_id.texte, e.text.text);
                    app->champ_id.longueur += (int)strlen(e.text.text);
                } else if (app->champ_mdp.actif &&
                           app->champ_mdp.longueur < MAX_INPUT - 1) {
                    strcat(app->champ_mdp.texte, e.text.text);
                    app->champ_mdp.longueur += (int)strlen(e.text.text);
                }
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (e.button.button == SDL_BUTTON_LEFT) {
                    int mx = e.button.x, my = e.button.y;

                    /* Champ ID */
                    if (mx >= app->champ_id.rect.x &&
                        mx <= app->champ_id.rect.x + app->champ_id.rect.w &&
                        my >= app->champ_id.rect.y &&
                        my <= app->champ_id.rect.y + app->champ_id.rect.h) {
                        app->champ_id.actif  = 1;
                        app->champ_mdp.actif = 0;
                        SDL_StartTextInput();
                    }
                    /* Champ MDP */
                    else if (mx >= app->champ_mdp.rect.x &&
                             mx <= app->champ_mdp.rect.x + app->champ_mdp.rect.w &&
                             my >= app->champ_mdp.rect.y &&
                             my <= app->champ_mdp.rect.y + app->champ_mdp.rect.h) {
                        app->champ_mdp.actif = 1;
                        app->champ_id.actif  = 0;
                        SDL_StartTextInput();
                        /* Clic sur [Afficher/Masquer] */
                        if (clicAfficherMdp(app, mx, my))
                            app->mdp_visible = !app->mdp_visible;
                    }
                    /* Bouton connexion */
                    else if (mx >= app->btn_connexion.rect.x &&
                             mx <= app->btn_connexion.rect.x + app->btn_connexion.rect.w &&
                             my >= app->btn_connexion.rect.y &&
                             my <= app->btn_connexion.rect.y + app->btn_connexion.rect.h) {
                        validerConnexion(app);
                    }
                    /* Lien mdp oublié */
                    else if (mx >= app->btn_mdp_oublie.rect.x &&
                             mx <= app->btn_mdp_oublie.rect.x + app->btn_mdp_oublie.rect.w &&
                             my >= app->btn_mdp_oublie.rect.y &&
                             my <= app->btn_mdp_oublie.rect.y + app->btn_mdp_oublie.rect.h) {
                        strcpy(app->message_erreur,
                               "  Contactez l'administrateur systeme.");
                        app->temps_erreur = SDL_GetTicks();
                    }
                    /* Rôles */
                    else {
                        app->champ_id.actif  = 0;
                        app->champ_mdp.actif = 0;
                        SDL_StopTextInput();
                        gererClicRole(app, mx, my);
                    }
                }
                break;

            case SDL_MOUSEMOTION: {
                int mx = e.motion.x, my = e.motion.y;
                /* Survol bouton connexion */
                app->btn_connexion.survol =
                    (mx >= app->btn_connexion.rect.x &&
                     mx <= app->btn_connexion.rect.x + app->btn_connexion.rect.w &&
                     my >= app->btn_connexion.rect.y &&
                     my <= app->btn_connexion.rect.y + app->btn_connexion.rect.h);
                /* Survol lien mdp */
                app->btn_mdp_oublie.survol =
                    (mx >= app->btn_mdp_oublie.rect.x &&
                     mx <= app->btn_mdp_oublie.rect.x + app->btn_mdp_oublie.rect.w &&
                     my >= app->btn_mdp_oublie.rect.y &&
                     my <= app->btn_mdp_oublie.rect.y + app->btn_mdp_oublie.rect.h);
                /* Curseur */
                SDL_SetCursor(SDL_CreateSystemCursor(
                    (app->btn_connexion.survol || app->btn_mdp_oublie.survol)
                    ? SDL_SYSTEM_CURSOR_HAND : SDL_SYSTEM_CURSOR_ARROW));
                break;
            }
            } /* switch */
        } /* while PollEvent */

        /* ── Rendu ── */
        SDL_SetRenderDrawBlendMode(app->rendu, SDL_BLENDMODE_BLEND);
        dessinerFond(app);

        if (app->ecran == ECRAN_LOGIN)
            dessinerPanneauLogin(app);
        else if (app->ecran == ECRAN_DASHBOARD) {
            /* Placeholder dashboard — à remplacer */
            SDL_SetRenderDrawColor(app->rendu, C_BG, 255);
            SDL_RenderClear(app->rendu);
            drawText(app->rendu, app->font_xl,
                     "Bienvenue dans SICKBASE !",
                     250, 260,
                     (SDL_Color){C_BLEU, 255});
            drawText(app->rendu, app->font_md,
                     "Dashboard en cours de developpement...",
                     230, 310,
                     (SDL_Color){C_GRIS, 255});
        }

        SDL_RenderPresent(app->rendu);
        SDL_Delay(16); /* ~60 FPS */
    }
}

/* ================================================================
   POINT D'ENTRÉE
   ================================================================ */
int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() < 0) {
        fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
        SDL_Quit(); return 1;
    }

    App app = {0};
    app.ecran             = ECRAN_LOGIN;
    app.role_selectionne  = 0;
    app.mdp_visible       = 0;

    app.fenetre = SDL_CreateWindow(
        "SICKBASE  -  Systeme de Gestion Hospitaliere",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIN_W, WIN_H, SDL_WINDOW_SHOWN);

    if (!app.fenetre) {
        fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        TTF_Quit(); SDL_Quit(); return 1;
    }

    app.rendu = SDL_CreateRenderer(app.fenetre, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!app.rendu) {
        fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(app.fenetre);
        TTF_Quit(); SDL_Quit(); return 1;
    }

    SDL_SetRenderDrawBlendMode(app.rendu, SDL_BLENDMODE_BLEND);

    /* Polices */
    app.font_sm  = chargerPolice(13);
    app.font_md  = chargerPolice(17);
    app.font_lg  = chargerPolice(24);
    app.font_xl  = chargerPolice(32);
    app.font_xxl = chargerPolice(52);

    SDL_StartTextInput();
    boucle(&app);
    SDL_StopTextInput();

    /* Nettoyage */
    if (app.font_sm)  TTF_CloseFont(app.font_sm);
    if (app.font_md)  TTF_CloseFont(app.font_md);
    if (app.font_lg)  TTF_CloseFont(app.font_lg);
    if (app.font_xl)  TTF_CloseFont(app.font_xl);
    if (app.font_xxl) TTF_CloseFont(app.font_xxl);
    SDL_DestroyRenderer(app.rendu);
    SDL_DestroyWindow(app.fenetre);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
