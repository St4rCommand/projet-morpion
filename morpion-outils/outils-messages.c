#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "outils-messages.h"
#define TAILLE_BUFFER 256

char* lireMessage(int socket) {
    char* message;
    message = malloc(TAILLE_BUFFER * sizeof(char));
    ssize_t longueur = read(socket, message, sizeof(message));

    if (longueur <= 0 ) {
        perror("Le message reçu est vide.");
        return "";
    }

    printf("Message reçu : %s\n", message);

    return message;
}

void envoyerMessage(int socket, char *message) {
    ssize_t longueurMessageEnvoye;

    longueurMessageEnvoye = write(socket, message, strlen(message)+1);
    if (longueurMessageEnvoye <= 0) {
        perror("Le message n'a pas pu etre envoyé.");
    }

    printf("Message envoyé : %s\n", message);
}