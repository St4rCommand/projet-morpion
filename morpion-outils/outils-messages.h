#ifndef PROJET_MORPION_OUTILS_MESSAGES_H
#define PROJET_MORPION_OUTILS_MESSAGES_H

#define TAILLE_BUFFER 256
#define MESSAGE_RECU_GAME 1
#define MESSAGE_RECU_PLAY 2
#define MESSAGE_RECU_BOARD 3
#define MESSAGE_RECU_PLAYERROR 4
#define MESSAGE_RECU_WIN 5
#define MESSAGE_RECU_LOOSE 6
#define MESSAGE_RECU_QUIT 7
#define MESSAGE_RECU_LOCATION 8
#define MESSAGE_RECU_OK 9

char* lireMessage(int socket);

void envoyerMessage(int socket, char *message);

void saisirMessage(char* messageSaisi, char *messageAffiche);

int analyserRequete(char *messageRecu);

char** analyserMessage(char *messageRecu);

int comparerCDC(char *chaine1, char *chaine2);

#endif //PROJET_MORPION_OUTILS_MESSAGES_H
