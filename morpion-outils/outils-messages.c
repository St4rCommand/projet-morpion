#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "outils-messages.h"

// Lire un message reçu
char* lireMessage(int socket) {

    char* message = malloc(TAILLE_BUFFER * sizeof(char));

    // TODO faire une vérification
    message = realloc(message, TAILLE_BUFFER * sizeof(char));

    ssize_t longueur = read(socket, message, TAILLE_BUFFER * sizeof(char));

    if (longueur < 0 ) {
        perror("Connexion interrompue");
        return "";
    }

    /*if (longueur = 0 ) {
        perror("Le message reçu est vide.");
        exit(1);
    }*/

    printf("Reçu de %d : %s\n", socket, message);

    return message;
}

// Envoyer un message
void envoyerMessage(int socket, char *message) {
    ssize_t longueurMessageEnvoye;

    longueurMessageEnvoye = write(socket, message, strlen(message)+2);
    if (longueurMessageEnvoye <= 0) {
        perror("Le message n'a pas pu etre envoyé.");
    }

    printf("Envoyé à %d : %s\n", socket, message);
}

// Saisir un message
void saisirMessage(char* messageSaisi, char *messageAffiche) {
    printf("%s", messageAffiche);
    scanf("%s", messageSaisi);
    printf("\n");
}

// Analyser le code du message
int analyserRequete(char *messageRecu) {

    if (comparerCDC(messageRecu, "GAME")) {
        return MESSAGE_RECU_GAME;
    } else if (comparerCDC(messageRecu, "PLAY")) {
        return MESSAGE_RECU_PLAY;
    } else if (comparerCDC(messageRecu, "BOARD")) {
        return MESSAGE_RECU_BOARD;
    } else if (comparerCDC(messageRecu, "PLAYERROR")) {
        return MESSAGE_RECU_PLAYERROR;
    } else if (comparerCDC(messageRecu, "WIN")) {
        return MESSAGE_RECU_WIN;
    } else if (comparerCDC(messageRecu, "LOOSE")) {
        return MESSAGE_RECU_LOOSE;
    } else if (comparerCDC(messageRecu, "QUIT")) {
        return MESSAGE_RECU_QUIT;
    } else if (comparerCDC(messageRecu, "LOCATION")) {
        return MESSAGE_RECU_LOCATION;
    } else if (comparerCDC(messageRecu, "OK")) {
        return MESSAGE_RECU_OK;
    }

    return -1;
}

// Traite le message reçu pour placer chaque paramètre dans un tableau
char** analyserMessage(char *messageRecu) {
    int i = 0;
    int j = 0;
    int k = 1;

    char **messageTraite;
    messageTraite = malloc(sizeof(char*));
    messageTraite[j] = malloc(0);

    while (messageRecu[i] != '\0') {


        if (messageRecu[i] == '_') {
            j++;
            k=0;

            messageTraite = realloc(messageTraite, (j+1) * sizeof(char*));
            messageTraite[j] = malloc(0);
        } else {
            messageTraite[j] = realloc(messageTraite[j], k*sizeof(char));
            messageTraite[j][k-1] = messageRecu[i];
        }

        i++;
        k++;
    }

    return messageTraite;
}

// Comparer deux chaines de caractères
int comparerCDC(char *chaine1, char *chaine2) {
    int vrai = 1;

    if (strlen(chaine1) != strlen(chaine2)) {
        vrai = 0;
    }

    while(*chaine1 != '\0' || *chaine2 != '\0')
    {

        if(*chaine1 != *chaine2)
        {
            vrai = 0;
        }
        chaine1++;
        chaine2++;
    }
    return vrai;
}
