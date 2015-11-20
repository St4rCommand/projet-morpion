#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "./../morpion-outils/outils-messages.h"
#include "morpion-serveur.h"

#define TAILLE_SERVEUR_MACHINE_NOM 256

int main(int argc, char **argv)
{
    // TODO à revoir pour vider la mémoire lors d'un Ctrl+C
//    if (signal(SIGINT, endHandler) == SIG_ERR) {printf("coucou");}

    printf("Lancement du serveur.\n");




    /* === INITIALISATION DU SERVEUR === */
    // Récupération du nom du serveur
    char serveurMachineNom[TAILLE_SERVEUR_MACHINE_NOM];
    gethostname(serveurMachineNom, TAILLE_SERVEUR_MACHINE_NOM);

    // Récupération des informations du serveur
    hostent *serveurInformations;
    serveurInformations = gethostbyname(serveurMachineNom);
    if (serveurInformations == NULL) {
        perror("Impossible de trouver le serveur à partir de son adresse.");
        exit(1);
    }

    // Initialisation du socket serveur
    int serveurSocket;
    serveurSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serveurSocket < 0) {
        perror("Impossible de créer la socket du serveur.");
        exit(1);
    }

    // Initialisation de l'adresse serveur
    sockaddr_in serveurAdresse;
    initServeurAdresse(&serveurAdresse, serveurInformations);

    // Ligature du socket et de l'adresse serveur
    bind(serveurSocket, (sockaddr*)&serveurAdresse, sizeof(serveurAdresse));

    printf("Serveur prêt.\n");




    /* === INITIALISATION DU CLIENT === */
    // Initialisation du socket client
    int clientSocket;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Impossible de créer la socket du client.");
        exit(1);
    }

    // Initialisation de l'adresse client
    sockaddr_in clientAdresse;
    int clientAdresseTaille = sizeof(clientAdresse);
    initClientAdresse(&clientAdresse);




    /* === LANCEMENT DU SERVEUR === */
    // Mise à l'écoute du serveur
    listen(serveurSocket, 5);

    // Attente de connexion d'un client
    clientSocket = accept(serveurSocket, (sockaddr*) &clientAdresse, (socklen_t *) &clientAdresseTaille);
    if (clientSocket < 0 ) {
        perror("Impossible d'accepter la connexion avec le client.");
        exit(1);
    }
    printf("Connexion avec le client établie.\n");


    /* === ECHANGE AVEC LE CLIENT === */
    // TODO Problème avec les espaces
//    char* messageRecu = "";
    char* messageEnvoye = "";
    while (1) {
        // Lire le message reçu
        lireMessage(clientSocket);

        // Saisie du message
        saisirMessage(messageEnvoye);

        // Envoyer une réponse
        envoyerMessage(clientSocket, messageEnvoye);

        if (strcmp(messageEnvoye, "EXIT")) {
            break;
        }
    }

    /* === FIN DE LA CONNEXION AVEC LE CLIENT === */
    close(clientSocket);
    printf("Fin de la connexion avec le client.\n");

    exit(0);
}

void initServeurAdresse(sockaddr_in *serveurAdresse, hostent *serveurInformations) {
    bcopy(
            serveurInformations->h_addr,
            (char *) &serveurAdresse->sin_addr,
            (size_t) serveurInformations->h_length
    );
    serveurAdresse->sin_family = AF_INET;
    serveurAdresse->sin_port = htons(5000);
    serveurAdresse->sin_addr.s_addr = INADDR_ANY;
//    serveurAdresse.sin_zero = memset(serveurAdresse.sin_zero, 0, 8);
}

void initClientAdresse(sockaddr_in *clientAdresse) {
    clientAdresse->sin_family = AF_INET;
    clientAdresse->sin_port = htons(5000);
    clientAdresse->sin_addr.s_addr = INADDR_ANY;
//    clientAdresse.sin_zero = memset(serveurAdresse.sin_zero, 0, 8);
}

/*void endHandler(int signal) {

    if (signal == SIGINT) {
        printf("Arrêt du serveur\n");

        exit(0);
    }
}*/
