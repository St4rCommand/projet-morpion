#ifndef PROJET_MORPION_MORPION_MOTEUR_H
#define PROJET_MORPION_MORPION_MOTEUR_H

#define TRUE 1
#define FALSE 0

#define VIDE 3
#define CROIX 1
#define ROND 2

#define NB_LIG 3
#define NB_COL 3

void *gestionnairePartie(void *arg);

int ** initialiseGrille();

char* grilleAEnvoyer(int **grille);

int metUnPionSurLaGrille(int ligne, int col, int **grille, int joueurCourant);

int testeFinJeu(int **grille);

#endif //PROJET_MORPION_MORPION_MOTEUR_H