#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // Pour la fonction usleep (gestion du délai d'affichage)

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

// Fonction pour générer et afficher la grille
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

    // Structure pour représenter les véhicules
    typedef struct {
        int x, y;   // Position actuelle
        int dx, dy; // Direction de déplacement
        int actif;  // 1 si le véhicule est actif (dans la grille), 0 sinon
    } Vehicule;

    Vehicule vehicules[nb_vehicules];

    // Placer les véhicules
    srand(time(NULL));
    for (int v = 0; v < nb_vehicules; v++) {
        int x, y, dx, dy;
        do {
            if (rand() % 2 == 0) { // Aléatoirement sur la route verticale
                x = colonne_verticale;
                y = rand() % hauteur;
                dx = 0;
                dy = -1;//(y < hauteur / 2) ? 1 : -1; // Bas ou haut
            } else { // Aléatoirement sur la route horizontale
                x = rand() % largeur;
                y = ligne_horizontale;
                dx = -1;//(x < largeur / 2) ? 1 : -1; // Droite ou gauche
                dy = 0;
            }
        } while (grille[y][x] != '-' && grille[y][x] != '|'); // Assure que le véhicule est sur une route

        grille[y][x] = '*'; // Place le véhicule sur la grille
        vehicules[v] = (Vehicule){x, y, dx, dy, 1};
    }

    // Déplacer les véhicules un par un
    // Boucle principale pour simuler tous les véhicules
// Boucle principale : continuer tant qu'il reste des véhicules actifs
while (1) {
    int vehicules_actifs = 0; // Compteur des véhicules actifs dans la grille

    // Parcourir chaque véhicule pour les déplacer
    for (int v = 0; v < nb_vehicules; v++) {
        if (vehicules[v].actif) {
            vehicules_actifs = 1; // Il reste au moins un véhicule actif

            // Effacer la position actuelle du véhicule
            grille[vehicules[v].y][vehicules[v].x] = (vehicules[v].dx == 0) ? '|' : '-';

            // Calculer la nouvelle position
            int new_x = vehicules[v].x + vehicules[v].dx;
            int new_y = vehicules[v].y + vehicules[v].dy;

            // Vérifier si le véhicule sort de la grille
            if (new_x < 0 || new_x >= largeur || new_y < 0 || new_y >= hauteur) {
                vehicules[v].actif = 0; // Désactiver le véhicule
            } else if (grille[new_y][new_x] == '*') {
                // Si la position suivante est occupée par un autre véhicule, attendre
                grille[vehicules[v].y][vehicules[v].x] = '*'; // Garder la position actuelle
                continue; // Passer au véhicule suivant
            } else {
                // Mettre à jour la position du véhicule
                vehicules[v].x = new_x;
                vehicules[v].y = new_y;

                // Placer le véhicule sur la nouvelle position
                grille[vehicules[v].y][vehicules[v].x] = '*';
            }
        }
    }

    // Si aucun véhicule n'est actif, arrêter la simulation
    if (!vehicules_actifs) {
        break; // Sortir de la boucle principale
    }

    // Afficher la grille mise à jour
    system("clear"); // Efface l'écran pour une animation fluide
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
int main() {
    // Variables pour les paramètres
    int largeur, hauteur, nb_vehicules, route_verticale, route_horizontale;

    // Lire les paramètres depuis le fichier
    lire_fichier("texte.txt", &largeur, &hauteur, &nb_vehicules, &route_verticale, &route_horizontale);

    // Générer et afficher la grille avec le déplacement des véhicules
    generer_grille(largeur, hauteur, nb_vehicules, route_verticale, route_horizontale);

    return 0;
}
