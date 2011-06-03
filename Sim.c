/* Sim.c
The purpose of this file is to run the game simulations -- Think Main()
Programmer: Nick Kolegraff
Date: 5/29/2011
*/

#include "Player.c"

void Print_Card(CARD c, int trailing)
{
  printf("%c%c "
	 ,ranks[c.rank]
	 ,suits[c.suit]);
  if(trailing = TRUE)
    printf("\n");
}

int main(int argc, char *argv)
{

  PLAYER p;
  int i;
  /* Debugging -- Delete Section when done */
  /* initialize player hand as a striaght */
  Init_Bin(&p.bin);
  /* Debugging Init the hand as a straight */
  for(i=0; i < MAX_HAND_SIZE; i++)
    {
      CARD temp;
      temp.rank = i;
      temp.suit = i % 4;
      p.hand[i] = temp;
    }
  Rank_Hand(p.hand, &p.bin);


}

