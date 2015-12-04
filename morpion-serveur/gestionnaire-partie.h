//
// Created by romain on 02/12/15.
//

#ifndef PROJET_MORPION_GESTIONNAIRE_PARTIE_H
#define PROJET_MORPION_GESTIONNAIRE_PARTIE_H

void *gestionnairePartie(void *arg);

int gestionnaireJoueur(int *clientSockets, int joueurCourant, int joueurSuivant);

void gestionnairePlacerSymbole(char **messageRecuTraite);

#endif //PROJET_MORPION_GESTIONNAIRE_PARTIE_H
