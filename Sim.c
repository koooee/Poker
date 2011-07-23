/* Sim.c
The purpose of this file is to run the game simulations -- Think Main()
Programmer: Nick Kolegraff
Date: 5/29/2011
*/

#include "Player.c"

CARD tstoc(char r, char s)
{/* string to card */
  int rank, suit,i;
  for(i = 0; i < MAX_NUM_RANKS; i++)
    {
      if(r == ranks[i])
	{
	  rank = i;
	}
    }
  for(i = 0; i < MAX_NUM_SUITS; i++)
    {
      if(s == suits[i])
	{
	  suit = i;
	}
    }
  CARD c;
  c.rank = rank;
  c.suit = suit;
  c.whos_card = NO_ONES;
  return c;
  
}

CARD *tconvert(char *s, int num_cards)
{/* input is a string of cards: AsTcJh8c2d4s */
  CARD *hand = (CARD *)malloc(num_cards * sizeof(CARD));
  int i;
  for(i = 0; i < 2*num_cards; i += 2)
    {
      hand[i / 2] = tstoc(s[i], s[i+1]);
    }
  return hand;
}

void big_test_seven()
{
  int one, two, three, four, five, six, seven, count, sumer;
  int freqs[9];
  int sums[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  int z;
  for(z = 0; z < 9; z++)
    freqs[z] = 0;
  count = 0;
  int size = 52, i, rank;
  char *cards[52];
  char *test_hand;
  CARD *hnd;
  PLAYER plyr;
  
  init_bin(&plyr.bin);
  test_hand = (char *)malloc(14 * sizeof(char));
  for(one = 0; one < size; one++)
    for(two = one+1; two < size; two++)
      for(three = two+1; three < size; three++)
      	for(four = three+1; four < size; four++)
      	  for(five = four+1; five < size; five++)
      	    for(six = five+1; six < size; six++)
      	      for(seven = six+1; seven < size; seven++)
		{
		      sprintf(test_hand, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c"
		      	      ,ranks[one % 13]
		      	      ,suits[one % 4]
		      	      ,ranks[two % 13]
		      	      ,suits[two % 4]
		      	      ,ranks[three % 13]
		      	      ,suits[three % 4]
		      	      ,ranks[four % 13]
		      	      ,suits[four % 4]
		      	      ,ranks[five % 13]
		      	      ,suits[five % 4]
		      	      ,ranks[six % 13]
		      	      ,suits[six % 4]
		      	      ,ranks[seven % 13]
		      	      ,suits[seven % 4]);

		      hnd = tconvert(test_hand, 7);
		      /* sort_hand(hnd, 7); */
		      /* printh(hnd, 7); */
		      for(i = 0; i < 7; i++){
			plyr.hand[i] = hnd[i];
		      }
		      free(hnd);
		      /* Rank it. Count it. */
		      rank = rank_hand(plyr.hand, &plyr.bin, 7);
		      freqs[rank]++;

		      /* Debugging */
		      if(rank == 5)
			{
			  printb(&plyr.bin);
			}
		      /* End Debugging */

		      /* Get ready for a new one */
		      reset_bin(&plyr.bin);

		}
  long total = 0;
  for(z = 0; z < 9; z++)
    {
      printf("Freq %d %d\n", z, freqs[z]);
      total += freqs[z];
    
    }
  printf("Total Number of Hands: %d\n", total);
}

int main(int argc, char *argv[])
{
  PLAYER p;
  int i, seed;
  CARD *h;
  /* Debugging -- Delete Section when done */
  /* Debugging Init the hand as a straight */
  big_test_seven();
}
