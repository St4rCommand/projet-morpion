/*********************************************************************
  Programme de  morpion. Le jeu s'arrete qd l'un des joueurs a gagne
  (ie 3 pions semblables sur une ligne, colonne ou diagonale) ou qd
  la grille est pleine.

**********************************************************************/
#include <stdio.h>
#include "gestionnaire-partie.h"

//typedef enum {FALSE, TRUE} Boolean;
//static ValeurGrille grille[NB_LIG][NB_COL]; /* grille du morpion valeurs possibles VIDE, ROND ou CROIX */

#define TRUE 1
#define FALSE 0

#define VIDE 0
#define CROIX 1
#define ROND 2

/* indique quel sera le prochain joueur a mettre un pion dans la grille ie soit ROND soit CROIX */
static int prochainJoueur = ROND;


/*
 * Initiliase la grille du morpion a vide
 */
void initialiseGrille(int grille[NB_LIG][NB_COL]) {
  int i, j;
  for (i=0; i<NB_LIG; i++) {
    for (j=0; j<NB_COL; j++) {
      grille[i][j] = VIDE;
    }
  }
}

/*
  Affiche la grille du morpion
  _ indique case vide, O pion joueur 1 et X pion jour 2
 */
/*void afficheGrille(int grille[NB_LIG][NB_COL]) {
  int i, j;
  for (i=0; i<NB_LIG; i++) {
    for (j=0; j<NB_COL; j++) {
      switch (grille[i][j]) {
      case VIDE:
	printf("_ ");
	break;
      case ROND:
	printf("O ");
	break;
      case CROIX:
	printf("X ");
	break;
      }
    }
    printf("\n"); /* fin de la ligne */
  /*}
}*/

/*
  Saisie les coordonnees du nouveau pion a mettre sur la grille
  Si les coordonnees sont en dehors de la grille ou si la case possede
  deja un pion, la saisie est refusee, un message d'erreur est affichee,
  et le joueur doit rejoue
 */
int metUnPionSurLaGrille(int ligne, int col, int grille[NB_LIG][NB_COL], int joueurCourant) {
  int saisieCorrecte = FALSE;

//  printf("Numeros de ligne et de colonne: ");

//  do {
//    scanf("%d %d", &ligne, &col);
//    printf("\n");

  if ((ligne > 0) && (ligne <= NB_LIG) && (col > 0) && (col <= NB_COL)) {
    ligne--;
    col--;

    if (grille[ligne][col] != VIDE) {
      return saisieCorrecte;
    } else {
      saisieCorrecte = TRUE;
      grille[ligne][col] = joueurCourant;
    }
  } else {
    return saisieCorrecte;
  }
//  } while (!saisieCorrecte);

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
int testeFinJeu(int grille[NB_LIG][NB_COL]) {
  int i,j;
  int joueurGagnant; /* pour connaitre quel est le gagnant ie soit CROIX soit ROND */
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
      joueurGagnant = grille[1][1]; /* ie ROND ou CROIX */
      estFini = TRUE;
    }
  }

  /* si la case 0,0 est vide, cela signifie que la ligne 0 et le colonne 0 ne sont pas gagnantes */
  if ((!estFini) && (grille[0][0] != VIDE)) {
    if ( /* ligne 0 */ ((grille[0][0] == grille[0][1]) && (grille[0][1] == grille[0][2])) ||
	 /* colonne 0*/ ((grille[0][0] == grille[1][0]) && (grille[1][0] == grille[2][0]))) {
      joueurGagnant = grille[0][0];
      estFini = TRUE;
    }
  }

  /* si la case 2,2 est vide, cela signifie que la ligne 2 et la colonne 2 ne sont gagnantes */
  if ((!estFini) && (grille[2][2] != VIDE)) {
    if ( /* ligne 2 */ ((grille[2][0] == grille[2][1]) && (grille[2][1] == grille[2][2])) ||
	 /* colonne 2 */ ((grille[0][2] == grille[1][2]) && (grille[1][2] == grille[2][2]))) {
      joueurGagnant = grille[2][2];
      estFini = TRUE;
    }
  }

  if (estFini) {
    printf("Felicitations au joueur ayant les ");
    if (joueurGagnant == ROND)
      printf("ronds ");
    else
      printf("croix ");
    printf("qui a gagne.\n");
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





