#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "outils-messages.h"
#define TAILLE_BUFFER 256

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

    printf("Message reçu : %s\n", message);

    return message;
}

void envoyerMessage(int socket, char *message) {
    ssize_t longueurMessageEnvoye;

    longueurMessageEnvoye = write(socket, message, strlen(message)+2);
    if (longueurMessageEnvoye <= 0) {
        perror("Le message n'a pas pu etre envoyé.");
    }

    printf("Message envoyé : %s\n", message);
}

void saisirMessage(char* messageSaisi) {
    printf("Message à saisir : ");
    scanf("%s", messageSaisi);
    printf("\n");
}