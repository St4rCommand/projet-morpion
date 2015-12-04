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

void *gestionnairePartie(void *arg) {

    int *clientSockets;
    clientSockets = (int*) arg;

    char *messageClient[2];
    messageClient[0] = lireMessage(clientSockets[0]);
    messageClient[1] = lireMessage(clientSockets[1]);



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
        envoyerMessage(clientSockets[0], message);
        envoyerMessage(clientSockets[1], message);
    }

    //Lire OK et demander au joueur 1 de jouer
    messageClient[0] = lireMessage(clientSockets[0]);
    messageClient[1] = lireMessage(clientSockets[1]);

    if ((int) strlen(messageClient[0]) <= 0 || (int) strlen(messageClient[1]) <= 0) {
        perror("connexion avec le client interrompue.\n");
    } else {
        // Envoyer une réponse
        envoyerMessage(clientSockets[0], "PLAY_");
    }

    int joueur = 0;

    while(1) {

        /*// Faire jouer le premier joueur
        messageClient[0] = lireMessage(clientSockets[0]);
        if ((int) strlen(messageClient[0]) <= 0) {
            perror("connexion avec le joueur 1 interrompue.\nFin de la partie.\n");
            envoyerMessage(clientSockets[1], "connexion avec le joueur 1 interrompue.\nFin de la partie.\n");
            break;
        } else {
            printf("Joueur 1 envoie : %s\n", messageClient[0]);

            envoyerMessage(clientSockets[0], messageClient[0]);
            envoyerMessage(clientSockets[1], messageClient[0]);
        }

        // Faire jouer le deuxième joueur
        messageClient[1] = lireMessage(clientSockets[1]);
        if ((int) strlen(messageClient[1]) <= 0) {
            perror("connexion avec le joueur 2 interrompue.\nFin de la partie.\n");
            envoyerMessage(clientSockets[0], "connexion avec le joueur 2 interrompue.\nFin de la partie.\n");
            break;
        } else {
            printf("Joueur 1 envoie : %s\n", messageClient[1]);

            envoyerMessage(clientSockets[0], messageClient[0]);
            envoyerMessage(clientSockets[1], messageClient[0]);
        }*/

        gestionnaireJoueur(clientSockets, joueur%2, (joueur+1)%2);

        joueur++;
    }

    /* === FIN DE LA CONNEXION AVEC LE CLIENT === */
    // Fermeture du socket
    close(clientSockets[0]);
    close(clientSockets[1]);
    printf("Fin de la connexion avec le client.\n");

    // Supprimer le thread
    pthread_exit(NULL);
}

int gestionnaireJoueur(int *clientSockets, int joueurCourant, int joueurSuivant) {
    char *messageRecu = lireMessage(clientSockets[joueurCourant]);

    if ((int) strlen(messageRecu) <= 0) {
        perror("Connexion avec le joueur courant interrompue.\nFin de la partie.\n");
        envoyerMessage(clientSockets[(joueurCourant+1)%2], "connexion avec le joueur 2 interrompue.\nFin de la partie.\n");
        return -1;
    }

    char **messageRecuTraite = analyserMessage(messageRecu);

    // Analyse de la requete
    switch (analyserRequete(messageRecuTraite[0])) {
        case MESSAGE_RECU_LOCATION:
            gestionnairePlacerSymbole(messageRecuTraite);
            envoyerMessage(clientSockets[joueurSuivant], "PLAY_");
            break;
        case MESSAGE_RECU_QUIT:
            break;
        default:
            break;
    }

    return 0;
}

void gestionnairePlacerSymbole(char **messageRecuTraite){
    int x = messageRecuTraite[1][0];
    int y = messageRecuTraite[2][0];

    // jouer un pion
    printf("Joueur %d - %d\n", x, y);
}
