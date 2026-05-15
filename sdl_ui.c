#include "sdl_ui.h"
#include "comptes.h"
#include "patient.h"
#include "personnel.h"
#include "consultations.h"
#include "utils.h"
#include "fichiers.h"
#include "fichiers_personnel.h"
#include "fichiers_relations.h"

// ==================== COULEURS ====================
Color COLOR_BG = {15, 25, 45, 255};
Color COLOR_SIDEBAR = {5, 10, 25, 220};
Color COLOR_PRIMARY = {0, 150, 210, 255};
Color COLOR_SUCCESS = {0, 200, 100, 255};
Color COLOR_ERROR = {220, 50, 70, 255};
Color COLOR_WARNING = {255, 160, 0, 255};
Color COLOR_TITLE = {255, 255, 255, 255};
Color COLOR_TEXT = {220, 220, 240, 255};
Color COLOR_BORDER = {100, 100, 120, 255};
Color COLOR_HOVER = {0, 170, 230, 255};

// ==================== VARIABLES GLOBALES ====================
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;
TTF_Font* gFontTitle = NULL;

int currentScreen = 0;
int currentUserProfile = 0;
int currentUserId = 0;

// Déclarations externes
extern int login_with_params(char* login, char* password);
extern void logout();
extern int estCompteComplet(int id_compte);
extern void completerDossierPatient(int id_compte);

// ==================== FONCTIONS DE DESSIN ====================
void setColor(Color color) {
    SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);
}

void drawRect(int x, int y, int w, int h, Color color, int filled) {
    setColor(color);
    SDL_Rect rect = {x, y, w, h};
    if (filled) SDL_RenderFillRect(gRenderer, &rect);
    else SDL_RenderDrawRect(gRenderer, &rect);
}

void drawText(TTF_Font* font, const char* text, int x, int y, Color color) {
    if (!font || !text) return;
    SDL_Color sdlColor = {color.r, color.g, color.b, color.a};
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, sdlColor);
    if (!surface) return;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_RenderCopy(gRenderer, texture, NULL, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void drawButton(Button* btn, TTF_Font* font) {
    Color bgColor = btn->is_hover ? COLOR_HOVER : (btn->is_active ? COLOR_PRIMARY : COLOR_SIDEBAR);
    drawRect(btn->rect.x, btn->rect.y, btn->rect.w, btn->rect.h, bgColor, 1);
    drawRect(btn->rect.x, btn->rect.y, btn->rect.w, btn->rect.h, COLOR_BORDER, 0);

    int textW, textH;
    TTF_SizeUTF8(font, btn->text, &textW, &textH);
    int textX = btn->rect.x + (btn->rect.w - textW) / 2;
    int textY = btn->rect.y + (btn->rect.h - textH) / 2;
    drawText(font, btn->text, textX, textY, COLOR_TEXT);
}

void drawInput(InputField* input, TTF_Font* font) {
    Color borderColor = input->is_focused ? COLOR_PRIMARY : COLOR_BORDER;
    drawRect(input->rect.x, input->rect.y, input->rect.w, input->rect.h, COLOR_BG, 1);
    drawRect(input->rect.x, input->rect.y, input->rect.w, input->rect.h, borderColor, 0);
    drawText(font, input->label, input->rect.x, input->rect.y - 20, COLOR_TEXT);
    drawText(font, input->value, input->rect.x + 10, input->rect.y + 10, COLOR_TEXT);
}

// ==================== INITIALISATION ====================
int initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 0;
    if (TTF_Init() < 0) return 0;

    gWindow = SDL_CreateWindow("SICKBASE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!gWindow) return 0;

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!gRenderer) return 0;

    gFont = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", FONT_SIZE);
    gFontTitle = TTF_OpenFont("C:/Windows/Fonts/arialbd.ttf", TITLE_SIZE);

    return 1;
}

void closeSDL() {
    if (gFont) TTF_CloseFont(gFont);
    if (gFontTitle) TTF_CloseFont(gFontTitle);
    if (gRenderer) SDL_DestroyRenderer(gRenderer);
    if (gWindow) SDL_DestroyWindow(gWindow);
    TTF_Quit();
    SDL_Quit();
}

