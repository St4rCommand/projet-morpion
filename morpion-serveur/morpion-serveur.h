#ifndef PROJET_MORPION_MORPION_SERVEUR_H
#define PROJET_MORPION_MORPION_SERVEUR_H

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

//void endHandler(int signal);



// Initialisation de l'adresse serveur
void initServeurAdresse(sockaddr_in *serveurAdresse, hostent *serveurInformations);

// Initialisation de l'adresse client
void initClientAdresse(sockaddr_in *clientAdresse);

#endif //PROJET_MORPION_MORPION_SERVEUR_H
