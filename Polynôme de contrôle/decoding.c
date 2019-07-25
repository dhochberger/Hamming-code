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

// Bits de contrôle

int thereIsError(CodeWord_t *cw, int data_size)
{
  CodeWord_t reste;
  int k,l;
  int error=0;
  CodeWord_t generateur;

  // On initialise le reste et le générateur à 0 pour éviter tout problème
  for (int i=0;i<16;i++){
    setNthBitCW(&reste,i+1,0);
    setNthBitCW(&generateur,i+1,0);
  }

  // On initialise le générateur avec le polynôme donné dans le TP
  setNthBitCW(&generateur,1,1);
  setNthBitCW(&generateur,3,1);
  setNthBitCW(&generateur,4,1);
  setNthBitCW(&generateur,5,1);
  setNthBitCW(&generateur,9,1);

  for (int i=0; i<data_size; i++){
    k=0;
    l=0;

    // On copie le message dans le reste pour faciliter l'opération
    for (int j = 0;j<16;j++){
        setNthBitCW(&reste,16-j,getNthBit(cw[i],16-j));
    }

    for (int j = 16;j>8;j--){

    // Tant que le bit est un 0 on skip
      while (getNthBit(reste,j)==0){
        j--;
      }
      // XOR entre le reste et le polynôme jusqu'à arriver à un reste de taille
      // inférieure à 9 bits
      for (k=j, l=9;k>j-9;k--,l--){
        if (getNthBit(reste,k)==1 && getNthBit(generateur,l)==1){
            setNthBitCW(&reste,k,0);
          }
          else if (getNthBit(reste,k)==0 && getNthBit(generateur,l)==0)  {
            setNthBitCW(&reste,k,0);
          }
          else setNthBitCW(&reste,k,1);
      }
    }

    // Si le reste n'est pas nul, il y a une erreur, on affiche la lettre
    for (int j = 0;j<16;j++){
      if (getNthBit(reste,j+1)==1){
        printBits(cw[i],"Erreur in");
        error=1;
        break;
        }
    }

    // On redécale le mot à droite et on réinitialise les bits de contrôle à gauche
    for (int j=0;j<8;j++){
      setNthBitCW(&(cw[i]),8-j,getNthBit(cw[i],16-j));
      setNthBitCW(&(cw[i]),16-j,0);
    }

    // On vérifie la lettre affichée ainsi que le reste (not needed)
    /*printBits(cw[i], "Mot");
    printBits(generateur, "Generateur");
    printBits(reste, "Reste");
    printf("\n");
    */
  }

  // On affiche le mot envoyé (not needed)
  for (int i=0; i<data_size; i++){
      printf("%c",cw[i]);
  }
  printf("\n");

  return error;
}

void decoding(char *cw, int cw_size, char *message, int *data_size)
{
  *data_size = cw_size / sizeof(CodeWord_t);
  //printf("datasize : %d cwsize :%d\n",*data_size, cw_size);

  //-- For decoding
  copyDataBitsDecoding((CodeWord_t*)cw, message, *data_size);

  //-- For error correction
  //-- to uncomment when complete and needed
  //errorCorrection((CodeWord_t*)cw, *data_size);

  //-- For error detection
  //-- to uncomment when complete and needed
  if(thereIsError((CodeWord_t*)cw, *data_size))
  {
    printf("PARITY ERROR: \"%s\"\n", message);
  }

  return;
}
