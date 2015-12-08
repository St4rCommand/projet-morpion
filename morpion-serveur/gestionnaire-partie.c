//
// Created by romain on 02/12/15.
//

#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include "gestionnaire-partie.h"
#include "../morpion-outils/outils-messages.h"
#include "morpion-moteur.c"

void *gestionnairePartie(void *arg) {

    struct donnees_partie *partie = arg;

    char *messageClient[2];
    messageClient[0] = lireMessage(partie->joueur_1);
    messageClient[1] = lireMessage(partie->joueur_2);

    int grille[NB_LIG][NB_COL];
    initialiseGrille(grille);

    // Lire le message reçu
    if ((int) strlen(messageClient[0]) <= 0 || (int) strlen(messageClient[1]) <= 0) {
        perror("connexion avec le client interrompue.\n");
    } else {
        printf("Messages reçus.\n");

        char *message = malloc(TAILLE_BUFFER * sizeof(char));
        strcat(message, "GAME_");
        strcat(message, messageClient[0]);
        strcat(message, "_");
        strcat(message, messageClient[1]);

        // Envoyer une réponse
        envoyerMessage(partie->joueur_1, message);
        envoyerMessage(partie->joueur_2, message);
    }

    //Lire OK et demander au joueur 1 de jouer
    messageClient[0] = lireMessage(partie->joueur_1);
    messageClient[1] = lireMessage(partie->joueur_2);

    if ((int) strlen(messageClient[0]) <= 0 || (int) strlen(messageClient[1]) <= 0) {
        perror("connexion avec le client interrompue.\n");
    } else {
        // Envoyer une réponse
        envoyerMessage(partie->joueur_1, "PLAY_");
    }

    int joueur = 0;

    while(1) {

        if (gestionTourJoueur(partie, joueur%2, (joueur+1)%2, grille) == PARTIE_FIN) {
            // TODO envoyer message de fin aux joueurs
            break;
        }

        joueur++;
    }

    /* === FIN DE LA CONNEXION AVEC LE CLIENT === */
    // Fermeture du socket
    close(partie->joueur_1);
    close(partie->joueur_2);
    printf("Fin de la connexion avec le client.\n");

    // Supprimer le thread
    pthread_exit(NULL);
}

int gestionTourJoueur(struct donnees_partie *partie, int joueurCourant, int joueurSuivant, int grille[NB_LIG][NB_COL]) {
    int clientSockets[2] = {partie->joueur_1,partie->joueur_2};

    char *messageRecu = lireMessage(clientSockets[joueurCourant]);

    if ((int) strlen(messageRecu) <= 0) {
        perror("Connexion avec le joueur courant interrompue.\nFin de la partie.\n");
        envoyerMessage(clientSockets[(joueurCourant+1)%2], "Connexion avec l'autre joueur interrompue.\nFin de la partie.\n");
        return PARTIE_FIN;
    }

    char **messageRecuTraite = analyserMessage(messageRecu);

    // Analyse de la requete
    switch (analyserRequete(messageRecuTraite[0])) {
        case MESSAGE_RECU_LOCATION:
            gestionnairePlacerSymbole(messageRecuTraite, grille, joueurCourant, clientSockets[joueurCourant]);
            envoyerMessage(clientSockets[joueurSuivant], "PLAY_");
            break;
        case MESSAGE_RECU_QUIT:
            return PARTIE_FIN;
        default:
            break;
    }

    return PARTIE_CONTINUER;
}

void gestionnairePlacerSymbole(char **messageRecuTraite, int grille[NB_LIG][NB_COL], int joueurCourant, int socketJoueurCourant){
    int ligne = messageRecuTraite[1][0];
    int colonne = messageRecuTraite[2][0];

    while (metUnPionSurLaGrille(ligne, colonne, grille, joueurCourant+1)) {
        envoyerMessage(socketJoueurCourant, "PLAYERROR_");

    }

    // jouer un pion
    printf("Pion joué : %d - %d\n", ligne, colonne);
}
