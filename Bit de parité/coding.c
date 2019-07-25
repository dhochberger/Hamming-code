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

// Bit de parité

void computeCtrlBits(CodeWord_t *message, int size)
{
  int pair = 0;
  for (int i = 0; i<size/2; i++)
  {
    pair = 0; // On réinitialise pair à 0 pour le mot suivant

    for (int j = 0; j<8; j++){  // On parcourt les 8 bits de message
      if (getNthBit(message[i], j+1) == 1) // Si le bit est à 1, on ajoute 1 à la variable pair
        pair++;
    }

    if (pair%2 == 0){
      setNthBitCW(&(message[i]), 9, 0); // Si pair, alors bit de parité à 0
    }
    else setNthBitCW(&(message[i]), 9, 1);  // Sinon bit de parité à 1

    printBits(message[i], "Mot avec bit de parité "); // On vérifie que le bit soit bien présent
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
