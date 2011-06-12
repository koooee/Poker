/* Sim.c
The purpose of this file is to run the game simulations -- Think Main()
Programmer: Nick Kolegraff
Date: 5/29/2011
*/

#include "Player.c"
#include "Test.h"

void Print_Card(CARD c, int trailing)
{
  printf("%c%c "
	 ,ranks[c.rank]
	 ,suits[c.suit]);
  if(trailing = TRUE)
    printf("\n");
}

int main(int argc, char *argv[])
{
  PLAYER p;
  int i, seed;
  CARD *h;
  /* Debugging -- Delete Section when done */
  /* Debugging Init the hand as a straight */
    ttest();
}