// ==================== ECRAN DE CONNEXION ====================
void showLoginScreen() {
    InputField loginInput = {{SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 30, 300, 40}, "Login", "", 0};
    InputField passwordInput = {{SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 20, 300, 40}, "Mot de passe", "", 0};
    Button loginBtn = {{SCREEN_WIDTH/2 - 80, SCREEN_HEIGHT/2 + 90, 160, 45}, "CONNEXION", 1};
    Button quitBtn = {{SCREEN_WIDTH/2 - 80, SCREEN_HEIGHT/2 + 150, 160, 45}, "QUITTER", 2};

    int running = 1;
    SDL_Event e;
    char errorMsg[200] = "";

    while (running) {
        setColor(COLOR_BG);
        SDL_RenderClear(gRenderer);

        drawText(gFontTitle, "SICKBASE", SCREEN_WIDTH/2 - 80, SCREEN_HEIGHT/2 - 200, COLOR_TITLE);
        drawText(gFont, "Systeme de Gestion Hospitaliere", SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 160, COLOR_TEXT);

        drawInput(&loginInput, gFont);
        drawInput(&passwordInput, gFont);
        drawButton(&loginBtn, gFont);
        drawButton(&quitBtn, gFont);

        if (strlen(errorMsg) > 0) {
            drawText(gFont, errorMsg, SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 210, COLOR_ERROR);
        }

        SDL_RenderPresent(gRenderer);

        while (SDL_PollEvent(&e)) {
            int mx = e.button.x, my = e.button.y;

            switch (e.type) {
                case SDL_QUIT:
                    running = 0;
                    currentScreen = -1;
                    break;

                case SDL_MOUSEMOTION:
                    loginBtn.is_hover = (mx >= loginBtn.rect.x && mx <= loginBtn.rect.x + loginBtn.rect.w &&
                                         my >= loginBtn.rect.y && my <= loginBtn.rect.y + loginBtn.rect.h);
                    quitBtn.is_hover = (mx >= quitBtn.rect.x && mx <= quitBtn.rect.x + quitBtn.rect.w &&
                                        my >= quitBtn.rect.y && my <= quitBtn.rect.y + quitBtn.rect.h);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        loginInput.is_focused = (mx >= loginInput.rect.x && mx <= loginInput.rect.x + loginInput.rect.w &&
                                                  my >= loginInput.rect.y && my <= loginInput.rect.y + loginInput.rect.h);
                        passwordInput.is_focused = (mx >= passwordInput.rect.x && mx <= passwordInput.rect.x + passwordInput.rect.w &&
                                                     my >= passwordInput.rect.y && my <= passwordInput.rect.y + passwordInput.rect.h);

                        if (loginBtn.is_hover) {
                            if (strlen(loginInput.value) > 0 && strlen(passwordInput.value) > 0) {
                                if (login_with_params(loginInput.value, passwordInput.value)) {
                                    currentUserProfile = utilisateur_actuel->profil;
                                    currentUserId = utilisateur_actuel->id_compte;
                                    running = 0;
                                    currentScreen = 1;
                                } else {
                                    strcpy(errorMsg, "Login ou mot de passe incorrect");
                                }
                            } else {
                                strcpy(errorMsg, "Veuillez remplir tous les champs");
                            }
                        }
                        if (quitBtn.is_hover) {
                            running = 0;
                            currentScreen = -1;
                        }
                    }
                    break;

                case SDL_TEXTINPUT:
                    if (loginInput.is_focused) {
                        strncat(loginInput.value, e.text.text, 49);
                    }
                    if (passwordInput.is_focused) {
                        strncat(passwordInput.value, e.text.text, 49);
                    }
                    break;

                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_BACKSPACE) {
                        if (loginInput.is_focused && strlen(loginInput.value) > 0)
                            loginInput.value[strlen(loginInput.value)-1] = '\0';
                        if (passwordInput.is_focused && strlen(passwordInput.value) > 0)
                            passwordInput.value[strlen(passwordInput.value)-1] = '\0';
                    }
                    if (e.key.keysym.sym == SDLK_RETURN) {
                        if (strlen(loginInput.value) > 0 && strlen(passwordInput.value) > 0) {
                            if (login_with_params(loginInput.value, passwordInput.value)) {
                                currentUserProfile = utilisateur_actuel->profil;
                                currentUserId = utilisateur_actuel->id_compte;
                                running = 0;
                                currentScreen = 1;
                            } else {
                                strcpy(errorMsg, "Login ou mot de passe incorrect");
                            }
                        }
                    }
                    break;
            }
        }
        SDL_Delay(16);
    }
}

