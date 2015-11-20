#ifndef PROJET_MORPION_OUTILS_MESSAGES_H
#define PROJET_MORPION_OUTILS_MESSAGES_H

// Lire un message
char* lireMessage(int socket);

// Envoyer un message
void envoyerMessage(int socket, char *message);

#endif //PROJET_MORPION_OUTILS_MESSAGES_H
