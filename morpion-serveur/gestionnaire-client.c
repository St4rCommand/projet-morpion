//
// Created by romain on 25/11/15.
//

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "gestionnaire-client.h"
#include "./../morpion-outils/outils-messages.h"

void *gestionnaireClient(void *arg) {

    // Récupération du socket
    int clientSocket;
    clientSocket = *((int*) arg);

    /* === ECHANGE AVEC LE CLIENT === */
    while (1) {
        // Lire le message reçu
        if (lireMessage(clientSocket) < 0) {
            perror("connexion avec le client interrompue.\n");
            break;
        }

        // Envoyer une réponse
        envoyerMessage(clientSocket, "Coucou, le message a bien été reçu.");
    }

    /* === FIN DE LA CONNEXION AVEC LE CLIENT === */
    // Fermeture du socket
    close(clientSocket);
    printf("Fin de la connexion avec le client.\n");

    // Supprimer le thread
    pthread_exit(NULL);
}