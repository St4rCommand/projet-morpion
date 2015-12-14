/*********************************************************************
  Programme de  morpion. Le jeu s'arrete qd l'un des joueurs a gagne
  (ie 3 pions semblables sur une ligne, colonne ou diagonale) ou qd
  la grille est pleine.

**********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "morpion-moteur.h"
#include "./../morpion-outils/outils-messages.h"

int ** initialiseGrille() {
    int i, j;
    int **grille;
    grille = malloc(NB_LIG*sizeof(int));

    for (i=0; i<NB_LIG; i++) {
        grille[i] = malloc(NB_COL * sizeof(int));
        for (j=0; j<NB_COL; j++) {
            grille[i][j] = VIDE;
        }
    }

    return grille;
}

char* grilleAEnvoyer(int **grille) {
    int i, j, x;
      x = 0;
    char* stringGrille = malloc(TAILLE_BUFFER*sizeof(char));
      strcat(stringGrille, "BOARD_");

      int rond = ROND;
      int croix = CROIX;
      int vide = VIDE;


    for (i=0; i<NB_LIG; i++) {
        for (j=0; j<NB_COL; j++) {

            switch (grille[i][j]) {
                case VIDE:
                    strcat(stringGrille, (const char *) &vide);
                    break;
                case CROIX:
                    strcat(stringGrille, (const char *) &croix);
                    break;
                case ROND:
                    strcat(stringGrille, (const char *) &rond);
                    break;
            }

            if(i+j != 4) {
                strcat(stringGrille, "_");
            }

            x++;
        }
    }

    return stringGrille;
}


/*
  Saisie les coordonnees du nouveau pion a mettre sur la grille
  Si les coordonnees sont en dehors de la grille ou si la case possede
  deja un pion, la saisie est refusee, un message d'erreur est affichee,
  et le joueur doit rejoue
 */
int metUnPionSurLaGrille(int ligne, int col, int **grille, int joueurCourant) {
    int saisieCorrecte = FALSE;

    int signeJoueurCourant;
    if(joueurCourant == 1) {
        signeJoueurCourant = CROIX;
    } else {
        signeJoueurCourant = ROND;
    }

    if ((ligne > 0) && (ligne <= NB_LIG) && (col > 0) && (col <= NB_COL)) {
        ligne--;
        col--;

        if (grille[ligne][col] != VIDE) {
            return saisieCorrecte;
        } else {
            saisieCorrecte = TRUE;
            grille[ligne][col] = signeJoueurCourant;
        }
    } else {
        return saisieCorrecte;
    }

    return saisieCorrecte;
}

/* Teste si l'un des joueurs a gagne (ligne, colonne ou diagonale remplit
   de pions semblables). Dans ce cas affiche un message pour indiquer le
   joueur qui a gagne.
   S'il n'y a pas de gagnant, teste que la grille n'est pas pleine. Si elle
   est pleine, affiche un message indiquant qu'aucun des joueurs a gagne
   Retourne TRUE si la grille est pleine ou si un joueur a gagne
            FALSE sinon
*/
int testeFinJeu(int **grille) {
  int i,j;
  int estFini = FALSE;

  /* Teste s'il y a un gagnant */
  /* L'algorithme utilise est le plus facile mais n'est pas le plus efficace
     car on n'utilise pas la position du dernier pion ajoute sur la grille. Cette information
     permettrait de reduire le temps de la recherche.
     De plus, cet algo suppose que la taille de la matrice est de 3 par 3
  */
  /* si la case 1,1 est VIDE, cela signifie que les diagonales, la ligne 1 et la colonne 1 ne sont
     pas gagnantes
  */
  if (grille[1][1] != VIDE) {
    if (/* colonne 1 */ ((grille[0][1] == grille[1][1]) && (grille[1][1] == grille[2][1])) ||
	/* ligne 1 */ ((grille[1][0] == grille[1][1]) && (grille[1][1] == grille[1][2])) ||
	/* diagonale */ ((grille[0][0] == grille[1][1]) && (grille[1][1] == grille[2][2])) ||
	/* autre diag */ ((grille[0][2] == grille[1][1]) && (grille[1][1] == grille[2][0]))) {
      estFini = TRUE;
    }
  }

  /* si la case 0,0 est vide, cela signifie que la ligne 0 et le colonne 0 ne sont pas gagnantes */
  if ((!estFini) && (grille[0][0] != VIDE)) {
    if ( /* ligne 0 */ ((grille[0][0] == grille[0][1]) && (grille[0][1] == grille[0][2])) ||
	 /* colonne 0*/ ((grille[0][0] == grille[1][0]) && (grille[1][0] == grille[2][0]))) {
      estFini = TRUE;
    }
  }

  /* si la case 2,2 est vide, cela signifie que la ligne 2 et la colonne 2 ne sont gagnantes */
  if ((!estFini) && (grille[2][2] != VIDE)) {
    if ( /* ligne 2 */ ((grille[2][0] == grille[2][1]) && (grille[2][1] == grille[2][2])) ||
	 /* colonne 2 */ ((grille[0][2] == grille[1][2]) && (grille[1][2] == grille[2][2]))) {
      estFini = TRUE;
    }
  }

  if (estFini) {
    return TRUE;
  }

  /* teste si la grille n'est pas pleine */
  for (i=0; i<NB_LIG; i++) {
    for (j=0; j<NB_COL; j++) {
      if (grille[i][j] == VIDE) /* Au moins une case est vide donc le jeu n'est pas fini */
	return FALSE;
    }
  }
  return TRUE;

}

/*
  Initialise la grille a vide puis tant que la grille n'est pas pleine ou
  qu'il n'y a pas un gagnant, saisie les pions des joueurs et affiche la grille
 */
/*int main() {
  initialiseGrille();
  do {
    metUnPionSurLaGrille();
    afficheGrille();
  }while(!testeFinJeu());
  return 1;
}*/





