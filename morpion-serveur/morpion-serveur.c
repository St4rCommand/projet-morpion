#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "morpion-serveur.h"

#define TAILLE_MAX_NOM_MACHINE_LOCALE 256

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

// Ecouter le réseau
// A chaque nouvelle connexion, il faut créer un thread
//

int main(int argc, char **argv)
{
    printf("Lancement du serveur\n");

    int descripteurSocket;
    int descripteurNouveauSocket;
    int longueurAdresseCourante;

    sockaddr_in adresseLocale;
    sockaddr_in adresseClientCourant;

    hostent* informationsHote;
//    servent* informationsService;

    pthread_t* clients = malloc(sizeof(pthread_t));
    int numClient = 0;
    int resultatCreationThread;

    // Récupérer le nom de la machine
    char nomMachineLocale[TAILLE_MAX_NOM_MACHINE_LOCALE+1]; //
    gethostname(nomMachineLocale, TAILLE_MAX_NOM_MACHINE_LOCALE);
    printf("nom de la machine : %s\n", nomMachineLocale);

    // Récupérer les informations sur le serveur
    informationsHote = gethostbyname(nomMachineLocale);
    if (informationsHote == NULL) {
        perror("Impossible de trouver le serveur à partir de son nom.");
        exit(1);
    }

    // Informations sur la machine locale
    bcopy(informationsHote->h_addr,
          (char*)&adresseLocale.sin_addr,
          (size_t) informationsHote->h_length);
    adresseLocale.sin_family = AF_INET;
    adresseLocale.sin_addr.s_addr = INADDR_ANY;
    adresseLocale.sin_port = htons(5000);

    printf("Port pour la connexion serveur : %d\n", adresseLocale.sin_port);

    // Création de la socket
    descripteurSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (descripteurSocket < 0) {
        perror("Impossible de créer la socket de connexion avec le client.");
        exit(1);
    }

    // Association descripteurSocket/adresseLocale
    int i = bind(descripteurSocket, (sockaddr*)(&adresseLocale), sizeof(adresseLocale));
    if (i < 0) {
        perror("Impossible de lier la socket à l'adresse de connexion.");
        exit(1);
    }

    // Initialisation de la file d'écoute
    listen(descripteurSocket, 5);

    // Traitement des données reçues
    for (;;) {
        longueurAdresseCourante = sizeof(adresseClientCourant);

        // Gestion de la connexion des clients
        descripteurNouveauSocket = accept(
                descripteurSocket,
                (sockaddr*)&adresseClientCourant,
                (socklen_t*)&longueurAdresseCourante);
        if (descripteurNouveauSocket < 0) {
            perror("Impossible d'accepter la connexion avec le client.");
            exit(1);
        }

        printf("Message reçu\n");

        // Création du thread pour le client
        resultatCreationThread = pthread_create(&clients[numClient], NULL, gererClient, NULL);
        if(resultatCreationThread == -1) {
            perror("Le thread n'a pas pu être créé");
        }

        /*if (pthread_join(clients[numClient], NULL)) {
            perror("Impossible de lier le thread créé");
            exit(1);
        }*/

        numClient++;
        clients = realloc(clients, (numClient + 1) * sizeof(pthread_t));



        // Fermeture du socket
        //close(descripteurNouveauSocket);
    }
}

void *gererClient(void *arg) {
    printf("Client connecté\n");

    (void) arg;

    sleep(100);

    printf("Fin du traitement\n");
    pthread_exit(NULL);
}
