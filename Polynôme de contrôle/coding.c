/**
 * @file coding.c
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
 * Generate code words from the initial data flow
 */

#include "codingdecoding.h"
#include <stdlib.h>
#include <stdio.h>

void copyDataBitsCoding(char *message, CodeWord_t *cw, int size)
{
  int i = 0;
  for(i=0; i<size; i++)
    {
      setNthBitCW(&(cw[i]), 1, getNthBit(message[i], 1));
      setNthBitCW(&(cw[i]), 2, getNthBit(message[i], 2));
      setNthBitCW(&(cw[i]), 3, getNthBit(message[i], 3));
      setNthBitCW(&(cw[i]), 4, getNthBit(message[i], 4));
      setNthBitCW(&(cw[i]), 5, getNthBit(message[i], 5));
      setNthBitCW(&(cw[i]), 6, getNthBit(message[i], 6));
      setNthBitCW(&(cw[i]), 7, getNthBit(message[i], 7));
      setNthBitCW(&(cw[i]), 8, getNthBit(message[i], 8));
    }

  return;
}


// Bits de contrôle

void computeCtrlBits(CodeWord_t *message, int size)
{

  int j,k,l;
  CodeWord_t generateur;
  CodeWord_t reste;

  // On initialise le reste et le générateur à 0 pour éviter tout problème
  for (int i=0;i<16;i++){
    setNthBitCW(&reste,i+1,0);
    setNthBitCW(&generateur,i+1,0);
  }

  // On remplace les bits du generateur par le polynôme du TP
  setNthBitCW(&generateur,1,1);
  setNthBitCW(&generateur,3,1);
  setNthBitCW(&generateur,4,1);
  setNthBitCW(&generateur,5,1);
  setNthBitCW(&generateur,9,1);

  for (int i = 0; i<size/2; i++)
  {

    // On décale le message à gauche et on copie le message dans le reste
    // afin de faire la division par le polynôme
    for (j = 0; j<8; j++){
      setNthBitCW(&(message[i]),16-j,getNthBit(message[i],8-j));
      setNthBitCW(&reste,16-j,getNthBit(message[i],16-j));
      setNthBitCW(&(message[i]),8-j,0);
      setNthBitCW(&reste,8-j,0);
    }

      for (j = 16;j>8;j--){

        // Tant que le bit du message est 0, on le skip
        while (getNthBit(reste,j)!=1){
          j--;
        }

        // On fait un XOR bit à bit entre le reste et le polynôme jusqu'à
        // ce qu'on atteigne un reste de longueur inférieure à 9 bits
        if (j>=9){
          for (k=j,l=9; k>j-9; k--,l--){
            if (getNthBit(reste,k)==1 && getNthBit(generateur,l)==1){
                setNthBitCW(&reste,k,0);
              }
              else if (getNthBit(reste,k)==0 && getNthBit(generateur,l)==0)  {
                setNthBitCW(&reste,k,0);
              }
              else setNthBitCW(&reste,k,1);
          }
        }
      }

      // On copie le reste à droite du message
      for (j = 0; j<8; j++){
        setNthBitCW(&(message[i]),j+1,getNthBit(reste,j+1));
      }

      // Affichage des 3 chaînes pour être sûr
      printBits(message[i], "Message");
      printBits(generateur, "Generateur");
      printBits(reste, "Reste");
      printf("\n");
    }
  return;
}


void coding(char *message, int data_size, char *cw, int *cw_size)
{
  *cw_size = data_size * sizeof(CodeWord_t);

  copyDataBitsCoding(message, (CodeWord_t*)cw, data_size);
  //-- to uncomment when complete and needed
  computeCtrlBits((CodeWord_t*)cw, *cw_size);

  return;
}
