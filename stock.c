#include "stock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// ================= AJOUT MEDICAMENT =================
void ajouterMedicament() {
    FILE *f = fopen("stock.txt", "ab");
    Medicament m;

    if (f == NULL) {
        printf("Erreur ouverture fichier !\n");
        return;
    }

    printf("ID: ");
    scanf("%d", &m.id);

    printf("Nom du medicament: ");
    scanf("%s", m.nom);

    printf("Quantite: ");
    scanf("%d", &m.quantite);

    printf("Seuil d'alerte: ");
    scanf("%d", &m.seuil_alerte);

    fwrite(&m, sizeof(Medicament), 1, f);
    fclose(f);

    printf("Medicament ajoute avec succes !\n");
}

// ================= AFFICHER STOCK =================
void afficherStock() {
    FILE *f = fopen("stock.txt", "rb");
    Medicament m;

    if (f == NULL) {
        printf("Aucun stock disponible.\n");
        return;
    }

    printf("\n===== STOCK MEDICAMENTS =====\n");

    while (fread(&m, sizeof(Medicament), 1, f)) {
        printf("ID: %d | Nom: %s | Quantite: %d | Seuil: %d\n",
               m.id, m.nom, m.quantite, m.seuil_alerte);
    }

    fclose(f);
}

// ================= ALERTE STOCK FAIBLE =================
void alerteStockFaible() {
    FILE *f = fopen("stock.txt", "rb");
    Medicament m;
    int alerte = 0;

    if (f == NULL) {
        printf("Aucun stock disponible.\n");
        return;
    }

    printf("\n===== ALERTES STOCK FAIBLE =====\n");

    while (fread(&m, sizeof(Medicament), 1, f)) {
        if (m.quantite <= m.seuil_alerte) {
            printf("⚠️ %s | Quantite restante: %d\n", m.nom, m.quantite);
            alerte = 1;
        }
    }

    if (!alerte) {
        printf("Aucune alerte.\n");
    }

    fclose(f);
}

// ================= VERIFIER EXISTENCE =================
int medicamentExiste(char nom_med[]) {
    FILE *f = fopen("stock.txt", "rb");
    Medicament m;

    if (f == NULL) return 0;

    while (fread(&m, sizeof(Medicament), 1, f)) {
        if (strcmp(m.nom, nom_med) == 0) {
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

// ================= DIMINUER STOCK =================
void diminuerStock(char nom_med[]) {
    FILE *f = fopen("stock.txt", "rb+");
    Medicament m;

    if (f == NULL) {
        printf("Erreur fichier stock.\n");
        return;
    }

    while (fread(&m, sizeof(Medicament), 1, f)) {
        if (strcmp(m.nom, nom_med) == 0) {

            if (m.quantite <= 0) {
                printf("Stock epuise pour %s !\n", m.nom);
                fclose(f);
                return;
            }

            m.quantite--;
fseek(f, -(long)sizeof(Medicament), SEEK_CUR);
            fwrite(&m, sizeof(Medicament), 1, f);

            printf("Stock mis a jour pour %s.\n", m.nom);
            fclose(f);
            return;
        }
    }

    printf("Medicament non trouve !\n");
    fclose(f);
}
