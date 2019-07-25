/**
 * @file decoding.c
 * @author Arash Habibi
 * @author Julien Montavont
 * @version 2.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * Functions to decipher the code words
 */

#include <stdio.h>
#include "codingdecoding.h"
#include <math.h>

char H[4][12]=
{
  {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0},
  {0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0},
  {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1},
  {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1}
};

void copyDataBitsDecoding(CodeWord_t *cw, char *message, int data_size)
{
  int i = 0;

  for(i=0; i<data_size; i++)
    {
      setNthBitW(&(message[i]), 1, getNthBit(cw[i], 1));
      setNthBitW(&(message[i]), 2, getNthBit(cw[i], 2));
      setNthBitW(&(message[i]), 3, getNthBit(cw[i], 3));
      setNthBitW(&(message[i]), 4, getNthBit(cw[i], 4));
      setNthBitW(&(message[i]), 5, getNthBit(cw[i], 5));
      setNthBitW(&(message[i]), 6, getNthBit(cw[i], 6));
      setNthBitW(&(message[i]), 7, getNthBit(cw[i], 7));
      setNthBitW(&(message[i]), 8, getNthBit(cw[i], 8));
    }
}


void errorCorrection(CodeWord_t *cw, int data_size)
{
  int res,error,powErr;
  int p[4] = {0,0,0,0};

  for (int i=0; i<data_size; i++){

    //printBits(cw[i],"Received Message");
    // Réinitialise la matrice d'erreur, error et powErr à chaque nouveau mot
    error = 0;
    powErr = 0;
    for (int j = 0;j<4;j++){
      p[j]=0;
    }

    for (int j = 0;j<4;j++){
      res = 0;
      for (int k = 0;k<12;k++){
        // On cumule tous les bits de la ligne [j] de H avec le mot codé
        res += H[j][k]*getNthBit(cw[i],k+1);
      }

      // On teste la parité du résultat, si elle est impaire il y a une erreur
      if (res%2==1){
        // On note l'erreur dans la matrice d'erreur et on place Error à 1 pour la fin
        p[j] = 1;
        error = 1;
      }
    }

    // On print la matrice d'erreur pour vérification
    /*printf("(");
    for (int j=0;j<4;j++) printf("%d ",p[j]);
    printf(")\n");*/

    // Si il y a une erreur
    if (error == 1){
        for (int j=0;j<4;j++){
          // On parcourt le tableau d'erreur, on cumule la puissance de chaque valeur à 1
          // afin de connaître la position de l'erreur dans le message à décoder
          if (p[j]==1){
            powErr += pow(2,j);
          }
        }

      // On échange le bit erroné avec son opposé
      setNthBitCW(&(cw[i]),powErr,!getNthBit(cw[i],powErr));

      //On print la position de l'erreur et le mot après correction
      //printf("powError = %d\n",powErr);
      printBits(cw[i],"Parity checked and corrected");
      //printf("PARITY ERROR: CHECK  \"%d\"\n", cw[i]);
    }
    else printf("No error in \"%d\"\n", cw[i]); // Si pas d'erreur on le mentionne également

    // On décale le message 3x afin de supprimer les bits de parité du message
    for (int j=1;j<=10;j++){
      setNthBitCW(&(cw[i]),j,getNthBit(cw[i],j+2));
    }
    for (int j=2;j<=9;j++){
      setNthBitCW(&(cw[i]),j,getNthBit(cw[i],j+1));
    }
    for (int j=5;j<=8;j++){
      setNthBitCW(&(cw[i]),j,getNthBit(cw[i],j+1));
    }
    // Les 8 bits de gauche du mot sont mis à 0 pour éviter tout problème de lecture
    for (int j=9;j<16;j++){
      setNthBitCW(&(cw[i]),j,0);
    }

    // On print la lettre une fois corrigée
    /*printBits(cw[i],"Message Final");
    printf("\n");*/

  }

  // On print le mot entier final reçu une fois corrigé
  for (int i=0; i<data_size; i++){
    printf("%c",cw[i]);
  }
  printf("\n");

  return;
}

void decoding(char *cw, int cw_size, char *message, int *data_size)
{
  *data_size = cw_size / sizeof(CodeWord_t);
  //printf("datasize : %d cwsize :%d\n",*data_size, cw_size);

  //-- For decoding
  copyDataBitsDecoding((CodeWord_t*)cw, message, *data_size);

  //-- For error correction
  //-- to uncomment when complete and needed
  errorCorrection((CodeWord_t*)cw, *data_size);

  //-- For error detection
  //-- to uncomment when complete and needed
  /*if(thereIsError((CodeWord_t*)cw, *data_size))
  {
    printf("PARITY ERROR: \"%s\"\n", message);
  }*/

  return;
}
