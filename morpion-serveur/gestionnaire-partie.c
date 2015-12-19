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
#include "morpion-moteur.h"

// Thread de gestion d'une partie
void *gestionnairePartie(void *arg) {

    struct donnees_partie *partie = arg;

    char *messageClient[2];
    messageClient[0] = lireMessage(partie->joueur_1);
    messageClient[1] = lireMessage(partie->joueur_2);

    int **grille = initialiseGrille();

    // Lire le message reçu
    if ((int) strlen(messageClient[0]) <= 0 || (int) strlen(messageClient[1]) <= 0) {
        perror("connexion avec le client interrompue.\n");
    } else {

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
            break;
        }

        joueur++;
    }

    /* === FIN DE LA CONNEXION AVEC LE CLIENT === */
    // Fermeture du socket
    envoyerMessage(partie->joueur_1, "QUIT_");
    envoyerMessage(partie->joueur_2, "QUIT_");
    close(partie->joueur_1);
    close(partie->joueur_2);
    printf("Fin de la partie. %d et %d déconnectés.\n", partie->joueur_1, partie->joueur_2);

    // Supprimer le thread
    pthread_exit(NULL);
}

// Gestion du tour d'un joueur
int gestionTourJoueur(struct donnees_partie *partie, int joueurCourant, int joueurSuivant, int **grille) {
    int clientSockets[2] = {partie->joueur_1,partie->joueur_2};

    char *messageRecu = lireMessage(clientSockets[joueurCourant]);

    if ((int) strlen(messageRecu) <= 0) {
        perror("Connexion avec le joueur courant interrompue.\nFin de la partie.\n");
        //envoyerMessage(clientSockets[(joueurCourant+1)%2], "QUIT_Connexion avec l'autre joueur interrompue.\nFin de la partie.\n");
        envoyerMessage(clientSockets[(joueurCourant+1)%2], "QUIT_");
        return PARTIE_FIN;
    }

    char **messageRecuTraite = analyserMessage(messageRecu);

    // Analyse de la requete
    switch (analyserRequete(messageRecuTraite[0])) {
        case MESSAGE_RECU_LOCATION:
            gestionnairePlacerSymbole(messageRecuTraite, grille, joueurCourant, clientSockets[joueurCourant]);
            envoyerMessage(clientSockets[joueurCourant], grilleAEnvoyer(grille));
            envoyerMessage(clientSockets[joueurSuivant], grilleAEnvoyer(grille));

            if ((int) strlen(lireMessage(clientSockets[joueurCourant])) <= 0 || (int) strlen(lireMessage(clientSockets[joueurSuivant])) <= 0) {
                perror("connexion avec le client interrompue.\n");
            } else {

                if(testeFinJeu(grille)) {
                    envoyerMessage(clientSockets[joueurCourant], "WIN_");
                    envoyerMessage(clientSockets[joueurSuivant], "LOOSE_");
                    return PARTIE_FIN;
                }

                envoyerMessage(clientSockets[joueurSuivant], "PLAY_");
            }

            return PARTIE_CONTINUER;
        case MESSAGE_RECU_QUIT:
            return PARTIE_FIN;
        default:
            return PARTIE_FIN;
    }
}

// Gestion du placement d'un pion par un joueur
// Demande au joueur de rejouer si la saisie est incorrecte
void gestionnairePlacerSymbole(char **messageRecuTraite, int **grille, int joueurCourant, int socketJoueurCourant){
    int ligne = messageRecuTraite[1][0];
    int colonne = messageRecuTraite[2][0];

    int saisieCorrect = metUnPionSurLaGrille(ligne, colonne, grille, joueurCourant+1);

    while (saisieCorrect == 0) {
        envoyerMessage(socketJoueurCourant, "PLAYERROR_");
        messageRecuTraite = analyserMessage(lireMessage(socketJoueurCourant));

        ligne = messageRecuTraite[1][0];
        colonne = messageRecuTraite[2][0];
        saisieCorrect = metUnPionSurLaGrille(ligne, colonne, grille, joueurCourant+1);
    }
}
