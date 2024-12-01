#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define DELAI 200000
#define MAX_LARGEUR 50
#define MAX_HAUTEUR 50

typedef struct {
    int x, y;   // Position actuelle
    int dx, dy; // Direction de déplacement
    int actif;  // 1 si le véhicule est actif (dans la grille), 0 sinon
} Vehicule;

void lire_fichier(const char *nom_fichier, int *largeur, int *hauteur, int *nb_vehicules, int *route_verticale, int *route_horizontale) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    fscanf(fichier, "%d %d", largeur, hauteur);
    fscanf(fichier, "%d", nb_vehicules);
    fscanf(fichier, "%d %d", route_verticale, route_horizontale);

    fclose(fichier);
}

void afficher_grille(char grille[][MAX_LARGEUR], int largeur, int hauteur) {
    system("cls"); // Pour Linux/macOS (remplacez par "cls" pour Windows)
    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            putchar(grille[i][j]);
        }
        putchar('\n');
    }
}

void deplacer_vehicules(Vehicule vehicules[], int nb_vehicules, char grille[][MAX_LARGEUR], int largeur, int hauteur) {
    while (1) {
        int vehicules_actifs = 0;

        for (int v = 0; v < nb_vehicules; v++) {
            if (vehicules[v].actif) {
                //int vehicules_actifs = 1;
                int bloque = 0;

                do {
                    int new_x = vehicules[v].x + vehicules[v].dx;
                    int new_y = vehicules[v].y + vehicules[v].dy;

                    if (new_x < 0 || new_x >= largeur || new_y < 0 || new_y >= hauteur) {
                        grille[vehicules[v].y][vehicules[v].x] = (vehicules[v].dx == 0) ? '|' : '-';
                        vehicules[v].actif = 0;
                        bloque = 1;

                    } else if (grille[new_y][new_x] == '*') {
                        grille[vehicules[v].y][vehicules[v].x] = '*';
                        bloque = 1;
                        vehicules_actifs =0;
                    } else {
                        grille[vehicules[v].y][vehicules[v].x] = (vehicules[v].dx == 0) ? '|' : '-';
                        vehicules[v].x = new_x;
                        vehicules[v].y = new_y;
                        grille[vehicules[v].y][vehicules[v].x] = '*';
                    }
                    afficher_grille(grille, largeur, hauteur);
                    usleep(DELAI);
                } while (!bloque);
            }
        }

        for (int v = 0; v < nb_vehicules; v++) {if (vehicules[v].actif ){continue;} else {break;} //afficher_grille(grille, largeur, hauteur);
        //usleep(DELAI);
    }
}}

void generer_grille(int largeur, int hauteur, int nb_vehicules, int route_verticale, int route_horizontale) {
    char grille[MAX_HAUTEUR][MAX_LARGEUR];
    Vehicule vehicules[nb_vehicules];

    // Initialiser la grille avec des espaces
    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            grille[i][j] = ' ';
        }
    }

    // Dessiner les routes
    int colonne_verticale = largeur / 2;
    int ligne_horizontale = hauteur / 2;

    for (int i = 0; i < hauteur; i++) {
        grille[i][colonne_verticale] = '|';
    }
    for (int j = 0; j < largeur; j++) {
        grille[ligne_horizontale][j] = '-';
    }

    // Placer les véhicules
    srand(time(NULL));
    for (int v = 0; v < nb_vehicules; v++) {
        int x, y, dx, dy;
        do {
            if (rand() % 2 == 0) {
                x = colonne_verticale;
                y = rand() % hauteur;
                dx = 0;
                dy = -1;
            } else {
                x = rand() % largeur;
                y = ligne_horizontale;
                dx = -1;
                dy = 0;
            }
        } while (grille[y][x] != '-' && grille[y][x] != '|');

        grille[y][x] = '*';
        vehicules[v] = (Vehicule){x, y, dx, dy, 1};
    }

    afficher_grille(grille, largeur, hauteur);
    usleep(DELAI);
    deplacer_vehicules(vehicules, nb_vehicules, grille, largeur, hauteur);
}

int main() {
    int largeur, hauteur, nb_vehicules, route_verticale, route_horizontale;

    lire_fichier("texte.txt", &largeur, &hauteur, &nb_vehicules, &route_verticale, &route_horizontale);
    generer_grille(largeur, hauteur, nb_vehicules, route_verticale, route_horizontale);

    return 0;
}
