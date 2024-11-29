#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // Pour la fonction usleep (gestion du d�lai d'affichage)

#define DELAI 200000 // Temps d'attente entre les affichages (en microsecondes)

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

// Fonction pour g�n�rer et afficher la grille
void generer_grille(int largeur, int hauteur, int nb_vehicules, int route_verticale, int route_horizontale) {
    // Initialiser la grille avec des espaces
    char grille[hauteur][largeur];
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

    // Structure pour repr�senter les v�hicules
    typedef struct {
        int x, y;   // Position actuelle
        int dx, dy; // Direction de d�placement
        int actif;  // 1 si le v�hicule est actif (dans la grille), 0 sinon
    } Vehicule;

    Vehicule vehicules[nb_vehicules];

    // Placer les v�hicules
    srand(time(NULL));
    for (int v = 0; v < nb_vehicules; v++) {
        int x, y, dx, dy;
        do {
            if (rand() % 2 == 0) { // Al�atoirement sur la route verticale
                x = colonne_verticale;
                y = rand() % hauteur;
                dx = 0;
                dy = (y < hauteur / 2) ? 1 : -1; // Bas ou haut
            } else { // Al�atoirement sur la route horizontale
                x = rand() % largeur;
                y = ligne_horizontale;
                dx = (x < largeur / 2) ? 1 : -1; // Droite ou gauche
                dy = 0;
            }
        } while (grille[y][x] != '-' && grille[y][x] != '|'); // Assure que le v�hicule est sur une route

        grille[y][x] = '*'; // Place le v�hicule sur la grille
        vehicules[v] = (Vehicule){x, y, dx, dy, 1};
    }

    // D�placer les v�hicules un par un
    for (int v = 0; v < nb_vehicules; v++) {
        while (vehicules[v].actif) {
            // Effacer la position actuelle du v�hicule
            grille[vehicules[v].y][vehicules[v].x] = (vehicules[v].dx == 0) ? '|' : '-';

            // Calculer la nouvelle position
            vehicules[v].x += vehicules[v].dx;
            vehicules[v].y += vehicules[v].dy;

            // V�rifier si le v�hicule sort de la grille
            if (vehicules[v].x < 0 || vehicules[v].x >= largeur || vehicules[v].y < 0 || vehicules[v].y >= hauteur) {
                vehicules[v].actif = 0; // D�sactiver le v�hicule
            } else {
                // Placer le v�hicule sur la nouvelle position
                grille[vehicules[v].y][vehicules[v].x] = '*';
            }

            // Afficher la grille mise � jour
            system("clear"); // Efface l'�cran pour une animation fluide
            for (int i = 0; i < hauteur; i++) {
                for (int j = 0; j < largeur; j++) {
                    putchar(grille[i][j]);
                }
                putchar('\n');
            }

            // Pause pour l'animation
            usleep(DELAI);
        }
    }
}

int main() {
    // Variables pour les param�tres
    int largeur, hauteur, nb_vehicules, route_verticale, route_horizontale;

    // Lire les param�tres depuis le fichier
    lire_fichier("texte.txt", &largeur, &hauteur, &nb_vehicules, &route_verticale, &route_horizontale);

    // G�n�rer et afficher la grille avec le d�placement des v�hicules
    generer_grille(largeur, hauteur, nb_vehicules, route_verticale, route_horizontale);

    return 0;
}
