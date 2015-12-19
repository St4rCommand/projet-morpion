#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "morpion-moteur.h"
#include "./../morpion-outils/outils-messages.h"

// Créer une grille pour une partie
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

// Renvoie la grille sous forme de chaine de caractère prête à être envoyée
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


// Placer un pion sur la grille en fonction des paramètres
// Gestion du cas d'une saisie incorrecte (en dehors de la grille ou case déjà prise)
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

// Test de fin de jeu
int testeFinJeu(int **grille) {
  int i,j;
  int estFini = FALSE;

  if (grille[1][1] != VIDE) {
    if (/* colonne 1 */ ((grille[0][1] == grille[1][1]) && (grille[1][1] == grille[2][1])) ||
	/* ligne 1 */ ((grille[1][0] == grille[1][1]) && (grille[1][1] == grille[1][2])) ||
	/* diagonale */ ((grille[0][0] == grille[1][1]) && (grille[1][1] == grille[2][2])) ||
	/* autre diag */ ((grille[0][2] == grille[1][1]) && (grille[1][1] == grille[2][0]))) {
      estFini = TRUE;
    }
  }

  if ((!estFini) && (grille[0][0] != VIDE)) {
    if ( /* ligne 0 */ ((grille[0][0] == grille[0][1]) && (grille[0][1] == grille[0][2])) ||
	 /* colonne 0*/ ((grille[0][0] == grille[1][0]) && (grille[1][0] == grille[2][0]))) {
      estFini = TRUE;
    }
  }

  if ((!estFini) && (grille[2][2] != VIDE)) {
    if ( /* ligne 2 */ ((grille[2][0] == grille[2][1]) && (grille[2][1] == grille[2][2])) ||
	 /* colonne 2 */ ((grille[0][2] == grille[1][2]) && (grille[1][2] == grille[2][2]))) {
      estFini = TRUE;
    }
  }

  if (estFini) {
    return TRUE;
  }

  for (i=0; i<NB_LIG; i++) {
    for (j=0; j<NB_COL; j++) {
      if (grille[i][j] == VIDE)
	return FALSE;
    }
  }
  return TRUE;

}

