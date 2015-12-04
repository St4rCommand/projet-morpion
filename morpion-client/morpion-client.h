#ifndef PROJET_MORPION_MORPION_CLIENT_H
#define PROJET_MORPION_MORPION_CLIENT_H

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;


void gestionnairePartieClient(int clientSocket);

char* gestionnaireMessageClient(char *messageRecu);

char* gestionnaireAccueil(char **messageRecu);

char* gestionnaireJouer();

#endif //PROJET_MORPION_MORPION_CLIENT_H
