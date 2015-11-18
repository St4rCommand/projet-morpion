#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

int main(int argc, char **argv)
{
    printf("Lancement du client\n");

    int descripteurSocket;
    ssize_t longueurBuffer;

    sockaddr_in adresseLocale;

    hostent* informationsHote;
//    servent* informationsService;

    char buffer[256];
    char* nomProgramme;
    char* nomMachineDistante;
    char* message;

    // Vérification de la présence des arguments
    if (argc != 3) {
        perror("Saisir les deux paramètres.");
        exit(1);
    }

    // Récupération des arguments
    nomProgramme = argv[0];
    nomMachineDistante = argv[1];
    message = argv[2];

    printf("Nom du programme : %s\n",nomProgramme);
    printf("Nom de la machine distante : %s\n", nomMachineDistante);
    printf("Message à envoyer : %s\n", message);

    // Récupération du nom de la machine
    informationsHote = gethostbyname(nomMachineDistante);
    if (informationsHote == NULL) {
        perror("Impossible de trouver le serveur à partir de son adresse.");
        exit(1);
    }

    // Copie des données de informationsHote vers adresseLocale
    bcopy(informationsHote->h_addr,
          (char*)&adresseLocale.sin_addr,
          (size_t) informationsHote->h_length);
    adresseLocale.sin_family = AF_INET;

    // Création du nouveau numéro de port
    adresseLocale.sin_port = htons(5000);

    printf("Port utilisé : %d\n", adresseLocale.sin_port);

    // Création de la socket
    descripteurSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (descripteurSocket < 0) {
        perror("Impossible de créer la socket de connexion avec le serveur.");
        exit(1);
    }

    printf("Socket créé\n");

    // Tentative de connexion au serveur avec les infos de adresseLocale
    int resultatConnexion = connect(descripteurSocket, (sockaddr*)(&adresseLocale), sizeof(adresseLocale));
    if (resultatConnexion < 0) {
        perror("Impossible de se connecter au serveur.");
        exit(1);
    }

    printf("Connexion établie avec le serveur\n");

    // Envoi d'un message au serveur
    ssize_t resultatEnvoieMessage = write(descripteurSocket, message, strlen(message));
    if (resultatEnvoieMessage < 0) {
        perror("Impossible d'écrire le message au serveur");
        exit(1);
    }

    // Lecture de la réponse en provenance du serveur
    while ((longueurBuffer = read(descripteurSocket, buffer, sizeof(buffer))) >0 ) {
        printf("Réponse du serveur : \n");
        write(1, buffer, (size_t) longueurBuffer);
    }

    printf("Fin de la récéption\n");

    // Fermeture du socket
    close(descripteurSocket);

    exit(0);
}