// ==================== MENU SUPER ADMIN SIMPLIFIE ====================
void showSuperAdminDashboard() {
    Button menuButtons[4];
    int currentMenu = 0;
    int running = 1;
    SDL_Event e;

    // Initialisation des boutons
    menuButtons[0].rect = (SDL_Rect){SIDEBAR_WIDTH + 50, 120, 200, 45};
    strcpy(menuButtons[0].text, "Patients");
    menuButtons[1].rect = (SDL_Rect){SIDEBAR_WIDTH + 50, 180, 200, 45};
    strcpy(menuButtons[1].text, "Personnel");
    menuButtons[2].rect = (SDL_Rect){SIDEBAR_WIDTH + 50, 240, 200, 45};
    strcpy(menuButtons[2].text, "Consultations");
    menuButtons[3].rect = (SDL_Rect){SIDEBAR_WIDTH + 50, 300, 200, 45};
    strcpy(menuButtons[3].text, "Deconnexion");

    while (running) {
        setColor(COLOR_BG);
        SDL_RenderClear(gRenderer);

        // Sidebar
        drawRect(0, 0, SIDEBAR_WIDTH, SCREEN_HEIGHT, COLOR_SIDEBAR, 1);
        drawText(gFontTitle, "SICKBASE", 20, 30, COLOR_TITLE);
        drawText(gFont, "Super Admin", 20, 70, COLOR_TEXT);

        // Header
        drawRect(0, 0, SCREEN_WIDTH, HEADER_HEIGHT, COLOR_SIDEBAR, 1);
        drawText(gFontTitle, "Super Administrateur", SIDEBAR_WIDTH + 25, 20, COLOR_TITLE);

        // Boutons du menu
        for (int i = 0; i < 4; i++) {
            menuButtons[i].is_active = (i == currentMenu);
            drawButton(&menuButtons[i], gFont);
        }

        // Contenu selon le menu s�lectionn�
        int cx = SIDEBAR_WIDTH + 300;
        int cy = HEADER_HEIGHT + 50;
        char buffer[100];

        switch (currentMenu) {
            case 0:
                drawText(gFontTitle, "GESTION DES PATIENTS", cx, cy, COLOR_TITLE);
                sprintf(buffer, "Nombre de patients: %d", nombrePatients);
                drawText(gFont, buffer, cx, cy + 50, COLOR_TEXT);
                drawText(gFont, "Cliquez sur le bouton ci-dessous", cx, cy + 100, COLOR_TEXT);

                Button btnAdd = {{cx, cy + 150, 150, 40}, "Ajouter Patient", 1};
                drawButton(&btnAdd, gFont);
                break;
            case 1:
                drawText(gFontTitle, "GESTION DU PERSONNEL", cx, cy, COLOR_TITLE);
                sprintf(buffer, "Nombre d'employes: %d", nombreEmployes);
                drawText(gFont, buffer, cx, cy + 50, COLOR_TEXT);
                break;
            case 2:
                drawText(gFontTitle, "GESTION DES CONSULTATIONS", cx, cy, COLOR_TITLE);
                sprintf(buffer, "Nombre de consultations: %d", nombreConsultations);
                drawText(gFont, buffer, cx, cy + 50, COLOR_TEXT);
                break;
            case 3:
                logout();
                running = 0;
                currentScreen = 0;
                break;
        }

        SDL_RenderPresent(gRenderer);

        while (SDL_PollEvent(&e)) {
            int mx = e.button.x, my = e.button.y;

            switch (e.type) {
                case SDL_QUIT:
                    running = 0;
                    currentScreen = -1;
                    break;

                case SDL_MOUSEMOTION:
                    for (int i = 0; i < 4; i++) {
                        menuButtons[i].is_hover = (mx >= menuButtons[i].rect.x && mx <= menuButtons[i].rect.x + menuButtons[i].rect.w &&
                                                    my >= menuButtons[i].rect.y && my <= menuButtons[i].rect.y + menuButtons[i].rect.h);
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        // V�rifier les boutons du menu
                        for (int i = 0; i < 4; i++) {
                            if (menuButtons[i].is_hover) {
                                currentMenu = i;
                                break;
                            }
                        }
                        // V�rifier le bouton Ajouter Patient
                        if (currentMenu == 0) {
                            Button testBtn = {{cx, cy + 150, 150, 40}, "Ajouter Patient", 1};
                            if (mx >= testBtn.rect.x && mx <= testBtn.rect.x + testBtn.rect.w &&
                                my >= testBtn.rect.y && my <= testBtn.rect.y + testBtn.rect.h) {
                                ajouterNouveauPatient();
                            }
                        }
                    }
                    break;
            }
        }
        SDL_Delay(16);
    }
}

// ==================== MENU PRINCIPAL ====================
void showMainMenu() {
    switch (currentUserProfile) {
        case ROLE_SUPER_ADMIN:
            showSuperAdminDashboard();
            break;
        default:
            printf("Profil non supporte dans cette version\n");
            break;
    }
}

// ==================== MAIN ====================
/*
int main(int argc, char* argv[]) {
    // Chargement des donn�es
    chargerDernierID();
    chargerPatients();
    chargerDernierIDPersonnel();
    chargerPersonnel();
    chargerConsultations();
    chargerComptes();

    printf("SICKBASE - Systeme pret\n");
    printf("Patients: %d, Employes: %d, Consultations: %d\n",
           nombrePatients, nombreEmployes, nombreConsultations);

    if (!initSDL()) {
        printf("Erreur d'initialisation SDL2\n");
        return 1;
    }

    currentScreen = 0;
    while (currentScreen != -1) {
        switch (currentScreen) {
            case 0: showLoginScreen(); break;
            case 1: showMainMenu(); break;
        }
    }

    sauvegarderPatients();
    sauvegarderPersonnel();
    sauvegarderConsultations();
    sauvegarderComptes();

    closeSDL();
    return 0;
}
*/
