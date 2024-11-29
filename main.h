#ifndef MAIN_H
#define MAIN_H

void lire_fichier(const char *nom_fichier, int *largeur, int *hauteur, int *nb_vehicules, int *route_verticale, int *route_horizontale);
void generer_grille(int largeur, int hauteur, int nb_vehicules, int route_verticale, int route_horizontale) ;
void deplacer_vehicules_sans_collision(int largeur, int hauteur, char grille[hauteur][largeur], Vehicule vehicules[], int nb_vehicules) ;
