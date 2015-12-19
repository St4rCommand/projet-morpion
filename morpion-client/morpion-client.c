#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "morpion-client.h"
#include "./../morpion-outils/outils-messages.h"
#include "../morpion-serveur/morpion-moteur.h"

#define SERVEUR_IP "127.0.0.1"

int main(int argc, char **argv)
{
    printf("Lancement du client.\n");




    /* === INITIALISATION DU CLIENT === */
    // Initialisation du nom
    char message[TAILLE_BUFFER] = "";

    // Initialisation du socket client
    int clientSocket;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Impossible de créer la socket du client.");
        exit(1);
    }




    /* === INITIALISATION DU SERVEUR === */
    // Adresse du serveur
    char* serveurIP;
    if(argc >= 2) {
        serveurIP = argv[1];
    } else {
        serveurIP = SERVEUR_IP;
    }
    printf("Connexion à %s.\n", serveurIP);

    // Récupération des informations du serveur
    hostent *serveurInformations;
    serveurInformations = gethostbyname(serveurIP);
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

    // Saisie du nom
    saisirMessage(message, "Pseudonyme : ");
    // Envoi du message
    envoyerMessage(clientSocket, message);
    // Gestion de la partie
    gestionnairePartieClient(clientSocket);


    /* === FIN DE LA CONNEXION AVEC LE SERVEUR === */
    // Fermeture du socket
    close(clientSocket);
    printf("Fin de la connexion avec le serveur.\n");

    exit(0);
}

// Gestion d'une partie de Morpion
void gestionnairePartieClient(int clientSocket) {

    char *messageRecu;
    char *messageEnvoye;

    while (1) {

        // Lire le message
        messageRecu = lireMessage(clientSocket);

        // Si le message reçu est vide, quitter
        if((int) strlen(messageRecu) <= 0) {
            perror("connexion avec le serveur interrompue.\n");
            break;
        // Si on reçoit l'ordre de fin, quitter
        } else if (comparerCDC(messageRecu, "QUIT_")) {
            break;
        }

        // Gestion du message reçu
        messageEnvoye = gestionnaireMessageClient(messageRecu);

        // Envoyer la réponse au serveur
        envoyerMessage(clientSocket, messageEnvoye);
    }
}

// Gestion du message reçu
char* gestionnaireMessageClient(char *messageRecu) {

    // Traitement du message reçu
    char **messageRecuTraite = analyserMessage(messageRecu);

    // Analyse de la requete
    int messageRecuCode = analyserRequete(messageRecuTraite[0]);
    switch (messageRecuCode) {
        case MESSAGE_RECU_GAME:
            return gestionnaireAccueil(messageRecuTraite);
        case MESSAGE_RECU_PLAY:
            return gestionnaireJouer();
        case MESSAGE_RECU_BOARD:
            gestionnairePlateau(messageRecuTraite);
            return "OK_";
        case MESSAGE_RECU_PLAYERROR:
            return gestionnaireReJouer();
        case MESSAGE_RECU_WIN:
            gestionnaireVictoire();
            return "OK_";
        case MESSAGE_RECU_LOOSE:
            gestionnaireDefaite();
            return "OK_";
        case MESSAGE_RECU_QUIT:
            return "OK_";
        default:
            return "";
    }
}

// Accueil de la partie
char* gestionnaireAccueil(char **messageRecu) {
    printf("Bienvenue %s et %s dans une nouvelle partie de morpion.\n", messageRecu[1], messageRecu[2]);

    return "OK_";
}

// Affichage du plateau
void gestionnairePlateau(char **messageRecu) {
    int i=0;

    for(i=1;i<=9;i++) {

        switch(messageRecu[i][0]) {
            case VIDE:
                printf("_ ");
                break;
            case CROIX:
                printf("X ");
                break;
            case ROND:
                printf("O ");
                break;
        }

        if(i%3 == 0) {
            printf("\n");
        }
    }

}

// Gestion d'une erreur de saisie
char* gestionnaireReJouer() {
    printf("Position incorrecte, veuillez ressaisir la position ! \n");

    return gestionnaireJouer();
}

// Saisie d'une position
char* gestionnaireJouer() {
    int ligne, colonne;

    printf("Ligne : ");
    scanf("%d",&ligne);

    printf("Colonne : ");
    scanf("%d", &colonne);

    char *message = malloc(TAILLE_BUFFER * sizeof(char));
    strcat(message, "LOCATION_");
    strcat(message, (const char *) &ligne);
    strcat(message, "_");
    strcat(message, (const char *) &colonne);

    return message;
}

// Affichage victoire
void gestionnaireVictoire() {
    printf("Vous avez gagné !!!\n");
}

// Affichage défaite
void gestionnaireDefaite() {
    printf("Vous avez perdu ...\n");
}
