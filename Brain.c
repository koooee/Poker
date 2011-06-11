/*
Brain.c
The purpose of Brain.c is to give the players intelligence/functionality.  Player.c will include a Brain.c which contains functions that will be able to answer common Player questions when analyzing their poker hand
such as... What is My current hand? What Am I drawing to? How many outs do I have? It will also contain various helperfunctions for
Programmer: Nick Kolegraff
Date: 5/18/2011
*/

#include "Deck.c"
#include "Bins.h"
#include "math.h"

#define GUNSHOT 'G'
#define OPENENDED 'O'

void swap(CARD *c, CARD *c2)
{/* Helper for SortHand function */
  CARD temp;
  temp = *c;
  *c = *c2;
  *c2 = temp;
}
void sort_hand(CARD *hand, int hand_size)
{
  /* bubble sort */ 
  /* should probably make this faster...but, N is going to be very limited. */
  int i,j;
  for(i = 0; i < hand_size; i++)
    for(j = i; j < hand_size; j++)
      {
	if(hand[i].rank < hand[j].rank)
	  {
	    swap(&hand[i], &hand[j]);
	  }
      }
}

char gunshot(CARD *hand)
{/* Check if our straights are gunshot or openended; G - Gunshot; O - Openened */

  /* TODO: wtrie this function */
  return 'G';
}

void drawing_to(BIN *bin)
{/* check which hands we are one card away from */
  
}

char player_card_makes_hand(CARD *hand, int hand_len)
{/* Is the players card included in the hand? if not, then the hand is on the board */
  int i;
  for(i = 0; i < hand_len; i++)
    {
      if(hand[i].whos_card == 'P')
	return TRUE;
    }
  return FALSE;
}

int distance(CARD c, CARD c2)
{
  return abs(c.rank - c2.rank);
}

