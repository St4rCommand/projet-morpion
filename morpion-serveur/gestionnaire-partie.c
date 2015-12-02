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

    char *client[2];
    client[0] = lireMessage(clientSockets[0]);
    client[1] = lireMessage(clientSockets[1]);



    // Lire le message reçu
    if ((int) strlen(client[0]) <= 0 || (int) strlen(client[1]) <= 0) {
        perror("connexion avec le client interrompue.\n");
    } else {
        printf("Messages reçus.\n");

        char *message = malloc(1024 * sizeof(char));
        strcat(message, "Bienvenue sur la nouvelle partie ");
        strcat(message, client[0]);
        strcat(message, " et ");
        strcat(message, client[1]);

        // Envoyer une réponse
        envoyerMessage(clientSockets[0], message);
        envoyerMessage(clientSockets[1], message);
    }

    /* === FIN DE LA CONNEXION AVEC LE CLIENT === */
    // Fermeture du socket
    close(clientSockets[0]);
    close(clientSockets[1]);
    printf("Fin de la connexion avec le client.\n");

    // Supprimer le thread
    pthread_exit(NULL);
}
