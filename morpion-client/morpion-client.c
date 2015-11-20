#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "morpion-client.h"
#include "./../morpion-outils/outils-messages.h"

#define SERVEUR_ADRESSE "127.0.0.1"

int main(int argc, char **argv)
{
    printf("Lancement du client.\n");




    /* === INITIALISATION DU CLIENT === */
    // Initialisation du socket client
    int clientSocket;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Impossible de créer la socket du client.");
        exit(1);
    }




    /* === INITIALISATION DU SERVEUR === */
    // Récupération des informations du serveur
    hostent *serveurInformations;
    serveurInformations = gethostbyname(SERVEUR_ADRESSE);
    if (serveurInformations == NULL) {
        perror("Impossible de trouver le serveur à partir de son adresse.");
        exit(1);
    }

    // Initialisation de l'adresse serveur
    sockaddr_in serveurAdresse;
    int serveurAdresseTaille = sizeof(serveurAdresse);
    bcopy(
            serveurInformations->h_addr,
            (char *) &serveurAdresse.sin_addr,
            (size_t) serveurInformations->h_length
    );
    serveurAdresse.sin_family = AF_INET;
    serveurAdresse.sin_port = htons(5000);
//    serveurAdresse.sin_addr.s_addr = (in_addr_t) *serveurInformations->h_addr;
//    serveurAdresse.sin_zero = memset(serveurAdresse.sin_zero, 0, 8);





    /* === LANCEMENT DU CLIENT === */
    // Connexion au serveur
    if (connect(clientSocket, (sockaddr*) &serveurAdresse, (socklen_t) serveurAdresseTaille) == -1) {
        perror("Echec de la connexion au serveur.");
        exit(1);
    } else {
        printf("Connexion au serveur réussie.\n");
    }




    /* === COMMUNICATION AVEC LE SERVEUR === */
    // TODO Problème avec les espaces
    // Envoi du message
    envoyerMessage(clientSocket, "Bonjour, je cherche à me connecter.");

    // Afficher la réponse reçue
    lireMessage(clientSocket);

    /* === FIN DE LA CONNEXION AVEC LE SERVEUR === */
    close(clientSocket);
    printf("Fin de la connexion avec le serveur.\n");

    exit(0);
}