int rank_hand(CARD *hand, BIN *bin, int size_of_hand)
{/* Rank the hand while keeping State in the BINS */
  /* hand size must be at least 2 */
  if(size_of_hand < 2)
    {
      printf("Hand size must be >= 2 to rank\n");
      exit(EXIT_FAILURE);
    }

  sort_hand(hand, size_of_hand);
  int i;

  bin->HC = hand[0]; /* this will always be the case since we are sorting */
  bin->is_full[0] = TRUE;

  for(i = 0; i < size_of_hand-1; i++)
    {
      CARD c = hand[i];
      CARD c2 = hand[i+1];

      /* 
	 Flushes:
	 regardless, we add every card to the flush bin.  it is a multi array where matching suits
	 get added to their own row
      */

      if(bin->is_full[5] == FALSE)
	{
	  if(i % 2 == 0)
	    {/* add on the evens otherwise we would count every card twice */
	      Add(c, &bin->F.b[c.suit], bin->F.b_max, &bin->F.b_count[c.suit]);
	      if(bin->F.b_count[c.suit] < bin->F.b_max)
		{/* we can add second card */
		  Add(c2, &bin->F.b[c2.suit], bin->F.b_max, &bin->F.b_count[c2.suit]);
		}
	      else
		{ /* our bin is full */
		  bin->is_full[5] = TRUE;
		}
	    }
	  else
	    {/* check for the last card */
	      if(i == size_of_hand-2){
		Add(hand[i+1], &bin->F.b[hand[i+1].suit], bin->F.b_max, &bin->F.b_count[hand[i+1].suit]);
	      }
	    }
	  /*
	    Now check if that bin is full....The Add function should probably be marking bins as full rather than
	    me manually doing it
	  */
	  if(bin->F.b_count[c.suit] == 5 || bin->F.b_count[c2.suit] == 5)
	    {
	      bin->is_full[5] = TRUE;
	    }
	}

      int delta = distance(hand[i], hand[i+1]);
      if(delta == 0)
	{/* Cards Match */

	  /* Fill Coresponding Bins */
	  
	  /* Fill the pair bin and mark it as full */
	  if(bin->is_full[1] == FALSE) {
	    Add(c, &bin->P.b, bin->P.b_max, &bin->P.b_count);
	    Add(c2, &bin->P.b, bin->P.b_max, &bin->P.b_count);
	    bin->is_full[1] = TRUE;
	  }
	  
	  /* Two Pair Bin */
	  /* Check to see if the ranks differ...if not....four of a kind would be considered two pair */
	  if(bin->is_full[2] == FALSE)
	    {
	      if(bin->TP.b_count != 0)
		{/* There are already cards in the bin */
		  if(c.rank != bin->TP.b[0].rank)
		    {
		      Add(c, &bin->TP.b, bin->TP.b_max, &bin->TP.b_count);
		      Add(c2, &bin->TP.b, bin->TP.b_max, &bin->TP.b_count);
		      bin->is_full[2] = TRUE;
		    }
		}
	      else
		{/* These are the first cards in the bin */
		  Add(c, &bin->TP.b, bin->TP.b_max, &bin->TP.b_count);
		  Add(c2, &bin->TP.b, bin->TP.b_max, &bin->TP.b_count);
		}
	    }	  

	  /* Three of a Kind bin */
	  /* Because of the nature of the algorithm, on any given iteration, the bin will either be full or empty */
	  if(bin->is_full[3] == FALSE)
	    {
	      if(size_of_hand - i >= 3)
		{/* make sure we have enough space for i+2 */
		  if(c.rank == hand[i+2].rank)
		    {/* We have three of a kind */
		      Add(c, &bin->TK.b, bin->TK.b_max, &bin->TK.b_count);
		      Add(c2, &bin->TK.b, bin->TK.b_max, &bin->TK.b_count);
		      Add(hand[i+2], &bin->TK.b, bin->TK.b_max, &bin->TK.b_count);
		      bin->is_full[3] = TRUE;
		    }
		}
	    }
	  
	  /* Fill Full House */
	  /* You'd think we would be checking this one here...but, actually, if you have a full TP and TK bin */
	  /* you have a full house...we will make this check at the end */
	  

	  /* Four of a Kind Bin */
	  /* Same heuristics as TK bin */
	  if(bin->is_full[7] == FALSE)
	    {
	      if(size_of_hand - i >= 4)
		{/* make sure we have enough space */
		  if(c.rank == hand[i+2].rank && c.rank == hand[i+3].rank)
		    {
		      	  Add(c, &bin->FK.b, bin->FK.b_max, &bin->FK.b_count);
			  Add(c2, &bin->FK.b, bin->FK.b_max, &bin->FK.b_count);
			  Add(hand[i+2], &bin->FK.b, bin->FK.b_max, &bin->FK.b_count);
			  Add(hand[i+3], &bin->FK.b, bin->FK.b_max, &bin->FK.b_count);
			  bin->is_full[7] = TRUE;
		    }
		}
	    }
	  
	}
      if(delta == 1)
	{/* Cards differ by one */

	  /* Fill Coresponding Bins */
	  
	  /* Straight Flush and Straight*/
	  if(bin->is_full[8] == FALSE)
	    {  /* SF special Case */
	      if(c.suit == c2.suit)
		{/* if the suits match add them to the bin */
		  if(bin->SF.b_count[c.suit] <= 0)
		    {/* we do this otherwise we would be adding the same card twice */
		      Add(c, &bin->SF.b[c.suit], bin->SF.b_max, &bin->SF.b_count[c.suit]);
		      Add(c2, &bin->SF.b[c.suit], bin->SF.b_max, &bin->SF.b_count[c.suit]);
		    }
		  else
		    {/* Should only need to add i+1 here (which is c2) */
		      Add(c2, &bin->SF.b[c.suit], bin->SF.b_max, &bin->SF.b_count[c.suit]);
		    }
		  if(bin->SF.b_count[c2.suit] == 5){
		    bin->is_full[8] = TRUE;
		  }
		}
	    }

	  /* Straights */
	  if(bin->is_full[4] == FALSE)
	    {
	      /* suits dont match.  Still good for a straight */
	      /* Straights */
	      if(bin->S.b_count <= 0)
	  	{/* These are the first two cards in the bin */
	  	  Add(c, &bin->S.b, bin->S.b_max, &bin->S.b_count);
	  	  Add(c2, &bin->S.b, bin->S.b_max, &bin->S.b_count);
	  	}
	      else
	  	{/* Just add the second otherwise we will get dups */
	  	  Add(c2, &bin->S.b, bin->S.b_max, &bin->S.b_count);
	  	}
	      if(bin->S.b_count == 5){
	  	bin->is_full[4] = TRUE;
	      }
		
	    }
	}/* END IF delta == 1 */

      if(delta > 1 && delta < 12)
	{/* any potential straight is ruined, clear the straight bin */
	  bin->S.b = clearbin(&bin->S.b, &bin->S.b_count, bin->S.b_max);
	}

    } /*END for(i = 0; i < size_of_hand; i++)

/* Final Checks for made hands */
  /*Case for Full House */
  /* If we have two pair and three of a kind bins full...we have a full house */
  if(bin->is_full[2] == TRUE && bin->is_full[3] == TRUE)
    {
      int t;
      for(t = 0; t < 3; t++)
	{ /* add all the three kind cards */
	  Add(bin->TK.b[t], &bin->FH.b, bin->FH.b_max, &bin->FH.b_count);
	}
      for(t = 0; t < 4; t++)
	{/* check TP bin for the highest rank that doesnt match the TK bin */
	  if(bin->TP.b[t].rank != bin->TK.b[0].rank)
	    {/* This is our pair, since we are sorted the first cards will be the largest */
	      Add(bin->TP.b[t], &bin->FH.b, bin->FH.b_max, &bin->FH.b_count);
	      Add(bin->TP.b[t+1], &bin->FH.b, bin->FH.b_max, &bin->FH.b_count);
	      bin->is_full[6] = TRUE;
	      break;
	    }
	}
    }

  /* Some Vars for Special Cases */
  CARD temp;
  CARD temp2;
  temp = hand[0];
  temp2 = hand[size_of_hand - 1];
  int d = distance(temp, temp2);

  /* Special Case for Straights: Ace in the front 2 in the back */
  if(bin->is_full[4] == FALSE && bin->S.b_count > 0 && d == 12)
    {/* if the bin isn't full AND has something in it and first and last card are Ace and Two */
      if(bin->S.b[0].rank == 3)
	{/* Highest Card in the Straight bin HAS to be 5...3 is 5 btw...rank starts at 0 which is 2 */
	  Add(temp, &bin->S.b, bin->S.b_max, &bin->S.b_count);
	  if(bin->S.b_count == 5)
	    {
	      bin->is_full[4] = TRUE;
	    }
	}
    }

  /* Special Case for Straight Flushes: Ace in the Front 2 in the Back*/
      int index;
      for(index = 0; index < MAX_NUM_SUITS; index++)
	{
	  if(bin->is_full[8] == FALSE && bin->SF.b_count[index] > 0 && d == 12)
	    {/* if the bin isn't full and has something in it and first and last card are Ace and Two */
	      if(temp.suit == temp2.suit)
		{/* suits must match since SF */
		  if(bin->SF.b[index][0].rank == 3)
		    {/* Highest Card in SF bin HAS to be 5 */
		      Add(temp, &bin->SF.b[index], bin->SF.b_max, &bin->SF.b_count[index]);
		      if(bin->SF.b_count[index] == 5)
			{
			  bin->is_full[8] = TRUE;
			}
		    }
		    
		}
	    }
	}
  int final_rank;
  for(final_rank = 8; final_rank >= 0; final_rank--)
    {
      if(bin->is_full[final_rank] == TRUE)
	{
	  return final_rank;
	}
    }
  return -1;

}



