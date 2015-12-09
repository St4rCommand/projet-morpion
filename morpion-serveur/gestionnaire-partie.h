//
// Created by romain on 02/12/15.
//

#ifndef PROJET_MORPION_GESTIONNAIRE_PARTIE_H
#define PROJET_MORPION_GESTIONNAIRE_PARTIE_H

#define PARTIE_CONTINUER 0
#define PARTIE_FIN 1

#include "morpion-moteur.h"



struct donnees_partie {
    int joueur_1;
    int joueur_2;
};

void *gestionnairePartie(void *arg);

int gestionTourJoueur(struct donnees_partie *partie, int joueurCourant, int joueurSuivant, int **grille);

void gestionnairePlacerSymbole(char **messageRecuTraite, int **grille, int joueurCourant, int socketJoueurCourant);


#endif //PROJET_MORPION_GESTIONNAIRE_PARTIE_H
