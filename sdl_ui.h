#ifndef SDL_UI_H
#define SDL_UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define FONT_SIZE 16
#define TITLE_SIZE 28
#define SIDEBAR_WIDTH 250
#define HEADER_HEIGHT 60

// ==================== COULEURS ====================
typedef struct {
    Uint8 r, g, b, a;
} Color;

extern Color COLOR_BG;
extern Color COLOR_SIDEBAR;
extern Color COLOR_PRIMARY;
extern Color COLOR_SUCCESS;
extern Color COLOR_ERROR;
extern Color COLOR_WARNING;
extern Color COLOR_TITLE;
extern Color COLOR_TEXT;
extern Color COLOR_BORDER;
extern Color COLOR_HOVER;

// ==================== STRUCTURES UI ====================
typedef struct {
    SDL_Rect rect;
    char text[100];
    int id;
    int is_hover;
    int is_clicked;
    int is_active;
} Button;

typedef struct {
    SDL_Rect rect;
    char label[50];
    char value[256];
    int is_focused;
} InputField;

// ==================== FONCTIONS ====================
void setColor(Color color);
void drawRect(int x, int y, int w, int h, Color color, int filled);
void drawText(TTF_Font* font, const char* text, int x, int y, Color color);
void drawButton(Button* btn, TTF_Font* font);
void drawInput(InputField* input, TTF_Font* font);

int initSDL();
void closeSDL();

void showLoginScreen();
void showMainMenu();

// ==================== VARIABLES GLOBALES ====================
extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
extern TTF_Font* gFont;
extern TTF_Font* gFontTitle;
extern int currentScreen;
extern int currentUserProfile;
extern int currentUserId;

// ==================== ROLES (DOIT CORRESPONDRE À comptes.h) ====================
// NE PAS REDEFINIR - ils viennent de comptes.h
// On declare juste les valeurs pour info
// ROLE_PATIENT = 1, ROLE_MEDECIN = 2, etc.

#endif
