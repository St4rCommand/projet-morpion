#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "morpion-serveur.h"
#include "gestionnaire-partie.h"

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

    //Initialisation du thread
    pthread_t threadClient;


    // Tableau des deux joueurs de la partie
    //int clientSockets[2];
    //clientSockets[0] = clientSocket;
    //clientSockets[1] = clientSocket;

    /* === LANCEMENT DU SERVEUR === */
    // Mise à l'écoute du serveur
    listen(serveurSocket, 5);
    int i = 0;
    struct donnees_partie parties[10];
    while (1) {

        printf("Attente de la connexion d'un client.\n");

        // Attente de connexion d'un client
        //clientSockets[0] = accept(serveurSocket, (sockaddr*) &clientAdresse, (socklen_t *) &clientAdresseTaille);
        //printf("Joueur 1.\n");
        //clientSockets[1] = accept(serveurSocket, (sockaddr*) &clientAdresse, (socklen_t *) &clientAdresseTaille);
        //printf("Joueur 2.\n");

        parties[i].joueur_1 = accept(serveurSocket, (sockaddr*) &clientAdresse, (socklen_t *) &clientAdresseTaille);
        parties[i].joueur_2 = accept(serveurSocket, (sockaddr*) &clientAdresse, (socklen_t *) &clientAdresseTaille);

        if (parties[i].joueur_1 < 0 && parties[i].joueur_2 < 0) {

            perror("Impossible d'accepter la connexion avec les clients.");
            exit(1);
        } else {

            // Création du thread pour le nouveau client
            if(pthread_create(&threadClient, NULL, gestionnairePartie, &parties[i]) == -1) {
                perror("Impossible de créer un thread pour le client.");
            }

            printf("Connexion avec un client établie.\n");

            i++;
        }
    }

//    exit(0);
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
